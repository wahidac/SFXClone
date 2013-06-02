#include "GLJoe/GLJoe.h"
#include "enemy.h"
#include "BulletType.h"
#include "bullets.h"
#include "EnemyTypes.h"
#include "Spaceship.h"
#include "OBJLoader/OBJObject.h"
#include "Background.h"
#include "Explosion.h"

//#define USE_AUDIO

#ifdef USE_AUDIO
#include <SFML/Audio.hpp>
#endif

using namespace std;
using namespace GLJoe;

// Parameters for game
#define MAX_ENEMIES 100
#define MAX_BULLETS 50
#define SPACESHIP_SPEED 25

const float APPEARANCE_RATE = 0.001;
const int POINTS_PER_KILL = 250;

// Parameters for view
const float ZOOM_OUT_FACTOR = 50.0;
const float EYE_POSITION_Z = 50.0;

//Lighting Parameters
Vec4 light_position(0,0,1,1);
Vec4 light_ambient(.8, .8, .8, 1.0 );
Vec4 light_diffuse( .8, .8,.8, 1.0 );
Vec4 light_specular( .8, .8, .8, 1.0 );


// Model view parameters
Vec3 initialEyePos;
GLfloat zoom;
GLint rotX;
GLint rotY;
GLfloat shiftX;
GLfloat shiftY;

// Background objects
Background* background;

// Explosion
Explosion* explosion;

// Spaceship and enemies
Spaceship* spaceship;
EnemyTypes* enemyTypes;
BulletTypes* bulletTypes;

//Specification of how the aircraft looks
OBJObject* aircraftModel;

Enemy* enemies[MAX_ENEMIES] = {0};
Bullet* bullets[MAX_BULLETS] = {0};
int iEnemy; // index of last enemy created
int iBullet; // index of last bullet created
int numBullets; // number of bullets currently in play
int number; // number of living enemies
int numberKilled = 0; // number of killed enemies

// Timers for animation
int lastTime;
int newTime;
int lastTimeEnemyAppeared;

// Program
GLuint program;

// Structure to store all the GLuints that reference variable locations in the shaders
OBJObjectShaderHandles shaderHandles;

#define DEFAULT_WINDOW_SIZE 512

#ifdef USE_AUDIO
// Sounds
sf::SoundBuffer bufferLaser;
sf::Sound soundLaser;
sf::SoundBuffer bufferExplosion;
sf::Sound soundExplosion;
sf::Music music;
#endif

void initView()
{
	initialEyePos = Vec3(0, 0, EYE_POSITION_Z);
	zoom = 1.0 / ZOOM_OUT_FACTOR;
	rotX = 0;
	rotY = 0;
	shiftX = 0;
	shiftY = 0;
}

void initShaderHandles() {
    shaderHandles.cMw = glGetUniformLocation(program, "cMw");
    shaderHandles.wMo = glGetUniformLocation( program, "wMo" );
    shaderHandles.Projection = glGetUniformLocation(program, "Projection");
    shaderHandles.LightAmbient = glGetUniformLocation(program, "LightAmbient");
    shaderHandles.LightDiffuse = glGetUniformLocation(program, "LightDiffuse");
    shaderHandles.LightSpecular = glGetUniformLocation(program, "LightSpecular");
    shaderHandles.LightPosition = glGetUniformLocation(program, "LightPosition");
    shaderHandles.vMaterialAmbient = glGetAttribLocation( program, "vMaterialAmbient" );
    shaderHandles.vMaterialDiffuse = glGetAttribLocation( program, "vMaterialDiffuse" );
    shaderHandles.vMaterialSpecular = glGetAttribLocation( program, "vMaterialSpecular" );
    shaderHandles.vMaterialShininess = glGetAttribLocation( program, "vMaterialShininess" );
    shaderHandles.vNormal = glGetAttribLocation( program, "vNormal" );
    shaderHandles.vPosition = glGetAttribLocation( program, "vPosition" );
	shaderHandles.vTexCoords = glGetAttribLocation( program, "vTexCoords" );
	shaderHandles.tex = glGetUniformLocation( program, "Tex" );
	shaderHandles.EnableTex = glGetUniformLocation( program, "EnableTex" );
	shaderHandles.MoveTex = glGetUniformLocation( program, "MoveTex" );
	shaderHandles.TexOffset = glGetUniformLocation( program, "TexOffset" );
    shaderHandles.isAnimatingExplosion = glGetUniformLocation(program, "isAnimatingExplosion");
}
void timerFunc(int val)
{
	//Function that gets called every 16ms (or 60 times per second)
	background->moveGroundTexture(0.0010);
	background->moveBuildings(1.6);
    
    
    //Wahid moved all the stuff previously in idle here because his machine was getting destroyed by the amount
    //of computation occuring there.
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
        
        //Pick a random enemy from our different types of enemies.
        int randomEnemyType = rand()%NUM_ENEMY_TYPES;
		enemies[iEnemy] = new Enemy(program,enemyTypes->enemies[randomEnemyType],explosion);
        
		iEnemy = (iEnemy + 1) % MAX_ENEMIES;
		number++;
		lastTimeEnemyAppeared = newTime;
		
		cout << "Creating enemy " << randomEnemyType << endl;
	}
	for(int i = 0; i < MAX_BULLETS; i++){
		if(bullets[i]){
			if(bullets[i]->offset.z > -30){
				bullets[i]->offset =bullets[i]->offset +  Vec4(0,0,bullets[i]->speed,0);
                
			}else{
				delete bullets[i];
                bullets[i] = 0;
                numBullets--;
                
                
			}
            
            
            
		}
        
        
	}
	for (int i = 0; i < MAX_ENEMIES; ++i)
    {
        if (enemies[i])
        {
            Vec3 oe = enemies[i]->offset.xyz();
            for(int j = 0; j < MAX_BULLETS; j++){
                if(bullets[j]){
                    Vec3 os = bullets[j]->offset.xyz();
                    if (os.x >= oe.x - 2 &&
                        os.x <= oe.x + 2 &&
                        os.y >= oe.y - 2 &&
                        os.y <= oe.y + 2 &&
                        (os.z >= oe.z - .25 && os.z <= oe.z + .25))
                    {
                        enemies[i]->killEnemy();
                       	delete bullets[j];
						bullets[j] = 0;
						numBullets--; 
#ifdef USE_AUDIO
                        
#ifdef __APPLE__
                        soundExplosion.play();
#else
                        soundExplosion.Play();
#endif
                        
#endif
                        
                    }
                }
            }
        }
    }
	for (int i = 0; i < MAX_ENEMIES; ++i)
	{
		if (enemies[i])
		{
            if (enemies[i]->isAlive()) {
                if (enemies[i]->offset.z < Z_ENEMIES_STOP)
                    enemies[i]->offset += Vec4(0, 0, elapsedTime / 1000.0 *
                                               enemies[i]->speed, 0);
            } else {
                
                if (!enemies[i]->animatingDeath()) {
                    //Done animating the explosion. We can free memory
                    delete enemies[i];
                    enemies[i] = 0;
                    number--;
                    numberKilled++;
                } else {
                    enemies[i]->updateExplosionAnimation(elapsedTime/1000.0);
                }
            }
            
		}
	}
    
    //Update the spaceship's position based on how much time has elapsed since the last time idle was called
    spaceship->updatePosition(elapsedTime/1000.0);
    
    

	glutPostRedisplay();

	//Recursively call the timer function to schedule it again
	glutTimerFunc(16, timerFunc, 0);
}


void init()
{
	// Load shaders and use the resulting program
    program = InitShader("vshader_apple.glsl", "fshader_apple.glsl");    
	glUseProgram(program);
    
    // Set the shader handles for OBJObjects to use
    initShaderHandles();
	
	// Set lighting parameters
    glUniform4fv( shaderHandles.LightPosition,1, light_position );
    glUniform4fv( shaderHandles.LightAmbient, 1, light_ambient );
    glUniform4fv( shaderHandles.LightDiffuse, 1, light_diffuse );
    glUniform4fv( shaderHandles.LightSpecular, 1, light_specular );
    
    
    // Initialize model view parameters
    initView();
    
    //world to camera and object to world matrices
    Transform cMw, wMo;
    
    cMw.translate(-initialEyePos);
    //wMo.rotateY(180);
	wMo.rotateX(15);
    wMo.scale(5);

	// Generate the spaceship
    aircraftModel = new OBJObject("Models/Aircraft/Arwing_001.obj", shaderHandles, cMw, wMo, NULL);
    //Initialize buffers before making any call to draw
    aircraftModel->initializeOpenGLBuffers();

    spaceship = new Spaceship(aircraftModel,SPACESHIP_SPEED,cMw,wMo);

    //Initialize Enemy vertices/normals/shader params/textures
    enemyTypes = new EnemyTypes(shaderHandles);
	bulletTypes = new BulletTypes(shaderHandles);
    
	//Initialize the background objects
	background = new Background("Images/mountainsky.png", "Images/grass2048.png", shaderHandles);
    
    //Initialize the explosion to show when an enemy dies
    explosion = new Explosion("Images/explosionTexture.png",shaderHandles);

	// Initialize timers
	lastTime = newTime = glutGet(GLUT_ELAPSED_TIME);
	
#ifdef USE_AUDIO

#ifdef __APPLE__  //apple
    // Sounds and music
	if (!bufferLaser.loadFromFile("Sounds/laser.wav"))
	{
		Error("Failed loading sound %s", "laser.wav");
	}
	soundLaser.setBuffer(bufferLaser);
    
    if (!bufferExplosion.loadFromFile("Sounds/explode2.wav"))
	{
		Error("Failed loading sound %s", "explode2.wav");
	}
	soundExplosion.setBuffer(bufferExplosion);
    
	if (!music.openFromFile("Sounds/music.ogg"))
	{
		Error("Failed loading music %s", "music.ogg");
	}
	music.setLoop(true);
	music.play();
#else
    // Sounds and music
	if (!bufferLaser.LoadFromFile("Sounds/laser.wav"))
	{
		Error("Failed loading sound %s", "laser.wav");
	}
	soundLaser.SetBuffer(bufferLaser);
    
    
    if (!bufferExplosion.LoadFromFile("Sounds/explode2.wav"))
	{
		Error("Failed loading sound %s", "explode2.wav");
	}
	soundExplosion.SetBuffer(bufferExplosion);
    
    
    
	if (!music.OpenFromFile("Sounds/music.ogg"))
	{
		Error("Failed loading music %s", "music.ogg");
	}
	music.SetLoop(true);
	music.Play();
#endif
    
#endif
	
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
#ifdef USE_AUDIO
            
#ifdef __APPLE__
	    soundLaser.play();
#else
        soundLaser.Play();
#endif
            
#endif

		iBullet = 0;
		while(bullets[iBullet])
			iBullet++;
		delete bullets[iBullet];
		int randomBulletType = rand()%NUM_BULLET_TYPES;
		bullets[iBullet] = new Bullet(program,bulletTypes->bullets[randomBulletType],spaceship->offset.x,spaceship->offset.y,true);
		numBullets++;
		cout << "firing bullet";


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
		if (spaceship->offset.x < LIMIT_X_RIGHT) {
            //Keep moving until user releases!
            spaceship->setInMotion('R');
        }
		break;
	case GLUT_KEY_LEFT:
		if (spaceship->offset.x > LIMIT_X_LEFT) {
            Vec4 dx(-.1, 0, 0, 0);
            spaceship->setInMotion('L');
        }
		break;
	case GLUT_KEY_UP:
		if (spaceship->offset.y < LIMIT_Y_RIGHT) {
            spaceship->setInMotion('U');
        }
		break;
	case GLUT_KEY_DOWN:
		if (spaceship->offset.y > LIMIT_Y_LEFT) {
            spaceship->setInMotion('D');
        }
		break;
	}
    
	
	(void) key, (void)x, (void)y;
}

void specialKeyReleased(int key, int x, int y) {
    glutPostRedisplay();

    switch (key)
	{
        case GLUT_KEY_RIGHT:
            if (spaceship->isInMotion && spaceship->direction =='R') {
                spaceship->stopShip();
            }
            break;
        case GLUT_KEY_LEFT:
            if (spaceship->isInMotion && spaceship->direction =='L') {
                spaceship->stopShip();
            }
            break;
        case GLUT_KEY_UP:
            if (spaceship->isInMotion && spaceship->direction =='U') {
                spaceship->stopShip();
            }
            break;
        case GLUT_KEY_DOWN:
            if (spaceship->isInMotion && spaceship->direction =='D') {
                spaceship->stopShip();
            }
            break;
	}
    
}

void reshape(int width, int height)
{
	glViewport(0, 0, width, height);

    Mat4 projection = Perspective(50, (float) width / (float) height, 1, 1000);
    glUniformMatrix4fv(shaderHandles.Projection, 1, GL_TRUE, projection);

	background->resize(width, height);
}

/*void idle()
{
	
    
	
	glutPostRedisplay();
}*/

void display()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	// Draw background
	background->draw();
       
	//glUniform1i(EnableTex, 1);
	for(int j = 0; j < MAX_BULLETS; j++){
		if(bullets[j]){
			bullets[j]->cMw = Translate(-initialEyePos);
			bullets[j]->wMo = Translate(bullets[j]->offset);


			bullets[j]->draw();




		}

	}
	for (int i = 0; i < MAX_ENEMIES; ++i)
	{
		if (enemies[i])
		{
            enemies[i]->cMw = Translate(-initialEyePos);
            enemies[i]->wMo = Translate(enemies[i]->offset);
            
            if (enemies[i]->animatingDeath()) {
                //Turn on transparency for the explosion
                glUniform1i(shaderHandles.isAnimatingExplosion, 1);
                glEnable(GL_BLEND);
                glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
                glDepthMask( GL_FALSE );
                enemies[i]->draw();                
                glDepthMask( GL_TRUE );
                glDisable(GL_BLEND);
                //Turn off transparency
                glUniform1i(shaderHandles.isAnimatingExplosion, 0);
            } else {
                enemies[i]->draw();
            }
		}
	}
    
    // Draw spaceship
	glUniform1i(glGetUniformLocation(program, "EnableTex"), 0);
    spaceship->draw();
	

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
	glutInitWindowSize(DEFAULT_WINDOW_SIZE, DEFAULT_WINDOW_SIZE);
	glutCreateWindow("SFX Clone");

	glewExperimental = GL_TRUE;
	glewInit();

	init();

	glutDisplayFunc(display);
	glutKeyboardFunc(keyboard);
	glutMouseFunc(mouse);
	glutSpecialFunc(special);
	glutReshapeFunc(reshape);
	//glutIdleFunc(idle);
    glutSpecialUpFunc(specialKeyReleased);
	glutTimerFunc(16, timerFunc, 0);
    
	glutMainLoop();
	return 0;
}

