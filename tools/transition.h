#pragma once

namespace Guy {

//-TRANSITION-(ABSTRACT-CLASS)-----------------------------------------------//
class Transition {
public:
	 Transition();
	~Transition() {}

	virtual void  setPos(double _pos) {m_pos = m_posToGo = _pos;}
	virtual void   goPos(double _pos) {m_posToGo = _pos;}

	virtual void update(double _deltaTime) = 0;

  virtual double pos() {return m_pos;}
	virtual bool reached() {return m_posToGo == m_pos;}

protected:

	double m_pos;
	double m_posToGo;
};

//-TRANSITION-CONSTANT-VELOCITY----------------------------------------------//
class TransitionVelConst : public Transition {
public:
	 TransitionVelConst() : m_vel(1.0) {}
	~TransitionVelConst() {}

	void update(double time);

	void setVel(double _secsTotal) {m_vel = _secsTotal;}

protected:

	double m_vel;
};

//-TRANSITION-LINEAR---------------------------------------------------------//
class TransitionLinear : public Transition {
public:
	 TransitionLinear() : m_time(1.0), m_percent(1.0) {}
	~TransitionLinear() {}

	void setPos(double _pos);
	void  goPos(double _pos);
	void update(double _deltaTime);

	void setTime(double _timeToReach) { m_time = _timeToReach;}

protected:

	double m_time;
	double m_percent;
	double m_initPos;
};

//-TRANSITION-SINUSOIDAL-----------------------------------------------------//
class TransitionSinus : public Transition {
public:
	 TransitionSinus() : m_sinPos(0.0), m_vel(0.5) {}
	~TransitionSinus() {}

	void setPos(double _pos);
	void  goPos(double _pos);
	void update(double _deltaTime);

	void setTime(double _secsTotal) { m_vel = _secsTotal;}

protected:

	double m_sinPos;
	double m_vel;
	double m_initPos;
};

//-TRANSITION-SINUSOIDAL-FADE-IN---------------------------------------------//

class TransitionSinusFadeIn : public Transition {
public:
	 TransitionSinusFadeIn() : m_sinPos(0.0), m_vel(0.5) {}
	~TransitionSinusFadeIn() {}

	void setPos(double _pos);
	void  goPos(double _pos);
	void update(double _deltaTime);

	void setTime(double _secsTotal) { m_vel = _secsTotal;}

protected:

	double m_sinPos;
	double m_vel;
	double m_initPos;
};

//-TRANSITION-SINUSOIDAL-FADE-OUT--------------------------------------------//
class TransitionSinusFadeOut : public Transition {
public:
	 TransitionSinusFadeOut() : m_sinPos(0.0), m_vel(0.5) {}
	~TransitionSinusFadeOut() {}

	void setPos(double _pos);
	void  goPos(double _pos);
	void update(double _deltaTime);

	void setTime(double _secsTotal) { m_vel = _secsTotal;}

protected:

	double m_sinPos;
	double m_vel;
	double m_initPos;
};

//-TRANSITION-BOUNCE---------------------------------------------------------//
class TransitionBounce : public Transition {
public:
	 TransitionBounce() : m_vel(0), m_acc(5), m_velLostPercent(0.4) {}
	~TransitionBounce();

	void update(double _deltaTime);
	bool reached() {return (m_posToGo == m_pos) && m_vel == 0;}

	void setAcc(double _unitsPerSec) {m_acc = _unitsPerSec;}
	void setVelLost(double _percentVel) {m_velLostPercent = _percentVel;}

protected:

	double   m_vel;
	double   m_acc;
	double   m_velLostPercent;
};

//-TRANSITION-INERTIAL-------------------------------------------------------//
class TransitionInertial : public Transition {
public:
	 TransitionInertial() : m_vel(0), m_acc(1.0), m_velMax(3.0) {}
	~TransitionInertial();

	void update(double _deltaTime);
	void update(int   _deltaTime) {update(double(_deltaTime)/1000.0);}
	bool reached() {return (m_posToGo == m_pos) && m_vel == 0;}

	void setAcc(double _unitsPerSec) {m_acc = _unitsPerSec;}
	void setVelMax(double _velMax) {m_velMax = _velMax;}

protected:

	double m_vel;
	double m_acc;
	double m_velMax;
};


} // namespace Guy
