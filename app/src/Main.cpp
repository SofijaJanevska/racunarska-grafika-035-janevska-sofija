#include <MyApp.hpp>
#include <engine/core/Engine.hpp>

/**
 * Start here...
 */
int main(int argc, char **argv) {
    // printf("bla\n");
    auto app = std::make_unique<app::MyApp>();
    return app->run(argc, argv);
}
