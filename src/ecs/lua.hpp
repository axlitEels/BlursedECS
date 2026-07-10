#ifndef BLURSEDECS_LUA_HPP_
#define BLURSEDECS_LUA_HPP_

#include "ecs.hpp"

#define SOL_ALL_SAFETIES_ON 1

#include <sol/sol.hpp>
#include <vector>

// Lua scripting integration for Blursed-ECS
namespace ecs::lua {

class LuaSystem : public ecs::System {
  public:
    LuaSystem();

    virtual void run(ecs::World& world) override;

    template <typename F>
    void expose_function(std::string name, F func) {
        lua.set_function(name, std::forward<F>(func));
    }

    template <typename Class, typename... Args>
		sol::usertype<Class> expose_type(Args&&... args) {
			return lua.new_usertype<Class>(std::forward<Args>(args)...);
		}

    bool load_lua(std::string code, ecs::World& world);
    bool load_lua_file(std::string path, ecs::World& world);

    bool add_lua_system(std::string name);

  private:
    sol::state lua;
    std::vector<std::string> systems;
};

} // namespace ecs::lua

#endif // BLURSEDECS_LUA_HPP_
