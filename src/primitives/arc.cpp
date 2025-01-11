#include "arc.hpp"
#include <iostream>

Arc::Arc(float radius, float startAngle, float endAngle, int pointCount) : radius(radius), startAngle(startAngle), endAngle(endAngle), pointCount(pointCount) {}

void Arc::setRadius(float radius) {
	this->radius = radius;
	needsUpdate = true;
}

void Arc::setStartAngle(float startAngle) {
	this->startAngle = startAngle;
	needsUpdate = true;
}

void Arc::setEndAngle(float endAngle) {
	this->endAngle = endAngle;
	needsUpdate = true;
}

void Arc::setThickness(float thickness) {
	this->thickness = thickness;
	needsUpdate = true;
}

void Arc::setFillColor(sf::Color color) {
	fillColor = color;
	needsUpdate = true;
}

float Arc::getRadius() const {
	return radius;
}

float Arc::getStartAngle() const {
	return startAngle;
}

float Arc::getEndAngle() const {
	return endAngle;
}

float Arc::getThickness() const {
	return thickness;
}

void Arc::draw(sf::RenderTarget &target, sf::RenderStates states) const {
	states.transform *= getTransform();
	target.draw(vertices, states);
}

void Arc::updateArc() {
	if (!needsUpdate) {
		return;
	}
	float delta = (endAngle - startAngle) / (float)pointCount;
	vertices.setPrimitiveType(sf::PrimitiveType::TrianglesStrip);
	vertices.resize(pointCount * 2);
	float radiusOuter = radius + thickness;
	float x,y;
    for (int i = 0; i < pointCount * 2; i += 2) {
		float angle = startAngle + (delta / 2) * (float)i;
		if (i == pointCount * 2 - 2) {
			angle = endAngle;
		}
		x = radiusOuter * std::cos(angle);
		y = radiusOuter * std::sin(angle);
		vertices[i].position = sf::Vector2f(x, y);
		vertices[i].color = fillColor;
		x = radius * std::cos(angle);
		y = radius * std::sin(angle);
		vertices[i + 1].position = sf::Vector2f(x, y);
		vertices[i + 1].color = fillColor;
    }
}
