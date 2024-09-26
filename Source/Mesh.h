#pragma once

#include "Core.h"
#include <glm/glm.hpp>


struct Vertex
{
	glm::vec2 Position;
};


class Mesh
{
public:
	Mesh(const Vertex* vertices, uint32_t verticesCount, const uint32_t* indices, uint32_t indicesCount);
	~Mesh();

	void Bind();
	uint32_t GetIndicesCount() const { return m_IndicesCount; }

private:
	uint32_t m_VBRendererID = 0;
	uint32_t m_IBRendererID = 0;
	uint32_t m_VAORendererID = 0;
	uint32_t m_IndicesCount = 0;
};
