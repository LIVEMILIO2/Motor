#pragma once
#include "Scene.h"
#include "Bird.h"
#include "Pipe.h"
#include "HUD.h"
#include <vector>
#include <memory>

class FlappyBirdScene : public Scene {
public:
    static FlappyBirdScene& instance();

    void load() override;
    void unload() override;
    void update() override;
    void draw() override;

private:
    FlappyBirdScene() = default;

    std::shared_ptr<Bird> bird;
    std::vector<PipePair> pipes;
    std::unique_ptr<HUD> hud;

    float pipeSpawnTimer = 0.0f;
    float pipeSpawnInterval = 2.0f;
    float pipeSpeed = 3.0f;
    float pipeGap = 4.0f;
    float pipeWidth = 1.5f;
    float groundY = 8.0f;
    bool gameOver = false;
    bool gameStarted = false;
    int score = 0;

    void spawnPipe();
    void resetGame();
};