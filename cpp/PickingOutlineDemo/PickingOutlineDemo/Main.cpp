///////////////////////////////////////////////////////////////////////////////
// FileName:	CPUInfo.h
// Author:		JZhang
// Created:		2012-11-27
// Modified:	2012-11-29
// Purpose:		WinMain function, program entry
///////////////////////////////////////////////////////////////////////////////
#include "SceneGraph.h"


int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE prevInstance,
				   PSTR cmdLine, int showCmd)
{
	// Enable run-time memory check for debug builds.
	#if defined(DEBUG) | defined(_DEBUG)
		_CrtSetDbgFlag( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );
	#endif

	SceneGraph *pScene = SceneGraph::Instance();
	//init base first!
	pScene->InitBase(hInstance, "JZhang_Picking_Demo", D3DDEVTYPE_HAL, D3DCREATE_HARDWARE_VERTEXPROCESSING);
	pScene->SetD3DApp(pScene);
	pScene->Init();
	pScene->Run();
}