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

# Try to extract game path from config.json first
CONFIG_GAME_PATH=$(grep -oP '"game_path": "\K[^"]+' "$PROJECT_ROOT/config.json")

# Auto-detect path if it's "auto", empty, or directory doesn't exist
if [ "$CONFIG_GAME_PATH" = "auto" ] || [ -z "$CONFIG_GAME_PATH" ] || [ ! -d "$CONFIG_GAME_PATH" ]; then
    echo "[*] Auto-detecting game path via Steam library folders..."
    DETECTED_PATH=$(python3 -c '
import os, sys
app_id = "'$APP_ID'"
steam_root = os.path.expanduser("~/.local/share/Steam")
if not os.path.exists(steam_root):
    steam_root = os.path.expanduser("~/.steam/steam")
lib_folders = os.path.join(steam_root, "steamapps", "libraryfolders.vdf")
if os.path.exists(lib_folders):
    with open(lib_folders, "r") as f:
        curr = None
        for line in f:
            if "\"path\"" in line:
                curr = line.split("\"")[3]
            if f"\"{app_id}\"" in line and curr:
                print(os.path.join(curr, "steamapps", "common", "BattleRanker"))
                sys.exit(0)
print("")
')
    if [ -n "$DETECTED_PATH" ] && [ -d "$DETECTED_PATH" ]; then
        GAME_PATH="$DETECTED_PATH"
        echo "[+] Found game path: $GAME_PATH"
    else
        echo "[-] Error: Could not auto-detect game path and config.json path is invalid."
        exit 1
    fi
else
    GAME_PATH="$CONFIG_GAME_PATH"
fi

echo "[*] Copying DLL to game directory to bypass Proton container path restrictions..."
cp "$DLL_PATH" "$GAME_PATH/$DLL_FILENAME"

# We pass just the DLL filename. LoadLibraryA will search the current directory of the target process (Battle Ranker.exe)
echo "[*] Injecting DLL into Proton prefix for App ID $APP_ID..."
protontricks -c "wine \"$INJECTOR_EXE\" \"Battle Ranker.exe\" \"$DLL_FILENAME\"" $APP_ID
