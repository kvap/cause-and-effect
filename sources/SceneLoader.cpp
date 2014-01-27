﻿#include "SceneLoader.h"

#include <map>

typedef std::map<std::string, GameScene*> GameSceneMap;
GameSceneMap gameScenes;

#include <cstdlib>
#include "GameObjects/Character.h"
#include <libxml++/libxml++.h>

#include "GameObjects/Character2.h"
#include "util/Logger.hpp"
#include "GameScene.h"
#include "GameObjects/Box.h"

GameScene* loadSceneFromFile(std::string filename) {
	xmlpp::DomParser parser(filename);
	xmlpp::Element *root = parser.get_document()->get_root_node();
	xmlpp::Node::PrefixNsMap prefixes;
	prefixes["svg"] = "http://www.w3.org/2000/svg";
	xmlpp::NodeSet ns = root->find("//svg:rect", prefixes);
	GameScene* gameScene = new GameScene(1);
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
				gameScene->setScenePlayer(0, c);
				gameScene->layers[0]->add(c, Physics::DYNAMIC);
			}
			else if (id == "zombie")
			{
				Character2* c = new Character2(position, size);
				gameScene->setScenePlayer(1, c);
				gameScene->layers[0]->add(c, Physics::DYNAMIC);

			}
			else
			{
				Box* box = new Box(position, size);
				gameScene->layers[0]->add(box, Physics::STATIC);
			}


			LOG_STRING("found a rect: " + x + ", " + y + ", " + wid + "x" + hei);
		} else {
			LOG_WARNING("found a rect, but it is not an Element instance!");
		}
	}
	return gameScene; // FIXME
}

GameScene* loadScene(std::string sceneName) {
	std::string filename = "scenes/" + sceneName;
	GameSceneMap::iterator it = gameScenes.find(filename);
	if (it != gameScenes.end())
		return it->second;
	gameScenes[filename] = loadSceneFromFile(filename);
	return gameScenes[filename];
}
