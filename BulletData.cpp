// 
// 
// 

#include "BulletData.h"

BulletData::BulletData()
{


}

BulletData::BulletData(int newIndex, int newType, Vector2 newPosition, int newFrameCount)
{
	index = newIndex;
	type = newType;
	position = newPosition;
	frameCount = newFrameCount;
}
