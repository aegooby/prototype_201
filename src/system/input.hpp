
#pragma once
#include "../__common.hpp"
#include "../world.hpp"
#include "system.hpp"

namespace p201
{
namespace systems
{
class input : public system
{
public:
    using __base = system;

    input(class world& world) : __base(world)
    {
        flag.set(components::input::flag);
        flag.set(components::movement::flag);
    }
    virtual ~input() = default;
    virtual void start() override;
    virtual void update() override;
};
} // namespace systems
} // namespace p201
