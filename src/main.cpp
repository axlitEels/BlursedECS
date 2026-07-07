#include "ecs/world.hpp"
#include "systems.hpp"
#include "components.hpp"

using namespace cmps;

int main() {
    ecs::World world<TransformComponent, HealthComponent>();


    ecs::Entity player = world.spawn();
    world.emplace_component<TransformComponent>(player, 0, 0, 0);
    world.emplace_component<HealthComponent>(player, 10);

    ecs::Entity enemy = world.spawn();
    world.emplace_component<TransformComponent>(enemy, 1, 2, 5);
    world.emplace_component<HealthComponent>(enemy, 30);

    ecs::Entity brick = world.spawn();
    world.emplace_component<TransformComponent>(brick, -2, -2, -2);


    systems::MovementSystem movement(-5, 0, 3);
    systems::DrawingSystem drawing;

    world.add_systems(movement, drawing);

    for (int i = 0; i < 3; i++) {
        world.update();
    }

    return 0;
}
