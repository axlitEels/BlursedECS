#ifndef ECS_HPP_
#define ECS_HPP_

#include "storage.hpp"

#include <queue>
#include <typeindex>
#include <unordered_map>
#include <vector>
#include <memory>

namespace ecs {

using Entity = std::size_t;
using ComponentTypeID = std::size_t;
using ComponentID = std::size_t;

// const Entity NULL_ENTITY = 0;
const ComponentID NULL_COMPONENT = 0;

namespace impl {

class EntityDescriptor {
  public:
    EntityDescriptor() = default;
    EntityDescriptor(ComponentTypeID n) : components(n, NULL_COMPONENT) {}

    ComponentID operator[](ComponentTypeID type) const;

    bool add_component(ComponentTypeID type, ComponentID id);
    bool remove_component(ComponentTypeID type);

    bool is_valid() const { return components.size(); }
    void clear() { components.clear(); }
    void resize(ComponentTypeID n) {
        if (n > components.capacity()) // Does not allow dropping elements
            components.resize(n, NULL_COMPONENT);
    }

  private:
    std::vector<ComponentID> components;
};

class ComponentRegistry {
  public:
    template <typename T>
    bool register_component();

    ComponentTypeID get_type_id(std::type_index index) const {
        return registered_components.at(index);
    }

    template <typename T>
    ComponentTypeID get_type_id() const {
        return registered_components.at(std::type_index(typeid(T)));
    }

    template <typename T>
    ComponentTypeID get_type_id(T comp) const {
        get_type_id<T>();
    }

    ComponentTypeID get_type_count() const { return next_id; }

  private:
    std::unordered_map<std::type_index, ComponentTypeID> registered_components;
    ComponentTypeID next_id = 0;
};

} // namespace impl

class System; // Declaring early so that World can store them

class World {
  public:
    template <typename T, typename... Ts>
    void register_components() {
        if (component_registry.register_component<T>())
            storages.push_back(impl::ComponentStorage<T>());
        if (sizeof...(Ts))
            register_components<Ts...>();
    }

    template <typename T, typename... Args>
    T &emplace(Entity, Args &&...args) {
        auto& istorage = storages[component_registry.get_type_id<T>()];
        auto& storage = static_cast<impl::ComponentStorage<T>&>(istorage);
        storage.emplace_back(std::forward<Args>(args)...); // TODO: Test if forward matters
    }

    Entity spawn();

    template <typename T, typename... Args>
    void add_system(Args &&...args) {
        systems.push_back(std::make_unique(T(args...)));
    }

  private:

    impl::ComponentRegistry component_registry;
    std::vector<impl::IComponentStorage> storages;

    std::deque<impl::EntityDescriptor> entities;
    // Reserve for NULL_ENTITY
    // = std::deque<impl::EntityDescriptor>(1);
    std::queue<Entity> vacant_entities;

    std::vector<std::unique_ptr<System>> systems;
};

class System {
  public:
    virtual System &operator()(World &world) = 0;
};

} // namespace ecs

#endif // ECS_HPP_
