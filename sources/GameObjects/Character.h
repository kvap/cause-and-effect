#ifndef GAME_OBJECTS_CHARACTER_H
#define GAME_OBJECTS_CHARACTER_H

#include "../Sound.h"
#include "../Point.h"
#include "../GameObject.h"
#include "../graphics/Sprite.hpp"

class Character : public GameObject
{
public:
    Character(Point position, Point size);
    ~Character();
private:
	Sound* jumpSound;
	Sound* stepSound;
    Sprite *sprite;
    int frame; // 0-3 - running, 4-6 - idle
    bool look_right;
protected:

public:
    void draw(const GameTime* gameTime);
    void update(const GameTime* gameTime);
};

#endif // GAME_OBJECTS_CHARACTER_H
