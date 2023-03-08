#pragma once

#include <cmath>
#include <iostream>
#include <vector>

#include "Particle.h"

class ParticleSpawner {
private:
    void crossProduct(float v_A[], float v_B[], float c_P[]) {
        c_P[0] = v_A[1] * v_B[2] - v_A[2] * v_B[1];
        c_P[1] = -(v_A[0] * v_B[2] - v_A[2] * v_B[0]);
        c_P[2] = v_A[0] * v_B[1] - v_A[1] * v_B[0];
    }

    void generateRandomPoint(float R, float& x, float& y, float xOffset, float yOffset) {
        float sigma = R / (float)sqrt(2);
        float u = (float)rand() / RAND_MAX;
        float v = (float)rand() / RAND_MAX;
        float r = sigma * sqrt(-2 * log(u));
        float theta = 2 * 3.14159265359f * v;
        x = r * cos(theta) + xOffset;
        y = r * sin(theta) + yOffset;
    }


public:
    ParticleSpawner () {  }

    void SpawnGalaxy(std::vector<float> centerCoordinates, std::vector<float> initialVel, int particleCount, float centerMass, float particleMass, float galaxyRadius,
        float spinSpeed, std::vector<Particle>& particlesVector) {

        ASSERT(centerCoordinates.size() == 2);
        ASSERT(initialVel.size() == 2);

        //keep track of x and y values used to prevent particles spawning on top of one another
        std::vector<float> usedXValues = { centerCoordinates[0] };
        std::vector<float> usedYValues = { centerCoordinates[1] };

        //galaxy center
        particlesVector.push_back(Particle(centerCoordinates[0], centerCoordinates[1], initialVel[0], initialVel[1], centerMass, .003f));

        for (int i = 0; i < particleCount; i++) {
            while (true) { //try to spawn a particle as many times as needed, failed spawn is when the position is same
                float x = 0.0f;
                float y = 0.0f;

                generateRandomPoint(galaxyRadius, x, y, centerCoordinates[0], centerCoordinates[1]);

                //if both x and y is used, bugs: the values might be of different particles
                if (std::find(usedXValues.begin(), usedXValues.end(), x) != usedXValues.end() && std::find(usedYValues.begin(), usedYValues.end(), y) != usedYValues.end()) {
                    continue;
                }

                std::vector<float> diff = { x - centerCoordinates[0]  , y - centerCoordinates[1] };
                float distFromCenter = (float)sqrt(pow(diff[0], 2) + pow(diff[1], 2));

                float vec1[] = { 0.0f, 0.0f, 1.0f };
                float vec2[] = { diff[0], diff[1], 0.0f };
                float vecOut[3];

                crossProduct(vec1, vec2, vecOut);

                std::vector<float> v_perp = { vecOut[0], vecOut[1] };

                particlesVector.push_back(Particle(x, y, initialVel[0] + v_perp[0] * spinSpeed * (1 / distFromCenter), initialVel[1] + v_perp[1] * spinSpeed * (1 / distFromCenter), particleMass, .0015f));
                break;
                
            }
        }
    }

    void SpawnSquare(unsigned int numberOfParticles, std::vector<Particle>& particlesVector) {
        std::vector<float> usedXValues = {};
        std::vector<float> usedYValues = {};

        for (unsigned int i = 0; i < numberOfParticles; i++) {
            float x;
            float y;

            while(true) { //FIND UNUSED X
                x = rand() % 2000 / 1000.0f - 1.0f;
                if (std::find(usedXValues.begin(), usedXValues.end(), x) == usedXValues.end()) {
                    break;
                }
                usedXValues.push_back(x);
            }

            while (true) {//FIND UNUSED Y
                y = rand() % 2000 / 1000.0f - 1.0f;
                if (std::find(usedYValues.begin(), usedYValues.end(), y) == usedYValues.end()) {
                    break;
                }
                usedYValues.push_back(y);
            }
            
            particlesVector.push_back(Particle(x, y, 0.0f, 0.0f, 3.0f, .0015f));
        }
    }
};



