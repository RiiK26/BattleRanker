#pragma once
#include "../imgui.h"  // IMGUI_IMPL_API
#ifndef IMGUI_DISABLE
  #include <windows.h>
  #include <dxgiformat.h>  // DXGI_FORMAT
  #include <d3d12.h>       // D3D12_CPU_DESCRIPTOR_HANDLE

  // Clang/GCC warnings with -Weverything
  #if defined(__clang__)
    #pragma clang diagnostic push
    #pragma clang diagnostic ignored "-Wold-style-cast"  // warning: use of old-style cast
  #endif

// Initialization data, for ImGui_ImplDX12_Init()
struct ImGui_ImplDX12_InitInfo
{
  ID3D12Device*       Device;
  ID3D12CommandQueue* CommandQueue;  // Command queue used for queuing texture uploads.
  int                 NumFramesInFlight;
  DXGI_FORMAT         RTVFormat;     // RenderTarget format.
  DXGI_FORMAT         DSVFormat;     // DepthStencilView format.
  void*               UserData;

  // Allocating SRV descriptors for textures is up to the application, so we provide callbacks.
  // (current version of the backend will only allocate one descriptor, from 1.92 the backend will need to allocate more)
  ID3D12DescriptorHeap* SrvDescriptorHeap;
  void (*SrvDescriptorAllocFn)(
    ImGui_ImplDX12_InitInfo*     info,
    D3D12_CPU_DESCRIPTOR_HANDLE* out_cpu_desc_handle,
    D3D12_GPU_DESCRIPTOR_HANDLE* out_gpu_desc_handle
  );
  void (*SrvDescriptorFreeFn)(
    ImGui_ImplDX12_InitInfo*    info,
    D3D12_CPU_DESCRIPTOR_HANDLE cpu_desc_handle,
    D3D12_GPU_DESCRIPTOR_HANDLE gpu_desc_handle
  );
  #ifndef IMGUI_DISABLE_OBSOLETE_FUNCTIONS
  D3D12_CPU_DESCRIPTOR_HANDLE
  LegacySingleSrvCpuDescriptor;  // To facilitate transition from single descriptor to allocator callback, you may use those.
  D3D12_GPU_DESCRIPTOR_HANDLE LegacySingleSrvGpuDescriptor;
  #endif

  ImGui_ImplDX12_InitInfo() { memset((void*) this, 0, sizeof(*this)); }
};

// Follow "Getting Started" link and check examples/ folder to learn about using backends!
IMGUI_IMPL_API bool ImGui_ImplDX12_Init(ImGui_ImplDX12_InitInfo* info);
IMGUI_IMPL_API void ImGui_ImplDX12_Shutdown();
IMGUI_IMPL_API void ImGui_ImplDX12_NewFrame();
IMGUI_IMPL_API void
ImGui_ImplDX12_RenderDrawData(ImDrawData* draw_data, ID3D12GraphicsCommandList* graphics_command_list);

// Use if you want to reset your rendering device without losing Dear ImGui state.
IMGUI_IMPL_API bool ImGui_ImplDX12_CreateDeviceObjects();
IMGUI_IMPL_API void ImGui_ImplDX12_InvalidateDeviceObjects();

// (Advanced) Use e.g. if you need to precisely control the timing of texture updates (e.g. for staged rendering), by setting ImDrawData::Textures = nullptr to handle this manually.
IMGUI_IMPL_API void ImGui_ImplDX12_UpdateTexture(ImTextureData* tex);

// [BETA] Selected render state data shared with callbacks.
// This is temporarily stored in GetPlatformIO().Renderer_RenderState during the ImGui_ImplDX12_RenderDrawData() call.
// (Please open an issue if you feel you need access to more data)
struct ImGui_ImplDX12_RenderState
{
  ID3D12Device*              Device;
  ID3D12GraphicsCommandList* CommandList;
};
IMGUI_IMPL_API ImGui_ImplDX12_RenderState* ImGui_ImplDX12_GetRenderState();

  #ifndef IMGUI_DISABLE_OBSOLETE_FUNCTIONS
  // Legacy initialization API Obsoleted in 1.91.5
  // - font_srv_cpu_desc_handle and font_srv_gpu_desc_handle are handles to a single SRV descriptor to use for the internal font texture, they must be in 'srv_descriptor_heap'
  // - When we introduced the ImGui_ImplDX12_InitInfo struct we also added a 'ID3D12CommandQueue* CommandQueue' field.
  // - DOES NOT SUPPORT ImGuiBackendFlags_RendererHasTextures.
  //IMGUI_IMPL_API bool     ImGui_ImplDX12_Init(ID3D12Device* device, int num_frames_in_flight, DXGI_FORMAT rtv_format, ID3D12DescriptorHeap* srv_descriptor_heap, D3D12_CPU_DESCRIPTOR_HANDLE font_srv_cpu_desc_handle, D3D12_GPU_DESCRIPTOR_HANDLE font_srv_gpu_desc_handle);
  #endif

  #if defined(__clang__)
    #pragma clang diagnostic pop
  #endif

#endif  // #ifndef IMGUI_DISABLE
