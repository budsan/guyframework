#ifndef _DAMAGE_LABEL_INCLUDED_
#define _DAMAGE_LABEL_INCLUDED_

#include "graphics/graphics.h"
#include "graphics/font.h"
#include "base/defines.h"
#include "tools/transition.h"
#include "math/vec2.h"

class DamageLabel {
public:
	DamageLabel(Font* _fontRef, int _fontSize);

	void draw();
	bool update(); //Returns false when it isn't alive.
	void add(int value);
	bool thresholdPassed(); //Returns true when it pass the threshold
	void reset();

	void setPosition(math::vec2f pos);
	void setPositionIncrease(float increase);
	void setAddThreshold(float percentOfLive);
	void setLive(int howManyUpdates);
	void setColor(float c[4]);
	void setHorzAlignment(Font::HorzAlignment type);

private:


	int m_count;
	int m_live;
	float m_threshold;
	float m_increase;
	math::vec2f m_position;
	TransitionLinear m_transition;
	Font* m_fontRef;
	int   m_fontSize;
	Font::HorzAlignment m_fontHorzAlign;
	float m_color[4];
};
#endif
