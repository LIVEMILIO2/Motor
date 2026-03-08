#include "GUI.h"
#include "Log.h"
#include "raylib.h"
#include "raygui.h"




GUI::GUI()
{
	EventManager::instance().subscribe(this, &GUI::onButtonClicked);
	GuiSetStyle(DEFAULT, TEXT_SIZE, 30);
}


void GUI::draw()
{
	GuiLabel({ 10, 10, 200, 100 }, ("Score: " + std::to_string(score)).c_str());
}



void GUI::onButtonClicked(const ClickButtonEvent& e)
{
	//Log::print("Play button clicked: " + e.buttonName);
	score++;
}

