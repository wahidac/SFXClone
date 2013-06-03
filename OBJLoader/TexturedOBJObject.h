//
//  TexturedOBJObject.h
//  Dev
//
//  Created by Wahid Chowdhury on 6/3/13.
//  Copyright (c) 2013 Wahid Chowdhury. All rights reserved.
//

#ifndef __Dev__TexturedOBJObject__
#define __Dev__TexturedOBJObject__

#include <iostream>
#include "OBJObject.h"
#include "ImgLoader.h"

//Inherits from OBJObject. Just adds texturing
class TexturedOBJObject: public OBJObject {
    
public:
    TexturedOBJObject(const char *texFileName,int width, int height,const char *objFileName, const OBJObjectShaderHandles &shaderHandles, const GLJoe::Transform &cMw, const GLJoe::Transform &wMo, OBJObjectParams *defaultShaderParams) : OBJObject(objFileName,shaderHandles,cMw,wMo,defaultShaderParams)
    {
        imageWidth = width;
        imageHeight = height;
        sizeOfImageBuffer = width * height * 3;
        image = new GLubyte[sizeOfImageBuffer];
        textureFileName = texFileName;
        
        initializeOpenGLBuffers();

    }
    
    virtual ~TexturedOBJObject() {
        delete [] image;
    }
    virtual void initializeOpenGLBuffers();
    virtual void drawSelf();
private:
    const char* textureFileName;
    int imageWidth;
    int imageHeight;
    ImgLoader imgloader;
    GLubyte *image;
    int sizeOfImageBuffer;
    //Vec2* tex_coords;    
	//GLfloat texture_offset;
	GLuint textureMemory;

};

#endif /* defined(__Dev__TexturedOBJObject__) */
