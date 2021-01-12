#include "Diagram.h"

Diagram::Diagram(const sf::Vector2f& in_size, const sf::Vector2f& in_position)
{
	position = in_position;
	size = in_size;

	xAxis = position.x + size.x / 2;
	yAxis = position.y + size.y / 2;

	xLine[0].position = sf::Vector2f(position.x, yAxis);
	xLine[1].position = sf::Vector2f(position.x + size.x, yAxis);

	xTriangle[0].position = { position.x + size.x, yAxis - TRI_SIZE };
	xTriangle[1].position = { position.x + size.x, yAxis + TRI_SIZE };
	xTriangle[2].position = { position.x + size.x + TRI_SIZE, yAxis };

	yTriangle[0].position = { xAxis - TRI_SIZE, position.y};
	yTriangle[1].position = { xAxis + TRI_SIZE, position.y };
	yTriangle[2].position = { xAxis, position.y - TRI_SIZE };

	yLine[0].position = sf::Vector2f(xAxis, position.y);
	yLine[1].position = sf::Vector2f(xAxis, position.y + size.y);

	xLine[0].color = absOrdColor;
	xLine[1].color = absOrdColor;
	yLine[0].color = absOrdColor;
	yLine[1].color = absOrdColor;

	point1.setFillColor(pointColor);
	point1.setOrigin({ pointRad, pointRad });
	point2.setFillColor(pointColor);
	point2.setOrigin({ pointRad, pointRad });

	elipse.setFillColor(sf::Color::Transparent);
	elipse.setPosition({ xAxis, yAxis });
	elipse.setOutlineThickness(elipseThickness);
	elipse.setOutlineColor(elipseColor);
}

// https://www.mathwarehouse.com/ellipse/focus-of-ellipse.php
void Diagram::refresh(float xE, float yE, float size, float m, bool isReversed)
{
	elipseRadius = size;
	elipseSize.x = size * xE;
	elipseSize.y = size * yE;

	// if anything changes
	if (elipseRadius != elipse.getRadius() || elipse.getScale() != sf::Vector2f{ xE, yE } || lastM != m || lastIsReversed != isReversed) {
		animationPlays = true;

		// tmp.setPosition(xAxis - elipseSize.x * cos(i), yAxis - elipseSize.y * sin(i));
		elipse.setRadius(elipseRadius);
		elipse.setOrigin({ elipseRadius, elipseRadius });
		elipse.setScale({ xE, yE });

		float c = sqrt(abs(pow(elipseSize.y, 2) - pow(elipseSize.x, 2)));

		point1.setPosition(xAxis + c, yAxis);
		point2.setPosition(xAxis - c, yAxis);

		float containVal =
			(powf((point1.getPosition().x - xAxis), 2) / powf(elipseSize.x, 2)) +
			(powf((point1.getPosition().y - yAxis), 2) / powf(elipseSize.y, 2));

		lightLines.clear();

		lightLines.push_back({});
		lightLines.back().p1.position = point1.getPosition();
		lightLines.back().p2.position = lightLines.back().p1.position;
		lightLines.back().leftFocus = false;

		lightLines.back().s = { -1, m };
		if (lightLines.back().s.y < 0) {
			lightLines.back().s.x = 1;
			lightLines.back().s.y = -lightLines.back().s.y;
		}
		if (isReversed) {
			lightLines.back().s.x = -lightLines.back().s.x;
			lightLines.back().s.y = -lightLines.back().s.y;
		}

		lightLines.back().p1.color = lightColor;
		lightLines.back().p2.color = lightColor;
	}
	lastM = m;
	lastIsReversed = isReversed;
}

void Diagram::animateLine(float pxPerCheck, float pxPerDraw)
{
	animationSpeed = pxPerDraw / pxPerCheck;

	for (int i = 0; i < animationSpeed; i++) {
		if (animationPlays) {
			lightLines.back().size += pxPerCheck;
		}

		sf::Vector2f nextP2Pos = {
			lightLines.back().p1.position.x + lightLines.back().size * lightLines.back().s.x,
			lightLines.back().p1.position.y - lightLines.back().size * lightLines.back().s.y };

		// checks if endpoint is in elipse
		float containVal =
			(powf((nextP2Pos.x - xAxis), 2) / powf(elipseSize.x, 2)) +
			(powf((nextP2Pos.y - yAxis), 2) / powf(elipseSize.y, 2));

		if (containVal > 1 && lightLines.size() < MAX_LIGHT_BEAMS) {

			// calculate m
			Light tmp;

			tmp.p1.position = lightLines.back().p2.position;
			tmp.leftFocus = !lightLines.back().leftFocus;
			if (tmp.leftFocus) {
				tmp.p2.position = point2.getPosition();
			}
			else {
				tmp.p2.position = point1.getPosition();
			}
			tmp.s.x = -1;
			tmp.s.y = (tmp.p1.position.y - tmp.p2.position.y) /
				(tmp.p1.position.x - tmp.p2.position.x); // m = (y - y1)/(x - x1)
			if (tmp.s.y < 0) {
				tmp.s.x = 1;
				tmp.s.y = -tmp.s.y;
			}
			if (tmp.p1.position.y < tmp.p2.position.y) {
				tmp.s.x = -tmp.s.x;
				tmp.s.y = -tmp.s.y;
			}

			nextP2Pos = {
				tmp.p1.position.x + tmp.size * tmp.s.x,
				tmp.p1.position.y - tmp.size * tmp.s.y };

			sf::Uint8 nB = lightColor.b - (50 * lightLines.size());
			tmp.p1.color = { lightColor.r, lightColor.g, nB };
			tmp.p2.color = { lightColor.r, lightColor.g, nB };

			lightLines.push_back(tmp);
		}

		if (lightLines.size() >= MAX_LIGHT_BEAMS) {
			animationPlays = false;
		}

		lightLines.back().p2.position = nextP2Pos;
	}
}

void Diagram::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(elipse);

	target.draw(xLine, 2, sf::Lines);
	target.draw(yLine, 2, sf::Lines);
	target.draw(point1);
	target.draw(point2);

	target.draw(xTriangle, 3, sf::Triangles);
	target.draw(yTriangle, 3, sf::Triangles);

	sf::Vertex tmp[2];
	for (auto& it : lightLines) {
		tmp[0] = it.p1;
		tmp[1] = it.p2;
		target.draw(tmp, 2, sf::Lines);
	}
}
