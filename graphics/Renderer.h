#pragma once

#include "../../nclgl/OGLRenderer.h"
#include "../../nclgl/Camera.h"
#include "../../nclgl/SceneNode.h"
#include "../../nclgl/Frustum.h"
#include "../../nclgl/HeightMap.h"
#include <algorithm>

class Renderer : public OGLRenderer
{
public:
	Renderer(Window & parent);
	virtual ~Renderer(void);

	virtual void UpdateScene(float msec);
	virtual void RenderScene();

	void SetHeightMapSnow(float offset) { heightMap->SetOffset(offset); }

protected:
	void BuildNodeLists(SceneNode* from);
	void SortNodeLists();
	void ClearNodeLists();
	void DrawNodes();
	void DrawNode(SceneNode * n);

	void DrawHeightmap();
	void DrawWater();
	void DrawMirror();
	void DrawSkybox();

	Shader* lightShader;
	Shader* reflectShader;
	Shader* skyboxShader;
	Shader* mirrorShader;

	SceneNode*	root;
	Camera*		camera;
	HeightMap*	heightMap;
	Mesh*		quad;
	Light*		light;

	GLuint		cubeMap;
	GLuint		cubeMap2;

	float		waterRotate;

	Frustum		frameFrustum;

	vector<SceneNode*> transparentNodeList;
	vector<SceneNode*> nodeList;
};
