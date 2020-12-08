#pragma once

namespace engine
{
	class mesh;

	/// \brief Class creating a roadBlock object with a mesh of a specified size
	class road_block
	{
	public:
		/// \brief Constructor
		road_block(glm::vec3 half_extents, bool inwards);

		/// \brief Destructor
		~road_block();

		/// \brief Getter methods
		glm::vec3 half_extents() const { return m_half_extents; }
		bool inwards() const { return m_inwards; }
		ref<engine::mesh> mesh() const { return m_mesh; }

		static ref<road_block> create(glm::vec3 half_extents, bool inwards);
	private:
		/// \brief Fields
		// Dimensions of the cuboid specified by a vector of half-extents. Cuboid is centered on the origin in local coordinates.
		glm::vec3 m_half_extents;

		// Boolean recording if the cube should be inverted (used in skybox)
		bool m_inwards;

		ref<engine::mesh> m_mesh;
	};
}
