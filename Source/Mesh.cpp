#include "Mesh.h"

#include <glad/glad.h>


Mesh::Mesh(const Vertex* vertices, uint32_t verticesCount, const uint32_t* indices, uint32_t indicesCount)
	: m_IndicesCount(indicesCount)
{
	// 1. Create GL objects
	glCreateVertexArrays(1, &m_VAORendererID);
	glBindVertexArray(0);

	glCreateBuffers(1, &m_VBRendererID);
	glBindBuffer(GL_ARRAY_BUFFER, m_VBRendererID);
	glBufferData(GL_ARRAY_BUFFER, verticesCount * sizeof(Vertex), vertices, GL_STATIC_DRAW);

	glCreateBuffers(1, &m_IBRendererID);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IBRendererID);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indicesCount * sizeof(uint32_t), indices, GL_STATIC_DRAW);

	// 2. Bind everything 
	glBindVertexArray(m_VAORendererID);
	glBindBuffer(GL_ARRAY_BUFFER, m_VBRendererID);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IBRendererID);

	// 3. Specify vertex layout (for now hard-coded)
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0,
		2,
		GL_FLOAT,
		GL_FALSE,
		sizeof(Vertex),
		reinterpret_cast<const void*>((uint64_t)offsetof(Vertex, Position)));

	glBindVertexArray(0);
}

Mesh::~Mesh()
{
	glDeleteVertexArrays(1, &m_VAORendererID);
	glDeleteBuffers(1, &m_VBRendererID);
	glDeleteBuffers(1, &m_IBRendererID);
}

void Mesh::Bind()
{
	glBindVertexArray(m_VAORendererID);
}
