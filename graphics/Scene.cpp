#include "Scene.h"

Scene::Scene(SceneNode* root, OGLRenderer* r) : SceneNode()
{
	camera = new Camera(15.0f, -60.0f, 0.0f, Vector3(1600, 200, 3500));

	this->parent = root;
	cubeMap[0] = 0;
	cubeMap[1] = 0;

	renderer = r;

	renderer->projMatrix = Matrix4::Perspective(1.0f, 15000.0f, (float)r->width / (float)r->height, 45.0f);
}

Scene::~Scene()
{
	delete camera;
}

void Scene::Update(float msec)
{
	camera->UpdateCamera(msec);
	renderer->viewMatrix = camera->BuildViewMatrix();
	CatchKeyboard(msec);
	frameFrustum.FromMatrix(renderer->projMatrix * renderer->viewMatrix);
	SceneNode::Update(msec);
}

void Scene::CatchKeyboard(float msec)
{
	if (children.front()->GetHeightMap()->GetSnowState())
	{
		float offset = children.front()->GetHeightMap()->GetOffset();

		if (Window::GetKeyboard()->KeyDown(KEYBOARD_UP))
			children.front()->GetHeightMap()->SetOffset(offset + 0.01f);
		if (Window::GetKeyboard()->KeyDown(KEYBOARD_DOWN))
			children.front()->GetHeightMap()->SetOffset(offset + 0.01f);

		offset = children.front()->GetHeightMap()->GetOffset();
		if (offset > 1.2f)
			children.front()->GetHeightMap()->SetOffset(1.2f);
		if (offset < -1.0f)
			children.front()->GetHeightMap()->SetOffset(-1.0f);

		children.front()->SetOffset(offset);
	}

	if (Window::GetKeyboard()->KeyDown(KEYBOARD_B))
	{
		delete children[0]->GetShader();
		children[0]->SetShader(new Shader("../../Shaders/BumpVertex.glsl", "../../Shaders/BumpFragment.glsl"));
		children[0]->GetShader()->LinkProgram();

		SceneNode* water = (*children[0]->GetChildIteratorStart());
		delete water->GetShader();
		water->SetShader(new Shader("../../Shaders/BumpVertex.glsl", "../../Shaders/reflectFragment.glsl"));
		water->GetShader()->LinkProgram();
	}
}

void Scene::BuildNodeLists(SceneNode* from)
{
	if (frameFrustum.InsideFrustum(*from))
	{
		Vector3 dir = from->GetWorldTransform().GetPositionVector() - camera->GetPosition();
		from->SetCameraDistance(Vector3::Dot(dir, dir));

		if (from->IsTransparent())
			transparentNodeList.push_back(from);
		else
			nodeList.push_back(from);

		for (vector<SceneNode*>::const_iterator i = from->GetChildIteratorStart(); i != from->GetChildIteratorEnd(); ++i)
			BuildNodeLists((*i));
	}
}

void Scene::SortNodeLists()
{
	std::sort(transparentNodeList.begin(), transparentNodeList.end(), SceneNode::CompareByCameraDistance);
	std::sort(nodeList.begin(), nodeList.end(), SceneNode::CompareByCameraDistance);
}

void Scene::DrawNodes()
{
	for (vector<SceneNode*>::const_iterator i = nodeList.begin(); i != nodeList.end(); ++i)
		DrawNode((*i));

	for (vector<SceneNode*>::const_reverse_iterator i = transparentNodeList.rbegin(); i != transparentNodeList.rend(); ++i)
		DrawNode((*i));
}

void Scene::DrawNode(SceneNode* n)
{
	if (n->GetMesh())
	{
		Scene* s = dynamic_cast<Scene*>(n);
		if (n->GetShader())
			renderer->SetCurrentShader(n->GetShader());
		Mesh* mesh = n->GetMesh();

		renderer->modelMatrix = n->GetWorldTransform() * Matrix4::Scale(n->GetModelScale());

		glUniform3fv(glGetUniformLocation(renderer->currentShader->GetProgram(), "cameraPos"), 1, (float*)&camera->GetPosition());

		if (n->GetHeightMap())
		{
			if (n->GetHeightMap()->GetSnowState())
			{
				glUniform1i(glGetUniformLocation(renderer->currentShader->GetProgram(), "snowTex"), 0);
				glUniform1i(glGetUniformLocation(renderer->currentShader->GetProgram(), "snowTex"), 0);
				glUniform1f(glGetUniformLocation(renderer->currentShader->GetProgram(), "offset"), n->GetHeightMap()->GetOffset());
			}
			renderer->textureMatrix.ToIdentity();
		}
		if (mesh->GetTexture())
			glUniform1i(glGetUniformLocation(renderer->currentShader->GetProgram(), "diffuseTex"), 1);
		if (mesh->GetBumpMap())
			glUniform1i(glGetUniformLocation(renderer->currentShader->GetProgram(), "bumpTex"), 2);
		if (mesh->GetSpecMap())
			glUniform1i(glGetUniformLocation(renderer->currentShader->GetProgram(), "specTex"), 3);
		if (mesh->GetGlossMap())
			glUniform1i(glGetUniformLocation(renderer->currentShader->GetProgram(), "glossTex"), 4);

		glUniform1i(glGetUniformLocation(renderer->currentShader->GetProgram(), "cubeTex"), 5);
		glActiveTexture(GL_TEXTURE5);
		glBindTexture(GL_TEXTURE_CUBE_MAP, cubeMap[0]);

		if (n->UseOffset())
			renderer->textureMatrix = Matrix4::Rotation(n->GetOffset(), Vector3(0.0f, 0.0f, 1.0f));

		if (n->UseAltHeightMap())
		{
			glUniform1i(glGetUniformLocation(renderer->currentShader->GetProgram(), "cubeTex"), 6);

			glActiveTexture(GL_TEXTURE6);
			glBindTexture(GL_TEXTURE_CUBE_MAP, cubeMap[1]);
		}

		if (NumberOfLights())
		{
			if (NumberOfLights() <= SHADOW_CASTING_LIGHTS)
				renderer->SetShaderLight(*lights[0]);
			else; //deffered rendering
		}

		renderer->UpdateShaderMatrices();
		n->Draw();
		glUseProgram(0);
	}
}

void Scene::RenderScene()
{
	BuildNodeLists(children[0]);
	SortNodeLists();

	renderer->SetCurrentShader(shader);

	glDepthMask(GL_FALSE);
	glDisable(GL_CULL_FACE);
	renderer->UpdateShaderMatrices();

	mesh->Draw();

	glUseProgram(0);
	glDepthMask(GL_TRUE);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);

	DrawNodes();

	ClearNodeLists();
}

void Scene::ClearNodeLists()
{
	transparentNodeList.clear();
	nodeList.clear();
}
