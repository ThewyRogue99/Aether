#include <memory>

#include "EditorApp.h"

int main(int argc, char** argv) {
    const auto app = std::make_unique<Aether::Editor::EditorApp>(argc, argv);
    app->Run();

    return 0;
}
