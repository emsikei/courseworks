#ifndef OVERLOADS_H
#define OVERLOADS_H

#include <iostream>
#include <vector>

std::ostream &operator<<(std::ostream &os, const std::vector<std::string> &values)
{
    if (!os.good())
        return os;

    for (auto value : values)
    {
        os << value << " ";
    }
    return os;
}

#endif