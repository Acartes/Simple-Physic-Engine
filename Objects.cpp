// 
// 
// 

#include "Objects.h"

Vector2::Vector2(int newX, int newY) {
	x = newX;
	y = newY;
}

Vector2& Vector2::operator+=(const Vector2& add) {
	this->x += add.x;
	this->y += add.y;
	return *this;
}

bool& Vector2::operator!=(const Vector2& add) {
	bool result;
	if (this->x == add.x || this->y == add.y) {
		result = false;
	}
	else
	{
		result = true;
	}
	return result;
}

Vector2::Vector2() {

}
Vector2::~Vector2() {

}


void Object::Draw() {
	gb.display.fillRect(position.x, position.y, rectData.x, rectData.y);
}
void Object::SetPhysics(bool state) {
	if (state == true) {
		isAffectedByPhysics = true;
		direction = Vector2(0, 1);
	}
	if (state == false) {
		isAffectedByPhysics = false;
		direction = Vector2(0, 0);
	}
}

Object::Object()
{

}

Object::~Object()
{
}

Object::Object(Vector2 newPosition, Vector2 newRectData, Color newColor, FrictionType newFriction, bool activatePhysics) {
	position = newPosition;
	rectData = newRectData;
	color = newColor;
	friction = newFriction;
	SetPhysics(activatePhysics);
}
