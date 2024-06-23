#include "RigelRenderer.hpp"

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

	auto scene = new rgr::Scene();
	rgr::SetScene(scene);

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

	rgr::Mesh quadMesh = rgr::Mesh(quadVertices, quadIndices, quadTexCoords);
	rgr::Mesh cubeMesh = rgr::Mesh("resources/objects/cube.obj");
	rgr::Mesh sphereMesh = rgr::Mesh("resources/objects/sphere.obj");

	auto containerDiffuse = new rgr::Texture("resources/textures/container_diffuse.png");
	auto containerSpecular = new rgr::Texture("resources/textures/container_specular.png");
	auto planeDiffuse = new rgr::Texture("resources/textures/plane_texture.png");
    auto testTexture = new rgr::Texture("resources/textures/test_texture.png");

	auto cube = new rgr::RenderableMesh(&cubeMesh);
	cube->diffuseTexture = containerDiffuse;
	cube->specularTexture = containerSpecular;
	cube->GetTransform().SetPosition(glm::vec3(0, 3, -2));

	auto cube1 = new rgr::RenderableMesh(&cubeMesh);
	cube1->diffuseTexture = testTexture;
	cube1->GetTransform().SetPosition(glm::vec3(2, 3, -2));

	auto plane = new rgr::RenderableMesh(&cubeMesh);
	plane->diffuseTexture = planeDiffuse;
	plane->GetTransform().SetPosition(glm::vec3(0, -1.5f, 0));
	plane->GetTransform().SetScale(glm::vec3(10, 0.01, 10));
	
	auto camera = new rgr::Camera(glm::radians(60.0f), WIDTH, HEIGHT, 0.1f, 100.0f);
	camera->GetTransform().SetPosition(glm::vec3(0.0f, 3, 0.0f));
	camera->GetTransform().SetRotation(glm::quat(glm::vec3(0, 0, 0)));
	camera->FlagAsMain();

	rgr::DirectionalLight dirLight = rgr::DirectionalLight(glm::vec3(1.0, 1.0, 1.0), 0.6f, glm::vec3(1, -1, 1));
	dirLight.GetTransform().SetPosition(glm::vec3(-3, 3, -3));

	rgr::DirectionalLight dirLight1 = rgr::DirectionalLight(glm::vec3(1.0, 1.0, 1.0), 0.6f, glm::vec3(1, -1, 0));
	dirLight1.GetTransform().SetPosition(glm::vec3(-3, 3, -3));

	rgr::PointLight pntLight = rgr::PointLight(
		glm::vec3(0.98, 0.76, 0.12),
		3.0f,
		1.0f,
		0.7f,
		1.8f
	);
	pntLight.GetTransform().SetPosition(glm::vec3(0, 1.2f, 0));

	rgr::PointLight pntLight1 = rgr::PointLight(
		glm::vec3(1, 1, 1),
		2.0f,
		1.0f,
		0.7f,
		1.8f
	);
	pntLight1.GetTransform().SetPosition(glm::vec3(-2.5, -0.9f, 0));

	rgr::SpotLight sptLight = rgr::SpotLight(
		glm::vec3(1.0f, 0.0f, 0.0f),
		4.0f, glm::vec3(0.0f, -1.0f, 0.0f),
		0.9978f, 0.953f,
		1.0f, 0.22f, 0.2f
	);
	sptLight.GetTransform().SetPosition(glm::vec3(2, 0, 0));

	rgr::SpotLight sptLight1 = rgr::SpotLight(
		glm::vec3(0.0f, 0.0f, 1.0f),
		4.0f, glm::vec3(1.0f, -0.5f, 0.0f),
		0.9978f, 0.953f,
		1.0f, 0.22f, 0.2f
	);
	sptLight1.GetTransform().SetPosition(glm::vec3(-2, 0, 0));

	scene->AddObject(camera);
	scene->AddObject(cube);
	scene->AddObject(cube1);
	scene->AddObject(plane);
	//scene->AddObject(quad);
	//scene->AddObject(sphere);
	//scene->AddObject(container);

	scene->AddObject(&dirLight);
	scene->AddObject(&dirLight1);
	scene->AddObject(&pntLight);
	scene->AddObject(&pntLight1);
	scene->AddObject(&sptLight);
	scene->AddObject(&sptLight1);

	const float sensitivity = 0.3f;
	const float flySpeed = 2.0f;
	const float flySpeedBoost = 4.5f;

	float yaw = 0;
	float pitch = 0;
	glm::vec3 pos = camera->GetTransform().GetPosition();
	glm::vec3 rot(0.0f);

	while (!rgr::WindowShouldClose())
	{	
		glm::vec2 mouseDelta = rgr::Input::GetMouseDelta();

		yaw -= mouseDelta.x * sensitivity * rgr::GetDeltaTime();
		pitch -= mouseDelta.y * sensitivity * rgr::GetDeltaTime();

		if (pitch > 1.5708f) pitch = 1.5708f;
		else if (pitch < -1.5708f) pitch = -1.5708f;

		glm::vec3 fv = camera->GetTransform().GetForwardVector();
		glm::vec3 rv = camera->GetTransform().GetRightVector();

		const float speed = rgr::Input::KeyHold(RGR_KEY_LEFT_SHIFT) ? flySpeedBoost : flySpeed;

		if (rgr::Input::KeyHold(RGR_KEY_W))
			pos += fv * speed * rgr::GetDeltaTime();
		if (rgr::Input::KeyHold(RGR_KEY_S))
			pos -= fv * speed * rgr::GetDeltaTime();
		if (rgr::Input::KeyHold(RGR_KEY_D))
			pos += rv * speed * rgr::GetDeltaTime();
		if (rgr::Input::KeyHold(RGR_KEY_A))
			pos -= rv * speed * rgr::GetDeltaTime();

		if (rgr::Input::KeyHold(RGR_KEY_Q))
			pos += glm::vec3(0.0f, 1.0f, 0.0f) * speed * rgr::GetDeltaTime();
		if (rgr::Input::KeyHold(RGR_KEY_E))
			pos -= glm::vec3(0.0f, 1.0f, 0.0f) * speed * rgr::GetDeltaTime();

		camera->GetTransform().SetPosition(pos);
		camera->GetTransform().SetRotation(glm::quat(glm::vec3(pitch, yaw, 0.0f)));

		rot += glm::vec3(1.0f, -1.0f, 0.5f) * rgr::GetDeltaTime();
		cube->GetTransform().SetRotation(rot);
		//sphere->GetTransform().SetRotation(rot);

		if (rgr::Input::KeyPressed(RGR_KEY_M))
		{
			if (camera->viewMode == rgr::Camera::ViewMode::WIREFRAME)
				camera->viewMode = rgr::Camera::ViewMode::FILL;
			else
				camera->viewMode = rgr::Camera::ViewMode::WIREFRAME;
		}

		auto x = (float)glm::cos(rgr::GetTimePassed() * 3);
		auto z = (float)glm::sin(rgr::GetTimePassed() * 3);
		sptLight.direction = glm::vec3(x, 0.0, z);

		rgr::Update();
	}

	rgr::Quit();
}
