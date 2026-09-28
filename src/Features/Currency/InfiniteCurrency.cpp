#include "InfiniteCurrency.hpp"
#include <cstdint>
#include "../../Modules/Hooks/Hooks.hpp"
#include "../../Modules/Hooks/Signatures.hpp"
#include "../../Modules/Menu/Menu.hpp"

namespace Features
{
  namespace InfiniteCurrency
  {
    int64_t (*Orig_RPGPlayerData_get_goldAmount)(void* this_ptr);
    void (*Orig_RPGPlayerData_set_goldAmount)(void* this_ptr, int64_t value);
    void Hook_RPGPlayerData_set_goldAmount(void* this_ptr, int64_t value)
    {
      if (Menu::Config.bInfiniteCurrency && Orig_RPGPlayerData_get_goldAmount) {
        int64_t current = Orig_RPGPlayerData_get_goldAmount(this_ptr);
        if (value < current)
          return;
      }
      Orig_RPGPlayerData_set_goldAmount(this_ptr, value);
    }

    int (*Orig_RPGPlayerData_get_RubyAmount)(void* this_ptr);
    void (*Orig_RPGPlayerData_set_RubyAmount)(void* this_ptr, int value);
    void Hook_RPGPlayerData_set_RubyAmount(void* this_ptr, int value)
    {
      if (Menu::Config.bInfiniteCurrency && Orig_RPGPlayerData_get_RubyAmount) {
        int current = Orig_RPGPlayerData_get_RubyAmount(this_ptr);
        if (value < current)
          return;
      }
      Orig_RPGPlayerData_set_RubyAmount(this_ptr, value);
    }

    int (*Orig_RPGPlayerData_get_JewelAmount)(void* this_ptr);
    void (*Orig_RPGPlayerData_set_JewelAmount)(void* this_ptr, int value);
    void Hook_RPGPlayerData_set_JewelAmount(void* this_ptr, int value)
    {
      if (Menu::Config.bInfiniteCurrency && Orig_RPGPlayerData_get_JewelAmount) {
        int current = Orig_RPGPlayerData_get_JewelAmount(this_ptr);
        if (value < current)
          return;
      }
      Orig_RPGPlayerData_set_JewelAmount(this_ptr, value);
    }

    int (*Orig_globalData_get_PickupSkillTicketCount)(void* this_ptr);
    void (*Orig_globalData_set_PickupSkillTicketCount)(void* this_ptr, int value);
    void Hook_globalData_set_PickupSkillTicketCount(void* this_ptr, int value)
    {
      if (Menu::Config.bInfiniteCurrency && Orig_globalData_get_PickupSkillTicketCount) {
        int current = Orig_globalData_get_PickupSkillTicketCount(this_ptr);
        if (value < current)
          return;
      }
      Orig_globalData_set_PickupSkillTicketCount(this_ptr, value);
    }

    int (*Orig_globalData_get_PremTicketCount)(void* this_ptr);
    void (*Orig_globalData_set_PremTicketCount)(void* this_ptr, int value);
    void Hook_globalData_set_PremTicketCount(void* this_ptr, int value)
    {
      if (Menu::Config.bInfiniteCurrency && Orig_globalData_get_PremTicketCount) {
        int current = Orig_globalData_get_PremTicketCount(this_ptr);
        if (value < current)
          return;
      }
      Orig_globalData_set_PremTicketCount(this_ptr, value);
    }

    int (*Orig_globalData_get_PickupTicketCount)(void* this_ptr);
    void (*Orig_globalData_set_PickupTicketCount)(void* this_ptr, int value);
    void Hook_globalData_set_PickupTicketCount(void* this_ptr, int value)
    {
      if (Menu::Config.bInfiniteCurrency && Orig_globalData_get_PickupTicketCount) {
        int current = Orig_globalData_get_PickupTicketCount(this_ptr);
        if (value < current)
          return;
      }
      Orig_globalData_set_PickupTicketCount(this_ptr, value);
    }

    int (*Orig_globalData_get_JewelMileage)(void* this_ptr);
    void (*Orig_globalData_set_JewelMileage)(void* this_ptr, int value);
    void Hook_globalData_set_JewelMileage(void* this_ptr, int value)
    {
      if (Menu::Config.bInfiniteCurrency && Orig_globalData_get_JewelMileage) {
        int current = Orig_globalData_get_JewelMileage(this_ptr);
        if (value < current)
          return;
      }
      Orig_globalData_set_JewelMileage(this_ptr, value);
    }

    int (*Orig_globalData_get_GoldenDiceCount)(void* this_ptr);
    void (*Orig_globalData_set_GoldenDiceCount)(void* this_ptr, int value);
    void Hook_globalData_set_GoldenDiceCount(void* this_ptr, int value)
    {
      if (Menu::Config.bInfiniteCurrency && Orig_globalData_get_GoldenDiceCount) {
        int current = Orig_globalData_get_GoldenDiceCount(this_ptr);
        if (value < current)
          return;
      }
      Orig_globalData_set_GoldenDiceCount(this_ptr, value);
    }

    int (*Orig_globalData_get_OptionDiceCount)(void* this_ptr);
    void (*Orig_globalData_set_OptionDiceCount)(void* this_ptr, int value);
    void Hook_globalData_set_OptionDiceCount(void* this_ptr, int value)
    {
      if (Menu::Config.bInfiniteCurrency && Orig_globalData_get_OptionDiceCount) {
        int current = Orig_globalData_get_OptionDiceCount(this_ptr);
        if (value < current)
          return;
      }
      Orig_globalData_set_OptionDiceCount(this_ptr, value);
    }

    int (*Orig_globalData_get_SkillDiceCount)(void* this_ptr);
    void (*Orig_globalData_set_SkillDiceCount)(void* this_ptr, int value);
    void Hook_globalData_set_SkillDiceCount(void* this_ptr, int value)
    {
      if (Menu::Config.bInfiniteCurrency && Orig_globalData_get_SkillDiceCount) {
        int current = Orig_globalData_get_SkillDiceCount(this_ptr);
        if (value < current)
          return;
      }
      Orig_globalData_set_SkillDiceCount(this_ptr, value);
    }

    int (*Orig_globalData_get_SkillTrancerCount)(void* this_ptr);
    void (*Orig_globalData_set_SkillTrancerCount)(void* this_ptr, int value);
    void Hook_globalData_set_SkillTrancerCount(void* this_ptr, int value)
    {
      if (Menu::Config.bInfiniteCurrency && Orig_globalData_get_SkillTrancerCount) {
        int current = Orig_globalData_get_SkillTrancerCount(this_ptr);
        if (value < current)
          return;
      }
      Orig_globalData_set_SkillTrancerCount(this_ptr, value);
    }

    int (*Orig_globalData_get_SkillEnhancerCount)(void* this_ptr);
    void (*Orig_globalData_set_SkillEnhancerCount)(void* this_ptr, int value);
    void Hook_globalData_set_SkillEnhancerCount(void* this_ptr, int value)
    {
      if (Menu::Config.bInfiniteCurrency && Orig_globalData_get_SkillEnhancerCount) {
        int current = Orig_globalData_get_SkillEnhancerCount(this_ptr);
        if (value < current)
          return;
      }
      Orig_globalData_set_SkillEnhancerCount(this_ptr, value);
    }

    int (*Orig_globalData_get_SubJMileage)(void* this_ptr);
    void (*Orig_globalData_set_SubJMileage)(void* this_ptr, int value);
    void Hook_globalData_set_SubJMileage(void* this_ptr, int value)
    {
      if (Menu::Config.bInfiniteCurrency && Orig_globalData_get_SubJMileage) {
        int current = Orig_globalData_get_SubJMileage(this_ptr);
        if (value < current)
          return;
      }
      Orig_globalData_set_SubJMileage(this_ptr, value);
    }

    int (*Orig_globalData_GetDunKeyCount)(void* this_ptr, int pIdx);
    void (*Orig_globalData_SetDunKeyCount)(void* this_ptr, int pIdx, int pValue);
    void Hook_globalData_SetDunKeyCount(void* this_ptr, int pIdx, int pValue)
    {
      if (Menu::Config.bInfiniteCurrency && Orig_globalData_GetDunKeyCount) {
        int current = Orig_globalData_GetDunKeyCount(this_ptr, pIdx);
        if (pValue < current)
          return;
      }
      Orig_globalData_SetDunKeyCount(this_ptr, pIdx, pValue);
    }

    int64_t (*Orig_RPGPlayerData_get_TreeStoneAmount)(void* this_ptr);
    void (*Orig_RPGPlayerData_set_TreeStoneAmount)(void* this_ptr, int64_t value);
    void Hook_RPGPlayerData_set_TreeStoneAmount(void* this_ptr, int64_t value)
    {
      if (Menu::Config.bInfiniteCurrency && Orig_RPGPlayerData_get_TreeStoneAmount) {
        int64_t current = Orig_RPGPlayerData_get_TreeStoneAmount(this_ptr);
        if (value < current)
          return;
      }
      Orig_RPGPlayerData_set_TreeStoneAmount(this_ptr, value);
    }

    int64_t (*Orig_RPGPlayerData_get_FireStoneAmount)(void* this_ptr);
    void (*Orig_RPGPlayerData_set_FireStoneAmount)(void* this_ptr, int64_t value);
    void Hook_RPGPlayerData_set_FireStoneAmount(void* this_ptr, int64_t value)
    {
      if (Menu::Config.bInfiniteCurrency && Orig_RPGPlayerData_get_FireStoneAmount) {
        int64_t current = Orig_RPGPlayerData_get_FireStoneAmount(this_ptr);
        if (value < current)
          return;
      }
      Orig_RPGPlayerData_set_FireStoneAmount(this_ptr, value);
    }

    int64_t (*Orig_RPGPlayerData_get_WaterStoneAmount)(void* this_ptr);
    void (*Orig_RPGPlayerData_set_WaterStoneAmount)(void* this_ptr, int64_t value);
    void Hook_RPGPlayerData_set_WaterStoneAmount(void* this_ptr, int64_t value)
    {
      if (Menu::Config.bInfiniteCurrency && Orig_RPGPlayerData_get_WaterStoneAmount) {
        int64_t current = Orig_RPGPlayerData_get_WaterStoneAmount(this_ptr);
        if (value < current)
          return;
      }
      Orig_RPGPlayerData_set_WaterStoneAmount(this_ptr, value);
    }

    int64_t (*Orig_RPGPlayerData_get_LightStoneAmount)(void* this_ptr);
    void (*Orig_RPGPlayerData_set_LightStoneAmount)(void* this_ptr, int64_t value);
    void Hook_RPGPlayerData_set_LightStoneAmount(void* this_ptr, int64_t value)
    {
      if (Menu::Config.bInfiniteCurrency && Orig_RPGPlayerData_get_LightStoneAmount) {
        int64_t current = Orig_RPGPlayerData_get_LightStoneAmount(this_ptr);
        if (value < current)
          return;
      }
      Orig_RPGPlayerData_set_LightStoneAmount(this_ptr, value);
    }

    void Initialize()
    {
      HOOK_SIGNATURE( // everyone know what is this called
        "RPGPlayerData::set_goldAmount", Signatures::RPGPlayerData_set_goldAmount, Hook_RPGPlayerData_set_goldAmount,
        Orig_RPGPlayerData_set_goldAmount
      );
      HOOK_SIGNATURE( // Cryptobit in-game name
        "RPGPlayerData::set_RubyAmount", Signatures::RPGPlayerData_set_RubyAmount, Hook_RPGPlayerData_set_RubyAmount,
        Orig_RPGPlayerData_set_RubyAmount
      );
      HOOK_SIGNATURE( // Diamond in-game name
        "RPGPlayerData::set_JewelAmount", Signatures::RPGPlayerData_set_JewelAmount, Hook_RPGPlayerData_set_JewelAmount,
        Orig_RPGPlayerData_set_JewelAmount
      );
      HOOK_SIGNATURE( // Skill ticket in-game name
        "globalData::set_PickupSkillTicketCount", Signatures::globalData_set_PickupSkillTicketCount,
        Hook_globalData_set_PickupSkillTicketCount, Orig_globalData_set_PickupSkillTicketCount
      );
      HOOK_SIGNATURE(
        "globalData::set_PremTicketCount", Signatures::globalData_set_PremTicketCount,
        Hook_globalData_set_PremTicketCount, Orig_globalData_set_PremTicketCount
      );
      HOOK_SIGNATURE(
        "globalData::set_PickupTicketCount", Signatures::globalData_set_PickupTicketCount,
        Hook_globalData_set_PickupTicketCount, Orig_globalData_set_PickupTicketCount
      );
      HOOK_SIGNATURE(
        "globalData::set_JewelMileage", Signatures::globalData_set_JewelMileage, Hook_globalData_set_JewelMileage,
        Orig_globalData_set_JewelMileage
      );
      HOOK_SIGNATURE(
        "globalData::set_GoldenDiceCount", Signatures::globalData_set_GoldenDiceCount,
        Hook_globalData_set_GoldenDiceCount, Orig_globalData_set_GoldenDiceCount
      );
      HOOK_SIGNATURE(
        "globalData::set_OptionDiceCount", Signatures::globalData_set_OptionDiceCount,
        Hook_globalData_set_OptionDiceCount, Orig_globalData_set_OptionDiceCount
      );
      HOOK_SIGNATURE(
        "globalData::set_SkillDiceCount", Signatures::globalData_set_SkillDiceCount, Hook_globalData_set_SkillDiceCount,
        Orig_globalData_set_SkillDiceCount
      );
      HOOK_SIGNATURE(
        "globalData::set_SkillTrancerCount", Signatures::globalData_set_SkillTrancerCount,
        Hook_globalData_set_SkillTrancerCount, Orig_globalData_set_SkillTrancerCount
      );
      HOOK_SIGNATURE(
        "globalData::set_SkillEnhancerCount", Signatures::globalData_set_SkillEnhancerCount,
        Hook_globalData_set_SkillEnhancerCount, Orig_globalData_set_SkillEnhancerCount
      );
      HOOK_SIGNATURE(
        "globalData::set_SubJMileage", Signatures::globalData_set_SubJMileage, Hook_globalData_set_SubJMileage,
        Orig_globalData_set_SubJMileage
      );
      HOOK_SIGNATURE(
        "globalData::SetDunKeyCount", Signatures::globalData_SetDunKeyCount, Hook_globalData_SetDunKeyCount,
        Orig_globalData_SetDunKeyCount
      );

      HOOK_SIGNATURE(
        "RPGPlayerData::set_TreeStoneAmount", Signatures::RPGPlayerData_set_TreeStoneAmount,
        Hook_RPGPlayerData_set_TreeStoneAmount, Orig_RPGPlayerData_set_TreeStoneAmount
      );
      HOOK_SIGNATURE(
        "RPGPlayerData::set_FireStoneAmount", Signatures::RPGPlayerData_set_FireStoneAmount,
        Hook_RPGPlayerData_set_FireStoneAmount, Orig_RPGPlayerData_set_FireStoneAmount
      );
      HOOK_SIGNATURE(
        "RPGPlayerData::set_WaterStoneAmount", Signatures::RPGPlayerData_set_WaterStoneAmount,
        Hook_RPGPlayerData_set_WaterStoneAmount, Orig_RPGPlayerData_set_WaterStoneAmount
      );
      HOOK_SIGNATURE(
        "RPGPlayerData::set_LightStoneAmount", Signatures::RPGPlayerData_set_LightStoneAmount,
        Hook_RPGPlayerData_set_LightStoneAmount, Orig_RPGPlayerData_set_LightStoneAmount
      );

      // Resolve the original getters to use them in our hooks
      if (Signatures::RPGPlayerData_get_goldAmount) {
        *(void**) &Orig_RPGPlayerData_get_goldAmount = Signatures::RPGPlayerData_get_goldAmount;
      }
      if (Signatures::RPGPlayerData_get_RubyAmount) {
        *(void**) &Orig_RPGPlayerData_get_RubyAmount = Signatures::RPGPlayerData_get_RubyAmount;
      }
      if (Signatures::RPGPlayerData_get_JewelAmount) {
        *(void**) &Orig_RPGPlayerData_get_JewelAmount = Signatures::RPGPlayerData_get_JewelAmount;
      }
      if (Signatures::globalData_get_PickupSkillTicketCount) {
        *(void**) &Orig_globalData_get_PickupSkillTicketCount = Signatures::globalData_get_PickupSkillTicketCount;
      }
      if (Signatures::globalData_get_PremTicketCount) {
        *(void**) &Orig_globalData_get_PremTicketCount = Signatures::globalData_get_PremTicketCount;
      }
      if (Signatures::globalData_get_PickupTicketCount) {
        *(void**) &Orig_globalData_get_PickupTicketCount = Signatures::globalData_get_PickupTicketCount;
      }
      if (Signatures::globalData_get_JewelMileage) {
        *(void**) &Orig_globalData_get_JewelMileage = Signatures::globalData_get_JewelMileage;
      }
      if (Signatures::globalData_get_GoldenDiceCount) {
        *(void**) &Orig_globalData_get_GoldenDiceCount = Signatures::globalData_get_GoldenDiceCount;
      }
      if (Signatures::globalData_get_OptionDiceCount) {
        *(void**) &Orig_globalData_get_OptionDiceCount = Signatures::globalData_get_OptionDiceCount;
      }
      if (Signatures::globalData_get_SkillDiceCount) {
        *(void**) &Orig_globalData_get_SkillDiceCount = Signatures::globalData_get_SkillDiceCount;
      }
      if (Signatures::globalData_get_SkillTrancerCount) {
        *(void**) &Orig_globalData_get_SkillTrancerCount = Signatures::globalData_get_SkillTrancerCount;
      }
      if (Signatures::globalData_get_SkillEnhancerCount) {
        *(void**) &Orig_globalData_get_SkillEnhancerCount = Signatures::globalData_get_SkillEnhancerCount;
      }
      if (Signatures::globalData_get_SubJMileage) {
        *(void**) &Orig_globalData_get_SubJMileage = Signatures::globalData_get_SubJMileage;
      }
      if (Signatures::globalData_GetDunKeyCount) {
        *(void**) &Orig_globalData_GetDunKeyCount = Signatures::globalData_GetDunKeyCount;
      }

      if (Signatures::RPGPlayerData_get_TreeStoneAmount)
        *(void**) &Orig_RPGPlayerData_get_TreeStoneAmount = Signatures::RPGPlayerData_get_TreeStoneAmount;
      if (Signatures::RPGPlayerData_get_FireStoneAmount)
        *(void**) &Orig_RPGPlayerData_get_FireStoneAmount = Signatures::RPGPlayerData_get_FireStoneAmount;
      if (Signatures::RPGPlayerData_get_WaterStoneAmount)
        *(void**) &Orig_RPGPlayerData_get_WaterStoneAmount = Signatures::RPGPlayerData_get_WaterStoneAmount;
      if (Signatures::RPGPlayerData_get_LightStoneAmount)
        *(void**) &Orig_RPGPlayerData_get_LightStoneAmount = Signatures::RPGPlayerData_get_LightStoneAmount;
    }

    void Uninitialize() { }
  }  // namespace InfiniteCurrency
}  // namespace Features
