#include "GLJoe.h"
#include "enemy.h"

using namespace std;
using namespace GLJoe;


// Parameters for game
#define MAX_ENEMIES 100
const float APPEARANCE_RATE = 0.001;
const int POINTS_PER_KILL = 250;

// Parameters for view
const float ZOOM_OUT_FACTOR = 50.0;
const float EYE_POSITION_Z = 2.0;


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

// Model view parameters
Vec3 initialEyePos;
GLfloat zoom;
GLint rotX;
GLint rotY;
GLfloat shiftX;
GLfloat shiftY;

// Spaceship and enemies
Cube spaceship;
Transform transformSpaceship;

Enemy* enemies[MAX_ENEMIES] = {0};
int iEnemy; // index of last enemy created
int number; // number of living enemies
Vec4 offsetSpaceship;
int numberKilled = 0; // number of killed enemies

// Timers for animation
int lastTime;
int newTime;
int lastTimeEnemyAppeared;

// Program
GLuint program;

void initView()
{
	initialEyePos = Vec3(0, 0, EYE_POSITION_Z);
	zoom = 1.0 / ZOOM_OUT_FACTOR;
	rotX = 0;
	rotY = 0;
	shiftX = 0;
	shiftY = 0;
}

void init()
{
	// Load shaders and use the resulting program
	program = InitShader("vshader.glsl", "fshader.glsl");
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
	
	// Set uniforms
	glUniform4f(AmbientProduct, 0.2, 0.2, 0.2, 1);
	glUniform4f(DiffuseProduct, 0.6, 0.6, 0.6, 1);
	glUniform4f(SpecularProduct, 0.2, 0.2, 0.2, 1);
	glUniform4f(LightPosition, 15, 15, 30, 0);
	glUniform1f(Shininess, 100);
	
	// Generate the spaceship
	spaceship.generate();
	
	// Initialize buffers
	spaceship.initDraw(program);
	
	// Initialize position of the spaceship
	offsetSpaceship = Vec4(0, 0, 0, 0);
	transformSpaceship = RotateZ(45);
		
	// Initialize model view parameters
	initView();
	
	// Initialize timers
	lastTime = newTime = glutGet(GLUT_ELAPSED_TIME);
	
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
	case ' ': // space
		for (int i = 0; i < MAX_ENEMIES; ++i)
		{
			if (enemies[i])
			{
				Vec2 oe = enemies[i]->offset.xy();
				Vec2 os = offsetSpaceship.xy();
				if (os.x >= oe.x - 5 &&
					os.x <= oe.x + 5 &&
					os.y >= oe.y - 5 &&
					os.y <= oe.y + 5)
				{
					delete enemies[i];
					enemies[i] = 0;
					number--;
					numberKilled++;
				}
			}
		}
		break;
	}
	
	glutPostRedisplay();
	
	(void)x, (void)y;
}

void mouse(int button, int state, int x, int y)
{
	glutPostRedisplay();
	
	(void) button, (void)state, (void)x, (void)y;
}

void special(int key, int x, int y)
{
	glutPostRedisplay();
	
	switch (key)
	{
	case GLUT_KEY_RIGHT:
		if (offsetSpaceship.x < LIMIT_X_RIGHT)
			offsetSpaceship += Vec4(1, 0, 0, 0);
		break;
	case GLUT_KEY_LEFT:
		if (offsetSpaceship.x > LIMIT_X_LEFT)
			offsetSpaceship += Vec4(-1, 0, 0, 0);
		break;
	case GLUT_KEY_UP:
		if (offsetSpaceship.y < LIMIT_Y_RIGHT)
			offsetSpaceship += Vec4(0, 1, 0, 0);
		break;
	case GLUT_KEY_DOWN:
		if (offsetSpaceship.y > LIMIT_Y_LEFT)
			offsetSpaceship += Vec4(0, -1, 0, 0);
		break;
	}
	
	(void) key, (void)x, (void)y;
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
	int elapsedTimeSinceLastEnemyAppeared = newTime
		- lastTimeEnemyAppeared;
	lastTime = newTime;	
	
	if (number < MAX_ENEMIES && 
		APPEARANCE_RATE * elapsedTimeSinceLastEnemyAppeared > 1)
	{
		while (enemies[iEnemy])
			iEnemy++;
		delete enemies[iEnemy];
		enemies[iEnemy] = new Enemy(program);
		iEnemy = (iEnemy + 1) % MAX_ENEMIES;
		number++;
		lastTimeEnemyAppeared = newTime;
	}
	
	for (int i = 0; i < MAX_ENEMIES; ++i)
	{
		if (enemies[i])
		{
			if (enemies[i]->offset.z < Z_ENEMIES_STOP)
				enemies[i]->offset += Vec4(0, 0, elapsedTime / 1000.0 *
					enemies[i]->speed, 0);
		}
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
	
	// Draw spaceship
	glUniform1i(EnableTex, 0);
	glUniform4fv(Offset, 1, offsetSpaceship);
	glUniformMatrix4fv(IndividualRotation, 1, GL_TRUE,
		transformSpaceship);
	spaceship.draw();
	
	
	// Draw cube 1
	glUniform1i(EnableTex, 1);
	for (int i = 0; i < MAX_ENEMIES; ++i)
	{
		if (enemies[i])
		{			
			glUniform4fv(Offset, 1, enemies[i]->offset);
			glUniformMatrix4fv(IndividualRotation, 1, GL_TRUE,
				enemies[i]->transform);
			enemies[i]->cube.draw();
		}
	}
	
	unsigned char score[40];
	unsigned char energy[40];
	sprintf((char*) score, "Score: %d", POINTS_PER_KILL * numberKilled);
	sprintf((char*) energy, "Energy: 100%%");
	
	glColor3f(1, 1, 1);
	glRasterPos2f(0.5, 0.8);
	glutBitmapString(GLUT_BITMAP_HELVETICA_18, score);
	glRasterPos2f(-0.8, 0.8);
	glutBitmapString(GLUT_BITMAP_HELVETICA_18, energy);
	
	glutSwapBuffers();
}

int main(int argc, char **argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
	glutInitWindowSize(512, 512);
	glutCreateWindow("SFX Clone");

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

