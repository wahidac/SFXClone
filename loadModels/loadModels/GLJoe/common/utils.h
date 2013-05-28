/**
 * @file GLJoe/common/utils.h
 * 
 * @brief Useful general-purpose methods and macros
 * 
 * @author Joachim Valente <joachim.valente@gmail.com>
 * @date 6/13/2013
 */

#ifndef DEF_GLJOE_UTILS_H
#define DEF_GLJOE_UTILS_H

#define BUFFER_OFFSET(offset) ((GLvoid*) (offset))

namespace GLJoe
{
	inline GLfloat Radians(const GLfloat theta)
	{
		return theta * M_PI / 180.0;
	}
	
	
	inline float Random(float a, float b)
	{
		float random = ((float) rand()) / (float) RAND_MAX;
		float diff = b - a;
		float r = random * diff;
		return a + r;
	}
	
	inline int RandomInt(int a, int b)
	{
		return static_cast<int>(Random(a, b));
	}
} // namespace GLJoe

#endif // DEF_GLJOE_UTILS_H
