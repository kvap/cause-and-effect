#ifndef GAME_SCENE_H
#define GAME_SCENE_H

#include <vector>
#include "GameTime.h"
#include "SceneLayer.h"

class GameScene
{
public:
    GameScene(int layerCount);
    ~GameScene();
private:

protected:

public:
    std::vector<SceneLayer*> layers;

    void draw(const GameTime* gameTime);
    void update(const GameTime* gameTime);
    void updatePhysics();
};

#endif // GAME_SCENE_H
