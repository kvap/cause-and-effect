#include "GameScene.h"

#include <cstdlib>
#include <libxml++/libxml++.h>

#include "GameObjects/Character.h"
#include "GameObjects/Character2.h"
#include "GameObjects/Box.h"

#include "util/Logger.hpp"

GameScene::GameScene(std::string filename)
{
	xmlpp::DomParser parser(filename);
	xmlpp::Element *root = parser.get_document()->get_root_node();
	xmlpp::Node::PrefixNsMap prefixes;
	prefixes["svg"] = "http://www.w3.org/2000/svg";
	xmlpp::NodeSet ns = root->find("//svg:rect", prefixes);

	layers.push_back(new SceneLayer());
	Point position, size;

	for (xmlpp::NodeSet::iterator i = ns.begin(); i != ns.end(); i++) {
		xmlpp::Element *rect = dynamic_cast<xmlpp::Element*>(*i);
		if (rect) {
			std::string x = rect->get_attribute_value("x");
			std::string y = rect->get_attribute_value("y");
			std::string wid = rect->get_attribute_value("width");
			std::string hei = rect->get_attribute_value("height");

			position.x = atof(x.c_str());
			position.y = atof(y.c_str());
			size.x = atof(wid.c_str()) / 2.0f;
			size.y = atof(hei.c_str()) / 2.0f;
			position.x += size.x;
			position.y = -position.y - size.y;

			std::string id = rect->get_attribute_value("id");
			if (id == "char")
			{
				Character* c = new Character(position, size);
				setScenePlayer(0, c);
				layers[0]->add(c, Physics::DYNAMIC);
			}
			else if (id == "zombie")
			{
				Character2* c = new Character2(position, size);
				setScenePlayer(1, c);
				layers[0]->add(c, Physics::DYNAMIC);

			}
			else
			{
				Box* box = new Box(position, size);
				layers[0]->add(box, Physics::STATIC);
			}


			LOG_STRING("found a rect: " + x + ", " + y + ", " + wid + "x" + hei);
		} else {
			LOG_WARNING("found a rect, but it is not an Element instance!");
		}
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
