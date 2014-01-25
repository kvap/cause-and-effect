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
