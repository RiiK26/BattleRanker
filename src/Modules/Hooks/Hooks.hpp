#ifndef HOOKS_H
#define HOOKS_H

#include <MinHook.h>
#include <fstream>   // IWYU pragma: keep
#include <iostream>  // IWYU pragma: keep

#define HOOK_SIGNATURE(Name, SignaturePtr, HookFunc, OrigFunc) \
  { \
    std::ofstream logfile("BattleRanker_Cheat_Log.txt", std::ios::app); \
    if (logfile.is_open()) \
      logfile << "Hooking: " << Name << " -> " << (SignaturePtr ? "FOUND" : "NOT FOUND") << std::endl; \
  } \
  if (SignaturePtr) { \
    MH_CreateHook(SignaturePtr, (void*) HookFunc, (void**) &OrigFunc); \
    MH_EnableHook(SignaturePtr); \
  }

namespace Hooks
{
  bool init();
  void shutdown();
}  // namespace Hooks

#endif  // HOOKS_H
