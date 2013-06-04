#include "powerup.h"

Powerup::Powerup(char* displayImage, char* colorPickingImage, Vec3 samplingColor, OBJObjectShaderHandles &shaderHandles)
{
	handles = shaderHandles;
	imgLoader.loadImage(displayImage, image, IMG_WIDTH, IMG_HEIGHT);
	imgLoader.loadImage(colorPickingImage, pickingImage, IMG_WIDTH, IMG_HEIGHT);
	
	num_vertices = 6;
	
	Vec4 vertices[6] = {
		Vec4(0.5, 0.5, 0.0, 1.0),
		Vec4(0.5, -0.5, 0.0, 1.0),
		Vec4(-0.5, 0.5, 0.0, 1.0),
		Vec4(0.5, -0.5, 0.0, 1.0),
		Vec4(-0.5, 0.5, 0.0, 1.0),
		Vec4(-0.5, -0.5, 0.0, 1.0)
	};
	plane_vertices = vertices;

	Vec2 tex[6] = {
		Vec2(1.0, 0.0),
		Vec2(1.0, 1.0),
		Vec2(0.0, 0.0),
		Vec2(1.0, 1.0),
		Vec2(0.0, 0.0),
		Vec2(0.0, 1.0)

	};
	tex_coords = tex;
	texture_offset = 0;

	Vec3 normals[6] = {
		Vec3(0.0, 0.0, 1.0),
		Vec3(0.0, 0.0, 1.0),
		Vec3(0.0, 0.0, 1.0),
		Vec3(0.0, 0.0, 1.0),
		Vec3(0.0, 0.0, 1.0),
		Vec3(0.0, 0.0, 1.0)
	};
	plane_normals = normals;

	Vec4 ambient[6] = {
		Vec4(0.0, 0.0, 0.0, 1.0),
		Vec4(0.0, 0.0, 0.0, 1.0),
		Vec4(0.0, 0.0, 0.0, 1.0),
		Vec4(0.0, 0.0, 0.0, 1.0),
		Vec4(0.0, 0.0, 0.0, 1.0),
		Vec4(0.0, 0.0, 0.0, 1.0)
	};
	material_ambient = ambient;

	Vec4 other[6] = {
		Vec4(0.0, 0.0, 0.0, 0.0),
		Vec4(0.0, 0.0, 0.0, 0.0),
		Vec4(0.0, 0.0, 0.0, 0.0),
		Vec4(0.0, 0.0, 0.0, 0.0),
		Vec4(0.0, 0.0, 0.0, 0.0),
		Vec4(0.0, 0.0, 0.0, 0.0)
	};
	material_other = other;
	
	cMw.translate(0.0, 0.0, 0.0);
	wMo.scale(8.0);

	initializeOpenGLBuffers();
	isVisible = false;
	colorPicking = false;
	colorSamplingColor = samplingColor;
}

void Powerup::initializeOpenGLBuffers() {
    
#ifdef __APPLE__  //apple
    glGenVertexArraysAPPLE( 1, &vao );
    glBindVertexArrayAPPLE(vao);
#else
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
#endif
        
	//Initialize textures
	glGenTextures(2, textures);
	glBindTexture(GL_TEXTURE_2D, textures[0]);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, IMG_WIDTH, IMG_HEIGHT, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
	glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );
	glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST );

	glBindTexture(GL_TEXTURE_2D, textures[1]);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, IMG_WIDTH, IMG_HEIGHT, 0, GL_RGB, GL_UNSIGNED_BYTE, pickingImage);
	glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );
	glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST );

	glUniform1i(handles.tex, 0);

    GLuint buffer[2];
    glGenBuffers( 2, buffer );
    glBindBuffer( GL_ARRAY_BUFFER, buffer[0] );
    
    GLsizeiptr numBytesVertices = num_vertices * sizeof(GLJoe::Vec4);
    GLsizeiptr numBytesNormals = num_vertices * sizeof(GLJoe::Vec3);
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
    
    
    //Create a buffer for material parameters
    GLsizeiptr numBytesVectorParams = num_vertices * sizeof(GLJoe::Vec4);
    GLsizeiptr numBytesFloatParams = num_vertices * sizeof(GLfloat);

    glBindBuffer( GL_ARRAY_BUFFER, buffer[1] );
    glBufferData( GL_ARRAY_BUFFER,numBytesVectorParams*3 + numBytesFloatParams,
                 NULL, GL_STATIC_DRAW );
    glBufferSubData(GL_ARRAY_BUFFER, 0, numBytesVectorParams, material_ambient);
    glBufferSubData(GL_ARRAY_BUFFER, numBytesVectorParams, numBytesVectorParams, material_other);
    glBufferSubData(GL_ARRAY_BUFFER, 2*numBytesVectorParams, numBytesVectorParams, material_other);
    glBufferSubData(GL_ARRAY_BUFFER, 3*numBytesVectorParams, numBytesFloatParams, material_other);

        
    glEnableVertexAttribArray( handles.vMaterialAmbient );
    glVertexAttribPointer(handles.vMaterialAmbient, 4, GL_FLOAT, GL_FALSE, 0,
                          BUFFER_OFFSET(0));
    
    glEnableVertexAttribArray( handles.vMaterialDiffuse );
    glVertexAttribPointer(handles.vMaterialDiffuse, 4, GL_FLOAT, GL_FALSE, 0,
                          BUFFER_OFFSET(numBytesVectorParams));
    
    glEnableVertexAttribArray( handles.vMaterialSpecular );
    glVertexAttribPointer(handles.vMaterialSpecular, 4, GL_FLOAT, GL_FALSE, 0,
                          BUFFER_OFFSET(numBytesVectorParams*2));
    
    glEnableVertexAttribArray( handles.vMaterialShininess );
    glVertexAttribPointer(handles.vMaterialShininess, 1, GL_FLOAT, GL_FALSE, 0,
                          BUFFER_OFFSET(numBytesVectorParams*3));
}

void Powerup::draw()
{
	#ifdef __APPLE__  //apple
        glBindVertexArrayAPPLE(vao);
    #else
        glBindVertexArray(vao);
    #endif

	if(colorPicking == false) {
		glBindTexture( GL_TEXTURE_2D, textures[0] );
	} else {
		glBindTexture( GL_TEXTURE_2D, textures[1] );
		glUniform1i(handles.useColorPicking, 0);
	}

	//Send correct Transformation information to the shaders
    glUniformMatrix4fv( handles.cMw, 1, GL_TRUE, cMw);
    glUniformMatrix4fv( handles.wMo, 1, GL_TRUE, wMo);

	//Turn on Textures
    glUniform1i(handles.EnableTex, 1);
	glUniform1i(handles.MoveTex, 0);
	glUniform1i(handles.blackTransparent, 1);
	glUniform1f(handles.TexOffset, texture_offset);
    //Draw the object

	glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glDepthMask( GL_FALSE );
    glDrawArrays( GL_TRIANGLES, 0, num_vertices);
	glDepthMask( GL_TRUE );
    glDisable(GL_BLEND);

	if(colorPicking == true)
		glUniform1i(handles.useColorPicking, 1);

	//Turn off Textures
	glUniform1i(handles.EnableTex, 0);
	glUniform1i(handles.blackTransparent, 0);
}

void Powerup::setLocation(Vec4 location)
{
	cMw = Translate(location.x, location.y, location.z - 60);
}

void Powerup::useColorPicking()
{
	colorPicking = true;
}

void Powerup::disableColorPicking()
{
	colorPicking = false;
}

Vec3 Powerup::getColorSamplingColor()
{
	return colorSamplingColor;
}