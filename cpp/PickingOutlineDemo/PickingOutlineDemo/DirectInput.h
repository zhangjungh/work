///////////////////////////////////////////////////////////////////////////////
// FileName:	DirectInput.h
// Author:		JZhang
// Created:		2012-11-27
// Modified:	2012-11-29
// Purpose:		DX Input message including keyboard and mouse
//				please init it after the instance and window handle have been created
///////////////////////////////////////////////////////////////////////////////

#pragma once

#define DIRECTINPUT_VERSION 0x0800
#include <dinput.h>

class D3DApp;

class DirectInput
{
public:
	DirectInput(D3DApp* pApp, DWORD keyboardCoopFlags, DWORD mouseCoopFlags);
	~DirectInput();

	void					Poll();							//print status
	bool					KeyDown(char key);				//test if keydown
	bool					MouseButtonDown(int button);	//test if mouse down
	float					MouseDX();
	float					MouseDY();
	float					MouseDZ();

private:
	//non-copy
	DirectInput(const DirectInput& rhs);
	DirectInput& operator=(const DirectInput& rhs);
		
private:
	IDirectInput8*       mDInput;

	IDirectInputDevice8* mKeyboard;
	char                 mKeyboardState[256]; 

	IDirectInputDevice8* mMouse;
	DIMOUSESTATE2        mMouseState;
};