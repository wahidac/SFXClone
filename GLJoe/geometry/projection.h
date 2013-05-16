/**
 * @file GLJoe/geometry/projection.h
 * 
 * @brief Helper projection functions
 * 
 * @author Joachim Valente <joachim.valente@gmail.com>
 * @date 6/13/2013
 */

#ifndef DEF_GLJOE_PROJECTION_H
#define DEF_GLJOE_PROJECTION_H

namespace GLJoe
{

Mat4 Ortho(const GLfloat left, const GLfloat right,
	const GLfloat bottom, const GLfloat top,
	const GLfloat zNear, const GLfloat zFar)
{
	Mat4 c;
	c[0][0] = 2.0 / (right - left);
	c[1][1] = 2.0 / (top - bottom);
	c[2][2] = 2.0 / (zNear - zFar);
	c[3][3] = 1.0;
	c[0][3] = -(right + left) / (right - left);
	c[1][3] = -(top + bottom) / (top - bottom);
	c[2][3] = -(zFar + zNear) / (zFar - zNear);
	return c;
}

Mat4 Ortho2D(const GLfloat left, const GLfloat right,
	const GLfloat bottom, const GLfloat top)
{
	return Ortho(left, right, bottom, top, -1.0, 1.0);
}

Mat4 Frustum(const GLfloat left, const GLfloat right,
	const GLfloat bottom, const GLfloat top,
	const GLfloat zNear, const GLfloat zFar)
{
	Mat4 c;
	c[0][0] = 2.0 * zNear / (right - left);
	c[0][2] = (right + left) / (right - left);
	c[1][1] = 2.0 * zNear / (top - bottom);
	c[1][2] = (top + bottom) / (top - bottom);
	c[2][2] = -(zFar + zNear) / (zFar - zNear);
	c[2][3] = -2.0 * zFar * zNear / (zFar - zNear);
	c[3][2] = -1.0;
	c[3][3] = 0.0;
	return c;
}

Mat4 Perspective(const GLfloat fovy, const GLfloat aspect,
	const GLfloat zNear, const GLfloat zFar)
{
	GLfloat top   = tan(Radians(fovy) / 2) * zNear;
	GLfloat right = top * aspect;

	Mat4 c;
	c[0][0] = zNear / right;
	c[1][1] = zNear / top;
	c[2][2] = -(zFar + zNear) / (zFar - zNear);
	c[2][3] = -2.0 * zFar * zNear / (zFar - zNear);
	c[3][2] = -1.0;
	return c;
}

Mat4 LookAt(const Vec4& eye, const Vec4& at, const Vec4& up)
{
	Vec4 n = (eye - at).normalize();
	Vec3 uu = (cross(up, n)).normalize();
	Vec4 u = Vec4(uu.x, uu.y, uu.z, 0.0);
	Vec3 vv = (cross(n, u)).normalize();
	Vec4 v = Vec4(vv.x, vv.y, vv.z, 0.0);
	Vec4 t = Vec4(0.0, 0.0, 0.0, 1.0);
	Mat4 c = Mat4(u, v, n, t);
	return c * Translate(-eye);
}

} // namespace GLJoe


#endif // DEF_GLJOE_PROJECTION_H
