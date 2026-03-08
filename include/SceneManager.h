#pragma once
#include "Scene.h"

class SceneManager
{
public:
    static SceneManager& instance();

    void update();
    void draw();
    void changeScene(Scene* newScene);

private:
    SceneManager() = default;
    Scene* currentScene = nullptr;
};
