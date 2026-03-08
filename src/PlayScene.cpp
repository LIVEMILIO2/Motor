#include "PlayScene.h"
#include "SceneManager.h"
#include "raylib.h"
#include "MenuScene.h"

#include "nlohmann/json.hpp"
#include <fstream>
#include <string>

using json = nlohmann::json;

PlayScene& PlayScene::instance()
{
    static PlayScene instance;
    return instance;
}

void PlayScene::load()
{
	if (isLoaded) return;
	birdDef.pos = { -4, 0};
	birdDef.isDynamic = false;
	birdDef.name = "Bird";
	birdDef.tag = "Bird";
	birdDef.radius = 1.0f;
	birdDef.enableCollisions = true; 

	birdStarPos = birdDef.pos; 

	boxDef.pos = { 0, 0 };
	boxDef.isDynamic = true;
	boxDef.name = "Box";
	boxDef.tag = "Box";
	boxDef.size = { 1, 1 };
	boxDef.enableCollisions = true;

	floorDef.pos = { -6,6 };
	floorDef.isDynamic = false;
	floorDef.name = "Floor";
	floorDef.size = {500, 2 };
    floorDef.enableCollisions = false;

	wallDef.pos = { 70, -5 };
	wallDef.isDynamic = false;
	wallDef.name = "Wall";
	wallDef.size = { 2, 20 };
	wallDef.enableCollisions = false;

	anchor = birdDef.pos; // El punto fijo de la resortera es la posición inicial del pájaro
	maxPull = 100.0f; // Máxima distancia de estiramiento
    forceMult = 30.0f;
   
    bird = physics.makeCircle(birdDef);// Guardamos el puntero para controlarlo fácilmente
    

	addEntity(bird);
	//addEntity(physics.makeBox(boxDef));
	addEntity(physics.makeBox(floorDef));
	addEntity(physics.makeBox(wallDef));
	wallDef.pos = { -10, -5 };
	addEntity(physics.makeBox(wallDef));


	std::ifstream file("assets/json/p_objects.json");
    if (file.is_open()) {
		printf("Archivo JSON abierto correctamente\n");
		json j = json::parse(file);

        for (auto& item: j.items())
        {
            json obj = item.value();
			std::string type = obj["type"];
			std::cout << type << std::endl;

            if (type == "PBox") {
                BodyData data;
                data.pos.x = obj["x"];
                data.pos.y = obj["y"];
				data.size.x = obj["w"];
				data.size.y = obj["h"];
                data.tag = "Box";
                data.isDynamic = true;
                addEntity(physics.makeBox(data));
            }
        }
		//printf("Tipo de objeto en JSON: %s\n", type.c_str());
        //std::cout << x << std::endl;
    }
    else {
        printf("ERROR: No se pudo abrir el archivo JSON\n");
        return;
	}



    for (int i = 0; i < 4; i++)
    {
        // Copiamos la definición base
        BodyData current = boxDef;

        // Ajustamos solo la posición en Y
        current.pos.y = boxDef.pos.y - i * boxDef.size.y;
        addEntity(physics.makeBox(current));
    }


}

void PlayScene::unload()
{
}

void PlayScene::update()
{
	
	updateCamera();

    Vector2 mouse = GetMousePosition();

    // --- LÓGICA DE LA RESORTERA ---
	Vector2 mousePos = GetScreenToWorld2D(mouse, cam); // Convertimos a coordenadas del mundo
    // A. Inicio del Drag
    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        // Usamos la función de Raylib para detectar si tocamos al pájaro
        if (CheckCollisionPointCircle(mousePos, bird->pos, bird->radius + 0.1f)) {
            isDragging = true;
            bird->setType(b2_kinematicBody); // Llamada única protegida
			Log::print("¡Se convierte kinematic otra vez!"); // Debug
        }
    }

    if (isDragging) {
        // C. Fin del Drag (Disparo) - MOVER ESTO AL PRINCIPIO DEL BLOQUE
        if (IsMouseButtonReleased(MOUSE_LEFT_BUTTON)) {
            isDragging = false;

            Vector2 diff = Vector2Subtract(anchor, mousePos);
			Vector2 norm = Vector2Normalize(diff);
            Vector2 impulse = Vector2Scale(norm, forceMult);

            bird->setType(b2_dynamicBody);
			//b2Body_ApplyMassFromShapes(bird->body); // Aseguramos que la masa esté actualizada antes de aplicar el impulso
            ///bird->setAwake(true);
        
			//printf("IMPULSO APLICADO: X: %.2f, Y: %.2f\n", impulse.x, impulse.y);
            bird->applyImpulse(impulse);
			isLaunch = true;
			cameraStartX = cam.target.x; // Guardamos la posición inicial de la cámara para el seguimiento
            //printf("Mass: %.2f\n", b2Body_GetMass(bird->body));
            //printf("Damping: %.2f\n", b2Body_GetLinearDamping(bird->body));
        }
        else {
             //B. Durante el Drag - Solo se mueve si NO se ha soltado este frame
           Vector2 constrainedPos = getClampedMousePos(mousePos);
           bird->setBodyPosition(constrainedPos);
           Log::print("Se cambia la posición");
        }
    }

    if (isLaunch) {
        b2Vec2 v = b2Body_GetLinearVelocity(bird->body);
        float speedSq = v.x * v.x + v.y * v.y;   // sin sqrt
        float angularSpeed = fabsf(b2Body_GetAngularVelocity(bird->body));

        if (speedSq < 0.5f && angularSpeed < 0.2f)
        {
            b2Body_SetLinearVelocity(bird->body, { 0, 0 });
            b2Body_SetAngularVelocity(bird->body, 0.0f);
			isLaunch = false; // El pájaro se ha detenido, podemos reiniciar el seguimiento
			bird->setType(b2_kinematicBody); // Llamada única protegida
			bird->setBodyPosition(birdStarPos); // Reiniciamos la posición del pájaro
			cam.target = birdStarPos; // Reiniciamos la cámara al inicio
        }
    }

 //   if (isLaunch && (b2Body_GetLinearVelocity(bird->body).x < 0.1f || b2Body_GetLinearVelocity(bird->body).y < 0.1f)) {
 //       isLaunch = false; // Aseguramos que el seguimiento se active si el pájaro empieza a moverse por cualquier razón
 //       bird->setType(b2_kinematicBody); // Llamada única protegida
	//	bird->setBodyPosition(birdStarPos); // Reiniciamos la posición del pájaro
	//}
	float smooth = 2.0f * GetFrameTime(); // Ajusta la velocidad de seguimiento

    if (!isLaunch || cam.target.x >= MAX_X) return;
 

    float dt = GetFrameTime();
    float smoothFactor = 1.0f - expf(-followSpeed * dt);

    float maxCameraX = cameraStartX + maxTravelDistance;

    if (cam.target.x >= maxCameraX)
    {
        cam.target.x = maxCameraX;
        return;
    }
    Vector2 targetPos = bird->pos;
    Vector2 desiredTarget = cam.target;

    // DEAD ZONE X
    float dx = targetPos.x - cam.target.x;
    if (fabs(dx) > deadZoneX)
        desiredTarget.x = targetPos.x - (dx > 0 ? deadZoneX : -deadZoneX);

    // DEAD ZONE Y
    float dy = targetPos.y - cam.target.y;
    if (fabs(dy) > deadZoneY)
        desiredTarget.y = targetPos.y - (dy > 0 ? deadZoneY : -deadZoneY);

    // SUAVIZADO
    cam.target.x += (desiredTarget.x - cam.target.x) * smoothFactor;
    cam.target.y += (desiredTarget.y - cam.target.y) * smoothFactor;

    // Clamp final
    if (cam.target.x > maxCameraX)
        cam.target.x = maxCameraX;
}

void PlayScene::draw()
{
    //ClearBackground(RAYWHITE);
    DrawText("PlayScene", 190, 200, 20, LIGHTGRAY);
	//DrawLineEx(anchor, bird->pos, 0.2f, RED); // Dibuja la línea de la resortera
}

Vector2 PlayScene::getClampedMousePos(Vector2 mousePos) {
    Vector2 dir = Vector2Subtract(mousePos, anchor);
    float dist = Vector2Length(dir);

    // Si el estiramiento supera el máximo...
    if (dist > maxPull) {
        // Normalizamos el vector (longitud 1) y lo escalamos al máximo permitido
        dir = Vector2Scale(Vector2Normalize(dir), maxPull);
    }

    return Vector2Add(anchor, dir);
}


void PlayScene::updateCamera()
{
    float dt = GetFrameTime();

    if (IsKeyDown(KEY_RIGHT))
        cam.target.x += camSpeed * dt;

    if (IsKeyDown(KEY_LEFT))
        cam.target.x -= camSpeed * dt;
}
