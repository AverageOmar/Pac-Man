#include "Pacman.h"

#include <sstream>

#include <time.h>

Pacman::Pacman(int argc, char* argv[]) : Game(argc, argv), _cPacmanSpeed(0.2f), _cPacmanFrameTime(50), _cMunchieFrameTime(500), _munchieCurrentFrameTime(0), _cCherryFrameTime(500), _cherryCurrentFrameTime(0), munchiesLeft(0)
{
	srand(time(NULL));

	_pacman = new Player();
	_pacman->speedMultiplier = 1.0f;
	_pacman->currentFrameTime = 0;
	_pacman->direction = 0;
	_pacman->frame = 0;
	_pacman->dead = false;
	_pacman->score = 0;
	_pacman->lives = 3;
	_pacman->power = 0;

	for (int i = 0; i < GHOSTCOUNT; i++)
	{
		_ghosts[i] = new GhostEnemy();
		_ghosts[i]->directionX = 0;
		_ghosts[i]->directionY = 0;
		_ghosts[i]->speed = 0.025f;
		_ghosts[i]->speedMultiplier = 1;
		_ghosts[i]->changeAI = 20000;
		_ghosts[i]->changeAIin = 0;
		_ghosts[i]->AIchanged = false;
	}

	for (int i = 0; i < MUNCHIECOUNT; i++)
	{
		_munchie[i] = new Enemy();
		_munchie[i]->frame = 0;
		_munchie[i]->frameCount = 0;
		_munchie[i]->currentFrameTime = rand() % 500 + 50;
	}

	_cherry = new Cherry();
	_cherry->frame = 0;
	_cherry->frameCount = 0;
	_cherry->cherrySpawn = rand() % 100000;
	_cherry->cherrySpawnIn = 0;
	_cherry->cherrySpawned = false;

	_paused = false;
	_pKeyDown = false;
	_gKeyDown = true;
	//_rKeyDown = true;
	_menuStart = false;
	go = 0;

	//Initialise important Game aspects
	Audio::Initialise();
	Graphics::Initialise(argc, argv, this, 1024, 768, false, 25, 25, "Pacman", 60);
	Input::Initialise();

	// Start the Game Loop - This calls Update and Draw in game loop
	Graphics::StartGameLoop();
}

Pacman::~Pacman()
{
	delete _pacman->texture;
	delete _pacman->sourceRect;

	for (int i = 0; i < MUNCHIECOUNT; i++)
	{
		delete _munchie[i]->texture;
		delete _munchie[i]->munchieRect;
	}

	for (int i = 0; i < GHOSTCOUNT; i++)
	{
		delete _ghosts[i]->texture;
		delete _ghosts[i]->sourceRect;
	}

	delete _cherry->texture;
	delete _cherry->cherryRect;
}

void Pacman::Input(int elapsedTime, Input::KeyboardState* State, Input::MouseState* mouseState)
{
	pacmanSpeed = _cPacmanSpeed * elapsedTime * _pacman->speedMultiplier;

	if (State->IsKeyDown(Input::Keys::D))
	{
		_pacman->direction = 0;
	}
	else if (State->IsKeyDown(Input::Keys::W))
	{
		_pacman->direction = 3;
	}
	else if (State->IsKeyDown(Input::Keys::S))
	{
		_pacman->direction = 1;
	}
	else if (State->IsKeyDown(Input::Keys::A))
	{
		_pacman->direction = 2;
	}

	if (_pacman->direction == 0)
	{
		_pacman->position->X += pacmanSpeed;
	}
	if (_pacman->direction == 3)
	{
		_pacman->position->Y += -pacmanSpeed;
	}
	if (_pacman->direction == 1)
	{
		_pacman->position->Y += pacmanSpeed;
	}
	if (_pacman->direction == 2)
	{
		_pacman->position->X += -pacmanSpeed;
	}

	//randomises munchie location
	/*if (State->IsKeyUp(Input::Keys::R))
	{
		_rKeyDown = false;
	}
	if (State->IsKeyDown(Input::Keys::R) && (!_rKeyDown))
	{
		for (int i = 0; i < MUNCHIECOUNT; i++)
		{
			int rndWidth = rand() % 1024;
			int rndHeight = rand() % 768;

			_munchie[i]->position->X = rndWidth;
			_munchie[i]->position->Y = rndHeight;
			_rKeyDown = true;
		}
	}*/

	//move munchie with mouse
	/*if (mouseState->LeftButton == Input::ButtonState::PRESSED)
	{
		_munchie->position->X = mouseState->X;
		_munchie->position->Y = mouseState->Y;
	}*/

	/*if ((State->IsKeyDown(Input::Keys::LEFTSHIFT)) || (State->IsKeyDown(Input::Keys::RIGHTSHIFT)))
	{
		_pacman->speedMultiplier = 2.0f;
	}
	else
	{
		_pacman->speedMultiplier = 1.0f;
	}*/
}

void Pacman::CheckViewportCollision()
{
	if (_pacman->position->X + _pacman->sourceRect->Width > 1024)
	{
		_pacman->position->X = -32 + _pacman->sourceRect->Width;
	}

	if (_pacman->position->Y + _pacman->sourceRect->Height > 768)
	{
		_pacman->position->Y = -32 + _pacman->sourceRect->Height;
	}

	if (_pacman->position->X + _pacman->sourceRect->Width < 32)
	{
		_pacman->position->X = 1024 - _pacman->sourceRect->Width;
	}

	if (_pacman->position->Y + _pacman->sourceRect->Height < 32)
	{
		_pacman->position->Y = 768 - _pacman->sourceRect->Height;
	}
}

bool Pacman::checkCollision(int x1, int y1, int w1, int h1, int x2, int y2, int w2, int h2)
{
	if ((y1 + h1 < y2) || (y1 > y2 + h2) || (x1 + w1 < x2) || (x1 > x2 + w2))
	{
		return false;
	}
	else
	{
		return true;
	}
}

void Pacman::UpdatePacman(int elapsedTime)
{
	_pacman->sourceRect->X = _pacman->sourceRect->Width * _pacman->frame;

	_pacman->currentFrameTime += elapsedTime;

	//pacmans facing direction
	_pacman->sourceRect->Y = _pacman->sourceRect->Height * _pacman->direction;

	//animate pacman
	if (_pacman->currentFrameTime > _cPacmanFrameTime)
	{
		_pacman->frame++;
		if (_pacman->frame >= 7)
		{
			_pacman->frame = 0;
		}
		_pacman->currentFrameTime = 0;
	}

}

void Pacman::UpdateGhosts(int elapstedTime)
{
	if (_pacman->direction < 5)
	{
		for (int i = 0; i < GHOSTCOUNT; i++)
		{
			_ghosts[i]->sourceRect->X = _ghosts[i]->sourceRect->Width * _ghosts[i]->direction;

			_ghosts[i]->changeAIin += elapstedTime;

			if (_ghosts[i]->changeAIin >= _ghosts[i]->changeAI)
			{
				_ghosts[i]->AIchanged = true;
			}
			else
			{
				_ghosts[i]->AIchanged = false;
			}

			if (_ghosts[i]->AIchanged)
			{
				//MOVING IN X AXIS
				if (_ghosts[i]->directionX == 0)
				{
					_ghosts[i]->posRect->X += _ghosts[i]->speed * elapstedTime * (i + _ghosts[i]->speedMultiplier);
				}
				else if (_ghosts[i]->directionX == 1)
				{
					_ghosts[i]->posRect->X -= _ghosts[i]->speed * elapstedTime * (i + _ghosts[i]->speedMultiplier);
				}
				else if (_ghosts[i]->directionX == 2)
				{
					_ghosts[i]->posRect->X = _ghosts[i]->speed * elapstedTime * (i + _ghosts[i]->speedMultiplier);
				}
				//MOVING IN Y AXIS
				if (_ghosts[i]->directionY == 0)
				{
					_ghosts[i]->posRect->Y += _ghosts[i]->speed * elapstedTime * (i + _ghosts[i]->speedMultiplier);
				}
				else if (_ghosts[i]->directionY == 1)
				{
					_ghosts[i]->posRect->Y -= _ghosts[i]->speed * elapstedTime * (i + _ghosts[i]->speedMultiplier);
				}
				else if (_ghosts[i]->directionY == 2)
				{
					_ghosts[i]->posRect->Y = _ghosts[i]->speed * elapstedTime * (i + _ghosts[i]->speedMultiplier);
				}

				//CHOOSING X AXIS DIRECTION
				if (_ghosts[i]->posRect->X < _pacman->position->X)
				{
					_ghosts[i]->directionX = 0;
					_ghosts[i]->direction = 0;
				}
				else if (_ghosts[i]->posRect->X > _pacman->position->X)
				{
					_ghosts[i]->directionX = 1;
					_ghosts[i]->direction = 2;
				}
				else if (_ghosts[i]->posRect->X == _pacman->position->X)
				{
					_ghosts[i]->directionX = 2;
				}
				//CHOOSING Y AXIS DIRECTION
				if (_ghosts[i]->posRect->Y < _pacman->position->Y)
				{
					_ghosts[i]->directionY = 0;
					_ghosts[i]->direction = 3;
				}
				else if (_ghosts[i]->posRect->Y > _pacman->position->Y)
				{
					_ghosts[i]->directionY = 1;
					_ghosts[i]->direction = 1;
				}
				else if (_ghosts[i]->posRect->Y == _pacman->position->Y)
				{
					_ghosts[i]->directionY = 2;
				}
			}
			else
			{
				//moving x axis
				if (_ghosts[i]->directionX == 0)
				{
					_ghosts[i]->posRect->X += _ghosts[i]->speed * elapstedTime * (i + 1);
				}
				else if (_ghosts[i]->directionX == 1)
				{
					_ghosts[i]->posRect->X -= _ghosts[i]->speed * elapstedTime * (i + 1);
				}

				//moving y axis
				if (_ghosts[i]->directionY == 0)
				{
					_ghosts[i]->posRect->Y += _ghosts[i]->speed * elapstedTime * (i + 1);
				}
				else if (_ghosts[i]->directionY == 1)
				{
					_ghosts[i]->posRect->Y -= _ghosts[i]->speed * elapstedTime * (i + 1);
				}

				//choosing x axis direction
				if (_ghosts[i]->posRect->X < 0)
				{
					_ghosts[i]->directionX = 0;
				}
				else if (_ghosts[i]->posRect->X > Graphics::GetViewportWidth())
				{
					_ghosts[i]->directionX = 1;
				}

				//choosing y axis direction
				if (_ghosts[i]->posRect->Y < 0)
				{
					_ghosts[i]->directionY = 0;
				}
				else if (_ghosts[i]->posRect->Y >= Graphics::GetViewportHeight())
				{
					_ghosts[i]->directionY = 1;
				}
			}
		}
	}
}

void Pacman::ghostCollision()
{
	for (int i = 0; i < GHOSTCOUNT; i++)
	{
		if (checkCollision(_pacman->position->X, _pacman->position->Y, _pacman->sourceRect->Width, _pacman->sourceRect->Height, _ghosts[i]->posRect->X, _ghosts[i]->posRect->Y, _ghosts[i]->sourceRect->Width, _ghosts[i]->sourceRect->Height))
		{
			Audio::Play(_death);
			_pacman->direction = 5;
			if (_pacman->lives > -1)
			{
				_pacman->lives--;
				_pacman->position = new Vector2(350.0f, 350.0f);
			}
			if (_pacman->lives == -1)
			{
				for (int i = 0; i < GHOSTCOUNT; i++)
				{
					_ghosts[i]->changeAIin = 0;
					go = 0;
					_menuStart = false;
					_pacman->lives = 3;
					_pacman->score = 0;
					_pacman->speedMultiplier = 1;
				}
			}
		}
	}
}

void Pacman::UpdateMunchie(int elapsedTime)
{
	for (int i = 0; i < MUNCHIECOUNT; i++)
	{
		//munchie move towards pacman
		if ((_munchie[i]->position->X < _pacman->position->X) && (_munchie[i]->position->Y < _pacman->position->Y))
		{
			if (((_pacman->position->X) - (_munchie[i]->position->X) < 50) && ((_pacman->position->Y) - (_munchie[i]->position->Y) < 50))
			{
				_munchie[i]->position->X += elapsedTime * 0.05f;
				_munchie[i]->position->Y += elapsedTime * 0.05f;
			}
		}
		if ((_munchie[i]->position->X > _pacman->position->X) && (_munchie[i]->position->Y > _pacman->position->Y))
		{
			if (((_munchie[i]->position->X) - (_pacman->position->X) < 50) && ((_munchie[i]->position->Y) - (_pacman->position->Y) < 50))
			{
				_munchie[i]->position->X -= elapsedTime * 0.05f;
				_munchie[i]->position->Y -= elapsedTime * 0.05f;
			}
		}

		//munchie move towards pacman
		if ((_munchie[i]->position->Y < _pacman->position->Y) && (_munchie[i]->position->X > _pacman->position->X))
		{
			if (((_pacman->position->Y) - (_munchie[i]->position->Y) < 50) && ((_munchie[i]->position->X) - (_pacman->position->X) < 50))
			{
				_munchie[i]->position->Y += elapsedTime * 0.05f;
				_munchie[i]->position->X -= elapsedTime * 0.05f;
			}
		}
		if ((_munchie[i]->position->Y > _pacman->position->Y) && (_munchie[i]->position->X < _pacman->position->X))
		{
			if (((_munchie[i]->position->Y) - (_pacman->position->Y) < 50) && ((_pacman->position->X) - (_munchie[i]->position->X) < 50))
			{
				_munchie[i]->position->Y -= elapsedTime * 0.05f;
				_munchie[i]->position->X += elapsedTime * 0.05f;
			}
		}

		_munchie[i]->currentFrameTime += elapsedTime;
		if (_munchie[i]->currentFrameTime > _cMunchieFrameTime)
		{
			_munchie[i]->frameCount++;
			if (_munchie[i]->frameCount >= 2)
			{
				_munchie[i]->frameCount = 0;
			}
			_munchie[i]->currentFrameTime = 0;
			_munchie[i]->munchieRect->X = _munchie[i]->munchieRect->Width * _munchie[i]->frameCount;
		}
	}
}

void Pacman::munchieCollision()
{
	for (int i = 0; i < MUNCHIECOUNT; i++)
	{
		if (checkCollision(_pacman->position->X, _pacman->position->Y, _pacman->sourceRect->Width, _pacman->sourceRect->Height, _munchie[i]->position->X, _munchie[i]->position->Y, _munchie[i]->munchieRect->Width, _munchie[i]->munchieRect->Height))
		{
			Audio::Play(_pop);
			_pacman->score = _pacman->score + 100;
			_munchie[i]->position = new Vector2(1000.0f, 1000.0f);
			munchiesLeft++;
			if (munchiesLeft == MUNCHIECOUNT)
			{
				for (int h = 0; h < MUNCHIECOUNT; h++)
				{
					_munchie[h]->position = new Vector2(rand() % (Graphics::GetViewportWidth() - 50) + 50, rand() % (Graphics::GetViewportHeight() - 50) + 50);
				}
				munchiesLeft = 0;
				for (int s = 0; s < GHOSTCOUNT; s++)
				{
					_ghosts[s]->speedMultiplier = _ghosts[s]->speedMultiplier + 0.5f;
				}
			}
		}
	}
}

void Pacman::UpdateCherry(int elapsedTime)
{
	_cherry->cherrySpawnIn += elapsedTime;
	
	if (_cherry->cherrySpawnIn >= _cherry->cherrySpawn)
	{
		_cherry->cherrySpawned = true;
		_cherry->cherrySpawnIn = 0;
	}

	_cherryCurrentFrameTime += elapsedTime;
	
	if (_cherryCurrentFrameTime > _cCherryFrameTime)
	{
		_cherry->frameCount++;
		if (_cherry->frameCount >= 4)
		{
			_cherry->frameCount = 0;
		}
		_cherryCurrentFrameTime = 0;
	}

	
	_cherry->cherryRect->X = _cherry->cherryRect->Width * _cherry->frameCount;
}

void Pacman::cherryCollision()
{
	if ((checkCollision(_pacman->position->X, _pacman->position->Y, _pacman->sourceRect->Width, _pacman->sourceRect->Height, _cherry->position->X, _cherry->position->Y, _cherry->cherryRect->Width, _cherry->cherryRect->Height)) && (_cherry->cherrySpawned))
	{
		Audio::Play(_cherryPickup);
		_cherry->position = new Vector2(1500.0f, 1500.0f);
		_cherry->cherrySpawned = false;
		_cherry->cherrySpawnIn = 0;
		_cherry->cherrySpawn = rand() % 100000;
		_cherry->position = new Vector2(512.0f, 500.0f);

		_pacman->power = rand() % 3 + 1;
		if (_pacman->power == 1)
		{
			_pacman->speedMultiplier = _pacman->speedMultiplier + 0.25f;
		}
		else if (_pacman->power == 2)
		{
			_pacman->lives++;
		}
		else if (_pacman->power == 3)
		{
			_pacman->score = _pacman->score + 1000;
		}
	}
}

void Pacman::IsGamePaused()
{
	Input::KeyboardState* keyboardState = Input::Keyboard::GetState();

	if (keyboardState->IsKeyDown(Input::Keys::P) && !_pKeyDown)
	{
		_pKeyDown = true;
		_paused = !_paused;
	}

	if (keyboardState->IsKeyUp(Input::Keys::P))
	{
		_pKeyDown = false;
	}
}

void Pacman::isGameStart()
{
	Input::KeyboardState* keyboardState = Input::Keyboard::GetState();

	if (keyboardState->IsKeyDown(Input::Keys::G) && !_gKeyDown && (go == 0))
	{
		_gKeyDown = true;
		_menuStart = !_menuStart;
		go++;
	}

	if (keyboardState->IsKeyUp(Input::Keys::G))
	{
		_gKeyDown = false;
	}
}

void Pacman::LoadContent()
{
	//load background
	_background = new Texture2D();
	_background->Load("Textures/PacmanBackground.png", false);
	_backgroundPosition = new Vector2(0.0f, 0.0f);
	_backgroundRect = new Rect(0.0f, 0.0f, 1024, 768);

	//load menu
	_menu = new Texture2D();
	_menu->Load("Textures/StartMenu.png", false);
	_menuPosition = new Vector2(0.0f, 0.0f);
	_menuRect = new Rect(0.0f, 0.0f, 1024, 768);

	// Load Pacman
	_pacman->texture = new Texture2D();
	_pacman->texture->Load("Textures/Pacman.png", false);
	_pacman->position = new Vector2(350.0f, 350.0f);
	_pacman->sourceRect = new Rect(0.0f, 0.0f, 32, 32);

	//load ghost
	for (int i = 0; i < GHOSTCOUNT; i++)
	{
		_ghosts[i]->texture = new Texture2D();
	}

	_ghosts[0]->texture->Load("Textures/GhostBlue.png", true);
	_ghosts[1]->texture->Load("Textures/GhostGreen.png", true);
	_ghosts[2]->texture->Load("Textures/GhostYellow.png", true);
	_ghosts[3]->texture->Load("Textures/GhostRed.png", true);

	for (int i = 0; i < GHOSTCOUNT;i++)
	{
		_ghosts[i]->posRect = new Rect(rand() % Graphics::GetViewportWidth(), rand() % Graphics::GetViewportHeight(), 20, 20);
		_ghosts[i]->sourceRect = new Rect(0.0f, 0.0f, 20, 20);
	}

	//Load Cherry
	_cherry->texture = new Texture2D();
	_cherry->texture->Load("Textures/Cherry.png", false);
	_cherry->position = new Vector2(512.0f, 500.0f);
	_cherry->cherryRect = new Rect(0.0f, 0.0f, 24, 24);

	// Load Munchie
	Texture2D* munchieTex = new Texture2D();
	munchieTex->Load("Textures/Munchie1.png", true);

	for (int i = 0; i < MUNCHIECOUNT; i++)
	{
		_munchie[i]->texture = munchieTex;
	}

	for (int i = 0; i < MUNCHIECOUNT; i++)
	{
		_munchie[i]->texture->Load("Textures/Munchie.png", false);
		_munchie[i]->position = new Vector2(rand() % (Graphics::GetViewportWidth() - 50) + 50, rand() % (Graphics::GetViewportHeight() - 50) + 50);
		_munchie[i]->munchieRect = new Rect(0.0f, 0.0f, 12, 12);
	}

	//load pause menu background
	_menuBackground = new Texture2D();
	_menuBackground->Load("Textures/Transparency.png", false);
	_menuRectangle = new Rect(0.0f, 0.0f, Graphics::GetViewportWidth(), Graphics::GetViewportHeight() / 2.0f);
	_menuStringPosition = new Vector2(Graphics::GetViewportWidth() / 2.0f, Graphics::GetViewportHeight() / 2.0f);

	//load sound
	_pop = new SoundEffect();
	_pop->Load("Sound/_pop.wav");

	_death = new SoundEffect();
	_death->Load("Sound/_death.wav");

	_cherryPickup = new SoundEffect();
	_cherryPickup->Load("Sound/_cherryPickup.wav");

	// Set string position
	_stringPosition = new Vector2(10.0f, 25.0f);
}

void Pacman::Update(int elapsedTime)
{
	Input::KeyboardState* keyboardState = Input::Keyboard::GetState();

	Input::MouseState* mouseState = Input::Mouse::GetState();

	isGameStart();
	
	if (_menuStart)
	{
		IsGamePaused();

		if (!_paused)
		{
			Input(elapsedTime, keyboardState, mouseState);

			UpdatePacman(elapsedTime);

			UpdateMunchie(elapsedTime);

			CheckViewportCollision();

			UpdateCherry(elapsedTime);

			UpdateGhosts(elapsedTime);

			munchieCollision();

			ghostCollision();

			cherryCollision();
		}
	}
	
}

void Pacman::Draw(int elapsedTime)
{
	// Allows us to easily create a string
	std::stringstream stream;
	//stream << "Pacman X: " << _pacman->position->X << " Y: " << _pacman->position->Y;;
	stream << "Score: " << _pacman->score << ",  Lives: " << _pacman->lives << ", Speed: " << _pacman->speedMultiplier;

	SpriteBatch::BeginDraw(); // Starts Drawing

	//draw background
	SpriteBatch::Draw(_background, _backgroundPosition, _backgroundRect);

	//draw pacman
	if (!_pacman->dead)
	{
		SpriteBatch::Draw(_pacman->texture, _pacman->position, _pacman->sourceRect); // Draws Pacman
	}

	//draw ghosts
	for (int i = 0; i < GHOSTCOUNT; i++)
	{
		SpriteBatch::Draw(_ghosts[i]->texture, _ghosts[i]->posRect, _ghosts[i]->sourceRect);
	}

	//draw munchie
	for (int i = 0; i < MUNCHIECOUNT; i++)
	{
		SpriteBatch::Draw(_munchie[i]->texture, _munchie[i]->position, _munchie[i]->munchieRect);
	}

	//draw cherry
	if (_cherry->cherrySpawned)
	{
		SpriteBatch::Draw(_cherry->texture, _cherry->position, _cherry->cherryRect);
	}

	if (!_menuStart)
	{
		SpriteBatch::Draw(_menu, _menuPosition, _menuRect);
	}

	if (_paused)
	{
		std::stringstream menuStream;
		menuStream << "PAUSED!";
		SpriteBatch::Draw(_menuBackground, _menuRectangle, nullptr);
		SpriteBatch::DrawString(menuStream.str().c_str(), _menuStringPosition, Color::Red);
	}
	
	SpriteBatch::DrawString(stream.str().c_str(), _stringPosition, Color::Green);
	SpriteBatch::EndDraw(); // Ends Drawing
}
