#include "systems.hpp"
#include "components.hpp"

#include <iostream>

namespace systems {

void MovementSystem::run(ecs::World& world) {
    world.for_each<cmps::TransformComponent>(
        [&](cmps::TransformComponent& comp) { comp.position += wind_vector; });
}

const std::string BAR = "########################################";

void DrawingSystem::run(ecs::World& world) {
    std::cout << '\n' << BAR << '\n';
    std::cout << "    TURN #" << turn << '\n';
    std::cout << BAR << '\n';

    for (auto i = world.begin(), end = world.end(); i != end; ++i) {
        ecs::Entity e = *i;
        std::cout << "ENTITY #" << e << '\n';

        if (const auto& ts = world.get<cmps::TransformComponent>(e)) {
            std::cout << "  Position: " << ts->position.x << ", "
                      << ts->position.y << ", " << ts->position.z << '\n';
        }

        if (const auto& health = world.get<cmps::HealthComponent>(e)) {
            std::cout << "  Health: " << health->health << '/'
                      << health->max_health << '\n';
        }
    }

    std::cout << BAR << std::endl; // Flush

    ++turn;
}

} // namespace systems
