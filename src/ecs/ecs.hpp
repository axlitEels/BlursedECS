#ifndef ECS_HPP_
#define ECS_HPP_

#include "storage.hpp"

#include <deque>
#include <memory>
#include <queue>
#include <typeindex>
#include <unordered_map>
#include <vector>

namespace ecs {

using Entity = std::size_t;
// const Entity NULL_ENTITY = 0;

namespace impl {

using ComponentTypeID = std::size_t;

class EntityDescriptor {
  public:
    EntityDescriptor() = default;
    EntityDescriptor(ComponentTypeID n) : components(n, NULL_COMPONENT) {}

    ComponentID operator[](ComponentTypeID type) const;

    bool add_component(ComponentTypeID type, ComponentID id);
    bool remove_component(ComponentTypeID type);

    ComponentTypeID size() const { return components.size(); }
    void clear() { components.clear(); }
    void resize(ComponentTypeID n);

  private:
    std::vector<ComponentID> components;
};

class ComponentRegistry {
  public:
    template <typename T>
    bool register_component() {
        auto type = std::type_index(typeid(T));

        if (registered_components.contains(type))
            return false;

        registered_components.insert({type, registered_components.size()});
        return true;
    }

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

    ComponentTypeID get_type_count() const {
        return registered_components.size();
    }

  private:
    std::unordered_map<std::type_index, ComponentTypeID> registered_components;
};

} // namespace impl

class System; // Declaring early so that World can store them

class World {
  public:
    template <typename T, typename... Ts>
    void register_components() {
        if (component_registry.register_component<T>())
            storages.push_back(std::make_unique<impl::ComponentStorage<T>>());
        if constexpr (sizeof...(Ts) > 0)
            register_components<Ts...>();
    }

    template <typename T, typename... Args>
    T& emplace_or_get(Entity entity, Args&&... args) {
        impl::ComponentTypeID type = component_registry.get_type_id<T>();
        impl::ComponentStorage<T>& storage = get_storage<T>();
        impl::EntityDescriptor& descriptor = entities[entity];

        if (descriptor[type] == impl::NULL_COMPONENT) {
            impl::ComponentID id = storage.emplace(std::forward<Args>(args)...);
            descriptor.add_component(type, id);
        }

        return storage[descriptor[type]];
    }

    // TODO:
    // get()
    // iterators()

    Entity spawn();

    template <typename T>
    bool remove(Entity e) {
        impl::ComponentTypeID type = component_registry.get_type_id<T>();
        impl::EntityDescriptor desc = entities[e];
        return storages[type]->remove(desc[type]);
    }

    bool destroy(Entity e) {
        impl::EntityDescriptor desc = entities[e];
        if (!desc.size()) return false;

        impl::ComponentTypeID n = desc.size();
        for (impl::ComponentTypeID i = 0; i < n; ++i) {
            if (desc[i]) {
                storages[i]->remove(desc[i]);
            }
        }

        entities[e].clear();
        vacant_entities.push(e);
        return true;
    }

    template <typename T, typename... Args>
    void add_system(Args&&... args) {
        systems.push_back(std::make_unique<T>(std::forward<Args>(args)...));
    }

    template <typename T, class F>
    void for_each(F func) {
        impl::ComponentStorage<T>& storage = get_storage<T>();
        for (auto i = storage.begin(), end = storage.end(); i != end; ++i)
            func(*i);
    }

    void update();

  private:
    template <typename T>
    impl::ComponentStorage<T>& get_storage() const {
        impl::ComponentTypeID type = component_registry.get_type_id<T>();
        return dynamic_cast<impl::ComponentStorage<T>&>(*storages[type]);
    }

    impl::ComponentRegistry component_registry;
    std::vector<std::unique_ptr<impl::IComponentStorage>> storages;

    std::deque<impl::EntityDescriptor> entities;
    // Reserve for NULL_ENTITY
    // = std::deque<impl::EntityDescriptor>(1);
    std::queue<Entity> vacant_entities;

    std::vector<std::unique_ptr<System>> systems;
};

class System {
  public:
    virtual void run(World& world) = 0;

    virtual ~System() = default;
};

} // namespace ecs

#endif // ECS_HPP_
