#ifndef GAME_OBJECTS_CHARACTER_H
#define GAME_OBJECTS_CHARACTER_H

#include "../Point.h"
#include "../GameObject.h"
#include "../Sound.h"

class Character : public GameObject
{
public:
    Character(Point position, Point size);
    ~Character();
private:
	Sound* jumpSound;
protected:

public:
    void draw(const GameTime* gameTime);
    void update(const GameTime* gameTime);
};

#endif // GAME_OBJECTS_CHARACTER_H
