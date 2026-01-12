//
// Created by ThewyRogue99 on 11.01.2026.
//

#include <Aether/Renderer/Renderer.h>

#include <Aether/Renderer/RenderThread.h>
#include <Aether/Renderer/RenderBackend.h>

#include <Aether/Core/Assert.h>
#include <Aether/Platform/Window.h>
#include <Aether/Core/Memory/Pointer.h>

#include "OpenGL/GLBackend.h"

namespace Aether::Renderer {
    static RenderThread s_RenderThread;
    static Engine::Scope<RenderBackend> s_Backend;
    static uint64_t s_FrameIndex = 0;

    static Platform::Window* s_Window = nullptr;

    static BackendInitInfo MakeBackendInitInfo() {
        BackendInitInfo bi{};
        bi.context = s_Window ? s_Window->GetGraphicsContext() : nullptr;
        bi.width  = s_Window ? s_Window->GetWidth() : 0;
        bi.height = s_Window ? s_Window->GetHeight() : 0;
        return bi;
    }

    void Renderer::Init(const RendererInitInfo& info) {
        AETHER_ASSERT(info.window, "Window cannot be null");
        s_Window = info.window;

        const auto context = s_Window->GetGraphicsContext();

        s_RenderThread.Start();

        s_RenderThread.Enqueue([=](){
            switch (context->GetAPI()) {
                case Platform::GraphicsAPI::OpenGL: s_Backend = Engine::MakeScope<GLBackend>(); break;
                default: /* TODO */ break;
            }

            if (s_Backend)
                s_Backend->Init(MakeBackendInitInfo());
        });

        s_RenderThread.Flush();
    }

    void Renderer::Shutdown() {
        s_RenderThread.Enqueue([](){
            if (s_Backend) {
                s_Backend->Shutdown();
                s_Backend.reset();
            }
        });

        s_RenderThread.Flush();

        s_RenderThread.Stop();
        s_Window = nullptr;
    }

    void Renderer::BeginFrame() {
        s_RenderThread.Enqueue([](){
            if (s_Backend) s_Backend->BeginFrame();
        });
    }

    void Renderer::EndFrame() {
        s_RenderThread.Enqueue([](){
            if (s_Backend) {
                s_Backend->EndFrame();
                s_Backend->Present();
            }
        });

        ++s_FrameIndex;
    }

    void Renderer::SetClearColor(float r, float g, float b, float a) {
        s_RenderThread.Enqueue([=](){
            if (s_Backend) s_Backend->SetClearColor(r, g, b, a);
        });
    }

    void Renderer::Clear() {
        s_RenderThread.Enqueue([](){
            if (s_Backend) s_Backend->Clear();
        });
    }

    RenderAPI Renderer::GetAPI() {
        return s_Backend ? s_Backend->API() : RenderAPI::None;
    }

    uint64_t Renderer::GetFrameIndex() {
        return s_FrameIndex;
    }

    BufferHandle Renderer::CreateBuffer(const BufferDesc& desc, const void* initialData) {
        BufferHandle handle;

        s_RenderThread.Enqueue([&]() {
            handle = s_Backend->CreateBuffer(desc, initialData);
        });

        s_RenderThread.Flush();
        return handle;
    }

    void Renderer::DestroyBuffer(const BufferHandle& handle) {
        s_RenderThread.Enqueue([=]() {
            s_Backend->DestroyBuffer(handle);
        });
    }

    void Renderer::UpdateBuffer(const BufferHandle& handle, uint32_t offset, const void *data, uint32_t size) {
        s_RenderThread.Enqueue([=]() {
            s_Backend->UpdateBuffer(handle, offset, data, size);
        });
    }
}
