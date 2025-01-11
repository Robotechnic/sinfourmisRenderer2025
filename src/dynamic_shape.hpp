#pragma once
#include <SFML/Graphics.hpp>

class DynamicShape {

  public:
    int zIndex_;

    explicit DynamicShape(int zIndex): zIndex_(zIndex) {

    }

    virtual void draw(sf::RenderWindow &window, float t) = 0;

    virtual ~DynamicShape() = default;
};