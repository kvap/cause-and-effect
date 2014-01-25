#include "Box.h"
#include <GLFW/glfw3.h>

Box::Box(Point position, Point size)
    : GameObject(position, size)
{ }

Box::~Box()
{ }

void Box::draw(const GameTime* gameTime)
{
    glPushMatrix();
    glTranslatef(this->position.x, this->position.y, 0);
    glRotatef(this->getAngle(), 0, 0, 1);
    glBegin(GL_QUADS);
    {
        glColor3f(1, 1, 1);
        glVertex2f(this->size.x * 2, this->size.y * 2);
        glVertex2f(0, this->size.y * 2);
        glVertex2f(0, 0);
        glVertex2f(this->size.x * 2, 0);
    }
    glEnd();
    glPopMatrix();
}

void Box::update(const GameTime* gameTime)
{

}
