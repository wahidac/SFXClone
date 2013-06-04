#include "GLJoe/GLJoe.h"
#include "enemy.h"
#include "BulletType.h"
#include "bullets.h"
#include "EnemyTypes.h"
#include "Spaceship.h"
#include "OBJLoader/OBJObject.h"
#include "Background.h"
#include "Explosion.h"
#include "powerup.h"
#include <cstring>


#ifndef _WINDOWS_
#define USE_AUDIO
#endif

#ifdef USE_AUDIO
#include <SFML/Audio.hpp>
#endif

using namespace std;
using namespace GLJoe;

// Parameters for game
#define MAX_ENEMIES 100
#define MAX_BULLETS 50
#define MAX_ENEMY_BULLETS 15
#define SPEED_BARREL 1
#define SPACESHIP_SPEED 25
#define ENEMY_SCALE 3


#define POINTS_FOR_POWERUP 10000

const float BULLET_APPEARANCE_RATE = 0.0005; // max frequency at which enemy can shoot
const float ENEMY_NICENESS = 100; // each enemy will shoot with proba 1 / ENEMY_NICENESS
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

// Powerup object
Powerup* powerup;

// Explosion
Explosion* explosion;

// Spaceship and enemies
Spaceship* spaceship;
EnemyTypes* enemyTypes;
BulletTypes* bulletTypes;
BulletTypes* enemyBulletTypes;

//Specification of how the aircraft looks
OBJObject* aircraftModel;

Enemy* enemies[MAX_ENEMIES] = {0};
Bullet* bullets[MAX_BULLETS] = {0};
Bullet* enemybullets[MAX_ENEMY_BULLETS] = {0};
int iEnemy; // index of last enemy created
int iBullet; // index of last bullet created
int numBullets; // number of bullets currently in play
int numEnemyBullets;
int shipHealth;
int number; // number of living enemies
int numberKilled = 0; // number of killed enemies
bool barrelRoll = false;
int degreesRotated = 0;
int zOrRCounter = 0;

// Timers for animation
int lastTime;
int newTime;
int lastTimeEnemyAppeared;

//Window parameters
int windowWidth;
int windowHeight;

// Program
GLuint program;

// Structure to store all the GLuints that reference variable locations in the shaders
OBJObjectShaderHandles shaderHandles;

//Game over params
bool gameOver = false;
int finalScore;

#define DEFAULT_WINDOW_SIZE 512

#ifdef USE_AUDIO
// Sounds
sf::SoundBuffer bufferLaser;
sf::Sound soundLaser;
sf::SoundBuffer bufferExplosion;
sf::Sound soundExplosion;
sf::SoundBuffer bufferWounded;
sf::Sound soundWounded;
sf::SoundBuffer bufferRoll;
sf::Sound soundRoll;
sf::SoundBuffer bufferGameOver;
sf::Sound soundGameOver;
sf::Music music;
#endif
bool soundGameOverPlayed = false;

void display();

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
    shaderHandles.blackTransparent = glGetUniformLocation(program, "blackTransparent");
    shaderHandles.calculateTexCoordInShader = glGetUniformLocation(program, "calculateTexCoordInShader");
    shaderHandles.alpha = glGetUniformLocation(program, "alpha");
    shaderHandles.blendModel = glGetUniformLocation(program, "blendModel");
	shaderHandles.useColorPicking = glGetUniformLocation(program, "useColorPicking");
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
	

	if(barrelRoll){
		zOrRCounter = 0;
		if(degreesRotated < 360){
			spaceship->wMo.rotateZ(elapsedTime * SPEED_BARREL);
			degreesRotated += elapsedTime * SPEED_BARREL;
#ifdef USE_AUDIO
                        
#ifdef __APPLE__
                        if (soundRoll.getStatus() != sf::Sound::Playing)
                        	soundRoll.play();
#else
                        if (soundRoll.GetStatus() != sf::Sound::Playing)
                        	soundRoll.Play();
#endif
                        
#endif
			if (degreesRotated > 360) {
				spaceship->wMo.rotateZ(360 - degreesRotated);
				degreesRotated = 0;
				barrelRoll = false;
				spaceship->barrelRoll = false;
				
			}
		}else{
			degreesRotated = 0;
			barrelRoll = false;
			spaceship->barrelRoll = false;
		}



	}
	if (number < MAX_ENEMIES &&
		APPEARANCE_RATE * elapsedTimeSinceLastEnemyAppeared > 1)
	{
		while (enemies[iEnemy])
			iEnemy++;
		delete enemies[iEnemy];
        
        //Pick a random enemy from our different types of enemies.
        int randomEnemyType = rand()%NUM_ENEMY_TYPES;
		enemies[iEnemy] = new Enemy(program,enemyTypes->enemies[randomEnemyType],explosion);
        
        //Set enemy orientation so enemy faces us!
        float orientation = 0;
        switch (randomEnemyType) {
            case 0:
                orientation = -90.0;
                break;
            case 1:
                orientation = 90;
                break;
            case 2:
                orientation = 0;
                break;
            default:
                break;
        }
        enemies[iEnemy]->enemyOrientation = orientation;
        
		iEnemy = (iEnemy + 1) % MAX_ENEMIES;
		number++;
		lastTimeEnemyAppeared = newTime;
		
		cout << "Creating enemy " << randomEnemyType << endl;
	}
	for(int i = 0; i < MAX_BULLETS; i++){
		if(bullets[i]){
			if(bullets[i]->offset.z > -30){
				bullets[i]->offset =bullets[i]->offset +  Vec4(0,0,bullets[i]->speed * elapsedTime / 10,0);
                
			}else{
				delete bullets[i];
                bullets[i] = 0;
                numBullets--;
                
                
			}
            
		}
        
        
	}
	for(int i = 0; i < MAX_ENEMY_BULLETS; i++){
		if(enemybullets[i]){
			if(enemybullets[i]->offset.z < 10){
				enemybullets[i]->offset =enemybullets[i]->offset -  Vec4(0,0,enemybullets[i]->speed * elapsedTime / 10.0,0);
                Vec3 oe = enemybullets[i]->offset.xyz();
				Vec3 os = spaceship->offset.xyz();
				if (os.x -2 <= oe.x  &&
                        os.x+ 2 >= oe.x  &&
                        os.y- 2 <= oe.y  &&
                        os.y+ 2 >= oe.y  &&
                        (os.z- .95 <= oe.z  && os.z + .95 >= oe.z)){
					shipHealth--;
					cout << "ship health is now: " << shipHealth << endl;
                    spaceship->beginFlickering(500, 5);
                    
					delete enemybullets[i];
					enemybullets[i] = 0;
					numEnemyBullets--;
                    
                    
                    spaceship->beginFlickering(100, 15);
                    
#ifdef USE_AUDIO
                        
#ifdef __APPLE__
                        soundWounded.play();
#else
                        soundWounded.Play();
#endif
                        
#endif
                    
                    if (shipHealth <= 0 && !gameOver) {
                        gameOver = true;
                        finalScore = POINTS_PER_KILL * numberKilled;
                    }

				}
						

			}else{
				delete enemybullets[i];
                enemybullets[i] = 0;
                numEnemyBullets--;
                
                
			}
            
            
            
		}


	}
	for (int i = 0; i < MAX_ENEMIES; ++i)
    {
        if (enemies[i])
        {

			if(numEnemyBullets < MAX_ENEMY_BULLETS && 
				BULLET_APPEARANCE_RATE * elapsedTimeSinceLastEnemyAppeared &&
				Random(1,ENEMY_NICENESS) < 3){
				iBullet = 0;
				while(enemybullets[iBullet])
					iBullet++;
				delete enemybullets[iBullet];
				int randomBulletType = rand()%NUM_BULLET_TYPES;
				enemybullets[iBullet] = new Bullet(program,enemyBulletTypes->bullets[randomBulletType],enemies[i]->offset.x,enemies[i]->offset.y,false);
				numEnemyBullets++;

			}
            Vec3 oe = enemies[i]->offset.xyz();
            for(int j = 0; j < MAX_BULLETS; j++){
                if(bullets[j]){
                    Vec3 os = bullets[j]->offset.xyz();
                    if (os.x >= oe.x - 3 &&
                        os.x <= oe.x + 3 &&
                        os.y >= oe.y - 3 &&
                        os.y <= oe.y + 3 &&
                        (os.z >= oe.z - .55 && os.z <= oe.z + .55))
                    {
						//Check to see if enough points gotten for a powerup
						if(((numberKilled + 1) * POINTS_PER_KILL) % POINTS_FOR_POWERUP == 0)
						{
							//If so, display one only if one isn't already shown
							if(powerup->isVisible == false)
							{
								powerup->setLocation(enemies[i]->offset);
								powerup->isVisible = true;
							}
						}

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
	glutTimerFunc(24, timerFunc, 0);
}


void init()
{
	// Load shaders and use the resulting program
    program = InitShader("vshader_apple.glsl", "fshader_apple.glsl");    
	glUseProgram(program);
    shipHealth = 10;
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
	spaceship->barrelRoll = false;
    //Initialize Enemy vertices/normals/shader params/textures
    enemyTypes = new EnemyTypes(shaderHandles);
	bulletTypes = new BulletTypes(shaderHandles, true);
	enemyBulletTypes = new BulletTypes(shaderHandles, false);
    
	//Initialize the background objects
	background = new Background("Images/mountainsky.png", "Images/grass2048.png", shaderHandles);
    
	//Initialize the powerup object
	powerup = new Powerup("Images/powerup.png", "Images/colorMappedPowerup.png", Vec3(255.0, 255.0, 0.0), shaderHandles);

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
	soundLaser.setVolume(50);
    
    if (!bufferExplosion.loadFromFile("Sounds/explode2.wav"))
	{
		Error("Failed loading sound %s", "explode2.wav");
	}
	soundExplosion.setBuffer(bufferExplosion);
	soundExplosion.setVolume(100);     
	
	if (!bufferGameOver.loadFromFile("Sounds/gameover.wav"))
	{
		Error("Failed loading sound %s", "gameover.wav");
	}
	soundGameOver.setBuffer(bufferGameOver);
	soundGameOver.setVolume(100);  
    
    if (!bufferWounded.loadFromFile("Sounds/boom.wav"))
	{
		Error("Failed loading sound %s", "boom.wav");
	}
	soundWounded.setBuffer(bufferWounded);
	soundWounded.SetPitch(2);
	soundWounded.setVolume(100);    
    
    if (!bufferRoll.loadFromFile("Sounds/roll.ogg"))
	{
		Error("Failed loading sound %s", "explode2.wav");
	}
	soundRoll.setBuffer(bufferRoll);
	soundRoll.setVolume(100);    
    
	if (!music.openFromFile("Sounds/music.ogg"))
	{
		Error("Failed loading music %s", "music.ogg");
	}
	music.setVolume(50);
	music.setLoop(true);
	music.play();
#else
    // Sounds and music
	if (!bufferLaser.LoadFromFile("Sounds/laser.wav"))
	{
		Error("Failed loading sound %s", "laser.wav");
	}
	soundLaser.SetBuffer(bufferLaser);
	soundLaser.SetVolume(50);
    
    
    if (!bufferExplosion.LoadFromFile("Sounds/explode2.wav"))
	{
		Error("Failed loading sound %s", "explode2.wav");
	}
	soundExplosion.SetBuffer(bufferExplosion);
	soundExplosion.SetVolume(100);  
	
	if (!bufferGameOver.LoadFromFile("Sounds/gameover.wav"))
	{
		Error("Failed loading sound %s", "gameover.wav");
	}
	soundGameOver.SetBuffer(bufferGameOver);
	soundGameOver.SetVolume(100);  
    
    if (!bufferRoll.LoadFromFile("Sounds/roll.ogg"))
	{
		Error("Failed loading sound %s", "explode2.wav");
	}
	soundRoll.SetBuffer(bufferRoll);
	soundRoll.SetVolume(100);    
    
    
    if (!bufferWounded.LoadFromFile("Sounds/boom.wav"))
	{
		Error("Failed loading sound %s", "boom.wav");
	}
	soundWounded.SetBuffer(bufferWounded);
	soundWounded.SetPitch(2);
	soundWounded.SetVolume(100);    
    
    
	if (!music.OpenFromFile("Sounds/music.ogg"))
	{
		Error("Failed loading music %s", "music.ogg");
	}
	music.SetVolume(50);
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
	case 'b':
		spaceship->barrelRoll = true;
		barrelRoll = true;
		break;
	case 'z':
	case 'r':
		if(zOrRCounter == 1){
			barrelRoll = true;
			spaceship->barrelRoll = true;
		}else
			zOrRCounter++;
		break;
	case ' ': // space
            
        if (!gameOver) {
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

	}
	
	glutPostRedisplay();
	
	(void)x, (void)y;
}

void mouse(int button, int state, int x, int y)
{
	//if powerup is on the screen and mouse is clicked redraw everything with simple colors
	if(powerup->isVisible && button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
	{
		glUniform1i(shaderHandles.useColorPicking, 1);

		//Alert everything that is pick-able here
		powerup->useColorPicking();

		display();

		unsigned char sample[3];
		glReadBuffer(GL_BACK);

		GLint viewport[4];
		glGetIntegerv(GL_VIEWPORT, viewport);
		glReadPixels(x, viewport[3] - y, 1, 1, GL_RGB, GL_UNSIGNED_BYTE, sample);
		Vec3 colorSample = Vec3(sample[0], sample[1], sample[2]);
		Vec3 powerupColor = powerup->getColorSamplingColor();

		if(colorSample.x == powerupColor.x && colorSample.y == powerupColor.y && colorSample.z == powerupColor.z)
		{
			for(int i = 0; i< MAX_ENEMIES; i++)
				if(enemies[i])
				{
					enemies[i]->killEnemy();
				}
			powerup->isVisible = false;
		}

		//Alert everything that is pick-able to no longer use picking mode again here
		powerup->disableColorPicking();

		glUniform1i(shaderHandles.useColorPicking, 0);
	}

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

    windowWidth = width;
    windowHeight = height;
	background->resize(width, height);
}


// This prints a string to the screen
void Sprint( int x, int y, char *st, int stringLength)
{    
    int l,i;
    
    l=stringLength; // see how many characters are in text string.
    glWindowPos2d( x, y); // location to start printing text
    for( i=0; i < l; i++) // loop until i is greater then l
    {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, st[i]); // Print a character on the screen
    }
    
}

void display()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
	// Draw background
	background->draw();
    
    if (gameOver) {
        glClearColor(0.0, 0.0, 0.0, 1);
        char gameOverMessage[] = "Game Over";
        char score[40];
        sprintf((char*) score, "Final Score: %d", finalScore);
        Sprint(windowWidth/2-35,windowHeight/2,gameOverMessage,strlen(gameOverMessage));
        Sprint(windowWidth/2-35,windowHeight/2-20,score,strlen(score));
        glutSwapBuffers();
        
        if (soundGameOverPlayed == false) {
		    #ifdef USE_AUDIO
		        
		    #ifdef __APPLE__
		        soundGameOver.play();
		        soundLaser.setVolume(0);
		        soundWounded.setVolume(0);
		    #else
		        soundGameOver.Play();
		        soundLaser.SetVolume(0);
		        soundWounded.SetVolume(0);
		    #endif
		        
		    #endif
		    soundGameOverPlayed = true;
        }
        return;
    }

	//Draw powerups if one is available
	if(powerup->isVisible)
	{
		powerup->draw();
	}
    
	//glUniform1i(EnableTex, 1);
	for(int j = 0; j < MAX_BULLETS; j++){
		if(bullets[j]){
			bullets[j]->cMw = Translate(-initialEyePos);
			bullets[j]->wMo = Translate(bullets[j]->offset);


			bullets[j]->draw();

		}

	}
	for(int j = 0; j < MAX_ENEMY_BULLETS; j++){
		if(enemybullets[j]){
			enemybullets[j]->cMw = Translate(-initialEyePos);
			enemybullets[j]->wMo = Translate(enemybullets[j]->offset);


			enemybullets[j]->draw();




		}

	}
	for (int i = 0; i < MAX_ENEMIES; ++i)
	{
		if (enemies[i])
		{
            enemies[i]->cMw = Translate(-initialEyePos);
            enemies[i]->wMo = Translate(enemies[i]->offset);
            enemies[i]->scale = ENEMY_SCALE;
            
            if (enemies[i]->animatingDeath()) {
                //Turn on transparency for the explosion
                glUniform1i(shaderHandles.blackTransparent, 1);
                glEnable(GL_BLEND);
                glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
                glDepthMask( GL_FALSE );
                enemies[i]->draw();                
                glDepthMask( GL_TRUE );
                glDisable(GL_BLEND);
                //Turn off transparency
                glUniform1i(shaderHandles.blackTransparent, 0);
            } else {
                enemies[i]->draw();
            }
		}
	}
    
    // Draw spaceship
	glUniform1i(glGetUniformLocation(program, "EnableTex"), 0);
    spaceship->draw();
	
	unsigned char score[40];
	unsigned char health[40];
	sprintf((char*) score, "Score: %d", POINTS_PER_KILL * numberKilled);
	sprintf((char*) health, "Health: %d",shipHealth);
	
	glColor3f(1, 1, 1);
    //Print score
    Sprint(windowWidth-175,windowHeight-20,(char *)score,strlen((const char*)score));
    Sprint(windowWidth-175,windowHeight-40,(char *)health,strlen((const char*)health));

	if(powerup->colorPicking == false)
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
    glutSpecialUpFunc(specialKeyReleased);
	glutTimerFunc(24, timerFunc, 0);
    
	glutMainLoop();
	return 0;
}

