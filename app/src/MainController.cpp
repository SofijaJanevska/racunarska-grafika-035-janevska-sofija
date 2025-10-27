#include "MainController.hpp"

#include <GUIController.hpp>
#include <engine/graphics/GraphicsController.hpp>
#include <engine/graphics/OpenGL.hpp>
#include <engine/platform/PlatformController.hpp>
#include <engine/resources/ResourcesController.hpp>
#include <spdlog/spdlog.h>

namespace app {
    class MainPlatformEventObserver : public engine::platform::PlatformEventObserver {
    public:
        void on_mouse_move(engine::platform::MousePosition position) override;
    };

    void MainPlatformEventObserver::on_mouse_move(engine::platform::MousePosition position) {
        auto gui_controller = engine::core::Controller::get<GUIController>();
        if (!gui_controller->is_enabled()) {
            auto camera = engine::core::Controller::get<engine::graphics::GraphicsController>()->camera();
            camera->rotate_camera(position.dx, position.dy);
        }
    }

    void MainController::initialize() {
        auto platform = engine::core::Controller::get<engine::platform::PlatformController>();
        platform->register_platform_event_observer(std::make_unique<MainPlatformEventObserver>());
        engine::graphics::OpenGL::enable_depth_testing();
    }

    bool MainController::loop() {
        auto platform = engine::core::Controller::get<engine::platform::PlatformController>();
        if (platform->key(engine::platform::KeyId::KEY_ESCAPE).is_down()) {
            return false;
        }
        return true;
    }

    void MainController::draw_skybox() {
        auto resources = engine::core::Controller::get<engine::resources::ResourcesController>();
        auto skybox    = resources->skybox("my_skybox");
        auto shader    = resources->shader("skybox");
        auto graphics  = engine::core::Controller::get<engine::graphics::GraphicsController>();
        graphics->draw_skybox(shader, skybox);
    }

    void MainController::draw_diplodocus() {
        auto resources                       = engine::core::Controller::get<engine::resources::ResourcesController>();
        auto graphics                        = engine::core::Controller::get<engine::graphics::GraphicsController>();
        engine::resources::Model *diplodocus = resources->model("diplodocus");
        engine::resources::Shader *shader    = resources->shader("my_shader");
        shader->use();

        shader->set_vec3("dirLightDir", glm::vec3(-0.2f, -1.0f, -0.3f));
        shader->set_vec3("dirLightColor", glm::vec3(1.0f, 1.0f, 1.0f));
        shader->set_vec3("pointLightPos", pointLightPos);
        shader->set_vec3("pointLightColor", pointLightColor);
        shader->set_vec3("viewPos", graphics->camera()->Position);
        shader->set_vec3("objectColor", glm::vec3(1.0f, 1.0f, 1.0f));
        shader->set_mat4("projection", graphics->projection_matrix());
        shader->set_mat4("view", graphics->camera()->view_matrix());

        glm::mat4 model = glm::mat4(1.0f);
        model           = glm::translate(model, glm::vec3(-5.0f, 0.0f, -9.0f));
        model           = glm::scale(model, glm::vec3(0.007f));
        shader->set_mat4("model", model);
        diplodocus->draw(shader);
    }

    void MainController::draw_rex() {
        auto resources                    = engine::core::Controller::get<engine::resources::ResourcesController>();
        auto graphics                     = engine::core::Controller::get<engine::graphics::GraphicsController>();
        engine::resources::Model *rex     = resources->model("rex");
        engine::resources::Shader *shader = resources->shader("my_shader");
        shader->use();

        shader->set_vec3("dirLightDir", glm::vec3(-0.2f, -1.0f, -0.3f));
        shader->set_vec3("dirLightColor", glm::vec3(1.0f, 1.0f, 1.0f));
        shader->set_vec3("pointLightPos", pointLightPos);
        shader->set_vec3("pointLightColor", pointLightColor);
        shader->set_vec3("viewPos", graphics->camera()->Position);
        shader->set_vec3("objectColor", glm::vec3(1.0f, 1.0f, 1.0f));
        shader->set_mat4("projection", graphics->projection_matrix());
        shader->set_mat4("view", graphics->camera()->view_matrix());

        glm::mat4 model = glm::mat4(1.0f);
        model           = glm::translate(model, glm::vec3(3.0f, 0.0f, -5.0f));
        model           = glm::scale(model, glm::vec3(0.03f));
        shader->set_mat4("model", model);
        rex->draw(shader);
    }

    void MainController::update_camera() {
        auto gui_controller = engine::core::Controller::get<GUIController>();
        if (gui_controller->is_enabled()) {
            return;
        }
        auto platform = engine::core::Controller::get<engine::platform::PlatformController>();
        auto graphics = engine::core::Controller::get<engine::graphics::GraphicsController>();
        auto camera   = graphics->camera();
        float dt      = platform->dt();

        if (platform->key(engine::platform::KeyId::KEY_W).is_down()) {
            camera->move_camera(engine::graphics::Camera::Movement::FORWARD, dt);
        }
        if (platform->key(engine::platform::KeyId::KEY_S).is_down()) {
            camera->move_camera(engine::graphics::Camera::Movement::BACKWARD, dt);
        }
        if (platform->key(engine::platform::KeyId::KEY_A).is_down()) {
            camera->move_camera(engine::graphics::Camera::Movement::LEFT, dt);
        }
        if (platform->key(engine::platform::KeyId::KEY_D).is_down()) {
            camera->move_camera(engine::graphics::Camera::Movement::RIGHT, dt);
        }
    }

    void MainController::update_point_light() {
        auto platform = engine::core::Controller::get<engine::platform::PlatformController>();
        float dt      = platform->dt();

        if (platform->key(engine::platform::KeyId::KEY_J).is_down()) {
            pointLightPos.x -= dt;
        }
        if (platform->key(engine::platform::KeyId::KEY_L).is_down()) {
            pointLightPos.x += dt;
        }
        if (platform->key(engine::platform::KeyId::KEY_I).is_down()) {
            pointLightPos.y += dt;
        }
        if (platform->key(engine::platform::KeyId::KEY_K).is_down()) {
            pointLightPos.y -= dt;
        }
        if (platform->key(engine::platform::KeyId::KEY_U).is_down()) {
            pointLightPos.z -= dt;
        }
        if (platform->key(engine::platform::KeyId::KEY_O).is_down()) {
            pointLightPos.z += dt;
        }
    }

    void MainController::update_point_light_actions() {
        auto platform = engine::core::Controller::get<engine::platform::PlatformController>();
        bool pressed  = platform->key(engine::platform::KeyId::KEY_P).is_down();

        if (pressed && !pPressed) {
            actionStage = 0;
            actionTimer = 0.0f;
        }

        pPressed = pressed;
        if (!pPressed) {
            return;
        }

        float dt = platform->dt();
        actionTimer += dt;

        if (actionStage == 0 && actionTimer >= 0.0f) {
            pointLightPos = glm::vec3(2.0f, 5.0f, 3.0f);
            actionStage   = 1;
            actionTimer   = 0.0f;
        } else if (actionStage == 1 && actionTimer >= 2.0f) {
            pointLightPos = glm::vec3(1.0f, 0.5f, 0.2f);
            actionStage   = 2;
            actionTimer   = 0.0f;
        } else if (actionStage == 2 && actionTimer >= 3.0f) {
            pointLightPos = glm::vec3(0.2f, 0.5f, 1.0f);
            actionStage   = 3;
        }
    }

    void MainController::update() {
        update_camera();
        update_point_light();
        update_point_light_actions();
    }

    void MainController::begin_draw() {
        engine::graphics::OpenGL::clear_buffers();
    }

    void MainController::draw() {
        draw_skybox();
        draw_diplodocus();
        draw_rex();
    }

    void MainController::end_draw() {
        auto platform = engine::core::Controller::get<engine::platform::PlatformController>();
        platform->swap_buffers();
    }
}
