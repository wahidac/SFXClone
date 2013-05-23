//
//  OBJObject.cpp
//  loadModels
//
//  Created by Wahid Chowdhury on 5/16/13.
//  Copyright (c) 2013 Wahid Chowdhury. All rights reserved.
//

#include "OBJObject.h"

OBJObject::OBJObject(char *objFileName, GLuint program): program(program) {
    mesh = parser.load(objFileName);
    numTriangles = mesh->getTriangles().size();
    numVertices = numTriangles*3; //Each triangle has 3 vertices
    
    //Allocate memory
    vertices = new vec4[numVertices];
    normals = new vec3[numVertices];
    
    loadVerticesAndNormals();
    
    //Copy necessary data to GPU
    initializeOpenGLBuffers();
}

void OBJObject::loadVerticesAndNormals() {
    //Get a list of all the trianges that make up the objet
    std::vector<OBJMeshTriangle>displayList = mesh->getTriangles();
    
    for (int i = 0; i < numTriangles; i++) {
        //Processing one triangle
        OBJMeshTriangle triangle = displayList[i];
        
        vec4 vertex = vec4(triangle.v1.x,triangle.v1.y,triangle.v1.z,1);
        vec3 normal = vec3(triangle.n1.i,triangle.n1.j,triangle.n1.k);
        vertices[i*3] = vertex;
        normals[i*3] = normal;

        vertex = vec4(triangle.v2.x,triangle.v2.y,triangle.v2.z,1);
        normal = vec3(triangle.n2.i,triangle.n2.j,triangle.n2.k);
        vertices[i*3+1] = vertex;
        normals[i*3+1] = normal;

        vertex = vec4(triangle.v3.x,triangle.v3.y,triangle.v3.z,1);
        normal = vec3(triangle.n3.i,triangle.n3.j,triangle.n3.k);
        vertices[i*3+2] = vertex;
        normals[i*3+2] = normal;
    }
}

void OBJObject::initializeOpenGLBuffers() {
    //NOTE: Apple specific right here!!!
    glGenVertexArraysAPPLE( 1, &vao );
    glBindVertexArrayAPPLE( vao );
    
    //Copy vertex data to the GPU
    GLuint buffer;
    glGenBuffers( 1, &buffer );
    glBindBuffer( GL_ARRAY_BUFFER, buffer );
    glBufferData( GL_ARRAY_BUFFER,numVertices * sizeof(vec4),
                 vertices, GL_STATIC_DRAW );

    //NOTE: hardcoding here
    GLuint vPosition = glGetAttribLocation( program, "vPosition" );
    glEnableVertexAttribArray( vPosition );
    glVertexAttribPointer( vPosition, 4, GL_FLOAT, GL_FALSE, 0,
                          BUFFER_OFFSET(0) );
    
    glUniformMatrix4fv(glGetUniformLocation( program, "ModelView" ),1,GL_TRUE,RotateY(0));

}

void OBJObject::drawSelf() {
    //Bind to vao that represents parameters of this object
    glBindVertexArrayAPPLE(vao);
    glDrawArrays( GL_TRIANGLES, 0, numVertices);
}


