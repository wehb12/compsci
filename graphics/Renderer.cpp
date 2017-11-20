#include "Renderer.h"

Renderer::Renderer(Window & parent) : OGLRenderer(parent)
{
	camera = new Camera(15.0f, -60.0f, 0.0f, Vector3(1600, 200, 3500));
	quad = Mesh::GenerateQuad();

	reflectShader = new Shader("../../Shaders/BumpVertex.glsl", "../../Shaders/reflectFragment.glsl");
	skyboxShader = new Shader("../../Shaders/skyboxVertex.glsl", "../../Shaders/skyboxFragment.glsl");
	lightShader = new Shader("../../Shaders/BumpVertex.glsl", "../../Shaders/BumpFragment.glsl");
	mirrorShader = new Shader("../../Shaders/BumpVertex.glsl", "../../Shaders/MirrorFragment.glsl");

	if (!reflectShader->LinkProgram() || !skyboxShader->LinkProgram() || !lightShader->LinkProgram() || !mirrorShader->LinkProgram())
		return;

	root = new SceneNode();
	heightMap = new HeightMap("../../Textures/terrain.raw");
	root->SetMesh(heightMap);
	root->SetBoundingRadius(7500.0f);

	heightMap->SetTexture1(SOIL_load_OGL_texture("../../Textures/mud.jpg ", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS));
	heightMap->SetTexture2(SOIL_load_OGL_texture("../../Textures/snow.jpg ", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS));
	heightMap->SetBumpMap(SOIL_load_OGL_texture("../../Textures/mud_Normal.jpg ", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS));
	heightMap->SetSpecMap(SOIL_load_OGL_texture("../../Textures/mud_Specular.jpg ", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS));
	heightMap->SetGlossMap(SOIL_load_OGL_texture("../../Textures/mud_Gloss.jpg ", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS));
	if (!root->GetMesh()->GetTexture1() || !root->GetMesh()->GetTexture2() || !root->GetMesh()->GetBumpMap())
		return;
	SetTextureRepeating(heightMap->GetTexture1(), true);
	SetTextureRepeating(heightMap->GetTexture2(), true);
	SetTextureRepeating(heightMap->GetBumpMap(), true);

	quad->SetTexture1(SOIL_load_OGL_texture("../../Textures/water2.jpg", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS));
	if (!quad->GetTexture1())
		return;

	SetTextureRepeating(quad->GetTexture1(), true);

	SceneNode* s = new SceneNode(quad);
	root->AddChild(s);
	s->SetBoundingRadius(10000.0f);

	cubeMap2 = SOIL_load_OGL_cubemap("../../Textures/snowy_west.jpg", "../../Textures/snowy_east.jpg",
		"../../Textures/snowy_up.jpg", "../../Textures/snowy_down.jpg",
		"../../Textures/snowy_south.jpg", "../../Textures/snowy_north.jpg",
		SOIL_LOAD_RGB, SOIL_CREATE_NEW_ID, SOIL_FLAG_POWER_OF_TWO);
	//snow cube map from https://gamebanana.com/textures/1951

	cubeMap = SOIL_load_OGL_cubemap("../../Textures/rusted_west.jpg", "../../Textures/rusted_east.jpg",
		"../../Textures/rusted_up.jpg", "../../Textures/rusted_down.jpg",
		"../../Textures/rusted_south.jpg", "../../Textures/rusted_north.jpg",
		SOIL_LOAD_RGB, SOIL_CREATE_NEW_ID, SOIL_FLAG_POWER_OF_TWO);

	if (!cubeMap || !cubeMap2)
		return;

	//for (int i = 0; i < 4; ++i)
	//{
	//	int x = i % 2;
	//	int z = i / 2;
	//	if (i != 3)
	//		light[i] = new Light(Vector3((x * (RAW_HEIGHT * HEIGHTMAP_X / 2.0f) + (RAW_HEIGHT * HEIGHTMAP_X / 4.0f)), 500.0f,
	//		(z * (RAW_HEIGHT * HEIGHTMAP_Z / 2.0f) + (RAW_HEIGHT * HEIGHTMAP_Z / 4.0f))),
	//			Vector4(1, 1, 1, 1), 0.1f,//(RAW_WIDTH * HEIGHTMAP_X) / 2.0f,
	//			Vector4(1, 1, 1, 1));
	//	else
	light/*[i]*/ = new Light(Vector3((RAW_HEIGHT * HEIGHTMAP_X / 2.0f), 500.0f, -(RAW_HEIGHT * HEIGHTMAP_Z / 2.0f)),
		Vector4(1, 1, 1, 1), 2000.0f,
		Vector4(1, 1, 1, 1), 1,
		Vector3(-1, 2, -1), 40.0f);
	//}

	waterRotate = 0.0f;

	projMatrix = Matrix4::Perspective(1.0f, 15000.0f, (float)width / (float)height, 45.0f);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);
	init = true;
}

Renderer ::~Renderer(void)
{
	delete root;
	delete heightMap;
	delete camera;
	delete quad;
	delete reflectShader;
	delete lightShader;
	delete skyboxShader;
	currentShader = 0;
	//for (int i = 0; i < 4; ++i)
	delete light/*[i]*/;
}

void Renderer::UpdateScene(float msec) {
	camera->UpdateCamera(msec);
	viewMatrix = camera->BuildViewMatrix();
	frameFrustum.FromMatrix(projMatrix * viewMatrix);
	waterRotate += msec / 10000.0f;

	//root->Update(msec);
}

void Renderer::BuildNodeLists(SceneNode* from)
{
	if (frameFrustum.InsideFrustum(*from))
	{
		Vector3 dir = from->GetWorldTransform().GetPositionVector() - camera->GetPosition();
		from->SetCameraDistance(Vector3::Dot(dir, dir));

		if (from->GetColour().w < 1.0f)
			transparentNodeList.push_back(from);
		else
			nodeList.push_back(from);

		for (vector<SceneNode*>::const_iterator i = from->GetChildIteratorStart(); i != from->GetChildIteratorEnd(); ++i)
			BuildNodeLists((*i));
	}
}

void Renderer::SortNodeLists()
{
	std::sort(transparentNodeList.begin(), transparentNodeList.end(), SceneNode::CompareByCameraDistance);
	std::sort(nodeList.begin(), nodeList.end(), SceneNode::CompareByCameraDistance);
}

void Renderer::DrawNodes()
{
	for (vector<SceneNode*>::const_iterator i = nodeList.begin(); i != nodeList.end(); ++i)
		DrawNode((*i));

	for (vector<SceneNode*>::const_reverse_iterator i = transparentNodeList.rbegin(); i != transparentNodeList.rend(); ++i)
		DrawNode((*i));
}

void Renderer::DrawNode(SceneNode* n)
{
	if (n->GetMesh())
	{
		Matrix4 transform = n->GetWorldTransform() * Matrix4::Scale(n->GetModelScale());
		glUniformMatrix4fv(glGetUniformLocation(currentShader->GetProgram(), "modelMatrix"), 1, false, (float*)&transform);

		glUniform4fv(glGetUniformLocation(currentShader->GetProgram(), "nodeColour"), 1, (float*)&n->GetColour());

		n->Draw(*this);
	}
}

void Renderer::RenderScene()
{
	BuildNodeLists(root);
	SortNodeLists();

	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

	DrawSkybox();
	DrawHeightmap();
	DrawWater();
	DrawMirror();

	SwapBuffers();
	ClearNodeLists();
}

void Renderer::DrawSkybox()
{
	glDepthMask(GL_FALSE);
	glDisable(GL_CULL_FACE);
	SetCurrentShader(skyboxShader);
	UpdateShaderMatrices();

	quad->Draw();

	glUseProgram(0);
	glDepthMask(GL_TRUE);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
}

void Renderer::DrawHeightmap()
{
	SetCurrentShader(lightShader);
	SetShaderLight(*light);

	glUniform3fv(glGetUniformLocation(currentShader->GetProgram(), "cameraPos"), 1, (float*)&camera->GetPosition());

	glUniform1i(glGetUniformLocation(currentShader->GetProgram(), "diffuseTex"), 0);
	glUniform1i(glGetUniformLocation(currentShader->GetProgram(), "snowTex"), 1);
	glUniform1i(glGetUniformLocation(currentShader->GetProgram(), "bumpTex"), 2);
	glUniform1i(glGetUniformLocation(currentShader->GetProgram(), "specTex"), 3);
	glUniform1i(glGetUniformLocation(currentShader->GetProgram(), "glossTex"), 4);

	glUniform1f(glGetUniformLocation(currentShader->GetProgram(), "offset"), heightMap->GetOffset());

	modelMatrix.ToIdentity();
	textureMatrix.ToIdentity();

	UpdateShaderMatrices();

	heightMap->Draw();

	glUseProgram(0);
}

void Renderer::DrawWater()
{
	SetCurrentShader(reflectShader);
	SetShaderLight(*light);
	glUniform3fv(glGetUniformLocation(currentShader->GetProgram(), "cameraPos"), 1, (float*)&camera->GetPosition());

	glUniform1i(glGetUniformLocation(currentShader->GetProgram(), "diffuseTex"), 0);

	glUniform1i(glGetUniformLocation(currentShader->GetProgram(), "cubeTex"), 3);

	glActiveTexture(GL_TEXTURE3);
	glBindTexture(GL_TEXTURE_CUBE_MAP, cubeMap);

	float heightX = (RAW_WIDTH * HEIGHTMAP_X / 2.0f);

	float heightY = 256 * HEIGHTMAP_Y / 3.0f;

	float heightZ = (RAW_HEIGHT * HEIGHTMAP_Z / 2.0f);

	modelMatrix = Matrix4::Translation(Vector3(heightX, heightY, heightZ)) *
		Matrix4::Scale(Vector3(heightX, 1, heightZ)) *
		Matrix4::Rotation(90, Vector3(1.0f, 0.0f, 0.0f));

	textureMatrix = Matrix4::Scale(Vector3(10.0f, 10.0f, 10.0f)) *
		Matrix4::Rotation(waterRotate, Vector3(0.0f, 0.0f, 1.0f));

	UpdateShaderMatrices();

	quad->Draw();

	glUseProgram(0);
}

void Renderer::DrawMirror()
{
	SetCurrentShader(mirrorShader);
	glUniform3fv(glGetUniformLocation(currentShader->GetProgram(), "cameraPos"), 1, (float*)&camera->GetPosition());

	glUniform1i(glGetUniformLocation(currentShader->GetProgram(), "cubeTex"), 3);

	glActiveTexture(GL_TEXTURE3);
	glBindTexture(GL_TEXTURE_CUBE_MAP, cubeMap2);

	float heightX = (RAW_WIDTH * HEIGHTMAP_X / 2.0f);

	float heightY = 256 * HEIGHTMAP_Y / 3.0f;

	float heightZ = (RAW_HEIGHT * HEIGHTMAP_Z / 2.0f);

	modelMatrix = Matrix4::Translation(Vector3(heightX, heightY + 1000, heightZ)) *
		Matrix4::Scale(Vector3(500, 800, 1));

	UpdateShaderMatrices();

	quad->Draw();

	glUseProgram(0);
}

void Renderer::ClearNodeLists()
{
	transparentNodeList.clear();
	nodeList.clear();
}
