
#pragma once
#include "__common.hpp"
#include "component.hpp"
#include "entity.hpp"
#include "hitbox.hpp"

namespace p201
{

struct quadrant
{
    float x;
    float y;
    float w;
    float h;

    constexpr quadrant(float x, float y, float w, float h) noexcept
        : x(x), y(y), w(w), h(h)
    {
    }
    ~quadrant() = default;

    constexpr float right() const noexcept
    {
        return x + w;
    }
    constexpr float left() const noexcept
    {
        return x;
    }
    constexpr float bottom() const noexcept
    {
        return y + h;
    }
    constexpr float top() const noexcept
    {
        return y;
    }
    vector_2 center() const noexcept
    {
        return vector_2(x + w / 2.0f, y + h / 2.0f);
    }
    constexpr bool contains(const quadrant& other) const noexcept
    {
        return left() <= other.left() && other.right() <= right() &&
               top() <= other.top() && other.bottom() <= bottom();
    }
    constexpr bool intersects(const quadrant& other) const noexcept
    {
        return !(left() >= other.right() || right() <= other.left() ||
                 top() >= other.bottom() || bottom() <= other.top());
    }
};

inline constexpr bool intersect(const quadrant& one,
                                const quadrant& two) noexcept
{
    return !(one.left() >= two.right() || one.right() <= two.left() ||
             one.top() >= two.bottom() || one.bottom() <= two.top());
}

class quadtree
{
public:
    struct node
    {
        std::array<std::unique_ptr<node>, 4> children;
        std::vector<float>                   values;

        bool leaf() const
        {
            return !static_cast<bool>(children.at(0));
        }
    };

protected:
    static constexpr size_t threshold = 16;
    static constexpr size_t max_depth = 8;

    quadrant              main_box;
    std::unique_ptr<node> root;
};

} // namespace p201
