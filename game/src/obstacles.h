#pragma once
#include <engine.h>
#include "glm/gtx/rotate_vector.hpp"
class obstacle
{

public:
	obstacle();
	~obstacle();
	void initialise(engine::ref<engine::game_object> object);
	engine::ref<engine::game_object> object() const { return m_object; }
	void on_render(const engine::ref<engine::shader>& shader, float xpos, float zpos, int index);

private:
	engine::ref< engine::game_object> m_object;
};
