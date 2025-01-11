#include "data_type.hpp"
#include "dynamic_shape.hpp"

class Edge : public DynamicShape{
    public:
        const EdgeData& data_;

    explicit Edge(const EdgeData& data ) : DynamicShape(5), data_(data) {
        
    }
};