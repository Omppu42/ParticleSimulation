#pragma once

#include <vector>

struct SquareIndicies {
private:
    std::vector<unsigned int> m_Points = { };

public:

    SquareIndicies(std::vector<unsigned int> _points);
    SquareIndicies(unsigned int _starting_point);
    void PushBackIndicies(std::vector<unsigned int>& output);
};