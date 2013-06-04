#ifndef POWERUP_H
#define POWERUP_H

#include "GLJoe/GLJoe.h"
#include "OBJLoader/OBJObject.h"
#include "ImgLoader.h"

using namespace GLJoe;

#define IMG_WIDTH 1200
#define IMG_HEIGHT 1200

class Powerup
{
public:
	Powerup(char* displayImage, char* colorPickingImage, Vec3 samplingColor, OBJObjectShaderHandles &shaderHandles);
	void draw();
	void setLocation(Vec4 location);

	void useColorPicking();
	void disableColorPicking();

	Vec3 getColorSamplingColor();

	bool isVisible;
	bool colorPicking;

private:
	Vec4* plane_vertices;
	Vec2* tex_coords;
	Vec3* plane_normals;
	Vec4* material_ambient;
	Vec4* material_other;

	Vec3 colorSamplingColor;

	GLubyte image[IMG_WIDTH * IMG_HEIGHT * 3];
	GLubyte pickingImage[IMG_WIDTH * IMG_HEIGHT * 3];

	Transform cMw;
	Transform wMo;

	GLuint vao;

	OBJObjectShaderHandles handles;

	ImgLoader imgLoader;

	int num_vertices;
	GLfloat texture_offset;

	GLuint textures[2];

	void initializeOpenGLBuffers();
};

#endif
