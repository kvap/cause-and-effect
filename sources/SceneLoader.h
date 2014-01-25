#ifndef SCENE_LOADER_H
#define SCENE_LOADER_H

#include <string>
#include "GameScene.h"
class SceneLoader
{
private:
	SceneLoader();
public:
	static void addSpr(GameScene* gs);

	static GameScene* load(char* fileName);

	static GameScene* loadScene(const char* sceneName);

	static GameScene* loadGameScene(std::string sceneName);
};
#endif // SCENE_LOADER_H
