#include "Renderer.h"

Renderer::Renderer(Window &parent) : OGLRenderer(parent)
{
	camera = new Camera(15.0f, -60.0f, 0.0f, Vector3(1600, 200, 3500));

	root = new SceneNode();

	for (int i = 0; i < NUM_SCENES; ++i)
		scene[i] = new Scene(root, this);

	if (!GenerateMountainScene(*scene[0]))
		return;

	currentScene = 0;

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
	delete camera;
	currentShader = 0;
	//delete[] scene;
}

bool Renderer::GenerateMountainScene(Scene &scene)
{
	root->AddChild(&scene);
	Mesh* quad = Mesh::GenerateQuad();

	//cubeMap
	scene.SetAltCubeMap(SOIL_load_OGL_cubemap("../../Textures/ame_iceflats/iceflats_bk.tga", "../../Textures/ame_iceflats/iceflats_ft.tga",
		"../../Textures/ame_iceflats/iceflats_up.tga", "../../Textures/ame_iceflats/iceflats_dn.tga",
		"../../Textures/ame_iceflats/iceflats_lf.tga", "../../Textures/ame_iceflats/iceflats_rt.tga",
		SOIL_LOAD_RGB, SOIL_CREATE_NEW_ID, SOIL_FLAG_POWER_OF_TWO));
	//snow cube map from www.custommapmakers.org/skyboxes

	scene.SetCubeMap(SOIL_load_OGL_cubemap("../../Textures/rusted_west.jpg", "../../Textures/rusted_east.jpg",
		"../../Textures/rusted_up.jpg", "../../Textures/rusted_down.jpg",
		"../../Textures/rusted_south.jpg", "../../Textures/rusted_north.jpg",
		SOIL_LOAD_RGB, SOIL_CREATE_NEW_ID, SOIL_FLAG_POWER_OF_TWO));

	if (!scene.GetAltCubeMap() || !scene.GetCubeMap())
		return false;

	scene.SetShader(new Shader("../../Shaders/skyboxVertex.glsl", "../../Shaders/skyboxFragment.glsl"));
	if (!scene.GetShader()->LinkProgram())
		return false;
	scene.SetMesh(quad);
	//cubemap

	//heightMap
	HeightMap* heightMap = new HeightMap("../../Textures/grandcanyon.data");

	heightMap->SetTexture(SOIL_load_OGL_texture("../../Textures/mud.jpg ", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS));
	//heightMap->SetSnowTex(SOIL_load_OGL_texture("../../Textures/snow.jpg ", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS));
	heightMap->SetBumpMap(SOIL_load_OGL_texture("../../Textures/mud_Normal.jpg ", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS));
	heightMap->SetSpecMap(SOIL_load_OGL_texture("../../Textures/mud_Specular.jpg ", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS));
	heightMap->SetGlossMap(SOIL_load_OGL_texture("../../Textures/mud_Gloss.jpg ", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS));
	if (!heightMap->GetTexture())
		return false;
	if (!heightMap->GetBumpMap())
		return false;
	if (!heightMap->GetSpecMap())
		return false;
	if (!heightMap->GetGlossMap())
		return false;
	SetTextureRepeating(heightMap->GetTexture(), true);
	SetTextureRepeating(heightMap->GetBumpMap(), true);

	SceneNode* sHeightMap = new SceneNode(heightMap, Vector4(1, 1, 1, 1));
	sHeightMap->SetHeightMap(heightMap);
	sHeightMap->SetBoundingRadius(15000.0f);
	sHeightMap->SetShader(new Shader("../../Shaders/BumpVertex.glsl", "../../Shaders/BumpFragment.glsl"));
	scene.AddChild(sHeightMap);
	if (!sHeightMap->GetShader()->LinkProgram())
		return false;
	//heightMap

	
	//water
	quad->SetTexture(SOIL_load_OGL_texture("../../Textures/water2.jpg", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS));
	if (!quad->GetTexture())
		return false;

	SetTextureRepeating(quad->GetTexture(), true);

	float heightX = (RAW_WIDTH * HEIGHTMAP_X / 2.0f);
	float heightY = 256 * HEIGHTMAP_Y / 10.0f;
	float heightZ = (RAW_HEIGHT * HEIGHTMAP_Z / 2.0f);

	Matrix4 tempModelMatrix = Matrix4::Translation(Vector3(heightX, heightY, heightZ)) *
				Matrix4::Scale(Vector3(heightX, 1, heightZ)) *
				Matrix4::Rotation(90, Vector3(1.0f, 0.0f, 0.0f));

	SceneNode* sWater = new SceneNode(quad, Vector4(1, 1, 1, 1));
	sWater->SetTransform(tempModelMatrix);
	sWater->SetUseOffset();
	sWater->SetBoundingRadius(15000.0f);
	sWater->SetOffset(0.0f);
	sWater->MakeTransparent();
	sWater->SetShader(new Shader("../../Shaders/BumpVertex.glsl", "../../Shaders/reflectFragment.glsl"));
	sHeightMap->AddChild(sWater);
	if (!sWater->GetShader()->LinkProgram())
		return false;
	//water
	
	//light
	scene.AddLight(new Light(Vector3((RAW_HEIGHT * HEIGHTMAP_X / 2.0f), 500.0f, -(RAW_HEIGHT * HEIGHTMAP_Z / 2.0f)),
		Vector4(1, 1, 1, 1), 2000.0f,
		Vector4(1, 1, 1, 1), 1,
		Vector3(-1, 2, -1), 40.0f));
	//light
	
	//mirror
	heightX = (RAW_WIDTH * HEIGHTMAP_X / 2.0f);
	
	heightY = 256 * HEIGHTMAP_Y / 3.0f;
	
	heightZ = (RAW_HEIGHT * HEIGHTMAP_Z / 2.0f);
	
	tempModelMatrix = Matrix4::Translation(Vector3(heightX, heightY + 1000, heightZ)) *
			Matrix4::Scale(Vector3(500, 800, 1));
	SceneNode* sMirror = new SceneNode(quad, Vector4(1, 1, 1, 1));	//mirror is 100% reflective, does not need its own mesh/ texture
	sMirror->SetShader(new Shader("../../Shaders/BumpVertex.glsl", "../../Shaders/MirrorFragment.glsl"));
	sMirror->SetTransform(tempModelMatrix);
	sMirror->SetBoundingRadius(RAW_WIDTH * HEIGHTMAP_X / 1.8f);
	sMirror->SetUseAltHeightMap();
	sHeightMap->AddChild(sMirror);
	if (!sMirror->GetShader()->LinkProgram())
		return false;
	//mirror
}

void Renderer::UpdateScene(float msec)
{
	//if (!splitScreen)
	scene[currentScene]->Update(msec);
	//else
	// UpdateSplitScreens()
}

void Renderer::RenderScene()
{
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

	//DrawNodes();
	for (int i = 0; i < NUM_SCENES; ++i)
		scene[i]->RenderScene();

	//DrawSkybox();
	//DrawHeightmap();
	//DrawWater();
	//DrawMirror();

	SwapBuffers();
	//ClearNodeLists();
}
