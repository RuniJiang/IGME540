#include "Game.h"
#include "Vertex.h"
#include "Input.h"
#include "PathHelpers.h"
#include "Mesh.h"

// This code assumes files are in "ImGui" subfolder!
// Adjust as necessary for your own folder structure
#include "ImGui/imgui.h"
#include "ImGui/imgui_impl_dx11.h"
#include "ImGui/imgui_impl_win32.h"

// Needed for a helper function to load pre-compiled shader files
#pragma comment(lib, "d3dcompiler.lib")
#include <d3dcompiler.h>

// For the DirectX Math library
using namespace DirectX;

// --------------------------------------------------------
// Constructor
//
// DXCore (base class) constructor will set up underlying fields.
// Direct3D itself, and our window, are not ready at this point!
//
// hInstance - the application's OS-level handle (unique ID)
// --------------------------------------------------------
Game::Game(HINSTANCE hInstance)
	: DXCore(
		hInstance,			// The application's handle
		L"DirectX Game",	// Text for the window's title bar (as a wide-character string)
		1280,				// Width of the window's client area
		720,				// Height of the window's client area
		false,				// Sync the framerate to the monitor refresh? (lock framerate)
		true)				// Show extra stats (fps) in title bar?
{
#if defined(DEBUG) || defined(_DEBUG)
	// Do we want a console window?  Probably only in debug mode
	CreateConsoleWindow(500, 120, 32, 120);
	printf("Console window created successfully.  Feel free to printf() here.\n");
#endif

	speed = 0.2f;
	colorTint = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	ambientColor = XMFLOAT3(0.1f, 0.1f, 0.25f);
	showImGuiDemo = false;

}

// --------------------------------------------------------
// Destructor - Clean up anything our game has created:
//  - Delete all objects manually created within this class
//  - Release() all Direct3D objects created within this class
// --------------------------------------------------------
Game::~Game()
{
	// Call delete or delete[] on any objects or arrays you've
	// created using new or new[] within this class
	// - Note: this is unnecessary if using smart pointers

	// Call Release() on any Direct3D objects made within this class
	// - Note: this is unnecessary for D3D objects stored in ComPtrs

	// ImGui clean up
	ImGui_ImplDX11_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();
}

// --------------------------------------------------------
// Called once per program, after Direct3D and the window
// are initialized but before the game loop.
// --------------------------------------------------------
void Game::Init()
{
	// Helper methods for loading shaders, creating some basic
	// geometry to draw and some simple camera matrices.
	//  - You'll be expanding and/or replacing these later
	LoadShaders();
	CreateGeometry();
	CreateCameras();
	
	// Set initial graphics API state
	//  - These settings persist until we change them
	//  - Some of these, like the primitive topology & input layout, probably won't change
	//  - Others, like setting shaders, will need to be moved elsewhere later
	{
		// Tell the input assembler (IA) stage of the pipeline what kind of
		// geometric primitives (points, lines or triangles) we want to draw.  
		// Essentially: "What kind of shape should the GPU draw with our vertices?"
		context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	}

	// Initial the UI
	{
		// Initialize ImGui itself & platform/renderer backends
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGui_ImplWin32_Init(hWnd);
		ImGui_ImplDX11_Init(device.Get(), context.Get());
		// Pick a style (uncomment one of these 3)
		ImGui::StyleColorsDark();
		//ImGui::StyleColorsLight();
		//ImGui::StyleColorsClassic();
	}
}

// --------------------------------------------------------
// Loads shaders from compiled shader object (.cso) files
// and also created the Input Layout that describes our 
// vertex data to the rendering pipeline. 
// - Input Layout creation is done here because it must 
//    be verified against vertex shader byte code
// - We'll have that byte code already loaded below
// --------------------------------------------------------
void Game::LoadShaders()
{
	vertexShader = std::make_shared<SimpleVertexShader>(device, context,
		FixPath(L"VertexShader.cso").c_str());
	pixelShader = std::make_shared<SimplePixelShader>(device, context,
		FixPath(L"PixelShader.cso").c_str());
	customPS = std::make_shared<SimplePixelShader>(device, context,
		FixPath(L"CustomPS.cso").c_str());
	customPS2 = std::make_shared<SimplePixelShader>(device, context,
		FixPath(L"CustomPS2.cso").c_str());
}

// --------------------------------------------------------
// Creates the geometry we're going to draw
// --------------------------------------------------------
void Game::CreateGeometry()
{
	// Create some temporary variables to represent colors
	// - Not necessary, just makes things more readable
	XMFLOAT4 red	= XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f);
	XMFLOAT4 green	= XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f);
	XMFLOAT4 blue	= XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f);
	XMFLOAT4 gray   = XMFLOAT4(0.5f, 0.5f, 0.5f, 1.0f);
	XMFLOAT4 brown  = XMFLOAT4(0.7f, 0.5f, 0.5f, 1.0f);
	XMFLOAT4 white  = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);

	materials.push_back(std::make_shared<Material>(white, 0.5, vertexShader, pixelShader));
	materials.push_back(std::make_shared<Material>(green, 0.5, vertexShader, customPS));
	materials.push_back(std::make_shared<Material>(blue, 0.5, vertexShader, customPS2));


	meshes.push_back(std::make_shared<Mesh>(FixPath(L"../../Assets/Models/cube.obj").c_str(), device));
	meshes.push_back(std::make_shared<Mesh>(FixPath(L"../../Assets/Models/cylinder.obj").c_str(), device));
	meshes.push_back(std::make_shared<Mesh>(FixPath(L"../../Assets/Models/helix.obj").c_str(), device));
	meshes.push_back(std::make_shared<Mesh>(FixPath(L"../../Assets/Models/quad.obj").c_str(), device));
	meshes.push_back(std::make_shared<Mesh>(FixPath(L"../../Assets/Models/quad_double_sided.obj").c_str(), device));
	meshes.push_back(std::make_shared<Mesh>(FixPath(L"../../Assets/Models/sphere.obj").c_str(), device));
	meshes.push_back(std::make_shared<Mesh>(FixPath(L"../../Assets/Models/torus.obj").c_str(), device));

	objects.push_back(std::make_shared<Entity>(meshes[0], materials[0]));
	objects.push_back(std::make_shared<Entity>(meshes[1], materials[1]));
	objects.push_back(std::make_shared<Entity>(meshes[2], materials[1]));
	objects.push_back(std::make_shared<Entity>(meshes[3], materials[2]));
	objects.push_back(std::make_shared<Entity>(meshes[4], materials[2]));
	objects.push_back(std::make_shared<Entity>(meshes[5], materials[0]));
	objects.push_back(std::make_shared<Entity>(meshes[6], materials[2]));

	// Adjust transforms
	objects[0]->GetTransform()->MoveAbsolute(-9, 0, 0);
	objects[1]->GetTransform()->MoveAbsolute(-6, 0, 0);
	objects[2]->GetTransform()->MoveAbsolute(-3, 0, 0);
	objects[3]->GetTransform()->MoveAbsolute(0, 0, 0);
	objects[4]->GetTransform()->MoveAbsolute(3, 0, 0);
	objects[5]->GetTransform()->MoveAbsolute(6, 0, 0);
	objects[6]->GetTransform()->MoveAbsolute(9, 0, 0);

}

// --------------------------------------------------------
// Create the cameras
// --------------------------------------------------------
void Game::CreateCameras()
{
	cameras.push_back(std::make_shared<Camera>(
		0.0f, 2.0f, -5.0f,
		5.0f,
		1.0f,
		XM_PIDIV4,
		(float) this->windowWidth / this->windowHeight,
		0.1f,
		1000.0f,
		true
		));

	cameras.push_back(std::make_shared<Camera>(
		1.0f, 2.0f, -10.0f,
		2.0f,
		0.5f,
		XM_PI / 8,
		(float) this->windowWidth / this->windowHeight,
		3.0f,
		1000.0f,
		true
		));

	activedCamera = cameras[0];
}



// --------------------------------------------------------
// Handle resizing to match the new window size.
//  - DXCore needs to resize the back buffer
//  - Eventually, we'll want to update our 3D camera
// --------------------------------------------------------
void Game::OnResize()
{
	for(auto& camera : cameras)
	camera->UpdateProjectionMatrix((float)this->windowWidth / this->windowHeight);

	// Handle base-level DX resize stuff
	DXCore::OnResize();
}

// --------------------------------------------------------
// Update your game here - user input, move objects, AI, etc.
// ------------------------------------------------------
void Game::Update(float deltaTime, float totalTime)
{
	activedCamera->Update(deltaTime);

	// Move one entity from -0.7 to 0.7 along x-axis
	if (objects[1]->GetTransform()->GetPosition().x >= 0.7 ||
		objects[1]->GetTransform()->GetPosition().x <= -0.7)
	{
		speed = -speed;
	}
	objects[1]->GetTransform()->MoveAbsolute(speed * deltaTime, 0, 0);

	// Change the scale based on sin
	float temp = abs(sin(totalTime));
	objects[4]->GetTransform()->SetScale(temp, temp,temp);


	UIUpdate(deltaTime);
	
	// Example input checking: Quit if the escape key is pressed
	if (Input::GetInstance().KeyDown(VK_ESCAPE))
		Quit();
}

// --------------------------------------------------------
// Clear the screen, redraw everything, present to the user
// --------------------------------------------------------
void Game::Draw(float deltaTime, float totalTime)
{
	// Frame START
	// - These things should happen ONCE PER FRAME
	// - At the beginning of Game::Draw() before drawing *anything*
	{
		// Clear the back buffer (erases what's on the screen)
		const float bgColor[4] = { 0.4f, 0.6f, 0.75f, 1.0f }; // Cornflower Blue
		context->ClearRenderTargetView(backBufferRTV.Get(), bgColor);

		// Clear the depth buffer (resets per-pixel occlusion information)
		context->ClearDepthStencilView(depthBufferDSV.Get(), D3D11_CLEAR_DEPTH, 1.0f, 0);
	}

	

	// FOR(auto& m : meshes)
	// {
	// m->SetBuffersAndDraw(context)
	// }
	//
	for (std::shared_ptr<Entity>& object : objects)
	{
		//mesh->Draw(context);
		object->GetMaterial()->GetPixelShader()->SetFloat("time", totalTime);
		object->GetMaterial()->GetPixelShader()->SetFloat3("ambient", ambientColor);
		object->Draw(context, activedCamera);
	}

	{
		ImGui::Render();
		ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData()); 
	}

	// Frame END
	// - These should happen exactly ONCE PER FRAME
	// - At the very end of the frame (after drawing *everything*)
	{
		// Present the back buffer to the user
		//  - Puts the results of what we've drawn onto the window
		//  - Without this, the user never sees anything
		bool vsyncNecessary = vsync || !deviceSupportsTearing || isFullscreen;
		swapChain->Present(
			vsyncNecessary ? 1 : 0,
			vsyncNecessary ? 0 : DXGI_PRESENT_ALLOW_TEARING);

		// Must re-bind buffers after presenting, as they become unbound
		context->OMSetRenderTargets(1, backBufferRTV.GetAddressOf(), depthBufferDSV.Get());
	}
}

void Game::UIUpdate(float deltaTime)
{
	// Feed fresh input data to ImGui
	ImGuiIO& io = ImGui::GetIO();
	io.DeltaTime = deltaTime;
	io.DisplaySize.x = (float)this->windowWidth;
	io.DisplaySize.y = (float)this->windowHeight;
	// Reset the frame
	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();
	// Determine new input capture
	Input& input = Input::GetInstance();
	input.SetKeyboardCapture(io.WantCaptureKeyboard);
	input.SetMouseCapture(io.WantCaptureMouse);

	// Show the demo window
	if (showImGuiDemo)
	{
		ImGui::ShowDemoWindow();
	}


	ImGui::Begin("Inspector"); // Everything after is part of the window
	ImGui::TableNextColumn(); ImGui::Checkbox("Show ImGui Demo Window", &showImGuiDemo);

	// === Stats of the program === //
	if (ImGui::TreeNode("Stats"))
	{
		ImGui::BulletText("Framerate: %f fps", ImGui::GetIO().Framerate);
		ImGui::BulletText("Window Dimension: %dx%d", windowWidth, windowHeight);
		ImGui::TreePop();
	}
	// === Controls === //
	if (ImGui::TreeNode("Controls"))
	{
		ImGui::Spacing();
		ImGui::Text("Camera Movement:");           ImGui::SameLine(200); ImGui::Text("WASD, X, Space");
		ImGui::Text("Camera Rotation:");           ImGui::SameLine(200); ImGui::Text("Left Click & Drag");
		ImGui::Text("Camera move speed up:");      ImGui::SameLine(200); ImGui::Text("Left Shift");
		ImGui::Text("Camera move slow down:");     ImGui::SameLine(200); ImGui::Text("Left Ctrl");
		ImGui::Spacing();

		ImGui::TreePop();
	}

	if (ImGui::TreeNode("Cameras"))
	{
		// Select Camera
		static int selected = 0;
		for (int n = 0; n < cameras.size(); n++)
		{
			char buf[32];
			sprintf_s(buf, "Camera %d", n);
			if (ImGui::Selectable(buf, selected == n))
			{
				selected = n;
				activedCamera = cameras[n];
			}

		}

		// Show UI for current camera
		CameraUI(activedCamera);

		// Finalize the tree node
		ImGui::TreePop();
	}
	if (ImGui::TreeNode("Scene Entities"))
	{
		for (int i = 0; i < objects.size(); i++)
		{
			ImGui::PushID(i);
			if (ImGui::TreeNode("Entity Node", "Entity %d", i))
			{
				ImGui::Spacing();

				// Transform details
				Transform* trans = objects[i]->GetTransform();
				XMFLOAT3 position = trans->GetPosition();
				XMFLOAT3 rotation = trans->GetRotation();
				XMFLOAT3 scale = trans->GetScale();
				XMFLOAT4 colorTint = objects[i]->GetMaterial()->GetColorTint();

				if (ImGui::DragFloat3("Position", &position.x, 0.01f)) trans->SetPosition(position);
				if (ImGui::DragFloat3("Rotation (Radians)", &rotation.x, 0.01f)) trans->SetRotation(rotation);
				if (ImGui::DragFloat3("Scale", &scale.x, 0.01f)) trans->SetScale(scale);
				if (ImGui::ColorEdit4("ColorTint", &colorTint.x)) objects[i]->GetMaterial()->SetColorTint(colorTint);
				ImGui::Spacing();

				ImGui::TreePop();
			}
			ImGui::PopID();
		}

		// Finalize the tree node
		ImGui::TreePop();
	}



	ImGui::End(); // Ends the current window


}

void Game::CameraUI(std::shared_ptr<Camera> cam)
{
	ImGui::Spacing();

	// Transform details
	XMFLOAT3 pos = cam->GetTransform()->GetPosition();
	XMFLOAT3 rot = cam->GetTransform()->GetRotation();

	if (ImGui::DragFloat3("Position", &pos.x, 0.01f))
		cam->GetTransform()->SetPosition(pos);
	if (ImGui::DragFloat3("Rotation (Radians)", &rot.x, 0.01f))
		cam->GetTransform()->SetRotation(rot);
	ImGui::Spacing();

	// Clip planes
	float nearClip = cam->GetNearClipDis();
	float farClip = cam->GetFarClipDis();
	float fov = cam->GetFov();
	if (ImGui::DragFloat("Near Clip Distance", &nearClip, 0.01f, 0.001f, 1.0f))
		cam->SetNearClipDis(nearClip);
	if (ImGui::DragFloat("Far Clip Distance", &farClip, 1.0f, 10.0f, 1500.0f))
		cam->SetFarClipDis(farClip);
	if (ImGui::DragFloat("Field of View", &fov, 0.01f, 0.15f, 1.0f))
		cam->SetFov(fov);

	// Perspective/Orthographic radio button
	bool isPerspective = activedCamera->GetisPerspective();
	if (ImGui::RadioButton("Perspective", isPerspective)) 
	{
		activedCamera->SetisPerspective(true);
	} ImGui::SameLine();
	if (ImGui::RadioButton("Orthograhphic", !isPerspective)) 
	{
		activedCamera->SetisPerspective(false);
	} 

	// Display specific menu for Perspective/orthographic view
	if (isPerspective)
	{
		float fov = cam->GetFov() * 180.0f / XM_PI;
		if (ImGui::SliderFloat("Field of View (Degrees)", &fov, 0.01f, 180.0f))
		cam->SetFov(fov * XM_PI / 180.0f); // Back to radians
	}
	else
	{
		float wid = cam->GetOrthographicWidth();
		if (ImGui::SliderFloat("Orthographic Width", &wid, 1.0f, 10.0f))
		cam->SetOrthographicWidth(wid);
	}

	ImGui::Spacing();
}