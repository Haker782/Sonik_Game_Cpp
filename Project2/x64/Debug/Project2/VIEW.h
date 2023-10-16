#pragma once
#include<SFML/Graphics.hpp>
using namespace sf;
sf::View view;
void viewXY(float x, float y) {
	view.setCenter(x +100, y );
}