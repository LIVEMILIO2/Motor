#pragma once
#pragma once
#include "Scene.h"


class PlayScene : public Scene
{
public:
    static PlayScene& instance();

    void load() override;
    void unload() override;
    void update() override;
    void draw() override;

private:
    PlayScene() {} // private constructor for singleton
    BodyData birdDef;
    BodyData boxDef;
	BodyData floorDef;
	BodyData wallDef;
    std::shared_ptr<PCircle> bird; // Puntero para acceder al pájaro fácilmente
   
    // Configuración de la Resortera
    Vector2 anchor;
    float maxPull;
    float forceMult;
    bool isDragging;
    Vector2 birdStarPos;

	bool isLaunch = false; 

    const float MAX_X = 250.0f; // Límite absoluto donde la cámara se detiene

    float followSpeed = 6.0f;
    float deadZoneX = 8.0f;   // en unidades mundo
    float deadZoneY = 4.0f;
    float maxTravelDistance = 40.0f; // 3 pantallas
    float cameraStartX = 0.0f;
    bool cameraInitialized = false;
    float camSpeed = 20.0f; // world units per second

    Vector2 getClampedMousePos(Vector2 mousePos);
	void updateCamera();

};