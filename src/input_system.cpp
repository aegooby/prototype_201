
#include "input_system.hpp"

#include "__common.hpp"
#include "entity.hpp"
#include "event.hpp"
#include "event_bus.hpp"
#include "input.hpp"
#include "world.hpp"

namespace p201
{

void input_system::start() { }
void input_system::update()
{
    auto& keyboard = world.keyboard;
    auto& mouse    = world.mouse;
    (void)mouse;
    for (auto& entity : __registered_entities)
    {
        auto& movement = entity.second.get().component<movement_component>();

        // Right
        if (keyboard.down(keycode::D)) movement.acceleration.x() += 10.0f;
        if (keyboard.up(keycode::D)) movement.acceleration.x() += -10.0f;

        // Left
        if (keyboard.down(keycode::A)) movement.acceleration.x() += -10.0f;
        if (keyboard.up(keycode::A)) movement.acceleration.x() += 10.0f;

        // Up
        if (keyboard.down(keycode::W)) movement.acceleration.y() += -10.0f;
        if (keyboard.up(keycode::W)) movement.acceleration.y() += 10.0f;

        // Down
        if (keyboard.down(keycode::S)) movement.acceleration.y() += 10.0f;
        if (keyboard.up(keycode::S)) movement.acceleration.y() += -10.0f;

        // Dash
        if (keyboard.down(keycode::SPACE)) movement.velocity.x() += 50.0;
    }
}

} // namespace p201
