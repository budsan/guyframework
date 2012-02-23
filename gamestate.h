#pragma once

class Game;
class GameState {
public:
	GameState();

	virtual void update(float deltaTime) = 0;
	virtual void draw() = 0;

	virtual void   load() = 0;
	virtual void unload() = 0;

	void setGame(Game *parent);
	Game *getGame();

private:
	Game *m_parent;
};
