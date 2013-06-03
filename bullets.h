/**
 * @file bullet.h
 */

#ifndef DEF_BULLET_H
#define DEF_BULLET_H

#include "GLJoe/GLJoe.h"
#include "ObjLoader/OBJObject.h"

using namespace GLJoe;
using namespace std;

// Parameters
const float START_Z_SHIP = -1.7;
const float START_Z_ENEMY = -9.1;
const float SPEED_MIN2 = 1.0;
const float SPEED_MAX2 = 10.0;


class Bullet
{
public:
	//Cube cube;
	Vec4 offset;
	GLfloat speed;
    //world to camera and object to world matrices
	Transform cMw, wMo;
    OBJObject *bulletType;

	Bullet(const GLuint& program, OBJObject *pEnemyType, GLfloat x, GLfloat y, bool ship) : bulletType(pEnemyType)
	{
		//offset = Vec4(Random(LIMIT_X_LEFT, LIMIT_X_RIGHT),
		//	Random(LIMIT_Y_LEFT, LIMIT_Y_RIGHT), Z_APPEARANCE, 0);
		if(ship)
			offset = Vec4(x,y,START_Z_SHIP,0);
		else
			offset = Vec4(x,y,START_Z_ENEMY,0);
		

		cMw = Identity();
        wMo = Identity();
		speed = -.5;

	}
    
    void draw() {
        bulletType->cMw = cMw;
        bulletType->wMo = wMo;
        bulletType->drawSelf();
    }

    
};


#endif // DEF_BULLET_H
