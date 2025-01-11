#include "world.cpp"
#include "data_type.hpp"
#include "dynamic_shape.hpp"

class Node : public DynamicShape {
    public:
        const World& world_;
        const NodeData& data_;

    explicit Node(const World& world, const NodeData& data ) : DynamicShape(5), world_(world), data_(data) {
        
    }
};