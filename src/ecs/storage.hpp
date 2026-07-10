#ifndef BLURSEDECS_STORAGE_HPP_
#define BLURSEDECS_STORAGE_HPP_

#include <deque>
#include <optional>
#include <queue>

namespace ecs {

namespace impl {

using ComponentID = std::size_t;
const ComponentID NULL_COMPONENT = 0;

class IComponentStorage {
  public:
    virtual bool remove(ComponentID id) = 0;

    virtual ~IComponentStorage() {}
};

template <typename T>
class ComponentStorage : public IComponentStorage {
  public:
    // A (hopefully proper) forward iterator
    // Name is lowercase in case the thing implements Container one day.
    class iterator {
      public:
        using difference_type = std::ptrdiff_t;
        using value_type = T;
        using deque_iter = std::deque<std::optional<value_type>>::iterator;

        iterator() = default;
        iterator(deque_iter iter, deque_iter end) : iter(iter), end(end) {}

        value_type& operator*() const { return **iter; }

        iterator& operator++() {
            do {
                ++iter;
            } while (iter != end && !iter->has_value());
            return *this;
        }

        iterator operator++(int) {
            auto tmp = *this;
            ++*this;
            return tmp;
        }

        bool operator==(const iterator& other) const {
            return iter == other.iter && end == other.end;
        }

      private:
        deque_iter iter, end;
    };
    static_assert(std::forward_iterator<iterator>);

    template <typename... Args>
    ComponentID emplace(Args&&... args) {
        if (vacant.empty()) {
            components.push_back(std::forward<Args>(args)...);
            return components.size() - 1;
        } else {
            ComponentID id = vacant.front();
            vacant.pop();
            components[id]= T(std::forward<Args>(args)...);
            return id;
        }
    }

    ComponentID put(T&& component) {
        if (vacant.empty()) {
            components.push_back(std::forward<T>(component));
            return components.size() - 1;
        } else {
            ComponentID id = vacant.front();
            vacant.pop();
            components[id] = component;
            return id;
        }
    }

    virtual bool remove(ComponentID id) override {
        if (id > components.size() || components[id] == std::nullopt)
            return false;

        components[id] = std::nullopt;
        vacant.push(id);
        return true;
    }

    std::optional<T>& operator[](ComponentID id) {
        return components[id];
    }

    iterator begin() {
        // Incrementing to skip NULL_COMPONENT
        return iterator(components.begin() + 1, components.end());
    }

    iterator end() { return iterator(components.end(), components.end()); }

  private:
    // Reserve for NULL_COMPONENT
    std::deque<std::optional<T>> components = std::deque<std::optional<T>>(1);
    std::queue<ComponentID> vacant;
};

} // namespace impl

} // namespace ecs

#endif // BLURSEDECS_STORAGE_HPP_
