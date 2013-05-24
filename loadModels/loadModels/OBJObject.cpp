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
    
    GLsizeiptr numBytesVertices = numVertices * sizeof(vec4);
    GLsizeiptr numBytesNormals = numVertices * sizeof(vec3);
    
    glBufferData( GL_ARRAY_BUFFER,numBytesNormals + numBytesVertices,
                 NULL, GL_STATIC_DRAW );
    glBufferSubData(GL_ARRAY_BUFFER, 0, numBytesNormals, normals);
    glBufferSubData(GL_ARRAY_BUFFER,numBytesNormals, numBytesVertices, vertices);

    //NOTE: hardcoding heree
    GLuint vNormal = glGetAttribLocation( program, "vNormal" );
    glEnableVertexAttribArray( vNormal );
    glVertexAttribPointer(vNormal, 3, GL_FLOAT, GL_FALSE, 0,
                          BUFFER_OFFSET(0));
    std::cout<<glGetError()<<" vNormal "<<vNormal;
    
    GLuint vPosition = glGetAttribLocation( program, "vPosition" );
    glEnableVertexAttribArray( vPosition );
    glVertexAttribPointer( vPosition, 4, GL_FLOAT, GL_FALSE, 0,
                          BUFFER_OFFSET(numBytesNormals) );
    
    std::cout<<glGetError()<<" vNormal "<<vNormal;

    
    //NOTE: what follows is to test the shading
    
    //NOTE: might want to make these parameters something you can define in a struct
    //and pass in to the constructor
    GLfloat near= 1.0, far=50.0;
    //GLfloat left= -5.0, right=5.0, top=5.0, bottom= -5.0, near=1.0, far=500.0;
    
    //Perspective projection parameters
    GLfloat  fovy = 90.0;  // Field-of-view in Y direction angle (in degrees)
    GLfloat  aspect = 1;       // Viewport aspect ratio
    
    mat4 worldToCamera = Translate(0, 0, -2);
    mat4 objectToWorld = mat4();
    mat4 projection = Perspective(fovy, aspect, near, far);
    
    //lighting parameters
    vec4 light_position(0,2,0,1);
    vec4 light_ambient(1.0, 1.0, 1.0, 1.0 );
    vec4 light_diffuse( 1.0, 1.0, 1.0, 1.0 );
    vec4 light_specular( 1.0, 1.0, 1.0, 1.0 );
    
    vec4 material_ambient( 0.1, 0.3, 0.0, 1.0 );
    vec4 material_diffuse( 0.3, 0.3, 0.0, 1.0 );
    vec4 material_specular( 1, .4, .7, 1.0 );
    float  material_shininess = 300.0;

    vec4 ambient_product = light_ambient * material_ambient;
    vec4 diffuse_product = light_diffuse * material_diffuse;
    vec4 specular_product = light_specular * material_specular;
    
    GLuint cMw = glGetUniformLocation(program, "cMw");
    GLuint wMo = glGetUniformLocation( program, "wMo" );
    GLuint Projection = glGetUniformLocation( program, "Projection" );
    GLuint AmbientProduct = glGetUniformLocation(program, "AmbientProduct");
    GLuint DiffuseProduct = glGetUniformLocation(program, "DiffuseProduct");
    GLuint SpecularProduct = glGetUniformLocation(program, "SpecularProduct");
    GLuint Shininess = glGetUniformLocation(program, "Shininess");
    GLuint ShaderType = glGetUniformLocation(program, "shaderType");
    GLuint LightPosition = glGetUniformLocation(program, "LightPosition");
    
    
    
    glUniformMatrix4fv( cMw, 1, GL_TRUE, worldToCamera);
    glUniformMatrix4fv( wMo, 1, GL_TRUE, objectToWorld);
    glUniformMatrix4fv( Projection, 1, GL_TRUE, projection );
    
    glUniform4fv( LightPosition,1, light_position );
    glUniform4fv( AmbientProduct, 1, ambient_product );
    glUniform4fv( DiffuseProduct, 1, diffuse_product );
    glUniform4fv( SpecularProduct, 1, specular_product );
    glUniform1f( Shininess, material_shininess );
    glUniform1i(ShaderType, 1 );
    

}

void OBJObject::drawSelf() {
    //Bind to vao that represents parameters of this object
    glBindVertexArrayAPPLE(vao);
    glDrawArrays( GL_TRIANGLES, 0, numVertices);
}


