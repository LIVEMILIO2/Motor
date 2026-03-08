#pragma once
#include "Bodies.h"
#include <memory>
#include "EventManager.h"
#include "EventTypes.h"
#include <string>


struct BodyData {
    Vector2 pos = { 30.0f, 30.0f };
    Vector2 size = { 40.0f, 40.0f };  // Para cajas
    float radius = 20.0f;             // Para círculos
    bool isDynamic = true;            // ¿Cae con la gravedad?
    bool enableCollisions = false;
	std::string name = "Entity";
    float density = 1.0f;
    float friction = 0.3f;
    float restitution = 0.5f;         // Rebote (0.0 a 1.0)
    std::string tag = "Entity";       // Para identificarlo en colisiones
};



class PhysicsSystem {
private:
    b2WorldId world;
    float accumulator = 0.0f;
    const float timeStep = 1.0f / 60.0f;

    // Constructor privado para el Singleton
    PhysicsSystem() {
        initWorld();
    }

    void initWorld() {
        b2WorldDef worldDef = b2DefaultWorldDef();
        worldDef.gravity = { 0.0f, 9.8f}; // Gravedad estilo Raylib
        world = b2CreateWorld(&worldDef);
    }

public:
    // Destructor para limpiar el mundo al cerrar
    ~PhysicsSystem() {
        if (b2World_IsValid(world)) {
            b2DestroyWorld(world);
        }
    }

    static PhysicsSystem& instance() {
        static PhysicsSystem ps;
        return ps;
    }

    // Reinicia el mundo (útil al cambiar de escena)
    void reset() {
        if (b2World_IsValid(world)) {
            b2DestroyWorld(world);
        }
        initWorld();
    }

    // Paso de simulación físico
    void update(float dt) {
     
            b2World_Step(world, dt, 12);

			processCollisions();
    }

    void processCollisions() {
        // Obtenemos el buffer de contactos acumulados
        b2ContactEvents events = b2World_GetContactEvents(world);

        for (int i = 0; i < events.beginCount; ++i) {
            b2ContactBeginTouchEvent* event = events.beginEvents + i;



            b2BodyId bodyA = b2Shape_GetBody(event->shapeIdA);
            b2BodyId bodyB = b2Shape_GetBody(event->shapeIdB);


            // Recuperamos nuestras entidades
            PhysicsEntity* entA = static_cast<PhysicsEntity*>(b2Body_GetUserData(bodyA));
            PhysicsEntity* entB = static_cast<PhysicsEntity*>(b2Body_GetUserData(bodyB));

            if (entA && entB) {
                // Fuerza: Diferencia de velocidad entre ambos cuerpos
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


    std::shared_ptr<PBox> makeBox(const BodyData& data) {
        return makeBox(data.name, data.tag, data.pos, data.size, data.isDynamic, data.enableCollisions);
	}

    std::shared_ptr<PCircle> makeCircle(const BodyData& data) {
        return makeCircle(data.name, data.tag, data.pos, data.radius, data.isDynamic);
    }

    // Fábrica para Cajas (PBox)
    std::shared_ptr<PBox> makeBox(std::string name, std::string tag, Vector2 pos, Vector2 size, bool isDynamic, bool enableCollisions) {
        // 1. Definir y crear el cuerpo
        b2BodyDef bodyDef = b2DefaultBodyDef();
        bodyDef.type = isDynamic ? b2_dynamicBody : b2_staticBody;
        bodyDef.position = { pos.x, pos.y };
        b2BodyId bodyId = b2CreateBody(world, &bodyDef);

        // 2. Definir y crear la forma (Shape)
        b2ShapeDef shapeDef = b2DefaultShapeDef();
      
		shapeDef.enableContactEvents = enableCollisions; // Habilitamos eventos de contacto para esta forma
		
        
        // Box2D 3.1.1 usa b2MakeBox para polígonos rectangulares
        b2Polygon box = b2MakeBox(size.x / 2.0f, size.y / 2.0f);
        b2CreatePolygonShape(bodyId, &shapeDef, &box);
        auto boxEntity = std::make_shared<PBox>(name, tag, bodyId, size, isDynamic);

        // 4. ¡VITAL! Vincular la instancia recién creada con el cuerpo de Box2D
        // Usamos boxEntity.get() para obtener el puntero crudo (Entity*)
        b2Body_SetUserData(bodyId, boxEntity.get());
        return boxEntity;
    }

    // Fábrica para Círculos (PCircle)
    std::shared_ptr<PCircle> makeCircle(std::string name, std::string tag, Vector2 pos, float radius, bool isDynamic) {
        // 1. Definir y crear el cuerpo
        b2BodyDef bodyDef = b2DefaultBodyDef();
        bodyDef.type = isDynamic ? b2_dynamicBody : b2_staticBody;
        bodyDef.position = { pos.x, pos.y };
        
        b2BodyId bodyId = b2CreateBody(world, &bodyDef);
     
        // 2. Definir y crear la forma circular
        b2ShapeDef shapeDef = b2DefaultShapeDef();
        shapeDef.material.friction = 2.0f;
        //shapeDef.density = 0.01f;

        shapeDef.enableContactEvents = true; // Habilitamos eventos de contacto para esta forma
        b2Circle circle = { {0.0f, 0.0f}, radius };
        b2CreateCircleShape(bodyId, &shapeDef, &circle);

        auto circleEntity = std::make_shared<PCircle>(name, tag, bodyId, radius, isDynamic);

        // 4. VINCULAR: Ahora que tenemos el objeto, le damos el puntero a Box2D
        b2Body_SetUserData(bodyId, circleEntity.get());

        return circleEntity;
    }


};