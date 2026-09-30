#include <fstream>
#include "Menu.hpp"
#include <MinHook.h>
#include "../../Features/Currency/SetCurrency.hpp"
#include "../../Features/Player/GodMode.hpp"

#include <d3d11.h>
#include "imgui_impl_dx11.h"

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

namespace Menu
{
  ConfigData Config;

  bool        g_ShowMenu   = true;
  std::string g_LogMessage = "[INSERT] to show/hide menu";
  ULONGLONG   g_LogTimer   = 0;

  void SetLogMessage(const std::string& msg)
  {
    g_LogMessage = msg;
    g_LogTimer   = GetTickCount64();
  }

  typedef HRESULT(__stdcall* Present_t)(IDXGISwapChain* pSwapChain, UINT SyncInterval, UINT Flags);
  Present_t oPresent = nullptr;

  typedef void(__stdcall* ExecuteCommandLists_t)(
    ID3D12CommandQueue* pCommandQueue, UINT NumCommandLists, ID3D12CommandList* const* ppCommandLists
  );
  ExecuteCommandLists_t oExecuteCommandLists     = nullptr;

  ID3D11Device*           g_pd3d11Device         = nullptr;
  ID3D11DeviceContext*    g_pd3d11DeviceContext  = nullptr;
  ID3D11RenderTargetView* g_mainRenderTargetView = nullptr;
  bool                    g_IsDX11               = false;
  bool                    g_IsDX12               = false;

  WNDPROC                    oWndProc;
  HWND                       g_hWnd                 = NULL;
  ID3D12Device*              g_pd3dDevice           = nullptr;
  ID3D12DescriptorHeap*      g_pd3dRtvDescHeap      = nullptr;
  ID3D12DescriptorHeap*      g_pd3dSrvDescHeap      = nullptr;
  ID3D12CommandQueue*        g_pd3dCommandQueue     = nullptr;
  ID3D12GraphicsCommandList* g_pd3dCommandList      = nullptr;
  ID3D12CommandAllocator*    g_pd3dCommandAllocator = nullptr;

  struct FrameContext
  {
    ID3D12CommandAllocator* CommandAllocator;
    UINT64                  FenceValue;
  };

  UINT                         g_FrameIndex                 = 0;
  UINT                         g_NumFrames                  = 0;
  FrameContext*                g_FrameContexts              = nullptr;
  ID3D12Resource**             g_mainRenderTargetResource   = nullptr;
  D3D12_CPU_DESCRIPTOR_HANDLE* g_mainRenderTargetDescriptor = nullptr;

  bool g_ImGuiInitialized                                   = false;

  LRESULT __stdcall WndProc(const HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
  {
    if (uMsg == WM_KEYDOWN && wParam == VK_INSERT) {
      g_ShowMenu = !g_ShowMenu;
      return 1;
    }

    if (g_ShowMenu) {
      if (ImGui_ImplWin32_WndProcHandler(hWnd, uMsg, wParam, lParam))
        return true;
    }
    return CallWindowProc(oWndProc, hWnd, uMsg, wParam, lParam);
  }

  void CleanupRenderTarget()
  {
    if (g_IsDX11) {
      if (g_mainRenderTargetView) {
        g_mainRenderTargetView->Release();
        g_mainRenderTargetView = nullptr;
      }
    }
    else {
      if (g_mainRenderTargetResource) {
        for (UINT i = 0; i < g_NumFrames; i++)
          if (g_mainRenderTargetResource[i]) {
            g_mainRenderTargetResource[i]->Release();
            g_mainRenderTargetResource[i] = nullptr;
          }
        delete[] g_mainRenderTargetResource;
        g_mainRenderTargetResource = nullptr;
      }
      if (g_mainRenderTargetDescriptor) {
        delete[] g_mainRenderTargetDescriptor;
        g_mainRenderTargetDescriptor = nullptr;
      }
      if (g_FrameContexts) {
        for (UINT i = 0; i < g_NumFrames; i++)
          if (g_FrameContexts[i].CommandAllocator) {
            g_FrameContexts[i].CommandAllocator->Release();
            g_FrameContexts[i].CommandAllocator = nullptr;
          }
        delete[] g_FrameContexts;
        g_FrameContexts = nullptr;
      }
    }
  }

  void RenderMenu(IDXGISwapChain* pSwapChain)
  {
    if (!g_ImGuiInitialized) {
      if (SUCCEEDED(pSwapChain->GetDevice(__uuidof(ID3D11Device), (void**) &g_pd3d11Device))) {
        g_IsDX11 = true;
        g_pd3d11Device->GetImmediateContext(&g_pd3d11DeviceContext);

        DXGI_SWAP_CHAIN_DESC sd;
        pSwapChain->GetDesc(&sd);
        g_hWnd   = sd.OutputWindow;
        oWndProc = (WNDPROC) SetWindowLongPtr(g_hWnd, GWLP_WNDPROC, (LONG_PTR) WndProc);

        ImGui::CreateContext();
        ImGui::StyleColorsDark();
        ImGui_ImplWin32_Init(g_hWnd);
        ImGui_ImplDX11_Init(g_pd3d11Device, g_pd3d11DeviceContext);

        ID3D11Texture2D* pBackBuffer = nullptr;
        pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**) &pBackBuffer);
        if (pBackBuffer) {
          g_pd3d11Device->CreateRenderTargetView(pBackBuffer, NULL, &g_mainRenderTargetView);
          pBackBuffer->Release();
        }

        g_ImGuiInitialized = true;
      }
      else if (SUCCEEDED(pSwapChain->GetDevice(IID_PPV_ARGS(&g_pd3dDevice)))) {
        g_IsDX12 = true;
        if (!g_pd3dCommandQueue)
          return;

        DXGI_SWAP_CHAIN_DESC sd;
        if (FAILED(pSwapChain->GetDesc(&sd)))
          return;

        g_NumFrames                  = sd.BufferCount;
        g_mainRenderTargetResource   = new ID3D12Resource*[g_NumFrames]();
        g_mainRenderTargetDescriptor = new D3D12_CPU_DESCRIPTOR_HANDLE[g_NumFrames]();
        g_FrameContexts              = new FrameContext[g_NumFrames]();

        for (UINT i = 0; i < g_NumFrames; i++) {
          pSwapChain->GetBuffer(i, IID_PPV_ARGS(&g_mainRenderTargetResource[i]));
        }

        D3D12_DESCRIPTOR_HEAP_DESC rtvdesc = {};
        rtvdesc.Type                       = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
        rtvdesc.NumDescriptors             = g_NumFrames;
        rtvdesc.Flags                      = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
        rtvdesc.NodeMask                   = 1;
        if (FAILED(g_pd3dDevice->CreateDescriptorHeap(&rtvdesc, IID_PPV_ARGS(&g_pd3dRtvDescHeap))))
          return;

        SIZE_T rtvDescriptorSize = g_pd3dDevice->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
        D3D12_CPU_DESCRIPTOR_HANDLE rtvHandle = g_pd3dRtvDescHeap->GetCPUDescriptorHandleForHeapStart();
        for (UINT i = 0; i < g_NumFrames; i++) {
          g_mainRenderTargetDescriptor[i] = rtvHandle;
          g_pd3dDevice->CreateRenderTargetView(g_mainRenderTargetResource[i], nullptr, rtvHandle);
          rtvHandle.ptr += rtvDescriptorSize;
          g_pd3dDevice->CreateCommandAllocator(
            D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&g_FrameContexts[i].CommandAllocator)
          );
        }

        D3D12_DESCRIPTOR_HEAP_DESC srvdesc = {};
        srvdesc.Type                       = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
        srvdesc.NumDescriptors             = 1;
        srvdesc.Flags                      = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
        if (FAILED(g_pd3dDevice->CreateDescriptorHeap(&srvdesc, IID_PPV_ARGS(&g_pd3dSrvDescHeap))))
          return;

        g_pd3dDevice->CreateCommandList(
          0, D3D12_COMMAND_LIST_TYPE_DIRECT, g_FrameContexts[0].CommandAllocator, nullptr,
          IID_PPV_ARGS(&g_pd3dCommandList)
        );

        g_hWnd   = sd.OutputWindow;
        oWndProc = (WNDPROC) SetWindowLongPtr(g_hWnd, GWLP_WNDPROC, (LONG_PTR) WndProc);

        ImGui::CreateContext();
        ImGuiIO& io = ImGui::GetIO();
        (void) io;
        ImGui::StyleColorsDark();
        ImGui_ImplWin32_Init(g_hWnd);
        ImGui_ImplDX12_InitInfo info      = {};
        info.Device                       = g_pd3dDevice;
        info.NumFramesInFlight            = g_NumFrames;
        info.RTVFormat                    = DXGI_FORMAT_R8G8B8A8_UNORM;
        info.SrvDescriptorHeap            = g_pd3dSrvDescHeap;
        info.CommandQueue                 = g_pd3dCommandQueue;
        info.LegacySingleSrvCpuDescriptor = g_pd3dSrvDescHeap->GetCPUDescriptorHandleForHeapStart();
        info.LegacySingleSrvGpuDescriptor = g_pd3dSrvDescHeap->GetGPUDescriptorHandleForHeapStart();
        ImGui_ImplDX12_Init(&info);

        g_pd3dCommandList->Close();
        g_ImGuiInitialized = true;
      }
    }

    if (g_ImGuiInitialized) {
      if (g_IsDX11) {
        ImGui_ImplDX11_NewFrame();
        ImGui_ImplWin32_NewFrame();
        ImGui::NewFrame();
      }
      else if (g_IsDX12) {
        IDXGISwapChain3* swapChain3 = nullptr;
        if (SUCCEEDED(pSwapChain->QueryInterface(__uuidof(IDXGISwapChain3), (void**) &swapChain3))) {
          g_FrameIndex = swapChain3->GetCurrentBackBufferIndex();
          swapChain3->Release();
        }
        else {
          static UINT fallbackFrameIndex = 0;
          g_FrameIndex                   = fallbackFrameIndex;
          fallbackFrameIndex             = (fallbackFrameIndex + 1) % g_NumFrames;
        }

        g_FrameContexts[g_FrameIndex].CommandAllocator->Reset();
        g_pd3dCommandList->Reset(g_FrameContexts[g_FrameIndex].CommandAllocator, nullptr);

        ImGui_ImplDX12_NewFrame();
        ImGui_ImplWin32_NewFrame();
        ImGui::NewFrame();
      }

      Features::SetCurrency::ProcessRequests();

      if (g_ShowMenu) {
        ImGui::SetNextWindowSizeConstraints(ImVec2(300, 300), ImVec2(500, 400));
        ImGui::Begin(
          "BattleRanker Cheat", &g_ShowMenu,
          ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse
        );

        // Auto-clear log message after 3 seconds
        if (GetTickCount64() - g_LogTimer > 3000) {
          g_LogMessage = "[INSERT] to show/hide menu";
        }

        if (ImGui::BeginTabBar("Tabs")) {
          if (ImGui::BeginTabItem("Player")) {
            ImGui::BeginChild("PlayerContent", ImVec2(0, -60.0f), true);
            ImGui::Spacing();
            ImGui::Text("Base Stats & Godmode");
            if (ImGui::Checkbox("Enable Invincibility", &Config.bGodMode_Invincibility)) {
              SetLogMessage("Invincibility toggled.");
            }
            ImGui::Spacing();
            ImGui::Separator();
            ImGui::Spacing();
            ImGui::Text("Override Base Stats");
            if (ImGui::Checkbox("Max Base Attack", &Config.bGodMode_MaxAtk)) {
              SetLogMessage("Max Base Attack toggled.");
            }
            if (ImGui::Checkbox("Max Base Defense", &Config.bGodMode_MaxDef)) {
              SetLogMessage("Max Base Defense toggled.");
            }
            if (ImGui::Checkbox("Max Base HP", &Config.bGodMode_MaxHP)) {
              SetLogMessage("Max Base HP toggled.");
            }
            ImGui::Spacing();
            ImGui::Separator();
            ImGui::Spacing();

            Features::GodMode::RenderUI();

            ImGui::Spacing();
            ImGui::Separator();
            ImGui::Spacing();
            ImGui::Text("Detailed Stats Upgrades");

            // First column
            ImGui::Columns(2, "PlayerStatsCols", false);
            if (ImGui::Checkbox("Max DNeg", &Config.bMaxDNeg))
              SetLogMessage("Max DNeg toggled.");
            if (ImGui::Checkbox("Max Critical", &Config.bMaxCritical))
              SetLogMessage("Max Critical toggled.");
            if (ImGui::Checkbox("Max Crit ATK", &Config.bMaxCritAtk))
              SetLogMessage("Max Crit ATK toggled.");
            if (ImGui::Checkbox("Max Dodge", &Config.bMaxDodge))
              SetLogMessage("Max Dodge toggled.");
            if (ImGui::Checkbox("Max EXP Bonus", &Config.bMaxEXP))
              SetLogMessage("Max EXP Bonus toggled.");
            if (ImGui::Checkbox("Max Gold Drop Bonus", &Config.bMaxGold))
              SetLogMessage("Max Gold Bonus toggled.");
            if (ImGui::Checkbox("Max Boss DMG Bonus", &Config.bMaxBossDmg))
              SetLogMessage("Max Boss DMG toggled.");
            if (ImGui::Checkbox("Max Skill Cooltime", &Config.bMaxSkillCooltime))
              SetLogMessage("Max Skill Cooltime toggled.");

            ImGui::NextColumn();

            // Second column
            if (ImGui::Checkbox("Max Total ELEM.ATK", &Config.bMaxElemAtk))
              SetLogMessage("Max ELEM.ATK toggled.");
            if (ImGui::Checkbox("Max Tree ELEM.ATK", &Config.bMaxTreeElemAtk))
              SetLogMessage("Max Tree ELEM toggled.");
            if (ImGui::Checkbox("Max Water ELEM.ATK", &Config.bMaxWaterElemAtk))
              SetLogMessage("Max Water ELEM toggled.");
            if (ImGui::Checkbox("Max Fire ELEM.ATK", &Config.bMaxFireElemAtk))
              SetLogMessage("Max Fire ELEM toggled.");
            if (ImGui::Checkbox("Max Light ELEM.ATK", &Config.bMaxLightElemAtk))
              SetLogMessage("Max Light ELEM toggled.");

            ImGui::Columns(1);
            ImGui::Spacing();
            ImGui::EndChild();
            ImGui::EndTabItem();
          }
          if (ImGui::BeginTabItem("Currency")) {
            ImGui::BeginChild("CurrencyContent", ImVec2(0, -60.0f), true);
            ImGui::Spacing();
            if (ImGui::Checkbox("Infinite Currency", &Config.bInfiniteCurrency)) {
              SetLogMessage("Infinite Currency toggled.");
            }
            ImGui::Spacing();
            ImGui::Separator();
            ImGui::Spacing();
            ImGui::Text("Override Currency Amount");

            auto renderCurrencyRow =
              [](const char* label, const char* id, const char* message, bool& request, int& value) {
                ImGui::TextUnformatted(label);
                ImGui::SameLine(150);
                if (ImGui::Button((std::string("Set##") + id).c_str())) {
                  request = true;
                  SetLogMessage(message);
                }
                ImGui::SameLine();
                ImGui::InputInt((std::string("##") + id).c_str(), &value, 0, 0);
              };

            renderCurrencyRow("Gold", "Gold", "Set Gold requested.", Config.bRequestSetGold, Config.iSetGoldValue);
            renderCurrencyRow(
              "Cryptobit", "Cryptobit", "Set Cryptobit requested.", Config.bRequestSetCryptobit,
              Config.iSetCryptobitValue
            );
            renderCurrencyRow(
              "Diamond", "Diamond", "Set Diamond requested.", Config.bRequestSetDiamond, Config.iSetDiamondValue
            );
            renderCurrencyRow(
              "Skill Ticket", "SkillTicket", "Set Skill Ticket requested.", Config.bRequestSetSkillTicket,
              Config.iSetSkillTicketValue
            );
            renderCurrencyRow(
              "Chance Ticket", "ChanceTicket", "Set Chance Ticket requested.", Config.bRequestSetChanceTicket,
              Config.iSetChanceTicketValue
            );
            renderCurrencyRow(
              "Pickup Ticket", "PickupTicket", "Set Pickup Ticket requested.", Config.bRequestSetPickupTicket,
              Config.iSetPickupTicketValue
            );
            renderCurrencyRow(
              "Mileage", "Mileage", "Set Mileage requested.", Config.bRequestSetMileage, Config.iSetMileageValue
            );
            renderCurrencyRow(
              "Golden Dice", "GoldenDice", "Set Golden Dice requested.", Config.bRequestSetGoldenDice,
              Config.iSetGoldenDiceValue
            );
            renderCurrencyRow(
              "Lucky Dice", "LuckyDice", "Set Lucky Dice requested.", Config.bRequestSetLuckyDice,
              Config.iSetOptionDiceValue
            );
            renderCurrencyRow(
              "Skill Dice", "SkillDice", "Set Skill Dice requested.", Config.bRequestSetSkillDice,
              Config.iSetSkillDiceValue
            );
            renderCurrencyRow(
              "Quantum Orb", "QuantumOrb", "Set Quantum Orb requested.", Config.bRequestSetQuantumOrb,
              Config.iSetQuantumOrbValue
            );
            renderCurrencyRow(
              "Candela", "Candela", "Set Candela requested.", Config.bRequestSetCandela, Config.iSetCandelaValue
            );
            renderCurrencyRow("CP", "CP", "Set CP requested.", Config.bRequestSetCP, Config.iSetCPValue);
            renderCurrencyRow(
              "Tree Core", "TreeCore", "Set Tree Core requested.", Config.bRequestSetTreeCore, Config.iSetTreeCoreValue
            );
            renderCurrencyRow(
              "Fire Core", "FireCore", "Set Fire Core requested.", Config.bRequestSetFireCore, Config.iSetFireCoreValue
            );
            renderCurrencyRow(
              "Water Core", "WaterCore", "Set Water Core requested.", Config.bRequestSetWaterCore,
              Config.iSetWaterCoreValue
            );
            renderCurrencyRow(
              "Light Core", "LightCore", "Set Light Core requested.", Config.bRequestSetLightCore,
              Config.iSetLightCoreValue
            );
            renderCurrencyRow(
              "Arena Ticket", "ArenaTicket", "Set Arena Ticket requested.", Config.bRequestSetArenaTicket,
              Config.iSetArenaTicketValue
            );
            renderCurrencyRow(
              "Quantum Ring", "QuantumRing", "Set Quantum Ring requested.", Config.bRequestSetQuantumRing,
              Config.iSetQuantumRingValue
            );
            renderCurrencyRow(
              "Poly Fiber", "PolyFiber", "Set Poly Fiber requested.", Config.bRequestSetPolyFiber,
              Config.iSetPolyFiberValue
            );
            renderCurrencyRow(
              "All Dungeon Keys", "AllKeys", "Set All Dungeon Keys.", Config.bRequestSetKeys, Config.iSetKeysValue
            );

            ImGui::Spacing();
            ImGui::EndChild();
            ImGui::EndTabItem();
          }
          if (ImGui::BeginTabItem("Misc")) {
            ImGui::BeginChild("MiscContent", ImVec2(0, -60.0f), true);
            ImGui::Spacing();
            ImGui::Text("Summon & Options");
            if (ImGui::Checkbox("Max Gacha Rolls", &Config.bMaxGachaRolls)) {
              SetLogMessage("Max Gacha Rolls toggled.");
            }
            if (ImGui::Checkbox("Max Option Roll (Always Max)", &Config.bMaxOptionRoll)) {
              SetLogMessage("Max Option Roll toggled.");
            }
            ImGui::SameLine();
            ImGui::InputInt("##OptionRollTier", &Config.iMaxOptionRollTier);
            if (Config.iMaxOptionRollTier < 0)
              Config.iMaxOptionRollTier = 0;
            ImGui::Spacing();
            ImGui::Separator();
            ImGui::Spacing();
            if (ImGui::Checkbox("Aura Kill", &Config.bAuraKill)) {
              SetLogMessage("Aura Kill toggled.");
            }
            ImGui::Spacing();
            ImGui::EndChild();
            ImGui::EndTabItem();
          }
          ImGui::EndTabBar();
        }

        ImGui::Separator();
        ImGui::Spacing();
        if (ImGui::Button("SAVE CONFIG", ImVec2(-1, 20))) {
          SetLogMessage("Saved config successfully!");
        }

        ImGui::Spacing();
        if (g_LogMessage == "[INSERT] to show/hide menu") {
          ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "%s", g_LogMessage.c_str());
        }
        else {
          ImGui::TextColored(ImVec4(0.2f, 0.8f, 0.2f, 1.0f), "%s", g_LogMessage.c_str());
        }

        ImGui::End();
      }

      ImGui::Render();
      if (g_IsDX11) {
        g_pd3d11DeviceContext->OMSetRenderTargets(1, &g_mainRenderTargetView, NULL);
        ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
      }
      else if (g_IsDX12) {
        D3D12_RESOURCE_BARRIER barrier = {};
        barrier.Type                   = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
        barrier.Flags                  = D3D12_RESOURCE_BARRIER_FLAG_NONE;
        barrier.Transition.pResource   = g_mainRenderTargetResource[g_FrameIndex];
        barrier.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;
        barrier.Transition.StateBefore = D3D12_RESOURCE_STATE_PRESENT;
        barrier.Transition.StateAfter  = D3D12_RESOURCE_STATE_RENDER_TARGET;
        g_pd3dCommandList->ResourceBarrier(1, &barrier);

        g_pd3dCommandList->OMSetRenderTargets(1, &g_mainRenderTargetDescriptor[g_FrameIndex], FALSE, nullptr);

        ID3D12DescriptorHeap* descriptorHeaps[] = {g_pd3dSrvDescHeap};
        g_pd3dCommandList->SetDescriptorHeaps(1, descriptorHeaps);

        ImGui_ImplDX12_RenderDrawData(ImGui::GetDrawData(), g_pd3dCommandList);

        barrier.Transition.StateBefore = D3D12_RESOURCE_STATE_RENDER_TARGET;
        barrier.Transition.StateAfter  = D3D12_RESOURCE_STATE_PRESENT;
        g_pd3dCommandList->ResourceBarrier(1, &barrier);
        g_pd3dCommandList->Close();

        if (g_pd3dCommandQueue) {
          g_pd3dCommandQueue->ExecuteCommandLists(1, (ID3D12CommandList* const*) &g_pd3dCommandList);
        }
      }
    }
  }

  HRESULT __stdcall hkPresent(IDXGISwapChain* pSwapChain, UINT SyncInterval, UINT Flags)
  {
    static bool firstCallPresent = true;
    if (firstCallPresent) {
      {
        std::ofstream logfile("BattleRanker_Log.txt", std::ios::app);
        if (logfile.is_open())
          logfile << "hkPresent (Index 8) called for the first time!" << std::endl;
      }
      firstCallPresent = false;
    }
    RenderMenu(pSwapChain);
    return oPresent(pSwapChain, SyncInterval, Flags);
  }

  typedef HRESULT(__stdcall* Present1_t)(
    IDXGISwapChain1* pSwapChain, UINT SyncInterval, UINT PresentFlags, const void* pPresentParameters
  );
  Present1_t oPresent1 = nullptr;

  HRESULT __stdcall
  hkPresent1(IDXGISwapChain1* pSwapChain, UINT SyncInterval, UINT PresentFlags, const void* pPresentParameters)
  {
    static bool firstCallPresent1 = true;
    if (firstCallPresent1) {
      {
        std::ofstream logfile("BattleRanker_Log.txt", std::ios::app);
        if (logfile.is_open())
          logfile << "hkPresent1 (Index 22) called for the first time!" << std::endl;
      }
      firstCallPresent1 = false;
    }
    RenderMenu(pSwapChain);
    {
      static bool firstCallEnd = true;
      if (firstCallEnd) {
        std::ofstream logfile("BattleRanker_Log.txt", std::ios::app);
        if (logfile.is_open())
          logfile << "hkPresent1: RenderMenu finished, calling oPresent1" << std::endl;
        firstCallEnd = false;
      }
    }
    return oPresent1(pSwapChain, SyncInterval, PresentFlags, pPresentParameters);
  }

  void __stdcall hkExecuteCommandLists(
    ID3D12CommandQueue* pCommandQueue, UINT NumCommandLists, ID3D12CommandList* const* ppCommandLists
  )
  {
    static bool firstCallCmd = true;
    if (firstCallCmd) {
      {
        std::ofstream logfile("BattleRanker_Log.txt", std::ios::app);
        if (logfile.is_open())
          logfile << "hkExecuteCommandLists called for the first time!" << std::endl;
      }
      firstCallCmd = false;
    }

    if (!g_pd3dCommandQueue) {
      g_pd3dCommandQueue = pCommandQueue;
    }
    return oExecuteCommandLists(pCommandQueue, NumCommandLists, ppCommandLists);
  }

  bool init()
  {
    WNDCLASSEX windowClass;
    windowClass.cbSize        = sizeof(WNDCLASSEX);
    windowClass.style         = CS_HREDRAW | CS_VREDRAW;
    windowClass.lpfnWndProc   = DefWindowProc;
    windowClass.cbClsExtra    = 0;
    windowClass.cbWndExtra    = 0;
    windowClass.hInstance     = GetModuleHandle(NULL);
    windowClass.hIcon         = NULL;
    windowClass.hCursor       = NULL;
    windowClass.hbrBackground = NULL;
    windowClass.lpszMenuName  = NULL;
    windowClass.lpszClassName = "DummyClassDX";
    windowClass.hIconSm       = NULL;

    ::RegisterClassEx(&windowClass);
    HWND window = ::CreateWindow(
      windowClass.lpszClassName, "DummyWindow", WS_OVERLAPPEDWINDOW, 0, 0, 100, 100, NULL, NULL, windowClass.hInstance,
      NULL
    );

    HMODULE libD3D11 = GetModuleHandle("d3d11.dll");
    if (!libD3D11)
      libD3D11 = LoadLibrary("d3d11.dll");

    if (!libD3D11) {
      ::DestroyWindow(window);
      ::UnregisterClass(windowClass.lpszClassName, windowClass.hInstance);
      return false;
    }

    auto D3D11CreateDeviceAndSwapChain_Func =
      (decltype(&D3D11CreateDeviceAndSwapChain)) GetProcAddress(libD3D11, "D3D11CreateDeviceAndSwapChain");
    if (!D3D11CreateDeviceAndSwapChain_Func) {
      return false;
    }

    D3D_FEATURE_LEVEL       featureLevel;
    const D3D_FEATURE_LEVEL featureLevels[] = {D3D_FEATURE_LEVEL_11_0, D3D_FEATURE_LEVEL_10_1, D3D_FEATURE_LEVEL_10_0};

    DXGI_SWAP_CHAIN_DESC swapChainDesc;
    ZeroMemory(&swapChainDesc, sizeof(swapChainDesc));
    swapChainDesc.BufferCount                 = 1;
    swapChainDesc.BufferDesc.Format           = DXGI_FORMAT_R8G8B8A8_UNORM;
    swapChainDesc.BufferUsage                 = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    swapChainDesc.OutputWindow                = window;
    swapChainDesc.SampleDesc.Count            = 1;
    swapChainDesc.Windowed                    = TRUE;
    swapChainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
    swapChainDesc.BufferDesc.Scaling          = DXGI_MODE_SCALING_UNSPECIFIED;
    swapChainDesc.SwapEffect                  = DXGI_SWAP_EFFECT_DISCARD;

    ID3D11Device*        dummyDevice          = nullptr;
    ID3D11DeviceContext* dummyContext         = nullptr;
    IDXGISwapChain*      dummySwapChain       = nullptr;

    if (
      FAILED(D3D11CreateDeviceAndSwapChain_Func(
        nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, 0, featureLevels, 3, D3D11_SDK_VERSION, &swapChainDesc,
        &dummySwapChain, &dummyDevice, &featureLevel, &dummyContext
      ))
    ) {
      ::DestroyWindow(window);
      ::UnregisterClass(windowClass.lpszClassName, windowClass.hInstance);
      return false;
    }

    void** pVTable = *reinterpret_cast<void***>(dummySwapChain);
    MH_CreateHook(pVTable[8], (void*) &hkPresent, (void**) &oPresent);
    MH_EnableHook(pVTable[8]);
    MH_CreateHook(pVTable[22], (void*) &hkPresent1, (void**) &oPresent1);
    MH_EnableHook(pVTable[22]);

    // DX12 Dummy creation to hook ExecuteCommandLists (Index 54 in ID3D12CommandQueue)
    HMODULE libDXGI  = GetModuleHandle("dxgi.dll");
    HMODULE libD3D12 = GetModuleHandle("d3d12.dll");
    if (libDXGI && libD3D12) {
      auto CreateDXGIFactory1_Func = (decltype(&CreateDXGIFactory1)) GetProcAddress(libDXGI, "CreateDXGIFactory1");
      auto D3D12CreateDevice_Func  = (decltype(&::D3D12CreateDevice)) GetProcAddress(libD3D12, "D3D12CreateDevice");

      if (CreateDXGIFactory1_Func && D3D12CreateDevice_Func) {
        IDXGIFactory4* factory;
        if (SUCCEEDED(CreateDXGIFactory1_Func(__uuidof(IDXGIFactory4), (void**) &factory))) {
          IDXGIAdapter* adapter;
          if (SUCCEEDED(factory->EnumAdapters(0, &adapter))) {
            ID3D12Device* device;
            if (
              SUCCEEDED(
                D3D12CreateDevice_Func(adapter, D3D_FEATURE_LEVEL_11_0, __uuidof(ID3D12Device), (void**) &device)
              )
            ) {
              D3D12_COMMAND_QUEUE_DESC queueDesc;
              queueDesc.Type     = D3D12_COMMAND_LIST_TYPE_DIRECT;
              queueDesc.Priority = 0;
              queueDesc.Flags    = D3D12_COMMAND_QUEUE_FLAG_NONE;
              queueDesc.NodeMask = 0;

              ID3D12CommandQueue* commandQueue;
              if (
                SUCCEEDED(device->CreateCommandQueue(&queueDesc, __uuidof(ID3D12CommandQueue), (void**) &commandQueue))
              ) {
                void** commandQueueVTable = *reinterpret_cast<void***>(commandQueue);
                MH_CreateHook(commandQueueVTable[10], (void*) &hkExecuteCommandLists, (void**) &oExecuteCommandLists);
                MH_EnableHook(commandQueueVTable[10]);
                commandQueue->Release();
              }
              device->Release();
            }
            adapter->Release();
          }
          factory->Release();
        }
      }
    }

    dummyDevice->Release();
    dummyContext->Release();
    dummySwapChain->Release();
    ::DestroyWindow(window);
    ::UnregisterClass(windowClass.lpszClassName, windowClass.hInstance);

    Beep(750, 300);
    return true;
  }

  void shutdown()
  {
    if (g_ImGuiInitialized) {
      if (g_IsDX11) {
        ImGui_ImplDX11_Shutdown();
      }
      else if (g_IsDX12) {
        ImGui_ImplDX12_Shutdown();
      }
      ImGui_ImplWin32_Shutdown();
      ImGui::DestroyContext();
    }

    CleanupRenderTarget();

    if (g_pd3dRtvDescHeap) {
      g_pd3dRtvDescHeap->Release();
      g_pd3dRtvDescHeap = nullptr;
    }
    if (g_pd3dSrvDescHeap) {
      g_pd3dSrvDescHeap->Release();
      g_pd3dSrvDescHeap = nullptr;
    }
    if (g_pd3dCommandList) {
      g_pd3dCommandList->Release();
      g_pd3dCommandList = nullptr;
    }

    if (oWndProc) {
      SetWindowLongPtr(g_hWnd, GWLP_WNDPROC, (LONG_PTR) oWndProc);
    }
  }
}  // namespace Menu
