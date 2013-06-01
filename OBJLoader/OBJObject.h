//
//  OBJObject.h
//  loadModels
//
//  Created by Wahid Chowdhury on 5/16/13.
//  Copyright (c) 2013 Wahid Chowdhury. All rights reserved.
//


#ifndef __loadModels__OBJObject__
#define __loadModels__OBJObject__

#include "GLJoe.h"
#include "objparser.h"


struct OBJObjectParams {
    GLJoe::Vec4 material_specular;
    GLJoe::Vec4 material_diffuse;
    GLJoe::Vec4 material_ambient;
    GLfloat material_shininess;
};

//All of the variables correspond to what glGetUniformLocation would return
struct OBJObjectShaderHandles {
    GLuint cMw;
    GLuint wMo;
    GLuint Projection;
    GLuint LightPosition;
    GLuint vMaterialAmbient;
    GLuint vMaterialDiffuse;
    GLuint vMaterialSpecular;
    GLuint vMaterialShininess;
    GLuint vNormal;
    GLuint vPosition;
	GLuint vTexCoords;
    GLuint LightAmbient;
    GLuint LightDiffuse;
    GLuint LightSpecular;
	GLuint tex;
	GLuint EnableTex;
	GLuint MoveTex;
	GLuint TexOffset;
};

class OBJObject {
    
public:
    OBJObject(const char *objFileName, const OBJObjectShaderHandles &shaderHandles, const GLJoe::Transform &cMw, const GLJoe::Transform &wMo, OBJObjectParams *defaultShaderParams);
    ~OBJObject() {
        delete [] vertices;
        delete [] normals;
        delete [] materials_specular;
        delete [] materials_diffuse;
        delete [] materials_ambient;
        delete [] materials_shininess;

        delete mesh;
    }
    void loadVerticesAndNormals();
    void loadMaterials();
    void initializeOpenGLBuffers();
    void setMaterialsToParams(OBJObjectParams *params);
    GLuint returnVAO() {
        return vao;
    }
    void drawSelf();
    
    GLJoe::Transform getcMw() {
        return cMw;
    }
    GLJoe::Transform getwMo() {
        return wMo;
    }
    void setcMw(const GLJoe::Mat4 &worldToCamera) {
        cMw = worldToCamera;
    }
    
    void setwMo(const GLJoe::Mat4 &objectToWorld) {
        wMo = objectToWorld;
    }
    
    //Directly manipulate to move the object around
    GLJoe::Transform cMw;
    GLJoe::Transform wMo;
    
private:
    GLuint vao; //The vertex attribute that defines this object;
    
    GLJoe::Vec4 *vertices;
    GLJoe::Vec3 *normals;
    GLJoe::Vec4 *materials_ambient;
    GLJoe::Vec4 *materials_diffuse;
    GLJoe::Vec4 *materials_specular;
    GLfloat *materials_shininess;
    OBJParser parser;
    OBJMesh* mesh;
    int numTriangles;
    int numVertices;
    OBJObjectShaderHandles shaderHandles;
    bool buffersInitialized;
};


#endif /* defined(__loadModels__OBJObject__) */
