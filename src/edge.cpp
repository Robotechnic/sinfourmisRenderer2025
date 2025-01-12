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
        for (auto &group : data_.groups) {
            float size = (config.node_size * 0.5 * std::exp(-1 / group.group_data.qt));
            sf::CircleShape group_repr(size);
            float progress = std::lerp(group.progress, group.anim_data->progress, t);
            // int transparency = ((t <= 0.05 || t >= 0.95) ? std::exp(-4 * std::max(t, 1 - t)) : 1)
            // * 255;
            group_repr.setFillColor(sf::Color(world_.teams.at(group.group_data.team)
                                                  .color_int)); // & (0xFFFFFF00 | transparency)));

            window.draw(group_repr,
                        config.graphic_transform * sf::Transform().translate(sf::Vector2f(
                                                       std::lerp(from.x, to.x, progress) - size,
                                                       std::lerp(from.y, to.y, progress) - size)));
        }
    };
};
