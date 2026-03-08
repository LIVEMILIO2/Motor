#pragma once
#include "Scene.h"
#include "EventManager.h"
#include <string>
#include "EventTypes.h"
#include "GUI.h"
#include <memory>
 


class MenuScene : public Scene
{
public:
	
    static MenuScene& instance();
    

    void load() override;
    void unload() override;
    void update() override;
    void draw() override;


private:
    MenuScene();
    GUI gui;
	std::shared_ptr<Texture2D> tex;
	std::shared_ptr<Music> bgMusic;
    std::shared_ptr<Font> font;
};