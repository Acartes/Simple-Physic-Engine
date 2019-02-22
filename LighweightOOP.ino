 #include "ObjectList.h"
#include "LinkedList.h"
#include "BulletData.h"
#include "Objects.h"
#include "LinkedList.h"

// OBJECTS PARAMETERS
static const int arraySize = 8;
objectList objList;
Object* tempObj;

// CHARACTER PARAMETERS
static const int characterIndex = 0;
static const int characterVelocityX = 1;
static const int characterVelocityY = 1;
static const int controlledIndex = 0;

// PHYSICS PARAMETERS
static const int gravity = 1;
static const int max_velocity = 3;
static const int gravityRefreshFrameRate = 5;
static const int frictionX = 0;
static const int frictionY = 0;
int gravityFrameRateCount;

// BULLET PARAMETERS
list bulletList;
BulletData* tempBullet;
int frameCount;
int bulletIndex;
bool canReadBulletIndex = true;

// The setup() function runs once each time the micro-controller starts
void setup()
{
	bulletList.createnode(BulletData(0, 1, Vector2(gb.display.width() / 2, 10), 50));
	bulletList.createnode(BulletData(0, 1, Vector2(gb.display.width() / 4, 10), 100));
	bulletList.createnode(BulletData(0, 1, Vector2(gb.display.width() - 10, 10), 120));
	bulletList.createnode(BulletData(0, 1, Vector2(gb.display.width() / 2 + 10, 10), 120));

  objList.createnode(Object(Vector2(gb.display.width() / 2, 10), Vector2(2, 1), RED, None, true)); // CHARACTER
  objList.createnode(Object(Vector2(gb.display.width() - max_velocity - 1, 0), Vector2(max_velocity + 1, gb.display.height()), GREEN)); // RIGHT WALL
  objList.createnode(Object(Vector2(0, 0), Vector2(max_velocity + 1, gb.display.height()), GREEN, None)); // LEFT WALL
  objList.createnode(Object(Vector2(0, 0), Vector2(gb.display.width(), max_velocity + 1), GREEN)); // CEILLING
  objList.createnode(Object(Vector2(0, gb.display.height() - max_velocity - 1), Vector2(gb.display.width(), max_velocity + 1), GREEN, Ground)); // FLOOR
	gb.begin();
}

// Add the main program code into the continuous loop() function
void loop()
{
  while (!gb.update());
  // gestion du chronomètre (commandes et incrémentation)
  gb.display.clear();
  CreateBullets();
  Control(characterIndex);
  Gravity();
  Collision();
  ApplyDirection();
  Graph();
  frameCount++;
}

void CreateBullets() {
	if (!canReadBulletIndex || bulletList.get_position(bulletIndex) == NULL)
		return;

	tempBullet = &bulletList.get_position(bulletIndex)->data;
	
	if (tempBullet->frameCount <= frameCount) {
		objList.createnode(Object(Vector2(tempBullet->position.x, tempBullet->position.y), Vector2(2, 2), BLUE, None, true));
		++bulletIndex;
		CreateBullets();// recursive check if there are more than one bullet spawning at the same time
	}
}

void Control(int controlledIndex) {
	// Contr�les de la raquette1
	tempObj = &objList.get_position(controlledIndex)->data;

	if (gb.buttons.repeat(BUTTON_RIGHT, 0)) {
		tempObj->direction += Vector2(characterVelocityX, 0);
		if (tempObj->stuck)
			tempObj->stuck = false;
	}
	if (gb.buttons.repeat(BUTTON_LEFT, 0)) {
		tempObj->direction += Vector2(-characterVelocityX, 0);
		if (tempObj->stuck)
			tempObj->stuck = false;
	}
	if (gb.buttons.repeat(BUTTON_UP, 0)) {
		tempObj->direction += Vector2(0, -characterVelocityY);
		if (tempObj->stuck)
			tempObj->stuck = false;
	}
	if (gb.buttons.repeat(BUTTON_DOWN, 0)) {
		tempObj->direction += Vector2(0, characterVelocityY);
		if (tempObj->stuck)
			tempObj->stuck = false;
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
	for (size_t i = 0; i < objList.size; i++)
	{
		tempObj = &objList.get_position(i)->data;
		if (tempObj->stuck)
			return;
		if (tempObj->isAffectedByPhysics && objList.get_position(i)->data.direction.y <= max_velocity) {
			tempObj->direction += Vector2(0, gravity);
		}
	}
}

void Collision() {
	for (size_t i = 0; i < objList.size; i++)
	{
		tempObj = &objList.get_position(i)->data;
		if (tempObj->isAffectedByPhysics) {
			CheckCollision(i);
		}
	}
}

void CheckCollision(int checkedIndex) {
	for (size_t i = 0; i < objList.size; i++)
	{
		if (checkedIndex != i) {
			// CHECK VERTICAL COLLISION WITH TEMP OBJ
			VerticalCollision(i);

			// CHECK HORIZONTAL COLLISION WITH TEMP OBJ
			VerticalHorizontalCollision(i);
		}
	}
}

void VerticalCollision(int nonMovingObjIndex) {
	while (tempObj->direction.y != 0) {
		//If there is a collision, then the y direction is reduced by one and the collision is tried again
		if (CheckMovingYCollision(nonMovingObjIndex))
		{
			SlowYDown();

			// Collision effect
			if (tempObj->direction.y == 0) {
				switch (objList.get_position(nonMovingObjIndex)->data.friction)
				{
				case Stick:
					StickEffect();
					break;
				case Ground:
					SlowXDown();
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

void VerticalHorizontalCollision(int nonMovingObjIndex) {
	while (tempObj->direction.x != 0) {
		// If there is a collision, then the x direction is reduced by one and the collision is tried again
		if (CheckMovingXYCollision(nonMovingObjIndex))
		{
			SlowXDown();

			// Collision effect
			switch (objList.get_position(nonMovingObjIndex)->data.friction)
			{
			case Stick:
				if (tempObj->direction.x == 0)
					StickEffect();
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

bool CheckMovingYCollision(int nonMovingObjIndex) {
	return(gb.collide.rectRect(tempObj->position.x, tempObj->position.y + tempObj->direction.y, tempObj->rectData.x, tempObj->rectData.y,
		objList.get_position(nonMovingObjIndex)->data.position.x, objList.get_position(nonMovingObjIndex)->data.position.y, objList.get_position(nonMovingObjIndex)->data.rectData.x, objList.get_position(nonMovingObjIndex)->data.rectData.y));
}

bool CheckMovingXYCollision(int nonMovingObjIndex) {
	return(gb.collide.rectRect(tempObj->position.x + tempObj->direction.x, tempObj->position.y + tempObj->direction.y, tempObj->rectData.x, tempObj->rectData.y,
		objList.get_position(nonMovingObjIndex)->data.position.x, objList.get_position(nonMovingObjIndex)->data.position.y, objList.get_position(nonMovingObjIndex)->data.rectData.x, objList.get_position(nonMovingObjIndex)->data.rectData.y));
}

void StickEffect() {
	tempObj->stuck = true;
}

void SlowXDown() {
	if (tempObj->direction.x > 0)
		tempObj->direction = Vector2(tempObj->direction.x - 1, tempObj->direction.y);
	if (tempObj->direction.x < 0)
		tempObj->direction = Vector2(tempObj->direction.x + 1, tempObj->direction.y);
}

void SlowYDown() {
	if (tempObj->direction.y > 0)
		tempObj->direction = Vector2(tempObj->direction.x, tempObj->direction.y - 1);
	if (tempObj->direction.y < 0)
		tempObj->direction = Vector2(tempObj->direction.x, tempObj->direction.y + 1);
}

void ApplyDirection() {
	for (size_t i = 0; i < objList.size; i++)
	{
		tempObj = &objList.get_position(i)->data;
		if (tempObj->stuck == true) {
			tempObj->direction = Vector2(0, 0);
		}

		ClampVelocity();
		ApplySpeed();
		ApplyFriction();
	}
}

void ClampVelocity() {
	if (tempObj->direction.x > max_velocity) {
		tempObj->direction = Vector2(max_velocity, tempObj->direction.y);
	}
	if (tempObj->direction.y > max_velocity) {
		tempObj->direction = Vector2(tempObj->direction.x, max_velocity);
	}
	if (tempObj->direction.x < -max_velocity) {
		tempObj->direction = Vector2(-max_velocity, tempObj->direction.y);
	}
	if (tempObj->direction.y < -max_velocity) {
		tempObj->direction = Vector2(tempObj->direction.x, -max_velocity);
	}
}

void ApplySpeed() {
	tempObj->position += Vector2(tempObj->direction.x, tempObj->direction.y);
}

void ApplyFriction() {
	if (tempObj->direction.x != 0) {
		if (tempObj->direction.x > 0) {
			tempObj->direction += Vector2(-frictionX, 0);
		}
		if (tempObj->direction.x < 0) {
			tempObj->direction += Vector2(frictionX, 0);
		}
	}
	if (tempObj->direction.y != 0) {
		if (tempObj->direction.y > 0) {
			tempObj->direction += Vector2(0, -frictionY);
		}
		if (tempObj->direction.y < 0) {
			tempObj->direction += Vector2(0, frictionY);
		}

	}
}

void Graph() {
	for (size_t i = 0; i < objList.size; i++)
	{
		tempObj = &objList.get_position(i)->data;
		gb.display.setColor(tempObj->color);
		tempObj->Draw();
	}
}

