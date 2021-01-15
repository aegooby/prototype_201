
#pragma once
#include "../__common.hpp"
#include "../entity.hpp"
#include "../event.hpp"
#include "../window.hpp"
#include "system.hpp"

namespace p201
{
namespace systems
{
class character : public system
{
public:
    using __base = system;

private:
    float direction_angle(const components::transform&);

public:
    character(class world& world) : __base(world)
    {
        flag.set(components::transform::flag);
        flag.set(components::physics::flag);
        flag.set(components::character::flag);
    }
    virtual ~character() override = default;
    virtual void start() override;
    virtual void update(float dt) override;

    character(const character&) = delete;
    character(character&&)      = delete;
    character& operator=(const character&) = delete;
    character& operator=(character&&) = delete;
};
} // namespace systems
} // namespace p201
