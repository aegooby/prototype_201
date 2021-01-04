
#include "collision.hpp"

#include "../__common.hpp"
#include "../entity_manager.hpp"
#include "../event.hpp"
#include "../world.hpp"

namespace p201
{
namespace systems
{
void collision::start()
{
    quadtree.start(4, 1);
    quadtree.bounds(100.0f, 100.0f, 600.0f, 600.0f);
}

bool collision::hitbox_check(const std::unique_ptr<hitbox>& __a,
                             const std::unique_ptr<hitbox>& __b)
{
    auto __ptr_a = __a.get();
    auto __ptr_b = __b.get();

    if (typeid(*__ptr_a) == typeid(hitboxes::circle) &&
        typeid(*__ptr_b) == typeid(hitboxes::circle))
    {
        auto& __circle_a = *dynamic_cast<hitboxes::circle*>(__ptr_a);
        auto& __circle_b = *dynamic_cast<hitboxes::circle*>(__ptr_b);
        return circle_check(__circle_a, __circle_b);
    }
    if (typeid(*__ptr_a) == typeid(hitboxes::square) &&
        typeid(*__ptr_b) == typeid(hitboxes::square))
    {
        auto& __square_a = *dynamic_cast<hitboxes::square*>(__ptr_a);
        auto& __square_b = *dynamic_cast<hitboxes::square*>(__ptr_b);
        return square_check(__square_a, __square_b);
    }
    if (typeid(*__ptr_a) == typeid(hitboxes::circle) &&
        typeid(*__ptr_b) == typeid(hitboxes::square))
    {
        auto& __circle = *dynamic_cast<hitboxes::circle*>(__ptr_a);
        auto& __square = *dynamic_cast<hitboxes::square*>(__ptr_b);
        return hybrid_check(__circle, __square);
    }
    if (typeid(*__ptr_a) == typeid(hitboxes::square) &&
        typeid(*__ptr_b) == typeid(hitboxes::circle))
    {
        auto& __square = *dynamic_cast<hitboxes::square*>(__ptr_a);
        auto& __circle = *dynamic_cast<hitboxes::circle*>(__ptr_b);
        return hybrid_check(__circle, __square);
    }
    throw std::runtime_error("Unknown hitbox types");
}

bool collision::point_in_square(const vector_3&         point,
                                const hitboxes::square& square)
{
    return (square.left() <= point.x() && square.right() >= point.x() &&
            square.top() <= point.y() && square.bottom() >= point.y());
}

bool collision::hybrid_check(const hitboxes::circle& circle,
                             const hitboxes::square& square)
{
    float circleDistance_x = abs(circle.center(0) - square.center(0));
    float circleDistance_y = abs(circle.center(1) - square.center(1));

    if (circleDistance_x > (square.width / 2 + circle.radius)) { return false; }
    if (circleDistance_y > (square.height / 2 + circle.radius))
    {
        return false;
    }

    if (circleDistance_x <= (square.width / 2)) { return true; }
    if (circleDistance_y <= (square.height / 2)) { return true; }

    float cornerDistance_sq = pow((circleDistance_x - square.width / 2), 2) +
                              pow((circleDistance_y - square.height / 2), 2);

    return (cornerDistance_sq <= (pow(circle.radius, 2)));
}

bool collision::circle_check(const hitboxes::circle& __a,
                             const hitboxes::circle& __b)
{
    return (__a.radius + __b.radius > distance(__a.center, __b.center));
}

bool collision::square_check(const hitboxes::square& __a,
                             const hitboxes::square& __b)
{

    return ((__a.left() <= __b.right()) && (__a.right() >= __b.left()) &&
            (__a.bottom() >= __b.top()) && (__a.top() <= __b.bottom()));
}

void collision::quad_check(class quadtree& quadtree)
{
    std::list<std::vector<std::size_t>> entity_list = quadtree.leaves();
    for (auto& node_entities : entity_list)
    {
        for (int i = 0; i < node_entities.size(); i++)
        {
            for (int j = 0; j < node_entities.size(); j++)
            {
                std::size_t id1 = node_entities[i];
                std::size_t id2 = node_entities[j];
                if (hitbox_check(world.entity(id1).component<components::collision>().hitbox, world.entity(id2).component<components::collision>().hitbox))
                {
                    //send collision event between the two entities
                }
            }
        }
    }
}

void collision::update(float dt)
{
    quadtree.remove(__registered_entities);
    quadtree.insert(__registered_entities);
    quad_check(quadtree);
    for (auto& id : __registered_entities)
    {
        auto& entity    = world.entity(id);
        auto& transform = entity.component<components::transform>();
        auto& collision = entity.component<components::collision>();

        collision.hitbox->center.x() = transform.position.x();
        collision.hitbox->center.y() = transform.position.y();
    }
}
} // namespace systems
} // namespace p201
