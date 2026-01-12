//
// Created by ThewyRogue99 on 11.01.2026.
//

#include "GLBackend.h"

#include <unordered_map>

#include <glad/gl.h>

#include <Aether/Log/Log.h>
#include <Aether/Core/Assert.h>
#include <Aether/Platform/GraphicsContext.h>

namespace {
    Aether::Platform::GraphicsContext* s_LoaderContext = nullptr;

    void* GLGetProcAddress(const char* name) {
        return s_LoaderContext
            ? s_LoaderContext->GetProcAddress(name)
            : nullptr;
    }
}

namespace Aether::Renderer {
    static GLenum ToGLTarget(const BufferUsage usage) {
        switch (usage) {
            case BufferUsage::Vertex:  return GL_ARRAY_BUFFER;
            case BufferUsage::Index:   return GL_ELEMENT_ARRAY_BUFFER;
            case BufferUsage::Uniform: return GL_UNIFORM_BUFFER;
        }
        AETHER_ASSERT(false);
        return 0;
    }

    static GLenum ToGLUsage(const BufferAccess access) {
        return access == BufferAccess::Dynamic
            ? GL_DYNAMIC_DRAW
            : GL_STATIC_DRAW;
    }

    class GLBackend::Impl {
    public:
        Impl() = default;
        ~Impl() = default;

        void Init(const BackendInitInfo& info) {
            m_Context = info.context;
            m_Width   = info.width;
            m_Height  = info.height;

            AETHER_ASSERT(m_Context, "GLBackend: GraphicsContext is null");
            AETHER_ASSERT(
                m_Context->GetAPI() == Aether::Platform::GraphicsAPI::OpenGL,
                "GLBackend: GraphicsContext is not OpenGL"
            );

            m_Context->MakeCurrent();

            s_LoaderContext = m_Context;
            AETHER_ASSERT(gladLoadGL(reinterpret_cast<GLADloadfunc>(GLGetProcAddress)), "Failed to initialize OpenGL via glad");
            s_LoaderContext = nullptr;

            glViewport(0, 0, static_cast<GLsizei>(m_Width), static_cast<GLsizei>(m_Height));
            glEnable(GL_DEPTH_TEST);
        }

        void Shutdown() {
            m_Context = nullptr;
        }

        void BeginFrame() {
            AETHER_ASSERT(!m_InFrame, "GLBackend::BeginFrame called twice");
            m_InFrame = true;
        }

        void EndFrame() {
            AETHER_ASSERT(m_InFrame, "GLBackend::EndFrame without BeginFrame");
            m_InFrame = false;
        }

        void SetClearColor(float r, float g, float b, float a) {
            m_ClearColor[0] = r;
            m_ClearColor[1] = g;
            m_ClearColor[2] = b;
            m_ClearColor[3] = a;
        }

        void Clear() {
            glClearColor(
                m_ClearColor[0],
                m_ClearColor[1],
                m_ClearColor[2],
                m_ClearColor[3]
            );

            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        }

        void Present() {
            AETHER_ASSERT(m_Context, "GLBackend::Present without context");
            m_Context->SwapBuffers();
        }

        BufferHandle CreateBuffer(const BufferDesc& desc, const void* initialData) {
            GLuint glId;
            glGenBuffers(1, &glId);

            const GLenum target = ToGLTarget(desc.usage);
            glBindBuffer(target, glId);
            glBufferData(target, desc.size, initialData, ToGLUsage(desc.access));
            glBindBuffer(target, 0);

            uint32_t handleId = m_NextBufferId++;
            m_Buffers.emplace(handleId, GLBuffer{
                .id = glId,
                .target = target,
                .size = desc.size
            });

            return BufferHandle{ handleId };
        }

        void DestroyBuffer(const BufferHandle& handle) {
            const auto it = m_Buffers.find(handle.id);
            if (it == m_Buffers.end())
                return;

            glDeleteBuffers(1, &it->second.id);
            m_Buffers.erase(it);
        }

        void UpdateBuffer(const BufferHandle& handle, uint32_t offset, const void* data, uint32_t size) {
            const auto it = m_Buffers.find(handle.id);
            AETHER_ASSERT(it != m_Buffers.end());

            const GLBuffer& buf = it->second;
            AETHER_ASSERT(offset + size <= buf.size);

            glBindBuffer(buf.target, buf.id);
            glBufferSubData(buf.target, offset, size, data);
            glBindBuffer(buf.target, 0);
        }

    private:
        struct GLBuffer {
            GLuint id;
            GLenum target;
            uint32_t size;
        };

    private:
        Platform::GraphicsContext* m_Context = nullptr;

        unsigned int m_Width  = 0;
        unsigned int m_Height = 0;

        float m_ClearColor[4] = { 0.f, 0.f, 0.f, 1.f };
        bool m_InFrame = false;

        std::unordered_map<uint32_t, GLBuffer> m_Buffers;
        uint32_t m_NextBufferId = 1;
    };

    GLBackend::GLBackend() : m_Impl(Engine::MakeScope<Impl>()) { }

    GLBackend::~GLBackend() = default;

    RenderAPI GLBackend::API() const {
        return RenderAPI::OpenGL;
    }

    void GLBackend::Init(const BackendInitInfo& info) {
        m_Impl->Init(info);
    }

    void GLBackend::Shutdown() {
        m_Impl->Shutdown();
    }

    void GLBackend::BeginFrame() {
        m_Impl->BeginFrame();
    }

    void GLBackend::EndFrame() {
        m_Impl->EndFrame();
    }

    void GLBackend::SetClearColor(float r, float g, float b, float a) {
        m_Impl->SetClearColor(r, g, b, a);
    }

    void GLBackend::Clear() {
        m_Impl->Clear();
    }

    void GLBackend::Present() {
        m_Impl->Present();
    }

    BufferHandle GLBackend::CreateBuffer(const BufferDesc& desc, const void* initialData) {
        return m_Impl->CreateBuffer(desc, initialData);
    }

    void GLBackend::DestroyBuffer(const BufferHandle& handle) {
        m_Impl->DestroyBuffer(handle);
    }

    void GLBackend::UpdateBuffer(const BufferHandle& handle, uint32_t offset, const void* data, uint32_t size) {
        m_Impl->UpdateBuffer(handle, offset, data, size);
    }
}
