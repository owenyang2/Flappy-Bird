#include "Game.h"

void Game::initWindow()
{
	this->window = new sf::RenderWindow(sf::VideoMode({ 800, 600 }), "Flappy Bird", sf::Style::Close | sf::Style::Titlebar);
	this->window->setFramerateLimit(60); // locked framerate, no need for deltaTime for now
	this->window->setVerticalSyncEnabled(false);

	// set background
	if (!this->bgTexture.loadFromFile("Textures/background.jpg")) {
		std::cout << "ERROR::GAME::INITTEXTURE::Could not load texture file." << "\n";
	}

	this->bgSprite.setTexture(this->bgTexture);
	this->bgSprite.scale(1.3157f, 1.3157f);
}

void Game::initPlayer()
{
	this->player = new Player(this->window->getSize());
}

void Game::initScore()
{
	this->score = 0;

	if (!this->font.loadFromFile("Fonts/arcade.ttf"))
	{
		std::cout << "ERROR::GAME::INITFONT::Could not load font file." << "\n";
	}

	this->scoreText.setFont(this->font);
	this->scoreText.setString(std::to_string(this->score));
	this->scoreText.setCharacterSize(50);
	this->scoreText.setFillColor(sf::Color::White);
	this->scoreText.setOutlineColor(sf::Color::Black);
	this->scoreText.setOutlineThickness(3.f);
}

void Game::initInfoText()
{
	this->infoText.setFont(this->font);
	this->infoText.setString("GAME OVER!");
	this->infoText.setCharacterSize(75);
	this->infoText.setFillColor(sf::Color::White);
	this->infoText.setOutlineColor(sf::Color::Black);
	this->infoText.setOutlineThickness(3.f);

	this->infoText.setPosition((this->window->getSize().x / 2) - (this->infoText.getGlobalBounds().getSize().x / 2), 200.f);
}

void Game::spawnPipe()
{
	this->pipes.push_back(new Pipe(this->player->getSize(), this->window->getSize()));
}

Game::Game()
{
	this->initWindow();
	this->initPlayer();
	this->initScore();
	this->initInfoText();
}

Game::~Game()
{
	delete this->window;
	delete this->player;

	for (auto& pipe : this->pipes)
	{
		delete pipe;
	}
}

//Functions

void Game::run()
{
	while (this->window->isOpen())
	{
		this->update();
		this->render();
	}
}

void Game::updatePollEvents()
{
	// Poll events
	sf::Event e;
	while (this->window->pollEvent(e))
	{
		if (e.Event::type == sf::Event::Closed)
		{
			this->window->close();
		}
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
	{
		if (!this->gameOver)
		{
			this->player->jump();
		}
		else
		{
			// reset game
			this->resetGame();
		}
	}
}

void Game::updateMovement()
{
	this->player->update();
	if (this->player->hitEdge())
	{
		gameOver = true;
		return;
	}
	
	auto it = this->pipes.begin();
	while (it != this->pipes.end())
	{
		Pipe* pipe = *it;
		
		if (pipe->checkPlayerHit(this->player->getPos(), this->player->getSize()))
		{
			gameOver = true;
			return;
		}

		pipe->update();

		if (pipe->shouldDelete())
		{
			delete pipe;
			it = this->pipes.erase(it);
		}
		else
		{
			it++;
		}
	}

	this->checkPipeSpawn();
}

void Game::updateScore()
{
	for (auto& pipe : this->pipes)
	{
		if (pipe->checkPlayerPass(this->player->getPos(), this->player->getSize()))
		{
			pipe->setCountedScore();
			this->score++;
		}
	}

	// match score to variable
	this->scoreText.setString(std::to_string(this->score));
	
	// recalculate position to keep centered
	this->scoreText.setPosition((this->window->getSize().x / 2) - (this->scoreText.getGlobalBounds().getSize().x / 2), 50.f);
}

void Game::checkPipeSpawn()
{
	if (this->pipeDB >= this->PIPE_SPAWN_DELAY)
	{
		this->pipeDB = 0;
		this->spawnPipe();
	}

	this->pipeDB++;
}

void Game::resetGame()
{
	// reset debounces
	this->currJumpDB = 0;
	this->pipeDB = 0;

	// delete all pipes
	auto it = this->pipes.begin();

	while (it != this->pipes.end())
	{
		Pipe* pipe = *it;
		delete pipe;
		it = this->pipes.erase(it);
	}

	// reset player position
	this->player->resetPos();

	// reset score
	this->score = 0;

	// start game
	this->gameOver = false;
}

void Game::update()
{
	this->updatePollEvents();

	if (this->gameOver)
		return;
	this->updateMovement();
	this->updateScore();
}

void Game::render()
{
	this->window->clear();
	// render background
	this->window->draw(this->bgSprite);
	
	for (auto& pipe : this->pipes)
	{
		pipe->render(*this->window);
	}

	this->player->render(*this->window);
	this->window->draw(this->scoreText);

	if (this->gameOver)
	{
		this->window->draw(this->infoText);
	}

	this->window->display();
}
