#pragma once
#include "Scene.h"

class GameOverScene : public Scene {
public:
    static GameOverScene& instance();

    void load() override;
    void unload() override;
    void update() override;
    void draw() override;

    void setFinalScore(int score); // Para mostrar la puntuación alcanzada

private:
    GameOverScene() = default;
    int finalScore = 0;
};