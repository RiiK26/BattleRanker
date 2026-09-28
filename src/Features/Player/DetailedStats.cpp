#include "DetailedStats.hpp"
#include "../../Modules/Hooks/Hooks.hpp"
#include "../../Modules/Hooks/Signatures.hpp"
#include "../../Modules/Utils/MonoUtils.hpp"
#include <imgui.h>

namespace Features
{
  namespace DetailedStats
  {
    ConfigData Config;

    // Macro for simple float hooks (no parameters)
#define HOOK_FLOAT_0(MethodName, HookName, OrigName, ConfigBool, ConfigValue) \
  float (*OrigName)(void* this_ptr); \
  float HookName(void* this_ptr) \
  { \
    if (Config.ConfigBool) \
      return Config.ConfigValue; \
    return OrigName(this_ptr); \
  }

    // Macro for simple float hooks (1 int parameter)
#define HOOK_FLOAT_1(MethodName, HookName, OrigName, ConfigBool, ConfigValue) \
  float (*OrigName)(void* this_ptr, int p1); \
  float HookName(void* this_ptr, int p1) \
  { \
    if (Config.ConfigBool) \
      return Config.ConfigValue; \
    return OrigName(this_ptr, p1); \
  }

    // Base
    HOOK_FLOAT_1(globalData_GetBaseATK, Hook_globalData_GetBaseATK, Orig_globalData_GetBaseATK, bSetBaseAtk, fBaseAtk)
    HOOK_FLOAT_1(globalData_GetBaseDEF, Hook_globalData_GetBaseDEF, Orig_globalData_GetBaseDEF, bSetBaseDef, fBaseDef)
    HOOK_FLOAT_1(globalData_GetBaseHP, Hook_globalData_GetBaseHP, Orig_globalData_GetBaseHP, bSetBaseHP, fBaseHP)

    // Upgrades
    HOOK_FLOAT_1(globalData_GetAtkBonus, Hook_globalData_GetAtkBonus, Orig_globalData_GetAtkBonus, bSetUpAtk, fUpAtk)
    HOOK_FLOAT_1(globalData_GetDefBonus, Hook_globalData_GetDefBonus, Orig_globalData_GetDefBonus, bSetUpDef, fUpDef)
    HOOK_FLOAT_1(globalData_GetHPBonus, Hook_globalData_GetHPBonus, Orig_globalData_GetHPBonus, bSetUpHP, fUpHP)
    HOOK_FLOAT_1(globalData_GetDNeg, Hook_globalData_GetDNeg, Orig_globalData_GetDNeg, bSetUpDNeg, fUpDNeg)
    HOOK_FLOAT_1(globalData_GetCrit, Hook_globalData_GetCrit, Orig_globalData_GetCrit, bSetUpCrit, fUpCrit)
    HOOK_FLOAT_1(
      globalData_GetCritAtk, Hook_globalData_GetCritAtk, Orig_globalData_GetCritAtk, bSetUpCritAtk, fUpCritAtk
    )
    HOOK_FLOAT_1(globalData_GetDodge, Hook_globalData_GetDodge, Orig_globalData_GetDodge, bSetUpDodge, fUpDodge)

    float (*Orig_globalData_GetLPElemAtk)(void* this_ptr, int pElem);
    float Hook_globalData_GetLPElemAtk(void* this_ptr, int pElem)
    {
      if (pElem == 1 && Config.bSetUpTreeAtk)
        return Config.fUpTreeAtk;
      if (pElem == 2 && Config.bSetUpWaterAtk)
        return Config.fUpWaterAtk;
      if (pElem == 3 && Config.bSetUpFireAtk)
        return Config.fUpFireAtk;
      if (pElem == 5 && Config.bSetUpLightAtk)
        return Config.fUpLightAtk;
      return Orig_globalData_GetLPElemAtk(this_ptr, pElem);
    }

    // Gear/Acc/Pet
    HOOK_FLOAT_0(
      globalData_GetCurrentLPGearBonusTotalAtk,
      Hook_globalData_GetCurrentLPGearBonusTotalAtk,
      Orig_globalData_GetCurrentLPGearBonusTotalAtk,
      bSetGearAtk,
      fGearAtk
    )
    HOOK_FLOAT_0(
      globalData_GetCurrentLPGearBonusTotalDef,
      Hook_globalData_GetCurrentLPGearBonusTotalDef,
      Orig_globalData_GetCurrentLPGearBonusTotalDef,
      bSetGearDef,
      fGearDef
    )
    HOOK_FLOAT_0(
      globalData_GetCurrentLPAccBonusTotalAtk,
      Hook_globalData_GetCurrentLPAccBonusTotalAtk,
      Orig_globalData_GetCurrentLPAccBonusTotalAtk,
      bSetAccAtk,
      fAccAtk
    )
    HOOK_FLOAT_0(
      globalData_GetCurrentLPAccBonusTotalDef,
      Hook_globalData_GetCurrentLPAccBonusTotalDef,
      Orig_globalData_GetCurrentLPAccBonusTotalDef,
      bSetAccDef,
      fAccDef
    )
    HOOK_FLOAT_0(
      globalData_GetCurrentLPPetBonusTotalDef,
      Hook_globalData_GetCurrentLPPetBonusTotalDef,
      Orig_globalData_GetCurrentLPPetBonusTotalDef,
      bSetPetDef,
      fPetDef
    )
    HOOK_FLOAT_0(
      globalData_GetCurrentLPPetTotalHP,
      Hook_globalData_GetCurrentLPPetTotalHP,
      Orig_globalData_GetCurrentLPPetTotalHP,
      bSetPetHP,
      fPetHP
    )

    // Implants
    HOOK_FLOAT_1(
      globalData_GetLPImplantTreeDef,
      Hook_globalData_GetLPImplantTreeDef,
      Orig_globalData_GetLPImplantTreeDef,
      bSetImpTreeDef,
      fImpTreeDef
    )
    HOOK_FLOAT_1(
      globalData_GetLPImplantFireHP,
      Hook_globalData_GetLPImplantFireHP,
      Orig_globalData_GetLPImplantFireHP,
      bSetImpFireHP,
      fImpFireHP
    )
    HOOK_FLOAT_1(
      globalData_GetLPImplantWaterDNeg,
      Hook_globalData_GetLPImplantWaterDNeg,
      Orig_globalData_GetLPImplantWaterDNeg,
      bSetImpWaterDNeg,
      fImpWaterDNeg
    )
    HOOK_FLOAT_1(
      globalData_GetLPImplantLightCrAtk,
      Hook_globalData_GetLPImplantLightCrAtk,
      Orig_globalData_GetLPImplantLightCrAtk,
      bSetImpLightCrAtk,
      fImpLightCrAtk
    )

    // Option Buffs (GDOptionScript)
    HOOK_FLOAT_1(
      GDOptionScript_GetLPGearOpPower,
      Hook_GDOptionScript_GetLPGearOpPower,
      Orig_GDOptionScript_GetLPGearOpPower,
      bSetOpGear,
      fOpGear
    )
    HOOK_FLOAT_1(
      GDOptionScript_GetLPCharOpPower,
      Hook_GDOptionScript_GetLPCharOpPower,
      Orig_GDOptionScript_GetLPCharOpPower,
      bSetOpChar,
      fOpChar
    )
    HOOK_FLOAT_1(
      GDOptionScript_GetLPPetOpPower,
      Hook_GDOptionScript_GetLPPetOpPower,
      Orig_GDOptionScript_GetLPPetOpPower,
      bSetOpPet,
      fOpPet
    )
    HOOK_FLOAT_1(
      GDOptionScript_GetLPAccOpPower,
      Hook_GDOptionScript_GetLPAccOpPower,
      Orig_GDOptionScript_GetLPAccOpPower,
      bSetOpAcc,
      fOpAcc
    )
    HOOK_FLOAT_1(
      GDOptionScript_GetLPAVSkinOpPower,
      Hook_GDOptionScript_GetLPAVSkinOpPower,
      Orig_GDOptionScript_GetLPAVSkinOpPower,
      bSetOpSkin,
      fOpSkin
    )
    HOOK_FLOAT_1(
      GDOptionScript_GetTotalSkinSettOpPower,
      Hook_GDOptionScript_GetTotalSkinSettOpPower,
      Orig_GDOptionScript_GetTotalSkinSettOpPower,
      bSetOpSkinSet,
      fOpSkinSet
    )
    HOOK_FLOAT_1(
      GDOptionScript_GetAllSkillAllOpPower,
      Hook_GDOptionScript_GetAllSkillAllOpPower,
      Orig_GDOptionScript_GetAllSkillAllOpPower,
      bSetOpSkill,
      fOpSkill
    )

    // Global Buffs (%)
    HOOK_FLOAT_0(
      globalData_GetLPTotalBuffAtk,
      Hook_globalData_GetLPTotalBuffAtk,
      Orig_globalData_GetLPTotalBuffAtk,
      bSetBuffAtk,
      fBuffAtk
    )
    HOOK_FLOAT_0(
      globalData_GetLPTotalBuffDef,
      Hook_globalData_GetLPTotalBuffDef,
      Orig_globalData_GetLPTotalBuffDef,
      bSetBuffDef,
      fBuffDef
    )
    HOOK_FLOAT_0(
      globalData_GetLPTotalBuffHP,
      Hook_globalData_GetLPTotalBuffHP,
      Orig_globalData_GetLPTotalBuffHP,
      bSetBuffHP,
      fBuffHP
    )
    HOOK_FLOAT_0(
      globalData_GetLPTotalBuffDNeg,
      Hook_globalData_GetLPTotalBuffDNeg,
      Orig_globalData_GetLPTotalBuffDNeg,
      bSetBuffDNeg,
      fBuffDNeg
    )
    HOOK_FLOAT_0(
      globalData_GetLPTotalBuffGold,
      Hook_globalData_GetLPTotalBuffGold,
      Orig_globalData_GetLPTotalBuffGold,
      bSetBuffGold,
      fBuffGold
    )
    HOOK_FLOAT_0(
      globalData_GetLPTotalBuffEXP,
      Hook_globalData_GetLPTotalBuffEXP,
      Orig_globalData_GetLPTotalBuffEXP,
      bSetBuffEXP,
      fBuffEXP
    )
    HOOK_FLOAT_0(
      globalData_GetLPTotalBossDmgBuff,
      Hook_globalData_GetLPTotalBossDmgBuff,
      Orig_globalData_GetLPTotalBossDmgBuff,
      bSetBuffBoss,
      fBuffBoss
    )
    HOOK_FLOAT_0(
      globalData_GetLPTotalElemAtkCommonBuff,
      Hook_globalData_GetLPTotalElemAtkCommonBuff,
      Orig_globalData_GetLPTotalElemAtkCommonBuff,
      bSetBuffElem,
      fBuffElem
    )

    void Initialize()
    {
      // Signatures must be added to Signatures.cpp and Modules/Hooks/Hooks.hpp
      // But we can resolve them right here to save editing Signatures.hpp if we want!
      // Wait, let's just resolve them locally.
      auto get_method               = mono::get_method;

      void* globalData_GetBaseATK   = get_method("Assembly-CSharp", "", "globalData", "GetBaseATK", 1);
      void* globalData_GetBaseDEF   = get_method("Assembly-CSharp", "", "globalData", "GetBaseDEF", 1);
      void* globalData_GetBaseHP    = get_method("Assembly-CSharp", "", "globalData", "GetBaseHP", 1);

      void* globalData_GetAtkBonus  = get_method("Assembly-CSharp", "", "globalData", "GetAtkBonus", 1);
      void* globalData_GetDefBonus  = get_method("Assembly-CSharp", "", "globalData", "GetDefBonus", 1);
      void* globalData_GetHPBonus   = get_method("Assembly-CSharp", "", "globalData", "GetHPBonus", 1);
      void* globalData_GetDNeg      = get_method("Assembly-CSharp", "", "globalData", "GetDNeg", 1);
      void* globalData_GetCrit      = get_method("Assembly-CSharp", "", "globalData", "GetCrit", 1);
      void* globalData_GetCritAtk   = get_method("Assembly-CSharp", "", "globalData", "GetCritAtk", 1);
      void* globalData_GetDodge     = get_method("Assembly-CSharp", "", "globalData", "GetDodge", 1);
      void* globalData_GetLPElemAtk = get_method("Assembly-CSharp", "", "globalData", "GetLPElemAtk", 1);

      void* globalData_GetCurrentLPGearBonusTotalAtk =
        get_method("Assembly-CSharp", "", "globalData", "GetCurrentLPGearBonusTotalAtk", 0);
      void* globalData_GetCurrentLPGearBonusTotalDef =
        get_method("Assembly-CSharp", "", "globalData", "GetCurrentLPGearBonusTotalDef", 0);
      void* globalData_GetCurrentLPAccBonusTotalAtk =
        get_method("Assembly-CSharp", "", "globalData", "GetCurrentLPAccBonusTotalAtk", 0);
      void* globalData_GetCurrentLPAccBonusTotalDef =
        get_method("Assembly-CSharp", "", "globalData", "GetCurrentLPAccBonusTotalDef", 0);
      void* globalData_GetCurrentLPPetBonusTotalDef =
        get_method("Assembly-CSharp", "", "globalData", "GetCurrentLPPetBonusTotalDef", 0);
      void* globalData_GetCurrentLPPetTotalHP =
        get_method("Assembly-CSharp", "", "globalData", "GetCurrentLPPetTotalHP", 0);

      void* globalData_GetLPImplantTreeDef = get_method("Assembly-CSharp", "", "globalData", "GetLPImplantTreeDef", 1);
      void* globalData_GetLPImplantFireHP  = get_method("Assembly-CSharp", "", "globalData", "GetLPImplantFireHP", 1);
      void* globalData_GetLPImplantWaterDNeg =
        get_method("Assembly-CSharp", "", "globalData", "GetLPImplantWaterDNeg", 1);
      void* globalData_GetLPImplantLightCrAtk =
        get_method("Assembly-CSharp", "", "globalData", "GetLPImplantLightCrAtk", 1);

      void* GDOptionScript_GetLPGearOpPower =
        get_method("Assembly-CSharp", "", "GDOptionScript", "GetLPGearOpPower", 1);
      void* GDOptionScript_GetLPCharOpPower =
        get_method("Assembly-CSharp", "", "GDOptionScript", "GetLPCharOpPower", 1);
      void* GDOptionScript_GetLPPetOpPower = get_method("Assembly-CSharp", "", "GDOptionScript", "GetLPPetOpPower", 1);
      void* GDOptionScript_GetLPAccOpPower = get_method("Assembly-CSharp", "", "GDOptionScript", "GetLPAccOpPower", 1);
      void* GDOptionScript_GetLPAVSkinOpPower =
        get_method("Assembly-CSharp", "", "GDOptionScript", "GetLPAVSkinOpPower", 1);
      void* GDOptionScript_GetTotalSkinSettOpPower =
        get_method("Assembly-CSharp", "", "GDOptionScript", "GetTotalSkinSettOpPower", 1);
      void* GDOptionScript_GetAllSkillAllOpPower =
        get_method("Assembly-CSharp", "", "GDOptionScript", "GetAllSkillAllOpPower", 1);

      void* globalData_GetLPTotalBuffAtk  = get_method("Assembly-CSharp", "", "globalData", "GetLPTotalBuffAtk", 0);
      void* globalData_GetLPTotalBuffDef  = get_method("Assembly-CSharp", "", "globalData", "GetLPTotalBuffDef", 0);
      void* globalData_GetLPTotalBuffHP   = get_method("Assembly-CSharp", "", "globalData", "GetLPTotalBuffHP", 0);
      void* globalData_GetLPTotalBuffDNeg = get_method("Assembly-CSharp", "", "globalData", "GetLPTotalBuffDNeg", 0);
      void* globalData_GetLPTotalBuffGold = get_method("Assembly-CSharp", "", "globalData", "GetLPTotalBuffGold", 0);
      void* globalData_GetLPTotalBuffEXP  = get_method("Assembly-CSharp", "", "globalData", "GetLPTotalBuffEXP", 0);
      void* globalData_GetLPTotalBossDmgBuff =
        get_method("Assembly-CSharp", "", "globalData", "GetLPTotalBossDmgBuff", 0);
      void* globalData_GetLPTotalElemAtkCommonBuff =
        get_method("Assembly-CSharp", "", "globalData", "GetLPTotalElemAtkCommonBuff", 0);

#define DO_HOOK(Name, Addr) HOOK_SIGNATURE(#Name, Addr, Hook_##Name, Orig_##Name)

      DO_HOOK(globalData_GetBaseATK, globalData_GetBaseATK);
      DO_HOOK(globalData_GetBaseDEF, globalData_GetBaseDEF);
      DO_HOOK(globalData_GetBaseHP, globalData_GetBaseHP);
      DO_HOOK(globalData_GetAtkBonus, globalData_GetAtkBonus);
      DO_HOOK(globalData_GetDefBonus, globalData_GetDefBonus);
      DO_HOOK(globalData_GetHPBonus, globalData_GetHPBonus);
      DO_HOOK(globalData_GetDNeg, globalData_GetDNeg);
      DO_HOOK(globalData_GetCrit, globalData_GetCrit);
      DO_HOOK(globalData_GetCritAtk, globalData_GetCritAtk);
      DO_HOOK(globalData_GetDodge, globalData_GetDodge);
      DO_HOOK(globalData_GetLPElemAtk, globalData_GetLPElemAtk);

      DO_HOOK(globalData_GetCurrentLPGearBonusTotalAtk, globalData_GetCurrentLPGearBonusTotalAtk);
      DO_HOOK(globalData_GetCurrentLPGearBonusTotalDef, globalData_GetCurrentLPGearBonusTotalDef);
      DO_HOOK(globalData_GetCurrentLPAccBonusTotalAtk, globalData_GetCurrentLPAccBonusTotalAtk);
      DO_HOOK(globalData_GetCurrentLPAccBonusTotalDef, globalData_GetCurrentLPAccBonusTotalDef);
      DO_HOOK(globalData_GetCurrentLPPetBonusTotalDef, globalData_GetCurrentLPPetBonusTotalDef);
      DO_HOOK(globalData_GetCurrentLPPetTotalHP, globalData_GetCurrentLPPetTotalHP);

      DO_HOOK(globalData_GetLPImplantTreeDef, globalData_GetLPImplantTreeDef);
      DO_HOOK(globalData_GetLPImplantFireHP, globalData_GetLPImplantFireHP);
      DO_HOOK(globalData_GetLPImplantWaterDNeg, globalData_GetLPImplantWaterDNeg);
      DO_HOOK(globalData_GetLPImplantLightCrAtk, globalData_GetLPImplantLightCrAtk);

      DO_HOOK(GDOptionScript_GetLPGearOpPower, GDOptionScript_GetLPGearOpPower);
      DO_HOOK(GDOptionScript_GetLPCharOpPower, GDOptionScript_GetLPCharOpPower);
      DO_HOOK(GDOptionScript_GetLPPetOpPower, GDOptionScript_GetLPPetOpPower);
      DO_HOOK(GDOptionScript_GetLPAccOpPower, GDOptionScript_GetLPAccOpPower);
      DO_HOOK(GDOptionScript_GetLPAVSkinOpPower, GDOptionScript_GetLPAVSkinOpPower);
      DO_HOOK(GDOptionScript_GetTotalSkinSettOpPower, GDOptionScript_GetTotalSkinSettOpPower);
      DO_HOOK(GDOptionScript_GetAllSkillAllOpPower, GDOptionScript_GetAllSkillAllOpPower);

      DO_HOOK(globalData_GetLPTotalBuffAtk, globalData_GetLPTotalBuffAtk);
      DO_HOOK(globalData_GetLPTotalBuffDef, globalData_GetLPTotalBuffDef);
      DO_HOOK(globalData_GetLPTotalBuffHP, globalData_GetLPTotalBuffHP);
      DO_HOOK(globalData_GetLPTotalBuffDNeg, globalData_GetLPTotalBuffDNeg);
      DO_HOOK(globalData_GetLPTotalBuffGold, globalData_GetLPTotalBuffGold);
      DO_HOOK(globalData_GetLPTotalBuffEXP, globalData_GetLPTotalBuffEXP);
      DO_HOOK(globalData_GetLPTotalBossDmgBuff, globalData_GetLPTotalBossDmgBuff);
      DO_HOOK(globalData_GetLPTotalElemAtkCommonBuff, globalData_GetLPTotalElemAtkCommonBuff);
    }

    void Uninitialize() { }

    void RenderUI()
    {
      ImGui::Text("Base Stats");
      ImGui::Checkbox("Set Base ATK", &Config.bSetBaseAtk);
      ImGui::SameLine(150);
      ImGui::InputFloat("##fBaseAtk", &Config.fBaseAtk);
      ImGui::Checkbox("Set Base DEF", &Config.bSetBaseDef);
      ImGui::SameLine(150);
      ImGui::InputFloat("##fBaseDef", &Config.fBaseDef);
      ImGui::Checkbox("Set Base HP", &Config.bSetBaseHP);
      ImGui::SameLine(150);
      ImGui::InputFloat("##fBaseHP", &Config.fBaseHP);

      ImGui::Separator();
      ImGui::Text("Upgrade Stats (Bonus Lv)");
      ImGui::Checkbox("Set UP ATK", &Config.bSetUpAtk);
      ImGui::SameLine(150);
      ImGui::InputFloat("##fUpAtk", &Config.fUpAtk);
      ImGui::Checkbox("Set UP DEF", &Config.bSetUpDef);
      ImGui::SameLine(150);
      ImGui::InputFloat("##fUpDef", &Config.fUpDef);
      ImGui::Checkbox("Set UP HP", &Config.bSetUpHP);
      ImGui::SameLine(150);
      ImGui::InputFloat("##fUpHP", &Config.fUpHP);
      ImGui::Checkbox("Set UP DNeg", &Config.bSetUpDNeg);
      ImGui::SameLine(150);
      ImGui::InputFloat("##fUpDNeg", &Config.fUpDNeg);
      ImGui::Checkbox("Set UP Crit%", &Config.bSetUpCrit);
      ImGui::SameLine(150);
      ImGui::InputFloat("##fUpCrit", &Config.fUpCrit);
      ImGui::Checkbox("Set UP CritATK", &Config.bSetUpCritAtk);
      ImGui::SameLine(150);
      ImGui::InputFloat("##fUpCritAtk", &Config.fUpCritAtk);
      ImGui::Checkbox("Set UP Dodge%", &Config.bSetUpDodge);
      ImGui::SameLine(150);
      ImGui::InputFloat("##fUpDodge", &Config.fUpDodge);
      ImGui::Checkbox("Set UP TreeELEM", &Config.bSetUpTreeAtk);
      ImGui::SameLine(150);
      ImGui::InputFloat("##fUpTree", &Config.fUpTreeAtk);
      ImGui::Checkbox("Set UP WaterELEM", &Config.bSetUpWaterAtk);
      ImGui::SameLine(150);
      ImGui::InputFloat("##fUpWater", &Config.fUpWaterAtk);
      ImGui::Checkbox("Set UP FireELEM", &Config.bSetUpFireAtk);
      ImGui::SameLine(150);
      ImGui::InputFloat("##fUpFire", &Config.fUpFireAtk);
      ImGui::Checkbox("Set UP LightELEM", &Config.bSetUpLightAtk);
      ImGui::SameLine(150);
      ImGui::InputFloat("##fUpLight", &Config.fUpLightAtk);

      ImGui::Separator();
      ImGui::Text("Gear / Accessory / Pet (JennyCore)");
      ImGui::Checkbox("Set Gear ATK", &Config.bSetGearAtk);
      ImGui::SameLine(150);
      ImGui::InputFloat("##fGearAtk", &Config.fGearAtk);
      ImGui::Checkbox("Set Gear DEF", &Config.bSetGearDef);
      ImGui::SameLine(150);
      ImGui::InputFloat("##fGearDef", &Config.fGearDef);
      ImGui::Checkbox("Set Acc ATK", &Config.bSetAccAtk);
      ImGui::SameLine(150);
      ImGui::InputFloat("##fAccAtk", &Config.fAccAtk);
      ImGui::Checkbox("Set Acc DEF", &Config.bSetAccDef);
      ImGui::SameLine(150);
      ImGui::InputFloat("##fAccDef", &Config.fAccDef);
      ImGui::Checkbox("Set Pet DEF", &Config.bSetPetDef);
      ImGui::SameLine(150);
      ImGui::InputFloat("##fPetDef", &Config.fPetDef);
      ImGui::Checkbox("Set Pet HP", &Config.bSetPetHP);
      ImGui::SameLine(150);
      ImGui::InputFloat("##fPetHP", &Config.fPetHP);

      ImGui::Separator();
      ImGui::Text("Implants");
      ImGui::Checkbox("Set Imp Tree DEF", &Config.bSetImpTreeDef);
      ImGui::SameLine(150);
      ImGui::InputFloat("##fImpTreeDef", &Config.fImpTreeDef);
      ImGui::Checkbox("Set Imp Fire HP", &Config.bSetImpFireHP);
      ImGui::SameLine(150);
      ImGui::InputFloat("##fImpFireHP", &Config.fImpFireHP);
      ImGui::Checkbox("Set Imp W.DNeg", &Config.bSetImpWaterDNeg);
      ImGui::SameLine(150);
      ImGui::InputFloat("##fImpWaterDNeg", &Config.fImpWaterDNeg);
      ImGui::Checkbox("Set Imp L.CrAtk", &Config.bSetImpLightCrAtk);
      ImGui::SameLine(150);
      ImGui::InputFloat("##fImpLightCrAtk", &Config.fImpLightCrAtk);

      ImGui::Separator();
      ImGui::Text("Option Stats (%) (Overrides ALL Types for Gear/Char/Acc, etc.)");
      ImGui::Checkbox("Set Op Gear %", &Config.bSetOpGear);
      ImGui::SameLine(150);
      ImGui::InputFloat("##fOpGear", &Config.fOpGear);
      ImGui::Checkbox("Set Op Char %", &Config.bSetOpChar);
      ImGui::SameLine(150);
      ImGui::InputFloat("##fOpChar", &Config.fOpChar);
      ImGui::Checkbox("Set Op Pet %", &Config.bSetOpPet);
      ImGui::SameLine(150);
      ImGui::InputFloat("##fOpPet", &Config.fOpPet);
      ImGui::Checkbox("Set Op Acc %", &Config.bSetOpAcc);
      ImGui::SameLine(150);
      ImGui::InputFloat("##fOpAcc", &Config.fOpAcc);
      ImGui::Checkbox("Set Op Skin %", &Config.bSetOpSkin);
      ImGui::SameLine(150);
      ImGui::InputFloat("##fOpSkin", &Config.fOpSkin);
      ImGui::Checkbox("Set Op SkinSet %", &Config.bSetOpSkinSet);
      ImGui::SameLine(150);
      ImGui::InputFloat("##fOpSkinSet", &Config.fOpSkinSet);
      ImGui::Checkbox("Set Op Skill %", &Config.bSetOpSkill);
      ImGui::SameLine(150);
      ImGui::InputFloat("##fOpSkill", &Config.fOpSkill);

      ImGui::Separator();
      ImGui::Text("Global Buffs (%)");
      ImGui::Checkbox("Set Buff ATK %", &Config.bSetBuffAtk);
      ImGui::SameLine(150);
      ImGui::InputFloat("##fBuffAtk", &Config.fBuffAtk);
      ImGui::Checkbox("Set Buff DEF %", &Config.bSetBuffDef);
      ImGui::SameLine(150);
      ImGui::InputFloat("##fBuffDef", &Config.fBuffDef);
      ImGui::Checkbox("Set Buff HP %", &Config.bSetBuffHP);
      ImGui::SameLine(150);
      ImGui::InputFloat("##fBuffHP", &Config.fBuffHP);
      ImGui::Checkbox("Set Buff DNeg %", &Config.bSetBuffDNeg);
      ImGui::SameLine(150);
      ImGui::InputFloat("##fBuffDNeg", &Config.fBuffDNeg);
      ImGui::Checkbox("Set Buff Gold %", &Config.bSetBuffGold);
      ImGui::SameLine(150);
      ImGui::InputFloat("##fBuffGold", &Config.fBuffGold);
      ImGui::Checkbox("Set Buff EXP %", &Config.bSetBuffEXP);
      ImGui::SameLine(150);
      ImGui::InputFloat("##fBuffEXP", &Config.fBuffEXP);
      ImGui::Checkbox("Set Buff Boss %", &Config.bSetBuffBoss);
      ImGui::SameLine(150);
      ImGui::InputFloat("##fBuffBoss", &Config.fBuffBoss);
      ImGui::Checkbox("Set Buff ELEM %", &Config.bSetBuffElem);
      ImGui::SameLine(150);
      ImGui::InputFloat("##fBuffElem", &Config.fBuffElem);
    }
  }  // namespace DetailedStats
}  // namespace Features
