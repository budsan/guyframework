#pragma once

#include "mesh.h"

namespace Guy {

class Model
{
public:
	Model(Mesh& _mesh);

	Mesh& mesh();
	void draw();

private:
	Mesh& m_mesh;
};

} //namespace Guy



