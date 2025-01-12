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
#include <filesystem>
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

    int width = 1500;
    int height = 900;

    AnimationConfig config = {20, 30, 15, 5, sf::Transform().translate(width/2, height/2), sf::Transform().scale(12, 12)};

	// int i = 0;
	// std::string name = "output";
    // while (std::filesystem::exists("output" + std::to_string(i))) {
	// 	i++;
	// }
	// name += std::to_string(i);
	// std::filesystem::create_directory(name);
	// sf::Texture screenRender;

    sf::ContextSettings settings;
    settings.antialiasingLevel = 8.0;

    sf::RenderWindow window(sf::VideoMode(width, height), "Sinfourmis Renderer", sf::Style::Close, settings);
	window.setFramerateLimit(config.frame_rate);

    unsigned int frame_counter = (unsigned int)config.frame_duration;
    unsigned int world_counter = 1;

	WorldData data(logs["data"][std::to_string(world_counter)]);
	sf::Vector2f min;
	sf::Vector2f max;

	for (auto &node : data.nodes) {
		auto pos = config.physic_transform.transformPoint(node.second.pos_);
		min.x = std::min(min.x, pos.x);
		min.y = std::min(min.y, pos.y);
		max.x = std::max(max.x, pos.x);
		max.y = std::max(max.y, pos.y);
	}
	// center the view
	config.graphic_transform.translate(- (min.x + max.x) / 2, - (min.y + max.y) / 2);

    while (window.isOpen()) {
        if (frame_counter >= (unsigned int)config.frame_duration) {
            frame_counter = 0;
			std::cout << "Frame " << world_counter << std::endl;
			if (world_counter == logs["data"].size()) {
				window.close();
				break;
			}
			elements.clear();
			data = WorldData(logs["data"][std::to_string(world_counter)]);
            for (auto &node : data.nodes) {
                node.second.pos_ = config.physic_transform.transformPoint(node.second.pos_);
            }

            for (auto &edge : data.edges) {
                elements.push_back(std::make_unique<Edge>(data, edge));
            }
            for (auto &node : data.nodes) {
                elements.push_back(std::make_unique<Node>(data, node.second));
            }
			world_counter++;
        }
		frame_counter++;

        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        window.clear(sf::Color(70, 70, 70));

        for (size_t i = 0; i < elements.size(); i++)
        {
            elements.at(i)->draw(window, config, ((float) frame_counter) / (float) config.frame_duration );
        }
        

        window.display();
		// screenRender.create(window.getSize().x, window.getSize().y);
		// screenRender.update(window);
		// screenRender.copyToImage().saveToFile(name + "/frame" + std::to_string(world_counter) + std::to_string(frame_counter) + ".png");
    }

	// std::stringstream command;
	// command << "ffmpeg -framerate " << config.frame_rate << " -i " << name << "/frame%d.png -c:v libx264 -profile:v high -crf 20 -pix_fmt yuv420p " << name << ".mp4";
	// system(command.str().c_str());

    return 0;
}