#pragma once

#include "../Vertex.h"
#include "../SquareIndicies.h"

#include <vector>
#include <iostream>

#define GRAVITY 9.809f

class Particle {
private:

	float m_VelX;
	float m_VelY;

	float m_Radius;
	float m_Alpha;

	float m_MassInverse;
	float m_GravityTimesMass;

public:
	float m_X;
	float m_Y;
	float m_Mass;

	Particle(float startX, float startY, float velX, float velY, float mass, float radius)
		: m_X(startX), m_Y(startY), m_VelX(velX), m_VelY(velY), m_Mass(mass), m_Radius(radius),
			m_Alpha(1.0f) {

		m_MassInverse = 1.0f / m_Mass;
		m_GravityTimesMass = GRAVITY * m_Mass;

	}

	void calculateForceWithParticle(const Particle& otherParticle);
	void calculateForceWithPosition(const float& _x, const float& _y, const float& _mass);

	inline float GetXVel() { return m_VelX; }

	void UpdatePosition();
	void createParticleVerticies(std::vector<Vertex>& verticies, std::vector<SquareIndicies>& squareIndicies);
	void createParticleVerticies(std::vector<Vertex>& verticies);
	friend std::ostream& operator<<(std::ostream& os, Particle& p);
	friend bool operator==(const Particle& p1, const Particle& p2);
};