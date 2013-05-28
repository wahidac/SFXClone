//
//  OBJObject.cpp
//  loadModels
//
//  Created by Wahid Chowdhury on 5/16/13.
//  Copyright (c) 2013 Wahid Chowdhury. All rights reserved.
//

#include "OBJObject.h"


OBJObject::OBJObject(char *objFileName, const OBJObjectShaderHandles &shaderHandles, const GLJoe::Transform &cMw,
                    const GLJoe::Transform &wMo, OBJObjectParams *defaultShaderParams): shaderHandles(shaderHandles),
                    cMw(cMw), wMo(wMo)
{
    mesh = parser.load(objFileName);
    numTriangles = mesh->getTriangles().size();
    numVertices = numTriangles*3; //Each triangle has 3 vertices
    
    //Allocate memory

    vertices = new GLJoe::Vec4[numVertices];
    normals = new GLJoe::Vec3[numVertices];
    
    materials_specular = new GLJoe::Vec4[numVertices];
    materials_diffuse= new GLJoe::Vec4[numVertices];
    materials_ambient = new GLJoe::Vec4[numVertices];
    materials_shininess = new GLfloat[numVertices];
    
    //Extract data from GLM structs and store them in the arrays defined above so we can
    //send the data to the shaders
    
    //If no materials defined, use the default parameters
    if (!mesh->mModel->nummaterials) {
        setMaterialsToParams(defaultShaderParams);
        loadVerticesAndNormals();
    } else {
        //Load both material parameters and normals/vertex positions
        loadMaterials();
        loadVerticesAndNormals();
    }

    //Copy necessary data to GPU
    initializeOpenGLBuffers();
}

void OBJObject::setMaterialsToParams(OBJObjectParams *params) {
    for (int i = 0; i < numVertices; i++) {
        materials_specular[i] = params->material_specular;
        materials_diffuse[i] = params->material_diffuse;
        materials_ambient[i] = params->material_ambient;
        materials_shininess[i] = params->material_shininess;
    }
}


void OBJObject::loadVerticesAndNormals() {
    //Get a list of all the trianges that make up the objet
    std::vector<OBJMeshTriangle>displayList = mesh->getTriangles();

    for (int i = 0; i < numTriangles; i++) {
        //Processing one triangle
        
        OBJMeshTriangle triangle = displayList[i];
        
        GLJoe::Vec4 vertex = GLJoe::Vec4(triangle.v1.x,triangle.v1.y,triangle.v1.z,1);
        GLJoe::Vec3 normal = GLJoe::Vec3(triangle.n1.i,triangle.n1.j,triangle.n1.k);
        vertices[i*3] = vertex;
        normals[i*3] = normal;
        
        vertex = GLJoe::Vec4(triangle.v2.x,triangle.v2.y,triangle.v2.z,1);
        normal = GLJoe::Vec3(triangle.n2.i,triangle.n2.j,triangle.n2.k);
        vertices[i*3+1] = vertex;
        normals[i*3+1] = normal;

        vertex = GLJoe::Vec4(triangle.v3.x,triangle.v3.y,triangle.v3.z,1);
        normal = GLJoe::Vec3(triangle.n3.i,triangle.n3.j,triangle.n3.k);
        vertices[i*3+2] = vertex;
        normals[i*3+2] = normal;
    }
}

void OBJObject::loadMaterials() {
    //Get a list of all the trianges that make up the objet
    std::vector<OBJMeshTriangle>displayList = mesh->getTriangles();
    
    for (int i = 0; i < numTriangles; i++) {
        //Processing one triangle
        
        OBJMeshTriangle triangle = displayList[i];
        GLMmaterial* material =  triangle.material;
        
        GLJoe::Vec4 material_ambient = GLJoe::Vec4(material->ambient[0],material->ambient[1],material->ambient[2],material->ambient[3]);
        GLJoe::Vec4 material_diffuse = GLJoe::Vec4(material->diffuse[0],material->diffuse[1],material->diffuse[2],material->diffuse[3]);
        GLJoe::Vec4 material_specular = GLJoe::Vec4(material->specular[0],material->specular[1],material->specular[2],material->specular[3]);
        GLfloat shininess = material->shininess;
        
        materials_ambient[i*3] = material_ambient;
        materials_diffuse[i*3] = material_diffuse;
        materials_specular[i*3] = material_specular;
        materials_shininess[i*3] = shininess;
        
        materials_ambient[i*3+1] = material_ambient;
        materials_diffuse[i*3+1] = material_diffuse;
        materials_specular[i*3+1] = material_specular;
        materials_shininess[i*3+1] = shininess;
        
        materials_ambient[i*3+2] = material_ambient;
        materials_diffuse[i*3+2] = material_diffuse;
        materials_specular[i*3+2] = material_specular;
        materials_shininess[i*3+2] = shininess;
    }
}


void OBJObject::initializeOpenGLBuffers() {
    
#ifdef __APPLE__  //apple
    glGenVertexArraysAPPLE( 1, &vao );
    glBindVertexArrayAPPLE(vao);
#else
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
#endif
    
    
    //Copy vertex data to the GPU. Initialize one buffer to hold data for
    //normal vectors and vertex positions and another buffer to hold shader
    //parameters. For simplicity, I store shader parameters for each individual
    //vertex because for a given object loaded using the GLM library, it is possibly
    //for different parts of the object itself to have different shading parameters.
    
    GLuint buffer[2];
    glGenBuffers( 2, buffer );
    glBindBuffer( GL_ARRAY_BUFFER, buffer[0] );
    
    GLsizeiptr numBytesVertices = numVertices * sizeof(GLJoe::Vec4);
    GLsizeiptr numBytesNormals = numVertices * sizeof(GLJoe::Vec3);
    
    glBufferData( GL_ARRAY_BUFFER,numBytesNormals + numBytesVertices,
                 NULL, GL_STATIC_DRAW );
    glBufferSubData(GL_ARRAY_BUFFER, 0, numBytesNormals, normals);
    glBufferSubData(GL_ARRAY_BUFFER,numBytesNormals, numBytesVertices, vertices);

    //GLuint vNormal = glGetAttribLocation( program, "vNormal" );
    glEnableVertexAttribArray( shaderHandles.vNormal );
    glVertexAttribPointer(shaderHandles.vNormal, 3, GL_FLOAT, GL_FALSE, 0,
                          BUFFER_OFFSET(0));
    
    //GLuint vPosition = glGetAttribLocation( program, "vPosition" );
    glEnableVertexAttribArray(shaderHandles.vPosition );
    glVertexAttribPointer( shaderHandles.vPosition, 4, GL_FLOAT, GL_FALSE, 0,
                          BUFFER_OFFSET(numBytesNormals) );
    
    
    //Create a buffer for material parameters
    GLsizeiptr numBytesVectorParams = numVertices * sizeof(GLJoe::Vec4);
    GLsizeiptr numBytesFloatParams = numVertices * sizeof(GLfloat);

    glBindBuffer( GL_ARRAY_BUFFER, buffer[1] );
    glBufferData( GL_ARRAY_BUFFER,numBytesVectorParams*3 + numBytesFloatParams,
                 NULL, GL_STATIC_DRAW );
    glBufferSubData(GL_ARRAY_BUFFER, 0, numBytesVectorParams, materials_ambient);
    glBufferSubData(GL_ARRAY_BUFFER, numBytesVectorParams, numBytesVectorParams, materials_diffuse);
    glBufferSubData(GL_ARRAY_BUFFER, 2*numBytesVectorParams, numBytesVectorParams, materials_specular);
    glBufferSubData(GL_ARRAY_BUFFER, 3*numBytesVectorParams, numBytesFloatParams, materials_shininess);

        
    glEnableVertexAttribArray( shaderHandles.vMaterialAmbient );
    glVertexAttribPointer(shaderHandles.vMaterialAmbient, 4, GL_FLOAT, GL_FALSE, 0,
                          BUFFER_OFFSET(0));
    
    glEnableVertexAttribArray( shaderHandles.vMaterialDiffuse );
    glVertexAttribPointer(shaderHandles.vMaterialDiffuse, 4, GL_FLOAT, GL_FALSE, 0,
                          BUFFER_OFFSET(numBytesVectorParams));
    
    glEnableVertexAttribArray( shaderHandles.vMaterialSpecular );
    glVertexAttribPointer(shaderHandles.vMaterialSpecular, 4, GL_FLOAT, GL_FALSE, 0,
                          BUFFER_OFFSET(numBytesVectorParams*2));
    
    glEnableVertexAttribArray( shaderHandles.vMaterialShininess );
    glVertexAttribPointer(shaderHandles.vMaterialShininess, 1, GL_FLOAT, GL_FALSE, 0,
                          BUFFER_OFFSET(numBytesVectorParams*3));
    

    /*

    
    GLJoe::Mat4 worldToCamera = GLJoe::Translate(0, 0, -2);
    GLJoe::Mat4 objectToWorld = GLJoe::RotateX(90);
    GLJoe::Mat4 projection = GLJoe::Perspective(fovy, aspect, near, far);

    */
    /*
    
    
    glUniformMatrix4fv( shaderHandles.cMw, 1, GL_TRUE, worldToCamera);
    glUniformMatrix4fv( shaderHandles.wMo, 1, GL_TRUE, objectToWorld);
    glUniformMatrix4fv( shaderHandles.Projection, 1, GL_TRUE, projection );
    
    glUniform4fv( shaderHandles.LightPosition,1, light_position );
    glUniform4fv( shaderHandles.LightAmbient, 1, light_ambient );
    glUniform4fv( shaderHandles.LightDiffuse, 1, light_diffuse );
    glUniform4fv( shaderHandles.LightSpecular, 1, light_specular );

    */

}

void OBJObject::drawSelf() {
    #ifdef __APPLE__  //apple
        glBindVertexArrayAPPLE(vao);
    #else
        glBindVertexArray(vao);
    #endif
    
    //Send correct Transformation information to the shaders
    glUniformMatrix4fv( shaderHandles.cMw, 1, GL_TRUE, cMw);
    glUniformMatrix4fv( shaderHandles.wMo, 1, GL_TRUE, wMo);
    
    //Draw the object
    glDrawArrays( GL_TRIANGLES, 0, numVertices);
}

