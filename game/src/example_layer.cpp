#include "example_layer.h"
#include "platform/opengl/gl_shader.h"

#include <glm/gtc/matrix_transform.hpp> // glm::translate, glm::rotate, glm::scale, glm::perspective
#include <glm/gtc/type_ptr.hpp>
#include "engine/events/key_event.h"
#include "engine/utils/track.h"
#include <engine\entities\shapes\pyramid.h>
#include <engine\entities\shapes\terrain_quad.h>
#include <engine\entities\shapes\life_shape.h>
#include "pickup.h"

example_layer::example_layer() 
    :m_2d_camera(-1.6f, 1.6f, -0.9f, 0.9f), 
    m_3d_camera((float)engine::application::window().width(), (float)engine::application::window().height())
{

	m_fog = true;
	glm::vec3 m_fog_colour = glm::vec3(0.5f);
	int m_fog_factor_type = 0.2;

    // Hide the mouse and lock it inside the window
    //engine::input::anchor_mouse(true);
    engine::application::window().hide_mouse_cursor();

	// Initialise audio and play background music
	m_audio_manager = engine::audio_manager::instance();
	m_audio_manager->init();
	m_audio_manager->load_sound("assets/audio/background.wav", engine::sound_type::event, "bg"); // Royalty free sound from freesound.org
	m_audio_manager->load_sound("assets/audio/collision.flac", engine::sound_type::event, "collision"); // Royalty free sound from freesound.org
	m_audio_manager->load_sound("assets/audio/car.wav", engine::sound_type::event, "car"); // Royalty free sound from freesound.org
	m_audio_manager->load_sound("assets/audio/explosion.wav", engine::sound_type::event, "explosion"); // Royalty free sound from freesound.org
	m_audio_manager->load_sound("assets/audio/moo.wav", engine::sound_type::event, "moo"); // Royalty free sound from freesound.org
	m_audio_manager->play("bg");
	m_audio_manager->loop("bg", true);

	// Initialise the shaders, materials and lights
	auto mesh__material_shader = engine::renderer::shaders_library()->get("mesh_material");
	auto mesh_lighting_shader = engine::renderer::shaders_library()->get("mesh_lighting");
	auto text_shader = engine::renderer::shaders_library()->get("text_2D");
	auto animated_mesh_shader = engine::renderer::shaders_library()->get("animated_mesh");

	m_directionalLight.Color = glm::vec3(1.f, 1.f, 1.f);
	m_directionalLight.AmbientIntensity = 0.75f;
	m_directionalLight.DiffuseIntensity = 0.9f;
	m_directionalLight.Direction = glm::normalize(glm::vec3(0.0f, -1.0f, 0.0f));

	//Left Headlight for the car
	headlightLeft.Color = glm::vec3(1.0f, 1.0f, 1.0f);
	headlightLeft.AmbientIntensity = 0.25f;
	headlightLeft.DiffuseIntensity = 0.6f;
	headlightLeft.Position = glm::vec3(0.0f, 2.3f, -15.0f);
	headlightLeft.Direction = glm::normalize(glm::vec3(0.0f, 0.0f, -1.0f));
	headlightLeft.Cutoff = 0.9f;

	//Right Headlight for the car
	headlightRight.Color = glm::vec3(1.0f, 1.0f, 1.0f);
	headlightRight.AmbientIntensity = 0.25f;
	headlightRight.DiffuseIntensity = 0.6f;
	headlightRight.Position = glm::vec3(0.0f, 2.3f, -15.0f);
	headlightRight.Direction = glm::normalize(glm::vec3(0.0f, 0.0f, -1.0f));
	headlightRight.Cutoff = 0.9f;

	//Police Blue Lights
	PoliceBluelight.Color = glm::vec3(0.0f, 0.0f, 1.0f);
	PoliceBluelight.AmbientIntensity = 5.95f;
	PoliceBluelight.DiffuseIntensity = 5.9f;
	PoliceBluelight.Position = glm::vec3(0.0f, 2.3f, -15.0f);
	PoliceBluelight.Direction = glm::normalize(glm::vec3(0.0f, -1.0f, 0.0f));
	PoliceBluelight.Cutoff = 0.9f;

	//Police Red Lights
	PoliceRedlight.Color = glm::vec3(1.0f, 0.0f, 0.0f);
	PoliceRedlight.AmbientIntensity = 5.95f;
	PoliceRedlight.DiffuseIntensity = 5.9f;
	PoliceRedlight.Position = glm::vec3(0.0f, 2.3f, -15.0f);
	PoliceRedlight.Direction = glm::normalize(glm::vec3(0.0f, -1.0f, 0.0f));
	PoliceRedlight.Cutoff = 0.9f;

	// Set color texture unit
	std::dynamic_pointer_cast<engine::gl_shader>(animated_mesh_shader)->bind();
	std::dynamic_pointer_cast<engine::gl_shader>(animated_mesh_shader)->set_uniform("gColorMap", 0);
	std::dynamic_pointer_cast<engine::gl_shader>(animated_mesh_shader)->set_uniform("lighting_on", true);
	m_directionalLight.submit(animated_mesh_shader);
	std::dynamic_pointer_cast<engine::gl_shader>(animated_mesh_shader)->set_uniform("gMatSpecularIntensity", 0.5f);
	std::dynamic_pointer_cast<engine::gl_shader>(animated_mesh_shader)->set_uniform("gSpecularPower", 5.f);
	std::dynamic_pointer_cast<engine::gl_shader>(animated_mesh_shader)->set_uniform("transparency", 1.0f);
	std::dynamic_pointer_cast<engine::gl_shader>(animated_mesh_shader)->set_uniform("fog_colour", m_fog_colour);
	std::dynamic_pointer_cast<engine::gl_shader>(animated_mesh_shader)->set_uniform("fog_factor_type", m_fog_factor_type);
	std::dynamic_pointer_cast<engine::gl_shader>(mesh__material_shader)->bind();
	std::dynamic_pointer_cast<engine::gl_shader>(mesh__material_shader)->set_uniform("lighting_on", true);
	m_directionalLight.submit(mesh__material_shader);
	std::dynamic_pointer_cast<engine::gl_shader>(mesh__material_shader)->set_uniform("gMatSpecularIntensity", 1.f);
	std::dynamic_pointer_cast<engine::gl_shader>(mesh__material_shader)->set_uniform("gSpecularPower", 10.f);
	std::dynamic_pointer_cast<engine::gl_shader>(mesh__material_shader)->set_uniform("fog_colour", m_fog_colour);
	std::dynamic_pointer_cast<engine::gl_shader>(mesh__material_shader)->set_uniform("fog_factor_type", m_fog_factor_type);
	std::dynamic_pointer_cast<engine::gl_shader>(mesh_lighting_shader)->bind();
	std::dynamic_pointer_cast<engine::gl_shader>(mesh_lighting_shader)->set_uniform("gColorMap", 0);
	std::dynamic_pointer_cast<engine::gl_shader>(mesh_lighting_shader)->set_uniform("lighting_on", true);
	m_directionalLight.submit(mesh_lighting_shader);
	std::dynamic_pointer_cast<engine::gl_shader>(mesh_lighting_shader)->set_uniform("gMatSpecularIntensity", 0.5f);
	std::dynamic_pointer_cast<engine::gl_shader>(mesh_lighting_shader)->set_uniform("gSpecularPower", 5.f);
	std::dynamic_pointer_cast<engine::gl_shader>(mesh_lighting_shader)->set_uniform("transparency", 1.0f);
	std::dynamic_pointer_cast<engine::gl_shader>(mesh_lighting_shader)->set_uniform("fog_colour", m_fog_colour);
	std::dynamic_pointer_cast<engine::gl_shader>(mesh_lighting_shader)->set_uniform("fog_factor_type", m_fog_factor_type);
	std::dynamic_pointer_cast<engine::gl_shader>(text_shader)->bind();
	std::dynamic_pointer_cast<engine::gl_shader>(text_shader)->set_uniform("projection",
		glm::ortho(0.f, (float)engine::application::window().width(), 0.f,
		(float)engine::application::window().height()));
	m_material = engine::material::create(1.0f, glm::vec3(1.0f, 0.1f, 0.07f),
		glm::vec3(1.0f, 0.1f, 0.07f), glm::vec3(0.5f, 0.5f, 0.5f), 1.0f);

	// Skybox texture for night
	m_skybox_night = engine::skybox::create(50.f,
		{ engine::texture_2d::create("assets/textures/skybox/Night/front.png", true),
		  engine::texture_2d::create("assets/textures/skybox/Night/right.png", true),
		  engine::texture_2d::create("assets/textures/skybox/Night/back.png", true),
		  engine::texture_2d::create("assets/textures/skybox/Night/left.png", true),
		  engine::texture_2d::create("assets/textures/skybox/Night/top.png", true),
		  engine::texture_2d::create("assets/textures/skybox/Night/bottom.png", true)
		});

	// Skybox texture for day
	m_skybox_day = engine::skybox::create(50.f,
		{ engine::texture_2d::create("assets/textures/skybox/Day/front.bmp", true),
		  engine::texture_2d::create("assets/textures/skybox/Day/right.bmp", true),
		  engine::texture_2d::create("assets/textures/skybox/Day/back.bmp", true),
		  engine::texture_2d::create("assets/textures/skybox/Day/left.bmp", true),
		  engine::texture_2d::create("assets/textures/skybox/Day/top.bmp", true),
		  engine::texture_2d::create("assets/textures/skybox/Day/bottom.bmp", true)
		});


	engine::ref<engine::skinned_mesh> m_skinned_mesh = engine::skinned_mesh::create("assets/models/animated/mannequin/free3Dmodel.dae");
	m_skinned_mesh->LoadAnimationFile("assets/models/animated/mannequin/walking.dae");
	m_skinned_mesh->LoadAnimationFile("assets/models/animated/mannequin/idle.dae");
	m_skinned_mesh->LoadAnimationFile("assets/models/animated/mannequin/jump.dae");
	m_skinned_mesh->LoadAnimationFile("assets/models/animated/mannequin/standard_run.dae");
	m_skinned_mesh->switch_root_movement(false);

	engine::game_object_properties mannequin_props;
	mannequin_props.animated_mesh = m_skinned_mesh;
	mannequin_props.scale = glm::vec3(1.f/ glm::max(m_skinned_mesh->size().x, glm::max(m_skinned_mesh->size().y, m_skinned_mesh->size().z)));
	mannequin_props.position = glm::vec3(0.0f, 0.5f, -15.0f);
	mannequin_props.type = 0;
	mannequin_props.bounding_shape = m_skinned_mesh->size() / 2.f * mannequin_props.scale.x;
	m_mannequin = engine::game_object::create(mannequin_props);

	// Loading the terrain
	std::vector<engine::ref<engine::texture_2d>> terrain_textures = { engine::texture_2d::create("assets/textures/terr.jpg", false) };
	engine::ref<engine::terrain_quad> terrain_shape = engine::terrain_quad::create(glm::vec3(4.f, 0.f, 500.f));
	engine::game_object_properties terrain_props;
	terrain_props.meshes = { terrain_shape->mesh() };
	terrain_props.textures = terrain_textures;
	terrain_props.is_static = true;
	terrain_props.type = 0;
	terrain_props.bounding_shape = glm::vec3(4.f, 0.f, 500.f);
	terrain_props.restitution = 0.0f;
	m_terrain = engine::game_object::create(terrain_props);

	//Loading the terrain boundaries
	engine::ref<engine::terrain_quad> terr_bou_shape = engine::terrain_quad::create(glm::vec3(2.f, 0.f, 10.f));
	std::vector<engine::ref<engine::texture_2d>> terr_bou_texture = { engine::texture_2d::create("assets/textures/Road.png", false)};
	engine::game_object_properties terr_bou_props;
	terr_bou_props.meshes = { terr_bou_shape->mesh() };
	terr_bou_props.textures = { terr_bou_texture };
	m_bound = engine::game_object::create(terr_bou_props);

	
	// Load the cow model. Create a cow object. Set its properties
	engine::ref <engine::model> cow_model = engine::model::create("assets/models/static/cow4.3ds");
	engine::game_object_properties cow_props;
	cow_props.meshes = cow_model->meshes();
	cow_props.textures = cow_model->textures();
	float cow_scale = 1.f / glm::max(cow_model->size().x, glm::max(cow_model->size().y, cow_model->size().z));
	cow_props.position = { 2.f, 0.f, -25.f };
	cow_props.scale = glm::vec3(cow_scale);
	cow_props.bounding_shape = cow_model->size() / 2.f * cow_scale;
	m_cow = engine::game_object::create(cow_props);
	m_obs_cow_box.set_box(cow_props.bounding_shape.x * 2.f,
		cow_props.bounding_shape.y * 2.f,
		cow_props.bounding_shape.z * 2.f,
		cow_props.position);
	m_obstacle_cow.initialise(m_cow, cow_props.position, glm::vec3(1.f, 0.f, 0.f));

	// Load the alien model. Create a cow object. Set its properties
	engine::ref <engine::model> alien_model = engine::model::create("assets/models/static/alien.obj");
	std::vector<engine::ref<engine::texture_2d>> alien_textures = { engine::texture_2d::create("assets/textures/alien.jpg", false) };
	engine::game_object_properties alien_props;
	alien_props.meshes = alien_model->meshes();
	alien_props.textures = { alien_textures };
	float alien_scale = 1.f / glm::max(alien_model->size().x, glm::max(alien_model->size().y, alien_model->size().z));
	alien_props.position = { 2.f, 0.f, -25.f };
	alien_props.scale = glm::vec3(alien_scale);
	alien_props.bounding_shape = alien_model->size() / 2.f * alien_scale;
	m_alien = engine::game_object::create(alien_props);
	m_obs_alien_box.set_box(cow_props.bounding_shape.x * 2.f,
		alien_props.bounding_shape.y * 2.f,
		alien_props.bounding_shape.z * 2.f,
		alien_props.position);
	m_enemy.initialise(m_alien, alien_props.position, glm::vec3(1.f, 0.f, 0.f));

	// Load the tree model. Create a tree object. Set its properties
	engine::ref <engine::model> tree_model = engine::model::create("assets/models/static/tree2.3ds");
	engine::game_object_properties tree_props;
	tree_props.meshes = tree_model->meshes();
	tree_props.textures = tree_model->textures();
	float tree_scale = 1.f / glm::max(tree_model->size().x, glm::max(tree_model->size().y, tree_model->size().z));
	tree_props.position = { 4.f, 0.5f, -5.f };
	tree_props.bounding_shape = tree_model->size() / 2.f * tree_scale;
	tree_props.scale = glm::vec3(tree_scale);
	m_tree = engine::game_object::create(tree_props);

	//Load a Car model and create an object
	engine::ref <engine::model> car_model = engine::model::create("assets/models/static/car1.obj");
	engine::ref<engine::texture_2d> car_texture = engine::texture_2d::create("assets/textures/car.png", true);
	engine::game_object_properties car_props;
	car_props.meshes = car_model->meshes();
	car_props.textures = { car_texture };
	float car_scale = 1.f / glm::max(car_model->size().x, glm::max(car_model->size().y, car_model->size().z));
	car_props.position = { 0.0f, 0.1f, -10.0f };
	car_props.bounding_shape = car_model->size() / 2.f * car_scale;
	car_props.scale = glm::vec3(car_scale);
	m_car = engine::game_object::create(car_props);
	m_player.initialise(m_car);
	m_car_box.set_box(car_props.bounding_shape.x * 2.f,
		car_props.bounding_shape.y *  4.f,
		car_props.bounding_shape.z * 7.f,
		car_props.position);

	//Load a Launcher and create an object
	engine::ref <engine::model> launcher_model = engine::model::create("assets/models/static/launcher.obj");
	engine::ref<engine::texture_2d> launcher_texture = engine::texture_2d::create("assets/textures/launcher.png", true);
	engine::game_object_properties launcher_props;
	launcher_props.meshes = launcher_model->meshes();
	launcher_props.textures = { launcher_texture };
	float launcher_scale = 1.f / glm::max(launcher_model->size().x, glm::max(launcher_model->size().y, launcher_model->size().z));
	launcher_props.position = { 0.0f, 1.0f, -15.0f };
	launcher_props.bounding_shape = launcher_model->size() / 2.f * launcher_scale;
	launcher_props.scale = glm::vec3(launcher_scale);
	m_launcher = engine::game_object::create(launcher_props);
	m_pickup_launcher = pickup::create(launcher_props);
	m_pickup_launcher->init();

	//Load a road block and create an object
	engine::ref <engine::model> rb_model = engine::model::create("assets/models/static/road_block.obj");
	engine::ref<engine::texture_2d> rb_texture = engine::texture_2d::create("assets/textures/road_block.jpg", true);
	engine::game_object_properties rb_props;
	rb_props.meshes = rb_model->meshes();
	rb_props.textures = { rb_texture };
	float rb_scale = 1.f / glm::max(rb_model->size().x, glm::max(rb_model->size().y, rb_model->size().z));
	rb_props.position = { 0.0f, 0.1f, -10.0f };
	rb_props.bounding_shape = rb_model->size() / 2.f * rb_scale;
	rb_props.scale = glm::vec3(rb_scale);
	m_rb = engine::game_object::create(rb_props);

	//Cuboid for Obstacles
	engine::ref<engine::cuboid> obs_shape = engine::cuboid::create(glm::vec3(1.0f,0.5f,0.5f), false);
	engine::ref<engine::texture_2d> obs_texture = engine::texture_2d::create("assets/textures/cuboid.jpg", true);
	engine::game_object_properties obs_props;
	obs_props.position = { 0.f, 0.5f, 15.f };
	obs_props.meshes = { obs_shape->mesh() };
	obs_props.textures = { obs_texture };
	obs_props.bounding_shape = glm::vec3(0.5f);
	m_obs = engine::game_object::create(obs_props);
	m_cuboid_obstacle.initialise(m_obs);
	m_obs_cuboid_box.set_box(obs_props.bounding_shape.x * 12.f,
		obs_props.bounding_shape.y * 5.f,
		obs_props.bounding_shape.z * 4.f,
		obs_props.position);

	//RoadBlocks for Obstacles
	engine::ref<engine::road_block> road_block_shape = engine::road_block::create(glm::vec3(0.7f, 0.7f, 0.7f), false);
	engine::ref<engine::texture_2d> road_block_texture = engine::texture_2d::create("assets/textures/Tile.jpg", true);
	engine::game_object_properties road_block_props;
	road_block_props.position = { 1.2f, 0.5f, 15.f };
	road_block_props.meshes = { road_block_shape->mesh() };
	road_block_props.textures = { road_block_texture };
	road_block_props.bounding_shape = glm::vec3(0.5f);
	m_roadBlock = engine::game_object::create(road_block_props);
	m_road_block_obstacle.initialise(m_roadBlock);
	m_obs_road_block_box.set_box(road_block_props.bounding_shape.x * 4.f,
		road_block_props.bounding_shape.y * 2.f,
		road_block_props.bounding_shape.z * 2.f,
		road_block_props.position);

	//Closed Pyramids for obstacles
	std::vector<glm::vec3> pyramid_vertices;
	pyramid_vertices.push_back(glm::vec3(0.f, 0.4f, 0.f));	//0
	pyramid_vertices.push_back(glm::vec3(-0.4f, 0.f, -0.4f));	//1
	pyramid_vertices.push_back(glm::vec3(-0.4f, 0.f, 0.4f));	//2
	pyramid_vertices.push_back(glm::vec3(0.4f, 0.f, 0.4f));//3
	pyramid_vertices.push_back(glm::vec3(0.4f, 0.f, -0.4f)); //4

	engine::ref<engine::pyramid> pyramid_shape = engine::pyramid::create(pyramid_vertices);
	engine::game_object_properties pyramid_props;
	pyramid_props.position = { 0.f, 0.f, 25.f };
	pyramid_props.meshes = { pyramid_shape->mesh() };
	std::vector<engine::ref<engine::texture_2d>>
		pyramid_textures = { engine::texture_2d::create("assets/textures/pyramid.jpg", false) };
	pyramid_props.textures = pyramid_textures;
	pyramid_props.bounding_shape = glm::vec3(0.5f);
	m_closed_pyramid = engine::game_object::create(pyramid_props);
	m_pyramid_obstacle.initialise(m_closed_pyramid);
	m_obs_pyramid_box.set_box(pyramid_props.bounding_shape.x * 8.f,
		pyramid_props.bounding_shape.y * 2.f,
		pyramid_props.bounding_shape.z * 2.f,
		pyramid_props.position);

	//Octahedron for life
	std::vector<glm::vec3> life_shape_vertices;
	life_shape_vertices.push_back(glm::vec3(0.f, 0.4f, 0.f));	//0
	life_shape_vertices.push_back(glm::vec3(-0.2f, 0.f, -0.1f));//1
	life_shape_vertices.push_back(glm::vec3(-0.2f, 0.f, 0.2f));	//2
	life_shape_vertices.push_back(glm::vec3(0.2f, 0.f, 0.2f));//3
	life_shape_vertices.push_back(glm::vec3(0.2f, 0.f, -0.2f)); //4
	life_shape_vertices.push_back(glm::vec3(0.0f, -0.4f, 0.f)); //5

	engine::ref<engine::life_shape> life_shape_pri = engine::life_shape::create(life_shape_vertices);
	engine::game_object_properties life_shape_props;
	life_shape_props.position = { -1.5f, 3.7f, 1.f };
	life_shape_props.meshes = { life_shape_pri->mesh() };
	std::vector<engine::ref<engine::texture_2d>> life_shape_textures = { engine::texture_2d::create("assets/textures/life_tex.jpg", false) };
	life_shape_props.textures = life_shape_textures;
	m_life_shape = engine::game_object::create(life_shape_props);
	m_pickup_life = pickup::create(life_shape_props);
	m_pickup_life->init();

	//Crossfade for intro
	m_cross_fade = crossfade::create("assets/textures/logo.jpg", 1.0f, 1.6f, 0.9f);

	//Crossfade for HUD
	m_direction_crossfade = crossfade::create("assets/textures/direction.png", 5.0f, 0.2f, 0.1f);
	m_camera_angle_crossfade = crossfade::create("assets/textures/C.png", 5.0f, 0.1f, 0.1f);
	m_life = crossfade::create("assets/textures/life.png", 5.f, 0.05f, 0.05f);
	m_shoot_crossfade = crossfade::create("assets/textures/F.png", 5.f, 0.1f, 0.1f);

	//Crossfade for pause
	m_cross_fade_pause = crossfade::create("assets/textures/pause.jpg", 2.0f, 1.6f, 0.9f);

	//Crossfade for crash
	m_crash_crossfade = crossfade::create("assets/textures/crash.png", 2.0f, 1.6f, 0.9f);

	//Crossfade for GameOver
	m_gameOver = crossfade::create("assets/textures/gameover.png", 2.0f, 0.5f, 0.5f);

	//Load a Planet and create an object
	engine::ref <engine::model> planet_model = engine::model::create("assets/models/static/mars.obj");
	engine::ref<engine::texture_2d> planet_texture = engine::texture_2d::create("assets/textures/mars.png", true);
	engine::game_object_properties planet_props;
	planet_props.meshes = planet_model->meshes();
	planet_props.textures = { planet_texture };
	float planet_scale = 5.f / glm::max(planet_model->size().x, glm::max(planet_model->size().y, planet_model->size().z));
	planet_props.position = { 4.f, 5.5f, -25.f };
	planet_props.bounding_shape = planet_model->size() / 2.f * planet_scale;
	planet_props.scale = glm::vec3(planet_scale);
	m_planet = engine::game_object::create(planet_props);

	engine::ref<engine::sphere> sphere_shape1 = engine::sphere::create(20, 40, 0.3f);
	engine::game_object_properties sphere_props1;
	sphere_props1.position = { -2.5f, 5.5f, -2.5f };
	sphere_props1.meshes = { sphere_shape1->mesh() };
	sphere_props1.type = 1;
	sphere_props1.bounding_shape = glm::vec3(0.5f);
	sphere_props1.restitution = 0.92f;
	sphere_props1.mass = 0.45f;
	m_ball1 = engine::game_object::create(sphere_props1);
	m_obs_sphere_box.set_box(sphere_props1.bounding_shape.x * 1.5f,
		sphere_props1.bounding_shape.y * 1.f,
		sphere_props1.bounding_shape.z * 2.f,
		sphere_props1.position);

	//Load the sphere for grenades
	engine::ref<engine::sphere> sphere_shape = engine::sphere::create(20, 40, 0.3f);
	engine::game_object_properties sphere_props;
	sphere_props.position = { -2.5f, 5.5f, -2.5f };
	sphere_props.meshes = { sphere_shape->mesh() };
	sphere_props.type = 1;
	sphere_props.bounding_shape = glm::vec3(0.5f);
	sphere_props.restitution = 0.92f;
	sphere_props.mass = 0.45f;
	m_ball = engine::game_object::create(sphere_props);
	m_obs_sphere_box.set_box(sphere_props.bounding_shape.x * 1.5f,
		sphere_props.bounding_shape.y * 1.f,
		sphere_props.bounding_shape.z * 2.f,
		sphere_props.position);

	m_game_objects.push_back(m_car);
	m_physics_manager = engine::bullet_manager::create(m_game_objects);

	m_text_manager = engine::text_manager::create();

	m_skinned_mesh->switch_animation(1);

	m_grenade.initialise(m_ball);

	/*for (int i = 0; i < 10; i++)
	{
		m_alien_grenades.push_back(m_grenade);
	}*/

	m_explosion = billboard::create("assets/textures/Explosion.tga", 4, 5, 16);

	m_smoke_trail.load_texture("assets/textures/smokeBlackAlpha.tga");
}

example_layer::~example_layer() {}

void example_layer::on_update(const engine::timestep& time_step) 
{
	if (startgame != true)
	{
		m_cross_fade->activate();
	}
	else if (pausegame == true)
	{
		m_cross_fade_pause->activate();
	}
	else if (game_over == true)
	{
		m_gameOver->activate();
	}
	else
	{
		//Assigning variables for the player and current obstacle position
		glm::vec3 pos = m_player.object()->position();
		glm::vec3 obs_pos = m_road_block_obstacle.object()->position();

		m_physics_manager->dynamics_world_update(m_game_objects, double(time_step));

		//Check the time for day/night change
		if (dayNight_timer <= 0)
			day = true;
		else if (dayNight_timer >= 13)
			day = false;

		if (day == true)
		{
			dayNight_timer += time_step;
			m_directionalLight.AmbientIntensity -= 0.05 * time_step;
			m_directionalLight.DiffuseIntensity -= 0.05 * time_step;
		}
		else
		{
			dayNight_timer -= time_step;
			m_directionalLight.AmbientIntensity += 0.05 * time_step;
			m_directionalLight.DiffuseIntensity += 0.05 * time_step;
		}

		//Check the time of the pickup to stay activated
		if (m_pickup_launcher->activate() == true)
		{
			launcher_timer += time_step;
		}
		else
		{
			launcher_timer = 0;
		}

		m_pickup_launcher->update(m_player.object()->position(), time_step);

		m_pickup_life->update(m_player.object()->position(), time_step);

		//Activate the cross_fades
		m_camera_angle_crossfade->activate();
		m_direction_crossfade->activate();
		m_life->activate();
		m_shoot_crossfade->activate();

		//Update the player(Car)
		if (carmoving == true)
		{
			m_distance += time_step * m_player.get_speed();
			m_player.on_update(time_step);
			m_car_box.on_update(glm::vec3(m_player.object()->position().x, 0, m_player.object()->position().z));
			//m_3d_camera.on_update(time_step);
			if (fpcam == false)
				m_player.update_camera(m_3d_camera, time_step, 1, 3, 5, false);
			else
				m_player.update_camera(m_3d_camera, time_step, 0.32, 0.06, 5, true);
			m_audio_manager->play("car");
			m_audio_manager->loop("car", true);
			m_audio_manager->volume("car", 0.02);
		}
		else
		{
			m_player.object()->set_forward(glm::vec3(0, 0, 0));
			m_player.on_update(time_step);
			m_car_box.on_update(glm::vec3(m_player.object()->position().x, 0, m_player.object()->position().z));
		}

		//Updating the cow and enemy AI
		m_obstacle_cow.on_update(time_step, m_player.object()->position());
		m_enemy.on_update(time_step, m_3d_camera.position());//m_player.object()->position());

		//Collision detection of the car with ground
		if (m_player.collision_detection_ground(0))
			m_player.object()->set_position(glm::vec3(m_player.object()->position().x, 0 , m_player.object()->position().z));

		//Bounding box for the obstacles
		if (crashed)
		{
			m_obs_cuboid_box.on_update(glm::vec3(m_cuboid_obstacle.object()->position().x, 0, m_cuboid_obstacle.object()->position().z));
			m_obs_pyramid_box.on_update(glm::vec3(m_pyramid_obstacle.object()->position().x - 1.5, 50, m_pyramid_obstacle.object()->position().z));
			m_obs_road_block_box.on_update(glm::vec3(m_road_block_obstacle.object()->position().x, 50, m_road_block_obstacle.object()->position().z));
			if(render_cow_box)
				m_obs_cow_box.on_update(glm::vec3(m_cow->position().x, 50, m_cow->position().z));
		}
		else
		{
			m_obs_cuboid_box.on_update(glm::vec3(m_cuboid_obstacle.object()->position().x, 0, m_cuboid_obstacle.object()->position().z));
			m_obs_pyramid_box.on_update(glm::vec3(m_pyramid_obstacle.object()->position().x - 1.5, 0, m_pyramid_obstacle.object()->position().z));
			m_obs_road_block_box.on_update(glm::vec3(m_road_block_obstacle.object()->position().x, 0, m_road_block_obstacle.object()->position().z));
			if (render_cow_box)
				m_obs_cow_box.on_update(glm::vec3(m_cow->position().x, 0, m_cow->position().z));
		}

		m_obs_sphere_box.on_update(glm::vec3(m_grenade.object()->position().x, m_grenade.object()->position().y - 0.2, m_grenade.object()->position().z));

		//Check if the car collides with the obstacle and perform the given action.
		if (m_obs_road_block_box.collision(m_car_box) || m_obs_pyramid_box.collision(m_car_box) || m_obs_cuboid_box.collision(m_car_box)
			|| m_obs_cow_box.collision(m_car_box))
		{
			m_audio_manager->play("collision");
			if (life > 0)
			{
				crashed = true;
				m_crash_crossfade->activate();
				m_player.object()->set_position(pos + glm::vec3(0, 0, 1));
				life -= 1;
				carmoving = false;
				crash_timer_start = true;
			}
		}
		//Play the sound when car cashes with the cow
		if (m_obs_cow_box.collision(m_car_box))
			m_audio_manager->play("moo");

		if(life == 0)
		{
			game_over = true;
		}
		

		if (crash_timer_start)
		{
			crash_timer += time_step;
		}

		if (crash_timer > 3)
		{
			crash_timer = 0.0;
			carmoving = true;
			crash_timer_start = false;
		}

		if (m_obs_cuboid_box.collision(m_obs_sphere_box))
		{
			m_audio_manager->play("explosion");
			crashed = true;
			m_explosion->activate(m_cuboid_obstacle.object()->position()- glm::vec3(0, 0, -2), 4.f, 4.f);
		}

		//Shoot if enemy is near
		//if (m_enemy.check_state() == "face_player")
		//{
		//	/*for (auto& shooters : m_alien_grenades)
		//	{
		//		shooters.shoot(m_alien, 200.0f);
		//	}*/
		//	m_grenade.shoot(m_alien, 200.0f);
		//}
			

		//Update the crossfades
		m_camera_angle_crossfade->on_update(time_step);
		m_direction_crossfade->on_update(time_step);
		m_life->on_update(time_step);
		m_crash_crossfade->on_update(time_step);

		//Set the value of life in the crossfade class
		m_life->set_life(life);

		//Update for the moving obstacles
		if (m_pos < -2.0f)
			goright = true;
		else if (m_pos > 2.0f)
		{
			goright = false;
		}

		if (goright)
			m_pos += obs_speed * time_step;
		else
			m_pos -= obs_speed * time_step;

		//Shooting Update
		m_grenade.on_update(time_step);

		//Set the lights with the player
		headlightLeft.Position = m_player.object()->position() + glm::vec3(-0.3f, 0.1f, -0.8f);
		headlightRight.Position = m_player.object()->position() + glm::vec3(0.3f, 0.1f, -0.8f);
		PoliceBluelight.Position = m_player.object()->position() + glm::vec3(0.1f, 0.4f, 0.0f);
		PoliceRedlight.Position = m_player.object()->position() + glm::vec3(-0.1f, 0.4f, 0.0f);

		//Police lights timer
		if (cutofftimer == 1)
		{
			blueRed = true;
			cutofftimer = 0;
		}
		else if (cutofftimer == 0)
		{
			blueRed = false;
			cutofftimer = 1;
		}
		if (blueRed == true)
		{
			PoliceBluelight.Cutoff = 0;
			PoliceRedlight.Cutoff = 1;
		}
		else
		{
			PoliceBluelight.Cutoff = 1;
			PoliceRedlight.Cutoff = 0;
		}

		//Special Effects update
		m_smoke_trail.on_update(time_step);
		m_explosion->on_update(time_step);

		//Set the planet rotation amount
		if (planet_rotation_amount > 0 && planet_rotation_amount < 360)
			planet_rotation_amount += 1;
		else if (planet_rotation_amount == 360)
			planet_rotation_amount = 1;

	}
} 

void example_layer::on_render()
{

	engine::render_command::clear_color({ 0.2f, 0.3f, 0.3f, 1.0f });
	engine::render_command::clear();

	//const auto textured_shader = engine::renderer::shaders_library()->get("mesh_static");
	//engine::renderer::begin_scene(m_3d_camera, textured_shader);

	const auto textured_lighting_shader = engine::renderer::shaders_library()->get("mesh_lighting");
	engine::renderer::begin_scene(m_3d_camera, textured_lighting_shader);

	// Set up some of the scene's parameters in the shader
	std::dynamic_pointer_cast<engine::gl_shader>(textured_lighting_shader)->set_uniform("gEyeWorldPos", m_3d_camera.position());
	std::dynamic_pointer_cast<engine::gl_shader>(textured_lighting_shader)->set_uniform("fog_on", m_fog);

	// Position the skybox centred on the player and render it
	m_directionalLight.submit(textured_lighting_shader);

	//Render the Spotlights
	std::dynamic_pointer_cast<engine::gl_shader>(textured_lighting_shader)->set_uniform("gNumSpotLights", (int)num_spot_lights);
	PoliceBluelight.submit(textured_lighting_shader, 2);
	PoliceRedlight.submit(textured_lighting_shader, 3);

	//Night Skybox render
	if (day == false)
	{
		glm::mat4 skybox_tranform(1.0f);
		skybox_tranform = glm::translate(skybox_tranform, m_3d_camera.position());
		for (const auto& texture : m_skybox_night->textures())
		{
			texture->bind();
		}
		skybox_tranform = glm::rotate(skybox_tranform, glm::radians(planet_rotation_amount), glm::vec3(0.f, 1.f, 0.f));
		engine::renderer::submit(textured_lighting_shader, m_skybox_night, skybox_tranform);
		headlightLeft.submit(textured_lighting_shader, 0);
		headlightRight.submit(textured_lighting_shader, 1);
	}
	else//Day Skybox render
	{
		glm::mat4 skybox_tranform1(1.0f);
		skybox_tranform1 = glm::translate(skybox_tranform1, m_3d_camera.position());
		for (const auto& texture : m_skybox_day->textures())
		{
			texture->bind();
		}
		skybox_tranform1 = glm::rotate(skybox_tranform1, glm::radians(planet_rotation_amount), glm::vec3(0.f, 1.f, 0.f));
		engine::renderer::submit(textured_lighting_shader, m_skybox_day, skybox_tranform1);
	}

	// Render the endless terrain
	if (m_3d_camera.position().z < pos - 10)
	{
		glm::mat4 terrain_transform(1.0f);
		terrain_transform = glm::translate(terrain_transform, glm::vec3(0.f, 0.f, pos - 20));
		terrain_transform = glm::rotate(terrain_transform, m_terrain->rotation_amount(), m_terrain->rotation_axis());
		terrain_transform = glm::scale(terrain_transform, m_terrain->scale());
		engine::renderer::submit(textured_lighting_shader, terrain_transform, m_terrain);
		glm::mat4 terrain_transform2(1.0f);
		terrain_transform2 = glm::translate(terrain_transform2, glm::vec3(-3.7f, 0.f, pos - 10));
		terrain_transform2 = glm::rotate(terrain_transform2, glm::radians(155.f), glm::vec3(0.f, 0.f, 1.f));
		terrain_transform2 = glm::rotate(terrain_transform2, glm::radians(180.f), glm::vec3(1.f, 0.f, 0.f));
		terrain_transform2 = glm::scale(terrain_transform2, m_terrain->scale());
		engine::renderer::submit(textured_lighting_shader, terrain_transform2, m_bound);
		glm::mat4 terrain_transform3(1.0f);
		terrain_transform3 = glm::translate(terrain_transform3, glm::vec3(3.7f, 0.f, pos - 10));
		terrain_transform3 = glm::rotate(terrain_transform3, glm::radians(25.f), glm::vec3(0.f, 0.f, 1.f));
		terrain_transform2 = glm::rotate(terrain_transform2, glm::radians(180.f), glm::vec3(1.f, 0.f, 0.f));
		terrain_transform3 = glm::scale(terrain_transform3, m_terrain->scale());
		engine::renderer::submit(textured_lighting_shader, terrain_transform3, m_bound);
		pos -= 10;
	}
	else
	{
		float pos_next = pos;
		float pos_next1 = pos;
		for (int i = 1; i < 10; i++)
		{
			glm::mat4 terrain_transform(1.0f);
			terrain_transform = glm::translate(terrain_transform, glm::vec3(0.f, 0.f, pos_next - 10));
			terrain_transform = glm::rotate(terrain_transform, m_terrain->rotation_amount(), m_terrain->rotation_axis());
			terrain_transform = glm::scale(terrain_transform, m_terrain->scale());
			engine::renderer::submit(textured_lighting_shader, terrain_transform, m_terrain);
			pos_next -= 20;
		}
		for (int i = 1; i < 50; i++)
		{
			glm::mat4 terrain_transform2(1.0f);
			terrain_transform2 = glm::translate(terrain_transform2, glm::vec3(-3.7f, 0.f, pos_next1 - 5));
			terrain_transform2 = glm::rotate(terrain_transform2, glm::radians(155.f), glm::vec3(0.f, 0.f, 1.f));
			terrain_transform2 = glm::rotate(terrain_transform2, glm::radians(180.f), glm::vec3(1.f, 0.f, 0.f));
			terrain_transform2 = glm::scale(terrain_transform2, m_terrain->scale());
			engine::renderer::submit(textured_lighting_shader, terrain_transform2, m_bound);
			glm::mat4 terrain_transform3(1.0f);
			terrain_transform3 = glm::translate(terrain_transform3, glm::vec3(3.7f, 0.f, pos_next1 - 5));
			terrain_transform3 = glm::rotate(terrain_transform3, glm::radians(25.f), glm::vec3(0.f, 0.f, 1.f));
			terrain_transform2 = glm::rotate(terrain_transform2, glm::radians(180.f), glm::vec3(1.f, 0.f, 0.f));
			terrain_transform3 = glm::scale(terrain_transform3, m_terrain->scale());
			engine::renderer::submit(textured_lighting_shader, terrain_transform3, m_bound);
			pos_next1 -= 4;
		}
	}

	//Show the bounding box on key press
	if (showBoundingBox)
	{
		m_car_box.on_render(5.5f, 0.f, 0.f, textured_lighting_shader);
		m_obs_cuboid_box.on_render(5.5f, 0.f, 0.f, textured_lighting_shader);
		m_obs_pyramid_box.on_render(5.5f, 0.f, 0.f, textured_lighting_shader);
		m_obs_road_block_box.on_render(5.5f, 0.f, 0.f, textured_lighting_shader);
		m_obs_sphere_box.on_render(5.5f, 0.f, 0.f, textured_lighting_shader);
		m_obs_cow_box.on_render(5.5f, 0.f, 0.f, textured_lighting_shader);
	}

	//Render the grenage and explosion
	m_grenade.on_render(textured_lighting_shader);
	m_explosion->on_render(m_3d_camera, textured_lighting_shader);

	//Render the life shape octahedron anmd launcher as pickup
	//and activate the respective functionality
	if (m_pickup_life->active() == true)
	{
		if (m_distance > pickupgenrange && m_distance < (pickupgenrange + 50) && generated_once == false)
		{
			istimetopickup = true;
			generated_once = true;
		}
		else if (m_distance > (pickupgenrange + 50))
		{
			pickupgenrange = pickupgenrange + 50;
			generated_once = false;
		}
		else
			istimetopickup = false;

		if (istimetopickup == true)
		{
			genrandpospickup = true;
		}
		if (genrandpospickup)
		{
			float pos2 = m_player.object()->position().z - 20.0;
			zpospickup = rand() % 15 + (pos2);
			xpospickup = rand() % 3 + (-2);
			pickup_index = rand() % 2 + 1;
			genrandpospickup = false;
			m_pickup_life->set_position(glm::vec3(xpospickup, 0.5, zpospickup));
			m_pickup_launcher->set_position(glm::vec3(xpospickup, 0.3, zpospickup));
		}
		if (pickup_index == 1)
			engine::renderer::submit(textured_lighting_shader, m_pickup_life);
		else if (pickup_index == 2)
			engine::renderer::submit(textured_lighting_shader, m_pickup_launcher);
	}
	else if (m_pickup_life->active() == false && act_pickup == true && pickup_index == 1)
	{
		act_pickup = false;
		life = life + 1;
	}
	else if (m_pickup_launcher->active() == false && m_pickup_launcher->activate() == true && launcher_timer <= 12 && pickup_index == 2)
	{
		//launcher in car render
		glm::mat4 launcher_transform(1.0f);
		launcher_transform = glm::translate(launcher_transform, glm::vec3(m_player.object()->position().x,
			m_player.object()->position().y + 0.5, m_player.object()->position().z));
		launcher_transform = glm::rotate(launcher_transform, glm::radians(90.f), glm::vec3(1.f, 0.f, 0.f));
		launcher_transform = glm::scale(launcher_transform, m_launcher->scale());
		engine::renderer::submit(textured_lighting_shader, launcher_transform, m_launcher);
	}
	else
	{
		m_pickup_launcher->activate_pickup = false;
	}

	//Render the trees
	for (int i = 0; i < 50; i++)
	{
		glm::mat4 tree_transform(1.0f);
		tree_transform = glm::translate(tree_transform, glm::vec3(3.5f, 0.1f, pos - i * 1.5f));
		tree_transform = glm::rotate(tree_transform, m_tree->rotation_amount(), m_tree->rotation_axis());
		tree_transform = glm::scale(tree_transform, m_tree->scale());
		engine::renderer::submit(textured_lighting_shader, tree_transform, m_tree);

		glm::mat4 tree_transform1(1.0f);
		tree_transform1 = glm::translate(tree_transform1, glm::vec3(-3.5f, 0.1f, pos - i * 1.5f));
		tree_transform1 = glm::rotate(tree_transform1, m_tree->rotation_amount(), m_tree->rotation_axis());
		tree_transform1 = glm::scale(tree_transform1, m_tree->scale());
		engine::renderer::submit(textured_lighting_shader, tree_transform1, m_tree);

		glm::mat4 rb_transform(1.0f);
		rb_transform = glm::translate(rb_transform, glm::vec3(3.5f, 0.1f, pos - (i - 2) * 2.0f));
		rb_transform = glm::rotate(rb_transform, glm::radians(90.f), glm::vec3(0.f, 1.f, 0.f));
		rb_transform = glm::scale(rb_transform, glm::vec3(0.3f, 0.2f, 0.3f));
		engine::renderer::submit(textured_lighting_shader, rb_transform, m_rb);

		glm::mat4 rb_transform1(1.0f);
		rb_transform1 = glm::translate(rb_transform1, glm::vec3(-3.5f, 0.1f, pos - (i - 2) * 2.0f));
		rb_transform1 = glm::rotate(rb_transform1, glm::radians(90.f), glm::vec3(0.f, -1.f, 0.f));
		rb_transform1 = glm::scale(rb_transform1, glm::vec3(0.3f, 0.2f, 0.3f));
		engine::renderer::submit(textured_lighting_shader, rb_transform1, m_rb);
	}

	//AI for shooting alien
	if (m_distance > AIspawnRange && m_distance < AIspawnRange + 100)
	{
		istimeforAI = true;
	}
	else if (m_distance > (pickupgenrange + 100))
	{
		AIspawnRange = AIspawnRange + 100;
	}
	else
		istimeforAI = false;

	if (istimeforAI == true)
	{
		float pos2 = m_player.object()->position().z - 50.0;
		shooting_cow_zpos = rand() % 15 + (pos2);
		generatedonceAI = false;
		m_alien->set_position(glm::vec3(-2, 0.5, zpospickup));
	}
	engine::renderer::submit(textured_lighting_shader, m_alien);

	//Render the Car /Interior
	glm::mat4 car_transform(1.0f);
	car_transform = glm::translate(car_transform, m_car->position() - m_car->offset() * m_car->scale());
	car_transform = glm::rotate(car_transform, glm::radians(90.f), glm::vec3(0.f, 1.f, 0.f));
	car_transform = glm::scale(car_transform, glm::vec3(0.3f, 0.3f, 0.3f));
	engine::renderer::submit(textured_lighting_shader, car_transform, m_player.object());

	//Smoke trail for the Car
	m_smoke_trail.on_render(m_3d_camera, textured_lighting_shader);

	//Render the planet
	glm::mat4 planet_transform(1.0f);
	planet_transform = glm::translate(planet_transform, glm::vec3(15, m_3d_camera.position().y + 3, m_3d_camera.position().z - 50));
	planet_transform = glm::rotate(planet_transform, glm::radians(planet_rotation_amount), glm::vec3(0.5f, -0.5f, 0.f));
	planet_transform = glm::scale(planet_transform, m_planet->scale());
	engine::renderer::submit(textured_lighting_shader, planet_transform, m_planet);

	float pos1 = m_3d_camera.position().z - 2.f;

	//Render the obstacles randomly based on the index generated from check_obstacle() function
		if (pos1 < obs_zpos - 2)
		{
			crashed = false;
			render_cow_box = false;
			//obs_xpos = -1.3 + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (1.3 - (-1.3))));
			//py_xpos = -1.0 + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (1.0 - (-1.0))));
			obs_xpos = rand() % 3 + (-2);
			py_xpos = rand() % 3 + (-1);
			obs_zpos -= 15;
			obsIndex = check_Obstacle();
			m_cow->set_position(glm::vec3(obs_xpos, 0, obs_zpos));
		}
		if (obsIndex == 1)
		{
			if(crashed)
				m_road_block_obstacle.on_render(textured_lighting_shader, 50, obs_zpos, 1);
			else
				m_road_block_obstacle.on_render(textured_lighting_shader, obs_xpos, obs_zpos, 1);
			glm::mat4 roadblock_transform(1.0f);
			roadblock_transform = glm::translate(roadblock_transform, m_road_block_obstacle.object()->position());
			roadblock_transform = glm::rotate(roadblock_transform, glm::radians(90.f), glm::vec3(0.0f, 0.f, -1.f));
			roadblock_transform = glm::scale(roadblock_transform, m_roadBlock->scale());
			engine::renderer::submit(textured_lighting_shader, roadblock_transform, m_road_block_obstacle.object());
		}
		else if (obsIndex == 2)
		{
			if (crashed)
			{
				m_pyramid_obstacle.on_render(textured_lighting_shader, 50, obs_zpos, 2);
				m_pyramid_obstacle.on_render(textured_lighting_shader, 50 + 1, obs_zpos, 2);
				m_pyramid_obstacle.on_render(textured_lighting_shader, 50 + 2, obs_zpos, 2);
				m_pyramid_obstacle.on_render(textured_lighting_shader, 50 + 3, obs_zpos, 2);
			}
			else
			{
				m_pyramid_obstacle.on_render(textured_lighting_shader, py_xpos, obs_zpos, 2);
				m_pyramid_obstacle.on_render(textured_lighting_shader, py_xpos + 1, obs_zpos, 2);
				m_pyramid_obstacle.on_render(textured_lighting_shader, py_xpos + 2, obs_zpos, 2);
				m_pyramid_obstacle.on_render(textured_lighting_shader, py_xpos + 3, obs_zpos, 2);
			}
		}
		else if (obsIndex == 3)
		{
			if (crashed)
				m_road_block_obstacle.on_render(textured_lighting_shader, 50, obs_zpos, 1);
			else
				m_road_block_obstacle.on_render(textured_lighting_shader, m_pos, obs_zpos, 1);
			glm::mat4 roadblock_transform(1.0f);
			roadblock_transform = glm::translate(roadblock_transform, m_road_block_obstacle.object()->position());
			roadblock_transform = glm::rotate(roadblock_transform, glm::radians(90.f), glm::vec3(0.0f, 0.f, -1.f));
			roadblock_transform = glm::scale(roadblock_transform, m_roadBlock->scale());
			engine::renderer::submit(textured_lighting_shader, roadblock_transform, m_road_block_obstacle.object());
		}
		else if (obsIndex == 4)
		{
			render_cow_box = true;
			glm::mat4 cow_transform(1.0f);
			cow_transform = glm::translate(cow_transform, m_cow->position());
			cow_transform = glm::rotate(cow_transform, m_cow->rotation_amount(), m_cow->rotation_axis());
			cow_transform = glm::scale(cow_transform, m_cow->scale());
			engine::renderer::submit(textured_lighting_shader, cow_transform, m_cow);	
		}
		else if (obsIndex == 5)
		{
			if (crashed)
				m_cuboid_obstacle.on_render(textured_lighting_shader, 50, obs_zpos, 1);
			else
				m_cuboid_obstacle.on_render(textured_lighting_shader, 0, obs_zpos, 1);
			glm::mat4 obs_transform6(1.0f);
			obs_transform6 = glm::translate(obs_transform6, m_cuboid_obstacle.object()->position());
			obs_transform6 = glm::rotate(obs_transform6, m_obs->rotation_amount(), m_obs->rotation_axis());
			obs_transform6 = glm::scale(obs_transform6, glm::vec3(3, 4.f, 1));
			engine::renderer::submit(textured_lighting_shader, obs_transform6, m_cuboid_obstacle.object());
		}

    engine::renderer::end_scene();

	// Set up material shader. (does not render textures, renders materials instead)
	const auto material_shader = engine::renderer::shaders_library()->get("mesh_material");
	engine::renderer::begin_scene(m_3d_camera, material_shader);

	m_material->submit(material_shader);
	std::dynamic_pointer_cast<engine::gl_shader>(material_shader)->set_uniform("gEyeWorldPos", m_3d_camera.position());

	engine::renderer::end_scene();

	const auto animated_mesh_shader = engine::renderer::shaders_library()->get("animated_mesh");
	engine::renderer::begin_scene(m_3d_camera, animated_mesh_shader);

	std::dynamic_pointer_cast<engine::gl_shader>(animated_mesh_shader)->set_uniform("gEyeWorldPos", m_3d_camera.position());
	std::dynamic_pointer_cast<engine::gl_shader>(animated_mesh_shader)->set_uniform("fog_on", m_fog);

	//glm::mat4 aniTransform = glm::mat4(1.0f);
	//engine::renderer::submit(animated_mesh_shader, m_mannequin);

	engine::renderer::end_scene();

	const auto text_shader = engine::renderer::shaders_library()->get("text_2D");
	engine::renderer::begin_scene(m_2d_camera, textured_lighting_shader);

	//Render the crossfades and texts for the startscreen, pausescreen and amin screen

	if (startgame != true)
	{
		//Render the intro Screen
		std::dynamic_pointer_cast<engine::gl_shader>(textured_lighting_shader)->set_uniform("lighting_on", false);

		m_cross_fade->on_render(textured_lighting_shader,1);

		std::dynamic_pointer_cast<engine::gl_shader>(textured_lighting_shader)->set_uniform("lighting_on", true);

	}
	else if (pausegame == true)
	{
		//Render the pause screen
		std::dynamic_pointer_cast<engine::gl_shader>(textured_lighting_shader)->set_uniform("lighting_on", false);

		m_cross_fade_pause->on_render(textured_lighting_shader, 1);

		std::dynamic_pointer_cast<engine::gl_shader>(textured_lighting_shader)->set_uniform("lighting_on", true);

	}
	else if (game_over == true)
	{
		//Render the game over screen
		std::dynamic_pointer_cast<engine::gl_shader>(textured_lighting_shader)->set_uniform("lighting_on", false);

		m_gameOver->on_render(textured_lighting_shader, 5);

		std::dynamic_pointer_cast<engine::gl_shader>(textured_lighting_shader)->set_uniform("lighting_on", true);
		m_text_manager->render_text(text_shader, "YOU CRASHED", 390.f,
			(float)engine::application::window().height() - 400.f, 2.f, glm::vec4(1.f, 0.5f, 0.f, 1.f));
		m_text_manager->render_text(text_shader, "DISTANCE", 480.f,
			(float)engine::application::window().height() - 500.f, 1.7f, glm::vec4(1.f, 0.5f, 0.f, 1.f));
		m_text_manager->render_text(text_shader, std::to_string((int)m_distance) + "m", 550.f,
			(float)engine::application::window().height() - 580.f, 1.5f, glm::vec4(1.f, 0.0f, 0.f, 1.f));
		m_text_manager->render_text(text_shader, "PRESS ESC TO QUIT", 380.f,
			(float)engine::application::window().height() - 650.f, 1.5f, glm::vec4(1.f, 0.5f, 0.f, 1.f));
	}
	else if (fpcam == false && startgame == true && pausegame == false)
	{
		std::dynamic_pointer_cast<engine::gl_shader>(textured_lighting_shader)->set_uniform("lighting_on", false);

		m_direction_crossfade->on_render(textured_lighting_shader,2);
		m_camera_angle_crossfade->on_render(textured_lighting_shader,3);
		m_crash_crossfade->on_render(textured_lighting_shader, 1);
		if (m_pickup_launcher->activate())
		{
				m_shoot_crossfade->on_render(textured_lighting_shader, 6);
		}

		for (int i = 0; i < life; i++)
		{
			m_life->on_render(textured_lighting_shader, 4);
		}

		std::dynamic_pointer_cast<engine::gl_shader>(textured_lighting_shader)->set_uniform("lighting_on", true);

		// Render texts
		m_text_manager->render_text(text_shader, "TO MOVE LEFT", 1.f,
			(float)engine::application::window().height() - 640.f, 0.4f, glm::vec4(0.f, 1.f, 0.f, 1.f));
		m_text_manager->render_text(text_shader, "TO MOVE RIGHT", 150.f,
			(float)engine::application::window().height() - 640.f, 0.4f, glm::vec4(1.f, 1.f, 0.f, 1.f));
		m_text_manager->render_text(text_shader, "TO CHANGE CAMERA VIEW", 350.f,
			(float)engine::application::window().height() - 640.f, 0.4f, glm::vec4(1.f, 1.f, 0.49f, 1.f));

		if (m_pickup_launcher->activate())
		{
			m_text_manager->render_text(text_shader, "TO SHOOT", 785.f,
				(float)engine::application::window().height() - 640.f, 0.4f, glm::vec4(1.f, 1.f, 0.49f, 1.f));
		}

		m_text_manager->render_text(text_shader, "Distance Travelled: " + std::to_string((int)m_distance) + "m", 10.f,
			(float)engine::application::window().height() - 35.f, 1.0f, glm::vec4(1.f, 0.f, 0.49f, 1.f));
		m_text_manager->render_text(text_shader, "Life: ", 10.f,
			(float)engine::application::window().height() - 100.f, 1.0f, glm::vec4(1.f, 0.5f, 0.f, 1.f));

	}

	engine::renderer::end_scene();

} 

void example_layer::on_event(engine::event& event) 
{ 
    if(event.event_type() == engine::event_type_e::key_pressed) 
    { 
        auto& e = dynamic_cast<engine::key_pressed_event&>(event); 
        if(e.key_code() == engine::key_codes::KEY_TAB) 
        { 
            engine::render_command::toggle_wireframe();
        }
		else if (e.key_code() == engine::key_codes::KEY_ENTER)
		{
			startgame = true;
		}
		else if (e.key_code() == engine::key_codes::KEY_P)
		{
			pausegame = true;
		}
		else if (e.key_code() == engine::key_codes::KEY_R)
		{
			pausegame = false;
		}
		else if (e.key_code() == engine::key_codes::KEY_C && startgame == true)
		{
			if (fpcam)
				fpcam = false;
			else
				fpcam = true;
		}
		else if (e.key_code() == engine::key_codes::KEY_F && m_pickup_launcher->activate() == true)
		{
			m_audio_manager->play("explosion");
			//m_audio_manager->volume("explosion", 1);
			m_grenade.shoot(m_player.object(), 200.0f);
		}
		else if (e.key_code() == engine::key_codes::KEY_1)
		{
			showBoundingBox = !showBoundingBox;
		}
		else if (e.key_code() == engine::key_codes::KEY_2)
		{
			m_fog = !m_fog;
		}

    } 
}

void example_layer::check_bounce()
{
	//if (m_prev_sphere_y_vel < 0.f && m_game_objects.at(1)->velocity().y > 0.f)
		//m_audio_manager->play("bounce");
	m_prev_sphere_y_vel = m_game_objects.at(1)->velocity().y;
}

//Generate a random number and depending on that the different obstacles are generated.
int example_layer::check_Obstacle()
{
	if (m_pickup_launcher->activate() == true)
	{
		int randObs = 5;
		return randObs;
	}
	else
	{
		int randObs = rand() % 4 + 1;
		return randObs;
	}
	
}
