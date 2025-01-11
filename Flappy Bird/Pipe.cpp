#include "Pipe.h"

using namespace std;

void Pipe::initTexture()
{
	if (!this->texture.loadFromFile("Textures/Pipe.png")) {
		std::cout << "ERROR::PIPE::INITTEXTURE::Could not load texture file." << "\n";
	}
}

void Pipe::initSprite()
{
	float remainingSpaceY = this->windowSize.y - (this->plrSize.y * 3.5f); // space remaining for pipes to use

	std::random_device dev;
	std::mt19937 rng(dev());
	std::uniform_int_distribution<std::mt19937::result_type> dist(1, (int) std::floor(remainingSpaceY)); // distribution in range [1, remainingSpaceY]

	float topSizeYPixels = dist(rng); // generate random size of top pipe
	float bottomSizeYPixels = remainingSpaceY - topSizeYPixels; // use rest for bottom pipe

	// initialize pipe textures
	this->topPart.setTexture(this->texture);
	this->bottomPart.setTexture(this->texture);

	// calclulate scale from px
	float origSizeY = this->texture.getSize().y; // get vertical size in px of orig image

	this->bottomPart.scale(WIDTH_SCALE, topSizeYPixels / origSizeY);
	this->bottomPart.setRotation(180.f); // anchor point now bottom right
	// set origin of bottom to bottom-left for easy off-screen detection
	// doesn't consider the scale transformations, so remove the scaling factor before setting origin
	this->bottomPart.setOrigin(this->bottomPart.getGlobalBounds().getSize().x / WIDTH_SCALE, 0);

	this->topPart.scale(WIDTH_SCALE, bottomSizeYPixels / origSizeY);

	// setup pipe start position off screen on right
	this->bottomPart.setPosition(this->windowSize.x, this->windowSize.y);
	this->topPart.setPosition(this->windowSize.x, 0.f);
}

void Pipe::move()
{
	this->topPart.move(-this->MOVE_SPEED, 0);
	this->bottomPart.move(-this->MOVE_SPEED, 0);
}

Pipe::Pipe(sf::Vector2f plrSize, sf::Vector2u windowSize)
{
	this->plrSize = plrSize;
	this->windowSize = windowSize;

	this->initTexture();
	this->initSprite();
}

Pipe::~Pipe()
{
}

void Pipe::update()
{
	// move pipes to the right
	this->move();
}

void Pipe::render(sf::RenderTarget& target)
{
	target.draw(this->topPart);
	target.draw(this->bottomPart);
}

bool Pipe::checkPlayerHit(sf::Vector2f plrPos, sf::Vector2f plrSize)
{
	sf::Rect<float> plrRect(plrPos, plrSize);

	// check if hit pipes
	return this->topPart.getGlobalBounds().intersects(plrRect) || this->bottomPart.getGlobalBounds().intersects(plrRect);
}

bool Pipe::checkPlayerPass(sf::Vector2f plrPos, sf::Vector2f plrSize)
{
	if (this->countedScore) // already counted
		return false;

	float plrTopLeftX = plrPos.x;
	float pipeTopRightX = this->topPart.getGlobalBounds().getPosition().x + this->topPart.getGlobalBounds().getSize().x;

	return pipeTopRightX < plrTopLeftX;
}

void Pipe::setCountedScore()
{
	this->countedScore = true;
}

bool Pipe::shouldDelete()
{
	sf::Rect<float> topPartBounds = this->topPart.getGlobalBounds();
	if (topPartBounds.getPosition().x + topPartBounds.getSize().x < 0)
	{
		return true;
	}

	return false;
}
