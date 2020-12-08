#pragma once
#include <engine.h>

class obstacle_cow
{
	enum class state
	{
		idle,
	};
public:
	obstacle_cow();
	~obstacle_cow();
	// set the parameters for the obstacle_cow
	void initialise(engine::ref<engine::game_object> object, glm::vec3 position, glm::vec3 forward);
	// update the obstacle_cow
	void on_update(const engine::timestep& time_step, const glm::vec3& player_position);

	// methods controlling the obstacle_cow’s behaviour in a certain state
	void patrol(const engine::timestep& time_step);

	engine::ref<engine::game_object> object() const { return m_object; }
private:
	// obstacle_cow's speed
	float m_speed{ 2.f };
	// timer controlling the direction switch and the reset value for this timer
	float m_default_time{ 4.f };
	float m_switch_direction_timer = m_default_time;
	// threshold distances
	float m_detection_radius{ 6.f };
	float m_trigger_radius{ 3.f };
	// game object bound to the obstacle_cow
	engine::ref< engine::game_object> m_object;
	//current state of the obstacle_cow's state machine
	state m_state = state::idle;
};

