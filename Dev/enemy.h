/**
 * @file enemy.h
 */

#ifndef DEF_ENEMY_H
#define DEF_ENEMY_H

#include "GLJoe.h"

using namespace GLJoe;
using namespace std;

class Enemy
{
public:
	Cube cube;
	Vec4 offset;
	float speed;
	Transform transform;
	
	Enemy(const GLuint& program)
	{
		offset = Vec4(Random(-50, 50), Random(-50, 50), -30, 0);
		transform = Identity();
		speed = Random(1, 10);
		cube.generate();
		
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
	}
};


#endif // DEF_ENEMY_H
