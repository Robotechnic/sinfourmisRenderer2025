#include "world.cpp"
#include "data_type.hpp"
#include "dynamic_shape.hpp"

class Edge : public DynamicShape{
    public:
        const World& world_;
        const EdgeData& data_;

    explicit Edge(const World& world, const EdgeData& data ) : DynamicShape(5), world_(world), data_(data) {
        
    }
};