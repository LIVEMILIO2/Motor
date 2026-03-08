#pragma once
#include "EventManager.h"
#include "ResourceManager.h"
#include "PhysicsSystem.h"
#include <vector>
#include <algorithm>

class Scene
{
public:
    std::vector<std::shared_ptr<Entity>> entities;

    Camera2D cam;
    EventManager& eventManager = EventManager::instance();
	ResourceManager& resourceManager = ResourceManager::instance();
	PhysicsSystem& physics = PhysicsSystem::instance();

	bool isLoaded = false;

    Scene() {
  
        cam = { 0 };
        cam.zoom = 20.0f;
        cam.offset = { GetScreenWidth() / 4.0f, GetScreenHeight() / 2.0f };
	}
    
    virtual ~Scene() = default;

    void updateScene() {
        // 1. Avanzar la física
        update();
    
        physics.update(GetFrameTime());

        // 2. Actualizar lógica de cada entidad
        for (auto& e : entities) e->update();

        // 3. Borrado diferido: elimina las marcadas con destroy()
        entities.erase(
            std::remove_if(entities.begin(), entities.end(),
                [](const std::shared_ptr<Entity>& e) {
                    return !e->isActive();
                }),
            entities.end()
        );
        
    
    }
	void drawScene() {
        BeginMode2D(cam);
        for (auto& e : entities) e->draw();
        draw();
		EndMode2D();
       
    }

    virtual void load() = 0;
    virtual void unload() = 0;
    virtual void update() = 0;
    virtual void draw() = 0;

    void clear() {
        entities.clear(); // Limpia memoria y Box2D automáticamente
    }

    void addEntity(std::shared_ptr<Entity> e) {
        if (e) entities.push_back(e);
    }
};