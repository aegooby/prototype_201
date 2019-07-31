
#pragma	once
#include	"__common.hpp"
#include	"ecs_common.hpp"
#include	<unordered_map>

__begin_ns_td

class	entity_manager
{
public:
	std::unordered_map<id_t, std::unique_ptr<entity>>	entities;
protected:
	size_t	__entityc = 0;
public:
	entity_manager() = default;
	~entity_manager() = default;
	entity&	new_entity(const std::string&, class world&);
	void	delete_entity(id_t);
};

__end_ns_td