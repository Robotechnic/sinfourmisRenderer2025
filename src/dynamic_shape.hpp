#pragma once
#include <SFML/Graphics.hpp>
#include <data_type.hpp>

class DynamicShape {

  public:
    int zIndex_;

    explicit DynamicShape(int zIndex): zIndex_(zIndex) {

    }

    virtual void draw(sf::RenderWindow &window, const AnimationConfig& config, float t) = 0;

    virtual ~DynamicShape() = default;
};