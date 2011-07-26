#ifndef MESH_H
#define MESH_H

#include <string>
#include <vector>
#include <map>

#include "graphics.h"

#include "math/vec3.h"
#include "math/vec2.h"
#include "math/tri.h"
#include "math/bbox.h"

using math::vec2f;
using math::vec3f;
using math::tri1i;
using math::bbox3f;

class Mesh
{
	public:

	struct vert {
		vec3f coord;
		vec3f norm;
		vec2f tex;
	};

	std::vector<vert> verts;
	std::vector<tri1i> faces;
	std::vector<vec3f> faces_norm;
        bbox3f bound;

	void Clear();
	bool Load (const char* filename);
	void Draw () const;

	void ComputeNormalsPerFace();
	void ComputeNormalsPerVertex();
};

struct OBJbuilder
{

	struct face {int f[3];};
	struct OBJface
	{
		int v;
		int vt;
		int vn;

		OBJface() : v(0), vt(0), vn(0) {}
		bool operator<(const OBJface &other) const
		{
			if ( v == other.v)
			{
				if( vt == other.vt) return vn < other.vn;
				return vt < other.vt;
			}
			return v < other.v;
		}
	};

	std::vector<vec3f> v;
	std::vector<vec3f> vn;
	std::vector<vec2f> vt;

	std::vector<OBJface> i2f;
	std::map<OBJface, int> f2i;
	std::vector<face> f;

	void ParseLine(std::string& l);
};

#endif
