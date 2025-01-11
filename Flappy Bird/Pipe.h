#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <iostream>
#include <random>

class Pipe
{
private:
	sf::Sprite topPart;
	sf::Sprite bottomPart;
	sf::Texture texture;

	sf::Vector2f plrSize;
	sf::Vector2u windowSize;

	const float WIDTH_SCALE = 0.25f;
	const float MOVE_SPEED = 5.f;

	bool countedScore = false;

	void initTexture();
	void initSprite();

	void move();
public:
	Pipe(sf::Vector2f plrSize, sf::Vector2u windowSize);
	virtual ~Pipe();

	void update();
	void render(sf::RenderTarget& target);

	bool checkPlayerHit(sf::Vector2f plrPos, sf::Vector2f plrSize);
	bool checkPlayerPass(sf::Vector2f plrPos, sf::Vector2f plrSize);

	void setCountedScore();

	bool shouldDelete();
};

