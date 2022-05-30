// used by A* to get rid of repeat entries on the frontier once one is expanded

#ifndef CUSTOMPQ_H
#define CUSTOMPQ_H

#include <queue>
#include <vector>
#include <algorithm>
#include "pQCompare.h"

template <class T>
class customPQ : public std::priority_queue<T, std::vector<T>, pQCompare>
{
public:
    bool remove(T &s1);
};

#endif