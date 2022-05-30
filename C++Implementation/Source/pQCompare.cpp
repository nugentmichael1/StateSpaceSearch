
#include "../Headers/pQCompare.h"

bool pQCompare::operator()(state *lhs, state *rhs)
{
    return lhs->estTtlCost > rhs->estTtlCost;
}