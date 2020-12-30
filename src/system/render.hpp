
#pragma once
#include "../__common.hpp"
#include "../entity.hpp"
#include "../exception.hpp"
#include "../quadtree.hpp"
#include "../sprite.hpp"
#include "../window.hpp"
#include "system.hpp"

#include <unordered_map>

namespace p201
{
namespace systems
{
class render : public system
{
public:
    using __base = system;

protected:
    SDL_Renderer* __sdl_renderer = nullptr;
    matrix_3      iso_matrix;

private:
    void transform_tile(const vector_3&, std::size_t, std::size_t,
                        std::int16_t*, std::int16_t*);
    void render_grid(SDL_Renderer*, std::size_t, std::uint8_t);
    void render_sprite(SDL_Texture*, SDL_FRect*);

    void render_node(std::size_t, std::size_t, const vector_3&);
    void render_quadtree(const quadtree&);

    SDL_FRect camera_transform(const SDL_FRect&);
    void      camera_transform(std::int16_t*, std::int16_t*);

public:
    render(class world& world) : __base(world)
    {
        flag.set(components::render::flag);
        flag.set(components::transform::flag);
    }
    virtual ~render()
    {
        stop();
    }
    virtual void  start() override;
    void          stop();
    SDL_Renderer* sdl_renderer()
    {
        return __sdl_renderer;
    }
    virtual void update() override { }
    void         render_frame();

    //	Preventing copying and moving
    render(const render&) = delete;
    render(render&&)      = delete;
    render& operator=(const render&) = delete;
    render& operator=(render&&) = delete;
};
} // namespace systems
} // namespace p201
