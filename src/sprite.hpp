
#pragma once
#include "__common.hpp"
#include "exception.hpp"
#include "util.hpp"

#include <filesystem>
#include <unordered_map>
#include <vector>

namespace p201
{

namespace sprite
{
    /**
     * @brief It shames me so much to have to put this in here that I might
     *        commit suicide like a disgraced Japanese office worker but this
     *        is here for sprite loading until I find a better way.
     */
    struct flipbook
    {
        /** @brief Textures in the flipbook (sequential). */
        std::vector<SDL_Texture*> textures;

        flipbook() = default;
        ~flipbook()
        {
            for (auto& texture : textures) SDL_DestroyTexture(texture);
        }
        std::size_t frames() const
        {
            return textures.size();
        }
        SDL_Texture* const& at(std::size_t i) const
        {
            return textures.at(i);
        }
        SDL_Texture*& at(std::size_t i)
        {
            return textures.at(i);
        }
    };
    class manager
    {
    protected:
        using flipbook_family = std::unordered_map<std::string, flipbook>;
        std::unordered_map<std::string, flipbook_family> families;
        std::filesystem::path                            main_path;

        SDL_Renderer* renderer = nullptr;

        void new_flipbook(const std::filesystem::path& path)
        {
            if (!std::filesystem::is_directory(path)) return;
            const auto& name   = path.filename();
            const auto& family = path.parent_path().filename();
            families.try_emplace(family, flipbook_family());
            families.at(family).emplace(name, sprite::flipbook());
            auto& flipbook = families.at(family).at(name);
            for (auto& entry : std::filesystem::directory_iterator(path))
            {
                const auto& path_str = entry.path().string();
                auto texture = IMG_LoadTexture(renderer, path_str.c_str());
                if (texture) flipbook.textures.emplace_back(texture);
            }
        }
        void delete_flipbook(const std::string& family, const std::string& name)
        {
            families.at(family).erase(name);
        }

    public:
        manager(const std::string& main_path) : main_path(main_path) { }
        ~manager() = default;
        void link(SDL_Renderer* renderer)
        {
            this->renderer = renderer;
        }
        void load()
        {
            for (auto& family : std::filesystem::directory_iterator(main_path))
            {
                const auto& path = family.path();
                if (!std::filesystem::is_directory(path)) continue;
                for (auto& name : std::filesystem::directory_iterator(path))
                    new_flipbook(name.path());
            }
        }
        flipbook& flipbook(std::string family, std::string name)
        {
            return families.at(family).at(name);
        }

        // Preventing copying and moving
        manager(const manager&) = delete;
        manager(manager&&)      = delete;
        manager& operator=(const manager&) = delete;
        manager& operator=(manager&&) = delete;
    };
} // namespace sprite

} // namespace p201
