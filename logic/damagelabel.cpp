#include "damagelabel.h"

DamageLabel::DamageLabel(Font* _fontRef, int _fontSize) :
		m_live((int)(UPDATES_PER_SECOND/2.0f)),
		m_threshold(0.5f),
		m_increase(TILE_SIZE)
{
	m_color[0] = 1.0f;
	m_color[1] = 1.0f;
	m_color[2] = 1.0f;
	m_color[3] = 1.0f;

	reset();
	m_fontRef  = _fontRef;
	m_fontSize = _fontSize;
	m_fontHorzAlign = Font::CENTER;
}

void DamageLabel::draw() {
	float transitionPos = m_transition.getPos();
	
	float alphaFactor = (1.0f-transitionPos)*2.0f;
	if (alphaFactor > 1.0f) alphaFactor = 1.0f;

	glColor4f(m_color[0],m_color[1],m_color[2], m_color[3]*alphaFactor);

	float x = m_position.x;
	float y = m_position.y + transitionPos*m_increase;
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();

	glTranslatef(x, y, 0);
	glScalef((TILE_SIZE/m_fontSize)*0.5f,
	         (TILE_SIZE/m_fontSize)*0.5f, 0);

	m_fontRef->setAlignment(Font::TOP, m_fontHorzAlign);
	m_fontRef->Print(0, 0, "%d", m_count);

	glPopMatrix();
}

bool DamageLabel::update() {
	if (m_transition.reached()) return false;
	m_transition.update(1);	
	return true;
}

void DamageLabel::add(int value) {
	m_count += value;
}

bool DamageLabel::thresholdPassed() {
	return m_transition.getPos() > m_threshold;
}

void DamageLabel::reset() {
	m_count = 0;
	m_transition.setTime((float)m_live);
	m_transition.setPos(0);
	m_transition.goPos(1);
}

void DamageLabel::setPosition(math::vec2f pos) {
	m_position = pos;
}

void DamageLabel::setPositionIncrease(float increase) {
	m_increase = increase;
}

void DamageLabel::setAddThreshold(float percentOfLive) {
	m_threshold = percentOfLive;
}

void DamageLabel::setLive(int howManyUpdates) {
	m_live = howManyUpdates;
}
void DamageLabel::setColor(float c[4])
{
	m_color[0] = c[0];
	m_color[1] = c[1];
	m_color[2] = c[2];
	m_color[3] = c[3];
}
void DamageLabel::setHorzAlignment(Font::HorzAlignment type)
{
	m_fontHorzAlign = type;
}
