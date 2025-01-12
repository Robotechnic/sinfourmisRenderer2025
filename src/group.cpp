#include "data_type.hpp"
#include "dynamic_shape.hpp"

class Group : public DynamicShape {
  public:
    const WorldData &world_;
    const EdgeData &data_;
    const EdgeGroupData &group_;
    sf::Vector2f from;
    sf::Vector2f to;

    explicit Group(const WorldData &world, const EdgeData &data, const EdgeGroupData &group)
        : DynamicShape(10), world_(world), data_(data), group_(group) {

        from = world.nodes.at(data.id_1).pos_;
        to = world.nodes.at(data.id_2).pos_;
        }

    void draw(sf::RenderWindow &window, const AnimationConfig &config, float t) override {
        float size = (2 * config.node_size * 0.5 * std::exp(-1 / group_.group_data.qt));
        sf::CircleShape group_repr(size);
        float progress = std::lerp(group_.progress, group_.anim_data->progress, t);
        group_repr.setFillColor(sf::Color(
            world_.teams.at(group_.group_data.team).color_int));

        window.draw(group_repr, config.graphic_transform *
                                    sf::Transform().translate(
                                        sf::Vector2f(std::lerp(from.x, to.x, progress) - size,
                                                     std::lerp(from.y, to.y, progress) - size)));
    }
};