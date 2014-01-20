#include "transition.h"

#define _USE_MATH_DEFINES
#include <math.h>
#include <iostream>

#define M_PI_2F ( (double) M_PI_2 )

namespace Guy {

//-TRANSITION----------------------------------------------------------------//
Transition::Transition() {
	m_pos = 0;
	m_posToGo = 0;
}

//-TRANSITION-CONSTANT-VELOCITY----------------------------------------------//
void TransitionVelConst::update(double time)
{
	double distLeft = m_pos - m_posToGo;
	if ( distLeft < 0 )
	{
		double distToMove = -m_vel * time;
		if ( distLeft >= distToMove )
			distLeft  = 0;
		else	distLeft -= distToMove;
	}
	else if ( distLeft > 0 )
	{
		double distToMove =  m_vel * time;
		if ( distLeft <= distToMove )
			distLeft  = 0;
		else	distLeft -= distToMove;
	}

	m_pos = distLeft + m_posToGo;
}

//-TRANSITION-LINEAL---------------------------------------------------------//
void TransitionLinear::setPos(double _pos)
{
	Transition::setPos(_pos);
	m_percent = 1.0;
}

void TransitionLinear::goPos(double _pos)
{
	m_percent = 0.0;
	m_initPos = m_pos;
	Transition::goPos(_pos);
}

void TransitionLinear::update(double _deltaTime)
{
	if(m_percent < 1.0)
	{
		m_percent += _deltaTime/m_time;
		if (m_percent >= 1.0)
		{
			m_percent = 1.0;
			m_pos = m_posToGo;
		}
		else m_pos =
			m_initPos * (1.0 - m_percent) +
			m_posToGo * (      m_percent);
	}
}

//-TRANSITION-SINUSOIDAL-----------------------------------------------------//
void TransitionSinus::setPos(double _pos)
{
	Transition::setPos(_pos);
	m_sinPos = (double)M_PI;
}

void TransitionSinus::goPos(double _pos)
{
	m_sinPos = 0.0;
	m_initPos = m_pos;
	Transition::goPos(_pos);
}

void TransitionSinus::update(double _deltaTime)
{
	if(m_sinPos < M_PI)
	{
		double percent = (cosf(m_sinPos)+1.0)/2.0;
		m_pos =
			m_initPos * (     percent) +
			m_posToGo * (1.0-percent);
		m_sinPos += (double)M_PI/m_vel*_deltaTime;

		if (m_sinPos > M_PI) {
			m_sinPos = (double)M_PI;
			m_pos = m_posToGo;
		}
	}
}

//-TRANSITION-SINUSOIDAL-FADE-IN---------------------------------------------//
void TransitionSinusFadeIn::setPos(double _pos)
{
	Transition::setPos(_pos);
	m_sinPos = M_PI_2F;
}

void TransitionSinusFadeIn::goPos(double _pos)
{
	m_sinPos = 0.0;
	m_initPos = m_pos;
	Transition::goPos(_pos);
}

void TransitionSinusFadeIn::update(double _deltaTime)
{
	if(m_sinPos < M_PI_2F)
	{
		double percent = cosf(m_sinPos);
		m_pos =
			m_initPos * (    percent) +
			m_posToGo * (1.0-percent);
		m_sinPos += M_PI_2F/m_vel*_deltaTime;

		if(m_sinPos > M_PI_2F) {
			m_sinPos = M_PI_2F;
			m_pos = m_posToGo;
		}
	}
}

//-TRANSITION-SINUSOIDAL-FADE-OUT--------------------------------------------//
void TransitionSinusFadeOut::setPos(double _pos)
{
	Transition::setPos(_pos);
	m_sinPos = (double) M_PI_2;
}

void TransitionSinusFadeOut::goPos(double _pos)
{
	m_sinPos = 0.0;
	m_initPos = m_pos;

	Transition::goPos(_pos);
}

void TransitionSinusFadeOut::update(double _deltaTime)
{
	if(m_sinPos < M_PI_2F)
	{
		double percent = sinf(m_sinPos);
		m_pos =
			m_initPos * (1.0-percent) +
			m_posToGo * (     percent);
		m_sinPos += M_PI_2F/m_vel*_deltaTime;

		if(m_sinPos >  M_PI_2F) {
			m_sinPos = M_PI_2F;
			m_pos = m_posToGo;
		}
	}
}

//-TRANSITION-BOUNCE---------------------------------------------------------//
void TransitionBounce::update(double time)
{
	double facing;

	     if (m_pos > m_posToGo) facing = -1;
	else if (m_pos < m_posToGo) facing = +1;
	else {
		     if (m_vel > 0) facing = -1;
		else if (m_vel < 0) facing = +1;
		else facing = 0;
	}

	//PHYSICS FORMULA
	double timeLeft = (-m_vel + facing * (double) sqrt(m_vel*m_vel -
			(4.0 * (facing  * m_acc * 0.5) *
			(m_pos - m_posToGo))))/(facing  * m_acc);
	double nextTime = (timeLeft - time);

	if ( nextTime < 0 )
	{
			update(timeLeft);

			m_pos = m_posToGo;
			m_vel = m_vel*m_velLostPercent;

			if(fabs(m_vel) < 0.001) m_vel = 0;

			update(double(fabs(nextTime)));
	}
	else {
		m_pos += (m_vel*time)+(time*time*facing*m_acc/ 2);
		m_vel       =  m_vel + facing*time*m_acc;
	}
}

//-TRANSITION-INERTIAL-------------------------------------------------------//
void TransitionInertial::update(double time)
{
	double acc = 0;
	     if (m_pos < m_posToGo) acc = +1 * m_acc;
	else if (m_pos > m_posToGo) acc = -1 * m_acc;
	else return;

	register double TimeToStop = (double)fabs(m_vel)/m_acc;
	register double DistToStop =
		((double) fabs(m_vel)*TimeToStop) + (-m_acc*TimeToStop*TimeToStop/2);
	register double distLeft   = (double) fabs(m_pos-m_posToGo);

	if((m_vel*acc) > 0 &&  distLeft <= DistToStop) acc *= -1.0;

	register double vel = m_vel + (time * acc);
	if (vel * m_vel < 0) vel = 0;
	if (vel >= m_velMax) vel = (vel/(double) fabs(vel)) * m_velMax;
	m_vel = vel;

	m_pos = m_pos + (m_vel*time) + (time*time*acc*0.5);
	if(fabs(m_pos-m_posToGo) <= (m_vel*time*0.5)) {
		m_pos = m_posToGo; m_vel = 0;
	}
}

} // namespace Guy
