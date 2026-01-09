//
// Created by ThewyRogue99 on 8.01.2026.
//

#pragma once

#include <utility>

#include <Aether/Input/KeyCode.h>
#include <Aether/Input/MouseButton.h>

namespace Aether::Engine {
    class Event;
    class KeyPressedEvent;
    class KeyReleasedEvent;
    class MouseButtonPressedEvent;
    class MouseButtonReleasedEvent;
    class MouseMovedEvent;

    class Input {
    public:
        static bool IsKeyPressed(KeyCode key);

        static bool IsMouseButtonPressed(MouseButton button);

        static std::pair<float, float> GetMousePosition();
        static float GetMouseX();
        static float GetMouseY();

        static void OnEvent(Event& e);

    private:
        static bool OnKeyPressed(KeyPressedEvent& e);
        static bool OnKeyReleased(KeyReleasedEvent& e);

        static bool OnMouseButtonPressed(MouseButtonPressedEvent& e);
        static bool OnMouseButtonReleased(MouseButtonReleasedEvent& e);

        static bool OnMouseMoved(MouseMovedEvent& e);
    };
}