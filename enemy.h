/**
 * @file enemy.h
 */

#ifndef DEF_ENEMY_H
#define DEF_ENEMY_H

#include "GLJoe/GLJoe.h"
#include "OBJLoader/OBJObject.h"
#include "Explosion.h"

using namespace GLJoe;
using namespace std;

// Parameters
const float LIMIT_X_LEFT = -20.0;
const float LIMIT_X_RIGHT = 20.0;
const float LIMIT_Y_LEFT = -10.0;
const float LIMIT_Y_RIGHT = 20.0;
const float Z_APPEARANCE = -30.0; // distance enemies appear at
const float Z_ENEMIES_STOP = -10.0; // distance enemies stop at
const float SPEED_MIN = 1.0;
const float SPEED_MAX = 10.0;


class Enemy
{
public:
	Vec4 offset;
	float speed;
    //world to camera and object to world matrices
	Transform cMw, wMo; //Right now main just manipulates these directly to move enemies
    float scale;
    float enemyOrientation;

	Enemy(const GLuint& program, OBJObject *pEnemyType, Explosion *explosionOnDeath) : enemyType(pEnemyType), explosionOnDeath(explosionOnDeath)
	{
		offset = Vec4(Random(LIMIT_X_LEFT, LIMIT_X_RIGHT),
			Random(LIMIT_Y_LEFT, LIMIT_Y_RIGHT), Z_APPEARANCE, 0);
		cMw = Identity();
        wMo = Identity();
		speed = Random(SPEED_MIN, SPEED_MAX);
        isDead = false;
        
        explosionSpeed = 40;
        explosionCurrentScale = 1;
	}
    
    void killEnemy() {
        isDead = true;
        isAnimatingDeath = true;
        
        //Final size of the explosion is a function of how far away the enemy is
        explosionEndScale = 150/abs(offset.z);

        //Init to that of this enemy so explosion appears at same position
        explosioncMw = cMw;
        explosionwMo = wMo;
        explosionwMo.scale(explosionCurrentScale);
    }
    
    void draw() {
        if (isDead && isAnimatingDeath) {
            explosionOnDeath->cMw = explosioncMw;
            explosionOnDeath->wMo = explosionwMo;
            explosionOnDeath->draw();
        }
        if (!isDead) {
            enemyType->cMw = cMw;
            enemyType->wMo = wMo;
            enemyType->wMo.scale(scale);
            enemyType->wMo.rotateY(enemyOrientation);
            enemyType->drawSelf();
        }
    }
    
    void updateExplosionAnimation(float dtime) {
        
        if (explosionCurrentScale >= explosionEndScale) {
            isAnimatingDeath = false;
            return;
        }
        
        float deltaScale = dtime * explosionSpeed;
        float newScale = explosionCurrentScale + deltaScale;
        
        //Bring it back to its original size first
        explosionwMo.scale(1/explosionCurrentScale);
        explosionwMo.scale(newScale);
        
        explosionCurrentScale = newScale;
    }
    
    bool isAlive() {
        return !isDead;
    }
    
    bool animatingDeath() {
        return isAnimatingDeath;
    }
    
private:
    GLfloat explosionEndScale;
    GLfloat explosionCurrentScale;
    GLfloat explosionSpeed;
    bool isAnimatingDeath;
    //The explosion to show on death
    Explosion *explosionOnDeath;
    Transform explosioncMw,explosionwMo;
    OBJObject *enemyType;
    bool isDead;
};


#endif // DEF_ENEMY_H
