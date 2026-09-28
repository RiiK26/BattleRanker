#ifndef DETAILED_STATS_HPP
#define DETAILED_STATS_HPP

namespace Features
{
  namespace DetailedStats
  {
    struct ConfigData
    {
      // Base Stats
      bool  bSetBaseAtk = false;
      float fBaseAtk    = 999999999.0f;
      bool  bSetBaseDef = false;
      float fBaseDef    = 999999999.0f;
      bool  bSetBaseHP  = false;
      float fBaseHP     = 999999999.0f;

      // Upgrade Stats (Bonus)
      bool  bSetUpAtk      = false;
      float fUpAtk         = 999999999.0f;
      bool  bSetUpDef      = false;
      float fUpDef         = 999999999.0f;
      bool  bSetUpHP       = false;
      float fUpHP          = 999999999.0f;
      bool  bSetUpDNeg     = false;
      float fUpDNeg        = 99999.0f;
      bool  bSetUpCrit     = false;
      float fUpCrit        = 100.0f;
      bool  bSetUpCritAtk  = false;
      float fUpCritAtk     = 99999.0f;
      bool  bSetUpDodge    = false;
      float fUpDodge       = 100.0f;
      bool  bSetUpTreeAtk  = false;
      float fUpTreeAtk     = 99999.0f;
      bool  bSetUpWaterAtk = false;
      float fUpWaterAtk    = 99999.0f;
      bool  bSetUpFireAtk  = false;
      float fUpFireAtk     = 99999.0f;
      bool  bSetUpLightAtk = false;
      float fUpLightAtk    = 99999.0f;

      // Gear / Acc / Pet
      bool  bSetGearAtk = false;
      float fGearAtk    = 999999999.0f;
      bool  bSetGearDef = false;
      float fGearDef    = 999999999.0f;
      bool  bSetAccAtk  = false;
      float fAccAtk     = 999999999.0f;
      bool  bSetAccDef  = false;
      float fAccDef     = 999999999.0f;
      bool  bSetPetDef  = false;
      float fPetDef     = 999999999.0f;
      bool  bSetPetHP   = false;
      float fPetHP      = 999999999.0f;

      // Implants
      bool  bSetImpTreeDef    = false;
      float fImpTreeDef       = 999999.0f;
      bool  bSetImpFireHP     = false;
      float fImpFireHP        = 999999999.0f;
      bool  bSetImpWaterDNeg  = false;
      float fImpWaterDNeg     = 99999.0f;
      bool  bSetImpLightCrAtk = false;
      float fImpLightCrAtk    = 99999.0f;

      // Global Buffs (%)
      bool  bSetBuffAtk  = false;
      float fBuffAtk     = 999.0f;
      bool  bSetBuffDef  = false;
      float fBuffDef     = 999.0f;
      bool  bSetBuffHP   = false;
      float fBuffHP      = 999.0f;
      bool  bSetBuffDNeg = false;
      float fBuffDNeg    = 999.0f;
      bool  bSetBuffGold = false;
      float fBuffGold    = 999.0f;
      bool  bSetBuffEXP  = false;
      float fBuffEXP     = 999.0f;
      bool  bSetBuffBoss = false;
      float fBuffBoss    = 999.0f;
      bool  bSetBuffElem = false;
      float fBuffElem    = 999.0f;

      // Option Buffs (Hooking GDOptionScript)
      bool  bSetOpGear    = false;
      float fOpGear       = 999.0f;
      bool  bSetOpChar    = false;
      float fOpChar       = 999.0f;
      bool  bSetOpPet     = false;
      float fOpPet        = 999.0f;
      bool  bSetOpAcc     = false;
      float fOpAcc        = 999.0f;
      bool  bSetOpSkin    = false;
      float fOpSkin       = 999.0f;
      bool  bSetOpSkinSet = false;
      float fOpSkinSet    = 999.0f;
      bool  bSetOpSkill   = false;
      float fOpSkill      = 999.0f;

      // Skin Set ATK/DEF/ELEM/DNEG total
      bool  bSetSkinAtkBo    = false;
      float fSkinAtkBo       = 999.0f;
      bool  bSetSkinDefBo    = false;
      float fSkinDefBo       = 999.0f;
      bool  bSetSkinElemBo   = false;
      float fSkinElemBo      = 999.0f;
      bool  bSetSkinDNegBo   = false;
      float fSkinDNegBo      = 999.0f;
      bool  bSetSkinPatchAtk = false;
      float fSkinPatchAtk    = 999999999.0f;
    };
    extern ConfigData Config;

    void Initialize();
    void Uninitialize();
    void RenderUI();
  }  // namespace DetailedStats
}  // namespace Features

#endif  // DETAILED_STATS_HPP
