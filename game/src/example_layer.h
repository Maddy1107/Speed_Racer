#pragma once
#include <engine.h>
#include "crossfade.h"
#include "player.h"
#include "pickup.h"
#include "smoke_particle_emitter.h"
#include "billboard.h"
#include "engine/entities/bounding_box.h"
#include "obstacles.h"
#include "grenade.h"
#include "obstacle_cow.h"
#include "enemy.h"
#include "engine/entities/shapes/road_block.h"


class example_layer : public engine::layer
{
public:
    example_layer();
	~example_layer();

    void on_update(const engine::timestep& time_step) override;
    void on_render() override; 
    void on_event(engine::event& event) override;

private:
	void check_bounce();
	int check_Obstacle();

	engine::ref<engine::skybox>			m_skybox_night{};
	engine::ref<engine::skybox>			m_skybox_day{};
	engine::ref<engine::game_object>	m_cow{};
	engine::ref<engine::game_object>	m_alien{};
	engine::ref<engine::game_object>	m_tree{};
	engine::ref<engine::game_object>	m_ball{};
	engine::ref<engine::game_object>	m_ball1{};
	engine::ref<engine::game_object>	m_mannequin{};
	engine::ref<engine::game_object>	m_terrain{};
	engine::ref<engine::game_object>	m_car{};
	engine::ref<engine::game_object>	m_obs{};
	engine::ref<engine::game_object>	m_planet{};
	engine::ref<engine::game_object>	m_closed_pyramid{};
	engine::ref<engine::game_object>	m_life_shape{};
	engine::ref<engine::game_object>	m_pyramid_door{};
	engine::ref<engine::game_object>	m_bound{};
	engine::ref<engine::game_object>	m_rb{};
	engine::ref<engine::game_object>	m_launcher{};
	engine::ref<engine::game_object>	m_roadBlock{};

	std::vector<engine::ref<engine::game_object>>     m_game_objects{};

	std::vector<grenade>     m_alien_grenades{};

	engine::DirectionalLight            m_directionalLight;

	engine::ref<engine::bullet_manager> m_physics_manager{};
	engine::ref<engine::audio_manager>  m_audio_manager{};
	float								m_prev_sphere_y_vel = 0.f;
	engine::ref<engine::text_manager>	m_text_manager{};

    engine::orthographic_camera       m_2d_camera; 
    engine::perspective_camera        m_3d_camera;

	engine::ref<crossfade>			m_cross_fade{};
	engine::ref<crossfade>			m_cross_fade_pause{};
	engine::ref<crossfade>			m_direction_crossfade{};
	engine::ref<crossfade>			m_camera_angle_crossfade{};
	engine::ref<crossfade>			m_life{};
	engine::ref<crossfade>			m_crash_crossfade{};
	engine::ref<crossfade>			m_gameOver{};
	engine::ref<crossfade>			m_shoot_crossfade{};

	engine::SpotLight headlightLeft;
	engine::SpotLight headlightRight;
	engine::SpotLight PoliceBluelight;
	engine::SpotLight PoliceRedlight;
	uint32_t num_spot_lights = 4;

	engine::ref<engine::material>		m_lightsource_material{};
	engine::ref<engine::material>		m_ballistic_material{};
	engine::ref<engine::material>		m_material{};

	engine::ref<pickup> m_pickup_launcher{};
	engine::ref<pickup> m_pickup_life{};

	engine::bounding_box				m_car_box;
	engine::bounding_box				m_obs_cuboid_box;
	engine::bounding_box				m_obs_pyramid_box;
	engine::bounding_box				m_obs_road_block_box;
	engine::bounding_box				m_obs_sphere_box;
	engine::bounding_box				m_obs_cow_box;
	engine::bounding_box				m_obs_alien_box;

	obstacle m_cuboid_obstacle;
	obstacle m_road_block_obstacle;
	obstacle m_pyramid_obstacle;

	smoke_particle_emitter m_smoke_trail;
	engine::ref<billboard>	 m_explosion{};
	grenade m_grenade;

	player m_player{};

	obstacle_cow m_obstacle_cow;

	enemy m_enemy;

	bool m_fog = false;
	glm::vec3 m_fog_colour;
	int32_t m_fog_factor_type;

	//Int/Float variables
	float crash_timer = 0.0;
	float pos = -10.f;
	float obs_xpos = 1.f;
	float py_xpos = 1.f;
	float obs_zpos = -15.f;
	float obs_speed = 1.5f;
	float m_pos = 0.0f;
	float m_distance = 0;
	int obsIndex;
	float zpospickup;
	float xpospickup;
	int pickupgenrange = 30;
	int AIspawnRange = 50;
	float pickup_index;
	float launcher_timer;
	float dayNight_timer = 0;
	int life = 3;
	float planet_rotation_amount = 1.0f;
	float shooting_cow_zpos;

	//Bollean variables
	bool pausegame = false;
	bool startgame = false;
	bool istimetopickup = false;
	bool genrandpospickup = false;
	bool generated_once = false;
	bool goright = true;
	bool carmoving = true;
	bool fpcam = false;
	bool day = true;
	bool act_pickup = true;
	bool crash_timer_start = false;
	bool crashed = false;
	bool game_over = false;
	bool cutofftimer = 0;
	bool blueRed;
	bool showBoundingBox = false;
	bool render_cow_box = false;
	bool generatedonceAI = false;
	bool istimeforAI = false;
};
