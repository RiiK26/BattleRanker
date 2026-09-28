#!/usr/bin/env bash

APP_ID=4821280
PROJECT_ROOT="$(cd "$(dirname "${BASH_SOURCE[0]}")/../.." && pwd)"
DLL_PATH="$PROJECT_ROOT/build/release/libBattleRankerInternalCheat.dll"

echo "=== BattleRanker Proton Runner ==="
echo "App ID : $APP_ID"
echo "DLL    : $DLL_PATH"
echo "=================================="

if [ ! -f "$DLL_PATH" ]; then
    echo "[-] Error: DLL not found! Please build the project first."
    exit 1
fi

INJECTOR_EXE="$PROJECT_ROOT/build/release/injector.exe"

if [ ! -f "$INJECTOR_EXE" ]; then
    echo "[-] Error: Injector not found! Please build the project first."
    exit 1
fi

DLL_FILENAME="libBattleRankerInternalCheat.dll"

# Extract game path from config.json
GAME_PATH=$(grep -oP '"game_path": "\K[^"]+' "$PROJECT_ROOT/config.json")

echo "[*] Copying DLL to game directory to bypass Proton container path restrictions..."
cp "$DLL_PATH" "$GAME_PATH/$DLL_FILENAME"

# We pass just the DLL filename. LoadLibraryA will search the current directory of the target process (Battle Ranker.exe)
echo "[*] Injecting DLL into Proton prefix for App ID $APP_ID..."
protontricks -c "wine \"$INJECTOR_EXE\" \"Battle Ranker.exe\" \"$DLL_FILENAME\"" $APP_ID
