#include "pch.h"
#include "pyramid.h"
#include <engine.h>		

engine::pyramid::pyramid(std::vector<glm::vec3> vertices) : m_vertices(vertices)
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
	// bottom normal
	normals.push_back(glm::cross(vertices.at(1) - vertices.at(2), vertices.at(1) - vertices.at(3)));

	std::vector<mesh::vertex> pyramid_vertices
	{
		//  position			normal			         tex coord       
		{ vertices.at(0),		    	normals.at(0),			{ 0.f,  0.f } },
		{ vertices.at(2),		    	normals.at(0),			{ 1.f,  0.f } },
		{ vertices.at(3),		    	normals.at(0),			{ 0.5f, 1.f } },

		{ vertices.at(0),		 	normals.at(1),			{ 0.f,  0.f } },
		{ vertices.at(1),			normals.at(1),			{ 1.f,  0.f } },
		{ vertices.at(2),			normals.at(1),			{ 0.5f, 1.f } },

		{ vertices.at(0),			normals.at(2),			{ 0.f,  0.f } },
		{ vertices.at(3),			normals.at(2),			{ 1.f,  0.f } },
		{ vertices.at(4),			normals.at(2),			{ 0.5f, 1.f } },

		{ vertices.at(0),			normals.at(3),			{ 0.f,  0.f } },
		{ vertices.at(4),			normals.at(3),			{ 1.f,  0.f } },
		{ vertices.at(1),			normals.at(3),			{ 0.5f, 1.f } },

		{ vertices.at(1),			normals.at(4),			{ 0.f,  0.f } },
		{ vertices.at(2),			normals.at(4),			{ 1.f,  0.f } },
		{ vertices.at(3),			normals.at(4),			{ 0.5f, 1.f } },
		{ vertices.at(4),			normals.at(4),			{ 0.5f, 1.f } },

	};

	const std::vector<uint32_t> pyramid_indices
	{
		0,  1,  2,	//front
		3,  4,  5,	//left
		6,	7,	8,	//Right
		9,	10,	11,	//Back
		12, 13, 14, //Bottom
		12, 14, 15, 
	};

	m_mesh = engine::mesh::create(pyramid_vertices, pyramid_indices);
}

engine::pyramid::~pyramid() {}

engine::ref<engine::pyramid> engine::pyramid::create(std::vector<glm::vec3> vertices)
{
	return std::make_shared<engine::pyramid>(vertices);
}
