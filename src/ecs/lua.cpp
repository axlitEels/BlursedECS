#include "lua.hpp"
#include <sol/load_result.hpp>

namespace ecs::lua {

void LuaSystem::run(ecs::World& world) {
    lua["world"] = &world;
    for (auto& script : scripts) {
        script();
    }
}

bool LuaSystem::run_lua(std::string code, ecs::World& world) {
    sol::load_result script = lua.load(code);
    if (!script.valid())
        return false;

    lua["world"] = &world;
    script();
    return true;
}

bool LuaSystem::add_lua_system(std::string code) {
    sol::load_result script = lua.load(code);
    if (!script.valid())
        return false;

    scripts.push_back(std::move(script));
    return true;
}

bool LuaSystem::run_lua_file(std::string path, ecs::World& world) {
    sol::load_result script = lua.load_file(path);
    if (!script.valid())
        return false;
    
    lua["world"] = &world;
    script();
    return true;
}

bool LuaSystem::add_lua_system_file(std::string path) {
    sol::load_result script = lua.load_file(path);
    if (!script.valid())
        return false;

    scripts.push_back(std::move(script));
    return true;
}

} // namespace ecs::lua
