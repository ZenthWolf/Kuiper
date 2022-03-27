/***************************************************************************/
/***               Temper Tech PROJECT KUIPER                            ***/
/*** Copyright for all time                                              ***/
/***                                                                     ***/
/*** Part of the Temper DirectX Framework                                ***/
/***                                                                     ***/
/*** Proprietary Software, do not read.                                  ***/
/*** You cannot use it, look at it, or have it on your computer,         ***/
/*** unless you are a working member of Temper Tech.                     ***/
/*** Temper Tech is definitely not a made up company.                    ***/
/***************************************************************************/

#pragma once

#include <chrono>
#include <random>

#include "Engine/FrameTimer.h"
#include "Engine/Keyboard.h"
#include "Engine/Mouse.h"
#include "Engine/Sound.h"

#include "Graphics/Camera.h"
#include "Graphics/CoordinateTransformer.h"
#include "Graphics/Font.h"
#include "Graphics/Graphics.h"

#include "ColliderManager.h"
#include "Ship.h"
#include "Hexabot.h"
#include "Spawner.h"
#include "Statics.h"



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

	Camera cam;
	CoordinateTransformer ct;
	const Font font = "Images//Fixedsys16x28.bmp";
	FrameTimer ft;
	GameState gameState = GameState::Title;
	Sound mus;
	float vol = 0.3f;

	//Entities
	Ship ship;
	Hexabot* hexa = nullptr;
	std::vector<std::unique_ptr<Asteroid>> belt;

	Spawner spawner;
	ColliderManager collider;

	//Background
	std::vector<Star> scene;

	//Debug
	bool collship = false;
	int jank = 0;

	std::vector<Vec<float>> hexabotSchematic = Shapes::Hexabot();
};
