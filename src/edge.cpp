#include "data_type.hpp"
#include "dynamic_shape.hpp"

#include <iostream>

class Edge : public DynamicShape{
    public:
        const WorldData& world_;
        const EdgeData& data_;
		sf::Vector2f from;
		sf::Vector2f to;
		float length;

    explicit Edge(const WorldData& world, const EdgeData& data ) : DynamicShape(5), world_(world), data_(data) {
		from = world.nodes.at(data.id_1).pos_;
		to = world.nodes.at(data.id_2).pos_;
        length = sqrt(pow(from.x - to.x, 2) + pow(from.y - to.y, 2));
    }

    void draw(sf::RenderWindow &window, const AnimationConfig &config, float t) override {
		sf::RectangleShape line(sf::Vector2f(length, config.line_thickness));
		float lerped = std::lerp(0, 255, data_.life_ratio);
		line.setFillColor(sf::Color(lerped, lerped, lerped));
		float angle = atan2(to.y - from.y, to.x - from.x);
		line.rotate(angle  * 180 / M_PI);
		
		window.draw(line, config.graphic_transform*sf::Transform().translate(from)
		.translate(std::sin(angle) * config.line_thickness/2, std::cos(angle) * config.line_thickness / 2));

		for (auto &group: data_.groups) {
			float size = (config.node_size * 0.5 * std::exp(-1 / group.group_data.qt));
			sf::CircleShape group_repr(size);
			float progress = std::lerp(group.progress, group.anim_data->progress, t);
			float transparency = (t <= 0.05 || t >= 0.95) ? std::exp(-4 * std::max(t, 1 - t)) : 1;
			group_repr.setFillColor(sf::Color((uint32_t) std::stoul(world_.teams.at(group.group_data.team).color.substr(1, 6)+"ff", nullptr, 16)));

			window.draw(group_repr, config.graphic_transform * sf::Transform().translate(sf::Vector2f(
				std::lerp(from.x , to.x, progress) - size/2,
				std::lerp(from.y, to.y, progress) - size/2
			)));
		}
	};
};
