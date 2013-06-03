//
//  EnemyTypes.h
//  Dev
//
//  Created by Wahid Chowdhury on 5/28/13.
//  Copyright (c) 2013 Wahid Chowdhury. All rights reserved.
//

#ifndef __Dev__BulletTypes__
#define __Dev__BulletTypes__

#include <iostream>
#include "ObjLoader/OBJObject.h"

#define NUM_BULLET_TYPES 1

//Defines a collection of different bullet types. Just a way to group
//all the data that defines how bullets look.
class BulletTypes  {
    
public:
    BulletTypes(const OBJObjectShaderHandles &shaderHandles) {
        Transform cMw, wMo;
        
        //Default shader params in case the file has none defined (will make things red and shiny)
        OBJObjectParams defaults;
        defaults.material_ambient = GLJoe::Vec4(1,0,0,1);
        defaults.material_diffuse = GLJoe::Vec4(1,0,0,1);
        defaults.material_specular = GLJoe::Vec4(1,1,1,1);
        defaults.material_shininess = 300;
        
		bullets[0] = new OBJObject("Models/Bullet.obj", shaderHandles, cMw, wMo, &defaults);
		bullets[0]->initializeOpenGLBuffers();
       /* bullets[0] = new OBJObject("Models/Animals/pig.obj", shaderHandles, cMw, wMo, &defaults);
        bullets[0]->initializeOpenGLBuffers();
        bullets[1] = new OBJObject("Models/Animals/shark.obj", shaderHandles, cMw, wMo, &defaults);
        bullets[1]->initializeOpenGLBuffers();
        bullets[2] = new OBJObject("Models/Animals/eagle.obj", shaderHandles, cMw, wMo, &defaults);
        bullets[2]->initializeOpenGLBuffers();     
		bullets[3] = new OBJObject("Models/Other/couch.obj",shaderHandles, cMw, wMo, &defaults);
        bullets[3]->initializeOpenGLBuffers(); */    
    }
    
    ~BulletTypes() {
        for(int i = 0; i < NUM_BULLET_TYPES;i++) {
            delete bullets[i];
        }
    }
    
    OBJObject *bullets[NUM_BULLET_TYPES];
};





#endif /* defined(__Dev__EnemyTypes__) */
