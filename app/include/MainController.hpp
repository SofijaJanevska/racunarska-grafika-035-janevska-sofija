#ifndef MAINCONTROLLER_HPP
#define MAINCONTROLLER_HPP

#include <engine/core/Controller.hpp>
#include <glm/vec3.hpp>

namespace app {
    class MainController : public engine::core::Controller {
        void initialize() override;

        bool loop() override;

        void draw_skybox();

        void draw_diplodocus();

        void draw_rex();

        void update_camera();

        void update_point_light();

        void update_point_light_actions();

        void update() override;

        void begin_draw() override;

        void draw() override;

        void end_draw() override;

        glm::vec3 pointLightPos   = glm::vec3(2.0f, 4.0f, 3.0f);
        glm::vec3 pointLightColor = glm::vec3(1.0f, 0.9f, 0.7f);

        float actionTimer = 0.0f;
        int actionStage   = 0;
        bool pPressed     = false;

    public:
        std::string_view name() const override {
            return "app::MainController";
        }
    };
}

#endif
