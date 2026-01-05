#include <memory>

#include "SandboxApp.h"

int main(int argc, char** argv) {
    const auto app = std::make_unique<SandboxApp>(argc, argv);
    app->Run();

    return 0;
}
