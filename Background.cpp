#include "Background.h"
#include "TexturedOBJObject.h"

Background::Background(char* backImage, char* groundImage, OBJObjectShaderHandles &shaderHandles)
{
	handles = shaderHandles;
	imgLoader.loadImage(backImage, back_image, IMG_WIDTH, IMG_HEIGHT);
	imgLoader.loadImage(groundImage, ground_image, IMG_WIDTH, IMG_HEIGHT);

	
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
		Vec2(1.0, 0.3),
		Vec2(1.0, 1.3),
		Vec2(0.0, 0.3),
		Vec2(1.0, 1.3),
		Vec2(0.0, 0.3),
		Vec2(0.0, 1.3)

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
		Vec4(1.0, 1.0, 1.0, 1.0),
		Vec4(1.0, 1.0, 1.0, 1.0),
		Vec4(1.0, 1.0, 1.0, 1.0),
		Vec4(1.0, 1.0, 1.0, 1.0),
		Vec4(1.0, 1.0, 1.0, 1.0),
		Vec4(1.0, 1.0, 1.0, 1.0)
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
	back_cMw.translate(0.0, 0.0, -999);
	back_cMw.scale(1000);

	ground_cMw.rotateX(90);
	ground_cMw.translate(0.0, -100.0, -500);
	ground_cMw.scale(1000);


	OBJObjectParams defaultBldgParams;
	defaultBldgParams.material_ambient = Vec4(0.8, 0.8, 0.8, 1.0);
	defaultBldgParams.material_diffuse = Vec4(0.2, 0.4, 1.0, 1.0);
	defaultBldgParams.material_specular = Vec4(0.0, 0.0, 0.0, 1.0);
	defaultBldgParams.material_shininess = 6.0;

	bldgDist[0] = -300;
	bldgDist[1] = -700;
	bldgDist[2] = -900;
	bldgDist[3] = -200;
	bldgDist[4] = -500;

	bldgWMO[0] = Translate(-43.0, -100, 0);
	bldgWMO[1] = Translate(50.0, -100, 0);
	bldgWMO[2] = Translate(10.0, -100, 0);
	bldgWMO[3] = Translate(100.0, -100, 0);
	bldgWMO[4] = Translate(-200.0, -100, 0);

	
	bldgWMO[0].scale(40, 100, 40);
	bldgWMO[1].scale(40, 100, 40);
	bldgWMO[2].scale(40, 70, 40);
	bldgWMO[3].scale(40, 100, 40);
	bldgWMO[4].scale(40, 100, 40);

	//buildings[0] = new OBJObject("Models/Buildings/skyscraper.obj", handles, bldgCMW[0], bldgWMO[0], &defaultBldgParams);
    buildings[0] = new TexturedOBJObject("Images/darkTexture.png",1024,685,"Models/Buildings/skyscraper.obj",handles,bldgCMW[0],bldgWMO[0],&defaultBldgParams);
	buildings[1] = new OBJObject("Models/Buildings/mayanTemple.obj", handles, bldgCMW[1], bldgWMO[1], NULL);
	buildings[2] = new OBJObject("Models/Buildings/mayanTemple.obj", handles, bldgCMW[2], bldgWMO[2], NULL);
	buildings[3] = new OBJObject("Models/Buildings/mayanTemple.obj", handles, bldgCMW[3], bldgWMO[3], NULL);
	buildings[4] = new OBJObject("Models/Buildings/skyscraper.obj", handles, bldgCMW[4], bldgWMO[4], &defaultBldgParams);

	num_buildings = 5;

	for(int i = 0; i < num_buildings; i++)
		buildings[i]->initializeOpenGLBuffers();

	initializeOpenGLBuffers();
}

void Background::initializeOpenGLBuffers() {
    
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
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, IMG_WIDTH, IMG_HEIGHT, 0, GL_RGB, GL_UNSIGNED_BYTE, back_image);
	glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );
	glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST );
    
	glBindTexture(GL_TEXTURE_2D, textures[1]);
    
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, IMG_WIDTH, IMG_HEIGHT, 0, GL_RGB, GL_UNSIGNED_BYTE, ground_image);
    glGenerateMipmap( GL_TEXTURE_2D );
	glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );
	glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR );
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR );

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

void Background::draw()
{
	#ifdef __APPLE__  //apple
        glBindVertexArrayAPPLE(vao);
    #else
        glBindVertexArray(vao);
    #endif

	glBindTexture( GL_TEXTURE_2D, textures[0] );

	//Send correct Transformation information to the shaders
    glUniformMatrix4fv( handles.cMw, 1, GL_TRUE, back_cMw);
    glUniformMatrix4fv( handles.wMo, 1, GL_TRUE, back_wMo);


	//Turn on Textures
    glUniform1i(handles.EnableTex, 1);
	glUniform1i(handles.MoveTex, 0);
	glUniform1f(handles.TexOffset, texture_offset);
    //Draw the object
    glDrawArrays( GL_TRIANGLES, 0, num_vertices);

	glBindTexture( GL_TEXTURE_2D, textures[1] );

	glUniformMatrix4fv( handles.cMw, 1, GL_TRUE, ground_cMw);
    glUniformMatrix4fv( handles.wMo, 1, GL_TRUE, ground_wMo);

	glUniform1i(handles.MoveTex, 1);
	glUniform1i(handles.TexOffset, texture_offset);

	//Draw the object
    glDrawArrays( GL_TRIANGLES, 0, num_vertices);

	//Turn off Textures
	glUniform1i(handles.EnableTex, 0);
	for(int i = 0; i < num_buildings; i++)
	{
		buildings[i]->setcMw(bldgCMW[i]);
		buildings[i]->drawSelf();
	}

}

void Background::resize(int width, int height)
{
	back_cMw = Translate(0.0, 0.0, -999);
	back_cMw.scale(1000.0 * width / height, 1000.0, 1.0);

	ground_cMw = RotateX(90);
	ground_cMw.translate(0.0, -100.0, -500);
	ground_cMw.scale(1000.0 * width / height, 1000.0, 1.0);
}

void Background::moveGroundTexture(GLfloat offset)
{
	texture_offset += offset;
	if(texture_offset >= 100)
		texture_offset = 0;
}

void Background::moveBuildings(int distance)
{
	for(int i = 0; i < num_buildings; i++)
	{
		bldgDist[i] += distance;
		if(bldgDist[i] >= 0)
			bldgDist[i] = -999;

		bldgCMW[i] = Translate(0.0, 0.0, bldgDist[i]);
	}
}
