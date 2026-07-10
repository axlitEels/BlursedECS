#ifndef BLURSEDECS_LUA_HPP_
#define BLURSEDECS_LUA_HPP_

#include "ecs.hpp"

// #define SOL_ALL_SAFETIES_ON 1

#include <functional>
#include <sol/sol.hpp>
#include <vector>

// Lua scripting integration for Blursed-ECS
namespace ecs::lua {

class LuaSystem : public ecs::System {
  public:
    virtual void run(ecs::World& world) override;

    template <class R, class... Args>
    void add_function(std::string name, std::function<R(Args...)> func) {
        lua[name] = func;
    }

    bool run_lua(std::string code, ecs::World& world);
    bool add_lua_system(std::string code);

    bool run_lua_file(std::string path, ecs::World& world);
    bool add_lua_system_file(std::string path);

  private:
    sol::state lua;
    std::vector<sol::load_result> scripts;
};

} // namespace ecs::lua

#endif // BLURSEDECS_LUA_HPP_
