#include "pch.h"
#include "terrain_quad.h"
#include <engine.h>

engine::terrain_quad::terrain_quad(glm::vec3 measurements) : m_measurements(measurements)
{

	std::vector<mesh::vertex> terrain_quad_vertices
	{
		{ {-1.f * measurements.x,  measurements.y,  1.f * measurements.z },		{ 0.0f,  1.0f, 0.0f},		{ 0.f, 0.f } },
		{ { 1.f * measurements.x,  measurements.y,  1.f * measurements.z },		{ 0.0f,  1.0f, 0.0f},		{ 200.f, 0.f } },
		{ { 1.f * measurements.x,  measurements.y, -1.f * measurements.z },		{ 0.0f,  1.0f, 0.0f},		{ 200.f, 200.f } },
		{ {-1.f * measurements.x,  measurements.y, -1.f * measurements.z },		{ 0.0f,  1.0f, 0.0f},		{ 0.f, 200.f } },
	};

	const std::vector<uint32_t> terrain_quad_indices
	{
		0, 1, 2,		0, 2, 3,
	};

	m_mesh = engine::mesh::create(terrain_quad_vertices, terrain_quad_indices);
}

engine::terrain_quad::~terrain_quad() {}

engine::ref<engine::terrain_quad> engine::terrain_quad::create(glm::vec3 measurements)
{
	return std::make_shared<engine::terrain_quad>(measurements);
}
