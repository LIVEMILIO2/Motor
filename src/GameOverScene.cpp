#include "GameOverScene.h"
#include "FlappyBirdScene.h"
#include "MenuScene.h"
#include "SceneManager.h"
#include "raylib.h"
#include "raygui.h"

GameOverScene& GameOverScene::instance() {
    static GameOverScene instance;
    return instance;
}

void GameOverScene::load() {
    isLoaded = true;
    // Configurar cámara para GUI
    cam.zoom = 1.0f;
    cam.offset = { 0, 0 };
    cam.target = { 0, 0 };
}

void GameOverScene::unload() {
    isLoaded = false;
}

void GameOverScene::update() {
    // No necesita lógica especial
}

void GameOverScene::draw() {
    // Dibujar fondo semitransparente (opcional)
    DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), Fade(BLACK, 0.7f));

    // Mostrar título
    DrawText("GAME OVER", 250, 100, 50, RED);

    // Mostrar puntuación final
    DrawText(TextFormat("Score: %d", finalScore), 300, 180, 30, WHITE);

    // Botones
    Rectangle restartBtn = { 300, 250, 200, 40 };
    Rectangle menuBtn = { 300, 310, 200, 40 };

    if (GuiButton(restartBtn, "Play Again")) {
        SceneManager::instance().changeScene(&FlappyBirdScene::instance());
    }

    if (GuiButton(menuBtn, "Main Menu")) {
        SceneManager::instance().changeScene(&MenuScene::instance());
    }
}

void GameOverScene::setFinalScore(int score) {
    finalScore = score;
}