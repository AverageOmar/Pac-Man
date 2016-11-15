#pragma once

// If Windows and not in Debug, this will run without a console window
// You can use this to output information when debugging using cout or cerr
#ifdef WIN32 
	#ifndef _DEBUG
		#pragma comment(linker, "/SUBSYSTEM:windows /ENTRY:mainCRTStartup")
	#endif
#endif

// Just need to include main header file
#include "S2D/S2D.h"

// Reduces the amount of typing by including all classes in S2D namespace
using namespace S2D;

#define MUNCHIECOUNT 20

#define GHOSTCOUNT 4

struct Player
{
	Vector2* position;
	Rect* sourceRect;
	Texture2D* texture;
	int direction;
	int frame;
	int currentFrameTime;
	float speedMultiplier;
	bool dead;
	int score;
	int lives;
	int power;
};

struct Enemy
{
	Vector2* position;
	Rect* munchieRect;
	Texture2D* texture;
	int frame;
	int frameCount;
	int currentFrameTime;
};

struct GhostEnemy
{
	Rect* posRect;
	Rect* sourceRect;
	Texture2D* texture;
	bool AIchanged;
	int changeAIin;
	int changeAI;
	int direction;
	int directionX;
	int directionY;
	float speed;
	float speedMultiplier;
};

struct Cherry
{
	Vector2* position;
	Rect* cherryRect;
	Texture2D* texture;
	int frame;
	int frameCount;
	int cherrySpawn;
	int cherrySpawnIn;
	bool cherrySpawned;
};

// Declares the Pacman class which inherits from the Game class.
// This allows us to overload the Game class methods to help us
// load content, draw and update our game.
class Pacman : public Game
{
private:

	void Input(int elapsedTime, Input::KeyboardState* State, Input::MouseState * mouseState); //input definition

	void CheckViewportCollision();

	void UpdatePacman(int elapsedTime);
	void UpdateMunchie(int elapsedTime);
	void UpdateCherry(int elapsedTime);
	void IsGamePaused();
	void isGameStart();
	bool checkCollision(int x1, int y1, int w1, int h1, int x2, int y2, int w2, int h2);
	void munchieCollision();
	void ghostCollision();
	void cherryCollision();

	void UpdateGhosts(int elapsedTime);
	void CheckGhostCollision();

	int _munchieCurrentFrameTime;
	const int _cMunchieFrameTime;
	int munchiesLeft;

	int _cherryCurrentFrameTime;
	const int _cCherryFrameTime;

	bool _pKeyDown;
	bool _rKeyDown;

	Player* _pacman;
	Enemy* _munchie[MUNCHIECOUNT];
	Cherry* _cherry;
	GhostEnemy* _ghosts[GHOSTCOUNT];

	const float _cPacmanSpeed;
	float pacmanSpeed;
	const int _cPacmanFrameTime;

	Texture2D* _menuBackground;
	Rect* _menuRectangle;
	Vector2* _menuStringPosition;
	bool _paused;

	Texture2D* _background;
	Rect* _backgroundRect;
	Vector2* _backgroundPosition;

	Texture2D* _menu;
	Rect* _menuRect;
	Vector2* _menuPosition;
	
	bool _gKeyDown;
	bool _menuStart;
	int go;

	SoundEffect* _pop;
	SoundEffect* _death;
	SoundEffect* _cherryPickup;

	// Position for String
	Vector2* _stringPosition;

public:
	/// <summary> Constructs the Pacman class. </summary>
	Pacman(int argc, char* argv[]);

	/// <summary> Destroys any data associated with Pacman class. </summary>
	virtual ~Pacman();

	/// <summary> All content should be loaded in this method. </summary>
	void virtual LoadContent();

	/// <summary> Called every frame - update game logic here. </summary>
	void virtual Update(int elapsedTime);

	/// <summary> Called every frame - draw game here. </summary>
	void virtual Draw(int elapsedTime);
};
