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
    if (isInMotion) {
        updatePosition();
    }
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

void Spaceship::updatePosition() {
    if (!isInMotion) {
        return;
    }
    
    switch (direction) {
        case 'U':
            offset += GLJoe::Vec4(0,speed,0,0);
            wMo.translate(0,speed,0);
            break;
        case 'D':
            offset += GLJoe::Vec4(0,-speed,0,0);
            wMo.translate(0,-speed,0);
            break;
        case 'L':
            offset += GLJoe::Vec4(-speed,0,0,0);
            wMo.translate(-speed,0,0);
            break;
        case 'R':
            offset += GLJoe::Vec4(speed,0,0,0);
            wMo.translate(speed,0,0);
            break;
        default:
            break;
    }
    
}