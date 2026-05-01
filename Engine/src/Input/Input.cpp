//
// Created by ThewyRogue99 on 8.01.2026.
//

#include <Aether/Input/Input.h>

#include <Aether/Events/KeyEvent.h>
#include <Aether/Events/MouseEvent.h>
#include <Aether/Events/EventDispatcher.h>

namespace {
    bool s_KeyStates[512] = { };
    bool s_MouseButtonStates[8] = { };
    float s_MouseX = 0.0f;
    float s_MouseY = 0.0f;
}

namespace Aether::Engine {
    bool Input::IsKeyPressed(KeyCode key) {
        const int index = static_cast<int>(key);
        if (index < 0 || index >= 512)
            return false;

        return s_KeyStates[index];
    }

    bool Input::IsMouseButtonPressed(MouseButton button) {
        const int index = static_cast<int>(button);
        if (index < 0 || index >= 8)
            return false;

        return s_MouseButtonStates[index];
    }

    std::pair<float, float> Input::GetMousePosition() {
        return { s_MouseX, s_MouseY };
    }

    float Input::GetMouseX() {
        return s_MouseX;
    }

    float Input::GetMouseY() {
        return s_MouseY;
    }

    void Input::OnEvent(Event& e) {
        EventDispatcher dispatcher(e);

        dispatcher.Dispatch<KeyPressedEvent>(OnKeyPressed);
        dispatcher.Dispatch<KeyReleasedEvent>(OnKeyReleased);

        dispatcher.Dispatch<MouseButtonPressedEvent>(OnMouseButtonPressed);
        dispatcher.Dispatch<MouseButtonReleasedEvent>(OnMouseButtonReleased);

        dispatcher.Dispatch<MouseMovedEvent>(OnMouseMoved);
    }

    bool Input::OnKeyPressed(KeyPressedEvent& e) {
        const int index = static_cast<int>(e.GetKeyCode());
        if (index >= 0 && index < 512)
            s_KeyStates[index] = true;

        return false;
    }

    bool Input::OnKeyReleased(KeyReleasedEvent& e) {
        const int index = static_cast<int>(e.GetKeyCode());
        if (index >= 0 && index < 512)
            s_KeyStates[index] = false;

        return false;
    }

    bool Input::OnMouseButtonPressed(MouseButtonPressedEvent& e) {
        const int index = static_cast<int>(e.GetMouseButton());
        if (index >= 0 && index < 8)
            s_MouseButtonStates[index] = true;

        return false;
    }

    bool Input::OnMouseButtonReleased(MouseButtonReleasedEvent& e) {
        const int index = static_cast<int>(e.GetMouseButton());
        if (index >= 0 && index < 8)
            s_MouseButtonStates[index] = false;

        return false;
    }

    bool Input::OnMouseMoved(MouseMovedEvent& e) {
        s_MouseX = e.GetX();
        s_MouseY = e.GetY();

        return false;
    }
}
