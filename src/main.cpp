#include "argparse/argparse.hpp"
#include "nlohmann/json.hpp"
#include "dynamic_shape.hpp"
#include "data_type.hpp"
#include "node.cpp"
#include "edge.cpp"
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Graphics/Transform.hpp>
#include <fstream>
#include <cmath>
#include "primitives/arc.hpp"
#include <memory>

using json = nlohmann::json;


int main(int argc, char **argv) {
    argparse::ArgumentParser program("SinfourmisRenderer");
    program.add_description("The 2025 sinfourmis renderer");
    program.add_epilog("MIT Licence - Copyright (c) 2025 - sinfonie");

	program.add_argument("logs").help("The logs file to use").required().metavar("LOGS");

	try {
		program.parse_args(argc, argv);
	} catch (const std::runtime_error &err) {
		std::cout << err.what() << std::endl;
		std::cout << program;
		return 1;
	}

	std::fstream file(program.get<std::string>("logs"));
	if (!file.is_open()) {
		std::cerr << "Failed to open logs file" << std::endl;
		return 1;
	}

	json logs;
	file >> logs;

    std::vector<std::unique_ptr<DynamicShape>> elements;

    int width = 800;
    int height = 600;

    AnimationConfig config = {10, 30, 15, 2, sf::Transform().translate(width/2, height/2), sf::Transform().scale(4, 4)};

    

    sf::RenderWindow window(sf::VideoMode(width, height), "Sinfourmis Renderer");
	window.setFramerateLimit(config.frame_rate);

	unsigned int frame_counter = 0;
	unsigned int world_counter = 1;

	WorldData data(logs["data"][std::to_string(world_counter)]);

    while (window.isOpen()) {
		if (frame_counter == 0) {
			std::cout << "Frame " << world_counter << std::endl;
			if (world_counter == logs["data"].size()) {
				window.close();
				break;
			}
			elements.clear();
			data = WorldData(logs["data"][std::to_string(world_counter)]);
            for (auto &node : data.nodes) {
                node.second.pos_ = config.physic_transform.transformPoint(node.second.pos_);
                elements.push_back(std::make_unique<Node>(node.second));
            }

            for (auto &edge : data.edges) {
                elements.push_back(std::make_unique<Edge>(edge));
            }
			frame_counter = config.frame_duration;
			world_counter++;
        }
		frame_counter--;

        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        window.clear(sf::Color::Black);

        for (size_t i = 0; i < elements.size(); i++)
        {
            elements.at(i)->draw(window, config, 0);
        }
        

        window.display();
    }

    return 0;
}