
#pragma	once
#include	"__common.hpp"
#include	"window.hpp"
#include	"clock.hpp"
#include	"renderer.hpp"
#include	"entity.hpp"
#include	"newton.hpp"
#include	"event.hpp"
#include	<thread>
#include	<chrono>
#include	<list>

__begin_ns_td

class	engine
{
protected:
	td::window			window;
	td::renderer		renderer;
	td::keyboard&		keyboard;
	td::mouse&			mouse;
	td::clock			clock;
	td::newton			newton;
	td::event_handler	event_handler;
	td::render_manager	render_manager;
	std::list<std::unique_ptr<entity>>	entities;
	bool		__running = false, __fpsdebug = false;
	const float	__time_per_frame = 1.0f / float(global::game_fps);
	
	bool	cmd_w() const
	{
#if		defined(TD_OS_MACOS)
		return (keyboard.down(keycode::W) && keyboard.modifier(modifier::GUI));
#elif	defined(TD_OS_WINDOWS)
		return (keyboard.down(keycode::W) && keyboard.modifier(modifier::CTRL));
#else
		return false;
#endif
	}
public:
	engine(const std::string& title, int width, int height, bool fpsdebug) : window(title, width, height), renderer(window), keyboard(window.keyboard), mouse(window.mouse), __fpsdebug(fpsdebug)
	{
		window.start();
		renderer.start();
		add_entity<player>("player");
		render_manager.add_component(*entities.back().get());
		dynamic_cast<player*>(entities.back().get())->render = render_manager.components.back();
		renderer.load(entities, "/Users/admin/Desktop/sprites");
	}
	~engine() = default;
	void	start()
	{
		if (__running) return;
		__running = true;
		
		float	time_last_it = clock.time_s();
		float	time_unprocessed = 0;
		
		int32_t		__frame_count = 0;
		float		__frame_time = 0;
		const float	__frame_rate_check = 1.0f;
		
		while (__running)
		{
			bool	render_ready = false;
			
			//	Set time for "this iteration"
			float	time_next_it = clock.time_s();
			float	time_taken_frame = time_next_it - time_last_it;
			
			time_last_it = time_next_it;
			
			//	To count the time that has passed from one iteration to the next
			time_unprocessed += time_taken_frame;
			__frame_time += time_taken_frame;
			
			// Check frame rate
			if (__frame_time >= __frame_rate_check)
			{
				if (__fpsdebug)
				{
					std::cout << "fps: ";
					std::cout << float(__frame_count) / __frame_rate_check;
					std::cout << std::endl;
				}
				__frame_time = 0;
				__frame_count = 0;
			}
			
			// Process anything that has happened in the last one or more frames
			// This avoids input lag and gives the engine a chance to catch up
			// on input/logic before trying to render
			// (Prefer visual lag over logic lag)
			
			// This all made sense to me at some point
			
			while (time_unprocessed >= __time_per_frame)
			{
				//	All non-graphical processing
				if (window.closed())
					__running = false;
				
				update();
				
				//	One frame has been rendered, so subtract
				time_unprocessed -= __time_per_frame;
				
				//	Once this loop is done, we will be ready to render everything
				render_ready = true;
			}
			//	Render a frame
			if (render_ready)
			{
				render();
				if (cmd_w())
				{
					stop();
				}
				
				//	Every time a frame is rendered successfully
				__frame_count++;
			}
			else
				std::this_thread::sleep_for(std::chrono::milliseconds(1));
		}
	}
	void	stop()
	{
		__running = false;
	}
	void	render()
	{
		renderer.render(entities);
	}
	void	update()
	{
		// Input
		window.update();
//		dynamic_cast<td::player*>(entities.back().get())->input->read(keyboard, mouse);
		
//		for (auto& entity : entities)
//		{
//			entity->update();
//		}
	}
	template	<typename type, typename ... types>
	void	add_entity(types&& ... args)
	{
		entities.emplace_back(std::make_unique<type>(event_handler, std::forward<types>(args)...));
	}
	void	remove_entity(const std::unique_ptr<entity>& entity)
	{
		entities.remove_if([&entity](const std::unique_ptr<td::entity>& __entity)
						   {
							   return &__entity == &entity;
						   });
	}
	
	//	Preventing copying and moving
	engine(const engine&) = delete;
	engine(engine&&) = delete;
	engine&	operator =(const engine&) = delete;
	engine&	operator =(engine&&) = delete;
};

__end_ns_td
