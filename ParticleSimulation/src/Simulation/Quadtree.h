#pragma once
#include <vector>
#include <memory>
#include <algorithm>
#include <queue>
#include <stack>

#include "Particle.h"

class Container {
public:
	float x;
	float y;
	float w;
	float h;

	Container() { 
		x = 0.0f;
		y = 0.0f;
		w = 0.0f;
		h = 0.0f;
	}

	Container(float _x, float _y, float _w, float _h)
	{
		x = _x;
		y = _y;
		w = _w;
		h = _h;
	}

	bool Intersects(float _x, float _y) {
		if (_x >= x && _x <= x + w &&
			_y >= y && _y <= y + h) {
			return true;
		}

		return false;
	}
};

#define MAX_PARTICLES 16
#define MAX_DEPTH 8
#define FAR_AWAY_TRESHOLD 50

class Quadtree {
private:

	std::unique_ptr<Quadtree> nw;
	std::unique_ptr<Quadtree> ne;
	std::unique_ptr<Quadtree> sw;
	std::unique_ptr<Quadtree> se;

	Container container;
	std::vector<Particle> particles;
	std::vector<Particle> allPartcilesInChildren;
	int depth;
	float totalMass;
	float sizePercent;
	std::vector<float> centerOfMass;

	void Divide() {
		Quadtree* _nw = new Quadtree({ container.x, container.y + container.h / 2,						container.w / 2, container.h / 2 }, depth + 1);
		Quadtree* _ne = new Quadtree({ container.x + container.w / 2, container.y + container.h / 2,	container.w / 2, container.h / 2 }, depth + 1);
		Quadtree* _sw = new Quadtree({ container.x, container.y,										container.w / 2, container.h / 2 }, depth + 1);
		Quadtree* _se = new Quadtree({ container.x + container.w / 2, container.y,						container.w / 2, container.h / 2 }, depth + 1);

		nw = std::unique_ptr<Quadtree>(_nw);
		ne = std::unique_ptr<Quadtree>(_ne);
		sw = std::unique_ptr<Quadtree>(_sw);
		se = std::unique_ptr<Quadtree>(_se);

		for (Particle p : particles) {
			ASSERT(InsertParticle(p, false)); //try to input the particle into deeper levels
		}
		particles.clear();
	}

public:
	Quadtree(Container _container, int _currentDepth) {
		nw = nullptr;
		ne = nullptr;
		sw = nullptr;
		se = nullptr;

		particles = {};
		allPartcilesInChildren = {};
		container = _container;
		depth = _currentDepth;
		totalMass = 0.0f;
		centerOfMass = {};

		if (_currentDepth == 0)
			sizePercent = 100.0f;
		else 
			sizePercent = 100.0f / (float)(pow(2, _currentDepth));
	}

	Container* GetContainer() {
		return &container;
	}

	int GetParticlesCount() {
		return particles.size();
	}
	int GetChildParticlesCount() {
		return allPartcilesInChildren.size();
	}
	float GetTotalMass() {
		return totalMass;
	}
	Quadtree* GetNWNode() {
		return nw.get();
	}

	bool InsertParticle(Particle& _particle, bool addMass = true) {
		if (!container.Intersects(_particle.m_X, _particle.m_Y)) //Not inside root container, return
			return false;

		if (addMass) {
			totalMass += _particle.m_Mass;
			allPartcilesInChildren.push_back(_particle);
		}

		if (depth >= MAX_DEPTH) {			//if max depth reached, add to particles vector
			particles.push_back(_particle);
			return true;
		}
		if (nw == nullptr) {
			if (particles.size() < MAX_PARTICLES) { //If room add to particles vector
				particles.push_back(_particle);
				return true;
			}

			//Divide if no nw
			Divide();
		}

		if (nw->InsertParticle(_particle)) return true;
		if (ne->InsertParticle(_particle)) return true;
		if (sw->InsertParticle(_particle)) return true;
		if (se->InsertParticle(_particle)) return true;

		return false;
	}

	void ClearTree() {
		if (nw != nullptr) {
			nw->ClearTree();
			ne->ClearTree();
			sw->ClearTree();
			se->ClearTree();

			nw.reset();
			ne.reset();
			sw.reset();
			se.reset();
		}

		if (depth != 0) return;

		particles = {};
		allPartcilesInChildren = {};
		totalMass = 0.0f;
	}

	void CalculateForceOnParticles(std::vector<Particle>& allPartciles) { //MAIN
		for (Particle& p : allPartciles) {
			CalculateParicleVelocityRecursive(p);
		}
	}

	void CalculateParicleVelocityRecursive(Particle& p) { //REUCURSIVE
		if (centerOfMass.size() == 0) {
			CalculateCOM();
		}

		if (nw == nullptr) {
			for (Particle& p2 : particles) {
				//std::cout << "calc" << std::endl;
				if (p == p2) continue;
				p.calculateForceWithParticle(p2);
			}
			return;
		}

		if (OnlyCalclateThisCOM(p, centerOfMass, sizePercent)) {
			p.calculateForceWithPosition(centerOfMass[0], centerOfMass[1], totalMass);
			return;
		}

		nw->CalculateParicleVelocityRecursive(p);
		ne->CalculateParicleVelocityRecursive(p);
		sw->CalculateParicleVelocityRecursive(p);
		se->CalculateParicleVelocityRecursive(p);
	}

	bool IntersectsRegionOfInfluence(const Particle& p, Quadtree* node) {
		float distance = (float)sqrt(pow(node->centerOfMass[0] - p.m_X, 2) + pow(node->centerOfMass[1] - p.m_Y, 2));
		return node->sizePercent / distance < FAR_AWAY_TRESHOLD;
	}

	bool OnlyCalclateThisCOM(Particle& p, std::vector<float>& thisQuadrantCOM, float size) {
		ASSERT(thisQuadrantCOM.size() == 2);

		float distance = sqrt(pow(thisQuadrantCOM[0] - p.m_X, 2) + pow(thisQuadrantCOM[1] - p.m_Y, 2));

		if (size / distance < FAR_AWAY_TRESHOLD) {
			return true;
		}

		return false;
	}

	void CalculateCOM() {
		float x = 0.0f;
		float y = 0.0f;

		for (Particle& p : allPartcilesInChildren) {
			x += (p.m_X * p.m_Mass);
			y += (p.m_Y * p.m_Mass);
		}

		centerOfMass =  { x / totalMass, y / totalMass };
	}

	friend std::ostream& operator<<(std::ostream& os, const Quadtree& tree) {
		os << "TREE NODE AT LEVEL " << tree.depth << ": X " << tree.container.x << ", Y " << tree.container.y << ", W " << tree.container.w << ", H " << tree.container.h;
		return os;
	}
};