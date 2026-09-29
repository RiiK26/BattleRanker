#include <fstream>
#include <iostream>
#include <set>
#include "MonoUtils.hpp"

namespace mono
{
  MonoDomain* (*mono_get_root_domain)();
  MonoThread* (*mono_thread_attach)(MonoDomain* domain);
  MonoAssembly* (*mono_domain_assembly_open)(MonoDomain* domain, const char* name);
  MonoImage* (*mono_assembly_get_image)(MonoAssembly* assembly);
  MonoClass* (*mono_class_from_name)(MonoImage* image, const char* name_space, const char* name);
  MonoMethod* (*mono_class_get_method_from_name)(MonoClass* klass, const char* name, int param_count);
  MonoClass* (*mono_class_get_parent)(MonoClass* klass);
  void* (*mono_class_get_methods)(MonoClass* klass, void** iter);
  const char* (*mono_method_get_name)(MonoMethod* method);
  void* (*mono_compile_method)(MonoMethod* method);

  void
  dump_class_methods(const std::string& assembly_name, const std::string& name_space, const std::string& class_name)
  {
    MonoDomain* domain = mono_get_root_domain();
    if (!domain)
      return;
    MonoAssembly* assembly = mono_domain_assembly_open(domain, assembly_name.c_str());
    if (!assembly) {
      std::string dll_name = assembly_name + ".dll";
      assembly             = mono_domain_assembly_open(domain, dll_name.c_str());
      if (!assembly) {
        std::string path_name = "Battle Ranker_Data/Managed/" + dll_name;
        assembly              = mono_domain_assembly_open(domain, path_name.c_str());
        if (!assembly)
          return;
      }
    }
    MonoImage* image = mono_assembly_get_image(assembly);
    if (!image)
      return;
    MonoClass* klass = mono_class_from_name(image, name_space.c_str(), class_name.c_str());
    if (!klass)
      return;

    std::ofstream logfile("BattleRanker_ClassDump_" + class_name + ".txt", std::ios::out);
    if (!logfile.is_open())
      return;

    logfile << "Methods for " << class_name << ":\n";
    void*       iter = nullptr;
    MonoMethod* method;
    while ((method = (MonoMethod*) mono_class_get_methods(klass, &iter)) != nullptr) {
      logfile << mono_method_get_name(method) << "\n";
    }
    logfile.close();
  }

  bool init()
  {
    HMODULE hMono = GetModuleHandleA("mono-2.0-bdwgc.dll");
    if (!hMono)
      hMono = GetModuleHandleA("mono.dll");
    if (!hMono)
      return false;

    mono_get_root_domain = (decltype(mono_get_root_domain)) GetProcAddress(hMono, "mono_get_root_domain");
    mono_thread_attach   = (decltype(mono_thread_attach)) GetProcAddress(hMono, "mono_thread_attach");
    mono_domain_assembly_open =
      (decltype(mono_domain_assembly_open)) GetProcAddress(hMono, "mono_domain_assembly_open");
    mono_assembly_get_image = (decltype(mono_assembly_get_image)) GetProcAddress(hMono, "mono_assembly_get_image");
    mono_class_from_name    = (decltype(mono_class_from_name)) GetProcAddress(hMono, "mono_class_from_name");
    mono_class_get_method_from_name =
      (decltype(mono_class_get_method_from_name)) GetProcAddress(hMono, "mono_class_get_method_from_name");
    mono_class_get_parent  = (decltype(mono_class_get_parent)) GetProcAddress(hMono, "mono_class_get_parent");
    mono_class_get_methods = (decltype(mono_class_get_methods)) GetProcAddress(hMono, "mono_class_get_methods");
    mono_method_get_name   = (decltype(mono_method_get_name)) GetProcAddress(hMono, "mono_method_get_name");
    mono_compile_method    = (decltype(mono_compile_method)) GetProcAddress(hMono, "mono_compile_method");

    return mono_get_root_domain && mono_compile_method;
  }

  void* get_method(
    const std::string& assembly_name,
    const std::string& name_space,
    const std::string& class_name,
    const std::string& method_name,
    int                param_count
  )
  {
    MonoDomain* domain = mono_get_root_domain();
    if (!domain) {
      std::ofstream logfile("BattleRanker_Log.txt", std::ios::app);
      if (logfile.is_open())
        logfile << "get_method: mono_get_root_domain failed for " << class_name << std::endl;
      return nullptr;
    }

    if (mono_thread_attach)
      mono_thread_attach(domain);

    MonoAssembly* assembly = mono_domain_assembly_open(domain, assembly_name.c_str());
    if (!assembly) {
      // Fallback: try adding .dll
      std::string dll_name = assembly_name + ".dll";
      assembly             = mono_domain_assembly_open(domain, dll_name.c_str());
      if (!assembly) {
        // Fallback 2: try full relative path
        std::string path_name = "Battle Ranker_Data/Managed/" + dll_name;
        assembly              = mono_domain_assembly_open(domain, path_name.c_str());
        if (!assembly) {
          static std::set<std::string> logged_assemblies;
          if (logged_assemblies.find(assembly_name) == logged_assemblies.end()) {
            std::ofstream logfile("BattleRanker_Log.txt", std::ios::app);
            if (logfile.is_open()) {
              logfile << "get_method: mono_domain_assembly_open failed for " << assembly_name << " and all fallbacks."
                      << std::endl;
            }
            logged_assemblies.insert(assembly_name);
          }
          return nullptr;
        }
      }
    }

    MonoImage* image = mono_assembly_get_image(assembly);
    if (!image) {
      std::ofstream logfile("BattleRanker_Log.txt", std::ios::app);
      if (logfile.is_open())
        logfile << "get_method: mono_assembly_get_image failed for " << assembly_name << std::endl;
      return nullptr;
    }

    MonoClass* klass = mono_class_from_name(image, name_space.c_str(), class_name.c_str());
    if (!klass) {
      std::ofstream logfile("BattleRanker_Log.txt", std::ios::app);
      if (logfile.is_open())
        logfile << "get_method: mono_class_from_name failed for " << class_name << std::endl;
      return nullptr;
    }

    MonoMethod* method = mono_class_get_method_from_name(klass, method_name.c_str(), param_count);

    // Search base classes if not found
    MonoClass* parent = klass;
    while (!method && mono_class_get_parent && (parent = mono_class_get_parent(parent)) != nullptr) {
      method = mono_class_get_method_from_name(parent, method_name.c_str(), param_count);
    }

    if (!method) {
      std::ofstream logfile("BattleRanker_Log.txt", std::ios::app);
      if (logfile.is_open()) {
        logfile << "get_method: mono_class_get_method_from_name failed for " << method_name << std::endl;
        if (mono_class_get_methods && mono_method_get_name) {
          logfile << "  Available methods in " << class_name << ":" << std::endl;
          void*       iter = nullptr;
          MonoMethod* cur_method;
          while ((cur_method = (MonoMethod*) mono_class_get_methods(klass, &iter)) != nullptr) {
            logfile << "    " << mono_method_get_name(cur_method) << std::endl;
          }
        }
      }
      return nullptr;
    }

    return mono_compile_method(method);
  }
}  // namespace mono
