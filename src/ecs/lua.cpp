#include "lua.hpp"
#include <sol/load_result.hpp>

namespace ecs::lua {

void LuaSystem::run(ecs::World& world) {
    lua["world"] = &world;
    for (auto& system : systems) {
        lua[system]();
    }
}

bool LuaSystem::load_lua(std::string code, ecs::World& world) {
    sol::load_result script = lua.load(code);
    if (!script.valid())
        return false;

    lua["world"] = &world;
    script();
    return true;
}

bool LuaSystem::load_lua_file(std::string path, ecs::World& world) {
    sol::load_result script = lua.load_file(path);
    if (!script.valid())
        return false;
    
    lua["world"] = &world;
    script();
    return true;
}

bool LuaSystem::add_lua_system(std::string name) {
    if (!lua[name]) return false;
    
    systems.push_back(std::move(name));
    return true;
}

} // namespace ecs::lua
