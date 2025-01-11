#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <cmath>

class Arc : public sf::Drawable, public sf::Transformable {
public:
	Arc() = default;
	Arc(float radius, float startAngle, float endAngle, int pointCount = 30);

	void setRadius(float radius);
	void setStartAngle(float startAngle);
	void setEndAngle(float endAngle);
	void setThickness(float thickness);
	void setFillColor(sf::Color color);

	float getRadius() const;
	float getStartAngle() const;
	float getEndAngle() const;
	float getThickness() const;

	void updateArc();
	
private:
	virtual void draw(sf::RenderTarget &target, sf::RenderStates states) const override;

  private:
	float radius = 0;
	float startAngle = 0;
	float endAngle = 0;
	float thickness = 2;
	int pointCount = 0;
	bool needsUpdate = true;
	sf::VertexArray vertices;
	sf::Color fillColor = sf::Color::White;
};