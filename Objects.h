// Objects.h

#ifndef _OBJECTS_h
#define _OBJECTS_h

#include <Gamebuino-Meta.h>

struct Vector2
{
	Vector2();
	~Vector2();
public:
	int x;
	int y;
	Vector2(int newX, int newY);
	Vector2& operator+=(const Vector2& add);
	bool& operator!=(const Vector2& add);
};


enum FrictionType
{
	Ice,
	Stick,
	Ground,
	None
};

class Object
{
public:
	Object();
	~Object();
	void Draw();
	void SetPhysics(bool state);
	Color color;
	Vector2 position;
	Vector2 rectData;
	Vector2 direction;
	bool isAffectedByPhysics = false;
	bool isActive;
	FrictionType friction;
	bool stuck;
};


#endif

