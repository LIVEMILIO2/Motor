#pragma once
#include "Bodies.h"
#include <memory>
#include "EventManager.h"
#include "EventTypes.h"
#include <string>
#include <type_traits>

struct BodyData {
    Vector2 pos = { 30.0f, 30.0f };
    Vector2 size = { 40.0f, 40.0f };
    float radius = 20.0f;
    bool isDynamic = true;
    bool enableCollisions = false;
    std::string name = "Entity";
    float density = 1.0f;
    float friction = 0.3f;
    float restitution = 0.5f;
    std::string tag = "Entity";
};

class PhysicsSystem {
private:
    b2WorldId world;
    float accumulator = 0.0f;
    const float timeStep = 1.0f / 60.0f;

    PhysicsSystem() { initWorld(); }
    void initWorld() {
        b2WorldDef worldDef = b2DefaultWorldDef();
        worldDef.gravity = { 0.0f, 9.8f };
        world = b2CreateWorld(&worldDef);
    }

public:
    ~PhysicsSystem() {
        if (b2World_IsValid(world)) {
            b2DestroyWorld(world);
        }
    }

    static PhysicsSystem& instance() {
        static PhysicsSystem ps;
        return ps;
    }

    void reset() {
        if (b2World_IsValid(world)) {
            b2DestroyWorld(world);
        }
        initWorld();
    }

    void update(float dt) {
        b2World_Step(world, dt, 12);
        processCollisions();
    }

    void processCollisions() {
        b2ContactEvents events = b2World_GetContactEvents(world);
        for (int i = 0; i < events.beginCount; ++i) {
            b2ContactBeginTouchEvent* event = events.beginEvents + i;
            b2BodyId bodyA = b2Shape_GetBody(event->shapeIdA);
            b2BodyId bodyB = b2Shape_GetBody(event->shapeIdB);

            PhysicsEntity* entA = static_cast<PhysicsEntity*>(b2Body_GetUserData(bodyA));
            PhysicsEntity* entB = static_cast<PhysicsEntity*>(b2Body_GetUserData(bodyB));

            if (entA && entB) {
                b2Vec2 vA = b2Body_GetLinearVelocity(bodyA);
                b2Vec2 vB = b2Body_GetLinearVelocity(bodyB);
                float force = b2Length(b2Sub(vA, vB));
                Collision cA = { entA, entB, force };
                Collision cB = { entB, entA, force };
                entA->onCollisionEnter(cA);
                entB->onCollisionEnter(cB);
            }
        }
    }

    // --- Funciones originales (sin cambios) ---
    std::shared_ptr<PBox> makeBox(const BodyData& data) {
        return makeBox(data.name, data.tag, data.pos, data.size, data.isDynamic, data.enableCollisions);
    }

    std::shared_ptr<PCircle> makeCircle(const BodyData& data) {
        return makeCircle(data.name, data.tag, data.pos, data.radius, data.isDynamic);
    }

    std::shared_ptr<PBox> makeBox(std::string name, std::string tag, Vector2 pos, Vector2 size, bool isDynamic, bool enableCollisions) {
        b2BodyDef bodyDef = b2DefaultBodyDef();
        bodyDef.type = isDynamic ? b2_dynamicBody : b2_staticBody;
        bodyDef.position = { pos.x, pos.y };
        b2BodyId bodyId = b2CreateBody(world, &bodyDef);

        b2ShapeDef shapeDef = b2DefaultShapeDef();
        shapeDef.enableContactEvents = enableCollisions;
        b2Polygon box = b2MakeBox(size.x / 2.0f, size.y / 2.0f);
        b2CreatePolygonShape(bodyId, &shapeDef, &box);

        auto boxEntity = std::make_shared<PBox>(name, tag, bodyId, size, isDynamic);
        b2Body_SetUserData(bodyId, boxEntity.get());
        return boxEntity;
    }

    std::shared_ptr<PCircle> makeCircle(std::string name, std::string tag, Vector2 pos, float radius, bool isDynamic) {
        b2BodyDef bodyDef = b2DefaultBodyDef();
        bodyDef.type = isDynamic ? b2_dynamicBody : b2_staticBody;
        bodyDef.position = { pos.x, pos.y };
        b2BodyId bodyId = b2CreateBody(world, &bodyDef);

        b2ShapeDef shapeDef = b2DefaultShapeDef();
        shapeDef.material.friction = 2.0f;
        shapeDef.enableContactEvents = true;
        b2Circle circle = { {0.0f, 0.0f}, radius };
        b2CreateCircleShape(bodyId, &shapeDef, &circle);

        auto circleEntity = std::make_shared<PCircle>(name, tag, bodyId, radius, isDynamic);
        b2Body_SetUserData(bodyId, circleEntity.get());
        return circleEntity;
    }

    // --- NUEVAS FUNCIONES TEMPLATE (añadidas, no reemplazan) ---
    template<typename T = PCircle>
    std::shared_ptr<T> makeCircle(const BodyData& data) {
        return makeCircle<T>(data.name, data.tag, data.pos, data.radius, data.isDynamic);
    }

    template<typename T = PCircle>
    std::shared_ptr<T> makeCircle(std::string name, std::string tag, Vector2 pos, float radius, bool isDynamic) {
        b2BodyDef bodyDef = b2DefaultBodyDef();
        bodyDef.type = isDynamic ? b2_dynamicBody : b2_staticBody;
        bodyDef.position = { pos.x, pos.y };
        b2BodyId bodyId = b2CreateBody(world, &bodyDef);

        b2ShapeDef shapeDef = b2DefaultShapeDef();
        shapeDef.material.friction = 2.0f;
        shapeDef.enableContactEvents = true;
        b2Circle circle = { {0.0f, 0.0f}, radius };
        b2CreateCircleShape(bodyId, &shapeDef, &circle);

        auto entity = std::make_shared<T>(name, tag, bodyId, radius, isDynamic);
        b2Body_SetUserData(bodyId, entity.get());
        return entity;
    }

    template<typename T = PBox>
    std::shared_ptr<T> makeBox(const BodyData& data) {
        return makeBox<T>(data.name, data.tag, data.pos, data.size, data.isDynamic, data.enableCollisions);
    }

    template<typename T = PBox>
    std::shared_ptr<T> makeBox(std::string name, std::string tag, Vector2 pos, Vector2 size, bool isDynamic, bool enableCollisions) {
        b2BodyDef bodyDef = b2DefaultBodyDef();
        bodyDef.type = isDynamic ? b2_dynamicBody : b2_staticBody;
        bodyDef.position = { pos.x, pos.y };
        b2BodyId bodyId = b2CreateBody(world, &bodyDef);

        b2ShapeDef shapeDef = b2DefaultShapeDef();
        shapeDef.enableContactEvents = enableCollisions;
        b2Polygon box = b2MakeBox(size.x / 2.0f, size.y / 2.0f);
        b2CreatePolygonShape(bodyId, &shapeDef, &box);

        auto entity = std::make_shared<T>(name, tag, bodyId, size, isDynamic);
        b2Body_SetUserData(bodyId, entity.get());
        return entity;
    }
};