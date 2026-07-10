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

World::iterator::iterator(World::iterator::deque_iter iter,
                          World::iterator::deque_iter end, std::size_t cur)
    : iter(iter), end(end), cur(cur) {
    if (iter != end && !iter->size()) // Skip first vacant entities, if any
        ++(*this);
}

World::iterator& World::iterator::operator++() {
    do {
        ++iter;
        ++cur;
    } while (iter != end && !iter->size());
    return *this;
}

World::iterator World::iterator::operator++(int) {
    auto tmp = *this;
    ++*this;
    return tmp;
}

bool World::iterator::operator==(const iterator& other) const {
    return iter == other.iter && end == other.end;
}

Entity World::spawn() {
    Entity e;
    // Pass this to avoid reallocations later
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

World::iterator World::begin() {
    return World::iterator(entities.begin(), entities.end(), 0);
}

World::iterator World::end() {
    return World::iterator(entities.end(), entities.end(), entities.size());
}

bool World::destroy(Entity e) {
    impl::EntityDescriptor desc = entities[e];
    if (!desc.size())
        return false;

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

void World::update() {
    for (auto& system : systems)
        system->run(*this);
}

} // namespace ecs
