#ifndef MONO_UTILS_HPP
#define MONO_UTILS_HPP

#include <windows.h>
#include <string>

// Mono types
typedef void* MonoDomain;
typedef void* MonoAssembly;
typedef void* MonoImage;
typedef void* MonoClass;
typedef void* MonoMethod;
typedef void* MonoThread;

namespace mono
{
  extern MonoDomain* (*mono_get_root_domain)();
  extern MonoThread* (*mono_thread_attach)(MonoDomain* domain);
  extern MonoAssembly* (*mono_domain_assembly_open)(MonoDomain* domain, const char* name);
  extern MonoImage* (*mono_assembly_get_image)(MonoAssembly* assembly);
  extern MonoClass* (*mono_class_from_name)(MonoImage* image, const char* name_space, const char* name);
  extern MonoMethod* (*mono_class_get_method_from_name)(MonoClass* klass, const char* name, int param_count);
  extern MonoClass* (*mono_class_get_parent)(MonoClass* klass);
  extern void* (*mono_class_get_methods)(MonoClass* klass, void** iter);
  extern const char* (*mono_method_get_name)(MonoMethod* method);
  extern void* (*mono_compile_method)(MonoMethod* method);

  bool init();
  void* get_method(
    const std::string& assembly_name,
    const std::string& name_space,
    const std::string& class_name,
    const std::string& method_name,
    int                param_count
  );
}  // namespace mono

#endif  // MONO_UTILS_H
