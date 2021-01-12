#pragma once
#define MAIN_LEVEL

#include "NGin.h"

#include "Diagram.h"

struct AppUISettings : public ng::UISettings {
	AppUISettings() {
		ng::UISettings::fontColor = { 0, 0 ,255 };
		ng::UISettings::baseColor = sf::Color(0, 255, 255);
		ng::UISettings::highlightColor = sf::Color(255, 255, 255);
	}
};

class Application : public ng::Main
{
public:
	Application();

	/* abstract overrides */
	virtual void handleEvents();
	virtual void update();
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
private:
	AppUISettings uiSettings;

	sf::Text widthText{ uiSettings.getTextStyle() };
	ng::Slider widthSlider{ uiSettings };
	
	sf::Text heightText{ uiSettings.getTextStyle() };
	ng::Slider heightSlider{ uiSettings };

	const std::string SPD_STRING = "Sebesség(px/s):";
	sf::Text speedText{ uiSettings.getTextStyle() };
	ng::Slider speedSlider{ uiSettings };
	float animationSpeed = 1;
	const float diagramChecks = 0.1F;

	ng::InputText inclInText{ uiSettings };
	const std::string INCL_STRING = "Szög tangense: ";
	sf::Text inclText{ uiSettings.getTextStyle() };
	float slope = 0.5F;

	ng::Switcher reverseSlope{ uiSettings };
	sf::Text reverseText{ uiSettings.getTextStyle() };

	Diagram diagram{ {1000.0F, 1000.0F}, { 100.0F, 40.0F } };
	const float elipseDist = 300.0F;
};

ng::Main* setMainLevel() {
	return new Application;
}