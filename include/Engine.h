#ifndef ENGINE
#define ENGINE
#include "SceneManager.h"


class Engine {

private:
	SceneManager& sceneManager = SceneManager::instance();

public:
	Engine();
    void intialize();
	void run();
    void update();
	void draw();
	void shutdown();
};

#endif // ENGINE