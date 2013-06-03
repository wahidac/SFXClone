//
//  TexturedOBJObject.cpp
//  Dev
//
//  Created by Wahid Chowdhury on 6/3/13.
//  Copyright (c) 2013 Wahid Chowdhury. All rights reserved.
//

#include "TexturedOBJObject.h"
#include "GLJoe.h"

using namespace GLJoe;

void TexturedOBJObject::initializeOpenGLBuffers() {
    
    OBJObject::initializeOpenGLBuffers();
    
    //Load the image we will use for texture mapping
    imgloader.loadImage(textureFileName, image, imageWidth, imageHeight);

    //All this state will be present in the vao of the parent class
    glGenTextures(1, &textureMemory);
	glBindTexture(GL_TEXTURE_2D, textureMemory);
    
  /*  GLuint bufferForTextureCoords;
    glGenBuffers(1, &bufferForTextureCoords);
    glBindBuffer(GL_ARRAY_BUFFER, bufferForTextureCoords);
    
    
    Vec2 tex[6] = {
		Vec2(1.0, 0.3),
		Vec2(1.0, 1.3),
		Vec2(0.0, 0.3),
		Vec2(1.0, 1.3),
		Vec2(0.0, 0.3),
		Vec2(0.0, 1.3)
        
	};
    
    GLsizeiptr numBytesTexCoords = num_vertices * sizeof(GLJoe::Vec2);
    
    glBufferData( GL_ARRAY_BUFFER,numBytesNormals + numBytesVertices + numBytesTexCoords,
                 NULL, GL_STATIC_DRAW );
    glBufferSubData(GL_ARRAY_BUFFER, 0, numBytesNormals, plane_normals);
    glBufferSubData(GL_ARRAY_BUFFER,numBytesNormals, numBytesVertices, plane_vertices);
	glBufferSubData(GL_ARRAY_BUFFER, numBytesNormals + numBytesVertices, numBytesTexCoords, tex_coords);
    
    //GLuint vNormal = glGetAttribLocation( program, "vNormal" );
    glEnableVertexAttribArray( handles.vNormal );
    glVertexAttribPointer(handles.vNormal, 3, GL_FLOAT, GL_FALSE, 0,
                          BUFFER_OFFSET(0));
    
    //GLuint vPosition = glGetAttribLocation( program, "vPosition" );
    glEnableVertexAttribArray(handles.vPosition );
    glVertexAttribPointer( handles.vPosition, 4, GL_FLOAT, GL_FALSE, 0,
                          BUFFER_OFFSET(numBytesNormals) );
    
	//GLuint vPosition = glGetAttribLocation( program, "vTexCoords" );
    glEnableVertexAttribArray(handles.vTexCoords );
    glVertexAttribPointer( handles.vTexCoords, 2, GL_FLOAT, GL_FALSE, 0,
                          BUFFER_OFFSET(numBytesNormals + numBytesVertices) );

    */
    
/*	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, imageWidth, imageHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
	glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );
	glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST );
    glUniform1i(shaderHandles.tex, 0);
  */  
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, imageWidth, imageHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
    glGenerateMipmap( GL_TEXTURE_2D );
	glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );
	glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR );
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR );
    glUniform1i(shaderHandles.tex, 0);
  
}

void TexturedOBJObject::drawSelf() {
#ifdef __APPLE__  //apple
    glBindVertexArrayAPPLE(vao);
#else
    glBindVertexArray(vao);
#endif
    glBindTexture( GL_TEXTURE_2D, textureMemory );
	//Turn on Textures
    glUniform1i(shaderHandles.EnableTex, 1);
    glUniform1i(shaderHandles.calculateTexCoordInShader, 1);
    OBJObject::drawSelf();
    glUniform1i(shaderHandles.calculateTexCoordInShader, 0);
	//Turn off Textures
    glUniform1i(shaderHandles.EnableTex, 0);
}