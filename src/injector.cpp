#include <windows.h>
#include <tlhelp32.h>
#include <iostream>
#include <string>

DWORD GetProcessIdByName(const std::string& processName)
{
  DWORD  processId = 0;
  HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);

  if (hSnapshot != INVALID_HANDLE_VALUE) {
    PROCESSENTRY32 processEntry;
    processEntry.dwSize = sizeof(processEntry);

    if (Process32First(hSnapshot, &processEntry)) {
      do {
        if (processName == processEntry.szExeFile) {
          processId = processEntry.th32ProcessID;
          break;
        }
      } while (Process32Next(hSnapshot, &processEntry));
    }
    CloseHandle(hSnapshot);
  }
  return processId;
}

bool FileExists(const std::string& filePath)
{
  DWORD attrib = GetFileAttributesA(filePath.c_str());
  return (attrib != INVALID_FILE_ATTRIBUTES && !(attrib & FILE_ATTRIBUTE_DIRECTORY));
}

int main(int argc, char* argv[])
{
  std::cout << "=== Robust DLL Injector ===\n";

  if (argc != 3) {
    std::cerr << "Usage: injector.exe <process_name> <dll_path>\n";
    std::cerr << "Example: injector.exe \"Battle Ranker.exe\" \"libBattleRankerInternalCheat.dll\"\n";
    return 1;
  }

  std::string processName = argv[1];
  std::string dllPath     = argv[2];

  // We no longer resolve the absolute path or check if it exists here,
  // because in Proton/Wine, passing just the DLL name to LoadLibraryA
  // works better when the DLL is copied directly to the game's directory.

  std::cout << "[*] Waiting for process '" << processName << "'...\n";
  DWORD processId = 0;
  while (!processId) {
    processId = GetProcessIdByName(processName);
    if (!processId) {
      Sleep(1000);
    }
  }

  std::cout << "[+] Found process ID: " << processId << "\n";

  HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, processId);
  if (!hProcess) {
    std::cerr << "[-] Error: Failed to open process. (Run as Administrator?)\n";
    return 1;
  }

  void* allocMem = VirtualAllocEx(hProcess, nullptr, dllPath.length() + 1, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
  if (!allocMem) {
    std::cerr << "[-] Error: Failed to allocate memory in target process.\n";
    CloseHandle(hProcess);
    return 1;
  }

  if (!WriteProcessMemory(hProcess, allocMem, dllPath.c_str(), dllPath.length() + 1, nullptr)) {
    std::cerr << "[-] Error: Failed to write DLL path to target process.\n";
    VirtualFreeEx(hProcess, allocMem, 0, MEM_RELEASE);
    CloseHandle(hProcess);
    return 1;
  }

  HMODULE hKernel32 = GetModuleHandleA("Kernel32.dll");
  if (!hKernel32) {
    std::cerr << "[-] Error: Failed to get handle for Kernel32.dll.\n";
    VirtualFreeEx(hProcess, allocMem, 0, MEM_RELEASE);
    CloseHandle(hProcess);
    return 1;
  }

  FARPROC loadLibraryAddr = GetProcAddress(hKernel32, "LoadLibraryA");
  if (!loadLibraryAddr) {
    std::cerr << "[-] Error: Failed to get address for LoadLibraryA.\n";
    VirtualFreeEx(hProcess, allocMem, 0, MEM_RELEASE);
    CloseHandle(hProcess);
    return 1;
  }

  HANDLE hThread =
    CreateRemoteThread(hProcess, nullptr, 0, (LPTHREAD_START_ROUTINE) loadLibraryAddr, allocMem, 0, nullptr);
  if (!hThread) {
    std::cerr << "[-] Error: Failed to create remote thread.\n";
    VirtualFreeEx(hProcess, allocMem, 0, MEM_RELEASE);
    CloseHandle(hProcess);
    return 1;
  }

  std::cout << "[*] Remote thread created successfully. Waiting for injection to finish...\n";
  WaitForSingleObject(hThread, INFINITE);

  DWORD exitCode;
  GetExitCodeThread(hThread, &exitCode);
  if (exitCode == 0) {
    std::cerr << "[-] Error: LoadLibraryA failed inside target process (returned 0).\n";
  }
  else {
    std::cout << "[+] DLL injected successfully!\n";
  }

  CloseHandle(hThread);
  VirtualFreeEx(hProcess, allocMem, 0, MEM_RELEASE);
  CloseHandle(hProcess);

  return 0;
}
