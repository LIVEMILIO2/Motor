#pragma once
#include "Entity.h"
#include "box2d/box2d.h"

class PhysicsEntity;

struct Collision {
    PhysicsEntity* self;      // quien recibe
    PhysicsEntity* other;
    float force;
};

class PhysicsEntity : public Entity {
public:
    b2BodyId body;
    Color color;

public:
    PhysicsEntity(std::string n, std::string t, b2BodyId id, bool isDynamic)
        : Entity(n, t), body(id) {
        this->color = isDynamic ? RED : GREEN;
        updatePhysics(); // Sincronización inicial al nacer
    }

    virtual ~PhysicsEntity() override {
        if (b2Body_IsValid(body)) {
            b2DestroyBody(body);
            body = b2_nullBodyId;
        }
    }

    // El motor llama a update(), y nosotros delegamos la sincronización
    void update() override {
        updatePhysics();
    }

    // Nombre mucho más claro: "Actualiza mis datos con lo que diga el motor"
    void updatePhysics() {
        if (b2Body_IsValid(body)) {
            b2Vec2 b2p = b2Body_GetPosition(body);
            this->pos = { b2p.x, b2p.y };

            b2Rot b2r = b2Body_GetRotation(body);
            this->angle = b2Rot_GetAngle(b2r) * RAD2DEG;
        }
    }

    // Métodos de control para el Sling
    void setType(b2BodyType type) {
        if (b2Body_IsValid(body)) {
            b2Body_SetType(body, type);
            setAwake(true); 
            b2Body_ApplyMassFromShapes(body);
			debugStatus();
        }
    }
    

    void debugStatus() {
            if (!b2Body_IsValid(body)) {
                printf("ERROR: ¡El body NO es válido!\n");
                return;
            }

            b2Vec2 p = b2Body_GetPosition(body);
            b2BodyType t = b2Body_GetType(body);
            bool awake = b2Body_IsAwake(body);
            float mass = b2Body_GetMass(body);

            printf("POS: %.2f, %.2f | TIPO: %d | AWAKE: %d | MASA: %.2f\n",
                p.x, p.y, (int)t, (int)awake, mass);
     }

    void setBodyPosition(Vector2 newPos) {
        if (!b2Body_IsValid(body)) return;

        // 1. Teletransporte absoluto
        b2Body_SetTransform(body, { newPos.x, newPos.y }, b2Rot_identity);

        // 2. EL SECRETO: En Kinematics, para que la sincronización sea instantánea,
        // a veces es necesario resetear el estado de "sleep" manualmente.
        b2Body_SetAwake(body, true);

        // 3. Sincronización forzada
        // Como tú quieres moverlo tú mismo, después de mover el cuerpo físico,
        // actualizamos la variable visual para que el DRAW no use el dato viejo.
        this->pos = newPos;
    }

    void applyImpulse(Vector2 impulse) {
        if (b2Body_IsValid(body)) {
			//b2Body_ApplyMassFromShapes(body); // Aseguramos que la masa esté actualizada    
            //b2Body_ApplyLinearImpulseToCenter(body, { impulse.x, impulse.y }, true);
			//b2Body_SetBullet(body, true); // Evita que el cuerpo atraviese otros objetos a altas velocidades
            b2Body_SetLinearVelocity(body, { impulse.x, impulse.y});
           // b2Vec2 velocity = b2Body_GetLinearVelocity(body);
            //printf("VELOCIDAD TRAS IMPULSO: X: %.2f, Y: %.2f\n", velocity.x, velocity.y);
        }
    }

    void setAwake(bool awake) {
        if (b2Body_IsValid(body)) {
            b2Body_SetAwake(body, awake);
           // b2Body_SetLinearVelocity(body, { 0, 0 });

        }
    }

    virtual void onCollisionEnter(const Collision& collision) {}
};
