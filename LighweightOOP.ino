#include "Objects.h"

// Visual Micro is in vMicro>General>Tutorial Mode
// 
/*
    Name:       LighweightOOP.ino
    Created:  2/14/2019 11:48:52 AM
    Author:     ACARTES-PC\Acartes
*/

// Define User Types below here or use a .h file
//



// Define Function Prototypes that use User Types below here or use a .h file
//


// Define Functions below here or use other .ino or cpp files
//


// OBJECTS PARAMETERS
static const int arraySize = 8;
Object A;
Object B;
Object C;
Object Sticker;
Object Ceiling;
Object RightWall;
Object LeftWall;
Object Floor;

Object objList[arraySize];

// CHARACTER PARAMETERS
static const int characterIndex = 0;
static const int characterVelocityX = 1;
static const int characterVelocityY = 1;

// PHYSICS PARAMETERS
static const int gravity = 1;
static const int max_velocity = 3;
static const int gravityRefreshFrameRate = 5;
static const int frictionX = 0;
static const int frictionY = 0;

int gravityFrameRateCount;

// The setup() function runs once each time the micro-controller starts
void setup()
{
	A.position = Vector2(gb.display.width() / 2, 10);
	A.rectData = Vector2(2, 1);
	A.color = RED;
	A.SetPhysics(true);

	B.position = Vector2(gb.display.width() / 2, 15);
	B.rectData = Vector2(max_velocity + 10, max_velocity + 1);
	B.color = BLUE;
	B.friction = Ice;

	C.position = Vector2(gb.display.width() / 2 + 15, 30);
	C.rectData = Vector2(max_velocity + 20, max_velocity + 1);
	C.color = BLUE;
	C.friction = Ice;

	Sticker.position = Vector2(gb.display.width() - 10, 10);
	Sticker.rectData = Vector2(max_velocity + 15, max_velocity + 40);
	Sticker.color = ORANGE;
	Sticker.friction = Stick;

	RightWall.position = Vector2(gb.display.width() - max_velocity - 1, 0);
	RightWall.rectData = Vector2(max_velocity + 1, gb.display.height());
	RightWall.color = GREEN;

	LeftWall.position = Vector2(0, 0);
	LeftWall.rectData = Vector2(max_velocity + 1, gb.display.height());
	LeftWall.color = GREEN;

	Ceiling.position = Vector2(0, 0);
	Ceiling.rectData = Vector2(gb.display.width(), max_velocity + 1);
	Ceiling.color = GREEN;

	Floor.position = Vector2(0, gb.display.height() - max_velocity - 1);
	Floor.rectData = Vector2(gb.display.width(), max_velocity + 1);
	Floor.color = GREEN;
	Floor.friction = Ground;

	gb.begin();

	objList[0] = A;
	objList[1] = C;
	objList[2] = B;
	objList[3] = Sticker;
	objList[4] = RightWall;
	objList[5] = LeftWall;
	objList[6] = Ceiling;
	objList[7] = Floor;
}

// Add the main program code into the continuous loop() function
void loop()
{
  while (!gb.update());
  gb.display.clear();
  Control(characterIndex);
  Gravity();
  Collision(characterIndex);
  ApplyDirection();
  Graph();
}

void Control(int controlledIndex) {
	// Contrôles de la raquette1
	if (gb.buttons.repeat(BUTTON_RIGHT, 0)) {
		objList[controlledIndex].direction += Vector2(characterVelocityX, 0);
		if (objList[controlledIndex].stuck)
			objList[controlledIndex].stuck = false;
	}
	if (gb.buttons.repeat(BUTTON_LEFT, 0)) {
		objList[controlledIndex].direction += Vector2(-characterVelocityX, 0);
		if (objList[controlledIndex].stuck)
			objList[controlledIndex].stuck = false;
	}
	if (gb.buttons.repeat(BUTTON_UP, 0)) {
		objList[controlledIndex].direction += Vector2(0, -characterVelocityY);
		if (objList[controlledIndex].stuck)
			objList[controlledIndex].stuck = false;
	}
	if (gb.buttons.repeat(BUTTON_DOWN, 0)) {
		objList[controlledIndex].direction += Vector2(0, characterVelocityY);
		if (objList[controlledIndex].stuck)
			objList[controlledIndex].stuck = false;
	}
}

void Gravity() {
	if (gravityFrameRateCount < gravityRefreshFrameRate) {
		gravityFrameRateCount++;
		return;
	}
	else {
		gravityFrameRateCount = 0;
	}
	for (size_t i = 0; i < arraySize; i++)
	{
		if (objList[i].stuck)
			return;
		if (objList[i].isAffectedByPhysics && objList[i].direction.y <= max_velocity) {
			objList[i].direction += Vector2(0, gravity);
		}
	}
}

void Collision(int checkedIndex) {
	for (size_t i = 0; i < arraySize; i++)
	{
		if (checkedIndex != i) {
			gb.display.setColor(objList[i].color);

			// CHECK VERTICAL COLLISION
			VerticalCollision(checkedIndex, i);

			// CHECK HORIZONTAL COLLISION
			VerticalHorizontalCollision(checkedIndex, i);
		}
	}
}

void VerticalCollision(int movingObjIndex, int nonMovingObjIndex) {
	while (objList[movingObjIndex].direction.y != 0) {
		//If there is a collision, then the y direction is reduced by one and the collision is tried again
		if (CheckMovingYCollision(movingObjIndex, nonMovingObjIndex))
		{
			SlowYDown(movingObjIndex);

			// Collision effect
			if (objList[movingObjIndex].direction.y == 0) {
				switch (objList[nonMovingObjIndex].friction)
				{
				case Stick:
					StickEffect(movingObjIndex);
					break;
				case Ground:
					SlowXDown(movingObjIndex);
					// Ground case : reduce horizontal velocity by one
					break;
				}
			}
		}
		// If there a no more collision, then we go to the vertical check
		else
		{
			break;
		}
	}
}

void VerticalHorizontalCollision(int movingObjIndex, int nonMovingObjIndex) {
	while (objList[movingObjIndex].direction.x != 0) {
		// If there is a collision, then the x direction is reduced by one and the collision is tried again
		if (CheckMovingXYCollision(movingObjIndex, nonMovingObjIndex))
		{
			SlowXDown(movingObjIndex);

			// Collision effect
			switch (objList[nonMovingObjIndex].friction)
			{
			case Stick:
				if (objList[movingObjIndex].direction.x == 0)
					StickEffect(movingObjIndex);
				break;
			}
		}
		// If there a no more collision, then it's over
		else
		{
			return;
		}
	}
}

bool CheckMovingYCollision(int movingObjIndex, int nonMovingObjIndex) {
	return(gb.collide.rectRect(objList[movingObjIndex].position.x, objList[movingObjIndex].position.y + objList[movingObjIndex].direction.y, objList[movingObjIndex].rectData.x, objList[movingObjIndex].rectData.y,
		objList[nonMovingObjIndex].position.x, objList[nonMovingObjIndex].position.y, objList[nonMovingObjIndex].rectData.x, objList[nonMovingObjIndex].rectData.y));
}

bool CheckMovingXYCollision(int movingObjIndex, int nonMovingObjIndex) {
	return(gb.collide.rectRect(objList[movingObjIndex].position.x + objList[movingObjIndex].direction.x, objList[movingObjIndex].position.y + objList[movingObjIndex].direction.y, objList[movingObjIndex].rectData.x, objList[movingObjIndex].rectData.y,
		objList[nonMovingObjIndex].position.x, objList[nonMovingObjIndex].position.y, objList[nonMovingObjIndex].rectData.x, objList[nonMovingObjIndex].rectData.y));
}

void StickEffect(int index) {
	objList[index].stuck = true;
}

void SlowXDown(int index) {
	if (objList[index].direction.x > 0)
		objList[index].direction = Vector2(objList[index].direction.x - 1, objList[index].direction.y);
	if (objList[index].direction.x < 0)
		objList[index].direction = Vector2(objList[index].direction.x + 1, objList[index].direction.y);
}

void SlowYDown(int index) {
	if (objList[index].direction.y > 0)
		objList[index].direction = Vector2(objList[index].direction.x, objList[index].direction.y - 1);
	if (objList[index].direction.y < 0)
		objList[index].direction = Vector2(objList[index].direction.x, objList[index].direction.y + 1);
}

void ApplyDirection() {
	for (size_t i = 0; i < arraySize; i++)
	{
		if (objList[i].stuck == true) {
			objList[i].direction = Vector2(0, 0);
		}

		ClampVelocity(i);
		ApplySpeed(i);
		ApplyFriction(i);
	}
}

void ClampVelocity(int index) {
	if (objList[index].direction.x > max_velocity) {
		objList[index].direction = Vector2(max_velocity, objList[index].direction.y);
	}
	if (objList[index].direction.y > max_velocity) {
		objList[index].direction = Vector2(objList[index].direction.x, max_velocity);
	}
	if (objList[index].direction.x < -max_velocity) {
		objList[index].direction = Vector2(-max_velocity, objList[index].direction.y);
	}
	if (objList[index].direction.y < -max_velocity) {
		objList[index].direction = Vector2(objList[index].direction.x, -max_velocity);
	}
}

void ApplySpeed(int index) {
	objList[index].position += Vector2(objList[index].direction.x, objList[index].direction.y);
}

void ApplyFriction(int index) {
	if (objList[index].direction.x != 0) {
		if (objList[index].direction.x > 0) {
			objList[index].direction += Vector2(-frictionX, 0);
		}
		if (objList[index].direction.x < 0) {
			objList[index].direction += Vector2(frictionX, 0);
		}
	}
	if (objList[index].direction.y != 0) {
		if (objList[index].direction.y > 0) {
			objList[index].direction += Vector2(0, -frictionY);
		}
		if (objList[index].direction.y < 0) {
			objList[index].direction += Vector2(0, frictionY);
		}

	}
}

void Graph() {
	for (size_t i = 0; i < arraySize; i++)
	{
		gb.display.setColor(objList[i].color);
		objList[i].Draw();
	}
}

