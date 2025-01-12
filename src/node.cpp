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

		// convert 8 bit integer color to 32 bit color
		uint32_t color = 0;
		int r = (uint8_t) (data_.phero >> 5) * 255 / 7;
		int g = (uint8_t) ((data_.phero >> 2) & 0b111) * 255 / 7;
		int b = (uint8_t) (data_.phero & 0b11) * 255 / 3;
		sf::Color phero_color(r, g, b);

        circle.setFillColor(phero_color);
        circle.setOutlineColor(sf::Color::White);
        circle.setOutlineThickness(config.line_thickness);

        const auto n = (int) world_.teams.size();
        for (int i = 0; i < n; i++) {
            auto angle_i = std::lerp(angle_before[i+1], angle_after[i+1], t) * 2.f * M_PI;
            auto angle_i_p_1 = std::lerp(angle_before[i + 2], angle_after[i + 2], t) * 2.f * M_PI;

            Arc arc(config.node_size, (float) angle_i, (float) angle_i_p_1);
            arc.setFillColor(
                sf::Color((uint32_t) std::stoul(teams_vector.at(i).color.substr(1, 6)+"ff", nullptr, 16)));
            arc.setThickness(10);
            arc.updateArc();
            window.draw(arc, config.graphic_transform * sf::Transform().translate(data_.pos_));
        }

        window.draw(circle, config.graphic_transform * sf::Transform().translate(data_.pos_).translate(-config.node_size/2.f, -config.node_size/2.f));
		if (data_.type == "REINE") {
			const float ANGLE = .4f;
			Arc arc1(config.node_size * .5, ANGLE, M_PI / 2 - ANGLE);
            arc1.setFillColor(sf::Color(
                (uint32_t)std::stoul(world_.teams.at(data_.team).color.substr(1, 6) + "ff", nullptr, 16)));
            arc1.setThickness(config.node_size * .3f);
			arc1.updateArc();
			window.draw(arc1, config.graphic_transform * sf::Transform().translate(data_.pos_));
			arc1.setStartAngle(M_PI / 2 + ANGLE);
			arc1.setEndAngle(M_PI - ANGLE);
			arc1.updateArc();
			window.draw(arc1, config.graphic_transform * sf::Transform().translate(data_.pos_));
			arc1.setStartAngle(M_PI + ANGLE);
			arc1.setEndAngle(3 * M_PI / 2 - ANGLE);
			arc1.updateArc();
			window.draw(arc1, config.graphic_transform * sf::Transform().translate(data_.pos_));
			arc1.setStartAngle(3 * M_PI / 2 + ANGLE);
			arc1.setEndAngle(2 * M_PI - ANGLE);
			arc1.updateArc();
			window.draw(arc1, config.graphic_transform * sf::Transform().translate(data_.pos_));
		} else if(data_.type == "EAU") {
            circle.setFillColor(sf::Color::Transparent);
            circle.setOutlineColor(sf::Color::Cyan);
            circle.setOutlineThickness(config.line_thickness);
			window.draw(circle, config.graphic_transform * sf::Transform().translate(data_.pos_).translate(-config.node_size/2.f, -config.node_size/2.f));
        } else if(data_.type == "NOURRITURE") {

            auto ratio = (float) (std::lerp(data_.food, 
                data_.anim.value_or<NodeDataAnimation>({std::make_optional(data_.food), std::nullopt})
                    .food.value_or(data_.food), t) / (float) world_.max_food);
            float radius =  .75f * config.node_size * ratio * .5f;

            circle.setRadius(radius);
            circle.setOutlineColor(sf::Color::Transparent);
            circle.setFillColor(sf::Color::Red);
            window.draw(circle, config.graphic_transform * sf::Transform().translate(data_.pos_).translate(-radius, -radius));
        }
    }
};