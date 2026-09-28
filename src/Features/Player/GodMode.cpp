#include "GodMode.hpp"
#include "../../Modules/Hooks/Hooks.hpp"
#include "../../Modules/Hooks/Signatures.hpp"
#include "../../Modules/Menu/Menu.hpp"

namespace Features
{
  namespace GodMode
  {
    void (*Orig_PlayerV3Script_GetHitV2)(void* this_ptr, float pAtk, float pDneg, void* pSender);
    void Hook_PlayerV3Script_GetHitV2(void* this_ptr, float pAtk, float pDneg, void* pSender)
    {
      if (Menu::Config.bGodMode_Invincibility)
        return;
      Orig_PlayerV3Script_GetHitV2(this_ptr, pAtk, pDneg, pSender);
    }

    double (*Orig_RPGPlayerData_get_currentHP)(void* this_ptr);
    double Hook_RPGPlayerData_get_currentHP(void* this_ptr) { return Orig_RPGPlayerData_get_currentHP(this_ptr); }

    void (*Orig_RPGPlayerData_set_currentHP)(void* this_ptr, double value);
    void Hook_RPGPlayerData_set_currentHP(void* this_ptr, double value)
    {
      if (Menu::Config.bGodMode_Invincibility && Orig_RPGPlayerData_get_currentHP) {
        double current = Orig_RPGPlayerData_get_currentHP(this_ptr);
        if (value < current)
          return;
      }
      Orig_RPGPlayerData_set_currentHP(this_ptr, value);
    }


    void Initialize()
    {
      HOOK_SIGNATURE(
        "PlayerV3Script::GetHitV2", Signatures::PlayerV3Script_GetHitV2, Hook_PlayerV3Script_GetHitV2,
        Orig_PlayerV3Script_GetHitV2
      );
      HOOK_SIGNATURE(
        "RPGPlayerData::get_currentHP", Signatures::RPGPlayerData_get_currentHP, Hook_RPGPlayerData_get_currentHP,
        Orig_RPGPlayerData_get_currentHP
      );
      HOOK_SIGNATURE(
        "RPGPlayerData::set_currentHP", Signatures::RPGPlayerData_set_currentHP, Hook_RPGPlayerData_set_currentHP,
        Orig_RPGPlayerData_set_currentHP
      );
    }

    void Uninitialize() { }
  }  // namespace GodMode
}  // namespace Features
