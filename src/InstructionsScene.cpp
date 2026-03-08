#include "InstructionsScene.h"
#include "MenuScene.h"
#include "SceneManager.h"
#include "raylib.h"

InstructionsScene& InstructionsScene::instance() {
    static InstructionsScene instance;
    return instance;
}

void InstructionsScene::load() {
    isLoaded = true;

    // Configurar cámara para que no afecte a la GUI
    cam.zoom = 1.0f;
    cam.offset = { 0, 0 };
    cam.target = { 0, 0 };
}

void InstructionsScene::unload() {
    isLoaded = false;
}

void InstructionsScene::update() {
    if (IsKeyPressed(KEY_SPACE) || IsKeyPressed(KEY_ESCAPE)) {
        SceneManager::instance().changeScene(&MenuScene::instance());
    }
}

void InstructionsScene::draw() {
    DrawText("Instrucciones:", 200, 100, 30, DARKGREEN);
    DrawText("- Presiona ESPACIO o clic para volar", 150, 160, 20, DARKGRAY);
    DrawText("- Evita las tuberias y el suelo", 150, 200, 20, DARKGRAY);
    DrawText("- Cada tuberia superada suma 1 punto", 150, 240, 20, DARKGRAY);
    DrawText("Presiona Space para volver al menu", 180, 300, 20, GRAY);
}