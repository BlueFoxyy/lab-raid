// This file provides the format of rooms
//
// ---- ROOM DIMENSIONS ----
// Room Height:	100
// Room Width: 	100
// The designed rooms are restricted in the above ranges.
// X in [0, 100], Y in [0, 100]
// The height and width will then be scaled up to a random value in the specified range below (see section FORMAT)
// 
// ---- DATA TYPES ----
// The rooms consists of two types of objects:
// 1. walls (represented as lines)
// 2. nodes (where objects will spawn at)
// NOTE: walls on the outside should not be included, as those are reserved for the levelGenerator to add doorways to other rooms.
//
// ---- IMPLEMENTATION OF STRUCTURES ----
// A 'Line' has the following properties:
// 1. Vector2D startPoint;
// 2. Vector2D endPoint;
// 
// ---- FORMAT ----
// [Input Section 1]
// 1.	widthLowerBound(int)	widthUpperBound(int)
// 2.	heightLowerBound(int)	heightUpperBound(int)
// 3. 	wallCount(int)		nodeCount(int)
//
// [Input Section 2] (walls)
// i.	beginPointX(double)	beginPointY(double)	endPointX(double)	endPointY(double)	thickness(double)
//
// [Input Section 3] (nodes)
// i.	nodePositionX(double)	nodePositionY(double)	objectType(string) 
//
// NOTE: objectType examples:
// 	- item.ammo
//	- item.medkit
//	- entity.enemy.robot
//	- entity.enemy.guard
//	- obstacle.woodenBox
// 
// ---- EXAMPLE ----
400	600
400	600
2	0
20	20	80	20	1
20	80	80	80	1