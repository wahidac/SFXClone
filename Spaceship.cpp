//
//  Spaceship.cpp
//  Dev
//
//  Created by Wahid Chowdhury on 5/30/13.
//  Copyright (c) 2013 Wahid Chowdhury. All rights reserved.
//

#include "Spaceship.h"


void Spaceship::draw()
{
    if (numTimesStateToggled >= numTimesToToggle ) {
        isFlickering = false;
        numTimesStateToggled = 0;
    }
    
    if (isFlickering) {
        int time = glutGet(GLUT_ELAPSED_TIME);
        
        //Is it time to toggle the state i.e blend w/ background vs. show ship normally?
        if (time >= timeToToggleState && isFlickering) {
            timeToToggleState = time + timeBetweenToggle;
            makeShipTransparent = !makeShipTransparent;
            numTimesStateToggled++;
        }
        
        if (makeShipTransparent) {
            //Turn on transparency
            OBJObjectShaderHandles shaderHandles = aircraftModel->getShaderHandles();
            
            glUniform1i(shaderHandles.blendModel, 1);
            glUniform1f(shaderHandles.alpha, .2);
            glEnable(GL_BLEND);
            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
            glDepthMask( GL_FALSE );
            
            aircraftModel->cMw = cMw;
            aircraftModel->wMo = wMo;
            aircraftModel->drawSelf();
            
            glDepthMask( GL_TRUE );
            glDisable(GL_BLEND);
            //Turn off transparency
            glUniform1i(shaderHandles.blendModel, 0);
        } else {
            //Draw the spaceship as before
            aircraftModel->cMw = cMw;
            aircraftModel->wMo = wMo;
            aircraftModel->drawSelf();
        }
        
    } else {
        
        //Draw the spaceship as before
        aircraftModel->cMw = cMw;
        aircraftModel->wMo = wMo;
        aircraftModel->drawSelf();
    }
}

void Spaceship::setInMotion(char dir) {
    isInMotion = true;
    direction = dir;
}

void Spaceship::stopShip() {
    isInMotion = false;
	resetRotation();
}

void Spaceship::updatePosition(float deltaTime) {
    if (!isInMotion) {
        return;
    }
    
    float delta = deltaTime * speed;
    
    switch (direction) {
        case 'U':
            offset += GLJoe::Vec4(0,delta,0,0);
            wMo.translate(0,delta,0);
            if (rotated != UP) {
            	resetRotation();
    	        wMo.rotateX(30);
				rotated = UP;         
            }
            break;
        case 'D':
            offset += GLJoe::Vec4(0,-delta,0,0);
            wMo.translate(0,-delta,0);
            if (rotated != DOWN) {
            	resetRotation();
            	wMo.rotateX(-30);
				rotated = DOWN;
            }            
            break;
        case 'L':
            offset += GLJoe::Vec4(-delta,0,0,0);
            wMo.translate(-delta,0,0);
            if (rotated != LEFT) {
            	resetRotation();
            	wMo.rotateZ(45);
				rotated = LEFT;
            }            
            break;
        case 'R':
            offset += GLJoe::Vec4(delta,0,0,0);
            wMo.translate(delta,0,0);
            if (rotated != RIGHT) {
            	resetRotation();
            	wMo.rotateZ(-45);
				rotated = RIGHT;
            }            
            break;
        default:
            break;
    }
    
}

void Spaceship::beginFlickering(int timeFlicker, int numTimesFlicker) {
    isFlickering = true;
    timeToToggleState = glutGet(GLUT_ELAPSED_TIME) + timeFlicker;
    timeBetweenToggle = timeFlicker;
    numTimesToToggle = numTimesFlicker;
}

void Spaceship::resetRotation()
{
	switch(rotated) {
		case NO:
			break;
		case UP:
			wMo.rotateX(-30);
			break;
		case DOWN:
			wMo.rotateX(30);
			break;
		case LEFT:
			wMo.rotateZ(-45);
			break;
		case RIGHT:
			wMo.rotateZ(45);
			break;
	}
	rotated = NO;
}
