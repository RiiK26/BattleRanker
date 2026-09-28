#include <fstream>
#include "Menu.hpp"
#include <MinHook.h>
#include "../../Features/Currency/SetCurrency.hpp"
#include "../../Features/Player/DetailedStats.hpp"

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
  ExecuteCommandLists_t oExecuteCommandLists = nullptr;

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

  void RenderMenu(IDXGISwapChain* pSwapChain)
  {
    if (!g_pd3dCommandQueue)
      return;

    if (!g_ImGuiInitialized) {
      {
        std::ofstream logfile("BattleRanker_Cheat_Log.txt", std::ios::app);
        if (logfile.is_open())
          logfile << "RenderMenu: Init starting" << std::endl;
      }
      if (SUCCEEDED(pSwapChain->GetDevice(IID_PPV_ARGS(&g_pd3dDevice)))) {
        {
          std::ofstream logfile("BattleRanker_Cheat_Log.txt", std::ios::app);
          if (logfile.is_open())
            logfile << "RenderMenu: GetDevice succeeded" << std::endl;
        }

        DXGI_SWAP_CHAIN_DESC sd;
        if (FAILED(pSwapChain->GetDesc(&sd))) {
          {
            std::ofstream logfile("BattleRanker_Cheat_Log.txt", std::ios::app);
            if (logfile.is_open())
              logfile << "RenderMenu: GetDesc failed" << std::endl;
          }
          return;
        }
        {
          std::ofstream logfile("BattleRanker_Cheat_Log.txt", std::ios::app);
          if (logfile.is_open())
            logfile << "RenderMenu: GetDesc succeeded" << std::endl;
        }

        g_NumFrames                  = sd.BufferCount;
        g_mainRenderTargetResource   = new ID3D12Resource*[g_NumFrames]();
        g_mainRenderTargetDescriptor = new D3D12_CPU_DESCRIPTOR_HANDLE[g_NumFrames]();
        g_FrameContexts              = new FrameContext[g_NumFrames]();

        for (UINT i = 0; i < g_NumFrames; i++) {
          pSwapChain->GetBuffer(i, IID_PPV_ARGS(&g_mainRenderTargetResource[i]));
        }
        {
          std::ofstream logfile("BattleRanker_Cheat_Log.txt", std::ios::app);
          if (logfile.is_open())
            logfile << "RenderMenu: GetBuffer loop finished" << std::endl;
        }

        D3D12_DESCRIPTOR_HEAP_DESC rtvdesc = {};
        rtvdesc.Type                       = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
        rtvdesc.NumDescriptors             = g_NumFrames;
        rtvdesc.Flags                      = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
        rtvdesc.NodeMask                   = 1;
        if (FAILED(g_pd3dDevice->CreateDescriptorHeap(&rtvdesc, IID_PPV_ARGS(&g_pd3dRtvDescHeap)))) {
          {
            std::ofstream logfile("BattleRanker_Cheat_Log.txt", std::ios::app);
            if (logfile.is_open())
              logfile << "RenderMenu: CreateDescriptorHeap (RTV) failed" << std::endl;
          }
          return;
        }

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
        {
          std::ofstream logfile("BattleRanker_Cheat_Log.txt", std::ios::app);
          if (logfile.is_open())
            logfile << "RenderMenu: CommandAllocators created" << std::endl;
        }

        D3D12_DESCRIPTOR_HEAP_DESC srvdesc = {};
        srvdesc.Type                       = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
        srvdesc.NumDescriptors             = 1;
        srvdesc.Flags                      = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
        if (FAILED(g_pd3dDevice->CreateDescriptorHeap(&srvdesc, IID_PPV_ARGS(&g_pd3dSrvDescHeap)))) {
          {
            std::ofstream logfile("BattleRanker_Cheat_Log.txt", std::ios::app);
            if (logfile.is_open())
              logfile << "RenderMenu: CreateDescriptorHeap (SRV) failed" << std::endl;
          }
          return;
        }

        g_pd3dDevice->CreateCommandList(
          0, D3D12_COMMAND_LIST_TYPE_DIRECT, g_FrameContexts[0].CommandAllocator, nullptr,
          IID_PPV_ARGS(&g_pd3dCommandList)
        );
        {
          std::ofstream logfile("BattleRanker_Cheat_Log.txt", std::ios::app);
          if (logfile.is_open())
            logfile << "RenderMenu: CreateCommandList succeeded" << std::endl;
        }

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
        {
          std::ofstream logfile("BattleRanker_Cheat_Log.txt", std::ios::app);
          if (logfile.is_open())
            logfile << "RenderMenu: ImGui initialized" << std::endl;
        }
        g_ImGuiInitialized = true;
      }
    }

    if (g_ImGuiInitialized) {
      IDXGISwapChain3* swapChain3 = nullptr;
      if (SUCCEEDED(pSwapChain->QueryInterface(__uuidof(IDXGISwapChain3), (void**) &swapChain3))) {
        g_FrameIndex = swapChain3->GetCurrentBackBufferIndex();
        swapChain3->Release();
      }
      else {
        // Fallback for older swapchains
        static UINT fallbackFrameIndex = 0;
        g_FrameIndex                   = fallbackFrameIndex;
        fallbackFrameIndex             = (fallbackFrameIndex + 1) % g_NumFrames;
      }

      g_FrameContexts[g_FrameIndex].CommandAllocator->Reset();
      g_pd3dCommandList->Reset(g_FrameContexts[g_FrameIndex].CommandAllocator, nullptr);

      ImGui_ImplDX12_NewFrame();
      ImGui_ImplWin32_NewFrame();
      ImGui::NewFrame();
      {
        std::ofstream logfile("BattleRanker_Cheat_Log.txt", std::ios::app);
        if (logfile.is_open())
          logfile << "RenderMenu: Drawing - NewFrame" << std::endl;
      }

      Features::SetCurrency::ProcessRequests();

      if (g_ShowMenu) {
        ImGui::SetNextWindowSizeConstraints(ImVec2(500, 400), ImVec2(800, 600));
        ImGui::Begin("BattleRanker Cheat", &g_ShowMenu, ImGuiWindowFlags_NoCollapse);

        // Auto-clear log message after 3 seconds
        if (GetTickCount64() - g_LogTimer > 3000) {
          g_LogMessage = "[INSERT] to show/hide menu";
        }

        if (ImGui::BeginTabBar("Tabs")) {
          if (ImGui::BeginTabItem("Detailed Stats Editor")) {
            ImGui::Spacing();
            Features::DetailedStats::RenderUI();
            ImGui::Spacing();
            ImGui::EndTabItem();
          }
          if (ImGui::BeginTabItem("Player")) {
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
            ImGui::EndTabItem();
          }
          if (ImGui::BeginTabItem("Currency")) {
            ImGui::Spacing();
            if (ImGui::Checkbox("Infinite Currency (Never Subtract)", &Config.bInfiniteCurrency)) {
              SetLogMessage("Infinite Currency toggled.");
            }
            ImGui::Spacing();
            ImGui::Separator();
            ImGui::Spacing();
            ImGui::Text("Override Currency Amount");

            // Gold
            if (ImGui::Button("Set##Gold")) {
              Config.bRequestSetGold = true;
              SetLogMessage("Set Gold requested.");
            }
            ImGui::SameLine(150);
            ImGui::InputInt("##SetGold", &Config.iSetGoldValue);

            // Cryptobit (was Ruby)
            if (ImGui::Button("Set##Cryptobit")) {
              Config.bRequestSetCryptobit = true;
              SetLogMessage("Set Cryptobit requested.");
            }
            ImGui::SameLine(150);
            ImGui::InputInt("##SetCryptobit", &Config.iSetCryptobitValue);

            // Diamond (was Jewel)
            if (ImGui::Button("Set##Diamond")) {
              Config.bRequestSetDiamond = true;
              SetLogMessage("Set Diamond requested.");
            }
            ImGui::SameLine(150);
            ImGui::InputInt("##SetDiamond", &Config.iSetDiamondValue);

            // Skill Ticket
            if (ImGui::Button("Set##SkillTicket")) {
              Config.bRequestSetSkillTicket = true;
              SetLogMessage("Set Skill Ticket requested.");
            }
            ImGui::SameLine(150);
            ImGui::InputInt("##SetSkillTicket", &Config.iSetSkillTicketValue);

            // Chance Ticket
            if (ImGui::Button("Set##ChanceTicket")) {
              Config.bRequestSetChanceTicket = true;
              SetLogMessage("Set Chance Ticket requested.");
            }
            ImGui::SameLine(150);
            ImGui::InputInt("##SetChanceTicket", &Config.iSetChanceTicketValue);

            // Pickup Ticket
            if (ImGui::Button("Set##PickupTicket")) {
              Config.bRequestSetPickupTicket = true;
              SetLogMessage("Set Pickup Ticket requested.");
            }
            ImGui::SameLine(150);
            ImGui::InputInt("##SetPickupTicket", &Config.iSetPickupTicketValue);

            // Mileage
            if (ImGui::Button("Set##Mileage")) {
              Config.bRequestSetMileage = true;
              SetLogMessage("Set Mileage requested.");
            }
            ImGui::SameLine(150);
            ImGui::InputInt("##SetMileage", &Config.iSetMileageValue);

            // Golden Dice
            if (ImGui::Button("Set##GoldenDice")) {
              Config.bRequestSetGoldenDice = true;
              SetLogMessage("Set Golden Dice requested.");
            }
            ImGui::SameLine(150);
            ImGui::InputInt("##SetGoldenDice", &Config.iSetGoldenDiceValue);

            // Lucky Dice (Internal: Option Dice)
            if (ImGui::Button("Set##LuckyDice")) {
              Config.bRequestSetLuckyDice = true;
              SetLogMessage("Set Lucky Dice requested.");
            }
            ImGui::SameLine(150);
            ImGui::InputInt("##SetOptionDice", &Config.iSetOptionDiceValue);

            // Skill Dice
            if (ImGui::Button("Set##SkillDice")) {
              Config.bRequestSetSkillDice = true;
              SetLogMessage("Set Skill Dice requested.");
            }
            ImGui::SameLine(150);
            ImGui::InputInt("##SetSkillDice", &Config.iSetSkillDiceValue);

            // Quantum Orb
            if (ImGui::Button("Set##QuantumOrb")) {
              Config.bRequestSetQuantumOrb = true;
              SetLogMessage("Set Quantum Orb requested.");
            }
            ImGui::SameLine(150);
            ImGui::InputInt("##SetQuantumOrb", &Config.iSetQuantumOrbValue);

            // Candela
            if (ImGui::Button("Set##Candela")) {
              Config.bRequestSetCandela = true;
              SetLogMessage("Set Candela requested.");
            }
            ImGui::SameLine(150);
            ImGui::InputInt("##SetCandela", &Config.iSetCandelaValue);

            // CP
            if (ImGui::Button("Set##CP")) {
              Config.bRequestSetCP = true;
              SetLogMessage("Set CP requested.");
            }
            ImGui::SameLine(150);
            ImGui::InputInt("##SetCP", &Config.iSetCPValue);

            // Cores
            if (ImGui::Button("Set##TreeCore")) {
              Config.bRequestSetTreeCore = true;
              SetLogMessage("Set Tree Core requested.");
            }
            ImGui::SameLine(150);
            ImGui::InputInt("##SetTreeCore", &Config.iSetTreeCoreValue);

            if (ImGui::Button("Set##FireCore")) {
              Config.bRequestSetFireCore = true;
              SetLogMessage("Set Fire Core requested.");
            }
            ImGui::SameLine(150);
            ImGui::InputInt("##SetFireCore", &Config.iSetFireCoreValue);

            if (ImGui::Button("Set##WaterCore")) {
              Config.bRequestSetWaterCore = true;
              SetLogMessage("Set Water Core requested.");
            }
            ImGui::SameLine(150);
            ImGui::InputInt("##SetWaterCore", &Config.iSetWaterCoreValue);

            if (ImGui::Button("Set##LightCore")) {
              Config.bRequestSetLightCore = true;
              SetLogMessage("Set Light Core requested.");
            }
            ImGui::SameLine(150);
            ImGui::InputInt("##SetLightCore", &Config.iSetLightCoreValue);

            // Dungeon Keys
            if (ImGui::Button("Set##AllKeys")) {
              Config.bRequestSetKeys = true;
              SetLogMessage("Set All Dungeon Keys requested.");
            }
            ImGui::SameLine(150);
            ImGui::InputInt("##SetKeys", &Config.iSetKeysValue);

            ImGui::Spacing();
            ImGui::EndTabItem();
          }
          if (ImGui::BeginTabItem("Combat")) {
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
            ImGui::EndTabItem();
          }
          ImGui::EndTabBar();
        }

        ImGui::Spacing();
        ImGui::Separator();
        ImGui::Spacing();

        if (ImGui::Button("SAVE CONFIG", ImVec2(-1, 35))) {
          // simulate save config
          SetLogMessage("Saved config successfully!");
        }

        ImGui::Spacing();
        ImGui::Separator();
        ImGui::Spacing();

        // Footer for info
        if (g_LogMessage == "[INSERT] to show/hide menu") {
          ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "%s", g_LogMessage.c_str());
        }
        else {
          ImGui::TextColored(ImVec4(0.2f, 0.8f, 0.2f, 1.0f), "%s", g_LogMessage.c_str());
        }

        ImGui::End();
      }

      ImGui::Render();
      {
        std::ofstream logfile("BattleRanker_Cheat_Log.txt", std::ios::app);
        if (logfile.is_open())
          logfile << "RenderMenu: Drawing - Rendered" << std::endl;
      }

      D3D12_RESOURCE_BARRIER barrier = {};
      barrier.Type                   = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
      barrier.Flags                  = D3D12_RESOURCE_BARRIER_FLAG_NONE;
      barrier.Transition.pResource   = g_mainRenderTargetResource[g_FrameIndex];
      barrier.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;
      barrier.Transition.StateBefore = D3D12_RESOURCE_STATE_PRESENT;
      barrier.Transition.StateAfter  = D3D12_RESOURCE_STATE_RENDER_TARGET;
      {
        std::ofstream logfile("BattleRanker_Cheat_Log.txt", std::ios::app);
        if (logfile.is_open())
          logfile << "RenderMenu: Drawing - Before ResourceBarrier" << std::endl;
      }
      g_pd3dCommandList->ResourceBarrier(1, &barrier);

      {
        std::ofstream logfile("BattleRanker_Cheat_Log.txt", std::ios::app);
        if (logfile.is_open())
          logfile << "RenderMenu: Drawing - Before OMSetRenderTargets" << std::endl;
      }
      g_pd3dCommandList->OMSetRenderTargets(1, &g_mainRenderTargetDescriptor[g_FrameIndex], FALSE, nullptr);

      {
        std::ofstream logfile("BattleRanker_Cheat_Log.txt", std::ios::app);
        if (logfile.is_open())
          logfile << "RenderMenu: Drawing - Before SetDescriptorHeaps" << std::endl;
      }
      ID3D12DescriptorHeap* descriptorHeaps[] = {g_pd3dSrvDescHeap};
      g_pd3dCommandList->SetDescriptorHeaps(1, descriptorHeaps);

      {
        std::ofstream logfile("BattleRanker_Cheat_Log.txt", std::ios::app);
        if (logfile.is_open())
          logfile << "RenderMenu: Drawing - Before RenderDrawData" << std::endl;
      }
      ImGui_ImplDX12_RenderDrawData(ImGui::GetDrawData(), g_pd3dCommandList);
      {
        std::ofstream logfile("BattleRanker_Cheat_Log.txt", std::ios::app);
        if (logfile.is_open())
          logfile << "RenderMenu: Drawing - DrawData submitted" << std::endl;
      }

      barrier.Transition.StateBefore = D3D12_RESOURCE_STATE_RENDER_TARGET;
      barrier.Transition.StateAfter  = D3D12_RESOURCE_STATE_PRESENT;
      g_pd3dCommandList->ResourceBarrier(1, &barrier);
      g_pd3dCommandList->Close();
      {
        std::ofstream logfile("BattleRanker_Cheat_Log.txt", std::ios::app);
        if (logfile.is_open())
          logfile << "RenderMenu: Drawing - CommandList Closed" << std::endl;
      }

      if (g_pd3dCommandQueue) {
        {
          std::ofstream logfile("BattleRanker_Cheat_Log.txt", std::ios::app);
          if (logfile.is_open())
            logfile << "RenderMenu: Drawing - Executing command lists" << std::endl;
        }
        g_pd3dCommandQueue->ExecuteCommandLists(1, (ID3D12CommandList* const*) &g_pd3dCommandList);
      }
    }
  }

  HRESULT __stdcall hkPresent(IDXGISwapChain* pSwapChain, UINT SyncInterval, UINT Flags)
  {
    static bool firstCallPresent = true;
    if (firstCallPresent) {
      {
        std::ofstream logfile("BattleRanker_Cheat_Log.txt", std::ios::app);
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
        std::ofstream logfile("BattleRanker_Cheat_Log.txt", std::ios::app);
        if (logfile.is_open())
          logfile << "hkPresent1 (Index 22) called for the first time!" << std::endl;
      }
      firstCallPresent1 = false;
    }
    RenderMenu(pSwapChain);
    {
      static bool firstCallEnd = true;
      if (firstCallEnd) {
        std::ofstream logfile("BattleRanker_Cheat_Log.txt", std::ios::app);
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
        std::ofstream logfile("BattleRanker_Cheat_Log.txt", std::ios::app);
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
    windowClass.lpszClassName = "DummyClassDX12";
    windowClass.hIconSm       = NULL;

    ::RegisterClassEx(&windowClass);
    HWND window = ::CreateWindow(
      windowClass.lpszClassName, "DummyWindow", WS_OVERLAPPEDWINDOW, 0, 0, 100, 100, NULL, NULL, windowClass.hInstance,
      NULL
    );

    HMODULE libDXGI  = GetModuleHandle("dxgi.dll");
    HMODULE libD3D12 = GetModuleHandle("d3d12.dll");
    if (!libDXGI || !libD3D12) {
      ::DestroyWindow(window);
      ::UnregisterClass(windowClass.lpszClassName, windowClass.hInstance);
      {
        std::ofstream logfile("BattleRanker_Cheat_Log.txt", std::ios::app);
        if (logfile.is_open())
          logfile << "Menu::init failed: dxgi.dll or d3d12.dll not found" << std::endl;
      }
      return false;
    }

    auto CreateDXGIFactory1_Func = (decltype(&CreateDXGIFactory1)) GetProcAddress(libDXGI, "CreateDXGIFactory1");
    auto D3D12CreateDevice_Func  = (decltype(&::D3D12CreateDevice)) GetProcAddress(libD3D12, "D3D12CreateDevice");

    if (!CreateDXGIFactory1_Func || !D3D12CreateDevice_Func) {
      {
        std::ofstream logfile("BattleRanker_Cheat_Log.txt", std::ios::app);
        if (logfile.is_open())
          logfile << "Menu::init failed: missing DXGI or D3D12 exports" << std::endl;
      }
      return false;
    }

    IDXGIFactory4* factory;
    if (FAILED(CreateDXGIFactory1_Func(__uuidof(IDXGIFactory4), (void**) &factory))) {
      {
        std::ofstream logfile("BattleRanker_Cheat_Log.txt", std::ios::app);
        if (logfile.is_open())
          logfile << "Menu::init failed: CreateDXGIFactory1 failed" << std::endl;
      }
      return false;
    }

    IDXGIAdapter* adapter;
    if (FAILED(factory->EnumAdapters(0, &adapter))) {
      {
        std::ofstream logfile("BattleRanker_Cheat_Log.txt", std::ios::app);
        if (logfile.is_open())
          logfile << "Menu::init failed: EnumAdapters failed" << std::endl;
      }
      return false;
    }

    ID3D12Device* device;
    if (FAILED(D3D12CreateDevice_Func(adapter, D3D_FEATURE_LEVEL_11_0, __uuidof(ID3D12Device), (void**) &device))) {
      {
        std::ofstream logfile("BattleRanker_Cheat_Log.txt", std::ios::app);
        if (logfile.is_open())
          logfile << "Menu::init failed: D3D12CreateDevice failed" << std::endl;
      }
      return false;
    }

    D3D12_COMMAND_QUEUE_DESC queueDesc;
    queueDesc.Type     = D3D12_COMMAND_LIST_TYPE_DIRECT;
    queueDesc.Priority = 0;
    queueDesc.Flags    = D3D12_COMMAND_QUEUE_FLAG_NONE;
    queueDesc.NodeMask = 0;

    ID3D12CommandQueue* commandQueue;
    if (FAILED(device->CreateCommandQueue(&queueDesc, __uuidof(ID3D12CommandQueue), (void**) &commandQueue))) {
      {
        std::ofstream logfile("BattleRanker_Cheat_Log.txt", std::ios::app);
        if (logfile.is_open())
          logfile << "Menu::init failed: CreateCommandQueue failed" << std::endl;
      }
      return false;
    }

    DXGI_SWAP_CHAIN_DESC1 swapChainDesc1 = {};
    swapChainDesc1.Width                 = 100;
    swapChainDesc1.Height                = 100;
    swapChainDesc1.Format                = DXGI_FORMAT_R8G8B8A8_UNORM;
    swapChainDesc1.SampleDesc.Count      = 1;
    swapChainDesc1.BufferUsage           = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    swapChainDesc1.BufferCount           = 2;
    swapChainDesc1.SwapEffect            = DXGI_SWAP_EFFECT_FLIP_DISCARD;

    IDXGISwapChain1* swapChain1          = nullptr;
    if (
      SUCCEEDED(factory->CreateSwapChainForHwnd(commandQueue, window, &swapChainDesc1, nullptr, nullptr, &swapChain1))
    ) {
      {
        std::ofstream logfile("BattleRanker_Cheat_Log.txt", std::ios::app);
        if (logfile.is_open())
          logfile << "Created swap chain, getting vtable" << std::endl;
      }
      void** swapChainVTable = *reinterpret_cast<void***>(swapChain1);

      {
        std::ofstream logfile("BattleRanker_Cheat_Log.txt", std::ios::app);
        if (logfile.is_open())
          logfile << "Hooking Present (Index 8)" << std::endl;
      }
      MH_CreateHook(swapChainVTable[8], (void*) &hkPresent, (void**) &oPresent);
      MH_EnableHook(swapChainVTable[8]);

      {
        std::ofstream logfile("BattleRanker_Cheat_Log.txt", std::ios::app);
        if (logfile.is_open())
          logfile << "Hooking Present1 (Index 22)" << std::endl;
      }
      MH_CreateHook(swapChainVTable[22], (void*) &hkPresent1, (void**) &oPresent1);
      MH_EnableHook(swapChainVTable[22]);

      {
        std::ofstream logfile("BattleRanker_Cheat_Log.txt", std::ios::app);
        if (logfile.is_open())
          logfile << "Releasing swap chain" << std::endl;
      }
      // swapChain1->Release();
    }
    else {
      {
        std::ofstream logfile("BattleRanker_Cheat_Log.txt", std::ios::app);
        if (logfile.is_open())
          logfile << "Menu::init failed: CreateSwapChainForHwnd failed" << std::endl;
      }
    }

    {
      std::ofstream logfile("BattleRanker_Cheat_Log.txt", std::ios::app);
      if (logfile.is_open())
        logfile << "Hooking ExecuteCommandLists (Index 10)" << std::endl;
    }
    void** commandQueueVTable = *reinterpret_cast<void***>(commandQueue);
    MH_CreateHook(commandQueueVTable[10], (void*) &hkExecuteCommandLists, (void**) &oExecuteCommandLists);
    MH_EnableHook(commandQueueVTable[10]);

    {
      std::ofstream logfile("BattleRanker_Cheat_Log.txt", std::ios::app);
      if (logfile.is_open())
        logfile << "Hooks set, releasing resources" << std::endl;
    }
    // commandQueue->Release();
    // device->Release();
    // adapter->Release();
    ::DestroyWindow(window);
    ::UnregisterClass(windowClass.lpszClassName, windowClass.hInstance);

    // Play a beep so we know Menu::init successfully finished and hooked DX12
    Beep(750, 300);

    return true;
  }

  void shutdown()
  {
    if (g_ImGuiInitialized) {
      ImGui_ImplDX12_Shutdown();
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
