#pragma once
#include <engine.h>
#include "glm/gtx/rotate_vector.hpp"
class player
{

public:
	player();
	~player();
	void initialise(engine::ref<engine::game_object> object);
	void on_update(const engine::timestep& time_step);
	engine::ref<engine::game_object> object() const { return m_object; }
	void move_player(const engine::timestep& time_step, const glm::vec3& pos);
	void update_camera(engine::perspective_camera& camera, const engine::timestep& time_step,float a, float b,float c, bool obj);
	void jump();
	int get_speed() { return m_speed; };
	int set_speed(int speed) { m_speed = speed; }
	bool collision_detection_ground(float y_plane);
	
private:
	float m_speed{ 0.f };
	float m_timer;
	engine::ref< engine::game_object> m_object;

	glm::vec3 cam_pos;

	glm::vec3   m_right{ 1.f,0.f,0.f };
};
