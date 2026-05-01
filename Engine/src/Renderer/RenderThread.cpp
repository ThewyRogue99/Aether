//
// Created by ThewyRogue99 on 11.01.2026.
//

#include <Aether/Renderer/RenderThread.h>

#include <thread>
#include <mutex>
#include <condition_variable>
#include <deque>
#include <atomic>

namespace Aether::Renderer {
    class RenderThread::Impl {
    public:
        Impl() = default;
        ~Impl() = default;

        std::thread thread;
        std::mutex mutex;
        std::condition_variable cv;
        std::deque<Command> queue;
        std::condition_variable spaceCv;
        size_t maxQueueSize = 8;

        bool stopRequested = false;

        std::mutex flushMutex;
        std::condition_variable flushCv;
        uint64_t enqueuedCount = 0;
        uint64_t executedCount = 0;

        void ThreadMain() {
            while (true) {
                Command cmd;

                {
                    std::unique_lock lock(mutex);
                    cv.wait(lock, [&] { return stopRequested || !queue.empty(); });

                    if (stopRequested && queue.empty())
                        break;

                    cmd = std::move(queue.front());
                    queue.pop_front();
                    spaceCv.notify_one();
                }

                if (cmd) cmd();

                {
                    std::lock_guard lock(flushMutex);
                    ++executedCount;
                }
                flushCv.notify_all();
            }
        }
    };

    RenderThread::RenderThread() : m_Impl(Engine::MakeScope<Impl>()) { }

    RenderThread::~RenderThread() { Stop(); }

    void RenderThread::Start() {
        if (m_Impl->thread.joinable())
            return;

        m_Impl->stopRequested = false;
        m_Impl->thread = std::thread([this] { m_Impl->ThreadMain(); });
    }

    void RenderThread::Stop() {
        {
            std::lock_guard lock(m_Impl->mutex);
            m_Impl->stopRequested = true;
        }
        m_Impl->cv.notify_all();
        m_Impl->spaceCv.notify_all();

        if (m_Impl->thread.joinable()) {
            if (std::this_thread::get_id() == m_Impl->thread.get_id()) {
                return;
            }

            m_Impl->thread.join();
        }
    }

    void RenderThread::Enqueue(Command cmd) {
        {
            std::unique_lock lock(m_Impl->mutex);
            if (m_Impl->stopRequested)
                return;

            m_Impl->spaceCv.wait(lock, [&] {
                return m_Impl->stopRequested || m_Impl->queue.size() < m_Impl->maxQueueSize;
            });

            if (m_Impl->stopRequested)
                return;

            m_Impl->queue.emplace_back(std::move(cmd));
            ++m_Impl->enqueuedCount;
        }
        m_Impl->cv.notify_one();
    }

    void RenderThread::Flush() {
        const uint64_t target = m_Impl->enqueuedCount;
        std::unique_lock lock(m_Impl->flushMutex);
        m_Impl->flushCv.wait(lock, [&] {
            return m_Impl->executedCount >= target || m_Impl->stopRequested;
        });
    }

    bool RenderThread::IsRunning() const {
        return m_Impl->thread.joinable() && !m_Impl->stopRequested;
    }
}