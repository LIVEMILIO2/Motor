#pragma once
#include "Bodies.h"

class Bird : public PCircle {
public:
    Bird(std::string name, std::string tag, b2BodyId id, float radius, bool dynamic)
        : PCircle(name, tag, id, radius, dynamic) {
    }

    void onCollisionEnter(const Collision& collision) override {
        if (collision.other->tag == "Pipe" || collision.other->tag == "Ground") {
            isDead = true;
        }
    }

    void flap(float impulse) {
        if (!isDead && b2Body_IsValid(body)) {
            b2Body_SetLinearVelocity(body, { 0.0f, -impulse });
        }
    }

    bool isDead = false;
};