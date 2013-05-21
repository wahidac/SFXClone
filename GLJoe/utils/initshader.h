/**
 * @file GLJoe/utils/initshader.h
 * 
 * @brief Compile and link shaders from files
 * 
 * @author Joachim Valente <joachim.valente@gmail.com>
 * @date 6/11/2013
 * 
 * Courtesy of Edward Angel
 * @see http://www.cs.unm.edu/~angel/
 */

#ifndef DEF_GLJOE_INITSHADER_h
#define DEF_GLJOE_INITSHADER_h


namespace GLJoe
{

// Create a NULL-terminated string by reading the provided file
static char* readShaderSource(const char* shaderFile)
{
	FILE* fp = fopen(shaderFile, "rb");

	if (fp == NULL) {
		return NULL;
	}
	fseek(fp, 0L, SEEK_END);
	long size = ftell(fp);

	fseek(fp, 0L, SEEK_SET);
	char* buf = new char[size + 1];
	size_t res = fread(buf, 1, size, fp);
	if (res != (size_t) size)
		return NULL;

	buf[size] = '\0';
	fclose(fp);
 
	return buf;
}

// Create a GLSL program object from vertex and fragment shader files
GLuint InitShader(const char* vShaderFile, const char* fShaderFile)
{	
	struct Shader
	{
		const char* filename;
		GLenum type;
		const GLchar* source;
	};
	
	Shader shaders[2] = {
		{vShaderFile, GL_VERTEX_SHADER, NULL},
		{fShaderFile, GL_FRAGMENT_SHADER, NULL}
	};

	GLuint program = glCreateProgram();
	
	for (int i = 0; i < 2; ++i)
	{
		Shader& s = shaders[i];
		s.source = readShaderSource(s.filename);
		if (shaders[i].source == NULL)
		{
			Error("Failed to read %s %s", s.filename, strerror(errno));
		}

		GLuint shader = glCreateShader(s.type);
		glShaderSource(shader, 1, (const GLchar**) &s.source, NULL);
		glCompileShader(shader);

		GLint compiled;
		glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);
		if (!compiled)
		{
			
			GLint logSize;
			glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &logSize);
			char* logMsg = new char[logSize];
			glGetShaderInfoLog(shader, logSize, NULL, logMsg);
			std::string strLogMsg(logMsg);
			delete[] logMsg;
			delete[] s.source;
			Error("%s failed to compile:\n%s", s.filename, strLogMsg.c_str());
		}

		delete[] s.source;

		glAttachShader(program, shader);
	}

	// Link  and error check
	glLinkProgram(program);

	GLint linked;
	glGetProgramiv(program, GL_LINK_STATUS, &linked);
	if (!linked)
	{
		GLint logSize;
		glGetProgramiv(program, GL_INFO_LOG_LENGTH, &logSize);
		char* logMsg = new char[logSize];
		glGetProgramInfoLog(program, logSize, NULL, logMsg);
		std::string strLogMsg(logMsg);
		delete [] logMsg;
		Error("Shader program failed to link:\n%s", strLogMsg.c_str());
	}

	// Use program object
	glUseProgram(program);

	return program;
}
	
} // namespace GLJoe



#endif // DEF_GLJOE_INITSHADER_H

