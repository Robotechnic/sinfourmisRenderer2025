#include "data_type.hpp"
#include "dynamic_shape.hpp"

class Edge : public DynamicShape{
    public:
        const EdgeData& data_;
		const float thikness = 5;
		sf::Vector2f from;
		sf::Vector2f to;
		float length;

    explicit Edge(const EdgeData& data ) : DynamicShape(5), data_(data) {
        length = sqrt(pow(from.x - to.x, 2) + pow(from.y - to.y, 2));
    }

    void draw(sf::RenderWindow &window, const AnimationConfig &config, float t) override {
		sf::RectangleShape line(sf::Vector2f(length, thikness));
		float lerped = std::lerp(0, 255, data_.life_ratio);
		line.setFillColor(sf::Color(lerped, lerped, lerped));
		line.rotate(atan2(to.y - from.y, to.x - from.x) * 180 / M_PI);
		window.draw(line);

		for (auto &group: data_.groups) {
			sf::CircleShape group_repr(0.5 * std::exp(-1 / group.group_data.qt));
			float progress = std::lerp(group.progress, group.anim_data->progress, t);
			group_repr.setPosition(sf::Vector2f(
				std::lerp(from.x, to.x, progress),
				std::lerp(from.y, to.y, progress)
			));
			float transparency = (t <= 0.05 || t >= 0.95) ? std::exp(-4 * std::max(t, 1 - t)) : 1;
			// todo: add color

			window.draw(group_repr);
		}
	};
};
