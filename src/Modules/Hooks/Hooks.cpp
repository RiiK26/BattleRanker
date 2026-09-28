#include "Hooks.hpp"
#include "Signatures.hpp"
#include "../../Features/Player/GodMode.hpp"
#include "../../Features/Player/Upgrades.hpp"
#include "../../Features/Player/DetailedStats.hpp"
#include "../../Features/Currency/InfiniteCurrency.hpp"
#include "../../Features/Currency/SetCurrency.hpp"
#include "../../Features/Misc/BypassACTk.hpp"

namespace Hooks
{
  bool init()
  {
    if (MH_Initialize() != MH_OK) {
      return false;
    }

    Signatures::Resolve();

    Features::GodMode::Initialize();
    Features::Upgrades::Initialize();
    Features::DetailedStats::Initialize();
    Features::InfiniteCurrency::Initialize();
    Features::SetCurrency::Initialize();
    Features::BypassACTk::Initialize();

    return true;
  }

  void shutdown()
  {
    Features::GodMode::Uninitialize();
    Features::Upgrades::Uninitialize();
    Features::DetailedStats::Uninitialize();
    Features::InfiniteCurrency::Uninitialize();
    Features::SetCurrency::Uninitialize();
    Features::BypassACTk::Uninitialize();

    MH_DisableHook(MH_ALL_HOOKS);
    MH_Uninitialize();
  }
}  // namespace Hooks
