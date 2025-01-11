#include "Player.h"

void Player::initTexture()
{
	// Load texture from file
	if (!this->texture.loadFromFile("Textures/flappybird.png")) {
		std::cout << "ERROR::PLAYER::INITTEXTURE::Could not load texture file." << "\n";
	}
}

void Player::initSprite()
{
	// Set texture to sprite
	this->sprite.setTexture(this->texture);

	this->sprite.scale(0.1f, 0.1f);
}

Player::Player(sf::Vector2u windowSize)
{
	this->windowSize = windowSize;

	this->movementSpeed = 100.f;
	this->jumping = false;
	this->airTime = 0;

	this->initTexture();
	this->initSprite();

	this->startPos = sf::Vector2f(50.f, (windowSize.y / 2.f) - (this->getSize().y / 2));
	this->sprite.setPosition(this->startPos);	
}

Player::~Player()
{
}

// Functions

void Player::move(const float dirX, const float dirY)
{
	this->sprite.move(this->movementSpeed * dirX, this->movementSpeed * dirY);
}

void Player::jump()
{
	if (this->jumping)
	{
		this->airTime = 0;
		return;
	}

	this->jumping = true;
}

void Player::update()
{
	// check if jumping
	if (this->jumping)
	{
		if (this->airTime <= this->JUMP_TIME)
		{
			this->move(0.f, -0.03);
		}
		else if (this->airTime >= this->JUMP_TIME + this->JUMP_RESET) 
		{
			this->jumping = false;
			this->airTime = 0;
		}

		this->airTime++;

		return;
	}

	this->move(0.f, 0.05f); // Gravity
}

void Player::render(sf::RenderTarget& target)
{
	target.draw(this->sprite);
}

sf::Vector2f Player::getSize()
{
	return this->sprite.getGlobalBounds().getSize();
}

sf::Vector2f Player::getPos()
{
	return this->sprite.getGlobalBounds().getPosition();
}

void Player::resetPos()
{
	this->sprite.setPosition(this->startPos);
}

bool Player::hitEdge()
{
	float yPos = this->getPos().y;
	float ySize = this->getSize().y;

	// check if hit top
	if (yPos < 0 || yPos + ySize > this->windowSize.y)
	{
		return true;
	}

	return false;
}
