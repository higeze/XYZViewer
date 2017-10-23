#pragma once
class COGLRenderer;

struct OGLPaintEventArgs
{
	COGLRenderer* RendererPtr;
	OGLPaintEventArgs(COGLRenderer* rendererPtr):RendererPtr(rendererPtr){}
	virtual ~OGLPaintEventArgs(){}
};