#include "Kiss/pch.h"
#include <imgui.h>
#include <kinc/input/keyboard.h>
#include <kinc/input/mouse.h>

using namespace kiss;

namespace kiss {
    namespace imgui {
        namespace keyboard
        {
            static inline void init();
            static void key_down(int key_code);
            static void key_up(int key_code);
            static void key_press(unsigned character);

            static inline void init() {
                auto& io = ImGui::GetIO();
                kinc_keyboard_key_down_callback = key_down;
                kinc_keyboard_key_up_callback = key_up;
                kinc_keyboard_key_press_callback = key_press;

                // Keyboard mapping. ImGui will use those indices to peek into the io.KeysDown[] array.
                io.KeyMap[ImGuiKey_Tab] = KINC_KEY_TAB;
                io.KeyMap[ImGuiKey_LeftArrow] = KINC_KEY_LEFT;
                io.KeyMap[ImGuiKey_RightArrow] = KINC_KEY_RIGHT;
                io.KeyMap[ImGuiKey_UpArrow] = KINC_KEY_UP;
                io.KeyMap[ImGuiKey_DownArrow] = KINC_KEY_DOWN;
                io.KeyMap[ImGuiKey_PageUp] = KINC_KEY_PAGE_UP;
                io.KeyMap[ImGuiKey_PageDown] = KINC_KEY_PAGE_DOWN;
                io.KeyMap[ImGuiKey_Home] = KINC_KEY_HOME;
                io.KeyMap[ImGuiKey_End] = KINC_KEY_END;
                io.KeyMap[ImGuiKey_Insert] = KINC_KEY_INSERT;
                io.KeyMap[ImGuiKey_Delete] = KINC_KEY_DELETE;
                io.KeyMap[ImGuiKey_Backspace] = KINC_KEY_BACKSPACE;
                io.KeyMap[ImGuiKey_Space] = KINC_KEY_SPACE;
                io.KeyMap[ImGuiKey_Enter] = KINC_KEY_RETURN;
                io.KeyMap[ImGuiKey_Escape] = KINC_KEY_ESCAPE;
                io.KeyMap[ImGuiKey_KeyPadEnter] = KINC_KEY_RETURN;
                io.KeyMap[ImGuiKey_A] = KINC_KEY_A;
                io.KeyMap[ImGuiKey_C] = KINC_KEY_C;
                io.KeyMap[ImGuiKey_V] = KINC_KEY_V;
                io.KeyMap[ImGuiKey_X] = KINC_KEY_X;
                io.KeyMap[ImGuiKey_Y] = KINC_KEY_Y;
                io.KeyMap[ImGuiKey_Z] = KINC_KEY_Z;
            }

            static void key_down(int key_code)
            {
                auto& io = ImGui::GetIO();
                switch (key_code)
                {
                case KINC_KEY_SHIFT:
                    io.KeyShift = true;
                    break;
                case KINC_KEY_CONTROL:
                    io.KeyCtrl = true;
                    break;
                case KINC_KEY_ALT:
                case KINC_KEY_ALT_GR:
                    io.KeyAlt = true;
                    break;
                default:
                    IM_ASSERT(key_code >= 0 && key_code < IM_ARRAYSIZE(io.KeysDown));
                    io.KeysDown[key_code] = true;
                    break;
                }
            }

            static void key_up(int key_code)
            {
                auto& io = ImGui::GetIO();
                switch (key_code)
                {
                case KINC_KEY_SHIFT:
                    io.KeyShift = false;
                    break;
                case KINC_KEY_CONTROL:
                    io.KeyCtrl = false;
                    break;
                case KINC_KEY_ALT:
                case KINC_KEY_ALT_GR:
                    io.KeyAlt = false;
                    break;
                default:
                    IM_ASSERT(key_code >= 0 && key_code < IM_ARRAYSIZE(io.KeysDown));
                    io.KeysDown[key_code] = false;
                    break;
                }
            }

            static void key_press(unsigned character)
            {
                auto& io = ImGui::GetIO();
                char text[2];
                text[0] = character;
                text[1] = 0;
                io.AddInputCharactersUTF8(text);
            }
        }

        namespace mouse
        {
            static bool pressed[5] = { false, false, false, false, false };
            static bool pressedCurrently[5] = { false, false, false, false, false };

            static inline void  init();
            static inline void  update();
            static void         move(int window, int x, int y, int movement_x, int movement_y);
            static void         press(int window, int button, int x, int y);
            static void         release(int window, int button, int x, int y);
            static void         scroll(int window, int delta);

            static inline void init() {
                kinc_mouse_move_callback = move;
                kinc_mouse_press_callback = press;
                kinc_mouse_release_callback = release;
                kinc_mouse_scroll_callback = scroll;
            }

            static inline void update() {
                auto& io = ImGui::GetIO();
                for (int i = 0; i < 5; ++i)
                {
                    io.MouseDown[i] = pressed[i] || pressedCurrently[i];  // If a mouse press event came, always pass it as "mouse held this frame", so we don't miss click-release events that are shorter than 1 frame.
                    pressed[i] = false;
                }
            }

            static void move(int window, int x, int y, int movement_x, int movement_y)
            {
                auto& io = ImGui::GetIO();
                io.MousePos = ImVec2((float)x, (float)y);
            }

            static void press(int window, int button, int x, int y)
            {
                if (button < 5)
                {
                    auto& io = ImGui::GetIO();
                    io.MousePos = ImVec2((float)x, (float)y);
                    pressed[button] = true;
                    pressedCurrently[button] = true;
                }
            }

            static void release(int window, int button, int x, int y)
            {
                if (button < 5)
                {
                    auto& io = ImGui::GetIO();
                    io.MousePos = ImVec2((float)x, (float)y);
                    pressedCurrently[button] = false;
                }
            }

            static void scroll(int window, int delta)
            {
                auto& io = ImGui::GetIO();
                io.MouseWheel -= delta;
            }
        }

        namespace input {
            void init() {
                auto& io = ImGui::GetIO();
                io.BackendFlags |= ImGuiBackendFlags_HasMouseCursors;       // We can honor GetMouseCursor() values (optional)
                io.BackendFlags |= ImGuiBackendFlags_HasSetMousePos;        // We can honor io.WantSetMousePos requests (optional, rarely used)
                //io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
                //io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
                keyboard::init();
                mouse::init();
                /*touch::init();
                kinc_surface_touch_start_callback = ;
                kinc_surface_move_callback
                kinc_surface_touch_end_callback*/
            }

            void update() {
                mouse::update();
            }
        }
    }
}