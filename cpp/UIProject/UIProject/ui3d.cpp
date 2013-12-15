//=============================================================================
// File:	ui3d.cpp
// Author:	JZhang
// Time:	2011/06/12
// 
// purpose: implementation of the UI3D window and application class
//
//=============================================================================
#include "ui3d.h"
#include <iostream>

const float cfDelta = 0.3f;	//zoom in/out delta value
const float cfRatio = 0.1f;	//move ratio
const float cfLookZ = 35.0f;//proper z value to look at the scene
const size_t cuiMax = 35;	//max objects user could create
const size_t cuiOiR = 7;	//max object in row
const char* cstrTip = "click button to add or remove object, left mouse click to pick";
const size_t cuiAdd = 900;	//button add name
const size_t cuiDel = 901;	//button delete name

UI3DWindow::~UI3DWindow(void)
{
	std::list<Object*>::iterator iter = m_listPtrObject.begin();
	for (; iter != m_listPtrObject.end(); ++iter)
	{
		delete *iter;
	}
}

void UI3DWindow::OnRender(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glInitNames();
	glPushName(0);

	glEnable(GL_LIGHTING);

	glPushMatrix();
	if (m_pKShader != NULL)
	{
		m_pKShader->begin();
	}
	//scene objects
	std::list<Object*>::iterator iter = m_listPtrObject.begin();
	for (; iter != m_listPtrObject.end(); ++iter)
	{
		glLoadName((*iter)->GetName()); //load name for picking
		glTranslatef((*iter)->GetX(), (*iter)->GetY(), (*iter)->GetZ());
		glutSolidTeapot(1.0);
	}
	if (m_pKShader != NULL) 
	{
		m_pKShader->end();
	}
	glPopMatrix();

	glDisable(GL_LIGHTING);
	glColor3f(0.0f, 0.0f, 0.0f);
	//texts and buttons should on fixed postion
	glPushMatrix();
	glTranslatef( m_fEyeX, m_fEyeY, m_fEyeZ - cfLookZ );
	//draw text
	TextDraw(-18.5f, -13.5f, cstrTip);
	TextDraw(-18.5f, -12.0f, m_strMsg);
	//draw button
	m_btnAdd.Render();
	m_btnDel.Render();
	glPopMatrix();

	glutSwapBuffers();

	Repaint();
}

void UI3DWindow::TextDraw(float x, float y, const char* str)
{
	glRasterPos2f(x, y); //using expression to calc is better
	for (size_t i=0; i<strlen(str); ++i)
	{
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, str[i]);
	}
}

// When OnInit is called, a render context (in this case GLUT-Window) 
// is already available!
void UI3DWindow::OnInit()
{
	glClearColor(0.5f, 0.5f, 1.0f, 0.0f);
	glShadeModel(GL_SMOOTH);
	glEnable(GL_DEPTH_TEST);

	m_pKShader = m_kGLSM.loadfromFile("vertexshader.txt","fragmentshader.txt"); // load (and compile, link) from file
	if (m_pKShader == NULL) 
	{
		std::cout << "Error Loading, compiling or linking shader\n";
	}
	else
	{
		m_uiProgramObject = m_pKShader->GetProgramObject();
	}

	m_iRBPosX = -1;
	m_iRBPosY = -1;

	//add one object into the scene
	AddObject();

	DemoLight();

	//after light set zoom in the scene a little bit
	m_fEyeZ = cfLookZ;
	SetLookAt(m_fEyeX, m_fEyeY, m_fEyeZ);

	m_btnAdd.Init(16.0, -10.5f, 0.0, cuiAdd);
	m_btnAdd.LoadTextureRAW("add.raw", 80, 20);
	m_btnDel.Init(16.0, -13.0f, 0.0, cuiDel);
	m_btnDel.LoadTextureRAW("del.raw", 80, 20);
}

void UI3DWindow::OnResize(int w, int h)
{
	if (h == 0) h = 1;
	float ratio = 1.0f * (float)w / (float)h;

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	glViewport(0, 0, w, h);

	gluPerspective(45.0f, ratio, 1.0f, 100.0f);
	glMatrixMode(GL_MODELVIEW);
	SetLookAt(m_fEyeX, m_fEyeY, m_fEyeZ);
}

void UI3DWindow::SetLookAt(float x, float y, float z)
{
	glLoadIdentity();
	gluLookAt(x, y, z, x, y, -1.0f, 0.0f, 1.0f, 0.0f);
}

void UI3DWindow::Reset(void)
{
	m_fEyeX = m_fEyeY = 0.0f;
	m_fEyeZ = 35.0f;
	SetLookAt(m_fEyeX, m_fEyeY, m_fEyeZ);
}

void UI3DWindow::OnMouseDown(int button, int x, int y)
{
	if (button == 0) //left button down
	{
		GLuint	buffer[512];
		GLint	hits;

		GLint	viewport[4];
		glGetIntegerv(GL_VIEWPORT, viewport);
		glSelectBuffer(512, buffer);

		glRenderMode(GL_SELECT);

		glInitNames();
		glPushName(0);

		glMatrixMode(GL_PROJECTION);
		glPushMatrix();
		glLoadIdentity();

		// This Creates A Matrix That Will Zoom Up To A Small Portion Of The Screen, Where The Mouse Is.
		gluPickMatrix((GLdouble)x, (GLdouble) (viewport[3]-y), 1.0f, 1.0f, viewport);

		// Apply The Perspective Matrix
		gluPerspective(45.0f, (GLfloat) (viewport[2]-viewport[0])/(GLfloat) (viewport[3]-viewport[1]), 1.0f, 100.0f);
		glMatrixMode(GL_MODELVIEW);
		OnRender();
		glMatrixMode(GL_PROJECTION);
		glPopMatrix();
		glMatrixMode(GL_MODELVIEW);
		hits=glRenderMode(GL_RENDER);
		//std::cout << "hits: " << hits << std::endl;

		if (hits > 0) 
		{
			if (buffer[3] == cuiAdd)
			{
				AddObject();
			}
			else if (buffer[3] == cuiDel)
			{
				DelObject();
			}
			else
			{
				//std::cout << "choose: " << buffer[3] << std::endl;
				sprintf_s(m_strMsg, sizeof(m_strMsg)/sizeof(char), "chosen object number: %d", buffer[3]);
			}
		}
		else
		{
			sprintf_s(m_strMsg, sizeof(m_strMsg)/sizeof(char), "chosen object: none");
		}
	}
	else if (button == 2) //right button down, save the position
	{
		m_iRBPosX = x;
		m_iRBPosY = y;
	}
}

void UI3DWindow::OnMouseUp(int button, int x, int y)
{
	if (button == 2) //right button up
	{
		m_iRBPosX = -1;
		m_iRBPosY = -1;
	}
}

void UI3DWindow::OnMouseWheel(int nWheelNumber, int nDirection, int x, int y)
{
	if (nDirection > 0)
	{
		m_fEyeZ += cfDelta;
		if (m_fEyeZ > 100.0f)
			m_fEyeZ = 100.0f;
	}
	else
	{
		m_fEyeZ -= cfDelta;
		if (m_fEyeZ < 1.0f)
			m_fEyeZ = 1.0f;
	}
	SetLookAt(m_fEyeX, m_fEyeY, m_fEyeZ);
	//std::cout << "z: " << m_fEyeZ << std::endl;	
}

void UI3DWindow::OnRightMouseDrag(int x, int y)
{
	if (m_iRBPosX != -1 && m_iRBPosY != -1)
	{
		int deltaX = x - m_iRBPosX;
		int deltaY = y - m_iRBPosY;
		m_fEyeX -= (cfRatio * deltaX);
		m_fEyeY += (cfRatio * deltaY);
		m_iRBPosX = x;
		m_iRBPosY = y;
		SetLookAt(m_fEyeX, m_fEyeY, m_fEyeZ);
		//std::cout << "x: " << m_fEyeX << "y: " << m_fEyeY << std::endl;	
	}

}

void UI3DWindow::OnKeyDown(int nKey, char cAscii)
{       
	if (cAscii == 27) // 0x1b = ESC
	{
		this->Close(); // Close Window!
	} 
}

void UI3DWindow::OnKeyUp(int nKey, char cAscii)
{
	if (cAscii == 's')      //use shader
		m_pKShader->enable();
	else if (cAscii == 'f') //fixed pipeline
		m_pKShader->disable();
	else if (cAscii == 'a') //add obbject
		AddObject();
	else if (cAscii == 'd') //delete object
		DelObject();
	else if (cAscii == 'r') //reset secene
		Reset();
}

void UI3DWindow::DemoLight(void)
{
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_NORMALIZE);

	// Light model parameters:
	// -------------------------------------------

	GLfloat lmKa[] = {0.0, 0.0, 0.0, 0.0 };
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, lmKa);

	glLightModelf(GL_LIGHT_MODEL_LOCAL_VIEWER, 1.0);
	glLightModelf(GL_LIGHT_MODEL_TWO_SIDE, 0.0);

	// -------------------------------------------
	// Spotlight Attenuation

	GLfloat spot_direction[] = {1.0, -1.0, -1.0 };
	GLint spot_exponent = 30;
	GLint spot_cutoff = 180;

	glLightfv(GL_LIGHT0, GL_SPOT_DIRECTION, spot_direction);
	glLighti(GL_LIGHT0, GL_SPOT_EXPONENT, spot_exponent);
	glLighti(GL_LIGHT0, GL_SPOT_CUTOFF, spot_cutoff);

	GLfloat Kc = 1.0;
	GLfloat Kl = 0.0;
	GLfloat Kq = 0.0;

	glLightf(GL_LIGHT0, GL_CONSTANT_ATTENUATION,Kc);
	glLightf(GL_LIGHT0, GL_LINEAR_ATTENUATION, Kl);
	glLightf(GL_LIGHT0, GL_QUADRATIC_ATTENUATION, Kq);


	// ------------------------------------------- 
	// Lighting parameters:

	GLfloat light_pos[] = {0.0f, 5.0f, 5.0f, 1.0f};
	GLfloat light_Ka[]  = {1.0f, 0.5f, 0.5f, 1.0f};
	GLfloat light_Kd[]  = {1.0f, 0.1f, 0.1f, 1.0f};
	GLfloat light_Ks[]  = {1.0f, 1.0f, 1.0f, 1.0f};

	glLightfv(GL_LIGHT0, GL_POSITION, light_pos);
	glLightfv(GL_LIGHT0, GL_AMBIENT, light_Ka);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light_Kd);
	glLightfv(GL_LIGHT0, GL_SPECULAR, light_Ks);

	// -------------------------------------------
	// Material parameters:

	GLfloat material_Ka[] = {0.5f, 0.0f, 0.0f, 1.0f};
	GLfloat material_Kd[] = {0.4f, 0.4f, 0.5f, 1.0f};
	GLfloat material_Ks[] = {0.8f, 0.8f, 0.0f, 1.0f};
	GLfloat material_Ke[] = {0.1f, 0.0f, 0.0f, 0.0f};
	GLfloat material_Se = 20.0f;

	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, material_Ka);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, material_Kd);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, material_Ks);
	glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, material_Ke);
	glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, material_Se);
}

void UI3DWindow::AddObject(void)
{
	size_t size = m_listPtrObject.size();
	if (size >= cuiMax)
	{
		sprintf_s(m_strMsg, sizeof(m_strMsg)/sizeof(char), "reach max count of objects: %d", cuiMax);
		std::cout << "reach max count of objects: " << cuiMax << std::endl;
	}
	else
	{
		float x, y, z = 0.0;
		if (size == 0) //empty list
		{
			x = -15.0f;
			y = 12.0f;
		}
		else
		{
			Object *l = m_listPtrObject.back();
			x = l->GetX();
			y = l->GetY();
			z = l->GetZ();
			if (size % cuiOiR == 0) //object in new row
			{
				x -= 5*(cuiOiR);
				y -= 5;
			}
			else
			{
				x = 5.0;
				y = 0.0;
			}
		}

		Object *p = new Object;
		p->Init(x, y, z, size+1);
		m_listPtrObject.push_back(p);
		sprintf_s(m_strMsg, sizeof(m_strMsg)/sizeof(char), "add object number: %d", size+1);
	}
}

void UI3DWindow::DelObject(void)
{
	if (!m_listPtrObject.empty())
	{
		sprintf_s(m_strMsg, sizeof(m_strMsg)/sizeof(char), "delete object number: %d", m_listPtrObject.size());
		m_listPtrObject.pop_back();
	}
	else
	{
		sprintf_s(m_strMsg, sizeof(m_strMsg)/sizeof(char), "no object to delete");
	}
}

//=============================================================================
void UI3DApp::OnInit(void)
{
	std::cout << "UI3D application started!\n";
}

//=============================================================================
GLuint Button::LoadTextureRAW(const char* filename, int width, int height)
{
	//read raw data from file
    unsigned char* data;
    FILE * file;
    fopen_s(&file, filename, "rb");
    if (file == NULL) return 0;
    data = (unsigned char*)malloc(width * height * 3); //rgb raw data
    fread(data, width * height * 3, 1, file);
    fclose(file);

	//build the texture
	glGenTextures(1, &m_uiTexture);
    glBindTexture(GL_TEXTURE_2D, m_uiTexture);
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    gluBuild2DMipmaps(GL_TEXTURE_2D, 3, width, height, GL_RGB, GL_UNSIGNED_BYTE, data);

    free(data);

    return m_uiTexture;
}

void Button::Render(void)
{
	if (m_uiTexture == 0) return;

	glEnable(GL_CULL_FACE);	
	glColor3f(1.0f, 1.0f, 1.0f);
	glLoadName(m_iName);

	glPushMatrix();

	glTranslatef(m_fx, m_fy, m_fz);
	//make it look like a button
	glScalef(3.0f, 1.0f, 0.5f);
	//rotate a little bit round y-axis each frame
	m_fRatio += m_iSign*0.0005f;
	if (m_fRatio >= 0.075 || m_fRatio <= -0.075) m_iSign *= -1;
	glRotatef(m_fRatio*360, 0.0f, 1.0f, 0.0f);
	//setup texture mapping
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, m_uiTexture);
	glBegin( GL_QUADS );
	//front face with texture
	glTexCoord2d(0.0, 0.0); 
	glVertex3d(-1.0, -1.0, 0.0);
	glTexCoord2d(1.0, 0.0); 
	glVertex3d(+1.0, -1.0, 0.0);
	glTexCoord2d(1.0, 1.0); 
	glVertex3d(+1.0, +1.0, 0.0);
	glTexCoord2d(0.0, 1.0); 
	glVertex3d(-1.0, +1.0, 0.0);

	//other four faces
	glVertex3d(-1.0, -1.0, 0.0);
	glVertex3d(-1.0, +1.0, 0.0);
	glVertex3d(-1.1, +1.1, 1.0);
	glVertex3d(-1.1, -1.1, 1.0);

	glVertex3d(+1.0, -1.0, 0.0);
	glVertex3d(+1.1, -1.1, 1.0);
	glVertex3d(+1.1, +1.1, 1.0);
	glVertex3d(+1.0, +1.0, 0.0);

	glVertex3d(+1.0, +1.0, 0.0);
	glVertex3d(+1.1, +1.1, 1.0);
	glVertex3d(-1.1, +1.1, 1.0);
	glVertex3d(-1.0, +1.0, 0.0);

	glVertex3d(-1.0, -1.0, 0.0);
	glVertex3d(-1.1, -1.1, 1.0);
	glVertex3d(+1.1, -1.1, 1.0);
	glVertex3d(+1.0, -1.0, 0.0);
	glEnd();

	glPopMatrix();

	glDisable(GL_CULL_FACE);	
}