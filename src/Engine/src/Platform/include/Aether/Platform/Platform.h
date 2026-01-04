//
// Created by ThewyRogue99 on 4.01.2026.
//

#pragma once

namespace Aether::Platform {
    class Platform {
    public:
        static void Init();

        static double GetTime();

        static void Shutdown();
    };
}
