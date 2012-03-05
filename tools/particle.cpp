#include "particle.h"
#include "particleemitter.h"

#include "math/bbox.h"
#include "graphics/graphics.h"

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

	glColor(current);
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	glVertexPointer  (2, GL_FLOAT, 0, vertcoordsArray);
	glTexCoordPointer(2, GL_FLOAT, 0, texcoordsArray);
	glDrawArrays(GL_QUADS, 0, 4);
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

	int indBase = vertcoords.size();
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
