#pragma once
#include "Scene.h"

class InstructionsScene : public Scene {
public:
    static InstructionsScene& instance();

    void load() override;
    void unload() override;
    void update() override;
    void draw() override;

private:
    InstructionsScene() = default;
};