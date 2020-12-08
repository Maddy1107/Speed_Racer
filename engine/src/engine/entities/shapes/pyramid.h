#pragma once
namespace engine
{
	class mesh;
	/// \brief Class creating a tetrahedron object with a mesh of a specified size
	class pyramid
	{
	public:
		/// \brief Constructor
		pyramid(std::vector<glm::vec3> vertices);
		/// \brief Destructor
		~pyramid();
		/// \brief Getter methods
		std::vector<glm::vec3> vertices() const { return m_vertices; }
		ref<engine::mesh> mesh() const { return m_mesh; }
		static ref<pyramid> create(std::vector<glm::vec3> vertices);
	private:
		/// \brief Fields
		// vertices of the tetrahedron specified by a vector of positions.
		std::vector<glm::vec3> m_vertices;
		ref<engine::mesh> m_mesh;
	};
}
