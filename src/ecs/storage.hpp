#ifndef ECS_STORAGE_HPP_
#define ECS_STORAGE_HPP_

#include <deque>
#include <tiny/optional.h>

namespace ecs {

namespace impl {

using ComponentID = std::size_t;
const ComponentID NULL_COMPONENT = 0;

class IComponentStorage {
  public:
    virtual ~IComponentStorage() {}
};

template <typename T>
class ComponentStorage : public IComponentStorage {
  public:
    class Iterator {
      public:
        using difference_type = std::ptrdiff_t;
        using value_type = T;

        value_type& operator*() const;

        Iterator& operator++();
        Iterator operator++(int) {
            auto tmp = *this;
            ++*this;
            return tmp;
        }

        bool operator==(const Iterator&) const;

      private:
        std::deque<std::optional<T>>::iterator iter;
        std::deque<std::optional<T>>::iterator end;
    };

    static_assert(std::forward_iterator<Iterator>);

    template <typename... Args>
    ComponentID emplace(Args&&... args) {
        components.push_back(std::forward<Args>(args)...);
        return components.size() - 1;
    }

    T& operator[](ComponentID id) {
        return *components[id];
        // TODO: safety
    }

    // TODO:
    // replace()
    // custom iterators

  private:
    std::deque<std::optional<T>> components;
};

} // namespace impl

} // namespace ecs

#endif // ECS_STORAGE_HPP_
