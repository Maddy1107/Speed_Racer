#include "pch.h"
#include "road_block.h"
#include <engine.h>

engine::road_block::road_block(glm::vec3 half_extents, bool inwards) : m_half_extents(half_extents), m_inwards(inwards)
{
	float orientation = 1;
	if (inwards)
		orientation = -1;

	std::vector<mesh::vertex> road_block_vertices
	{
		////front
		////                                 position														 normal					      tex coord       
		{ {-0.6f * m_half_extents.x, -1.f * m_half_extents.y,  0.1f * m_half_extents.z },		{ -1.0f, 0.0f,  1.0f * orientation},		{ 0.f, 0.f } },
		{ { 0.3f * m_half_extents.x , -1.2f * m_half_extents.y, 0.2f * m_half_extents.z },		{ -1.0f, 0.0f,  1.0f * orientation},		{ 1.f, 0.f } },
		{ { 0.3f * m_half_extents.x,	1.2f * m_half_extents.y,  0.2f * m_half_extents.z },		{ -1.0f, 0.0f,  1.0f * orientation},		{ 1.f, 1.f } },
		{ {-0.6f * m_half_extents.x,	1.f * m_half_extents.y,  0.1f * m_half_extents.z},			{ -1.0f, 0.0f,  1.0f * orientation},		{ 0.f, 1.f } },

		//back
		{ { 0.3f * m_half_extents.x, -1.2f * m_half_extents.y, -0.2f * m_half_extents.z },		{ 0.0f, 0.0f, -1.0f * orientation},		{ 0.f, 0.f } },
		{ {-0.6f * m_half_extents.x, -1.f * m_half_extents.y, -0.1f * m_half_extents.z },		{ 0.0f, 0.0f, -1.0f * orientation},		{ 1.f, 0.f } },
		{ {-0.6f * m_half_extents.x,	1.f * m_half_extents.y, -0.1f * m_half_extents.z },		{ 0.0f, 0.0f, -1.0f * orientation},		{ 1.f, 1.f } },
		{ { 0.3f * m_half_extents.x, 1.2f * m_half_extents.y, -0.2f * m_half_extents.z },		{ 0.0f, 0.0f, -1.0f * orientation},		{ 0.f, 1.f } },

		//right
		{ { 0.3f * m_half_extents.x, -1.2f * m_half_extents.y,  0.2f * m_half_extents.z},		{ -1.0f * orientation, 0.0f, 0.0f},		{ 0.f, 0.f } },
		{ { 0.7f * m_half_extents.x, -1.4f * m_half_extents.y, 0.4f * m_half_extents.z },		{ -1.0f * orientation, 0.0f, 0.0f},		{ 1.f, 0.f } },
		{ { 0.7f * m_half_extents.x,	1.4f * m_half_extents.y, 0.4f * m_half_extents.z },		{ -1.0f * orientation, 0.0f, 0.0f},		{ 1.f, 1.f } },
		{ { 0.3f * m_half_extents.x,	1.2f * m_half_extents.y,  0.2f * m_half_extents.z },		{ -1.0f * orientation, 0.0f, 0.0f},		{ 0.f, 1.f } },

		//right1
		{ { 0.7f * m_half_extents.x, -1.4f * m_half_extents.y,  -0.4f * m_half_extents.z},		{ 1.0f * orientation, 0.0f, 0.0f},		{ 0.f, 0.f } },
		{ { 0.3f * m_half_extents.x, -1.2f * m_half_extents.y, -0.2f * m_half_extents.z },		{ 1.0f * orientation, 0.0f, 0.0f},		{ 1.f, 0.f } },
		{ { 0.3f * m_half_extents.x,	1.2f * m_half_extents.y, -0.2f * m_half_extents.z },		{ 1.0f * orientation, 0.0f, 0.0f},		{ 1.f, 1.f } },
		{ { 0.7f * m_half_extents.x,	1.4f * m_half_extents.y,  -0.4f * m_half_extents.z },		{ 1.0f * orientation, 0.0f, 0.0f},		{ 0.f, 1.f } },

		//right2
		{ { 1.f * m_half_extents.x, -1.4f * m_half_extents.y,  -0.4f * m_half_extents.z},		{ 1.0f * orientation, 0.0f, 0.0f},		{ 0.f, 0.f } },
		{ { 0.7f * m_half_extents.x, -1.4f * m_half_extents.y, -0.4f * m_half_extents.z },		{ 1.0f * orientation, 0.0f, 0.0f},		{ 1.f, 0.f } },
		{ { 0.7f * m_half_extents.x,  1.4f * m_half_extents.y, -0.4f * m_half_extents.z },		{ 1.0f * orientation, 0.0f, 0.0f},		{ 1.f, 1.f } },
		{ { 1.f * m_half_extents.x,  1.4f * m_half_extents.y,  -0.4f * m_half_extents.z },		{ 1.0f * orientation, 0.0f, 0.0f},		{ 0.f, 1.f } },

		//right3
		{ { 0.7f * m_half_extents.x, -1.4f * m_half_extents.y,  0.4f * m_half_extents.z},		{ -1.0f * orientation, 0.0f, 0.0f},		{ 0.f, 0.f } },
		{ { 1.f * m_half_extents.x, -1.4f * m_half_extents.y, 0.4f * m_half_extents.z },		{ -1.0f * orientation, 0.0f, 0.0f},		{ 1.f, 0.f } },
		{ { 1.f * m_half_extents.x,	1.4f * m_half_extents.y, 0.4f * m_half_extents.z },		{ -1.0f * orientation, 0.0f, 0.0f},		{ 1.f, 1.f } },
		{ { 0.7f * m_half_extents.x,	1.4f * m_half_extents.y,  0.4f * m_half_extents.z },		{ -1.0f * orientation, 0.0f, 0.0f},		{ 0.f, 1.f } },

		//left
		{ {-0.6f * m_half_extents.x, -1.f * m_half_extents.y, -0.1f * m_half_extents.z },		{-1.0f * orientation, 0.0f, 0.0f},		{ 0.f, 0.f } },
		{ {-0.6f * m_half_extents.x, -1.f * m_half_extents.y,  0.1f * m_half_extents.z },		{-1.0f * orientation, 0.0f, 0.0f},		{ 1.f, 0.f } },
		{ {-0.6f * m_half_extents.x,	1.f * m_half_extents.y,  0.1f * m_half_extents.z },		{-1.0f * orientation, 0.0f, 0.0f},		{ 1.f, 1.f } },
		{ {-0.6f * m_half_extents.x,	1.f * m_half_extents.y, -0.1f * m_half_extents.z },		{-1.0f * orientation, 0.0f, 0.0f},		{ 0.f, 1.f } },

		//top
		{ {-0.6f * m_half_extents.x,  1.f * m_half_extents.y,  0.1f * m_half_extents.z },		{ 0.0f,  1.0f * orientation, 0.0f},		{ 0.f, 0.f } },
		{ { 0.3f * m_half_extents.x,  1.2f * m_half_extents.y,  0.2f * m_half_extents.z },		{ 0.0f,  1.0f * orientation, 0.0f},		{ 1.f, 0.f } },
		{ { 0.3f * m_half_extents.x,  1.2f * m_half_extents.y, -0.2f * m_half_extents.z },		{ 0.0f,  1.0f * orientation, 0.0f},		{ 1.f, 1.f } },
		{ {-0.6f * m_half_extents.x,  1.f * m_half_extents.y, -0.1f * m_half_extents.z },		{ 0.0f,  1.0f * orientation, 0.0f},		{ 0.f, 1.f } },

		//top1
		{ {0.3f * m_half_extents.x,  1.2f * m_half_extents.y,  0.2f * m_half_extents.z },		{ 0.0f,  1.0f * orientation, 0.0f},		{ 0.f, 0.f } },
		{ { 0.7f * m_half_extents.x,  1.4f * m_half_extents.y,  0.4f * m_half_extents.z },		{ 0.0f,  1.0f * orientation, 0.0f},		{ 1.f, 0.f } },
		{ { 0.7f * m_half_extents.x,  1.4f * m_half_extents.y, -0.4f * m_half_extents.z },		{ 0.0f,  1.0f * orientation, 0.0f},		{ 1.f, 1.f } },
		{ {0.3f * m_half_extents.x,  1.2f * m_half_extents.y, -0.2f * m_half_extents.z },		{ 0.0f,  1.0f * orientation, 0.0f},		{ 0.f, 1.f } },

		//top2
		{ {0.7f * m_half_extents.x,  1.4f * m_half_extents.y,  0.4f * m_half_extents.z },		{ 0.0f,  1.0f * orientation, 0.0f},		{ 0.f, 0.f } },
		{ { 1.f * m_half_extents.x,  1.4f * m_half_extents.y,  0.4f * m_half_extents.z },		{ 0.0f,  1.0f * orientation, 0.0f},		{ 1.f, 0.f } },
		{ { 1.f * m_half_extents.x,  1.4f * m_half_extents.y, -0.4f * m_half_extents.z },		{ 0.0f,  1.0f * orientation, 0.0f},		{ 1.f, 1.f } },
		{ {0.7f * m_half_extents.x,  1.4f * m_half_extents.y, -0.4f * m_half_extents.z },		{ 0.0f,  1.0f * orientation, 0.0f},		{ 0.f, 1.f } },

		//bottom
		{ {-0.6f * m_half_extents.x, -1.f * m_half_extents.y, -0.1f * m_half_extents.z },		{ 0.0f, -1.0f * orientation, 0.0f},		{ 0.f, 0.f } },
		{ { 0.3f * m_half_extents.x, -1.2f * m_half_extents.y, -0.2f * m_half_extents.z },		{ 0.0f, -1.0f * orientation, 0.0f},		{ 1.f, 0.f } },
		{ { 0.3f * m_half_extents.x, -1.2f * m_half_extents.y,  0.2f * m_half_extents.z },		{ 0.0f, -1.0f * orientation, 0.0f},		{ 1.f, 1.f } },
		{ {-0.6f * m_half_extents.x, -1.f * m_half_extents.y,  0.1f * m_half_extents.z },		{ 0.0f, -1.0f * orientation, 0.0f},		{ 0.f, 1.f } },

		//bottom1
		{ {0.3f * m_half_extents.x, -1.2f * m_half_extents.y, -0.2f * m_half_extents.z },		{ 0.0f, -1.0f * orientation, 0.0f},		{ 0.f, 0.f } },
		{ { 0.7f * m_half_extents.x, -1.4f * m_half_extents.y, -0.4f * m_half_extents.z },		{ 0.0f, -1.0f * orientation, 0.0f},		{ 1.f, 0.f } },
		{ { 0.7f * m_half_extents.x, -1.4f * m_half_extents.y,  0.4f * m_half_extents.z },		{ 0.0f, -1.0f * orientation, 0.0f},		{ 1.f, 1.f } },
		{ {0.3f * m_half_extents.x, -1.2f * m_half_extents.y,  0.2f * m_half_extents.z },		{ 0.0f, -1.0f * orientation, 0.0f},		{ 0.f, 1.f } },

		//bottom2
		{ {0.7f * m_half_extents.x, -1.4f * m_half_extents.y, -0.4f * m_half_extents.z },		{ 0.0f, -1.0f * orientation, 0.0f},		{ 0.f, 0.f } },
		{ { 1.f * m_half_extents.x, -1.4f * m_half_extents.y, -0.4f * m_half_extents.z },		{ 0.0f, -1.0f * orientation, 0.0f},		{ 1.f, 0.f } },
		{ { 1.f * m_half_extents.x, -1.4f * m_half_extents.y,  0.4f * m_half_extents.z },		{ 0.0f, -1.0f * orientation, 0.0f},		{ 1.f, 1.f } },
		{ {0.7f * m_half_extents.x, -1.4f * m_half_extents.y,  0.4f * m_half_extents.z },		{ 0.0f, -1.0f * orientation, 0.0f},		{ 0.f, 1.f } },

		//right4
		{ { 1.f * m_half_extents.x, -1.4f * m_half_extents.y,  0.4f * m_half_extents.z },		{ 1.0f * orientation, 0.0f, 0.0f},		{ 0.f, 0.f } },
		{ { 1.f * m_half_extents.x, -1.4f * m_half_extents.y, -0.4f * m_half_extents.z },		{ 1.0f * orientation, 0.0f, 0.0f},		{ 1.f, 0.f } },
		{ { 1.f * m_half_extents.x,  1.4f * m_half_extents.y, -0.4f * m_half_extents.z },		{ 1.0f * orientation, 0.0f, 0.0f},		{ 1.f, 1.f } },
		{ { 1.f * m_half_extents.x,  1.4f * m_half_extents.y,  0.4f * m_half_extents.z },		{ 1.0f * orientation, 0.0f, 0.0f},		{ 0.f, 1.f } },
	};

	const std::vector<uint32_t> road_block_indices
	{
		 0,  1,  2,		 0,  2,  3,  //front
		 4,  5,  6,		 4,  6,  7,  //right
		 8,  9, 10,		 8, 10, 11,  //right1
		12, 13, 14,		12, 14, 15,  //back
		16, 17, 18,		16, 18, 19,  //left
		20, 21, 22,		20, 22, 23,  //top1
		24, 25, 26,     24, 26, 27,	 //top1
		28, 29, 30,     28, 30, 31,  //bottom
		32, 33, 34,     32, 34, 35,  //bottom1
		36, 37, 38,     36, 38, 39,  //right4
		40, 41, 42,     40, 42, 43,  //right2
		44, 45, 46,     44, 46, 47,  //right3
		48, 49, 50,     48, 50, 51,  //top1
		52, 53, 54,     52, 54, 55,  //bottom1
	};

	m_mesh = engine::mesh::create(road_block_vertices, road_block_indices);
}

engine::road_block::~road_block() {}

engine::ref<engine::road_block> engine::road_block::create(glm::vec3 half_extents, bool inwards)
{
	return std::make_shared<engine::road_block>(half_extents, inwards);
}
