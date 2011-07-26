#ifndef _EVENT_H_
#define _EVENT_H_

enum EventType { 
	EVENT_CREATE,
	EVENT_ATTACK_UNIT,
	EVENT_ATTACK_WALL,
	EVENT_REMOVE_UNIT
};

struct Event {
	Event(int _type) : type(_type) {}
	int type;
};

struct EventCreate : public Event {
	EventCreate(): Event(EVENT_CREATE) {}
	int team;
	int unitType;
	float position;
};

struct EventAttackUnit : public Event {
	EventAttackUnit(): Event(EVENT_ATTACK_UNIT) {}
	int attack;
	int origin;
	int target;
};

struct EventAttackWall : public Event {
	EventAttackWall(): Event(EVENT_ATTACK_WALL) {}
	int attack;
	float positionY;
	int targetWall;
};

struct EventRemoveUnit : public Event {
	EventRemoveUnit(): Event(EVENT_REMOVE_UNIT) {}
	int target;
};

#endif
