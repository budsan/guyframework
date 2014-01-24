#include "mesh.h"
#include "debug.h"

namespace Guy {

Mesh::Mesh(const Vertex::Format& _vertexFormat, std::size_t _vertexCount, bool _dynamic)
	: m_vertexFormat(_vertexFormat), m_vertexCount(_vertexCount), m_vertexBuffer(0), m_primitiveType(TRIANGLES),
	  m_dynamic(_dynamic)
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

	GL_CHECK( glBufferData(GL_ARRAY_BUFFER, _vertexFormat.vertexSize() * _vertexCount, NULL, _dynamic ? GL_DYNAMIC_DRAW : GL_STATIC_DRAW) );
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

std::size_t Mesh::vertexCount() const
{
	return m_vertexCount;
}

std::size_t Mesh::vertexSize() const
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

void Mesh::setPrimitiveType(Mesh::PrimitiveType _type)
{
	m_primitiveType = _type;
}

void Mesh::setVertexData(float* _vertexData, std::size_t _vertexStart, std::size_t _vertexCount)
{
	GL_ASSERT( glBindBuffer(GL_ARRAY_BUFFER, m_vertexBuffer) );

	if (_vertexStart == 0 && _vertexCount == 0)
	{
		GL_ASSERT( glBufferData(GL_ARRAY_BUFFER, m_vertexFormat.vertexSize() * m_vertexCount, _vertexData, m_dynamic ? GL_DYNAMIC_DRAW : GL_STATIC_DRAW) );
	}
	else
	{
		if (_vertexCount == 0)
		{
			_vertexCount = m_vertexCount - _vertexStart;
		}

		GL_ASSERT( glBufferSubData(GL_ARRAY_BUFFER, _vertexStart * m_vertexFormat.vertexSize(), _vertexCount * m_vertexFormat.vertexSize(), _vertexData) );
	}
}

} //namespace Guy
