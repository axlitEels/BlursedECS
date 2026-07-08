#include "components.hpp"
#include "ecs/ecs.hpp"
#include "systems.hpp"

using namespace cmps;

int main() {
    ecs::World world;

    world.register_components<TransformComponent, HealthComponent>();

    ecs::Entity player = world.spawn();
    world.emplace_or_get<TransformComponent>(player, Vec3{0, 0, 0});
    world.emplace_or_get<HealthComponent>(player, 10);

    ecs::Entity enemy = world.spawn();
    world.emplace_or_get<TransformComponent>(enemy, Vec3{1, 2, 5});
    world.emplace_or_get<HealthComponent>(enemy, 30);

    ecs::Entity brick = world.spawn();
    world.emplace_or_get<TransformComponent>(brick, Vec3{-2, -2, -2});

    world.add_system<systems::MovementSystem>(Vec3{-5, 0, 3});
    world.add_system<systems::DrawingSystem>();

    for (int i = 0; i < 3; i++) {
        world.update();
    }

    return 0;
}
