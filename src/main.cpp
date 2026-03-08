// RaylibBasics.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#include <iostream>
#include "raylib.h"
#define RAYGUI_IMPLEMENTATION
#include "raygui.h"
//#include "imgui.h"
//#include "rlImGui.h"
#include "Engine.h"

int main()
{

	Engine engine;
	engine.intialize();
	engine.run();
	engine.shutdown();


	//   // Initialization
	//       //--------------------------------------------------------------------------------------
	//   const int screenWidth = 800;
	//   const int screenHeight = 450;
	//   Vector2 pos = { (float)screenWidth / 2, (float)screenHeight / 2 };
	//   float speed = 5.0f;
	//   
	//   
	//   

	//   InitWindow(screenWidth, screenHeight, "raylib [core] example - basic window");
	//   rlImGuiSetup(true);
	//   Image image = LoadImage("assets/textures/apple.png");
	//   Texture2D texture = LoadTextureFromImage(image);
	//   SetTargetFPS(60);               // Set our game to run at 60 frames-per-second
	//   //--------------------------------------------------------------------------------------
	   //InitAudioDevice();          // Initialize audio device
	   //Sound snd = LoadSound("assets/audio/e_sound.wav");
	//   // Main game loop
	//   while (!WindowShouldClose())    // Detect window close button or ESC key
	//   {
	//       // Update
	//       //----------------------------------------------------------------------------------
	//       //
	//       //----------------------------------------------------------------------------------

	//       // Draw
	//       //----------------------------------------------------------------------------------
	//       if (IsKeyDown(KEY_RIGHT)) pos.x += speed;
	   //	if (IsKeyDown(KEY_LEFT)) pos.x -= speed;
	   //	if (IsKeyDown(KEY_UP)) pos.y -= speed;
	//       if (IsKeyDown(KEY_DOWN)) pos.y += speed;
	   //	if (IsKeyPressed(KEY_SPACE)) PlaySound(snd);
	//       
	//       BeginDrawing();

	//       ClearBackground(RAYWHITE);

	//       DrawText("Congrats! You created your first window!", 190, 200, 20, LIGHTGRAY);
	   //	DrawCircleV(pos, 25, GREEN);
	//       DrawTexture(texture, pos.x, pos.y, WHITE);

	   //	rlImGuiBegin();
	   //	ImGui::Begin("Hello, ImGui!");
	   //	ImGui::Text("This is some useful text.");
	   //	ImGui::End();

	   //	rlImGuiEnd();

	//       EndDrawing();
	//       //----------------------------------------------------------------------------------
	//   }

	   //UnloadSound(snd);
	//   rlImGuiShutdown();
	//   // De-Initialization
	//   //--------------------------------------------------------------------------------------
	//   CloseWindow();        // Close window and OpenGL context
	   //--------------------------------------------------------------------------------------

	return 0;
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
