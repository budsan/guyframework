#include "model.h"

namespace Guy {

Model::Model(Mesh& _mesh) : m_mesh(_mesh)
{
}

Mesh& Model::mesh()
{
	return m_mesh;
}

void Model::draw()
{
	/*
	if (_material)
	{
		Technique* technique = _material->getTechnique();
		GP_ASSERT(technique);
		unsigned int passCount = technique->getPassCount();
		for (unsigned int i = 0; i < passCount; ++i)
		{
			Pass* pass = technique->getPassByIndex(i);
			GP_ASSERT(pass);
			pass->bind();
			GL_ASSERT( glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0) );
			GL_ASSERT( glDrawArrays(_mesh->getPrimitiveType(), 0, _mesh->getVertexCount()) );
			pass->unbind();
		}
	}
	*/
}

} //namespace Guy


