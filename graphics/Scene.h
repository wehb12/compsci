#pragma once

#include "../../nclgl//SceneNode.h"
#include "../../nclgl/Frustum.h"
#include "../../nclgl/Camera.h"
//#include "../../nclgl/OGLRenderer.h"
#include <algorithm>

#define SHADOW_CASTING_LIGHTS 1

class OGLRenderer;

class Scene : public SceneNode
{
public:
	Scene(SceneNode* root = NULL, OGLRenderer* r = NULL);
	~Scene();

	void CatchKeyboard(float msec);

	virtual void Update(float msec);
	virtual void RenderScene();

	inline void SetRoot(SceneNode* root)	{ parent = root; }
	inline void SerRenderer(OGLRenderer* r) { renderer = r; }

	inline void AddLight(Light* l)			{ lights.push_back(l); }

	inline void SetCubeMap(GLuint cm)		{ cubeMap[0] = cm; }
	inline GLuint GetCubeMap()				{ return cubeMap[0]; }

	inline void SetAltCubeMap(GLuint cm)	{ cubeMap[1] = cm; }
	inline GLuint GetAltCubeMap()			{ return cubeMap[1]; }

	inline int NumberOfLights()				{ return lights.size(); }

protected:
	void BuildNodeLists(SceneNode* from);
	void SortNodeLists();
	void ClearNodeLists();
	void DrawNodes();
	void DrawNode(SceneNode * n);

	GLuint cubeMap[2];

	OGLRenderer* renderer;

	Camera* camera;

	vector<Light*> lights;

	Frustum		frameFrustum;

	vector<SceneNode*> transparentNodeList;
	vector<SceneNode*> nodeList;
};
