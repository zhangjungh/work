///////////////////////////////////////////////////////////////////////////////
// FileName:	SceneGraph.h
// Author:		JZhang
// Created:		2012-11-28
// Modified:	2012-11-29
// Purpose:		SceneGraph is the manager of the game for all resoruce
//				the object, light, effect, camera, input and so on
//				it is a singleton class because each game would only have one
//				scene manager. please be sure to init D3DApp first
//				the picking happens in Update, better use octree to manage
//				all the objects in real situation
///////////////////////////////////////////////////////////////////////////////
#pragma once

#include "D3DApp.h"
#include "Object.h"
#include "GfxStats.h"

class DirectInput;

class SceneGraph : public D3DApp
{
public:
	static SceneGraph* Instance(void) {				//singleton to hold only one instance
		static SceneGraph s;
		return &s;
	}

	~SceneGraph();

	void						Init(void);			//init the scene elements
	void						Update(float dt);	//update all elements
	void						Render(void);		//render objects
	
	bool						CheckDeviceCaps();	//check the caps, vs2.0 and ps2.0 are enough
	void						OnLostDevice();
	void						OnResetDevice();
protected:
	SceneGraph() : m_pCamera(0), m_pEffect(0), m_pInput(0), m_pGfxStats(0) {}	//protected constructor

private:
	//non copy
	SceneGraph(const SceneGraph& s);
	SceneGraph& operator=(const SceneGraph& s);

	vector<D3DObject*>			m_vSceneObjs;		//hold all the ingame objects
	
	DirectInput*				m_pInput;			//the dx input 
	Camera*						m_pCamera;			//camera
	Effect*						m_pEffect;			//effect that objects need
	GfxStats*					m_pGfxStats;		//statistic info
	DirLight					m_sLight;			//the light
};