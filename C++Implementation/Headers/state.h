// state object.  conceptualizes all the atributes associated with a state.

#ifndef STATE_H
#define STATE_H

#include <string>
#include <iostream>

struct state
{
    std::string perm;        // permutation (eg "123450786")
    state *parent;      // pointer to parent state: the state found this one.
    char movement;      // the move used to get from parent state to this one. (U,D,L,R) S is for start.
    int level = 0;      // What depth has it reached.  Only used by iterative deepening dfs.  Also used by iterative deepening A* (although it probably shouldn't)
    int costSoFar = 0;  // acts same as level in tile-slider domain, but has different name for readability purposes.
    int estTtlCost = 0; // estimated total cost: f(n) = g(n) + h(n) = costSoFar + hueristic.

    // constructor for non-heuristic algorithms
    state(std::string perm, state *parent, char movement, int level);

    // constructor for heuristic algorithms
    state(std::string perm, state *parent, char movement, int costSoFar, int estTtlCost);

    // debug tool
    void print();
};

#endif