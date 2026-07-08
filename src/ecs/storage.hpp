#ifndef ECS_STORAGE_HPP_
#define ECS_STORAGE_HPP_

#include <deque>
#include <tiny/optional.h>

namespace ecs {

namespace impl {

using ComponentID = std::size_t;
const ComponentID NULL_COMPONENT = 0;

class IComponentStorage {};

template <typename T>
class ComponentStorage : public IComponentStorage {
  public:
    template <typename... Args>
    ComponentID emplace(Args &&...args) {
        // TODO
    }

    T &operator[](ComponentID id) {
        return *components[id];
        // TODO: safety
    }

  private:
    std::deque<tiny::optional<T>> components;
};

} // namespace impl

} // namespace ecs

#endif // ECS_STORAGE_HPP_
