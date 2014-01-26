#include "SceneLoader.h"

#include <map>

typedef std::map<std::string, GameScene*> GameSceneMap;
GameSceneMap gameScenes;

#include <cstdlib>
#include "GameObjects/Character.h"
#include <libxml++/libxml++.h>

#include "util/Logger.hpp"
#include "GameScene.h"
#include "GameObjects/Box.h"
//#include <libxml++/parsers/domparser.h>

//#include <stdio.h>
//#include <string.h>
//#include <iostream>
//#include <fstream>
//#include "GameScene.h"
//#include "GameObjects/Box.h"
//using namespace std;
//#define STR_LENGTH 1024
//int layerNumber;
//struct puzzleBlock{
//	double width;
//	double height;
//	double x;
//	double y;
//	char* name;
//};
//puzzleBlock sceneElem;
//SceneLoader::SceneLoader()
//{ }
//
//void SceneLoader::addSpr(GameScene* gs){
//	printf("%s\n", sceneElem.name);
//	if (strcmp(sceneElem.name, "earth") == 0){
//	    printf("addSpr->earth\n");
//		Box* b1 = new Box(Point(sceneElem.x+sceneElem.width/2,sceneElem.y+sceneElem.height/2),Point(sceneElem.width,sceneElem.height));
//		gs->layers[layerNumber]->add(b1, Physics::STATIC);
//	}
//	if (strcmp(sceneElem.name, "apple") == 0){
//	    printf("addSpr->apple\n");
//		Box* b2 = new Box(Point(sceneElem.x+sceneElem.width/2,sceneElem.y+sceneElem.height/2),Point(sceneElem.width,sceneElem.height));
//		gs->layers[layerNumber]->add(b2, Physics::DYNAMIC);
//	}
//
//}
//
//GameScene* SceneLoader::load(char* fileName){
//	char str[STR_LENGTH];	//ïîäãðóæàåìàÿ ñòðîêà
//	char *strpart;
//	string tempstr;
//	char* name;
//	GameScene* gs = new GameScene(1);
//	//ñòðîêà îáðàáîòêè
//	ifstream dev(fileName);
//	bool endRead=false;
//	double wei=0,hei=0,x=0,y=0;
//	while (!endRead){
//		dev.getline(str,STR_LENGTH);
//		strpart=strstr(str,"<rect");
//		if (strpart==NULL){
//			strpart=strstr(str,"</g");
//			if (strpart!=NULL){
//				endRead=true;
//			}
//		}else{
//			dev.getline(str,STR_LENGTH);
//			while (strstr(str,"<title")==NULL){
//				tempstr=(const char*)str;
//				if (strstr(str,"width=")!=NULL){
//					int step=tempstr.find("\"")+1;
//					tempstr=tempstr.substr(step,tempstr.length()-1-step);
//					wei=atof(tempstr.c_str());
//				}
//
//				if (strstr(str,"height=")!=NULL){
//					int step=tempstr.find("\"")+1;
//					tempstr=tempstr.substr(step,tempstr.length()-1-step);
//					hei=atof(tempstr.c_str());
//				}
//				if (strstr(str,"x=")!=NULL){
//					int step=tempstr.find("\"")+1;
//					tempstr=tempstr.substr(step,tempstr.length()-1-step);
//					x=atof(tempstr.c_str());
//				}
//				if (strstr(str,"y=")!=NULL){
//					int step=tempstr.find("\"")+1;
//					tempstr=tempstr.substr(step,tempstr.length()-1-step);
//					y=atof(tempstr.c_str());
//				}
//
//				dev.getline(str,STR_LENGTH);	//next line
//			}
//			dev.getline(str,STR_LENGTH);
//			tempstr=(const char*)str;
//			int step=tempstr.find(">")+1;
//			tempstr=tempstr.substr(step,tempstr.find("<")-step);
//			name= new char[tempstr.size()+1] ;
//			strcpy(name, tempstr.c_str());
//
//			sceneElem.width=wei;
//			sceneElem.height=hei;
//			sceneElem.x=x;
//			sceneElem.y=y;
//			sceneElem.name=name;
//			addSpr(gs);
//		}
//	}
//	dev.close();
//	return gs;
//}

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
			if (id == "player")
			{
				Character* c = new Character(position, size);
				gameScene->setScenePlayer(c);
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
