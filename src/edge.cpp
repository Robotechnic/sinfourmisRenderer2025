#include "data_type.hpp"
#include "dynamic_shape.hpp"

class Edge : public DynamicShape{
    public:
        const WorldData& world_;
        const EdgeData& data_;

    explicit Edge(const WorldData& world, const EdgeData& data ) : DynamicShape(5), world_(world), data_(data) {
        
    }
};