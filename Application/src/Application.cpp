#include "Application.h"

Application::Application()
{
	windowVideoMode = { 1366 , 768 };
	windowName = "Ez az elipszis fenyesebb mint a jovom";
	windowClearColor = sf::Color(30, 30, 30);

	widthSlider.setLevel(0.98F);
	widthSlider.setPosition({ 1400.0F, 80.0F });
	widthText.setPosition({ 1400.0F, 45.0F });
	widthText.setString(std::to_string(widthSlider.getLevel()));

	heightSlider.setLevel(0.9799F);
	heightSlider.setPosition({ 1400.0F, 175.0F });
	heightText.setPosition({ 1400.0F, 140.0F });
	heightText.setString(std::to_string(heightSlider.getLevel()));

	inclInText.setString(std::to_string(slope));
	inclInText.setPosition({ 1400.0F, 300.0F });
	inclText.setPosition({ 1400.0F, 260.0F });
	inclText.setString(INCL_STRING);
	
	reverseSlope.setPosition({ 1400.0F, 370.0F });
	reverseText.setPosition({1475.0F, 380.0F});
	reverseText.setString("Fordított szög");

	speedSlider.setPosition({ 1400.0F, 600.0F });
	speedSlider.setLevel(0.2F);
	speedText.setPosition({ 1400.0F, 560.0F });
	speedText.setString(SPD_STRING + std::to_string(speedSlider.getLevel()));

	diagram.refresh(widthSlider.getLevel(), heightSlider.getLevel(), elipseDist, slope, reverseSlope.isActive());
}

void Application::handleEvents()
{
	widthSlider.handleEvents(event_, ng::Cursor::getPosition());
	heightSlider.handleEvents(event_, ng::Cursor::getPosition());
	reverseSlope.handleEvents(event_, ng::Cursor::getPosition());

	widthText.setString(std::to_string(widthSlider.getLevel()));
	heightText.setString(std::to_string(heightSlider.getLevel()));

	speedSlider.handleEvents(event_, ng::Cursor::getPosition());
	animationSpeed = speedSlider.getLevel() * 5000;
	speedText.setString(SPD_STRING + std::to_string((int)animationSpeed));

	inclInText.handleEvents(event_, ng::Cursor::getPosition());
	if (inclInText.getIsActive() == false) {
		try {
			slope = std::stof(inclInText.getString().toAnsiString());
		}
		catch (...) {
			slope = 0.5F;
			inclInText.setString(std::to_string(slope));
			NG_LOG_ERROR("slope reset!");
		}
	}

	diagram.refresh(widthSlider.getLevel(), heightSlider.getLevel(), elipseDist, slope, reverseSlope.isActive());
}

void Application::update()
{
	diagram.animateLine(diagramChecks, animationSpeed * ng::Timer::getDeltaTime());
}

void Application::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(widthSlider);
	target.draw(heightSlider);
	target.draw(inclInText);
	target.draw(speedSlider);
	target.draw(reverseSlope);

	target.draw(widthText);
	target.draw(heightText);
	target.draw(speedText);
	target.draw(reverseText);
	target.draw(inclText);

	target.draw(diagram);
}
