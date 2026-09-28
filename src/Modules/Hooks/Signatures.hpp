#ifndef SIGNATURES_H
#define SIGNATURES_H

namespace Signatures
{
  extern void* PlayerV3Script_GetHitV2;
  extern void* RPGPlayerData_get_goldAmount;
  extern void* RPGPlayerData_set_goldAmount;
  extern void* RPGPlayerData_get_RubyAmount;
  extern void* RPGPlayerData_set_RubyAmount;
  extern void* RPGPlayerData_get_JewelAmount;
  extern void* RPGPlayerData_set_JewelAmount;

  extern void* globalData_get_PickupSkillTicketCount;
  extern void* globalData_set_PickupSkillTicketCount;
  extern void* globalData_get_PremTicketCount;
  extern void* globalData_set_PremTicketCount;
  extern void* globalData_get_PickupTicketCount;
  extern void* globalData_set_PickupTicketCount;
  extern void* globalData_get_JewelMileage;
  extern void* globalData_set_JewelMileage;
  extern void* globalData_get_GoldenDiceCount;
  extern void* globalData_set_GoldenDiceCount;
  extern void* globalData_get_OptionDiceCount;
  extern void* globalData_set_OptionDiceCount;
  extern void* globalData_get_SkillDiceCount;
  extern void* globalData_set_SkillDiceCount;
  extern void* globalData_GetDunKeyCount;
  extern void* globalData_SetDunKeyCount;

  extern void* globalData_get_SkillTrancerCount;   // Quantum Orb
  extern void* globalData_set_SkillTrancerCount;
  extern void* globalData_get_SkillEnhancerCount;  // Candela
  extern void* globalData_set_SkillEnhancerCount;
  extern void* globalData_get_SubJMileage;         // CP
  extern void* globalData_set_SubJMileage;

  extern void* globalData_get_EnhanceStoneCount;  // Lumino
  extern void* globalData_set_EnhanceStoneCount;
  extern void* globalData_get_TransStoneCount;    // Quantum Cube
  extern void* globalData_set_TransStoneCount;

  extern void* GDPVPScript_get_PvpTicket;              // Arena Ticket
  extern void* GDPVPScript_set_PvpTicket;
  extern void* globalData_get_AvataSkinTrancerCount;   // Quantum Ring
  extern void* globalData_set_AvataSkinTrancerCount;
  extern void* globalData_get_AvataSkinEnhancerCount;  // Poly Fiber
  extern void* globalData_set_AvataSkinEnhancerCount;

  extern void* RPGPlayerData_get_TreeStoneAmount;
  extern void* RPGPlayerData_set_TreeStoneAmount;
  extern void* RPGPlayerData_get_FireStoneAmount;
  extern void* RPGPlayerData_set_FireStoneAmount;
  extern void* RPGPlayerData_get_WaterStoneAmount;
  extern void* RPGPlayerData_set_WaterStoneAmount;
  extern void* RPGPlayerData_get_LightStoneAmount;
  extern void* RPGPlayerData_set_LightStoneAmount;
  extern void* globalData_GetBaseATK;
  extern void* globalData_GetBaseDEF;
  extern void* globalData_GetBaseHP;

  // Upgrade functions
  extern void* globalData_GetLPTotalDNeg;
  extern void* globalData_GetLPTotalCritical;
  extern void* globalData_GetLPTotalCritATK;
  extern void* globalData_GetLPTotalDodge;
  extern void* globalData_GetLPTotalBuffEXP;
  extern void* globalData_GetLPTotalBuffGold;
  extern void* globalData_GetLPTotalBossDmgBuff;
  extern void* globalData_GetLPTotalElemAtkCommonBuff;
  extern void* globalData_GetLPTotalElemAtk;
  extern void* GDOptionScript_GetLPAllOpPower;

  extern void* RPGPlayerData_get_currentHP;
  extern void* RPGPlayerData_set_currentHP;
  extern void* ACTk_InjectionDetector_StartDetectionAutomatically;
  extern void* ACTk_ObscuredCheatingDetector_StartDetectionAutomatically;
  extern void* ACTk_SpeedHackDetector_StartDetectionAutomatically;
  extern void* ACTk_TimeCheatingDetector_StartDetectionAutomatically;
  extern void* ACTk_WallHackDetector_StartDetectionAutomatically;

  extern void* MonsterV2Script_Update;
  extern void* MonsterV2Script_InstantDeath;
  extern void* MonsterV2Script_get_IsDead;
  extern void* MonsterV2Script_get_IsPuppet;
  extern void* MonsterV2Script_get_IsTargetable;

  extern void* GDOptionScript_PullOneOption;
  extern void* RPGOptionItem_set_OptionGrade;

  extern void* GearBoxScript_GetRandomPickLv;
  extern void* GearBoxScript_GetActualSkillGrade;
  extern void* GearBoxScript_SubActualSkinPull;

  void Resolve();
}  // namespace Signatures

#endif  // SIGNATURES_H
