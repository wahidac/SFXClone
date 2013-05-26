//
//  OBJObject.h
//  loadModels
//
//  Created by Wahid Chowdhury on 5/16/13.
//  Copyright (c) 2013 Wahid Chowdhury. All rights reserved.
//

#ifndef __loadModels__OBJObject__
#define __loadModels__OBJObject__

#include "Angel.h"
#include "objparser.h"
#endif /* defined(__loadModels__OBJObject__) */




class OBJObject {
    
public:
    OBJObject(char *objFileName,GLuint program);
    ~OBJObject() {
        delete [] vertices;
        delete [] normals;
        delete mesh;
    }
    void loadVerticesNormalsMaterials();
    void initializeOpenGLBuffers();
    GLuint returnVAO() {
        return vao;
    }
    void drawSelf();
    //Private members = point arrays, normal array for shading, maybe texture stuff
    
    
private:
    GLuint vao; //The vertex attribute that defines this object;
    vec4 *vertices;
    vec3 *normals;
    vec4 *materials_ambient;
    vec4 *materials_diffuse;
    vec4 *materials_specular;
    GLfloat *materials_shininess;
    OBJParser parser;
    OBJMesh* mesh;
    int numTriangles;
    int numVertices;
    GLuint program;
};