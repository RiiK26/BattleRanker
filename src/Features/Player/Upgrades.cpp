#include "Upgrades.hpp"
#include "../../Modules/Hooks/Hooks.hpp"
#include "../../Modules/Hooks/Signatures.hpp"
#include "../../Modules/Menu/Menu.hpp"

namespace Features
{
  namespace Upgrades
  {
    float (*Orig_globalData_GetLPTotalDNeg)(void* this_ptr);
    float Hook_globalData_GetLPTotalDNeg(void* this_ptr)
    {
      if (Menu::Config.bMaxDNeg)
        return 99999.0f;
      return Orig_globalData_GetLPTotalDNeg(this_ptr);
    }

    float (*Orig_globalData_GetLPTotalCritical)(void* this_ptr);
    float Hook_globalData_GetLPTotalCritical(void* this_ptr)
    {
      if (Menu::Config.bMaxCritical)
        return 100.0f;
      return Orig_globalData_GetLPTotalCritical(this_ptr);
    }

    float (*Orig_globalData_GetLPTotalCritATK)(void* this_ptr);
    float Hook_globalData_GetLPTotalCritATK(void* this_ptr)
    {
      if (Menu::Config.bMaxCritAtk)
        return 999999.0f;
      return Orig_globalData_GetLPTotalCritATK(this_ptr);
    }

    float (*Orig_globalData_GetLPTotalDodge)(void* this_ptr);
    float Hook_globalData_GetLPTotalDodge(void* this_ptr)
    {
      if (Menu::Config.bMaxDodge)
        return 100.0f;
      return Orig_globalData_GetLPTotalDodge(this_ptr);
    }

    float (*Orig_globalData_GetLPTotalBuffEXP)(void* this_ptr);
    float Hook_globalData_GetLPTotalBuffEXP(void* this_ptr)
    {
      if (Menu::Config.bMaxEXP)
        return 9999999.0f;
      return Orig_globalData_GetLPTotalBuffEXP(this_ptr);
    }

    float (*Orig_globalData_GetLPTotalBuffGold)(void* this_ptr);
    float Hook_globalData_GetLPTotalBuffGold(void* this_ptr)
    {
      if (Menu::Config.bMaxGold)
        return 9999999.0f;
      return Orig_globalData_GetLPTotalBuffGold(this_ptr);
    }

    float (*Orig_globalData_GetLPTotalBossDmgBuff)(void* this_ptr);
    float Hook_globalData_GetLPTotalBossDmgBuff(void* this_ptr)
    {
      if (Menu::Config.bMaxBossDmg)
        return 9999999.0f;
      return Orig_globalData_GetLPTotalBossDmgBuff(this_ptr);
    }

    float (*Orig_globalData_GetLPTotalElemAtkCommonBuff)(void* this_ptr);
    float Hook_globalData_GetLPTotalElemAtkCommonBuff(void* this_ptr)
    {
      if (Menu::Config.bMaxElemAtk)
        return 999999.0f;
      return Orig_globalData_GetLPTotalElemAtkCommonBuff(this_ptr);
    }

    float (*Orig_globalData_GetLPTotalElemAtk)(void* this_ptr, int pElem);
    float Hook_globalData_GetLPTotalElemAtk(void* this_ptr, int pElem)
    {
      // Tree = 1, Water = 2, Fire = 3, Dark = 4, Light = 5
      if (pElem == 1 && Menu::Config.bMaxTreeElemAtk)
        return 999999.0f;
      if (pElem == 2 && Menu::Config.bMaxWaterElemAtk)
        return 999999.0f;
      if (pElem == 3 && Menu::Config.bMaxFireElemAtk)
        return 999999.0f;
      if (pElem == 5 && Menu::Config.bMaxLightElemAtk)
        return 999999.0f;

      return Orig_globalData_GetLPTotalElemAtk(this_ptr, pElem);
    }

    float (*Orig_GDOptionScript_GetLPAllOpPower)(void* this_ptr, int pType);
    float Hook_GDOptionScript_GetLPAllOpPower(void* this_ptr, int pType)
    {
      // RPGOptionTypeEnum::CoolTime = 14
      if (pType == 14 && Menu::Config.bMaxSkillCooltime)
        return 99.0f;
      return Orig_GDOptionScript_GetLPAllOpPower(this_ptr, pType);
    }

    void Initialize()
    {
      HOOK_SIGNATURE(
        "globalData::GetLPTotalDNeg", Signatures::globalData_GetLPTotalDNeg, Hook_globalData_GetLPTotalDNeg,
        Orig_globalData_GetLPTotalDNeg
      );
      HOOK_SIGNATURE(
        "globalData::GetLPTotalCritical", Signatures::globalData_GetLPTotalCritical, Hook_globalData_GetLPTotalCritical,
        Orig_globalData_GetLPTotalCritical
      );
      HOOK_SIGNATURE(
        "globalData::GetLPTotalCritATK", Signatures::globalData_GetLPTotalCritATK, Hook_globalData_GetLPTotalCritATK,
        Orig_globalData_GetLPTotalCritATK
      );
      HOOK_SIGNATURE(
        "globalData::GetLPTotalDodge", Signatures::globalData_GetLPTotalDodge, Hook_globalData_GetLPTotalDodge,
        Orig_globalData_GetLPTotalDodge
      );
      HOOK_SIGNATURE(
        "globalData::GetLPTotalBuffEXP", Signatures::globalData_GetLPTotalBuffEXP, Hook_globalData_GetLPTotalBuffEXP,
        Orig_globalData_GetLPTotalBuffEXP
      );
      HOOK_SIGNATURE(
        "globalData::GetLPTotalBuffGold", Signatures::globalData_GetLPTotalBuffGold, Hook_globalData_GetLPTotalBuffGold,
        Orig_globalData_GetLPTotalBuffGold
      );
      HOOK_SIGNATURE(
        "globalData::GetLPTotalBossDmgBuff", Signatures::globalData_GetLPTotalBossDmgBuff,
        Hook_globalData_GetLPTotalBossDmgBuff, Orig_globalData_GetLPTotalBossDmgBuff
      );
      HOOK_SIGNATURE(
        "globalData::GetLPTotalElemAtkCommonBuff", Signatures::globalData_GetLPTotalElemAtkCommonBuff,
        Hook_globalData_GetLPTotalElemAtkCommonBuff, Orig_globalData_GetLPTotalElemAtkCommonBuff
      );
      HOOK_SIGNATURE(
        "globalData::GetLPTotalElemAtk", Signatures::globalData_GetLPTotalElemAtk, Hook_globalData_GetLPTotalElemAtk,
        Orig_globalData_GetLPTotalElemAtk
      );
      HOOK_SIGNATURE(
        "GDOptionScript::GetLPAllOpPower", Signatures::GDOptionScript_GetLPAllOpPower,
        Hook_GDOptionScript_GetLPAllOpPower, Orig_GDOptionScript_GetLPAllOpPower
      );
    }

    void Uninitialize() { }
  }  // namespace Upgrades
}  // namespace Features
