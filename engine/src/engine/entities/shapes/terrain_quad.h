#pragma once

namespace engine
{
	class mesh;

	/// \brief Class creating a cuboid object with a mesh of a specified size
	class terrain_quad
	{
	public:
		/// \brief Constructor
		terrain_quad(glm::vec3 measurement);

		/// \brief Destructor
		~terrain_quad();

		/// \brief Getter methods
		glm::vec3 half_extents() const { return m_measurements; }
		ref<engine::mesh> mesh() const { return m_mesh; }

		static ref<terrain_quad> create(glm::vec3 half_extents);
	private:
		/// \brief Fields
		// Dimensions of the terrain
		glm::vec3 m_measurements;

		ref<engine::mesh> m_mesh;
	};
}
