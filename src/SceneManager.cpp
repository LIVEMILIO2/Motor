#include "SceneManager.h"

SceneManager& SceneManager::instance()
{
    static SceneManager instance;
    return instance;
}

void SceneManager::update()
{
    if (currentScene)
        currentScene->updateScene();
}

void SceneManager::draw()
{
    if (currentScene)
        currentScene->drawScene();
}

void SceneManager::changeScene(Scene* newScene)
{
    if (currentScene)
        currentScene->unload();

    currentScene = newScene;

    if (currentScene)
        currentScene->load();
}
