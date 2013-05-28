/**
 * @file GLJoe/geometry/cube.h
 * 
 * @brief Geometry for a cube
 * 
 * @author Joachim Valente <joachim.valente@gmail.com>
 * @date 6/11/2013
 */
 
#ifndef DEF_GLJOE_CUBE_H
#define DEF_GLJOE_CUBE_H

namespace GLJoe
{
	
class Cube
{
protected:
	Vec4* _point; // 8 points
	Vec4* _vertex; // 2 * 3 * 6 = 36 vertices
	Vec3* _normal; // 36 normals
	Vec2* _tex; // 36 textures coordinates
	Vec4 _texRange; // ranges for s and t; by default (0, 1, 0, 1)
	
	unsigned _index; // variable used during the generation
	
	GLuint _vao; // vertex array object
	GLuint _texImage; // texture image
	
	/**
	 * Generate two triangles per face
	 * Face is described counter-clockwise from the outside,
	 * starting with the top-right corner
	 */
	void generateFace(unsigned a, unsigned b, unsigned c, unsigned d)
	{
		Vec3 normal = cross(_point[b] - _point[a], _point[d] - _point[a])
			.normalize();
		
		GLfloat s1 = _texRange[0];
		GLfloat s2 = _texRange[1];
		GLfloat t1 = _texRange[2];
		GLfloat t2 = _texRange[3];
		
		// First triangle
		_vertex[_index] = _point[a]; _tex[_index] = Vec2(s2, t2);
			_normal[_index] = normal; _index++;		
		_vertex[_index] = _point[b]; _tex[_index] = Vec2(s1, t2);
			_normal[_index] = normal; _index++;		
		_vertex[_index] = _point[d]; _tex[_index] = Vec2(s2, t1);
			_normal[_index] = normal; _index++;
				
		// Second triangle
		_vertex[_index] = _point[c]; _tex[_index] = Vec2(s1, t1);
			_normal[_index] = normal; _index++;		
		_vertex[_index] = _point[b]; _tex[_index] = Vec2(s1, t2);
			_normal[_index] = normal; _index++;		
		_vertex[_index] = _point[d]; _tex[_index] = Vec2(s2, t1);
			_normal[_index] = normal; _index++;
	}
public:
	
	/**
	 * Constructor and destructor
	 */
	Cube()
	{
		_point = new Vec4[8];
		_vertex = new Vec4[36];
		_normal = new Vec3[36];
		_tex = new Vec2[36];
		_texRange = Vec4(0, 1, 0, 1);
		_texImage = 0;
	}
	
	~Cube()
	{
		delete[] _point;
		delete[] _vertex;
		delete[] _normal;
		delete[] _tex;
	}
	
	/**
	 * Generation
	 */
	void generate()
	{
		_point[0] = Vec4( 0.5,  0.5,  0.5, 1.0);
		_point[1] = Vec4(-0.5,  0.5,  0.5, 1.0);
		_point[2] = Vec4(-0.5, -0.5,  0.5, 1.0);
		_point[3] = Vec4( 0.5, -0.5,  0.5, 1.0);
		_point[4] = Vec4( 0.5,  0.5, -0.5, 1.0);
		_point[5] = Vec4(-0.5,  0.5, -0.5, 1.0);
		_point[6] = Vec4(-0.5, -0.5, -0.5, 1.0);
		_point[7] = Vec4( 0.5, -0.5, -0.5, 1.0);
		
		_index = 0;
		
		generateFace(0, 1, 2, 3);
		generateFace(5, 4, 7, 6);
		generateFace(4, 5, 1, 0);
		generateFace(3, 2, 6, 7);
		generateFace(1, 5, 6, 2);
		generateFace(4, 0, 3, 7);
	}
	
	/**
	 * Getters
	 */
	GLsizei numPoints() const
	{
		return 36;
	}
	
	/**
	 * Setters
	 */
	
	void setTexture(const TgaImage& img, const Vec4& range = Vec4(0, 1, 0, 1), 
		GLfloat wrapS = GL_CLAMP, GLfloat wrapT = GL_CLAMP,
		GLfloat magFilter = GL_NEAREST,	GLfloat minFilter = GL_NEAREST)
	{
		// Modify range for texture mapping
		_texRange = range;
		
		// Generate a new texture
		glGenTextures(1, &_texImage);
		glBindTexture(GL_TEXTURE_2D, _texImage);
		
		// Define mipmaps
		glTexImage2D(GL_TEXTURE_2D, 0, 4, img.width(), img.height(), 0, 
			(img.byteCount() == 3) ? GL_BGR : GL_BGRA,
			GL_UNSIGNED_BYTE, img.data());
		glGenerateMipmap(GL_TEXTURE_2D);
		
		// Define wrapping mode
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrapS);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrapT);
		
		// Define interpolation mode
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, magFilter);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, minFilter);
	
	}
	
	/**
	 * Drawing
	 */
	
	// Create a vertex array object and initialize the buffers
	void initDraw(const GLuint& program)
	{
		// Create a vertex array object
        #ifdef __APPLE__  //apple
            glGenVertexArraysAPPLE(1, &_vao);
            glBindVertexArrayAPPLE(_vao);
        #else 
            glGenVertexArrays(1, &_vao);
            glBindVertexArray(_vao);
        #endif 

        
		
		// Create 3 buffers
		GLuint buffer[3];
		glGenBuffers(3, buffer); // 0->vertices, 1->normals, 2->texture
	
		// Vertices
		glBindBuffer(GL_ARRAY_BUFFER, buffer[0]);
		glBufferData(GL_ARRAY_BUFFER, numPoints() * sizeof(Vec4),
			_vertex, GL_STATIC_DRAW);
		GLuint vPosition = glGetAttribLocation(program, "vPosition");
		glEnableVertexAttribArray(vPosition);
		glVertexAttribPointer(vPosition, 4, GL_FLOAT, GL_FALSE, 0,
			BUFFER_OFFSET(0));
		
		// Normals
		glBindBuffer(GL_ARRAY_BUFFER, buffer[1]);
		glBufferData(GL_ARRAY_BUFFER, numPoints() * sizeof(Vec3),
			_normal, GL_STATIC_DRAW);
		GLuint vNormal = glGetAttribLocation(program, "vNormal");
		glEnableVertexAttribArray(vNormal);
		glVertexAttribPointer(vNormal, 3, GL_FLOAT, GL_FALSE, 0,
			BUFFER_OFFSET(0));
	
		// Texture
		glBindBuffer(GL_ARRAY_BUFFER, buffer[2]);
		glBufferData(GL_ARRAY_BUFFER, numPoints() * sizeof(Vec2),
			_tex, GL_STATIC_DRAW);
		GLuint vTexCoords = glGetAttribLocation(program, "vTexCoords");
		glEnableVertexAttribArray(vTexCoords);
		glVertexAttribPointer(vTexCoords, 2, GL_FLOAT, GL_FALSE, 0,
			BUFFER_OFFSET(0));
	}
	
	// Draw the cube
	void draw(GLenum mode = GL_TRIANGLES)
	{
        #ifdef __APPLE__  //apple
            glBindVertexArrayAPPLE(_vao);
        #else
            glBindVertexArray(_vao);
        #endif

		glBindTexture(GL_TEXTURE_2D, _texImage);
		glDrawArrays(mode, 0, numPoints());
	}
};

	
} // namespace GLJoe






#endif // DEF_GLJOE_CUBE_H

