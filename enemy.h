/**
 * @file enemy.h
 */

#ifndef DEF_ENEMY_H
#define DEF_ENEMY_H

#include "GLJoe.h"
#include "OBJObject.h"

using namespace GLJoe;
using namespace std;

// Parameters
const float LIMIT_X_LEFT = -50.0;
const float LIMIT_X_RIGHT = 50.0;
const float LIMIT_Y_LEFT = -50.0;
const float LIMIT_Y_RIGHT = 50.0;
const float Z_APPEARANCE = -30.0; // distance enemies appear at
const float Z_ENEMIES_STOP = -10.0; // distance enemies stop at
const float SPEED_MIN = 1.0;
const float SPEED_MAX = 10.0;


class Enemy
{
public:
	//Cube cube;
	Vec4 offset;
	float speed;
    //world to camera and object to world matrices
	Transform cMw, wMo;
    OBJObject *enemyType;

	Enemy(const GLuint& program, OBJObject *pEnemyType) : enemyType(pEnemyType)
	{
		offset = Vec4(Random(LIMIT_X_LEFT, LIMIT_X_RIGHT),
			Random(LIMIT_Y_LEFT, LIMIT_Y_RIGHT), Z_APPEARANCE, 0);
		cMw = Identity();
        wMo = Identity();
		speed = Random(SPEED_MIN, SPEED_MAX);
		/*cube.generate();
		
		// Texture
		TgaImage img;
		const char pathImg[] = "texture.tga";
		if (!img.loadTGA(pathImg))
		{
			Error("Impossible to load image %s", pathImg);
		}
		cube.setTexture(img);
		
		// Initialize buffers
		cube.initDraw(program);
         */
	}
    
    void draw() {
        enemyType->cMw = cMw;
        enemyType->wMo = wMo;
        enemyType->drawSelf();
    }

    
};


#endif // DEF_ENEMY_H
