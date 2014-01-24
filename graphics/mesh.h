#pragma once

#include "graphics.h"
#include "vertexattribute.h"

#include "math/bbox.h"

namespace Guy
{

class Mesh
{
public:
	enum PrimitiveType
	{
		TRIANGLES = GL_TRIANGLES,
		TRIANGLE_STRIP = GL_TRIANGLE_STRIP,
		LINES = GL_LINES,
		LINE_STRIP = GL_LINE_STRIP,
		POINTS = GL_POINTS
	};

	Mesh(const Vertex::Format& _vertexFormat, std::size_t _vertexCount, bool _dynamic = false);

	const Vertex::Format& vertexFormat() const;
	std::size_t vertexCount() const;
	std::size_t vertexSize() const;
	bool isDynamic() const;
	GLuint vertexBuffer() const;

	PrimitiveType primitiveType() const;
	void setPrimitiveType(Mesh::PrimitiveType _type);
	void setVertexData(float* _vertexData, std::size_t _vertexStart, std::size_t _vertexCount);

protected:

	const Vertex::Format m_vertexFormat;
	std::size_t m_vertexCount;
	GLuint m_vertexBuffer;
	PrimitiveType m_primitiveType;
	bool m_dynamic;
};

} //namespace Guy
