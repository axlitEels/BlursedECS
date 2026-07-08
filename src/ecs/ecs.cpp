#include "ecs.hpp"
#include "storage.hpp"

namespace ecs {

namespace impl {

ComponentID EntityDescriptor::operator[](ComponentTypeID type) const {
    if (type < components.size())
        return components[type];
    else
        return NULL_COMPONENT;
}

bool EntityDescriptor::add_component(ComponentTypeID type, ComponentID id) {
    if (type > components.size())
        components.resize(type, NULL_COMPONENT);
    else if (components[type])
        return false; // Can't add a component over another component

    components[type] = id;
    return true;
}

bool EntityDescriptor::remove_component(ComponentTypeID type) {
    if (type > components.size() || !components[type])
        return false;

    components[type] = NULL_COMPONENT;
    return true;
}

void EntityDescriptor::resize(ComponentTypeID n) {
    if (n > components.capacity()) // Does not allow dropping elements
        components.resize(n, NULL_COMPONENT);
}

} // namespace impl

Entity World::spawn() {
    Entity e;
    impl::ComponentTypeID type_count = component_registry.get_type_count();

    if (vacant_entities.empty()) {
        e = entities.size();

        entities.push_back(impl::EntityDescriptor(type_count));
    } else {
        e = vacant_entities.front();
        vacant_entities.pop();

        entities[e].resize(type_count);
    }
    return e;
}

void World::update() {
    for (auto &system : systems)
        system->run(*this);
}

} // namespace ecs
