#pragma once

#include "../../nclgl/OGLRenderer.h"
#include "../../nclgl/Camera.h"
#include "../../nclgl/SceneNode.h"
#include "../../nclgl/HeightMap.h"
#include "Scene.h"
#include <algorithm>

#define NUM_SCENES 1

class Renderer : public OGLRenderer
{
public:
	Renderer(Window & parent);
	virtual ~Renderer(void);

	virtual void UpdateScene(float msec);
	virtual void RenderScene();

	int GetWidth() { return width; }
	int GetHeight() { return height; }
protected:

	bool GenerateMountainScene(Scene &scene);

	SceneNode*	root;
	Scene*		scene[NUM_SCENES];
	int			currentScene;
	Camera*		camera;
};
