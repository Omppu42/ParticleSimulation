#include "SquareIndicies.h"

SquareIndicies::SquareIndicies(std::vector<unsigned int> _points) {
    if (_points.size() != 6) __debugbreak();

    m_Points.insert(m_Points.begin(), _points.begin(), _points.end());
}

SquareIndicies::SquareIndicies(unsigned int _starting_point) {
    m_Points.push_back(_starting_point);
    m_Points.push_back(_starting_point + 1);
    m_Points.push_back(_starting_point + 2);
    m_Points.push_back(_starting_point + 2);
    m_Points.push_back(_starting_point + 3);
    m_Points.push_back(_starting_point);
}

void SquareIndicies::PushBackIndicies(std::vector<unsigned int>& output) {
    output.insert(output.end(), m_Points.begin(), m_Points.end()); //TODO: replace int array of indicies with this
}