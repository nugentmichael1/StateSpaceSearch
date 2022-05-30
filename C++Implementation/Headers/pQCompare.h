// custom comparator for priority queue to use

#ifndef PQCOMPARE_H
#define PQCOMPARE_H

#include "state.h"

class pQCompare
{
public:
    bool operator()(state *lhs, state *rhs);
};

#endif