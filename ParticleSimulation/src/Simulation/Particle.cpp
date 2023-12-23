#include <iostream>

#include "Particle.h"
#include "../Vertex.h"
#include "../SquareIndicies.h"

#include <iostream>

#define TIMESTEP 0.00001f
#define GRAVITY 9.809f
#define MAX_SPEED 20.0f


void Particle::calculateForceWithParticle(const Particle& otherParticle) {
	const float x_diff = otherParticle.m_X - m_X;
	const float y_diff = otherParticle.m_Y - m_Y;

	const float distanceSquared = x_diff * x_diff + y_diff * y_diff;

	const float F = GRAVITY * m_Mass * otherParticle.m_Mass / distanceSquared;
	const float x = F * x_diff;
	const float y = F * y_diff;

	m_VelX += x / m_Mass * TIMESTEP;
	m_VelY += y / m_Mass * TIMESTEP;
}

void Particle::calculateForceWithPosition(const float& _x, const float& _y, const float& _mass) {
	const float x_diff = _x - m_X;
	const float y_diff = _y - m_Y;

	const float distanceSquared = x_diff * x_diff + y_diff * y_diff;

	const float F = GRAVITY * m_Mass * _mass / distanceSquared;
	const float x = F * x_diff;
	const float y = F * y_diff;

	m_VelX += x / m_Mass * TIMESTEP;
	m_VelY += y / m_Mass * TIMESTEP;
}

void Particle::UpdatePosition() {
	if (m_X > 1) {
		m_X = .995f;
		m_VelX *= -1;
	}
	else if (m_X < -1) {
		m_X = -.995f;
		m_VelX *= -1;
	}
	else if (m_Y > 1) {
		m_Y = .995f;
		m_VelY *= -1;
	}
	else if (m_Y < -1) {
		m_Y = -.995f;
		m_VelY *= -1;
	}

	m_X += m_VelX * TIMESTEP;
	m_Y += m_VelY * TIMESTEP;

	float vel = sqrt(abs(m_VelX) * abs(m_VelX) * TIMESTEP + abs(m_VelY) * abs(m_VelY) * TIMESTEP);
	//std::cout << "VelX: " << m_VelX << ", VelY: " << m_VelY << ", Vel: " << vel << std::endl;

	vel /= MAX_SPEED;

	//std::cout << "speed of particle: " << vel << std::endl;
	vel = vel < 1 ? vel : 1;

	m_A = vel;

}

void Particle::createParticleVerticies(std::vector<Vertex>& verticies, std::vector<SquareIndicies>& squareIndicies) {
    std::vector<Vertex> newVerticies = { {m_X - m_Radius, m_Y - m_Radius, m_R, m_G, m_B, m_A},
                                         {m_X + m_Radius, m_Y - m_Radius, m_R, m_G, m_B, m_A},
                                         {m_X + m_Radius, m_Y + m_Radius, m_R, m_G, m_B, m_A},
                                         {m_X - m_Radius, m_Y + m_Radius, m_R, m_G, m_B, m_A} };

    verticies.insert(verticies.end(), newVerticies.begin(), newVerticies.end());

    unsigned int i = squareIndicies.size() * 4;
    squareIndicies.push_back(SquareIndicies({ 0 + i, 1 + i, 2 + i, 2 + i, 3 + i, 0 + i }));
}
void Particle::createParticleVerticies(std::vector<Vertex>& verticies) {
    std::vector<Vertex> newVerticies = { {m_X - m_Radius, m_Y - m_Radius, m_R, m_G, m_B, m_A},
                                         {m_X + m_Radius, m_Y - m_Radius, m_R, m_G, m_B, m_A},
                                         {m_X + m_Radius, m_Y + m_Radius, m_R, m_G, m_B, m_A},
                                         {m_X - m_Radius, m_Y + m_Radius, m_R, m_G, m_B, m_A} };

    verticies.insert(verticies.end(), newVerticies.begin(), newVerticies.end());
}

std::ostream& operator<<(std::ostream& os, Particle& p) {
	os << " X " << p.m_X << ", Y " << p.m_Y;
	return os;
}

bool operator==(const Particle& p1, const Particle& p2) {
	if (p1.m_X == p2.m_X && p1.m_Y == p2.m_Y) 
		return true;
	return false;
}