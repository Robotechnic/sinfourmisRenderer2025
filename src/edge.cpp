#include "data_type.hpp"
#include "dynamic_shape.hpp"

#include <iostream>

class Edge : public DynamicShape {
  public:
    const WorldData &world_;
    const EdgeData &data_;
    sf::Vector2f from;
    sf::Vector2f to;
    float length;

    explicit Edge(const WorldData &world, const EdgeData &data)
        : DynamicShape(5), world_(world), data_(data) {
        from = world.nodes.at(data.id_1).pos_;
        to = world.nodes.at(data.id_2).pos_;
        length = sqrt(pow(from.x - to.x, 2) + pow(from.y - to.y, 2));
    }

    void draw(sf::RenderWindow &window, const AnimationConfig &config, float t) override {
        sf::RectangleShape line(sf::Vector2f(length, config.line_thickness));
        float lerped = std::lerp(0.0f, 255.0f, data_.life_ratio);
        float angle = atan2(to.y - from.y, to.x - from.x);

        line.setOrigin(sf::Vector2f(length / 2, config.line_thickness / 2));
        line.setFillColor(sf::Color(lerped, lerped, lerped));
        line.setRotation(angle * 180 / M_PI);

        line.setPosition((from+to)/2.f);

        // Draw the rectangle with the appropriate transformation
        window.draw(line, config.graphic_transform);
    };
};
