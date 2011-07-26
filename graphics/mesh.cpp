#include "mesh.h"

#include <vector>
#include <map>
#include <list>
#include <cmath>
#include <iostream>
#include <fstream>
#include <sstream>
#include <utility>

/*---------------------------------------------------------------------------*/

void OBJbuilder::ParseLine(std::string& l)
{
	std::stringstream line(l);
	std::string param;

	line >> param;
	if (param.compare("v")==0) //Vertices
	{
		vec3f vert;
		line >> vert.x >> vert.y >> vert.z;
		v.push_back(vert);
	}
	else if (param.compare("vn")==0) //Normales
	{
		vec3f norm;
		line >> norm.x >> norm.y >> norm.z;
		vn.push_back(norm);
	}
	else if (param.compare("vt")==0) //Coordenadas de textura
	{
		vec2f t;
		line >> t.x >> t.y;
		vt.push_back(t);
	}
	else if (param.compare("f")==0) //Caras
	{
		std::vector<int> poly;
		while(line>>param)
		{
			OBJface objf;
			size_t pos;

			// Indice a la coordenada del vertice
			{
				pos = param.find("/");
				std::stringstream param1(param.substr(0, pos));
				param1 >> objf.v;
				if (pos == std::string::npos) goto endparsing;
			}

			// Indice a la coordenada de textura
			{
				param = param.substr(pos+1);
				pos = param.find("/");
				std::stringstream param2(param.substr(0, pos));
				param2 >> objf.vt;
				if (pos == std::string::npos) goto endparsing;
			}

			// Normal del vertice
			{
				param = param.substr(pos+1);
				std::stringstream param3(param.substr(0, pos));
				param3 >> objf.vn;
			}

                        endparsing:

			objf.v -= 1; objf.vt -= 1; objf.vn -= 1;
			std::map<OBJface, int>::iterator it = f2i.find(objf);
			if (it != f2i.end())
			{
				poly.push_back(it->second);
			}
			else
			{
				int index = i2f.size();
				f2i[objf] = index;
				i2f.push_back(objf);
				poly.push_back(index);
			}
		}

		for (unsigned int i = 2; i < poly.size(); i++)
		{
			face triangle;
			triangle.f[0] = poly[0];
			triangle.f[1] = poly[i-1];
			triangle.f[2] = poly[i];

			f.push_back(triangle);
		}
	}
  
}

/*---------------------------------------------------------------------------*/

void Mesh::Clear()
{
	bound.clear();
	verts.clear();
	faces.clear();
	faces_norm.clear();
}

bool Mesh::Load(const char* filename)
{
	std::ifstream file;
	std::string line;

	file.open(filename, std::ifstream::in);
	if(file.fail()) return false;
	Clear();

	char* locale = setlocale(LC_ALL,NULL);
	setlocale(LC_NUMERIC, "C");

	OBJbuilder obj;
	while (std::getline(file, line)) obj.ParseLine(line);

	setlocale(LC_ALL, locale);
	file.close();
	
	for (unsigned int i = 0; i < obj.i2f.size(); i++)
	{
		vert v;
		OBJbuilder::OBJface &objf = obj.i2f[i];
		if (objf.v > 0) v.coord = obj.v[objf.v];
		if (objf.vn> 0) v.norm  = obj.vn[objf.vn];
		if (objf.vt> 0) v.tex   = obj.vt[objf.vt];

		bound.update(v.coord);
		verts.push_back(v);
	}

	for (unsigned int i = 0; i < obj.f.size(); i++)
	{
		tri1i newface; OBJbuilder::face &objt = obj.f[i];
		newface.v[0] = objt.f[0];
		newface.v[1] = objt.f[1];
		newface.v[2] = objt.f[2];
		faces.push_back(newface);
	}

	ComputeNormalsPerFace();
	if (obj.vn.empty()) ComputeNormalsPerVertex();

	return true;
}

void Mesh::Draw() const
{
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_NORMAL_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	glVertexPointer  (3, GL_FLOAT, sizeof(vert), &(verts[0].coord));
	glNormalPointer  (   GL_FLOAT, sizeof(vert), &(verts[0].norm));
	glTexCoordPointer(2, GL_FLOAT, sizeof(vert), &(verts[0].tex));
	glDrawElements(GL_TRIANGLES,faces.size()*3,GL_UNSIGNED_INT, &faces[0]);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	glDisableClientState(GL_NORMAL_ARRAY);
	glDisableClientState(GL_VERTEX_ARRAY);
}

void Mesh::ComputeNormalsPerFace()
{
	faces_norm.clear();
	faces_norm.resize(faces.size());

	for (unsigned int it = 0; it < faces.size(); it++)
	{
		tri1i &f = faces[it]; vec3f norm;
		for(int i = 0; i < 3; i++)
		{
			int j = (i+1)%3;
			norm.x += ((verts[f.v[i]].coord.z+verts[f.v[j]].coord.z)*
				   (verts[f.v[i]].coord.y-verts[f.v[j]].coord.y));
			norm.y += ((verts[f.v[i]].coord.x+verts[f.v[j]].coord.x)*
				   (verts[f.v[i]].coord.z-verts[f.v[j]].coord.z));
			norm.z += ((verts[f.v[i]].coord.y+verts[f.v[j]].coord.y)*
				   (verts[f.v[i]].coord.x-verts[f.v[j]].coord.x));
		}
		norm.normalize();
		faces_norm[it] = norm;
	}
}

void Mesh::ComputeNormalsPerVertex()
{
	std::map<vec3f, std::list<int> > aux;
	for(unsigned  int i = 0; i < verts.size(); i++)
	{
		verts[i].norm = vec3f(0,0,0);
		std::list<int> &l = aux[verts[i].coord];
		l.push_back(i);
	}

	for (unsigned int i = 0; i < faces.size(); i++)
	{
		tri1i &f = faces[i];
		vec3f &norm = faces_norm[i];
		for(unsigned int j = 0; j < 3; j++)
		{
			std::list<int> &l = aux[verts[f.v[j]].coord];
			std::list<int>::iterator it = l.begin();
			for (;it != l.end();it++)
			{
				verts[*it].norm += norm;
			}
		}
	}
	
	for(unsigned int i = 0; i < verts.size(); i++)
	{
		verts[i].norm.normalize();
	}
}
