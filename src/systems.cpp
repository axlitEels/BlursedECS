#include "systems.hpp"
#include "components.hpp"

namespace systems {

void MovementSystem::run(ecs::World& world) {
    world.for_each<cmps::TransformComponent>(
        [&](cmps::TransformComponent& comp) { comp.position += wind_vector; });
}

void DrawingSystem::run(ecs::World& world) {
    for (auto i = world.begin(), end = world.end(); i != end; ++i) {
        ecs::Entity e = *i;
        // TODO: print out stuff
    }
}

} // namespace systems
