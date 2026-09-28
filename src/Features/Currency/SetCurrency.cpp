#include "SetCurrency.hpp"
#include <cstdint>
#include "../../Modules/Hooks/Hooks.hpp"
#include "../../Modules/Hooks/Signatures.hpp"
#include "../../Modules/Menu/Menu.hpp"
#include "../../Modules/Utils/MonoUtils.hpp"

namespace Features
{
  namespace SetCurrency
  {
    void* g_globalData    = nullptr;
    void* g_RPGPlayerData = nullptr;
    void* g_GDPVPScript   = nullptr;

    int64_t (*Orig_RPGPlayerData_get_goldAmount)(void* this_ptr);
    int64_t Hook_RPGPlayerData_get_goldAmount(void* this_ptr)
    {
      g_RPGPlayerData = this_ptr;
      return Orig_RPGPlayerData_get_goldAmount(this_ptr);
    }

    int (*Orig_globalData_get_PickupSkillTicketCount)(void* this_ptr);
    int Hook_globalData_get_PickupSkillTicketCount(void* this_ptr)
    {
      g_globalData = this_ptr;
      ProcessRequests();
      return Orig_globalData_get_PickupSkillTicketCount(this_ptr);
    }

    int (*Orig_GDPVPScript_get_PvpTicket)(void* this_ptr);
    int Hook_GDPVPScript_get_PvpTicket(void* this_ptr)
    {
      g_GDPVPScript = this_ptr;
      return Orig_GDPVPScript_get_PvpTicket(this_ptr);
    }

    void ProcessRequests()
    {
      if (!g_globalData && !g_RPGPlayerData)
        return;

      bool inf                       = Menu::Config.bInfiniteCurrency;
      Menu::Config.bInfiniteCurrency = false;

      typedef void (*FuncInt)(void*, int);
      typedef void (*FuncInt64)(void*, int64_t);

      if (g_RPGPlayerData) {
        if (Menu::Config.bRequestSetGold) {
          Menu::Config.bRequestSetGold = false;
          ((FuncInt64) Signatures::RPGPlayerData_set_goldAmount)(g_RPGPlayerData, Menu::Config.iSetGoldValue);
        }
        if (Menu::Config.bRequestSetCryptobit) {
          Menu::Config.bRequestSetCryptobit = false;
          ((FuncInt) Signatures::RPGPlayerData_set_RubyAmount)(g_RPGPlayerData, Menu::Config.iSetCryptobitValue);
        }
        if (Menu::Config.bRequestSetDiamond) {
          Menu::Config.bRequestSetDiamond = false;
          ((FuncInt) Signatures::RPGPlayerData_set_JewelAmount)(g_RPGPlayerData, Menu::Config.iSetDiamondValue);
        }
        if (Menu::Config.bRequestSetTreeCore) {
          Menu::Config.bRequestSetTreeCore = false;
          ((FuncInt64) Signatures::RPGPlayerData_set_TreeStoneAmount)(g_RPGPlayerData, Menu::Config.iSetTreeCoreValue);
        }
        if (Menu::Config.bRequestSetFireCore) {
          Menu::Config.bRequestSetFireCore = false;
          ((FuncInt64) Signatures::RPGPlayerData_set_FireStoneAmount)(g_RPGPlayerData, Menu::Config.iSetFireCoreValue);
        }
        if (Menu::Config.bRequestSetWaterCore) {
          Menu::Config.bRequestSetWaterCore = false;
          ((FuncInt64) Signatures::RPGPlayerData_set_WaterStoneAmount)(
            g_RPGPlayerData, Menu::Config.iSetWaterCoreValue
          );
        }
        if (Menu::Config.bRequestSetLightCore) {
          Menu::Config.bRequestSetLightCore = false;
          ((FuncInt64) Signatures::RPGPlayerData_set_LightStoneAmount)(
            g_RPGPlayerData, Menu::Config.iSetLightCoreValue
          );
        }
      }

      if (g_globalData) {
        if (Menu::Config.bRequestSetChanceTicket) {
          Menu::Config.bRequestSetChanceTicket = false;
          ((FuncInt) Signatures::globalData_set_PremTicketCount)(g_globalData, Menu::Config.iSetChanceTicketValue);
        }
        if (Menu::Config.bRequestSetPickupTicket) {
          Menu::Config.bRequestSetPickupTicket = false;
          ((FuncInt) Signatures::globalData_set_PickupTicketCount)(g_globalData, Menu::Config.iSetPickupTicketValue);
        }
        if (Menu::Config.bRequestSetMileage) {
          Menu::Config.bRequestSetMileage = false;
          ((FuncInt) Signatures::globalData_set_JewelMileage)(g_globalData, Menu::Config.iSetMileageValue);
        }
        if (Menu::Config.bRequestSetSkillTicket) {
          Menu::Config.bRequestSetSkillTicket = false;
          ((FuncInt) Signatures::globalData_set_PickupSkillTicketCount)(
            g_globalData, Menu::Config.iSetSkillTicketValue
          );
        }
        if (Menu::Config.bRequestSetQuantumOrb) {
          Menu::Config.bRequestSetQuantumOrb = false;
          ((FuncInt) Signatures::globalData_set_SkillTrancerCount)(g_globalData, Menu::Config.iSetQuantumOrbValue);
        }
        if (Menu::Config.bRequestSetCandela) {
          Menu::Config.bRequestSetCandela = false;
          ((FuncInt) Signatures::globalData_set_SkillEnhancerCount)(g_globalData, Menu::Config.iSetCandelaValue);
        }
        if (Menu::Config.bRequestSetCP) {
          Menu::Config.bRequestSetCP = false;
          ((FuncInt) Signatures::globalData_set_SubJMileage)(g_globalData, Menu::Config.iSetCPValue);
        }
        if (Menu::Config.bRequestSetGoldenDice) {
          Menu::Config.bRequestSetGoldenDice = false;
          ((FuncInt) Signatures::globalData_set_GoldenDiceCount)(g_globalData, Menu::Config.iSetGoldenDiceValue);
        }
        if (Menu::Config.bRequestSetLuckyDice) {  // Option Dice internally
          Menu::Config.bRequestSetLuckyDice = false;
          ((FuncInt) Signatures::globalData_set_OptionDiceCount)(g_globalData, Menu::Config.iSetOptionDiceValue);
        }
        if (Menu::Config.bRequestSetSkillDice) {
          Menu::Config.bRequestSetSkillDice = false;
          ((FuncInt) Signatures::globalData_set_SkillDiceCount)(g_globalData, Menu::Config.iSetSkillDiceValue);
        }
        if (Menu::Config.bRequestSetLumino) {
          Menu::Config.bRequestSetLumino = false;
          ((FuncInt) Signatures::globalData_set_EnhanceStoneCount)(g_globalData, Menu::Config.iSetLuminoValue);
        }
        if (Menu::Config.bRequestSetQuantumCube) {
          Menu::Config.bRequestSetQuantumCube = false;
          ((FuncInt) Signatures::globalData_set_TransStoneCount)(g_globalData, Menu::Config.iSetQuantumCubeValue);
        }
        if (Menu::Config.bRequestSetQuantumRing) {
          Menu::Config.bRequestSetQuantumRing = false;
          ((FuncInt) Signatures::globalData_set_AvataSkinTrancerCount)(g_globalData, Menu::Config.iSetQuantumRingValue);
        }
        if (Menu::Config.bRequestSetPolyFiber) {
          Menu::Config.bRequestSetPolyFiber = false;
          ((FuncInt) Signatures::globalData_set_AvataSkinEnhancerCount)(g_globalData, Menu::Config.iSetPolyFiberValue);
        }
        if (Menu::Config.bRequestSetSoulStone) {
          Menu::Config.bRequestSetSoulStone = false;
          typedef void (*FuncSetSoulStone)(void*, int);
          FuncSetSoulStone SetSoulStone =
            (FuncSetSoulStone) mono::get_method("Assembly-CSharp", "", "globalData", "set_SoulStoneCount", 1);
          if (SetSoulStone)
            SetSoulStone(g_globalData, Menu::Config.iSetSoulStoneValue);
        }
        if (Menu::Config.bRequestSetKeys) {
          Menu::Config.bRequestSetKeys = false;
          typedef void (*FuncSetKey)(void*, int, int);
          FuncSetKey setKey = (FuncSetKey) Signatures::globalData_SetDunKeyCount;
          for (int i = 0; i < 20; i++) {
            setKey(g_globalData, i, Menu::Config.iSetKeysValue);
          }
        }
      }

      if (g_GDPVPScript) {
        if (Menu::Config.bRequestSetArenaTicket) {
          Menu::Config.bRequestSetArenaTicket = false;
          ((FuncInt) Signatures::GDPVPScript_set_PvpTicket)(g_GDPVPScript, Menu::Config.iSetArenaTicketValue);
        }
      }

      Menu::Config.bInfiniteCurrency = inf;
    }

    void Initialize()
    {
      HOOK_SIGNATURE(
        "RPGPlayerData::get_goldAmount", Signatures::RPGPlayerData_get_goldAmount, Hook_RPGPlayerData_get_goldAmount,
        Orig_RPGPlayerData_get_goldAmount
      );
      HOOK_SIGNATURE(
        "globalData::get_PickupSkillTicketCount", Signatures::globalData_get_PickupSkillTicketCount,
        Hook_globalData_get_PickupSkillTicketCount, Orig_globalData_get_PickupSkillTicketCount
      );
      HOOK_SIGNATURE(
        "GDPVPScript::get_PvpTicket", Signatures::GDPVPScript_get_PvpTicket, Hook_GDPVPScript_get_PvpTicket,
        Orig_GDPVPScript_get_PvpTicket
      );
    }

    void Uninitialize() { }
  }  // namespace SetCurrency
}  // namespace Features
