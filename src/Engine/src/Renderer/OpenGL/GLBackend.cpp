//
// Created by ThewyRogue99 on 11.01.2026.
//

#include "GLBackend.h"

#include <vector>
#include <string>
#include <unordered_map>

#include <glad/gl.h>

#include <Aether/Log/Log.h>
#include <Aether/Core/Assert.h>
#include <Aether/Platform/GraphicsContext.h>
#include <Aether/Renderer/VertexLayout.h>

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

    static GLenum ToGLInternalFormat(TextureFormat f) {
        switch (f) {
            case TextureFormat::RGBA8: return GL_RGBA8;
            case TextureFormat::RGB8:  return GL_RGB8;
        }
        return GL_RGBA8;
    }

    static GLenum ToGLFormat(TextureFormat f) {
        switch (f) {
            case TextureFormat::RGBA8: return GL_RGBA;
            case TextureFormat::RGB8:  return GL_RGB;
        }
        return GL_RGBA;
    }

    static GLint ToGLFilter(Filter f) {
        return (f == Filter::Nearest) ? GL_NEAREST : GL_LINEAR;
    }

    static GLint ToGLWrap(Wrap w) {
        return (w == Wrap::Clamp) ? GL_CLAMP_TO_EDGE : GL_REPEAT;
    }

    static GLuint CompileStage(GLenum stage, const char* src, const char* debugName) {
        AETHER_ASSERT(src && src[0] != '\0');

        const GLuint sh = glCreateShader(stage);
        glShaderSource(sh, 1, &src, nullptr);
        glCompileShader(sh);

        GLint ok = 0;
        glGetShaderiv(sh, GL_COMPILE_STATUS, &ok);
        if (!ok) {
            GLint len = 0;
            glGetShaderiv(sh, GL_INFO_LOG_LENGTH, &len);
            std::string log(len, '\0');
            glGetShaderInfoLog(sh, len, &len, log.data());
            glDeleteShader(sh);

            AETHER_ASSERT_MSG(false, "Shader compile failed (%s)\n%s", debugName ? debugName : "unnamed", log.c_str());
        }
        return sh;
    }

    static GLuint LinkProgram(GLuint vs, GLuint fs, const char* debugName) {
        const GLuint prog = glCreateProgram();
        glAttachShader(prog, vs);
        glAttachShader(prog, fs);
        glLinkProgram(prog);

        GLint ok = 0;
        glGetProgramiv(prog, GL_LINK_STATUS, &ok);
        if (!ok) {
            GLint len = 0;
            glGetProgramiv(prog, GL_INFO_LOG_LENGTH, &len);
            std::string log(len, '\0');
            glGetProgramInfoLog(prog, len, &len, log.data());

            glDeleteProgram(prog);
            AETHER_ASSERT_MSG(false, "Program link failed (%s)\n%s", (debugName ? debugName : "unnamed"), log.c_str());
        }

        glDetachShader(prog, vs);
        glDetachShader(prog, fs);
        return prog;
    }

    class GLBackend::Impl {
    public:
        Impl() = default;
        ~Impl() = default;

        void Init(const BackendInitInfo& info) {
            m_Context = info.context;
            m_Width   = info.width;
            m_Height  = info.height;

            AETHER_ASSERT_MSG(m_Context, "GLBackend: GraphicsContext is null");
            AETHER_ASSERT_MSG(
                m_Context->GetAPI() == Aether::Platform::GraphicsAPI::OpenGL,
                "GLBackend: GraphicsContext is not OpenGL"
            );

            m_Context->MakeCurrent();

            s_LoaderContext = m_Context;
            AETHER_ASSERT_MSG(gladLoadGL(reinterpret_cast<GLADloadfunc>(GLGetProcAddress)), "Failed to initialize OpenGL via glad");
            s_LoaderContext = nullptr;

            const auto framebuffer_size = m_Context->GetFrameBufferSize();

            glViewport(0, 0, framebuffer_size.Width, framebuffer_size.Height);
            glEnable(GL_DEPTH_TEST);
        }

        void Shutdown() {
            m_Context = nullptr;
        }

        void BeginFrame() {
            AETHER_ASSERT_MSG(!m_InFrame, "GLBackend::BeginFrame called twice");
            m_InFrame = true;
        }

        void EndFrame() {
            AETHER_ASSERT_MSG(m_InFrame, "GLBackend::EndFrame without BeginFrame");
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
            AETHER_ASSERT_MSG(m_Context, "GLBackend::Present without context");
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

        ShaderHandle CreateShader(const ShaderDesc& desc) {
            const auto vs = CompileStage(GL_VERTEX_SHADER,   desc.vertexSource,   desc.debugName);
            const auto fs = CompileStage(GL_FRAGMENT_SHADER, desc.fragmentSource, desc.debugName);
            const auto prog = LinkProgram(vs, fs, desc.debugName);

            glDeleteShader(vs);
            glDeleteShader(fs);

            uint32_t id = m_NextShaderId++;
            m_Programs.emplace(id, GLShaderProgram{ .program = prog });
            return ShaderHandle{ id };
        }

        void DestroyShader(const ShaderHandle& handle) {
            const auto it = m_Programs.find(handle.id);
            if (it == m_Programs.end()) return;

            glDeleteProgram(it->second.program);
            m_Programs.erase(it);
        }

        [[nodiscard]] uint32_t GetShaderProgram(const ShaderHandle& handle) const {
            const auto it = m_Programs.find(handle.id);
            AETHER_ASSERT(it != m_Programs.end());
            return it->second.program;
        }

        PipelineHandle CreatePipeline(const PipelineDesc& desc) {
            GLPipeline pipeline{};
            pipeline.program  = GetShaderProgram(desc.shader);
            pipeline.cull = desc.cull;
            pipeline.depth = desc.depth;
            pipeline.blending = desc.blending;

            {
                const GLuint prog = pipeline.program;

                for (const auto& binding : desc.layout.uniformBufferLayout) {
                    const auto& name = binding.Name;

                    const GLuint blockIndex = glGetUniformBlockIndex(prog, name.c_str());
                    if (blockIndex != GL_INVALID_INDEX)
                        glUniformBlockBinding(prog, blockIndex, binding.Binding);
                }

                GLint loc = glGetUniformLocation(prog, "u_Albedo");
                if (loc >= 0) {
                    GLint prev = 0;
                    glGetIntegerv(GL_CURRENT_PROGRAM, &prev);

                    glUseProgram(prog);
                    glUniform1i(loc, 0);
                    glUseProgram(static_cast<GLuint>(prev));
                }
            }

            glGenVertexArrays(1, &pipeline.vao);
            glBindVertexArray(pipeline.vao);

            const auto& vertexLayout = desc.layout.vertexLayout;
            pipeline.stride = vertexLayout.stride;
            pipeline.attributes.assign(vertexLayout.attributes, vertexLayout.attributes + vertexLayout.attributeCount);

            glBindVertexArray(0);

            uint32_t id = m_NextPipelineId++;
            m_Pipelines.emplace(id, pipeline);

            return PipelineHandle{ id };
        }

        void DestroyPipeline(const PipelineHandle& handle) {
            const auto it = m_Pipelines.find(handle.id);
            if (it == m_Pipelines.end()) return;

            glDeleteVertexArrays(1, &it->second.vao);
            m_Pipelines.erase(it);
        }

        UniformBufferHandle CreateUniformBuffer(const UniformBufferDesc& desc) {
            GLuint ubo = 0;
            glGenBuffers(1, &ubo);
            glBindBuffer(GL_UNIFORM_BUFFER, ubo);
            glBufferData(GL_UNIFORM_BUFFER, desc.size, nullptr, GL_DYNAMIC_DRAW);

            const uint32_t handle = ++m_NextUniformBufferId;
            m_UniformBuffers[handle] = { ubo, desc.size };
            return { handle };
        }

        void DestroyUniformBuffer(const UniformBufferHandle& handle) {
            const auto it = m_UniformBuffers.find(handle.id);
            if (it == m_UniformBuffers.end())
                return;

            glDeleteBuffers(1, &it->second.id);
            m_UniformBuffers.erase(it);
        }

        void UpdateUniformBuffer(
            const UniformBufferHandle& handle,
            const void* data,
            uint32_t size,
            uint32_t offset
        ) {
            const auto& ubo = m_UniformBuffers[handle.id];
            glBindBuffer(GL_UNIFORM_BUFFER, ubo.id);
            glBufferSubData(GL_UNIFORM_BUFFER, offset, size, data);
        }

        TextureHandle CreateTexture2D(const TextureDesc& desc) {
            GLuint tex = 0;
            glGenTextures(1, &tex);
            glBindTexture(GL_TEXTURE_2D, tex);

            const GLenum internalFmt = ToGLInternalFormat(desc.format);
            const GLenum fmt = ToGLFormat(desc.format);

            glTexImage2D(
                GL_TEXTURE_2D,
                0,
                static_cast<GLint>(internalFmt),
                static_cast<GLsizei>(desc.width),
                static_cast<GLsizei>(desc.height),
                0,
                fmt,
                GL_UNSIGNED_BYTE,
                desc.data
            );

            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

            //glGenerateMipmap(GL_TEXTURE_2D);

            const uint32_t handle = ++m_NextTex;
            m_Textures2D[handle] = { tex, desc.width, desc.height, internalFmt, fmt };
            return { handle };
        }

        void DestroyTexture(const TextureHandle& texture) {
            if (!texture) return;

            const auto it = m_Textures2D.find(texture.id);
            if (it == m_Textures2D.end()) return;

            glDeleteTextures(1, &it->second.id);
            m_Textures2D.erase(it);
        }

        SamplerHandle CreateSampler(const SamplerDesc& desc) {
            GLuint s = 0;
            glGenSamplers(1, &s);

            glSamplerParameteri(s, GL_TEXTURE_MIN_FILTER, ToGLFilter(desc.min));
            glSamplerParameteri(s, GL_TEXTURE_MAG_FILTER, ToGLFilter(desc.mag));
            glSamplerParameteri(s, GL_TEXTURE_WRAP_S, ToGLWrap(desc.u));
            glSamplerParameteri(s, GL_TEXTURE_WRAP_T, ToGLWrap(desc.v));

            const uint32_t handle = ++m_NextSampler;
            m_Samplers[handle] = { s };
            return { handle };
        }

        void DestroySampler(const SamplerHandle& sampler) {
            if (!sampler) return;

            const auto it = m_Samplers.find(sampler.id);
            if (it == m_Samplers.end()) return;

            glDeleteSamplers(1, &it->second.id);
            m_Samplers.erase(it);
        }

        void BindTexture2D(const TextureHandle& texture, uint32_t slot) {
            glActiveTexture(GL_TEXTURE0 + slot);

            if (!texture) {
                glBindTexture(GL_TEXTURE_2D, 0);
                return;
            }

            const auto& t = m_Textures2D[texture.id];
            glBindTexture(GL_TEXTURE_2D, t.id);
        }

        void BindSampler(const SamplerHandle& sampler, uint32_t slot) {
            if (!sampler) {
                glBindSampler(slot, 0);
                return;
            }

            const auto& s = m_Samplers[sampler.id];
            glBindSampler(slot, s.id);
        }

        void BindPipeline(const PipelineHandle& handle) {
            const auto& pipe = m_Pipelines.at(handle.id);

            glUseProgram(pipe.program);

            if (pipe.depth == DepthTest::Disabled) {
                glDisable(GL_DEPTH_TEST);
            } else {
                glEnable(GL_DEPTH_TEST);
                glDepthFunc(GL_LEQUAL);
            }

            if (pipe.cull == CullMode::None) {
                glDisable(GL_CULL_FACE);
            } else {
                glEnable(GL_CULL_FACE);
                glCullFace(pipe.cull == CullMode::Back ? GL_BACK : GL_FRONT);
            }

            if (pipe.blending) {
                glEnable(GL_BLEND);
                glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
            } else {
                glDisable(GL_BLEND);
            }

            glBindVertexArray(pipe.vao);
            m_BoundPipelineId = handle.id;
            m_BoundVAO = pipe.vao;
        }

        void BindVertexBuffer(const BufferHandle& handle) {
            AETHER_ASSERT_MSG(m_BoundPipelineId != 0, "BindVertexBuffer called before BindPipeline");

            const auto& buf = m_Buffers.at(handle.id);
            const auto& pipe = m_Pipelines.at(m_BoundPipelineId);

            glBindVertexArray(pipe.vao);
            m_BoundVAO = pipe.vao;

            glBindBuffer(GL_ARRAY_BUFFER, buf.id);
            m_BoundVBO = buf.id;

            for (const auto& attr : pipe.attributes) {
                glEnableVertexAttribArray(attr.location);

                GLint componentCount = 0;
                switch (attr.format) {
                    case VertexFormat::Float:  componentCount = 1; break;
                    case VertexFormat::Float2: componentCount = 2; break;
                    case VertexFormat::Float3: componentCount = 3; break;
                    case VertexFormat::Float4: componentCount = 4; break;
                }

                glVertexAttribPointer(
                    attr.location,
                    componentCount,
                    GL_FLOAT,
                    attr.normalized ? GL_TRUE : GL_FALSE,
                    static_cast<GLsizei>(pipe.stride),
                    reinterpret_cast<const void*>(static_cast<uintptr_t>(attr.offset))
                );
            }
        }

        void BindIndexBuffer(const BufferHandle& handle) {
            AETHER_ASSERT_MSG(m_BoundPipelineId != 0, "BindIndexBuffer called before BindPipeline");

            const auto& buf = m_Buffers.at(handle.id);
            const auto& pipe = m_Pipelines.at(m_BoundPipelineId);

            glBindVertexArray(pipe.vao);
            m_BoundVAO = pipe.vao;

            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buf.id);
            m_BoundIBO = buf.id;
        }

        void BindUniformBuffer(const UniformBufferHandle& handle, uint32_t bindingSlot) {
            const auto& ubo = m_UniformBuffers[handle.id];
            glBindBufferBase(GL_UNIFORM_BUFFER, bindingSlot, ubo.id);
        }

        void Draw(uint32_t vertexCount, uint32_t firstVertex) {
            GLint buf = 0;
            glGetVertexAttribiv(0, GL_VERTEX_ATTRIB_ARRAY_BUFFER_BINDING, &buf);
            AETHER_ASSERT_MSG(buf != 0, "Attrib 0 has no VBO bound (VAO captured buffer 0)");
            glDrawArrays(GL_TRIANGLES, static_cast<GLint>(firstVertex), static_cast<GLint>(vertexCount));
        }

        void DrawIndexed(uint32_t indexCount, uint32_t firstIndex) {
            glDrawElements(
                GL_TRIANGLES,
                static_cast<GLint>(indexCount),
                GL_UNSIGNED_INT,
                reinterpret_cast<const void*>(firstIndex * sizeof(uint32_t))
            );
        }

        RenderSurfaceHandle CreateRenderSurface(const RenderSurfaceDesc& desc) {
            GLRenderSurface surface{};
            surface.width = desc.width;
            surface.height = desc.height;
            surface.internalFmt = ToGLInternalFormat(desc.colorFormat);
            surface.fmt = ToGLFormat(desc.colorFormat);

            // Color texture
            glGenTextures(1, &surface.colorTexture);
            glBindTexture(GL_TEXTURE_2D, surface.colorTexture);
            glTexImage2D(
                GL_TEXTURE_2D, 0,
                static_cast<GLint>(surface.internalFmt),
                static_cast<GLsizei>(desc.width),
                static_cast<GLsizei>(desc.height),
                0, surface.fmt, GL_UNSIGNED_BYTE, nullptr
            );
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
            glBindTexture(GL_TEXTURE_2D, 0);

            // Register in texture map so GetRenderSurfaceColorAttachment returns a usable TextureHandle
            const uint32_t texHandle = ++m_NextTex;
            m_Textures2D[texHandle] = { surface.colorTexture, desc.width, desc.height, surface.internalFmt, surface.fmt };
            surface.colorTextureHandle = texHandle;

            // Depth + stencil renderbuffer
            glGenRenderbuffers(1, &surface.depthRBO);
            glBindRenderbuffer(GL_RENDERBUFFER, surface.depthRBO);
            glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8,
                static_cast<GLsizei>(desc.width), static_cast<GLsizei>(desc.height));
            glBindRenderbuffer(GL_RENDERBUFFER, 0);

            // Framebuffer
            glGenFramebuffers(1, &surface.fbo);
            glBindFramebuffer(GL_FRAMEBUFFER, surface.fbo);
            glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, surface.colorTexture, 0);
            glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, surface.depthRBO);

            AETHER_ASSERT_MSG(
                glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE,
                "RenderSurface framebuffer is not complete"
            );

            glBindFramebuffer(GL_FRAMEBUFFER, 0);

            const uint32_t id = ++m_NextRenderSurfaceId;
            m_RenderSurfaces[id] = surface;
            return { id };
        }

        void DestroyRenderSurface(const RenderSurfaceHandle& handle) {
            const auto it = m_RenderSurfaces.find(handle.id);
            if (it == m_RenderSurfaces.end()) return;

            const auto& surface = it->second;

            glDeleteFramebuffers(1, &surface.fbo);
            glDeleteRenderbuffers(1, &surface.depthRBO);

            // Remove color texture from the texture map and delete it
            m_Textures2D.erase(surface.colorTextureHandle);
            glDeleteTextures(1, &surface.colorTexture);

            m_RenderSurfaces.erase(it);
        }

        void ResizeRenderSurface(const RenderSurfaceHandle& handle, uint32_t width, uint32_t height) {
            const auto it = m_RenderSurfaces.find(handle.id);
            AETHER_ASSERT(it != m_RenderSurfaces.end());

            auto& surface = it->second;

            if (surface.width == width && surface.height == height) return;

            surface.width = width;
            surface.height = height;

            // Recreate color texture at new size
            glBindTexture(GL_TEXTURE_2D, surface.colorTexture);
            glTexImage2D(
                GL_TEXTURE_2D, 0,
                static_cast<GLint>(surface.internalFmt),
                static_cast<GLsizei>(width),
                static_cast<GLsizei>(height),
                0, surface.fmt, GL_UNSIGNED_BYTE, nullptr
            );
            glBindTexture(GL_TEXTURE_2D, 0);

            // Update the texture map entry
            m_Textures2D[surface.colorTextureHandle] = {
                surface.colorTexture, width, height, surface.internalFmt, surface.fmt
            };

            // Recreate depth renderbuffer at new size
            glBindRenderbuffer(GL_RENDERBUFFER, surface.depthRBO);
            glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8,
                static_cast<GLsizei>(width), static_cast<GLsizei>(height));
            glBindRenderbuffer(GL_RENDERBUFFER, 0);
        }

        TextureHandle GetRenderSurfaceColorAttachment(const RenderSurfaceHandle& handle) {
            const auto it = m_RenderSurfaces.find(handle.id);
            AETHER_ASSERT(it != m_RenderSurfaces.end());

            return { it->second.colorTextureHandle };
        }

        void BeginRenderSurface(const RenderSurfaceHandle& handle) {
            const auto it = m_RenderSurfaces.find(handle.id);
            AETHER_ASSERT(it != m_RenderSurfaces.end());

            const auto& surface = it->second;

            glGetIntegerv(GL_FRAMEBUFFER_BINDING, &m_PreviousFBO);
            glBindFramebuffer(GL_FRAMEBUFFER, surface.fbo);
            glViewport(0, 0, static_cast<GLsizei>(surface.width), static_cast<GLsizei>(surface.height));
        }

        void EndRenderSurface() {
            glBindFramebuffer(GL_FRAMEBUFFER, static_cast<GLuint>(m_PreviousFBO));
            m_PreviousFBO = 0;
        }

    private:
        struct GLBuffer {
            GLuint id;
            GLenum target;
            uint32_t size;
        };

        struct GLShaderProgram {
            GLuint program = 0;
        };

        struct GLPipeline {
            GLuint program = 0;
            GLuint vao = 0;

            CullMode cull{};
            DepthTest depth{};
            bool blending{};

            uint32_t stride = 0;
            std::vector<VertexAttribute> attributes = { };
        };

        struct GLUniformBuffer {
            GLuint id = 0;
            uint32_t size = 0;
            UniformUsage usage;
        };

        struct GLTexture2D {
            GLuint id = 0;
            uint32_t w=0, h=0;
            GLenum internalFmt=0, fmt=0;
        };

        struct GLSampler {
            GLuint id = 0;
        };

        struct GLRenderSurface {
            GLuint fbo = 0;
            GLuint colorTexture = 0;
            GLuint depthRBO = 0;
            uint32_t width = 0;
            uint32_t height = 0;
            GLenum internalFmt = 0;
            GLenum fmt = 0;
            uint32_t colorTextureHandle = 0;
        };

    private:
        Platform::GraphicsContext* m_Context = nullptr;

        unsigned int m_Width  = 0;
        unsigned int m_Height = 0;

        float m_ClearColor[4] = { 0.f, 0.f, 0.f, 1.f };
        bool m_InFrame = false;

        std::unordered_map<uint32_t, GLBuffer> m_Buffers;
        uint32_t m_NextBufferId = 1;

        std::unordered_map<uint32_t, GLShaderProgram> m_Programs;
        uint32_t m_NextShaderId = 1;

        std::unordered_map<uint32_t, GLPipeline> m_Pipelines;
        uint32_t m_NextPipelineId = 1;

        uint32_t m_BoundPipelineId = 0;
        uint32_t m_BoundVAO = 0;
        uint32_t m_BoundVBO = 0;
        uint32_t m_BoundIBO = 0;

        std::unordered_map<uint32_t, GLUniformBuffer> m_UniformBuffers;
        uint32_t m_NextUniformBufferId = 1;

        std::unordered_map<uint32_t, GLTexture2D> m_Textures2D;
        uint32_t m_NextTex = 0;

        std::unordered_map<uint32_t, GLSampler> m_Samplers;
        uint32_t m_NextSampler = 0;

        std::unordered_map<uint32_t, GLRenderSurface> m_RenderSurfaces;
        uint32_t m_NextRenderSurfaceId = 0;
        GLint m_PreviousFBO = 0;
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

    void GLBackend::SetViewport(int width, int height) {
        glViewport(0, 0, width, height);
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

    ShaderHandle GLBackend::CreateShader(const ShaderDesc& desc) {
        return m_Impl->CreateShader(desc);
    }

    void GLBackend::DestroyShader(const ShaderHandle& handle) {
        m_Impl->DestroyShader(handle);
    }

    uint32_t GLBackend::GetShaderProgram(const ShaderHandle& handle) const {
        return m_Impl->GetShaderProgram(handle);
    }

    PipelineHandle GLBackend::CreatePipeline(const PipelineDesc& desc) {
        return m_Impl->CreatePipeline(desc);
    }

    void GLBackend::DestroyPipeline(const PipelineHandle& handle) {
        m_Impl->DestroyPipeline(handle);
    }

    UniformBufferHandle GLBackend::CreateUniformBuffer(const UniformBufferDesc& desc) {
        return m_Impl->CreateUniformBuffer(desc);
    }

    void GLBackend::DestroyUniformBuffer(const UniformBufferHandle& handle) {
        m_Impl->DestroyUniformBuffer(handle);
    }

    void GLBackend::UpdateUniformBuffer(
        const UniformBufferHandle& handle,
        const void *data,
        uint32_t size,
        uint32_t offset
    ) {
        return m_Impl->UpdateUniformBuffer(handle, data, size, offset);
    }

    TextureHandle GLBackend::CreateTexture2D(const TextureDesc& desc) {
        return m_Impl->CreateTexture2D(desc);
    }

    void GLBackend::DestroyTexture(const TextureHandle& texture) {
        m_Impl->DestroyTexture(texture);
    }

    SamplerHandle GLBackend::CreateSampler(const SamplerDesc& desc) {
        return m_Impl->CreateSampler(desc);
    }

    void GLBackend::DestroySampler(const SamplerHandle& sampler) {
        m_Impl->DestroySampler(sampler);
    }

    void GLBackend::BindTexture2D(const TextureHandle& texture, uint32_t slot) {
        m_Impl->BindTexture2D(texture, slot);
    }

    void GLBackend::BindSampler(const SamplerHandle& sampler, uint32_t slot) {
        m_Impl->BindSampler(sampler, slot);
    }

    RenderSurfaceHandle GLBackend::CreateRenderSurface(const RenderSurfaceDesc& desc) {
        return m_Impl->CreateRenderSurface(desc);
    }

    void GLBackend::DestroyRenderSurface(const RenderSurfaceHandle& handle) {
        m_Impl->DestroyRenderSurface(handle);
    }

    void GLBackend::ResizeRenderSurface(const RenderSurfaceHandle& handle, uint32_t width, uint32_t height) {
        m_Impl->ResizeRenderSurface(handle, width, height);
    }

    TextureHandle GLBackend::GetRenderSurfaceColorAttachment(const RenderSurfaceHandle& handle) {
        return m_Impl->GetRenderSurfaceColorAttachment(handle);
    }

    void GLBackend::BeginRenderSurface(const RenderSurfaceHandle& handle) {
        m_Impl->BeginRenderSurface(handle);
    }

    void GLBackend::EndRenderSurface() {
        m_Impl->EndRenderSurface();
    }

    void GLBackend::BindPipeline(const PipelineHandle& handle) {
        m_Impl->BindPipeline(handle);
    }

    void GLBackend::BindVertexBuffer(const BufferHandle& handle) {
        m_Impl->BindVertexBuffer(handle);
    }

    void GLBackend::BindIndexBuffer(const BufferHandle& handle) {
        m_Impl->BindIndexBuffer(handle);
    }

    void GLBackend::BindUniformBuffer(const UniformBufferHandle& handle, uint32_t bindingSlot) {
        m_Impl->BindUniformBuffer(handle, bindingSlot);
    }

    void GLBackend::Draw(uint32_t vertexCount, uint32_t firstVertex) {
        m_Impl->Draw(vertexCount, firstVertex);
    }

    void GLBackend::DrawIndexed(uint32_t indexCount, uint32_t firstIndex) {
        m_Impl->DrawIndexed(indexCount, firstIndex);
    }
}
