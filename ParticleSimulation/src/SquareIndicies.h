#pragma once
#include <vector>
#include "Renderer.h"

struct SquareIndicies {
public:
    std::vector<unsigned int> points = { };

    SquareIndicies(std::vector<unsigned int> _points) {
        ASSERT(_points.size() == 6);
        points.insert(points.begin(), _points.begin(), _points.end());
    }

    void PushBackIndicies(std::vector<unsigned int>& output) {
        output.insert(output.end(), points.begin(), points.end()); //TODO: replace int array of indicies with this
    }
};