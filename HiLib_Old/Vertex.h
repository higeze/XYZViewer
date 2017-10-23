#pragma once

//CVertex
struct CVertex
{
	CVertex(GLfloat xArg = 0.0f, GLfloat yArg = 0.0f, GLfloat zArg = 0.0f):x(xArg),y(yArg),z(zArg){}
	GLfloat x, y, z;
	void SetVertex(GLfloat xArg, GLfloat yArg, GLfloat zArg)
	{x=xArg;y=yArg;z=zArg;}
};