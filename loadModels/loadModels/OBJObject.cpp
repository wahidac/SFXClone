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

    materials_specular = new vec4[numVertices];
    materials_diffuse= new vec4[numVertices];
    materials_ambient = new vec4[numVertices];
    materials_shininess = new GLfloat[numVertices];

    //extract data from GLM structs and store them in the arrays defined above so we can
    //send the data to the shaders
    loadVerticesNormalsMaterials();
    

    //Copy necessary data to GPU
    initializeOpenGLBuffers();
}

void OBJObject::loadVerticesNormalsMaterials() {
    //Get a list of all the trianges that make up the objet
    std::vector<OBJMeshTriangle>displayList = mesh->getTriangles();

    for (int i = 0; i < numTriangles; i++) {
        //Processing one triangle
        
        OBJMeshTriangle triangle = displayList[i];
        GLMmaterial* material =  triangle.material;
        
        vec4 material_ambient = vec4(material->ambient[0],material->ambient[1],material->ambient[2],material->ambient[3]);
        vec4 material_diffuse = vec4(material->diffuse[0],material->diffuse[1],material->diffuse[2],material->diffuse[3]);
        vec4 material_specular = vec4(material->specular[0],material->specular[1],material->specular[2],material->specular[3]);
        GLfloat shininess = material->shininess;
        
        vec4 vertex = vec4(triangle.v1.x,triangle.v1.y,triangle.v1.z,1);
        vec3 normal = vec3(triangle.n1.i,triangle.n1.j,triangle.n1.k);
        vertices[i*3] = vertex;
        normals[i*3] = normal;
        materials_ambient[i*3] = material_ambient;
        materials_diffuse[i*3] = material_diffuse;
        materials_specular[i*3] = material_specular;
        materials_shininess[i*3] = shininess;
        
        vertex = vec4(triangle.v2.x,triangle.v2.y,triangle.v2.z,1);
        normal = vec3(triangle.n2.i,triangle.n2.j,triangle.n2.k);
        vertices[i*3+1] = vertex;
        normals[i*3+1] = normal;
        materials_ambient[i*3+1] = material_ambient;
        materials_diffuse[i*3+1] = material_diffuse;
        materials_specular[i*3+1] = material_specular;
        materials_shininess[i*3+1] = shininess;


        vertex = vec4(triangle.v3.x,triangle.v3.y,triangle.v3.z,1);
        normal = vec3(triangle.n3.i,triangle.n3.j,triangle.n3.k);
        vertices[i*3+2] = vertex;
        normals[i*3+2] = normal;
        materials_ambient[i*3+2] = material_ambient;
        materials_diffuse[i*3+2] = material_diffuse;
        materials_specular[i*3+2] = material_specular;
        materials_shininess[i*3+2] = shininess;
    }
}

void OBJObject::initializeOpenGLBuffers() {
    //NOTE: Apple specific right here!!!
    glGenVertexArraysAPPLE( 1, &vao );
    glBindVertexArrayAPPLE( vao );
    
    //Copy vertex data to the GPU
    GLuint buffer[2];
    glGenBuffers( 2, buffer );
    glBindBuffer( GL_ARRAY_BUFFER, buffer[0] );
    
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
    
    GLuint vPosition = glGetAttribLocation( program, "vPosition" );
    glEnableVertexAttribArray( vPosition );
    glVertexAttribPointer( vPosition, 4, GL_FLOAT, GL_FALSE, 0,
                          BUFFER_OFFSET(numBytesNormals) );
    
    
    //Create a buffer for material parameters
    GLsizeiptr numBytesVectorParams = numVertices * sizeof(vec4);
    GLsizeiptr numBytesFloatParams = numVertices * sizeof(GLfloat);

    glBindBuffer( GL_ARRAY_BUFFER, buffer[1] );
    glBufferData( GL_ARRAY_BUFFER,numBytesVectorParams*3 + numBytesFloatParams,
                 NULL, GL_STATIC_DRAW );
    glBufferSubData(GL_ARRAY_BUFFER, 0, numBytesVectorParams, materials_ambient);
    glBufferSubData(GL_ARRAY_BUFFER, numBytesVectorParams, numBytesVectorParams, materials_diffuse);
    glBufferSubData(GL_ARRAY_BUFFER, 2*numBytesVectorParams, numBytesVectorParams, materials_specular);
    glBufferSubData(GL_ARRAY_BUFFER, 3*numBytesVectorParams, numBytesFloatParams, materials_shininess);

    //NOTE: allow way to set these to default values in the event that there are no materials present in the .obj file
    GLuint mat_ambient = glGetAttribLocation( program, "vMaterialAmbient" );
    GLuint mat_diffuse = glGetAttribLocation( program, "vMaterialDiffuse" );
    GLuint mat_specular = glGetAttribLocation( program, "vMaterialSpecular" );
    GLuint mat_shininess = glGetAttribLocation( program, "vMaterialShininess" );

    
    glEnableVertexAttribArray( mat_ambient );
    glVertexAttribPointer(mat_ambient, 4, GL_FLOAT, GL_FALSE, 0,
                          BUFFER_OFFSET(0));
    
    glEnableVertexAttribArray( mat_diffuse );
    glVertexAttribPointer(mat_diffuse, 4, GL_FLOAT, GL_FALSE, 0,
                          BUFFER_OFFSET(numBytesVectorParams));
    
    glEnableVertexAttribArray( mat_specular );
    glVertexAttribPointer(mat_specular, 4, GL_FLOAT, GL_FALSE, 0,
                          BUFFER_OFFSET(numBytesVectorParams*2));
    
    glEnableVertexAttribArray( mat_shininess );
    glVertexAttribPointer(mat_shininess, 1, GL_FLOAT, GL_FALSE, 0,
                          BUFFER_OFFSET(numBytesVectorParams*3));
    

    //NOTE: what follows is to test the shading
    
    //NOTE: might want to make these parameters something you can define in a struct
    //and pass in to the constructor
    GLfloat near= 1.0, far=50.0;
    //GLfloat left= -5.0, right=5.0, top=5.0, bottom= -5.0, near=1.0, far=500.0;
    
    //Perspective projection parameters
    GLfloat  fovy = 90.0;  // Field-of-view in Y direction angle (in degrees)
    GLfloat  aspect = 1;       // Viewport aspect ratio
    
    mat4 worldToCamera = Translate(0, 0, -2);
    mat4 objectToWorld = RotateX(90);
    mat4 projection = Perspective(fovy, aspect, near, far);
    
    //lighting parameters
    vec4 light_position(0,0,1,1);
    vec4 light_ambient(.8, .8, .8, 1.0 );
    vec4 light_diffuse( .8, .8,.8, 1.0 );
    vec4 light_specular( .8, .8, .8, 1.0 );
    
    //vec4 material_ambient( 0.1, 0.3, 0.0, 1.0 );
    //vec4 material_diffuse( 0.3, 0.3, 0.0, 1.0 );
    //vec4 material_specular( 1, .4, .7, 1.0 );
    float  material_shininess = 300.0;

    //vec4 ambient_product = light_ambient * material_ambient;
    //vec4 diffuse_product = light_diffuse * material_diffuse;
    //vec4 specular_product = light_specular * material_specular;
    
    GLuint cMw = glGetUniformLocation(program, "cMw");
    GLuint wMo = glGetUniformLocation( program, "wMo" );
    GLuint Projection = glGetUniformLocation( program, "Projection" );
  //  GLuint AmbientProduct = glGetUniformLocation(program, "AmbientProduct");
  //  GLuint DiffuseProduct = glGetUniformLocation(program, "DiffuseProduct");
  //  GLuint SpecularProduct = glGetUniformLocation(program, "SpecularProduct");
    GLuint LightAmbient = glGetUniformLocation(program, "LightAmbient");
    GLuint LightDiffuse = glGetUniformLocation(program, "LightDiffuse");
    GLuint LightSpecular = glGetUniformLocation(program, "LightSpecular");
    GLuint ShaderType = glGetUniformLocation(program, "shaderType");
    GLuint LightPosition = glGetUniformLocation(program, "LightPosition");
    
    
    
    glUniformMatrix4fv( cMw, 1, GL_TRUE, worldToCamera);
    glUniformMatrix4fv( wMo, 1, GL_TRUE, objectToWorld);
    glUniformMatrix4fv( Projection, 1, GL_TRUE, projection );
    
    glUniform4fv( LightPosition,1, light_position );
    //glUniform4fv( AmbientProduct, 1, ambient_product );
    //glUniform4fv( DiffuseProduct, 1, diffuse_product );
    //glUniform4fv( SpecularProduct, 1, specular_product );
    glUniform4fv( LightAmbient, 1, light_ambient );
    glUniform4fv( LightDiffuse, 1, light_diffuse );
    glUniform4fv( LightSpecular, 1, light_specular );

    glUniform1i(ShaderType, 1 );
    

}

void OBJObject::drawSelf() {
    //Bind to vao that represents parameters of this object
    glBindVertexArrayAPPLE(vao);
    glDrawArrays( GL_TRIANGLES, 0, numVertices);
}

