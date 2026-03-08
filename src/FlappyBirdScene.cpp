#include "FlappyBirdScene.h"
#include "MenuScene.h"
#include "SceneManager.h"
#include "raylib.h"
#include "Log.h"
#include "GameOverScene.h"

FlappyBirdScene& FlappyBirdScene::instance() {
    static FlappyBirdScene instance;
    return instance;
}

void FlappyBirdScene::load() {
    if (isLoaded) return;

    hud = std::make_unique<HUD>();

    // ---- Configurar cámara como en PlayScene ----
    cam.zoom = 20.0f;
    cam.offset = { GetScreenWidth() / 4.0f, GetScreenHeight() / 2.0f };
    cam.target = { 0.0f, 0.0f };  // Podemos ajustarlo después para seguir al pájaro

    // ---- Crear el pájaro (Bird) ----
    BodyData birdData;
    birdData.pos = { -5.0f, 0.0f };
    birdData.isDynamic = true;
    birdData.name = "Bird";
    birdData.tag = "Bird";
    birdData.radius = 1.0f;
    birdData.enableCollisions = true;
    bird = physics.makeCircle<Bird>(birdData);
    addEntity(bird);

    // ---- Suelo estático (PBox) ----
    BodyData groundData;
    groundData.pos = { 0.0f, groundY };
    groundData.isDynamic = false;
    groundData.name = "Ground";
    groundData.tag = "Ground";
    groundData.size = { 100.0f, 2.0f };
    groundData.enableCollisions = true;
    addEntity(physics.makeBox(groundData));  // PBox normal

    isLoaded = true;
    gameStarted = false;
    gameOver = false;
    score = 0;
}

void FlappyBirdScene::unload() {
    isLoaded = false;
}

void FlappyBirdScene::update() {
    if (!isLoaded) return;

    // --- Input: espacio o clic para iniciar/flapear ---
    if (!gameOver && (IsKeyPressed(KEY_SPACE) || IsMouseButtonPressed(MOUSE_LEFT_BUTTON))) {
        if (!gameStarted) {
            gameStarted = true;
            bird->setType(b2_dynamicBody);
            bird->setAwake(true);
        }
        bird->flap(8.0f);
    }

    if (!gameStarted || gameOver) return;

    // --- Generar tuberías ---
    pipeSpawnTimer += GetFrameTime();
    if (pipeSpawnTimer >= pipeSpawnInterval) {
        spawnPipe();
        pipeSpawnTimer = 0.0f;
    }
    if (bird->isDead) {
        // gameOver = true;  // Ya no usamos esto
        GameOverScene::instance().setFinalScore(score);
        SceneManager::instance().changeScene(&GameOverScene::instance());
        return; // Importante salir para no seguir procesando esta escena
    }
    // --- Mover tuberías (velocidad cinemática) y eliminar las que salen ---
    for (auto& pair : pipes) {
        pair.top->setVelocity({ -pipeSpeed, 0.0f });
        pair.bottom->setVelocity({ -pipeSpeed, 0.0f });
    }

    // Eliminar tuberías fuera de la pantalla y marcarlas para destrucción
    pipes.erase(std::remove_if(pipes.begin(), pipes.end(),
        [](const PipePair& p) {
            if (p.top->pos.x < -30.0f) {
                p.top->destroy();
                p.bottom->destroy();
                return true;
            }
            return false;
        }), pipes.end());

    // --- Puntuación: cuando el pájaro pasa una tubería ---
    for (auto& pair : pipes) {
        if (!pair.scored && bird->pos.x > pair.top->pos.x + pipeWidth / 2) {
            score++;
            pair.scored = true;
        }
    }

    // --- Detectar colisión del pájaro ---
    if (bird->isDead) {
        gameOver = true;
        bird->destroy();
    }

    // --- Reiniciar con R ---
    if (gameOver && IsKeyPressed(KEY_R)) {
        resetGame();
    }
}

void FlappyBirdScene::draw() {
    // El fondo azul ya se dibuja en el bucle principal (ClearBackground)
    // Las entidades se dibujan automáticamente en Scene::drawScene()
    // El HUD se dibuja con DrawText (coordenadas de pantalla)
    hud->draw(score, gameOver, gameStarted);

    if (!gameStarted && !gameOver) {
        DrawText("Press SPACE or CLICK to start", 200, 200, 20, DARKGRAY);
    }
}

void FlappyBirdScene::spawnPipe() {
    float gapY = GetRandomValue(-3, 5);   // posición vertical del hueco
    float x = 15.0f;                       // aparecen a la derecha

    BodyData pipeData;
    pipeData.isDynamic = false;            // serán cinemáticas
    pipeData.name = "Pipe";
    pipeData.tag = "Pipe";
    pipeData.size = { pipeWidth, 10.0f };
    pipeData.enableCollisions = true;

    // Tubería superior
    pipeData.pos = { x, gapY - pipeGap / 2 - pipeData.size.y / 2 };
    auto top = physics.makeBox<Pipe>(pipeData);
    top->setType(b2_kinematicBody);
    addEntity(top);

    // Tubería inferior
    pipeData.pos = { x, gapY + pipeGap / 2 + pipeData.size.y / 2 };
    auto bottom = physics.makeBox<Pipe>(pipeData);
    bottom->setType(b2_kinematicBody);
    addEntity(bottom);

    pipes.push_back({ top, bottom });
}

void FlappyBirdScene::resetGame() {
    // Reiniciar estado
    gameStarted = false;
    gameOver = false;
    score = 0;

    // Limpiar todas las entidades (incluye pájaro, suelo, tuberías)
    clear();

    // Volver a cargar la escena
    load();
}