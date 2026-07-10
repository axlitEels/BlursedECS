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

    template <typename T, typename... Args>
		sol::usertype<T> expose_type(Args&&... args) {
			return lua.new_usertype<T>(std::forward<Args>(args)...);
		}

    template <class F>
    void expose_function(std::string name, F func) {
        lua.set_function(name, std::forward<F>(func));
    }

    bool load_lua(std::string code, ecs::World& world);
    bool load_lua_file(std::string path, ecs::World& world);

    bool add_lua_system(std::string name);

    virtual void run(ecs::World& world) override;

  private:
    sol::state lua;
    std::vector<std::string> systems;
};

} // namespace ecs::lua

#endif // BLURSEDECS_LUA_HPP_
