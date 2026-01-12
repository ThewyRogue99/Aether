//
// Created by ThewyRogue99 on 11.01.2026.
//

#pragma once

#include <functional>

#include <Aether/Core/Memory/Pointer.h>

namespace Aether::Renderer {
    class RenderThread {
    public:
        using Command = std::function<void()>;

        RenderThread();
        ~RenderThread();

        RenderThread(const RenderThread&) = delete;
        RenderThread& operator=(const RenderThread&) = delete;

        void Start();
        void Stop();

        void Enqueue(Command cmd);
        void Flush();

        [[nodiscard]] bool IsRunning() const;

    private:
        class Impl;
        Engine::Scope<Impl> m_Impl;
    };
}
