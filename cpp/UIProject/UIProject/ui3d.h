//=============================================================================
// File:	ui3d.h
// Author:	JZhang
// Time:	2011/06/12
// 
// purpose: generate subclass from SDK library to build the 3D UI
//
//=============================================================================
#ifndef __UI3D_H__
#define __UI3D_H__

//=============================================================================
//the includes
//=============================================================================
#include <GL/glew.h>
#include <GL/freeglut.h>
#include "glApplication.h"
#include "glutWindow.h"
#include "glsl.h"
#include <time.h>

//=============================================================================
//the Teapot object management class
//=============================================================================
class Object
{
public:
	Object(void) { fx = fy = fz = 0; name = -1; }
	virtual ~Object(void) {}
	void	Init(float x, float y, float z, size_t n) { fx = x, fy = y, fz = z; name = n;}
	float	GetX(void) { return fx; }
	float	GetY(void) { return fy; }
	float	GetZ(void) { return fz; }
	int		GetName(void) { return name; }

private:
	float	fx;
	float	fy;
	float	fz;
	size_t	name;
};

//=============================================================================
//the button class
//=============================================================================
class Button
{
public:
	Button(void) : m_iName(-1), m_uiTexture(0), m_fRatio(0.0), m_iSign(1) {}
	virtual ~Button(void) { if (m_uiTexture != 0) glDeleteTextures( 1, &m_uiTexture ); }
	void		Init(float x, float y, float z, int name) { m_fx = x, m_fy = y, m_fz = z; m_iName = name; }
	GLuint		LoadTextureRAW(const char* filename, int width, int height);
	void		Render(void);
private:
	int			m_iName;
	float		m_fx;
	float		m_fy;
	float		m_fz;
	GLuint		m_uiTexture;
	float		m_fRatio;
	int			m_iSign;
};

//=============================================================================
//the main window subclass
//=============================================================================
class UI3DWindow : public cwc::glutWindow
{
public:
	UI3DWindow(void) : m_fEyeX(0.0f), m_fEyeY(0.0f), m_fEyeZ(10.0f) { m_strMsg[0] = '\0'; }
	virtual ~UI3DWindow(void);

	//render
	virtual void OnRender(void);

	//init function
	virtual void OnInit();

	//resize window
	virtual void OnResize(int w, int h);

	virtual void OnIdle() {}	
	virtual void OnClose(void) {}
	//mouse functions
	virtual void OnMouseDown(int button, int x, int y);   
	virtual void OnMouseUp(int button, int x, int y);
	virtual void OnMouseWheel(int nWheelNumber, int nDirection, int x, int y);
	virtual void OnRightMouseDrag(int x, int y);
	//key functions
	virtual void OnKeyDown(int nKey, char cAscii);
	virtual void OnKeyUp(int nKey, char cAscii);

	//light mode function
	void		 DemoLight(void);

	void		 AddObject(void);
	void		 DelObject(void);
	void		 Reset(void);
private:
	void		 SetLookAt(float x, float y, float z);
	void		 TextDraw(float x, float y, const char* str);

private:
	cwc::glShaderManager	m_kGLSM;
	cwc::glShader*			m_pKShader;
	GLuint					m_uiProgramObject;
	float					m_fEyeX;
	float					m_fEyeY;
	float					m_fEyeZ;
	int						m_iRBPosX;
	int						m_iRBPosY;
	std::list<Object*>		m_listPtrObject;
	char					m_strMsg[128];
	Button					m_btnAdd;
	Button					m_btnDel;
};

//=============================================================================
//the application subclass
//=============================================================================
class UI3DApp : public cwc::glApplication
{
public:
	virtual void OnInit();
};

#endif