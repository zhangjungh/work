#pragma once

#include "D3DApp.h"
#include "Object.h"
#include "GfxStats.h"

class DirectInput;

class SceneGraph : public D3DApp
{
public:
	static SceneGraph* Instance(void) {
		static SceneGraph s;
		return &s;
	}

	~SceneGraph();

	void						Init(void);
	void						Update(float dt);
	void						Render(void);
	
	bool						CheckDeviceCaps();
	void						OnLostDevice();
	void						OnResetDevice();
protected:
	SceneGraph() : m_pCamera(0), m_pEffect(0), m_pInput(0), m_pGfxStats(0) {}

private:
	SceneGraph(const SceneGraph& s);
	SceneGraph& operator=(const SceneGraph& s);

	vector<D3DObject*>			m_vSceneObjs;
	
	DirectInput*				m_pInput;
	Camera*						m_pCamera;
	Effect*						m_pEffect;
	GfxStats*					m_pGfxStats;
	DirLight					m_sLight;
};