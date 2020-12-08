#include "pch.h"
#include "obstacles.h"

obstacle::obstacle()
{
	;
}

obstacle::~obstacle()
{
}

void obstacle::initialise(engine::ref<engine::game_object> object)
{
	m_object = object;
	m_object->set_position(m_object->position());
}

void obstacle::on_render(const engine::ref<engine::shader>& shader, float xpos, float zpos, int index)
{
	if (index == 1)
	{
		m_object->set_position(glm::vec3(xpos, 0.6f, zpos));
	}
	else if (index == 2)
	{
		m_object->set_position(glm::vec3(xpos, 0.0f, zpos));
		engine::renderer::submit(shader, m_object);
	}

}



