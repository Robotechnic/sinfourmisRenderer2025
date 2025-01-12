#include "data_type.hpp"
#include "dynamic_shape.hpp"
#include "primitives/arc.hpp"
#include <iostream>
#include <math.h>

class Node : public DynamicShape {
  public:
    const WorldData &world_;
    const NodeData &data_;
    std::vector<float> angle_before;
    std::vector<float> angle_after;
    std::vector<TeamData> teams_vector;

    explicit Node(const WorldData &world, const NodeData &data)
        : DynamicShape(5), world_(world), data_(data) {

        std::map<int, int> before_map;
        std::map<int, int> after_map;

        int before_sum = 0;
        int after_sum = 0;

        for (const auto &[_, team] : world_.teams) {
            before_map[team.id] = 0;
            after_map[team.id] = 0;
            teams_vector.push_back(team);
        }

        std::ranges::sort(teams_vector, [](auto &a, auto &b) { return a.id - b.id > 0; });

        for (const auto &[_, group] : data_.ants) {
            before_sum += group.qt;
            before_map[group.team] += group.qt;
        }

        for (const auto &[_, group] :
             data_.anim.value_or<NodeDataAnimation>({0, std::make_optional(data_.ants)})
                 .ants.value_or(data_.ants)) {
            after_sum += group.qt;
            after_map[group.team] += group.qt;
        }

        const auto n = (int) world_.teams.size();
        angle_after.resize(n);
        angle_before.resize(n);

        int acc_before = 0;
        int acc_after = 0;

        for (int i = 0; i < n; i++) {
            angle_before[i + 1] = ((float)acc_before) / (float) std::max(before_sum, 1);
            angle_after[i + 1] = ((float)acc_after) / (float) std::max(after_sum, 1);

            acc_before += before_map[teams_vector.at(i).id];
            acc_after += after_map[teams_vector.at(i).id];
        }

        angle_before[n + 1] = before_sum > 0 ? 1 : 0;
        angle_after[n + 1] = after_sum > 0 ? 1 : 0;
    }

    void draw(sf::RenderWindow &window, const AnimationConfig &config, float t) override {
        sf::CircleShape circle(config.node_size/2);
        circle.setFillColor(sf::Color::Black);
        circle.setOutlineColor(sf::Color::White);
        circle.setOutlineThickness(config.line_thickness);

        const auto n = (int) world_.teams.size();
        for (int i = 0; i < n; i++) {
            float angle_i = std::lerp(angle_before[i+1], angle_after[i+1], t) * 2 * M_PI;
            float angle_i_p_1 = std::lerp(angle_before[i + 2], angle_after[i + 2], t) * 2 * M_PI;

            Arc arc(config.node_size, angle_i, angle_i_p_1);
            arc.setFillColor(
                sf::Color((uint32_t) std::stoul(teams_vector.at(i).color.substr(1, 6)+"ff", nullptr, 16)));
            arc.setThickness(10);
            arc.updateArc();
            window.draw(arc, config.graphic_transform * sf::Transform().translate(data_.pos_));
        }

        window.draw(circle, config.graphic_transform * sf::Transform().translate(data_.pos_).translate(-config.node_size/2.f, -config.node_size/2.f));
    }
};