#include "MenuScene.h"
#include "PlayScene.h"
#include "SceneManager.h"
#include "raylib.h"
#include "raygui.h"
#include "Log.h"

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
    DrawText("Menu Scene!", 190, 200, 20, LIGHTGRAY);
	Rectangle buttonRect = { 200, 100, 200, 100 };
    if (GuiButton(buttonRect, "#191#Show Message")) {
		Log::print("Button Clicked in MenuScene");
		eventManager.emit(ClickButtonEvent{ "Show button"});
    }

	DrawTexture(*tex, 300, 250, WHITE);
	DrawTextEx(*font, "Menú Principal", { 10,10 }, 32, 2, DARKBLUE);
	gui.draw();
}


