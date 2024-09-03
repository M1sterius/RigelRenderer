#include "RigelRenderer.hpp"
#include "glm.hpp"

#include <iostream>
#include <vector>
#include <memory>

int main(int argc, char* argv[])
{
    const size_t WIDTH = 1600;
    const size_t HEIGHT = 900;
    const char* TITLE = "It works!";

    if (!rgr::Core::Init(WIDTH, HEIGHT, TITLE, true))
        return -1;
//    rgr::Core::DisableVSync();
//    rgr::Core::SetTargetFPS(1000);

    rgr::Cursor::SetCursorState(rgr::Cursor::CURSOR_STATE::DISABLED);

    auto scene = new rgr::Scene();
    scene->name = "Test Scene";
    rgr::Core::LoadScene(scene);

    auto camera = std::make_shared<rgr::Camera>(glm::radians(75.0f), WIDTH, HEIGHT, 0.1f, 100.0f);
    camera->GetTransform().SetPosition(glm::vec3(-3.0f, 0, 0.0f));
    //camera->FlagAsMain();

    auto backpackModel = std::make_shared<rgr::Model>("resources/models/backpack/backpack.obj");
    auto cubeModel = std::make_shared<rgr::Model>("resources/models/cube/cube.obj");
    auto sponzaModel = std::make_shared<rgr::Model>("resources/models/sponza/sponza.obj");

    auto sponza = std::make_shared<rgr::RenderableModel>(sponzaModel);
    sponza->GetTransform().SetScale(glm::vec3(0.005, 0.005, 0.005));

    auto backpack = std::make_shared<rgr::RenderableModel>(backpackModel);
    backpack->GetTransform().SetPosition(glm::vec3(2, 0, 0));
    backpack->GetTransform().SetScale(glm::vec3(0.5, 0.5, 0.5));

    auto backpack1 = std::make_shared<rgr::RenderableModel>(backpackModel);
    backpack1->GetTransform().SetPosition(glm::vec3(0, 0, 0));
    backpack1->GetTransform().SetScale(glm::vec3(0.5, 0.5, 0.5));

    auto backpack2 = std::make_shared<rgr::RenderableModel>(backpackModel);
    backpack2->GetTransform().SetPosition(glm::vec3(-2, 0, 0));
    backpack2->GetTransform().SetScale(glm::vec3(0.5, 0.5, 0.5));

    auto backpack3 = std::make_shared<rgr::RenderableModel>(backpackModel);
    backpack3->GetTransform().SetPosition(glm::vec3(2, 0, -2));
    backpack3->GetTransform().SetScale(glm::vec3(0.5, 0.5, 0.5));

    auto backpack4 = std::make_shared<rgr::RenderableModel>(backpackModel);
    backpack4->GetTransform().SetPosition(glm::vec3(0, 0, -2));
    backpack4->GetTransform().SetScale(glm::vec3(0.5, 0.5, 0.5));

    auto backpack5 = std::make_shared<rgr::RenderableModel>(backpackModel);
    backpack5->GetTransform().SetPosition(glm::vec3(-2, 0, -2));
    backpack5->GetTransform().SetScale(glm::vec3(0.5, 0.5, 0.5));

    auto cube = std::make_shared<rgr::RenderableModel>(cubeModel);
    cube->GetTransform().SetPosition(glm::vec3(0, -0.6, 2));

    auto dirLight = std::make_shared<rgr::DirectionalLight>(glm::vec3(1.0, 1.0, 1.0), 0.5f, glm::vec3(0.46, -0.88, 0.05));
    dirLight->GetTransform().SetPosition(glm::vec3(-5.84, 8.4, -0.18));
    dirLight->smoothShadows = true;

    auto dirLight1 = std::make_shared<rgr::DirectionalLight>(glm::vec3(1.0, 1.0, 1.0), 0.1f, glm::vec3(-0.1, -1, -1));
    dirLight1->GetTransform().SetPosition(glm::vec3(-5.84, 8.4, -0.18));
    dirLight1->smoothShadows = true;

    auto pntLight = std::make_shared<rgr::PointLight>(
            glm::vec3(1, 0, 0),
            3.0f,
            1.0f,
            0.7f,
            1.8f
    );
    pntLight->GetTransform().SetPosition(glm::vec3(0, 0, 2));

    auto sptLight = std::make_shared<rgr::SpotLight>(
            glm::vec3(1.0f, 1.0f, 1.0f),
            3.0f, glm::vec3(0, -1, 1),
            0.9978f, 0.953f,
            1.0f, 0.22f, 0.2f
    );
    sptLight->GetTransform().SetPosition(glm::vec3(0, 1, 2));
    sptLight->castShadows = true;
    sptLight->smoothShadows = true;

    scene->AddObject(camera);
    scene->AddObject(backpack);
    scene->AddObject(sponza);
//    scene->AddObject(backpack1);
//    scene->AddObject(backpack2);
//    scene->AddObject(backpack3);
//    scene->AddObject(backpack4);
//    scene->AddObject(backpack5);
    scene->AddObject(cube);

    scene->AddObject(dirLight);
//    scene->AddObject(dirLight1);
    scene->AddObject(pntLight);
    scene->AddObject(sptLight);

    const float sensitivity = 0.3f;
    const float flySpeed = 2.0f;
    const float flySpeedBoost = 7.0f;

    float yaw = 0;
    float pitch = 0;
    glm::vec3 pos = camera->GetTransform().GetPosition();
    glm::vec3 rot(0.0f);

    rgr::Cursor::CURSOR_STATE cursorState = rgr::Cursor::GetCursorState();

    while (rgr::Core::AppShouldRun())
    {
        glm::vec2 mouseDelta = rgr::Input::GetMouseDelta();

        if (rgr::Cursor::GetCursorState() == rgr::Cursor::CURSOR_STATE::DISABLED)
        {
            yaw -= mouseDelta.x * sensitivity * rgr::Time::GetDeltaTimeF();
            pitch -= mouseDelta.y * sensitivity * rgr::Time::GetDeltaTimeF();
        }

        if (pitch > 1.5708f) pitch = 1.5708f;
        else if (pitch < -1.5708f) pitch = -1.5708f;

        glm::vec3 fv = camera->GetTransform().GetForwardVector();
        glm::vec3 rv = camera->GetTransform().GetRightVector();

        const float speed = rgr::Input::KeyHold(RGR_KEY_LEFT_SHIFT) ? flySpeedBoost : flySpeed;

        if (rgr::Input::KeyHold(RGR_KEY_W))
            pos += fv * speed * rgr::Time::GetDeltaTimeF();
        if (rgr::Input::KeyHold(RGR_KEY_S))
            pos -= fv * speed * rgr::Time::GetDeltaTimeF();
        if (rgr::Input::KeyHold(RGR_KEY_D))
            pos += rv * speed * rgr::Time::GetDeltaTimeF();
        if (rgr::Input::KeyHold(RGR_KEY_A))
            pos -= rv * speed * rgr::Time::GetDeltaTimeF();

        if (rgr::Input::KeyHold(RGR_KEY_Q))
            pos += glm::vec3(0.0f, 1.0f, 0.0f) * speed * rgr::Time::GetDeltaTimeF();
        if (rgr::Input::KeyHold(RGR_KEY_E))
            pos -= glm::vec3(0.0f, 1.0f, 0.0f) * speed * rgr::Time::GetDeltaTimeF();

        if (rgr::Input::KeyPressed(RGR_KEY_ESCAPE))
        {
            if (rgr::Cursor::GetCursorState() == rgr::Cursor::CURSOR_STATE::NORMAL)
                rgr::Cursor::SetCursorState(rgr::Cursor::CURSOR_STATE::DISABLED);
            else if(rgr::Cursor::GetCursorState() == rgr::Cursor::CURSOR_STATE::DISABLED)
                rgr::Cursor::SetCursorState(rgr::Cursor::CURSOR_STATE::NORMAL);
        }

        camera->GetTransform().SetPosition(pos);
        camera->GetTransform().SetRotation(glm::quat(glm::vec3(pitch, yaw, 0.0f)));

        rot += glm::vec3(1.0f, -1.0f, 0.5f) * rgr::Time::GetDeltaTimeF();

        sptLight->GetTransform().SetPosition(camera->GetTransform().GetPosition() + glm::vec3(0, -1, 0));
        sptLight->direction = camera->GetForwardVector();

        rgr::Core::Update();
    }

    rgr::Core::Quit();
}
