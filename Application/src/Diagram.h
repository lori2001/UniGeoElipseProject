#pragma once
#include "NGin.h"

#define PI 3.14159265

struct Light {
	sf::Vertex p1, p2;
	sf::Vector2f s; // slope
	float size = 0;

	bool leftFocus = false; // is part of left or right focus
};

class Diagram : public sf::Drawable {
public:
	Diagram(const sf::Vector2f& in_size, const sf::Vector2f& in_position);

	void refresh(float xE, float yE, float dist, float m, bool isReversed);
	void animateLine(float pxPerCheck, float pxPerDraw);

	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
private:
	const sf::Color absOrdColor = { 255, 255, 255 };
	const sf::Color pointColor = { 255, 0, 0 };
	const sf::Color elipseColor = {0, 255, 0};
	const float pointRad = 6.0F;

	// position and size of diagram
	sf::Vector2f size;
	sf::Vector2f position;
	
	sf::Vertex xLine[2];
	sf::Vertex yLine[2];

	sf::CircleShape point1{ pointRad };
	sf::CircleShape point2{ pointRad };

	const float TRI_SIZE = 8.0F;
	sf::Vertex xTriangle[3];
	sf::Vertex yTriangle[3];

	sf::CircleShape elipse;
	const float elipseThickness = 2.9F;
	sf::Vector2f elipseSize;
	float elipseRadius = 5;

	// line, slope
	sf::Color lightColor = {200, 0, 0};
	std::vector<Light> lightLines;
	const float lightLineY = 900.0F;
	float lastM = 0;
	bool lastIsReversed = false;

	const float MAX_LIGHT_BEAMS = 500;

	bool animationPlays = false;
	float animationSpeed = 1.0F;
	
	// O(0, 0)
	float xAxis = 0;
	float yAxis = 0;

	float distSq(sf::Vector2f a, sf::Vector2f b) {
		return powf(abs(a.x - b.x), 2) + powf(abs(a.y - b.y), 2);
	}

};
