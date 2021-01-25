
#pragma once
#include "window.hpp"
#include "world.hpp"

#include <__common.hpp>
#include <ecs.hpp>
#include <event.hpp>
#include <functional>
#include <input.hpp>
#include <physx.hpp>
#include <thread>
#include <util.hpp>
#include <vulkan.hpp>

namespace p201
{

class engine
{
public:
    static constexpr double dt    = 1.0 / 60.0;
    static constexpr bool   vsync = true;

private:
    std::function<void(float)> render_function;

protected:
    vulkan    vulkan;
    window    window;
    keyboard& keyboard;
    mouse&    mouse;
    clock     clock;

    bool  __running = false;
    float fps       = 60.0f;

    /** @brief Checks for CMD-W or CTRL-W to close the window. */
    bool window_close_key() const
    {
#if defined(P201_OS_MACOS)
        return (keyboard.down(keycode::W) && keyboard.modifier(modifier::GUI));
#elif defined(P201_OS_WINDOWS)
        return (keyboard.down(keycode::W) && keyboard.modifier(modifier::CTRL));
#else
        return false;
#endif
    }

public:
    world world;

public:
    engine();
    ~engine() = default;
    void start();
    void stop();
    void update(float dt);
    void render(float alpha);

    void bind_render_function(const std::function<void(float)>&);

    engine(const engine&) = delete;
    engine(engine&&)      = delete;
    engine& operator=(const engine&) = delete;
    engine& operator=(engine&&) = delete;
};

} // namespace p201
