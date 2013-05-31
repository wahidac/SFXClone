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
    aircraftModel->cMw = cMw;
    aircraftModel->wMo = wMo;
    aircraftModel->drawSelf();
}

void Spaceship::setInMotion(char dir) {
    isInMotion = true;
    direction = dir;
}

void Spaceship::stopShip() {
    isInMotion = false;
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
            break;
        case 'D':
            offset += GLJoe::Vec4(0,-delta,0,0);
            wMo.translate(0,-delta,0);
            break;
        case 'L':
            offset += GLJoe::Vec4(-delta,0,0,0);
            wMo.translate(-delta,0,0);
            break;
        case 'R':
            offset += GLJoe::Vec4(delta,0,0,0);
            wMo.translate(delta,0,0);
            break;
        default:
            break;
    }
    
}