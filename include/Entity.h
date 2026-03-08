#pragma once
#include <string>
#include <raylib.h>


class Entity {
public:
    std::string name;
    std::string tag;
    Vector2 pos;
    float angle;
    bool active = true;

public:
    Entity(std::string name, std::string tag)
        : name(name), tag(tag), pos({ 0,0 }), angle(0) {
    }

    virtual ~Entity() = default;

    virtual void update() = 0;
    virtual void draw() = 0;

    bool isActive() const { return active; }
    void destroy() { active = false; }

    std::string getName() const { return name;}
    std::string getTag() const { return tag; }
};