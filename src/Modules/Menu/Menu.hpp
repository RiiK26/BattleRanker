#ifndef MENU_H
#define MENU_H

#include <windows.h>
#include <d3d12.h>
#include <dxgi1_4.h>
#include <imgui.h>
#include <imgui_impl_win32.h>
#include <imgui_impl_dx12.h>
#include <string>

namespace Menu
{
  struct ConfigData
  {
    bool bMasterGodMode         = false;
    bool bGodMode_Invincibility = false;
    bool bGodMode_MaxAtk        = false;
    bool bGodMode_MaxDef        = false;
    bool bGodMode_MaxHP         = false;

    // Upgrades (Player)
    bool bMaxDNeg             = false;
    bool bMaxCritical         = false;
    bool bMaxCritAtk          = false;
    bool bMaxDodge            = false;
    bool bMaxEXP              = false;
    bool bMaxGold             = false;
    bool bMaxBossDmg          = false;
    bool bMaxSkillCooltime    = false;
    bool bAuraKill            = false;
    bool bMaxOptionRoll       = false;
    int  iMaxOptionRollTier   = 5;
    bool bMaxGachaRolls       = false;
    bool bMaxElemAtk          = false;
    bool bMaxTreeElemAtk      = false;
    bool bMaxWaterElemAtk     = false;
    bool bMaxFireElemAtk      = false;
    bool bMaxLightElemAtk     = false;

    bool bInfiniteCurrency    = false;

    bool bRequestSetGold      = false;
    int  iSetGoldValue        = 999999999;
    bool bRequestSetCryptobit = false;
    int  iSetCryptobitValue   = 999999999;
    bool bRequestSetDiamond   = false;
    int  iSetDiamondValue     = 999999999;

    // New Currencies
    bool bRequestSetChanceTicket = false;
    int  iSetChanceTicketValue   = 999999999;
    bool bRequestSetPickupTicket = false;
    int  iSetPickupTicketValue   = 999999999;
    bool bRequestSetMileage      = false;
    int  iSetMileageValue        = 999999999;
    bool bRequestSetSkillTicket  = false;
    int  iSetSkillTicketValue    = 999999999;
    bool bRequestSetQuantumOrb   = false;
    int  iSetQuantumOrbValue     = 999999999;
    bool bRequestSetCandela      = false;
    int  iSetCandelaValue        = 999999999;
    bool bRequestSetSoulStone    = false;
    int  iSetSoulStoneValue      = 999999999;
    bool bRequestSetTreeCore     = false;
    int  iSetTreeCoreValue       = 999999999;
    bool bRequestSetFireCore     = false;
    int  iSetFireCoreValue       = 999999999;
    bool bRequestSetWaterCore    = false;
    int  iSetWaterCoreValue      = 999999999;
    bool bRequestSetLightCore    = false;
    int  iSetLightCoreValue      = 999999999;
    bool bRequestSetCP           = false;
    int  iSetCPValue             = 999999999;
    bool bRequestSetLumino       = false;
    int  iSetLuminoValue         = 999999999;
    bool bRequestSetQuantumCube  = false;
    int  iSetQuantumCubeValue    = 999999999;
    bool bRequestSetArenaTicket  = false;
    int  iSetArenaTicketValue    = 999999999;
    bool bRequestSetQuantumRing  = false;
    int  iSetQuantumRingValue    = 999999999;
    bool bRequestSetPolyFiber    = false;
    int  iSetPolyFiberValue      = 999999999;

    bool bRequestSetGoldenDice   = false;
    int  iSetGoldenDiceValue     = 999999999;
    bool bRequestSetLuckyDice    = false;
    int  iSetOptionDiceValue     = 999999999;
    bool bRequestSetSkillDice    = false;
    int  iSetSkillDiceValue      = 999999999;

    bool bRequestSetKeys         = false;
    int  iSetKeysValue           = 999999999;

    bool  bBypassACTk            = true;
    float fSpeedMultiplier       = 1.0f;
  };
  extern ConfigData Config;

  extern bool        g_ShowMenu;
  extern std::string g_LogMessage;

  bool init();
  void shutdown();
  void SaveConfig();
  void LoadConfig();
}  // namespace Menu

#endif  // MENU_H
