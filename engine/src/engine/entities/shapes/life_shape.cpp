#include "pch.h"
#include "life_shape.h"
#include <engine.h>		

engine::life_shape::life_shape(std::vector<glm::vec3> vertices) : m_vertices(vertices)
{
	std::vector<glm::vec3> normals;
	// front normal
	normals.push_back(glm::cross(vertices.at(0) - vertices.at(2), vertices.at(0) - vertices.at(3)));
	// left normal
	normals.push_back(glm::cross(vertices.at(0) - vertices.at(1), vertices.at(0) - vertices.at(2)));
	// right normal
	normals.push_back(glm::cross(vertices.at(0) - vertices.at(3), vertices.at(0) - vertices.at(4)));
	// back normal
	normals.push_back(glm::cross(vertices.at(0) - vertices.at(4), vertices.at(0) - vertices.at(1)));
	// down front normal
	normals.push_back(glm::cross(vertices.at(5) - vertices.at(2), vertices.at(5) - vertices.at(3)));
	// down left normal
	normals.push_back(glm::cross(vertices.at(5) - vertices.at(1), vertices.at(5) - vertices.at(2)));
	// down right normal
	normals.push_back(glm::cross(vertices.at(5) - vertices.at(3), vertices.at(5) - vertices.at(4)));
	// down back normal
	normals.push_back(glm::cross(vertices.at(5) - vertices.at(4), vertices.at(5) - vertices.at(1)));

	std::vector<mesh::vertex> life_shape_vertices
	{
		//  position				normal			         tex coord       
		{ vertices.at(0),		    normals.at(0),			{ 0.f,  0.f } },
		{ vertices.at(2),		    normals.at(0),			{ 1.f,  0.f } },
		{ vertices.at(3),		    normals.at(0),			{ 0.5f, 1.f } },

		{ vertices.at(0),		 	normals.at(1),			{ 0.f,  0.f } },
		{ vertices.at(1),			normals.at(1),			{ 1.f,  0.f } },
		{ vertices.at(2),			normals.at(1),			{ 0.5f, 1.f } },

		{ vertices.at(0),			normals.at(2),			{ 0.f,  0.f } },
		{ vertices.at(3),			normals.at(2),			{ 1.f,  0.f } },
		{ vertices.at(4),			normals.at(2),			{ 0.5f, 1.f } },

		{ vertices.at(0),			normals.at(3),			{ 0.f,  0.f } },
		{ vertices.at(4),			normals.at(3),			{ 1.f,  0.f } },
		{ vertices.at(1),			normals.at(3),			{ 0.5f, 1.f } },

		{ vertices.at(5),		    normals.at(5),			{ 0.f,  0.f } },
		{ vertices.at(3),		    normals.at(5),			{ 1.f,  0.f } },
		{ vertices.at(2),		    normals.at(5),			{ 0.5f, 1.f } },

		{ vertices.at(5),		 	normals.at(1),			{ 0.f,  0.f } },
		{ vertices.at(2),			normals.at(1),			{ 1.f,  0.f } },
		{ vertices.at(1),			normals.at(1),			{ 0.5f, 1.f } },

		{ vertices.at(5),			normals.at(2),			{ 0.f,  0.f } },
		{ vertices.at(4),			normals.at(2),			{ 1.f,  0.f } },
		{ vertices.at(3),			normals.at(2),			{ 0.5f, 1.f } },

		{ vertices.at(5),			normals.at(3),			{ 0.f,  0.f } },
		{ vertices.at(1),			normals.at(3),			{ 1.f,  0.f } },
		{ vertices.at(4),			normals.at(3),			{ 0.5f, 1.f } },

	};

	const std::vector<uint32_t> life_shape_indices
	{
		0,  1,  2,	//front
		3,  4,  5,	//left
		6,	7,	8,	//Right
		9,	10,	11,	//Back

		12,  13,  14,	//down front
		15,  16,  17,	//down left
		18,	19,	20,	//down right
		21,	22,	23,	//down back

	};

	m_mesh = engine::mesh::create(life_shape_vertices, life_shape_indices);
}

engine::life_shape::~life_shape() {}

engine::ref<engine::life_shape> engine::life_shape::create(std::vector<glm::vec3> vertices)
{
	return std::make_shared<engine::life_shape>(vertices);
}
