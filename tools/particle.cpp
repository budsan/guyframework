#include "particle.h"
#include "particleemitter.h"

#include "math/bbox.h"
#include "graphics/graphics.h"

namespace Guy {

Particle::Particle(const vec2f &_pos, const vec2f &_vel, float live):
	m_pos0(_pos), m_pos (_pos), m_vel0(_vel), m_live(live), m_time(0)
{

}

bool Particle::update(float deltaTime, ParticleEmitter &parent)
{
	if(m_live == m_time) return false;

	m_time += deltaTime;
	if (m_live < m_time) m_time = m_live;

	vec2f acc = parent.m_grav;
	m_pos = m_pos0 + m_vel0*m_time + acc*m_time*m_time*0.5f;

	return true;
}

void Particle::draw(ParticleEmitter &parent)
{
	float weight = (m_live-m_time)/m_live;
	rgba current = rgba::interpolate(parent.m_c0, parent.m_c1, weight);
	float size = ((parent.m_z0*weight) + (parent.m_z1*(1.0f-weight)))*0.5f;

	struct { float u, v; } texcoordsArray[4] ={
		{ 0, 0 }, { 1, 0 }, { 1, 1 }, { 0, 1 }
	};

	struct { float x, y; } vertcoordsArray[4] = {
		{m_pos.x-size, m_pos.y-size},
		{m_pos.x+size, m_pos.y-size},
		{m_pos.x+size, m_pos.y+size},
		{m_pos.x-size, m_pos.y+size}
	};

	unsigned short indices[] = { 3, 0, 1, 1, 2, 3 };

	glColor(current);
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	glVertexPointer  (2, GL_FLOAT, 0, vertcoordsArray);
	glTexCoordPointer(2, GL_FLOAT, 0, texcoordsArray);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, indices);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	glDisableClientState(GL_VERTEX_ARRAY);
}

void Particle::fillDrawArray(ParticleEmitter &parent,
	std::vector<vec2f> &vertcoords,
	std::vector<vec2f> &texcoords,
	std::vector<rgba>  &vertcolor,
	std::vector<unsigned int> &indices)
{
	float weight = (m_live-m_time)/m_live;
	rgba current = rgba::interpolate(parent.m_c0, parent.m_c1, weight);
	float size = ((parent.m_z0*weight) + (parent.m_z1*(1.0f-weight)))*0.5f;

	unsigned int indBase = (unsigned int) vertcoords.size();

	texcoords.push_back(math::vec2f(0,0));
	texcoords.push_back(math::vec2f(1,0));
	texcoords.push_back(math::vec2f(1,1));
	texcoords.push_back(math::vec2f(0,1));

	vertcoords.push_back(math::vec2f(m_pos.x-size, m_pos.y-size));
	vertcoords.push_back(math::vec2f(m_pos.x+size, m_pos.y-size));
	vertcoords.push_back(math::vec2f(m_pos.x+size, m_pos.y+size));
	vertcoords.push_back(math::vec2f(m_pos.x-size, m_pos.y+size));

	vertcolor.push_back(current);
	vertcolor.push_back(current);
	vertcolor.push_back(current);
	vertcolor.push_back(current);

	indices.push_back(indBase + 3);
	indices.push_back(indBase + 0);
	indices.push_back(indBase + 1);
	indices.push_back(indBase + 1);
	indices.push_back(indBase + 2);
	indices.push_back(indBase + 3);
}

void Particle::fillDrawArray(ParticleEmitter &parent,
	vec2f *vertcoords,
	vec2f *texcoords,
	rgba *vertcolor,
	unsigned int *indices, unsigned int indBase)
{
	float weight = (m_live-m_time)/m_live;
	rgba current = rgba::interpolate(parent.m_c0, parent.m_c1, weight);
	float size = ((parent.m_z0*weight) + (parent.m_z1*(1.0f-weight)))*0.5f;

	texcoords[0] = math::vec2f(0,0);
	texcoords[1] = math::vec2f(1,0);
	texcoords[2] = math::vec2f(1,1);
	texcoords[3] = math::vec2f(0,1);

	vertcoords[0] = math::vec2f(m_pos.x-size, m_pos.y-size);
	vertcoords[1] = math::vec2f(m_pos.x+size, m_pos.y-size);
	vertcoords[2] = math::vec2f(m_pos.x+size, m_pos.y+size);
	vertcoords[3] = math::vec2f(m_pos.x-size, m_pos.y+size);

	vertcolor[0] = current;
	vertcolor[1] = current;
	vertcolor[2] = current;
	vertcolor[3] = current;

	indices[0] = indBase + 3;
	indices[1] = indBase + 0;
	indices[2] = indBase + 1;
	indices[3] = indBase + 1;
	indices[4] = indBase + 2;
	indices[5] = indBase + 3;
}

} // namespace Guy
