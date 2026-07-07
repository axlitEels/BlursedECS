#ifndef ECS_STORAGE_HPP_
#define ECS_STORAGE_HPP_

namespace ecs {

namespace impl {

class IComponentStorage {};

template<typename T>
class ComponentStorage : public IComponentStorage {

};

} // namespace impl

} // namespace ecs

#endif // ECS_STORAGE_HPP_
