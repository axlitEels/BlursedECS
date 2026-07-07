#include "components.hpp"
#include "ecs/ecs.hpp"
#include "systems.hpp"

using namespace cmps;

int main() {
    ecs::World world;

    world.register_components<TransformComponent, HealthComponent>();

    ecs::Entity player = world.spawn();
    world.emplace<TransformComponent>(player, 0, 0, 0);
    world.emplace<HealthComponent>(player, 10);

    ecs::Entity enemy = world.spawn();
    world.emplace<TransformComponent>(enemy, 1, 2, 5);
    world.emplace<HealthComponent>(enemy, 30);

    ecs::Entity brick = world.spawn();
    world.emplace<TransformComponent>(brick, -2, -2, -2);

    world.add_system<systems::MovementSystem>(Vec3{-5, 0, 3});
    world.add_system<systems::DrawingSystem>();

    for (int i = 0; i < 3; i++) {
        world.update();
    }

    return 0;
}
