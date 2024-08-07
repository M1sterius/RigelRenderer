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

    rgr::Cursor::SetCursorState(rgr::Cursor::CURSOR_STATE::DISABLED);

    auto scene = new rgr::Scene();
    scene->name = "Test Scene";
    rgr::Core::LoadScene(scene);

    auto backpackModel = std::make_shared<rgr::Model>("resources/models/backpack/backpack.obj");
    auto cubeModel = std::make_shared<rgr::Model>("resources/models/cube/cube.obj");

    auto backpack = std::make_shared<rgr::RenderableModel>(backpackModel);
    backpack->GetTransform().SetPosition(glm::vec3(0, 0, 0));
    backpack->GetTransform().SetScale(glm::vec3(0.5, 0.5, 0.5));

    auto cube = std::make_shared<rgr::RenderableModel>(cubeModel);
    cube->GetTransform().SetPosition(glm::vec3(0, 0, 2));

    auto camera = std::make_shared<rgr::Camera>(glm::radians(75.0f), WIDTH, HEIGHT, 0.1f, 100.0f);
    camera->GetTransform().SetPosition(glm::vec3(-3.0f, 0, 0.0f));
    camera->FlagAsMain();

    auto dirLight = std::make_shared<rgr::DirectionalLight>(glm::vec3(1.0, 1.0, 1.0), 0.6f, glm::vec3(-0.1, -1, 1));
    dirLight->GetTransform().SetPosition(glm::vec3(0.1, 5, -5));

    auto dirLight1 = std::make_shared<rgr::DirectionalLight>(glm::vec3(1.0, 1.0, 1.0), 0.3f, glm::vec3(-0.1, -1, -1));
    dirLight1->GetTransform().SetPosition(glm::vec3(0.1, 5, 5));

//    auto pntLight = std::make_shared<rgr::PointLight>(
//            glm::vec3(0.98, 0.76, 0.12),
//            3.0f,
//            1.0f,
//            0.7f,
//            1.8f
//    );
//    pntLight->GetTransform().SetPosition(glm::vec3(0, 1.2f, 0));
//
//    auto pntLight1 = std::make_shared<rgr::PointLight>(
//            glm::vec3(1, 1, 1),
//            2.0f,
//            1.0f,
//            0.7f,
//            1.8f
//    );
//    pntLight1->GetTransform().SetPosition(glm::vec3(-2.5, -0.9f, 0));
//
//    auto sptLight = std::make_shared<rgr::SpotLight>(
//            glm::vec3(1.0f, 1.0f, 1.0f),
//            3.0f, glm::vec3(0.05, -1.0, 0.0),
//            0.9978f, 0.953f,
//            1.0f, 0.22f, 0.2f
//    );
//    sptLight->GetTransform().SetPosition(glm::vec3(0.0, -1.3, 0));
//
//    auto sptLight1 = std::make_shared<rgr::SpotLight>(
//            glm::vec3(1.0f, 1.0f, 1.0f),
//            2.0f, glm::vec3(3, -3, 4),
//            0.9978f, 0.953f,
//            1.0f, 0.22f, 0.2f
//    );
//    sptLight1->smoothShadows = true;
//    sptLight1->GetTransform().SetPosition(glm::vec3(-3, 2, -4));

    scene->AddObject(camera);
    scene->AddObject(backpack);
    scene->AddObject(cube);

    scene->AddObject(dirLight);
    scene->AddObject(dirLight1);
//    scene->AddObject(pntLight);
//    scene->AddObject(pntLight1);
//    scene->AddObject(sptLight);
//    scene->AddObject(sptLight1);

    const float sensitivity = 0.3f;
    const float flySpeed = 2.0f;
    const float flySpeedBoost = 4.5f;

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

        auto x = (float)glm::cos(rgr::Time::GetTimeF() * 3);
        auto z = (float)glm::sin(rgr::Time::GetTimeF() * 3);
//        sptLight->direction = glm::vec3(x, 0, z);
//
//        sptLight1->GetTransform().SetPosition(camera->GetTransform().GetPosition() + glm::vec3(0.0f, -1.0f, 0.0f));
//        sptLight1->direction = camera->GetForwardVector();

        rgr::Core::Update();
    }

    rgr::Core::Quit();
}
