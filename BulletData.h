// BulletData.h

#ifndef _BULLETDATA_h
#define _BULLETDATA_h

#include "Objects.h"

class BulletData
{
public:
	int index;
	int type;
	Vector2 position;
	int frameCount;
	BulletData();
	BulletData(int index, int type, Vector2 position, int frameCount);
};

#endif

