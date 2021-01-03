
#pragma once
#include "__common.hpp"
#include "clock.hpp"
#include "entity_manager.hpp"
#include "event.hpp"
#include "input.hpp"
#include "serialize.hpp"
#include "system.hpp"
#include "window.hpp"
#include "world.hpp"

#include <chrono>
#include <thread>

namespace p201
{

class engine
{
protected:
    window    window;
    keyboard& keyboard;
    mouse&    mouse;
    clock     clock;
    world     world;

    bool        __running = false;
    const float fps       = 60.0f;
    const float __dt      = 1.0f / global::dt_factor;

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
    engine()
        : window("Prototype 201"),
          keyboard(window.keyboard),
          mouse(window.mouse),
          world(window, keyboard, mouse)
    {
        window.start();
        for (auto& system : world.systems) system.second->start();

        world.serializer.directory = "entities";
        world.serializer.load_entity(world.new_entity(), "player");
        world.serializer.load_entity(world.new_entity(), "platform");
    }
    ~engine() = default;
    void start()
    {
        if (__running) return;
        __running = true;

        double time_prev   = clock.time_s();
        double accumulator = 0.0;

        while (__running)
        {
            bool idle = true;

            double time_next  = clock.time_s();
            double time_frame = time_next - time_prev;

            time_prev = time_next;
            accumulator += time_frame;

            while (accumulator >= __dt)
            {
                if (window.closed()) __running = false;
                update(__dt);
                accumulator -= __dt;
                idle = false;
            }
            if (!idle)
            {
                display();
                if (window_close_key()) stop();
            }
            else
                std::this_thread::sleep_for(std::chrono::milliseconds(1));
        }
    }
    void stop()
    {
        __running = false;
    }
    void display()
    {
        world.system<systems::render>().display();
    }
    void update(float dt)
    {
        window.update();
        for (auto& system : world.systems) system.second->update(dt);
    }

    // Preventing copying and moving
    engine(const engine&) = delete;
    engine(engine&&)      = delete;
    engine& operator=(const engine&) = delete;
    engine& operator=(engine&&) = delete;
};

} // namespace p201
