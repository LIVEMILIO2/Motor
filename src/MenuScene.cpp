#include "MenuScene.h"
#include "PlayScene.h"
#include "SceneManager.h"
#include "raylib.h"
#include "raygui.h"
#include "Log.h"
#include "FlappyBirdScene.h"
#include "InstructionsScene.h"

MenuScene::MenuScene()
{
   
}

MenuScene& MenuScene::instance()
{
    static MenuScene instance;
    return instance;
}

void MenuScene::load()
{
	
	if (isLoaded) {
		PlayMusicStream(*bgMusic);
		return;
	}
	isLoaded = true;
	tex = resourceManager.getTexture("apple.png");
	bgMusic = resourceManager.getMusic("menu.ogg");
	PlayMusicStream(*bgMusic);
	font = resourceManager.getFont("Roboto-Regular.ttf");

	
}

void MenuScene::unload()
{
	if(isLoaded)
		StopMusicStream(*bgMusic);
}

void MenuScene::update()
{
	UpdateMusicStream(*bgMusic);
    if (IsKeyPressed(KEY_SPACE)) {
        SceneManager::instance().changeScene(&PlayScene::instance());
	}

}

void MenuScene::draw()
{
    // Salir del modo cámara para dibujar GUI en coordenadas de pantalla
    EndMode2D();

    // Dibujar botones y otros elementos de GUI
    DrawText("Menu Scene!", 190, 200, 20, LIGHTGRAY);
    Rectangle playBtn = { 200, 100, 200, 40 };
    Rectangle instrBtn = { 200, 160, 200, 40 };
    Rectangle json = { 200, 220, 200, 40 };
    if (GuiButton(playBtn, "Play Flappy Bird")) {
        SceneManager::instance().changeScene(&FlappyBirdScene::instance());
    }
    if(GuiButton(json, "Prueba Json")) {
        SceneManager::instance().changeScene(&PlayScene::instance());
	}
    if (GuiButton(instrBtn, "Instructions")) {
        SceneManager::instance().changeScene(&InstructionsScene::instance());
    }

    // Volver a entrar al modo cámara si después quieres dibujar algo en coordenadas del mundo
    BeginMode2D(cam);

    // El resto de dibujo (texturas, etc.) que dependan de la cámara
    DrawTexture(*tex, 300, 250, WHITE);
    DrawTextEx(*font, "Menú Principal", { 10,10 }, 32, 2, DARKBLUE);
    gui.draw();
}


