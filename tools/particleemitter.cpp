#include "particleemitter.h"

#include <GL/gl.h>
#include <stdlib.h>
#include <cmath>
#include <iostream>
#include <time.h>

#include <random>

#include <functional>

#include "graphics/graphics.h"
#include "graphics/texturemanager.h"

ParticleEmitter::ParticleEmitter() : random(time(0))
{
	Restart();
}

ParticleEmitter::ParticleEmitter(const ParticleEmitter &other) : random(time(0))
{
	m_pos = other.m_pos;
	m_dir = other.m_dir;
	m_angle = other.m_angle;

	m_num0 = other.m_num0;
	m_freq = other.m_freq;
	m_grav = other.m_grav;

	m_l0 = other.m_l0;
	m_l1 = other.m_l1;
	m_c0 = other.m_c0;
	m_c1 = other.m_c1;
	m_s0 = other.m_s0;
	m_s1 = other.m_s1;
	m_z0 = other.m_z0;
	m_z1 = other.m_z1;
	m_accumBlending = other.m_accumBlending;
	m_material = other.m_material;

	Restart();
}

void ParticleEmitter::Load()
{
	if (!m_material.empty())
	{
		TextureManager& texman = TextureManager::Instance();
		texman.getTexture(m_material);
	}
}

void ParticleEmitter::Unload()
{
	if (!m_material.empty())
	{
		TextureManager& texman = TextureManager::Instance();
		texman.DeleteTexture(m_material);
	}
}

void ParticleEmitter::Update(float GameTime)
{
	std::list<Particle>::iterator it = m_particles.begin();
	while(it != m_particles.end())
	{
		if (!it->Update(GameTime, *this)) it = m_particles.erase(it);
		else it++;
	}

	m_time += GameTime;
	while (m_time >= m_freq && m_num != 0)
	{
		m_time -= m_freq;
		if (m_num > 0) m_num--;
		newParticle();
		m_particles.back().Update(m_time, *this);
	}
}

void ParticleEmitter::Draw()
{
	if (!m_material.empty())
	{
		glPushAttrib(GL_ALL_ATTRIB_BITS);
		TextureManager& texman = TextureManager::Instance();
		const Texture &tex = texman.getTexture(m_material);
		tex.Bind();
		//tex.setWrap(GL_CLAMP_TO_BORDER);
		//glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, rgba(0,0,0,0).raw());
		glEnable(GL_TEXTURE_2D);
		glEnable(GL_BLEND);
	}

	if (m_accumBlending) glBlendFunc(GL_SRC_ALPHA, GL_ONE);
	else glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	std::vector<vec2f> vertcoords;
	std::vector<vec2f> texcoords;
	std::vector<rgba>  vertcolor;

	vertcoords.reserve(m_particles.size()*4);
	texcoords.reserve(m_particles.size()*4);
	vertcolor.reserve(m_particles.size()*4);

	std::list<Particle>::iterator it = m_particles.begin();
	for (;it != m_particles.end(); it++)
		it->FillDrawArray(*this, vertcoords, texcoords, vertcolor);

	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	glEnableClientState(GL_COLOR_ARRAY);
	glVertexPointer  (2, GL_FLOAT, 0, &vertcoords[0]);
	glTexCoordPointer(2, GL_FLOAT, 0, &texcoords[0]);
	glColorPointer(4,    GL_FLOAT, 0, &vertcolor[0]);
	glDrawArrays(GL_QUADS, 0, m_particles.size()*4);
	glDisableClientState(GL_COLOR_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	glDisableClientState(GL_VERTEX_ARRAY);

	if (!m_material.empty()) glPopAttrib();
}

void ParticleEmitter::Restart()
{
	m_particles.clear();
	m_num = m_num0;
	m_time = 0;
}

void ParticleEmitter::newParticle()
{
	vec2f pos = m_pos, vel;
	//std::uniform_real_distribution<float> dis(0, 1.0f);

	//float weight0 = dis(random);
	//float weight1 = dis(random);
	//float weight2 = dis(random);

	float weight0 = float(random()%(1000))/float(1000);
	float weight1 = float(random()%(1000))/float(1000);
	float weight2 = float(random()%(1000))/float(1000);

	float live  = (m_l0*weight0) + (m_l1*(1-weight0));
	float speed = (m_s0*weight1) + (m_s1*(1-weight1));
	float angle = ((m_angle * weight2)-(m_angle/2))+m_dir;
	vel.x = cos(angle)*speed; vel.y = sin(angle)*speed;

	m_particles.push_back(Particle(pos, vel, live));
}

void ParticleEmitter::setPosition(const vec2f &pos)
{
	m_pos = pos;
}

void ParticleEmitter::setDirection(const vec2f &dir)
{
	m_dir = atan2(dir.y, dir.x);
}

void ParticleEmitter::setDirection(float dir)
{
	m_dir = dir;
}

void ParticleEmitter::setAngle(float angle)
{
	m_angle = angle;
}

void ParticleEmitter::setParticleNumber(int num)
{
	m_num = m_num0 = num;
}

void ParticleEmitter::setFrequency(float freq)
{
	m_freq = freq;
}

void ParticleEmitter::setGravity(const vec2f &grav)
{
	m_grav = grav;
}

void ParticleEmitter::setParticleLive(float l)
{
	m_l0 = m_l1 = l;
}

void ParticleEmitter::setParticleLive(float l0, float l1)
{
	if (l0 < l1)
	{
		m_l0 = l0;
		m_l1 = l1;
	}
	else
	{
		m_l1 = l0;
		m_l0 = l1;
	}
}

void ParticleEmitter::setParticleColor(const rgba &c)
{
	m_c0 = m_c1 = c;
}

void ParticleEmitter::setParticleColor(const rgba &c0, const rgba &c1)
{
	m_c0 = c0; m_c1 = c1;
}

void ParticleEmitter::setParticleSpeed(float s)
{
	m_s0 = m_s1 = s;
}

void ParticleEmitter::setParticleSpeed(float s0, float s1)
{
	if (s0 < s1)
	{
		m_s0 = s0;
		m_s1 = s1;
	}
	else
	{
		m_s1 = s0;
		m_s0 = s1;
	}
}

void ParticleEmitter::setParticleSize(float z)
{
	m_z0 = m_z1 = z;
}

void ParticleEmitter::setParticleSize(float z0, float z1)
{
	m_z0 = z0; m_z1 = z1;
}

void ParticleEmitter::setParticleAccumulativeColor(bool enable)
{
	m_accumBlending = enable;
}

void ParticleEmitter::setParticleMaterial(std::string name)
{
	m_material = name;
}

void ParticleEmitter::write(std::ofstream &file)
{
	write(file, m_pos);
	file.write((char*)&m_dir, sizeof(m_dir));
	file.write((char*)&m_angle, sizeof(m_angle));
	file.write((char*)&m_num0, sizeof(m_num0));
	file.write((char*)&m_freq, sizeof(m_freq));
	write(file, m_grav);
	file.write((char*)&m_l0, sizeof(m_l0));
	file.write((char*)&m_l1, sizeof(m_l1));
	write(file, m_c0);
	write(file, m_c1);
	file.write((char*)&m_s0, sizeof(m_s0));
	file.write((char*)&m_s1, sizeof(m_s1));
	file.write((char*)&m_z0, sizeof(m_z0));
	file.write((char*)&m_z1, sizeof(m_z1));
	file.write((char*)&m_accumBlending, sizeof(m_accumBlending));
	file.write((char*)&m_material[0], m_material.size());
}

void ParticleEmitter::read(std::ifstream &file)
{
	read(file, m_pos);
	file.read((char*)&m_dir, sizeof(m_dir));
	file.read((char*)&m_angle, sizeof(m_angle));
	file.read((char*)&m_num0, sizeof(m_num0));
	file.read((char*)&m_freq, sizeof(m_freq));
	read(file, m_grav);
	file.read((char*)&m_l0, sizeof(m_l0));
	file.read((char*)&m_l1, sizeof(m_l1));
	read(file, m_c0);
	read(file, m_c1);
	file.read((char*)&m_s0, sizeof(m_s0));
	file.read((char*)&m_s1, sizeof(m_s1));
	file.read((char*)&m_z0, sizeof(m_z0));
	file.read((char*)&m_z1, sizeof(m_z1));
	file.read((char*)&m_accumBlending, sizeof(m_accumBlending));

	m_material.clear();
	for(;;)
	{
		char c; file.read(&c ,sizeof(char));
		if (file.eof()) break;
		m_material.append(&c, 1);
	}
}

void ParticleEmitter::read(std::ifstream &file, vec2f &v)
{
	file.read((char*)&v.x,sizeof(float));
	file.read((char*)&v.y,sizeof(float));
}

void ParticleEmitter::write(std::ofstream &file, const vec2f &v)
{
	file.write((char*)&v.x,sizeof(float));
	file.write((char*)&v.y,sizeof(float));
}

void ParticleEmitter::read(std::ifstream &file, rgba &c)
{
	file.read((char*)&c.r,sizeof(float));
	file.read((char*)&c.g,sizeof(float));
	file.read((char*)&c.b,sizeof(float));
	file.read((char*)&c.a,sizeof(float));
}

void ParticleEmitter::write(std::ofstream &file, const rgba &c)
{
	file.write((char*)&c.r,sizeof(float));
	file.write((char*)&c.g,sizeof(float));
	file.write((char*)&c.b,sizeof(float));
	file.write((char*)&c.a,sizeof(float));
}
