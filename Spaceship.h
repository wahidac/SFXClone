//
//  spaceship.h
//  Dev
//
//  Created by Wahid Chowdhury on 5/30/13.
//  Copyright (c) 2013 Wahid Chowdhury. All rights reserved.
//

#ifndef Dev_spaceship_h
#define Dev_spaceship_h

#include "GLJoe/GLJoe.h"
#include "ObjLoader/OBJObject.h"

using namespace GLJoe;


class Spaceship
{
public:
    Vec4 offset;
	float speed;
    //world to camera and object to world matrices
    Transform cMw, wMo;
    OBJObject *aircraftModel;
    bool isInMotion;
    bool isFlickering;
    char direction;
    
	Spaceship(OBJObject *aircraftModel, float speed, const Transform &cMw, const Transform &wMo) : aircraftModel(aircraftModel), speed(speed),
    cMw(cMw), wMo(wMo), isInMotion(false)
	{
        isFlickering = false;
    }
    ~Spaceship() {
    }
    void beginFlickering(int timeFlicker, int numTimesFlicker);
    void draw(); 
    void setInMotion(char dir);
    void stopShip();
    void updatePosition(float deltaTime);
    
private:
    //Just parameters to make the ship flicker when it's hit
    bool makeShipTransparent;
    int timeToToggleState;
    int timeBetweenToggle;
    int numTimesStateToggled;
    int numTimesToToggle;
};

#endif
