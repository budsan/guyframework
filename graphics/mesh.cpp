#include "mesh.h"
#include "debug.h"

namespace Guy {

Mesh::Mesh(const Vertex::Format& vertexFormat, unsigned int vertexCount, bool dynamic)
	: m_vertexFormat(vertexFormat), m_vertexCount(vertexCount), m_vertexBuffer(0), m_primitiveType(TRIANGLES),
	  m_dynamic(dynamic)
{
	GLuint vbo;
	GL_ASSERT( glGenBuffers(1, &vbo) );
	if (GL_LAST_ERROR())
	{
		GUY_ERROR("Failed to create VBO for mesh with OpenGL error %d.", GL_LAST_ERROR());
		return;
	}

	GL_ASSERT( glBindBuffer(GL_ARRAY_BUFFER, vbo) );
	if (GL_LAST_ERROR())
	{
		GUY_ERROR("Failed to bind VBO for mesh with OpenGL error %d.", GL_LAST_ERROR());
		glDeleteBuffers(1, &vbo);
		return;
	}

	GL_CHECK( glBufferData(GL_ARRAY_BUFFER, vertexFormat.vertexSize() * vertexCount, NULL, dynamic ? GL_DYNAMIC_DRAW : GL_STATIC_DRAW) );
	if (GL_LAST_ERROR())
	{
		GUY_ERROR("Failed to load VBO with vertex data with OpenGL error %d.", GL_LAST_ERROR());
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glDeleteBuffers(1, &vbo);
		return;
	}
}

const Vertex::Format& Mesh::vertexFormat() const
{
	return m_vertexFormat;
}

unsigned int Mesh::vertexCount() const
{
	return m_vertexCount;
}

unsigned int Mesh::vertexSize() const
{
	return m_vertexFormat.vertexSize();
}

bool Mesh::isDynamic() const
{
	return m_dynamic;
}

GLuint Mesh::vertexBuffer() const
{
	return m_vertexBuffer;
}

Mesh::PrimitiveType Mesh::primitiveType() const
{
	return m_primitiveType;
}

void Mesh::setPrimitiveType(Mesh::PrimitiveType type)
{
	m_primitiveType = type;
}

void Mesh::setVertexData(float* vertexData, unsigned int vertexStart, unsigned int vertexCount)
{
	GL_ASSERT( glBindBuffer(GL_ARRAY_BUFFER, m_vertexBuffer) );

	if (vertexStart == 0 && vertexCount == 0)
	{
		GL_ASSERT( glBufferData(GL_ARRAY_BUFFER, m_vertexFormat.vertexSize() * m_vertexCount, vertexData, m_dynamic ? GL_DYNAMIC_DRAW : GL_STATIC_DRAW) );
	}
	else
	{
		if (vertexCount == 0)
		{
			vertexCount = m_vertexCount - vertexStart;
		}

		GL_ASSERT( glBufferSubData(GL_ARRAY_BUFFER, vertexStart * m_vertexFormat.vertexSize(), vertexCount * m_vertexFormat.vertexSize(), vertexData) );
	}
}

} //namespace Guy
