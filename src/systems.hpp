#ifndef SYSTEMS_HPP_
#define SYSTEMS_HPP_

#include "aux.hpp"
#include "ecs/ecs.hpp"

namespace systems {

class MovementSystem : public ecs::System {
  public:
    MovementSystem() = default;
    MovementSystem(Vec3 vec) : wind_vector(vec) {
    }

    virtual System &operator()(ecs::World &world) override;

  private:
    Vec3 wind_vector;
};

class DrawingSystem : public ecs::System {
  public:
    virtual System &operator()(ecs::World &world) override;
};

}  // namespace systems

#endif  // SYSTEMS_HPP_
