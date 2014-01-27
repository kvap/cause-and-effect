#ifndef GAME_OBJECTS_CHARACTER2_H
#define GAME_OBJECTS_CHARACTER2_H

#include "Point.h"
#include "GameObject.h"
#include "graphics/Sprite.hpp"

class Character2 : public GameObject
{
public:
    Character2(Point position, Point size);
    ~Character2();
private:
    Sprite *sprite;
    int frame; // 0-3 - running, 4-6 - idle
    bool look_right;
protected:

public:
    void draw(const GameTime* gameTime);
    void update(const GameTime* gameTime);
};

#endif // GAME_OBJECTS_CHARACTER2_H
