# Project: BattleRanker

* Game Directory: `/data/SteamLibrary/steamapps/common/BattleRanker`
* Dll: `Assembly-CSharp.dll`, have `ACTk.Runtime.dll`
* Language: C#
* Framework: Mono
* Game Engine: Unity
* Target Platform: Linux & Windows
* Target Architecture: x64
* Graphics API: DX12 (NATIVE)

## Tech Stack

* Language: C/C++ 20+
* Build System: CMake
* GUI: ImGui
* Hook: MinHook

## Code Conventions
- **Modular Architecture**: 1 Feature = 1 `.hpp`/`.cpp` pair located inside `src/Features/<Domain>/` (e.g. `src/Features/Combat/GodMode.cpp`). **No god files.**
- **UI/Feature Sync**: The `src/Features/<Domain>/` directory structure MUST map exactly 1:1 with the ImGui Tab names defined in `Menu.cpp` (e.g., `Combat`, `Player`, `Currency`). If a feature is displayed in the "Player" tab, its source code MUST reside in `src/Features/Player/`.
- **Hooking**: Always prefer AOB pattern scanning or Mono method hooking over static offsets where possible. Dynamic resolving and signatures are resilient to game updates and obfuscation, whereas hardcoded offsets break immediately.
- **Signature Database**: `config.json` at the root of the project serves as the baseline signature and extraction rule database.
- **Toggles & Config**: Store feature toggles and parameters in `Menu::ConfigData`. Load/save these values using `config.txt` at the project binaries directory. Avoid hardcoding magic numbers for features.
- **Initialization**: Every feature file must have an `Initialize()` and `Uninitialize()` function

## Patterns

### Feature Implementation Pattern
Every feature should be modular and follow this structure (e.g. `src/Features/Combat/SpeedHack.cpp`). It defines the original function pointer, the hook function, and registers it in `Initialize()`:

```cpp
#include "SpeedHack.hpp"
#include "../../Modules/Hooks/Hooks.hpp"
#include "../../Modules/Hooks/Signatures.hpp"
#include "../../Modules/Menu/Menu.hpp"

namespace Features
{
  namespace SpeedHack
  {
    // 1. Declare original function pointer
    void (*Orig_Time_set_timeScale)(float value, void* method_info);

    // 2. Define the hook overriding the behavior
    void Hook_Time_set_timeScale(float value, void* method_info)
    {
      if (Menu::Config.bSpeedHack) {
        value = Menu::Config.fSpeedMultiplier;
      }
      Orig_Time_set_timeScale(value, method_info);
    }

    // 3. Register the hook in Initialize()
    void Initialize()
    {
      HOOK_SIGNATURE(
        "Time::set_timeScale", Signatures::Time_set_timeScale, Hook_Time_set_timeScale, Orig_Time_set_timeScale
      );
    }

    void Uninitialize() { }
  }  // namespace SpeedHack
}  // namespace Features
```

## Game Knowledge

### Instances & Data
- **`globalData`**: Manages transient/system currencies (Dice, Tickets, Keys, Mileage, CP, Quantum Orb, Candela).
- **`RPGPlayerData`**: Manages persistent player stats and items (Gold, Ruby/Cryptobit, Jewel/Diamond, Cores/Stones).

### Terminology Mapping (Game vs Memory)
- **Ruby** = Cryptobit
- **Jewel** = Diamond
- **Option Dice** = Lucky Dice
- **Soul Stone** = `SoulStoneCount` (managed in `globalData`). Required for Character Awakening, from Hunt Skeleton.
- **Quantum Cube** = `TransStoneCount` (managed in `globalData`). Can transcend Weapon.
- **Lumino** = `EnhanceStoneCount` (managed in `globalData`). Can enhance Weapon, from Dungeon Kings Garden.
- **Poly Fiber** = `AvataSkinEnhancerCount` (managed in `globalData`).
- **Quantum Ring** = `AvataSkinTrancerCount` (managed in `globalData`).
- **Arena Ticket** = `PvpTicket` (managed in `GDPVPScript`, which is `PVPData` inside `globalData`).
- **Dungeon Keys**: Indexed 0-19, managed via `GetDunKeyCount(this_ptr, idx)` and `SetDunKeyCount(this_ptr, idx, val)`.

### Gacha & Summon Mechanics
Summoning logic is managed primarily by `GearBoxScript`. Randomness is calculated per pull using these methods:
- **Gear/Accessory/Pet Level**: Uses `GetRandomPickLv(int pType, int pickLevel)` returning an int 1-20.
- **Skill Grade**: Uses `GetActualSkillGrade(bool isPickup)` returning `RPGSkillV3Grade` (Legend = 3).
- **Skin Tier**: Handled by `SubActualSkinPull(RPGAvataPartV2 pPart, RPGAvataGrade pGrade, List<InvenCeremObject> pCeremList)`. Can be hooked to override the grade (Head/Weapon Max=S(1), Pet Max=SS(2), Suit Max=SSS(3)).

### Advanced Patterns

#### One-Time Execution from UI
For actions that modify game state (like giving currency) via ImGui buttons, DO NOT hook `get_` methods to fake visual values. Instead:
1. Define request flags in `Menu::ConfigData` (e.g., `bool bRequestSetGold = false;`).
2. Continuously capture the required class instances (e.g., `g_RPGPlayerData`) by placing hooks on frequently called methods (like `get_goldAmount`).
3. Implement a `ProcessRequests()` function that checks the flags and safely invokes the original `set_` Mono methods (casting native pointers from `Signatures::`) using the captured instances.
4. Call `ProcessRequests()` every frame in the DX12 Present hook (e.g., in `Menu.cpp`'s `RenderMenu`).
