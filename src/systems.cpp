#include "systems.hpp"
#include "components.hpp"

namespace systems {

void MovementSystem::run(ecs::World& world) {
    world.for_each<cmps::TransformComponent>(
        [&](cmps::TransformComponent& comp) {
            comp.position += wind_vector;
        });
}

void DrawingSystem::run(ecs::World& world) {
    // TODO: iterate through entities, printing PositionComponent and
    // HealthComponents of them
}

} // namespace systems
