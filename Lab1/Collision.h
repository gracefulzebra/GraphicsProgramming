#pragma once

#include "ObjectHandler.h"

struct Collision {

	Collision() {};
	~Collision() {};

	/// <summary>
	/// Compares Two Object Positions And Collider Sizes.
	/// </summary>
	/// <returns>
	/// Returns True If Objects Are Colliding
	/// </returns>
	bool CheckCollisions(glm::vec3& pos1, glm::vec3& pos2, float& rad1, float& rad2)
	{ 
		float distance = ((pos2.x - pos1.x) * (pos2.x - pos1.x) + (pos2.y - pos1.y) * (pos2.y - pos1.y) + (pos2.z - pos1.z) * (pos2.z - pos1.z));
		
		return (distance * distance) < (rad1 + rad2);
	}


};