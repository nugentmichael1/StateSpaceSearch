

#include "../Headers/state.h"

// constructor for non-heuristic algorithms
state::state(std::string perm, state *parent, char movement, int level) : perm(perm), parent(parent), movement(movement), level(level) {}

// constructor for heuristic algorithms
state::state(std::string perm, state *parent, char movement, int costSoFar, int estTtlCost) : perm(perm), parent(parent), movement(movement), costSoFar(costSoFar), estTtlCost(estTtlCost) {}

// debug tool
void state::print()
{
    std::cout << "-- State -- perm: " << perm << "; parent: " << parent << "; movement: " << movement << "; level: " << level << "; costSoFar: " << costSoFar << "; estTtlCost: " << estTtlCost << "\n";
}