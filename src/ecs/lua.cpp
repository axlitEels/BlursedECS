#include "lua.hpp"
#include <sol/load_result.hpp>

#ifndef LUA_NO_PRINT
#include <iostream>
#endif

namespace ecs::lua {

LuaSystem::LuaSystem() {
    lua.open_libraries(sol::lib::base, sol::lib::math);

    lua.new_usertype<ecs::World>("World");

    // Just allow iterating through entities
    // Everything else should be done (or deliberately NOT done) on users' side
    lua.set_function(
        "for_each_entity", [](ecs::World& world, sol::function callback) {
            for (auto entity : world) {
                auto result = callback(entity);
                if (!result.valid()) {
                    // These errors can't be captured by the system runner
                    // as even if there was an error, execution continues
                    sol::error err = result;
                    std::cerr << "Error in for_each_entity callback: " << err.what()
                              << std::endl;
                }
            }
        });

#ifndef LUA_NO_PRINT
    lua.set_function("print", [](const char* message) {
        std::cout << message << std::endl;
    });
#endif
}

void LuaSystem::run(ecs::World& world) {
    lua["world"] = std::ref(world);
    for (auto& system : systems) {
        sol::protected_function func = lua[system];
        auto result = func();
        if (!result.valid()) {
            sol::error err = result;
            std::cerr << "Lua error in system '" << system
                      << "': " << err.what() << std::endl;
        }
    }
}

bool LuaSystem::load_lua(std::string code, ecs::World& world) {
    sol::load_result script = lua.load(code);
    if (!script.valid())
        return false;

    lua["world"] = std::ref(world);
    script();
    return true;
}

bool LuaSystem::load_lua_file(std::string path, ecs::World& world) {
    sol::load_result script = lua.load_file(path);
    if (!script.valid())
        return false;

    lua["world"] = std::ref(world);
    script();
    return true;
}

bool LuaSystem::add_lua_system(std::string name) {
    if (!lua[name].valid())
        return false;

    systems.push_back(std::move(name));
    return true;
}

} // namespace ecs::lua
