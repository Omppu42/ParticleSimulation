#include <iostream>

#include "Particle.h"
#include "../Vertex.h"
#include "../SquareIndicies.h"

#include <iostream>

#define TIMESTEP 0.00001f
#define MAX_SPEED 20.0f


void Particle::calculateForceWithParticle(const Particle& otherParticle) {
	const float x_diff = otherParticle.m_X - m_X;
	const float y_diff = otherParticle.m_Y - m_Y;

	const float distanceSquared = x_diff * x_diff + y_diff * y_diff;

	const float F = m_GravityTimesMass * otherParticle.m_Mass / distanceSquared;
	const float x = F * x_diff;
	const float y = F * y_diff;

	m_VelX += x * m_MassInverse * TIMESTEP;
	m_VelY += y * m_MassInverse * TIMESTEP;
}

void Particle::calculateForceWithPosition(const float& _x, const float& _y, const float& _mass) {
	const float x_diff = _x - m_X;
	const float y_diff = _y - m_Y;

	const float distanceSquared = x_diff * x_diff + y_diff * y_diff;

	const float F = m_GravityTimesMass * _mass / distanceSquared;
	const float x = F * x_diff;
	const float y = F * y_diff;

	m_VelX += x * m_MassInverse * TIMESTEP;
	m_VelY += y * m_MassInverse * TIMESTEP;
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

	m_Alpha = vel;

}

void Particle::createParticleVerticies(VertexBuffer& vbo, IndexBuffer& ibo) {
	vbo.AddVertex({ m_X - m_Radius, m_Y - m_Radius, m_Alpha });
	vbo.AddVertex({ m_X + m_Radius, m_Y - m_Radius, m_Alpha });
	vbo.AddVertex({ m_X + m_Radius, m_Y + m_Radius, m_Alpha });
	vbo.AddVertex({ m_X - m_Radius, m_Y + m_Radius, m_Alpha });

    unsigned int i = ibo.GetNextFreeIndicie();
	ibo.AddIndicies({ i, i + 1, i + 2, i + 2,i + 3,i });
}

void Particle::createParticleVerticies(VertexBuffer& vbo) {
	vbo.AddVertex({ m_X - m_Radius, m_Y - m_Radius, m_Alpha });
	vbo.AddVertex({ m_X + m_Radius, m_Y - m_Radius, m_Alpha });
	vbo.AddVertex({ m_X + m_Radius, m_Y + m_Radius, m_Alpha });
	vbo.AddVertex({ m_X - m_Radius, m_Y + m_Radius, m_Alpha });
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