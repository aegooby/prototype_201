
#pragma	once
#include	"__common.hpp"
#include	"sprite.hpp"
#include	"key.hpp"
#include	"vector.hpp"
#include	<map>
#include	<unordered_map>
#include	<vector>
#include	<string>

__begin_ns_td

class	component
{
public:
	td::entity&	entity;
protected:
	const std::string	__id;
public:
	component(td::entity& entity, const std::string& id) : entity(entity), __id(id) {  }
	virtual ~component() = 0;
	inline __attribute__((always_inline))
	const std::string&	id() const
	{
		return __id;
	}
};


class	render_component : public component
{
public:
	using __base = component;
public:
	std::unordered_map<std::string, sprite_flipbook>	flipbooks;
	SDL_Rect	rect;
public:
	render_component(td::entity& entity, size_t width, size_t height) : __base(entity, "render")
	{
		rect.w = width;
		rect.h = height;
	}
	render_component(td::entity& entity, const std::string& id, size_t width, size_t height) : __base(entity, id)
	{
		rect.w = width;
		rect.h = height;
	}
	virtual ~render_component() = default;
	template	<typename ... types>
	void	add_flipbook(const std::string& name, types&& ... args)
	{
		flipbooks.emplace(name, sprite_flipbook(name, std::forward<types>(args)...));
	}
	void	remove_flipbook(const std::string& name)
	{
		flipbooks.erase(name);
	}
};


class	physics_component : public component
{
public:
	using __base = component;
public:
	physics_component(td::entity& entity, const std::string& id) : __base(entity, id) {  }
	virtual ~physics_component() = default;
};

class	transform_component : public physics_component
{
public:
	using __base = physics_component;
public:
	vector_3	velocity;
	vector_3	acceleration = vector_3(0.00f, 0.00f, 0.00f);
public:
	transform_component(td::entity& entity) : __base(entity, "transform") {  }
	transform_component(td::entity& entity, const std::string& id) : __base(entity, id) {  }
	virtual ~transform_component() = default;
};

class	collision_component : public physics_component
{
public:
	using __base = physics_component;
public:
	
public:
	collision_component(td::entity& entity) : __base(entity, "collision") {  }
	collision_component(td::entity& entity, const std::string& id) : __base(entity, id) {  }
	virtual ~collision_component() = default;
};

// TODO: finish
class	audio_component : public component
{
public:
	using __base = component;
public:
	audio_component(td::entity& entity) : __base(entity, "audio") {  }
	audio_component(td::entity& entity, const std::string& id) : __base(entity, id) {  }
	virtual ~audio_component() = default;
};

class	input_component : public component
{
public:
	using __base = component;
public:
	std::map<action, std::pair<keycode, modifier>>		key_mappings;
	std::map<action, std::pair<mousecode, modifier>>	mouse_mappings;
public:
	input_component(td::entity& entity) : __base(entity, "input") {  }
	input_component(td::entity& entity, const std::string& id) : __base(entity, id) {  }
	virtual ~input_component() = default;
	void	add_mapping(action, keycode, modifier);
	void	add_mapping(action, mousecode, modifier);
	void	remove_mapping(action);
};

__end_ns_td
