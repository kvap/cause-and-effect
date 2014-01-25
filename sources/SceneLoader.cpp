#include "SceneLoader.h"

#include <map>

typedef std::map<std::string, GameScene*> GameSceneMap;
GameSceneMap gameScenes;

#include <stdio.h>
#include <string.h>
#include <iostream>
#include <fstream>
#include "GameScene.h"
#include "GameObjects/Box.h"
using namespace std;
#define STR_LENGTH 1024
int layerNumber;
struct puzzleBlock{
	double width;
	double height;
	double x;
	double y;
	char* name;
};
puzzleBlock sceneElem;
SceneLoader::SceneLoader()
{ }

void SceneLoader::addSpr(GameScene* gs){
	printf("%s\n", sceneElem.name);
	if (strcmp(sceneElem.name, "earth") == 0){
	    printf("addSpr->earth\n");
		Box* b1 = new Box(Point(sceneElem.x+sceneElem.width/2,sceneElem.y+sceneElem.height/2),Point(sceneElem.width,sceneElem.height));
		gs->layers[layerNumber]->add(b1, Physics::STATIC);
	}
	if (strcmp(sceneElem.name, "apple") == 0){
	    printf("addSpr->apple\n");
		Box* b2 = new Box(Point(sceneElem.x+sceneElem.width/2,sceneElem.y+sceneElem.height/2),Point(sceneElem.width,sceneElem.height));
		gs->layers[layerNumber]->add(b2, Physics::DYNAMIC);
	}

}

GameScene* SceneLoader::load(char* fileName){
	char str[STR_LENGTH];	//ïîäãðóæàåìàÿ ñòðîêà
	char *strpart;
	string tempstr;
	char* name;
	GameScene* gs = new GameScene(1);
	//ñòðîêà îáðàáîòêè
	ifstream dev(fileName);
	bool endRead=false;
	double wei=0,hei=0,x=0,y=0;
	while (!endRead){
		dev.getline(str,STR_LENGTH);
		strpart=strstr(str,"<rect");
		if (strpart==NULL){
			strpart=strstr(str,"</g");
			if (strpart!=NULL){
				endRead=true;
			}
		}else{
			dev.getline(str,STR_LENGTH);
			while (strstr(str,"<title")==NULL){
				tempstr=(const char*)str;
				if (strstr(str,"width=")!=NULL){
					int step=tempstr.find("\"")+1;
					tempstr=tempstr.substr(step,tempstr.length()-1-step);
					wei=atof(tempstr.c_str());
				}

				if (strstr(str,"height=")!=NULL){
					int step=tempstr.find("\"")+1;
					tempstr=tempstr.substr(step,tempstr.length()-1-step);
					hei=atof(tempstr.c_str());
				}
				if (strstr(str,"x=")!=NULL){
					int step=tempstr.find("\"")+1;
					tempstr=tempstr.substr(step,tempstr.length()-1-step);
					x=atof(tempstr.c_str());
				}
				if (strstr(str,"y=")!=NULL){
					int step=tempstr.find("\"")+1;
					tempstr=tempstr.substr(step,tempstr.length()-1-step);
					y=atof(tempstr.c_str());
				}

				dev.getline(str,STR_LENGTH);	//next line
			}
			dev.getline(str,STR_LENGTH);
			tempstr=(const char*)str;
			int step=tempstr.find(">")+1;
			tempstr=tempstr.substr(step,tempstr.find("<")-step);
			name= new char[tempstr.size()+1] ;
			strcpy(name, tempstr.c_str());

			sceneElem.width=wei;
			sceneElem.height=hei;
			sceneElem.x=x;
			sceneElem.y=y;
			sceneElem.name=name;
			addSpr(gs);
		}
	}
	dev.close();
	return gs;
}

GameScene* SceneLoader::loadScene(const char* sceneName)
{
	//base test
	layerNumber=0;
	char* allPathName=new char[strlen(sceneName)+strlen("/temp1.svg")+2];
	strcpy(allPathName,sceneName);
	strcat(allPathName,"/temp1.svg");

	return load(allPathName);
}

GameScene* SceneLoader::loadGameScene(std::string sceneName)
{
	GameSceneMap::iterator it = gameScenes.find(sceneName);
	if (it != gameScenes.end())
		return it->second;
	gameScenes[sceneName] = loadScene(sceneName.c_str());
	return gameScenes[sceneName];
}
