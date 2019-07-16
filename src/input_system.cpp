
#include	"__common.hpp"
#include	"input_system.hpp"
#include	"entity.hpp"

__begin_ns_td

void	input_system::read(class keyboard& keyboard, class mouse& mouse)
{
	for (auto& entity : __registered_entities)
	{
		auto&	input = entity.second.get().component<input_component>();
		for (auto& mapping : input.key_mappings)
		{
			if (keyboard.down(mapping.second.first) && keyboard.modifier(mapping.second.second))
			{
				std::cout << int(mapping.first) << std::endl;
			}
		}
		for (auto& mapping : input.mouse_mappings)
		{
			if (mouse.down(mapping.second.first) && keyboard.modifier(mapping.second.second))
			{
				
			}
		}
	}
}

__end_ns_td
