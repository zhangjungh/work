// File Created by JZhang[zhangjun.cn@gmail.com] for solving following question
//=============================================================================
//Imaginary 3D UI, which has two (2) buttons and a list of 3D objects.
//
//    - OpenGL
//    - Per pixel shading
//    - 3D picking (recognizing if user has clicked 3D object)
//    - Add and Remove 3D buttons
//      * Add Button executes adding a new 3D object to a list
//      * Remove Button again removes the last 3D object from the list
//    - As 3D objects you can use generated models (box, sphere, etc.)
//
//- We require executable Win32 and source code 
//=============================================================================
//Environment of my computer
//OS: Microsoft Vista Home Basic with SP2
//IDE: Microsoft Visual C++ 2008 Express Edition with SP1
//OpenGL Libs: GLUT/GLEW/CWC
//=============================================================================
//functionalities:
//Mouse wheel: zoom in/out
//Right mouse button drag: move the objects in scene
//Left mouse click: choose object in scene
//'a' / add 3D button: add object
//'d' / del 3D button: delete the last object
//'r': reset objects position in scene
//'s': using shader
//'f': using fixed pipeline

#include "ui3d.h"

int main(void)
{
	UI3DApp*		pApp = new UI3DApp;
	UI3DWindow*		pWin = new UI3DWindow();

	pApp->run();

	delete pApp;
	delete pWin;
	return 0;
}

