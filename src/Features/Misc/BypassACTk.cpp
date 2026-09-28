#include "BypassACTk.hpp"
#include "../../Modules/Hooks/Hooks.hpp"
#include "../../Modules/Hooks/Signatures.hpp"
#include "../../Modules/Menu/Menu.hpp"

namespace Features
{
  namespace BypassACTk
  {
    typedef void (*StartAuto_t)(void* this_ptr);

    StartAuto_t Orig_InjStartAuto = nullptr;
    StartAuto_t Orig_ObsStartAuto = nullptr;
    StartAuto_t Orig_SpdStartAuto = nullptr;
    StartAuto_t Orig_TimStartAuto = nullptr;
    StartAuto_t Orig_WalStartAuto = nullptr;

    void Hook_InjStartAuto(void* this_ptr)
    {
      if (Menu::Config.bBypassACTk)
        return;
      Orig_InjStartAuto(this_ptr);
    }
    void Hook_ObsStartAuto(void* this_ptr)
    {
      if (Menu::Config.bBypassACTk)
        return;
      Orig_ObsStartAuto(this_ptr);
    }
    void Hook_SpdStartAuto(void* this_ptr)
    {
      if (Menu::Config.bBypassACTk)
        return;
      Orig_SpdStartAuto(this_ptr);
    }
    void Hook_TimStartAuto(void* this_ptr)
    {
      if (Menu::Config.bBypassACTk)
        return;
      Orig_TimStartAuto(this_ptr);
    }
    void Hook_WalStartAuto(void* this_ptr)
    {
      if (Menu::Config.bBypassACTk)
        return;
      Orig_WalStartAuto(this_ptr);
    }

    void Initialize()
    {
      HOOK_SIGNATURE(
        "InjectionDetector::StartDetectionAutomatically",
        Signatures::ACTk_InjectionDetector_StartDetectionAutomatically, Hook_InjStartAuto, Orig_InjStartAuto
      );
      HOOK_SIGNATURE(
        "ObscuredCheatingDetector::StartDetectionAutomatically",
        Signatures::ACTk_ObscuredCheatingDetector_StartDetectionAutomatically, Hook_ObsStartAuto, Orig_ObsStartAuto
      );
      HOOK_SIGNATURE(
        "SpeedHackDetector::StartDetectionAutomatically",
        Signatures::ACTk_SpeedHackDetector_StartDetectionAutomatically, Hook_SpdStartAuto, Orig_SpdStartAuto
      );
      HOOK_SIGNATURE(
        "TimeCheatingDetector::StartDetectionAutomatically",
        Signatures::ACTk_TimeCheatingDetector_StartDetectionAutomatically, Hook_TimStartAuto, Orig_TimStartAuto
      );
      HOOK_SIGNATURE(
        "WallHackDetector::StartDetectionAutomatically", Signatures::ACTk_WallHackDetector_StartDetectionAutomatically,
        Hook_WalStartAuto, Orig_WalStartAuto
      );
    }

    void Uninitialize() { }
  }  // namespace BypassACTk
}  // namespace Features
