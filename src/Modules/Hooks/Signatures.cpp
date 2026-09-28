#include "Signatures.hpp"
#include "../Utils/MonoUtils.hpp"

namespace Signatures
{
  void* PlayerV3Script_GetHitV2                                   = nullptr;
  void* RPGPlayerData_get_goldAmount                              = nullptr;
  void* RPGPlayerData_set_goldAmount                              = nullptr;
  void* RPGPlayerData_get_RubyAmount                              = nullptr;
  void* RPGPlayerData_set_RubyAmount                              = nullptr;
  void* RPGPlayerData_get_JewelAmount                             = nullptr;
  void* RPGPlayerData_set_JewelAmount                             = nullptr;
  void* globalData_get_PickupSkillTicketCount                     = nullptr;
  void* globalData_set_PickupSkillTicketCount                     = nullptr;
  void* globalData_get_PremTicketCount                            = nullptr;
  void* globalData_set_PremTicketCount                            = nullptr;
  void* globalData_get_PickupTicketCount                          = nullptr;
  void* globalData_set_PickupTicketCount                          = nullptr;
  void* globalData_get_JewelMileage                               = nullptr;
  void* globalData_set_JewelMileage                               = nullptr;
  void* globalData_get_GoldenDiceCount                            = nullptr;
  void* globalData_set_GoldenDiceCount                            = nullptr;
  void* globalData_get_OptionDiceCount                            = nullptr;
  void* globalData_set_OptionDiceCount                            = nullptr;
  void* globalData_get_SkillDiceCount                             = nullptr;
  void* globalData_set_SkillDiceCount                             = nullptr;
  void* globalData_GetDunKeyCount                                 = nullptr;
  void* globalData_SetDunKeyCount                                 = nullptr;

  void* globalData_get_SkillTrancerCount                          = nullptr;
  void* globalData_set_SkillTrancerCount                          = nullptr;
  void* globalData_get_SkillEnhancerCount                         = nullptr;
  void* globalData_set_SkillEnhancerCount                         = nullptr;
  void* globalData_get_SubJMileage                                = nullptr;
  void* globalData_set_SubJMileage                                = nullptr;

  void* RPGPlayerData_get_TreeStoneAmount                         = nullptr;
  void* RPGPlayerData_set_TreeStoneAmount                         = nullptr;
  void* RPGPlayerData_get_FireStoneAmount                         = nullptr;
  void* RPGPlayerData_set_FireStoneAmount                         = nullptr;
  void* RPGPlayerData_get_WaterStoneAmount                        = nullptr;
  void* RPGPlayerData_set_WaterStoneAmount                        = nullptr;
  void* RPGPlayerData_get_LightStoneAmount                        = nullptr;
  void* RPGPlayerData_set_LightStoneAmount                        = nullptr;
  void* globalData_GetBaseATK                                     = nullptr;
  void* globalData_GetBaseDEF                                     = nullptr;
  void* globalData_GetBaseHP                                      = nullptr;

  void* globalData_GetLPTotalDNeg                                 = nullptr;
  void* globalData_GetLPTotalCritical                             = nullptr;
  void* globalData_GetLPTotalCritATK                              = nullptr;
  void* globalData_GetLPTotalDodge                                = nullptr;
  void* globalData_GetLPTotalBuffEXP                              = nullptr;
  void* globalData_GetLPTotalBuffGold                             = nullptr;
  void* globalData_GetLPTotalBossDmgBuff                          = nullptr;
  void* globalData_GetLPTotalElemAtkCommonBuff                    = nullptr;
  void* globalData_GetLPTotalElemAtk                              = nullptr;
  void* GDOptionScript_GetLPAllOpPower                            = nullptr;

  void* RPGPlayerData_get_currentHP                               = nullptr;
  void* RPGPlayerData_set_currentHP                               = nullptr;
  void* ACTk_InjectionDetector_StartDetectionAutomatically        = nullptr;
  void* ACTk_ObscuredCheatingDetector_StartDetectionAutomatically = nullptr;
  void* ACTk_SpeedHackDetector_StartDetectionAutomatically        = nullptr;
  void* ACTk_TimeCheatingDetector_StartDetectionAutomatically     = nullptr;
  void* ACTk_WallHackDetector_StartDetectionAutomatically         = nullptr;

  void Resolve()
  {
    PlayerV3Script_GetHitV2       = mono::get_method("Assembly-CSharp", "", "PlayerV3Script", "GetHitV2", 3);
    RPGPlayerData_get_goldAmount  = mono::get_method("Assembly-CSharp", "", "RPGPlayerData", "get_goldAmount", 0);
    RPGPlayerData_set_goldAmount  = mono::get_method("Assembly-CSharp", "", "RPGPlayerData", "set_goldAmount", 1);
    RPGPlayerData_get_RubyAmount  = mono::get_method("Assembly-CSharp", "", "RPGPlayerData", "get_RubyAmount", 0);
    RPGPlayerData_set_RubyAmount  = mono::get_method("Assembly-CSharp", "", "RPGPlayerData", "set_RubyAmount", 1);
    RPGPlayerData_get_JewelAmount = mono::get_method("Assembly-CSharp", "", "RPGPlayerData", "get_JewelAmount", 0);
    RPGPlayerData_set_JewelAmount = mono::get_method("Assembly-CSharp", "", "RPGPlayerData", "set_JewelAmount", 1);
    globalData_get_PickupSkillTicketCount =
      mono::get_method("Assembly-CSharp", "", "globalData", "get_PickupSkillTicketCount", 0);
    globalData_set_PickupSkillTicketCount =
      mono::get_method("Assembly-CSharp", "", "globalData", "set_PickupSkillTicketCount", 1);
    globalData_get_PremTicketCount = mono::get_method("Assembly-CSharp", "", "globalData", "get_PremTicketCount", 0);
    globalData_set_PremTicketCount = mono::get_method("Assembly-CSharp", "", "globalData", "set_PremTicketCount", 1);
    globalData_get_PickupTicketCount =
      mono::get_method("Assembly-CSharp", "", "globalData", "get_PickupTicketCount", 0);
    globalData_set_PickupTicketCount =
      mono::get_method("Assembly-CSharp", "", "globalData", "set_PickupTicketCount", 1);
    globalData_get_JewelMileage    = mono::get_method("Assembly-CSharp", "", "globalData", "get_JewelMileage", 0);
    globalData_set_JewelMileage    = mono::get_method("Assembly-CSharp", "", "globalData", "set_JewelMileage", 1);
    globalData_get_GoldenDiceCount = mono::get_method("Assembly-CSharp", "", "globalData", "get_GoldenDiceCount", 0);
    globalData_set_GoldenDiceCount = mono::get_method("Assembly-CSharp", "", "globalData", "set_GoldenDiceCount", 1);
    globalData_get_OptionDiceCount = mono::get_method("Assembly-CSharp", "", "globalData", "get_OptionDiceCount", 0);
    globalData_set_OptionDiceCount = mono::get_method("Assembly-CSharp", "", "globalData", "set_OptionDiceCount", 1);
    globalData_get_SkillDiceCount  = mono::get_method("Assembly-CSharp", "", "globalData", "get_SkillDiceCount", 0);
    globalData_set_SkillDiceCount  = mono::get_method("Assembly-CSharp", "", "globalData", "set_SkillDiceCount", 1);
    globalData_GetDunKeyCount      = mono::get_method("Assembly-CSharp", "", "globalData", "GetDunKeyCount", 1);
    globalData_SetDunKeyCount      = mono::get_method("Assembly-CSharp", "", "globalData", "SetDunKeyCount", 2);

    globalData_get_SkillTrancerCount =
      mono::get_method("Assembly-CSharp", "", "globalData", "get_SkillTrancerCount", 0);
    globalData_set_SkillTrancerCount =
      mono::get_method("Assembly-CSharp", "", "globalData", "set_SkillTrancerCount", 1);
    globalData_get_SkillEnhancerCount =
      mono::get_method("Assembly-CSharp", "", "globalData", "get_SkillEnhancerCount", 0);
    globalData_set_SkillEnhancerCount =
      mono::get_method("Assembly-CSharp", "", "globalData", "set_SkillEnhancerCount", 1);
    globalData_get_SubJMileage = mono::get_method("Assembly-CSharp", "", "globalData", "get_SubJMileage", 0);
    globalData_set_SubJMileage = mono::get_method("Assembly-CSharp", "", "globalData", "set_SubJMileage", 1);

    RPGPlayerData_get_TreeStoneAmount =
      mono::get_method("Assembly-CSharp", "", "RPGPlayerData", "get_TreeStoneAmount", 0);
    RPGPlayerData_set_TreeStoneAmount =
      mono::get_method("Assembly-CSharp", "", "RPGPlayerData", "set_TreeStoneAmount", 1);
    RPGPlayerData_get_FireStoneAmount =
      mono::get_method("Assembly-CSharp", "", "RPGPlayerData", "get_FireStoneAmount", 0);
    RPGPlayerData_set_FireStoneAmount =
      mono::get_method("Assembly-CSharp", "", "RPGPlayerData", "set_FireStoneAmount", 1);
    RPGPlayerData_get_WaterStoneAmount =
      mono::get_method("Assembly-CSharp", "", "RPGPlayerData", "get_WaterStoneAmount", 0);
    RPGPlayerData_set_WaterStoneAmount =
      mono::get_method("Assembly-CSharp", "", "RPGPlayerData", "set_WaterStoneAmount", 1);
    RPGPlayerData_get_LightStoneAmount =
      mono::get_method("Assembly-CSharp", "", "RPGPlayerData", "get_LightStoneAmount", 0);
    RPGPlayerData_set_LightStoneAmount =
      mono::get_method("Assembly-CSharp", "", "RPGPlayerData", "set_LightStoneAmount", 1);
    globalData_GetBaseATK         = mono::get_method("Assembly-CSharp", "", "globalData", "GetBaseATK", 1);
    globalData_GetBaseDEF         = mono::get_method("Assembly-CSharp", "", "globalData", "GetBaseDEF", 1);
    globalData_GetBaseHP          = mono::get_method("Assembly-CSharp", "", "globalData", "GetBaseHP", 1);

    globalData_GetLPTotalDNeg     = mono::get_method("Assembly-CSharp", "", "globalData", "GetLPTotalDNeg", 0);
    globalData_GetLPTotalCritical = mono::get_method("Assembly-CSharp", "", "globalData", "GetLPTotalCritical", 0);
    globalData_GetLPTotalCritATK  = mono::get_method("Assembly-CSharp", "", "globalData", "GetLPTotalCritATK", 0);
    globalData_GetLPTotalDodge    = mono::get_method("Assembly-CSharp", "", "globalData", "GetLPTotalDodge", 0);
    globalData_GetLPTotalBuffEXP  = mono::get_method("Assembly-CSharp", "", "globalData", "GetLPTotalBuffEXP", 0);
    globalData_GetLPTotalBuffGold = mono::get_method("Assembly-CSharp", "", "globalData", "GetLPTotalBuffGold", 0);
    globalData_GetLPTotalBossDmgBuff =
      mono::get_method("Assembly-CSharp", "", "globalData", "GetLPTotalBossDmgBuff", 0);
    globalData_GetLPTotalElemAtkCommonBuff =
      mono::get_method("Assembly-CSharp", "", "globalData", "GetLPTotalElemAtkCommonBuff", 0);
    globalData_GetLPTotalElemAtk   = mono::get_method("Assembly-CSharp", "", "globalData", "GetLPTotalElemAtk", 1);
    GDOptionScript_GetLPAllOpPower = mono::get_method("Assembly-CSharp", "", "GDOptionScript", "GetLPAllOpPower", 1);

    RPGPlayerData_get_currentHP    = mono::get_method("Assembly-CSharp", "", "RPGPlayerData", "get_currentHP", 0);
    RPGPlayerData_set_currentHP    = mono::get_method("Assembly-CSharp", "", "RPGPlayerData", "set_currentHP", 1);

    const char* actk_ns            = "CodeStage.AntiCheat.Detectors";
    const char* actk_dll           = "ACTk.Runtime";
    const char* method             = "StartDetectionAutomatically";

    ACTk_InjectionDetector_StartDetectionAutomatically =
      mono::get_method(actk_dll, actk_ns, "InjectionDetector", method, 0);
    ACTk_ObscuredCheatingDetector_StartDetectionAutomatically =
      mono::get_method(actk_dll, actk_ns, "ObscuredCheatingDetector", method, 0);
    ACTk_SpeedHackDetector_StartDetectionAutomatically =
      mono::get_method(actk_dll, actk_ns, "SpeedHackDetector", method, 0);
    ACTk_TimeCheatingDetector_StartDetectionAutomatically =
      mono::get_method(actk_dll, actk_ns, "TimeCheatingDetector", method, 0);
    ACTk_WallHackDetector_StartDetectionAutomatically =
      mono::get_method(actk_dll, actk_ns, "WallHackDetector", method, 0);
  }
}  // namespace Signatures
