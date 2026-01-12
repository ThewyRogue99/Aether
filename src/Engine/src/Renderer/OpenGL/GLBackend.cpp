//
// Created by ThewyRogue99 on 11.01.2026.
//

#include "GLBackend.h"

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
    GLBackend::GLBackend() = default;

    GLBackend::~GLBackend() = default;

    RenderAPI GLBackend::API() const {
        return RenderAPI::OpenGL;
    }

    void GLBackend::Init(const BackendInitInfo& info) {
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

    void GLBackend::Shutdown() {
        m_Context = nullptr;
    }

    void GLBackend::BeginFrame() {
        AETHER_ASSERT(!m_InFrame, "GLBackend::BeginFrame called twice");
        m_InFrame = true;
    }

    void GLBackend::EndFrame() {
        AETHER_ASSERT(m_InFrame, "GLBackend::EndFrame without BeginFrame");
        m_InFrame = false;
    }

    void GLBackend::SetClearColor(float r, float g, float b, float a) {
        m_ClearColor[0] = r;
        m_ClearColor[1] = g;
        m_ClearColor[2] = b;
        m_ClearColor[3] = a;
    }

    void GLBackend::Clear() {
        glClearColor(
            m_ClearColor[0],
            m_ClearColor[1],
            m_ClearColor[2],
            m_ClearColor[3]
        );

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }

    void GLBackend::Present() {
        AETHER_ASSERT(m_Context, "GLBackend::Present without context");
        m_Context->SwapBuffers();
    }
}
