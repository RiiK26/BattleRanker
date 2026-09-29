#include <fstream>
#include <windows.h>
#include <iostream>
#include "Modules/Utils/MonoUtils.hpp"
#include "Modules/Hooks/Hooks.hpp"
#include "Modules/Menu/Menu.hpp"

void MainThread(HMODULE hModule)
{
  {
    std::ofstream logfile("BattleRanker_Log.txt", std::ios::trunc);
    if (logfile.is_open())
      logfile << "Thread Started" << std::endl;
  }

  // Wait for the game to initialize
  while (!GetModuleHandleA("mono-2.0-bdwgc.dll") && !GetModuleHandleA("mono.dll")) {
    Sleep(100);
  }

  {
    std::ofstream logfile("BattleRanker_Log.txt", std::ios::app);
    if (logfile.is_open())
      logfile << "Mono loaded" << std::endl;
  }

  // Wait for graphics APIs to be loaded (Timeout after 30 seconds)
  int  retries     = 300;
  bool dxgiLoaded  = false;
  bool d3d11Loaded = false;
  bool d3d12Loaded = false;
  while (retries > 0) {
    dxgiLoaded  = GetModuleHandleA("dxgi.dll") != nullptr;
    d3d11Loaded = GetModuleHandleA("d3d11.dll") != nullptr;
    d3d12Loaded = GetModuleHandleA("d3d12.dll") != nullptr;
    if (dxgiLoaded && (d3d11Loaded || d3d12Loaded))
      break;
    Sleep(100);
    retries--;
  }

  if (!dxgiLoaded || (!d3d11Loaded && !d3d12Loaded)) {
    char errorMsg[256];
    sprintf(
      errorMsg, "Graphics API wait timed out!\ndxgi.dll loaded: %s\nd3d11.dll loaded: %s\nd3d12.dll loaded: %s",
      dxgiLoaded ? "Yes" : "No", d3d11Loaded ? "Yes" : "No", d3d12Loaded ? "Yes" : "No"
    );
    {
      std::ofstream logfile("BattleRanker_Log.txt", std::ios::app);
      if (logfile.is_open())
        logfile << errorMsg << std::endl;
    }
    // Don't exit, try to continue just in case GetModuleHandle lied to us or we can force load it
  }
  else {
    {
      std::ofstream logfile("BattleRanker_Log.txt", std::ios::app);
      if (logfile.is_open())
        logfile << "DXGI and D3D12 loaded successfully!" << std::endl;
    }
  }

  // Additional wait just in case mono is loaded but not fully initialized
  Sleep(2000);

  if (mono::init()) {
    {
      std::ofstream logfile("BattleRanker_Log.txt", std::ios::app);
      if (logfile.is_open())
        logfile << "mono::init success, calling Hooks::init" << std::endl;
    }
    Hooks::init();
    {
      std::ofstream logfile("BattleRanker_Log.txt", std::ios::app);
      if (logfile.is_open())
        logfile << "Hooks::init success, calling Menu::init" << std::endl;
    }
    Menu::init();
  }
  else {
    {
      std::ofstream logfile("BattleRanker_Log.txt", std::ios::app);
      if (logfile.is_open())
        logfile << "Failed to initialize Mono Utils." << std::endl;
    }
  }

  // Wait for unload key if necessary, or just run infinitely
  while (!(GetAsyncKeyState(VK_END) & 1)) {
    Sleep(100);
  }

  Menu::shutdown();
  Hooks::shutdown();

  FreeLibraryAndExitThread(hModule, 0);
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved)
{
  switch (ul_reason_for_call) {
  case DLL_PROCESS_ATTACH :
    DisableThreadLibraryCalls(hModule);
    CreateThread(nullptr, 0, (LPTHREAD_START_ROUTINE) MainThread, hModule, 0, nullptr);
    break;
  case DLL_THREAD_ATTACH :
  case DLL_THREAD_DETACH :
  case DLL_PROCESS_DETACH :
    break;
  }
  return TRUE;
}
