#include "Engine.h"
#include "PlayScene.h"
#include "raylib.h"
#include "Log.h"
#include "MenuScene.h"
#include "FlappyBirdScene.h"
Engine::Engine() {
	Log::print("Engine creado");
	
}

void Engine::intialize() {
	setlocale(LC_ALL, "");
	InitWindow(800, 450, "Raylib Basics Engine");
	InitAudioDevice();
	SetTargetFPS(60);
	Log::print("Se inicializo engine");
	sceneManager.changeScene(&MenuScene::instance());  // <--- cambiar a MenuScene
}

void Engine::run() {
	while (!WindowShouldClose()) {
		update();
		draw();
	}
}

void Engine::update() {
	sceneManager.update();
}


void Engine::draw() {

	BeginDrawing();

	ClearBackground(RAYWHITE);

	DrawFPS(30, 10);
	sceneManager.draw();
	EndDrawing();
}


void Engine::shutdown() {
	// Shutdown code here
	CloseWindow();
	Log::print("Se cerro engine");
}