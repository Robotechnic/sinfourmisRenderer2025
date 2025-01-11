#include "data_type.hpp"
#include "dynamic_shape.hpp"

class Node : public DynamicShape {
    public:
        const NodeData& data_;

    explicit Node(const NodeData& data ) : DynamicShape(5), data_(data) {
        
    }

    void draw(sf::RenderWindow &window, const AnimationConfig& config, float t) override {
        sf::CircleShape circle(config.node_size);
        circle.setFillColor(sf::Color::Black);
        circle.setOutlineColor(sf::Color::White);
        circle.setOutlineThickness(config.line_thickness);

        window.draw(circle, config.graphic_transform * sf::Transform().translate(data_.pos_));
    }
};