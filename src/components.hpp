#ifndef COMPONENTS_HPP_
#define COMPONENTS_HPP_

#include "aux.hpp"

namespace cmps {

struct TransformComponent {
    Vec3 position{0, 0, 0};
    Vec3 rotation{0, 0, 0};
    Vec3 scale{1, 1, 1};
};

struct HealthComponent {
    const int BASE_HEALTH = 100;

    int health = BASE_HEALTH;
    int max_health = BASE_HEALTH;

    HealthComponent() = default;
    HealthComponent(int max_health)
        : health(max_health), max_health(max_health) {}
};

}  // namespace cmps

#endif // COMPONENTS_HPP_
