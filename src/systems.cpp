#include "systems.hpp"

namespace systems {

void MovementSystem::run(ecs::World &world) {
    // TODO: iterate through PositionComponent storage, add wind_vector to each
}

void DrawingSystem::run(ecs::World &world) {
    // TODO: iterate through entities, printing PositionComponent and
    // HealthComponents of them
}

}  // namespace systems
