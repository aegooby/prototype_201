
#pragma once
#include <__common.hpp>
#include <shaderc/shaderc.hpp>
#include <vulkan/vulkan.hpp>

namespace p201
{
class vulkan
{
private:
    std::uint32_t graphics_index = 0;
    std::uint32_t present_index  = 0;

    /** @todo Triple buffering. */
    static constexpr std::size_t swapchain_image_count = 2;
    static constexpr vk::Format  format = vk::Format::eB8G8R8A8Unorm;
    std::vector<std::uint32_t>   queue_family_indices;
    vk::Extent2D                 extent;

    /* SHADER */
    static constexpr auto shader_infer   = shaderc_glsl_infer_from_source;
    static constexpr auto shader_success = shaderc_compilation_status_success;

protected:
    /* SHADER */
    shaderc::Compiler                    spirv;
    [[nodiscard]] vk::UniqueShaderModule create_shader(const std::string&);

public:
    /**
     * @brief Connects application to Vulkan library.
     * @see vulkan::create_instance()
     */
    vk::UniqueInstance instance;
    /**
     * @brief Connects Vulkan instance to window. Requires adding extension
     *        "VK_KHR_surface" (done by SDL/GLFW).
     * @see vulkan::create_surface()
     */
    vk::UniqueSurfaceKHR surface;
    /**
     * @brief Represents the GPU used.
     * @todo Impement functionality for external + Intel integrated GPU.
     */
    vk::PhysicalDevice gpu;
    /**
     * @brief Interface for physical device(s).
     * @see vulkan::create_device()
     */
    vk::UniqueDevice device;
    /**
     * @brief Contains the frame buffers used in rendering and swapping.
     *        The number of buffers is set by vulkan::swapchain_image_count.
     * @see vulkan::create_swapchain()
     */
    vk::UniqueSwapchainKHR swapchain;
    /** @brief Views into the buffers in the swapchain. */
    std::vector<vk::UniqueImageView> image_views;
    vk::UniquePipelineLayout         pipeline_layout;

    vulkan()  = default;
    ~vulkan() = default;

    /* INIT */
    /** @see vulkan::instance */
    void create_instance(handle_types::window*);
    /** @see vulkan::surface */
    void create_surface(handle_types::window*);
    /** @see vulkan::device */
    void create_device();
    /** @see vulkan::swapchain */
    void create_swapchain(handle_types::window*);

    /* PIPELINE */
    void create_pipeline();
};
} // namespace p201