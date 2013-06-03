//
//  EnemyTypes.h
//  Dev
//
//  Created by Wahid Chowdhury on 5/28/13.
//  Copyright (c) 2013 Wahid Chowdhury. All rights reserved.
//

#ifndef __Dev__EnemyTypes__
#define __Dev__EnemyTypes__

#include <iostream>
#include "ObjLoader/OBJObject.h"
#include "OBJLoader/TexturedOBJObject.h"

#define NUM_ENEMY_TYPES 3

//Defines a collection of different enemy types. Just a way to group
//all the data that defines how enemies look.
class EnemyTypes  {
    
public:
    EnemyTypes(const OBJObjectShaderHandles &shaderHandles) {
        Transform cMw, wMo;
        
        //Default shader params in case the file has none defined (will make things red and shiny)
        OBJObjectParams defaults;
        defaults.material_ambient = GLJoe::Vec4(.1,.1,.1,1);
        defaults.material_diffuse = GLJoe::Vec4(.7,.7,.3,1);
        defaults.material_specular = GLJoe::Vec4(1,1,1,1);
        defaults.material_shininess = 300;
        
        enemies[0] = new OBJObject("Models/Animals/pig.obj", shaderHandles, cMw, wMo, &defaults);
        enemies[0]->initializeOpenGLBuffers();
        enemies[1] = new TexturedOBJObject("Images/waterTexture.png",1280,800,"Models/Animals/shark.obj", shaderHandles, cMw, wMo, &defaults);
        enemies[1]->initializeOpenGLBuffers();
         enemies[2] = new OBJObject("Models/Animals/eagle.obj", shaderHandles, cMw, wMo, &defaults);
        enemies[2]->initializeOpenGLBuffers();

    }
    
    ~EnemyTypes() {
        for(int i = 0; i < NUM_ENEMY_TYPES;i++) {
            delete enemies[i];
        }
    }
    
    OBJObject *enemies[NUM_ENEMY_TYPES];
};





#endif /* defined(__Dev__EnemyTypes__) */
