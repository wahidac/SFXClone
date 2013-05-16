#include "GLJoe.h"

using namespace std;
using namespace GLJoe;


// Uniforms
GLuint ModelView;
GLuint Projection;
GLuint LightPosition;
GLuint AmbientProduct;
GLuint DiffuseProduct;
GLuint SpecularProduct;
GLuint Shininess;
GLuint Tex;
GLuint EnableTex;
GLuint Offset;
GLuint IndividualRotation;
GLuint TextureTransform;

// Model view parameters
Vec3 initialEyePos;
GLfloat zoom;
GLint rotX;
GLint rotY;
GLfloat shiftX;
GLfloat shiftY;

// Cubes and their absolute positions
Cube cube1;
Cube cube2;
const Vec4 offsetCube1(-1, 0, 0, 0);
const Vec4 offsetCube2(1, 0, 0, 0);

// Timers for animation
int lastTime;
int newTime;
bool anim1; // rotation of the cubes
bool anim2; // animation of the textures

// Parameters for cube individual rotations
const float rateY1 = 60; // rate of cube1 around Y axis in rpm
const float rateX2 = 30; // rate of cube2 around X axis in rpm
Transform individualRotationY1;
Transform individualRotationX2;

// Rotation of the textures
const float rateTex1 = 60; // in rpm
const float rateTex2 = 1e-3; // translational increment
Transform2D textureTransform1;
Transform2D textureTransform2;

void initView()
{
	initialEyePos = Vec3(0, 0, 5);
	zoom = 1;
	rotX = 30;
	rotY = 15;
	shiftX = 0;
	shiftY = 0;
}

void init()
{
	// Load shaders and use the resulting program
	GLuint program = InitShader("vshader.glsl", "fshader.glsl");
	glUseProgram(program);
	
	// Get uniform locations
	ModelView = glGetUniformLocation(program, "ModelView");
	Projection = glGetUniformLocation(program, "Projection");
	LightPosition = glGetUniformLocation(program, "LightPosition");
	AmbientProduct = glGetUniformLocation(program, "AmbientProduct");
	DiffuseProduct = glGetUniformLocation(program, "DiffuseProduct");
	SpecularProduct = glGetUniformLocation(program, "SpecularProduct");
	Shininess = glGetUniformLocation(program, "Shininess");
	Tex = glGetUniformLocation(program, "Tex");
	EnableTex = glGetUniformLocation(program, "EnableTex");
	Offset = glGetUniformLocation(program, "Offset");
	IndividualRotation = glGetUniformLocation(program, "IndividualRotation");
	TextureTransform = glGetUniformLocation(program, "TextureTransform");
	
	// Set uniforms
	glUniform4f(AmbientProduct, 0.2, 0.2, 0.2, 1);
	glUniform4f(DiffuseProduct, 0.6, 0.6, 0.6, 1);
	glUniform4f(SpecularProduct, 0.2, 0.2, 0.2, 1);
	glUniform4f(LightPosition, 15, 15, 30, 0);
	glUniform1f(Shininess, 100);
	
	// Texture
	TgaImage img;
	const char pathImg[] = "texture.tga";
	if (!img.loadTGA(pathImg))
	{
		Error("Impossible to load image %s", pathImg);
	}
	cube1.setTexture(img);
	cube2.setTexture(img, Vec4(-0.5, 1.5, -0.5, 1.5),
		GL_REPEAT, GL_REPEAT, GL_LINEAR, GL_LINEAR_MIPMAP_LINEAR);
	
	glUniform1i(EnableTex, 1);
	
	// Generate the cubes
	cube1.generate();
	cube2.generate();
	
	// Initialize buffers
	cube1.initDraw(program);
	cube2.initDraw(program);
		
	// Initialize model view parameters
	initView();
	
	// Initialize timers and rotation parameters
	lastTime = newTime = glutGet(GLUT_ELAPSED_TIME);
	individualRotationY1.setIdentity();
	individualRotationX2.setIdentity();
	textureTransform1.setIdentity();
	textureTransform2.setIdentity();
	anim1 = false;
	anim2 = false;
	
	// Clear color
	glClearColor(0.1, 0.1, 0.2, 1);
	
	glEnable(GL_DEPTH_TEST);
}

void keyboard(unsigned char key, int x, int y)
{
	switch(key)
	{
	case 033: // Escape key
	case 'q':
	case 'Q': // Quit
		exit(EXIT_SUCCESS);
		break;
	case 'r': // restart the view
		initView();
		break;
	case '+':
	case 'i': // zoom in
		zoom *= 1.03;
		break;
	case '-':
	case 'o': // zoom out
		zoom /= 1.03;
		break;
	case 'h': // shift view left
		shiftX += 0.1;
		break;
	case 'u': // shift view up
		shiftY -= 0.1;
		break;
	case 'j': // shift view right
		shiftX -= 0.1;
		break;
	case 'n': // shift view down
		shiftY += 0.1;
		break;
	case 'p': // anim/resume animation
		anim1 = !anim1;
		break;
	case 'l':
		anim2 = !anim2;
		break;
	}
	
	glutPostRedisplay();
	
	(void)x, (void)y;
}

void mouse(int button, int state, int x, int y)
{
	if (button == 3) // wheel up
		zoom *= 1.03;
	else if (button == 4) // wheel down
		zoom /= 1.03;
		
	glutPostRedisplay();
	
	(void)state, (void)x, (void)y;
}

void special(int key, int x, int y)
{
	switch(key)
	{
	case GLUT_KEY_RIGHT:
		rotY += 3;
		break;
	case GLUT_KEY_LEFT:
		rotY -= 3;
		break;
	case GLUT_KEY_UP:
		rotX -= 3;
		break;
	case GLUT_KEY_DOWN:
		rotX += 3;
		break;
	}
	
	glutPostRedisplay();
	
	(void)x, (void)y;
}

void reshape(int width, int height)
{
	glViewport(0, 0, width, height);

    Mat4 projection = Perspective(50, (float) width / (float) height, 1, 1000);
    glUniformMatrix4fv(Projection, 1, GL_TRUE, projection);
}

void idle()
{
	newTime = glutGet(GLUT_ELAPSED_TIME);
	int elapsedTime = newTime - lastTime;
	lastTime = newTime;
	
	if (anim1)
	{
		// X rpm = X * 360 deg / 60,000 ms
		individualRotationY1.rotateY(360 * elapsedTime * rateY1 / 60000);
		individualRotationX2.rotateX(360 * elapsedTime * rateX2 / 60000);		
	}
	
	if (anim2)
	{
		textureTransform1 = Translate(0.5, 0.5) 
			* Rotate(rateTex1 * newTime * 360 / 60000)
			* Translate(-0.5, -0.5);
		textureTransform2.translate(0, elapsedTime * rateTex2);
	}
	
	glutPostRedisplay();
}

void display()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	// Model view
	Transform modelView;
		
	modelView.rotateX(rotX);
	modelView.rotateY(rotY);
	modelView.translate(-initialEyePos);
	modelView.scale(zoom);
	modelView.translate(shiftX, shiftY, 0);
	
	glUniformMatrix4fv(ModelView, 1, GL_TRUE, modelView);	
	
	// Draw cube 1
	glUniform4fv(Offset, 1, offsetCube1);
	glUniformMatrix4fv(IndividualRotation, 1, GL_TRUE,
		individualRotationY1);
	glUniformMatrix3fv(TextureTransform, 1, GL_TRUE, textureTransform1);
	cube1.draw();
	
	glUniform4fv(Offset, 1, offsetCube2);
	glUniformMatrix4fv(IndividualRotation, 1, GL_TRUE,
		individualRotationX2);
	glUniformMatrix3fv(TextureTransform, 1, GL_TRUE, textureTransform2);
	cube2.draw();
	
	glutSwapBuffers();
}

int main(int argc, char **argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
	glutInitWindowSize(512, 512);
	glutCreateWindow("Spinning Cube Madness");

	glewExperimental = GL_TRUE;
	glewInit();

	init();

	glutDisplayFunc(display);
	glutKeyboardFunc(keyboard);
	glutMouseFunc(mouse);
	glutSpecialFunc(special);
	glutReshapeFunc(reshape);
	glutIdleFunc(idle);

	glutMainLoop();
	return 0;
}

