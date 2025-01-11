#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <iostream>

class Player
{
private:
	sf::Sprite sprite;
	sf::Texture texture;

	sf::Vector2u windowSize;

	float movementSpeed;
	
	bool jumping;
	int airTime; // current amount of time since jump

	const int JUMP_TIME = 15; // time it takes to fully jump, 1 second
	const int JUMP_RESET = 0; // time it takes to jump again, debounce

	sf::Vector2f startPos;

	void initTexture();
	void initSprite();

public:
	Player(sf::Vector2u windowSize);
	virtual ~Player();

	void move(const float dirX, const float dirY);
	void jump();

	void update();
	void render(sf::RenderTarget& target);

	sf::Vector2f getSize();
	sf::Vector2f getPos();

	void resetPos();

	bool hitEdge();
};

