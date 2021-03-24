/***************************************************************************/
/***               Temper Tech DirectX Framework V 1.0.0                 ***/
/*** Game.h                                                              ***/
/*** Copyright for all time (if your law disagrees, it can suck it)      ***/
/***                                                                     ***/
/*** Part of the Temper DirectX Framework                                ***/
/***                                                                     ***/
/*** Proprietary Software, do not read.                                  ***/
/*** You cannot use it, look at it, or have it on your computer,         ***/
/*** unless you are a working member of Temper Tech                      ***/
/***                                                                     ***/
/*** If you need help, go fuck yourself.                                 ***/
/***************************************************************************/
#pragma once

#include "Keyboard.h"
#include "Mouse.h"
#include "Graphics.h"
#include "FrameTimer.h"
#include <chrono>
#include "Sound.h"
#include <random>
#include "Font.h"
#include "CoordinateTransformer.h"
#include "Camera.h"

#include "Spawner.h"
#include "Statics.h"
#include "Ship.h"


class Game
{
public:
	Game(class MainWindow& wnd);
	Game(const Game&) = delete;
	Game& operator=(const Game&) = delete;
	void Play();
private:
	void ComposeFrame();
	void UpdateModel(float dt);
	/********************************/
	/*  User Functions              */
	/********************************/
	enum class GameState
	{
		Title,
		Play
	};
private:
	MainWindow& wnd;
	Graphics gfx;
	/********************************/
	/*  User Variables              */
	/********************************/
	std::mt19937 rng;

	GameState gameState = GameState::Title;
	FrameTimer ft;
	Font font = "Images//Fixedsys16x28.bmp";
	CoordinateTransformer ct;
	Camera cam;

	Sound mus;

	Ship ship;
	std::vector<Star> scene;
	std::vector<std::unique_ptr<Asteroid>> belt;
	Spawner spawner;

	bool collship = false;
};
