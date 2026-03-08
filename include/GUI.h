#pragma once
#include "EventManager.h"
#include "EventTypes.h"



class GUI 
{
public:
	void draw();
	void onButtonClicked(const ClickButtonEvent &e);
	int score = 0;
	GUI();
};