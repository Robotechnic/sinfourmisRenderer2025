#include "argparse/argparse.hpp"
#include "nlohmann/json.hpp"
#include "dynamic_shape.hpp"
#include "data_type.hpp"
#include "node.cpp"
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Graphics/Transform.hpp>
#include <fstream>
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

    WorldData data(logs["data"]["1"]);

    std::vector<std::unique_ptr<DynamicShape>> elements;

    int width = 800;
    int height = 600;


    AnimationConfig config = {10, 2, sf::Transform().translate(width/2, height/2), sf::Transform().scale(4, 4)};

    for (auto &node : data.nodes) {
        node.second.pos_ = config.physic_transform.transformPoint(node.second.pos_);
        elements.push_back(std::make_unique<Node>(node.second));
    }

    sf::RenderWindow window(sf::VideoMode(width, height), "Sinfourmis Renderer");


    while (window.isOpen()) {
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