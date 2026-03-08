#pragma once
#include "raylib.h"

class HUD {
public:
    void draw(int score, bool gameOver, bool gameStarted) {
        DrawText(TextFormat("Score: %d", score), 10, 10, 30, BLACK);

        if (gameOver) {
            const char* msg = "GAME OVER\nPress R to restart";
            DrawText(msg, 250, 150, 30, RED);
        }
    }
};