#pragma comment(lib, "nclgl.lib")

#include "../../nclgl/window.h"
#include "Renderer.h"

int main()
{
	Window w("Cube Mapping! sky textures courtesy of http://www.hazelwhorley.com", 1280, 720, false);
	if (!w.HasInitialised())
		return -1;

	Renderer renderer(w);
	if (!renderer.HasInitialised())
		return -1;

	w.LockMouseToWindow(true);
	w.ShowOSPointer(false);

	float offset = 0;
	while (w.UpdateWindow() && !Window::GetKeyboard()->KeyDown(KEYBOARD_ESCAPE))
	{
		if (Window::GetKeyboard()->KeyDown(KEYBOARD_UP))
			offset += 0.01f;
		if (Window::GetKeyboard()->KeyDown(KEYBOARD_DOWN))
			offset -= 0.01f;
		if (offset > 1.2f)
			offset = 1.2f;
		if (offset < -1.0f)
			offset = -1.0f;

		renderer.SetHeightMapSnow(offset);
		renderer.UpdateScene(w.GetTimer()->GetTimedMS());
		renderer.RenderScene();
	}

	return 0;
}
