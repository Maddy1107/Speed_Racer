#include "pch.h"
#include "player.h"

player::player() : m_timer(0.0f), m_speed(10.0f)
{
	;
}

player::~player()
{
}

void player::initialise(engine::ref<engine::game_object> object)
{
	m_object = object;
	m_object->set_forward(glm::vec3(0.f, 0.f, -1.f));
	m_object->set_position(m_object->position());
	m_object->set_rotation_amount(atan2(m_object->forward().x, m_object->forward().z));
	m_object->set_acceleration(glm::vec3(0.f, -9.8f, 0.f));
}

void player::move_player(const engine::timestep& time_step, const glm::vec3& pos)
{
	m_object->set_position(m_object->position() -= m_speed * time_step * pos);
}

void player::on_update(const engine::timestep& time_step)
{
	m_object->set_velocity(m_object->forward() * m_speed);
	//m_object->set_position(m_object->position() += m_object->forward() * m_speed *
		//(float)time_step);

	if (engine::input::key_pressed(engine::key_codes::KEY_LEFT))// left
	{
		if (m_object->position().x >= -2.3f)
		{
			move_player(time_step, m_right);
		}
	}
	else if (engine::input::key_pressed(engine::key_codes::KEY_RIGHT)) // right
	{
		if (m_object->position().x <= 2.3f)
		{
			move_player(time_step, -m_right);
		}
	}
}
void player::update_camera(engine::perspective_camera& camera, const engine::timestep& time_step, float a, float b, float c, bool obj)
{
	if (obj)
	{
		cam_pos = m_object->position() - glm::vec3(0.13f, 0, 0) - glm::normalize(m_object->forward()) * b;
		
	}
	else
	{
		cam_pos = m_object->position() - glm::normalize(m_object->forward()) * b;

	}

	cam_pos.y += a;

	glm::vec3 cam_look_at = m_object->position() + glm::normalize(m_object->forward()) * c;
	cam_look_at.y = 0.f;
	camera.set_view_matrix(cam_pos, cam_look_at);
}

bool player::collision_detection_ground(float y_plane)
{
	// Check for collision with the ground by looking at the y value of the ball's position
	if (m_object->position().y < y_plane) {
		return true;
	}
	return false;
}



