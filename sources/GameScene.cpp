#include "GameScene.h"

GameScene::GameScene(int layerCount)
{
    while (layerCount--)
    {
        this->layers.push_back(new SceneLayer());
    }
}

GameScene::~GameScene()
{
}

void GameScene::setScenePlayer(int id, GameObject* scenePlayer)
{
	this->scenePlayer[id] = scenePlayer;
}

GameObject* GameScene::getScenePlayer(int id)
{
	return this->scenePlayer[id];
}

void GameScene::draw(const GameTime* gameTime)
{
    for (int i = 0; i < this->layers.size(); ++i)
        this->layers[i]->draw(gameTime);
}

void GameScene::update(const GameTime* gameTime)
{
    for (int i = 0; i < this->layers.size(); ++i)
        this->layers[i]->update(gameTime);
}

void GameScene::updatePhysics()
{
    for (int i = 0; i < this->layers.size(); ++i)
        this->layers[i]->updatePhysics();
}
