#include "pch.h"
#include "crossfade.h"
#include "quad.h"
#include "example_layer.h"

crossfade::crossfade(const std::string& path, float max_time, float width, float height)
{
	m_max_time = max_time;
	m_texture = engine::texture_2d::create(path, true);

	m_transparency = 0.0f;

	m_quad = quad::create(glm::vec2(width, height));
	s_active = false;
}

crossfade::~crossfade()
{}

void crossfade::on_update(const engine::timestep& time_step)
{
	if (!s_active)
		return;

	m_timer += (float)time_step;

	m_transparency = 0.8f - 0.8f * m_timer / m_max_time;

	if (m_timer > m_max_time)
		s_active = false;
}

void crossfade::on_render(engine::ref<engine::shader> shader, int cf_index)
{
	if (!s_active)
		return;
	glm::mat4 transform(1.0f);
	if (cf_index == 1)
	{
		transform = glm::translate(transform, glm::vec3(0.0f, 0.0f, 0.01f));
	}
	else if (cf_index == 2)
	{
		
		transform = glm::translate(transform, glm::vec3(-1.25f, -0.8f, 0.01f));
	}
	else if (cf_index == 3)
	{
		transform = glm::translate(transform, glm::vec3(-0.45f, -0.8f, 0.01f));
	}
	else if (cf_index == 4)
	{
		transform = glm::translate(transform, glm::vec3(-1.30f + xpos_gap, 0.68f, 0.01f));
		if (xpos_gap <= 0.1 * (life_value - 1) && life_value > 1)
			xpos_gap += 0.1;
		else
			xpos_gap = 0;
	}
	else if (cf_index == 5)
	{
		transform = glm::translate(transform, glm::vec3(0.0f, 0.6f, 0.01f));
	}
	else if (cf_index == 6)
	{
		transform = glm::translate(transform, glm::vec3(0.45f, -0.8f, 0.01f));
	}

	std::dynamic_pointer_cast<engine::gl_shader>(shader)->set_uniform("transparency", m_transparency);
	m_texture->bind();
	engine::renderer::submit(shader, m_quad->mesh(), transform);
	std::dynamic_pointer_cast<engine::gl_shader>(shader)->set_uniform("transparency", 1.0f);
}

void crossfade::activate()
{
	s_active = true;
	m_transparency = 1.0f;
	m_timer = 0.0f;
}

engine::ref<crossfade> crossfade::create(const std::string& path, float max_time, float width, float height)
{
	
	return std::make_shared<crossfade>(path, max_time, width, height);
}

void set_dynamic_pos(int life)
{

}
