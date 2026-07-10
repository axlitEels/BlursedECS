#include "components.hpp"
#include "ecs/ecs.hpp"
#include "ecs/lua.hpp"
#include "systems.hpp"

using namespace cmps;

ecs::World init_world() {
    ecs::World world;

    world.register_components<TransformComponent, HealthComponent>();

    ecs::Entity player = world.spawn();
    world.emplace_or_get<TransformComponent>(player, Vec3{0, 0, 0});
    world.emplace_or_get<HealthComponent>(player, 10);

    ecs::Entity enemy = world.spawn();
    world.put_or_replace(enemy, TransformComponent(Vec3{1, 2, 5}));
    world.put_or_replace(enemy, HealthComponent(30));

    ecs::Entity brick = world.spawn();
    TransformComponent brick_transform(Vec3{-2, -2, -2});
    // This move is unnecessary, as brick_transform does not use on heap
    // But this is how one would do it if it did require moves
    world.put_or_replace(brick, std::move(brick_transform));

    return world;
}

void demo() {
    ecs::World world = init_world();

    // Systems will be executed in the order they're added
    // Conditional/simultaneous execution is not implemented
    world.add_system<systems::MovementSystem>(Vec3{-5, 0, 3});
    world.add_system<systems::DrawingSystem>();

    for (int i = 0; i < 3; i++) {
        world.update();
    }
}
ecs::lua::LuaSystem configure_lua(ecs::World& world) {
    ecs::lua::LuaSystem lua;

    // Only register types & components we want to have accessible in Lua
    lua.expose_type<Vec3>("Vec3", "x", &Vec3::x, "y", &Vec3::y, "z", &Vec3::z);
    lua.expose_type<TransformComponent>(
        "Transform", "position", &TransformComponent::position, "rotation",
        &TransformComponent::rotation, "scale", &TransformComponent::scale);

    // World getter (using raw pointer to get nil semantics in Lua)
    lua.expose_function(
        "get_transform",
        [](ecs::World& world, ecs::Entity e) -> TransformComponent* {
            auto& opt = world.get<TransformComponent>(e);
            if (opt.has_value()) {
                return &opt.value();
            }
            return nullptr; // = nil
        });

    // Import the Lua code
    assert(lua.load_lua_file("scripts/example.lua", world));
    assert(lua.add_lua_system("sys_spin"));

    return lua;
}

void lua_demo() {
    ecs::World world = init_world();

    world.add_system<systems::DrawingSystem>();

    world.add_system<ecs::lua::LuaSystem>(configure_lua(world));

    for (int i = 0; i < 5; i++) {
        world.update();
    }
}

int main() {
    std::cout << "  ### ECS DEMO ###" << std::endl;
    demo();

    std::cout << "\n  ### LUA DEMO ###" << std::endl;
    lua_demo();

    return 0;
}
