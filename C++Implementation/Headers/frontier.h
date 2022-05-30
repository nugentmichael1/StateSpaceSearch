

#ifndef FRONTIER_H
#define FRONTIER_H

#include <queue>
#include <stack>
#include "customPQ.h"
#include <string>

class frontier
{
    std::queue<state *> q;   // a queue object.  used by breadth first search (bfs)
    std::stack<state *> stk; // stack object.  used by depth first search (dfs), iterative deepening depth first search (iddfs), and iterative deepening A* (IDA*)
    customPQ<state *> pQ;    // priority queue with custom remove function.  Used by A*
    std::string type;        // tracks which type frontier should use.

public:
    frontier();
    frontier(std::string type);

    // push a state onto the frontier.  used by all algorithms.
    void push(state *s);

    // take a state off the frontier and return it.  Used by all algorithms.
    state *pop();

    // check size of frontier.  Only useful for iterative deepening algorithms, and statistics.
    int size();

    // only for priority queue, which is only used in A* (not IDA*).  gets rid of all states with the same representation since there is no descrimination against non-unique states until one is expanded.
    void removeAll(state *s);

    
};

#endif