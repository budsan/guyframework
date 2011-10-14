#pragma once

class Game;
class GameState {
public:
	GameState();

	virtual void Update(float GameTime) = 0;
	virtual void Draw() = 0;

	virtual void   Load() = 0;
	virtual void Unload() = 0;

	void setGame(Game* parent);
	Game* getGame();

private:
	Game* myParent;
};
