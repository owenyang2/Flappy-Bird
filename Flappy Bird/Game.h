#pragma once
#include "Player.h"
#include "Pipe.h"

class Game
{
private:
	sf::RenderWindow* window;

	const int MAX_DB = 60; // 1 second
	int currJumpDB;

	const int PIPE_SPAWN_DELAY = 60 * 2; // spawn every 2 seconds
	int pipeDB; // spawn pipe when pipeDB == PIPE_SPAWN_DELAY

	Player* player;
	std::vector<Pipe*> pipes;

	bool gameOver = false;

	sf::Texture bgTexture;
	sf::Sprite bgSprite;

	sf::Text scoreText;
	sf::Text infoText;
	sf::Font font;

	int score;

	void initWindow();
	void initPlayer();
	void initScore();
	void initInfoText();

	void spawnPipe();
public:
	Game();
	virtual ~Game();

	void run();

	void updatePollEvents();
	void updateMovement();
	void updateScore();
	void checkPipeSpawn();
	void resetGame();

	void update();
	void render();
};

