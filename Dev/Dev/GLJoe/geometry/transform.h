/**
 * @file GLJoe/geometry/transform.h
 *
 * @brief Transform matrices
 * 
 * @author Joachim Valente <joachim.valente@gmail.com
 * @date 6/13/2013
 */

#ifndef DEF_GLJOE_TRANSFORM_H
#define DEF_GLJOE_TRANSFORM_H

namespace GLJoe
{


/**
 * Non-class versions
 */
 
Mat4 Identity()
{
	return Mat4(
		1, 0, 0, 0,		
		0, 1, 0, 0,
		0, 0, 1, 0,
		0, 0, 0, 1
	);
}

Mat4 RotateX(const GLfloat theta)
{
	GLfloat angle = Radians(theta);
	Mat4 r = Identity();
	r[2][2] = r[1][1] = cos(angle);
	r[2][1] = sin(angle);
	r[1][2] = -r[2][1];
	
	return r;
}

Mat4 RotateY(const GLfloat theta)
{
	GLfloat angle = Radians(theta);
	Mat4 r = Identity();
	r[2][2] = r[0][0] = cos(angle);
	r[0][2] = sin(angle);
	r[2][0] = -r[0][2];
	
	return r;
}

Mat4 RotateZ(const GLfloat theta)
{
	GLfloat angle = Radians(theta);
	Mat4 r = Identity();
	r[1][1] = r[0][0] = cos(angle);
	r[1][0] = sin(angle);
	r[0][1] = -r[1][0];
	
	return r;
}

Mat4 Translate(const GLfloat x, const GLfloat y, const GLfloat z)
{
	Mat4 t = Identity();
	t[0][3] = x;
	t[1][3] = y;
	t[2][3] = z;
	
	return t;
}

Mat4 Translate(const Vec3& v)
{
	return Translate(v.x, v.y, v.z);
}

Mat4 Translate(const Vec4& v)
{
	return Translate(v.x, v.y, v.z);
}

Mat4 Scale(const GLfloat x, const GLfloat y, const GLfloat z)
{
	return Mat4(
		x, 0, 0, 0,
		0, y, 0, 0,
		0, 0, z, 0,
		0, 0, 0, 1
	);
}

Mat4 Scale(const GLfloat s)
{
	return Scale(s, s, s);
}

Mat4 Scale(const Vec3& v)
{
	return Scale(v.x, v.y, v.z);
}


/**
 * Class Transform
 */

class Transform
{
protected:
	Mat4 mat;
	
public:
	Transform()
	{
		setIdentity();
	}
	
	Transform& operator=(const Transform& mt)
	{
		mat = mt.mat;
		
		return *this;
	}
	
	Transform& operator=(const Mat4& m)
	{
		mat = m;
		
		return *this;
	}
	
	void setIdentity()
	{
		mat = Mat4(
			1, 0, 0, 0,		
			0, 1, 0, 0,
			0, 0, 1, 0,
			0, 0, 0, 1
		);
	}
	
	void rotateX(const GLfloat theta)
	{
		GLfloat angle = Radians(theta);
		Mat4 r = Identity();
		r[2][2] = r[1][1] = cos(angle);
		r[2][1] = sin(angle);
		r[1][2] = -r[2][1];
		
		mat *= r;
	}
	
	void rotateY(const GLfloat theta)
	{
		GLfloat angle = Radians(theta);
		Mat4 r = Identity();
		r[2][2] = r[0][0] = cos(angle);
		r[0][2] = sin(angle);
		r[2][0] = -r[0][2];
		r[3][3] = 1.0;
		
		mat *= r;
	}
	
	void rotateZ(const GLfloat theta)
	{
		GLfloat angle = Radians(theta);
		Mat4 r = Identity();
		r[1][1] = r[0][0] = cos(angle);
		r[1][0] = sin(angle);
		r[0][1] = -r[1][0];
		r[3][3] = 1.0;
		
		mat *= r;
	}
	
	void translate(const GLfloat x, const GLfloat y, const GLfloat z)
	{
		mat[0][3] += x;
		mat[1][3] += y;
		mat[2][3] += z;
	}
	
	void translate(const Vec3& v)
	{
		translate(v.x, v.y, v.z);
	}
	
	void translate(const Vec4& v)
	{
		translate(v.x, v.y, v.z);
	}
	
	void scale(const GLfloat x, const GLfloat y, const GLfloat z)
	{
		mat *= Mat4(
			x, 0, 0, 0,
			0, y, 0, 0,
			0, 0, z, 0,
			0, 0, 0, 1
		);
	}
	
	void scale(const GLfloat s)
	{
		scale(s, s, s);
	}
	
	void scale(const Vec3& v)
	{
		scale(v.x, v.y, v.z);
	}
	
	const Mat4& t() const
	{
		return mat;
	}
	
	operator const GLfloat*() const
	{
		return (const GLfloat*) mat;
	}
	
	operator GLfloat*()
	{
		return (GLfloat*) mat;
	}
	
};

typedef Stack_<Transform> MatrixStack;

} // namespace GLJoe

#endif // DEF_GLJOE_TRANSFORM_H

