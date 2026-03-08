#pragma once
#include "Bodies.h"

class Pipe : public PBox {
public:
    Pipe(std::string name, std::string tag, b2BodyId id, Vector2 size, bool dynamic)
        : PBox(name, tag, id, size, dynamic) {
    }

    void setVelocity(Vector2 vel) {
        if (b2Body_IsValid(body) && b2Body_GetType(body) == b2_kinematicBody) {
            b2Body_SetLinearVelocity(body, { vel.x, vel.y });
        }
    }
};

struct PipePair {
    std::shared_ptr<Pipe> top;
    std::shared_ptr<Pipe> bottom;
    bool scored = false;
};