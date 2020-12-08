#pragma once
#include <engine.h>

class quad;

class crossfade
{
public:
	crossfade(const std::string& path, float max_time, float width, float height);
	~crossfade();

	void on_update(const engine::timestep& time_step);
	void on_render(engine::ref<engine::shader> shader, int cf_index);
	void activate();
	void set_life(int life) {
		life_value = life;
	};

	static engine::ref<crossfade> create(const std::string& path, float max_time, float width, float height);

private:
	bool s_active;
	float xpos_gap = 0.0;
	int life_value;

	float m_timer;
	float m_max_time;

	engine::ref<engine::texture_2d> m_texture;
	float m_transparency;
	engine::ref<quad> m_quad;
};
