
#pragma once
#include "../__common.hpp"
#include "../linalg.hpp"

#include <list>
#include <typeindex>
#include <unordered_map>

namespace p201
{

class event
{
public:
    event()          = default;
    virtual ~event() = 0;
};

inline event::~event() = default;

namespace events
{
class animation : public event
{
public:
    class entity& entity;
    std::string   name;

    animation(class entity& entity, const std::string& name)
        : entity(entity), name(name)
    {
    }
    virtual ~animation() = default;
};
class collision : public event
{
public:
    class entity& entity;

    collision(class entity& entity) : entity(entity) { }
    virtual ~collision() = default;
};
} // namespace events

} // namespace p201
