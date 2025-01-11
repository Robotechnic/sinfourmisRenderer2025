#include "argparse/argparse.hpp"
#include "nlohmann/json.hpp"
#include <SFML/Graphics.hpp>
#include <fstream>

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

    sf::RenderWindow window(sf::VideoMode(800, 600), "Sinfourmis Renderer");

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        window.clear(sf::Color::Black);
        window.display();
    }

    return 0;
}