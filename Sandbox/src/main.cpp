#include "core/RigelRenderer.hpp"
#include "core/input/Input.hpp"

#include "materials/Material2D.hpp"
#include "materials/Material3D.hpp"

#include "gtx/quaternion.hpp"
#include "gtx/string_cast.hpp"
#include "glm.hpp"

#include <iostream>
#include <vector>

int main()
{	
	const int WIDTH = 1600;
	const int HEIGHT = 900;
	const char* TITLE = "It works!";

	if (rgr::Init(WIDTH, HEIGHT, TITLE) != RIGEL_OK)
		return -1;

	rgr::Scene scene = rgr::Scene();
	rgr::SetScene(&scene);

	const std::vector<float> quadVertices {
		-50.0f, 50.0f, 0.0f,
		 50.0f, 50.0f, 0.0f,
		 50.0f, -50.0f, 0.0f,
		-50.0f, -50.0f, 0.0f
	};
	const std::vector<unsigned int> quadIndices {
		0, 1, 3, 
		1, 2, 3
	};
	const std::vector<float> quadTexCoords{
		0.0f, 1.0f,
		1.0f, 1.0f,
		1.0f, 0.0f,
		0.0f, 0.0f
	};

	const std::vector<float> planeVertices{
		-0.5f, 0.0f, -0.5f,
		0.5f, 0.0f, -0.5f,
		0.5f, 0.0f, 0.5f,
		-0.5f, 0.0f, 0.5f
	};
	const std::vector<unsigned int> planeIndices{
		0, 1, 3,
		1, 2, 3
	};
	const std::vector<float> planeTexCoords{
		0.0f, 1.0f,
		1.0f, 1.0f,
		1.0f, 0.0f,
		0.0f, 0.0f
	};

	rgr::Mesh* planeMesh = new rgr::Mesh(planeVertices, planeIndices, planeTexCoords);
	rgr::Mesh* quadMesh = new rgr::Mesh(quadVertices, quadIndices, quadTexCoords);
	rgr::Mesh* cubeMesh = new rgr::Mesh("resources/objects/cube.obj");
	rgr::Mesh* sphereMesh = new rgr::Mesh("resources/objects/sphere.obj");
	
	rgr::Shader* shader2D = rgr::Shader::FromFiles("resources/shaders/vertex_2d.glsl", "resources/shaders/fragment_2d.glsl");
	rgr::Shader* shader3D = rgr::Shader::FromFiles("resources/shaders/vertex_3d_lit.glsl", "resources/shaders/fragment_3d_lit.glsl");

	Material2D* quadMaterial = new Material2D(new rgr::Texture("resources/textures/Misterius3Dk.png"), shader2D);
	Material3D* cubeMaterial = new Material3D(new rgr::Texture("resources/textures/wall.jpg"), shader3D);
	Material3D* planeMaterial = new Material3D(new rgr::Texture("resources/textures/plane_texture.png"), shader3D);
	Material3D* sphereMaterial = new Material3D(new rgr::Texture("resources/textures/world_map.jpg"), shader3D);

	rgr::Renderable quad = rgr::Renderable(quadMesh, quadMaterial);
	quad.GetTransform().space = rgr::Transform::Space::SCREEN_2D;
	quad.GetTransform().SetPosition(glm::vec3(-610, 330, 0.0f));
	quad.GetTransform().SetScale(glm::vec3(0.5, 0.5, 1));

	rgr::Renderable cube = rgr::Renderable(cubeMesh, cubeMaterial);
	cube.GetTransform().SetPosition(glm::vec3(0, 0, 2));

	rgr::Renderable plane = rgr::Renderable(planeMesh, planeMaterial);
	plane.GetTransform().SetPosition(glm::vec3(0, -1.5f, 0));
	plane.GetTransform().SetScale(glm::vec3(10, 1, 10));
	plane.GetTransform().SetRotation(glm::quat(glm::vec3(0, 0, 0)));

	rgr::Renderable sphere = rgr::Renderable(sphereMesh, sphereMaterial);
	sphere.GetTransform().SetScale(glm::vec3(0.5f, 0.5f, 0.5f));
	sphere.GetTransform().SetPosition(glm::vec3(0.0, 0.0f, 0.0f));

	rgr::Camera camera = rgr::Camera(glm::radians(60.0f), WIDTH, HEIGHT, 0.1f, 100.0f);
	camera.GetTransform().SetPosition(glm::vec3(0.0f, 0, -2.0f));
	camera.GetTransform().SetRotation(glm::quat(glm::vec3(0.0f, 0.0f, 0.0f)));
	camera.FlagAsMain();

	rgr::DirectionalLight dirLight = rgr::DirectionalLight(glm::vec3(1.0, 1.0, 1.0), 0.3, glm::vec3(0.0, -1.0, 0.0));
	rgr::PointLight pntLight = rgr::PointLight(
		glm::vec3(1.0, 1.0, 1.0),
		0.3f,
		1.0f,
		0.2f,
		0.1f
	);

	scene.AddObject(&camera);
	scene.AddObject(&quad);
	scene.AddObject(&cube);
	scene.AddObject(&plane);
	scene.AddObject(&sphere);

	scene.AddObject(&dirLight);
	scene.AddObject(&pntLight);

	const float sensitivity = 0.3f;
	const float walkSpeed = 1.5f;

	float yaw = 0;
	float pitch = 0;
	glm::vec3 pos = camera.GetTransform().GetPosition();
	glm::vec3 rot(0.0f);

	while (!rgr::WindowShouldClose())
	{	
		glm::vec2 mouseDelta = rgr::Input::GetMouseDelta();

		yaw -= mouseDelta.x * sensitivity * rgr::GetDeltaTime();
		pitch -= mouseDelta.y * sensitivity * rgr::GetDeltaTime();

		if (pitch > 1.5708f) pitch = 1.5708f;
		else if (pitch < -1.5708f) pitch = -1.5708f;

		glm::vec3 fv = camera.GetTransform().GetForwardVector();
		glm::vec3 rv = camera.GetTransform().GetRightVector();

		if (rgr::Input::KeyHold(RGR_KEY_W))
			pos += fv * walkSpeed * rgr::GetDeltaTime();
		if (rgr::Input::KeyHold(RGR_KEY_S))
			pos -= fv * walkSpeed * rgr::GetDeltaTime();
		if (rgr::Input::KeyHold(RGR_KEY_D))
			pos += rv * walkSpeed * rgr::GetDeltaTime();
		if (rgr::Input::KeyHold(RGR_KEY_A))
			pos -= rv * walkSpeed * rgr::GetDeltaTime();

		if (rgr::Input::KeyHold(RGR_KEY_Q))
			pos += glm::vec3(0.0f, 1.0f, 0.0f) * walkSpeed * rgr::GetDeltaTime();
		if (rgr::Input::KeyHold(RGR_KEY_E))
			pos -= glm::vec3(0.0f, 1.0f, 0.0f) * walkSpeed * rgr::GetDeltaTime();

		camera.GetTransform().SetPosition(pos);
		camera.GetTransform().SetRotation(glm::quat(glm::vec3(pitch, yaw, 0.0f)));

		rot += glm::vec3(1.0f, -1.0f, 0.5f) * rgr::GetDeltaTime();
		cube.GetTransform().SetRotation(rot);
		sphere.GetTransform().SetRotation(rot);

		if (rgr::Input::KeyPressed(RGR_KEY_M))
		{
			if (camera.viewMode == rgr::Camera::ViewMode::WIREFRAME)
				camera.viewMode = rgr::Camera::ViewMode::FILL;
			else
				camera.viewMode = rgr::Camera::ViewMode::WIREFRAME;
		}

		rgr::Update();
	}

	rgr::Quit();
	std::cin.get();
}
