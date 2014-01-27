#ifndef GAME_OBJECTS_BOX_H
#define GAME_OBJECTS_BOX_H

#include "Point.h"
#include "GameObject.h"

class Box : public GameObject
{
public:
    Box(Point position, Point size);
    ~Box();
private:

protected:

public:
    void draw(const GameTime* gameTime);
    void update(const GameTime* gameTime);
};

#endif // GAME_OBJECTS_BOX_H
