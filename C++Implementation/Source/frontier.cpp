
#include "../Headers/frontier.h"

frontier::frontier() {}

frontier::frontier(std::string type) : type(type) {}

// push a state onto the frontier.  used by all algorithms.
void frontier::push(state *s)
{
    if (this->type == "queue")
    {
        this->q.push(s);
    }
    else if (this->type == "stack")
    {
        this->stk.push(s);
    }
    else if (this->type == "pQueue")
    {
        this->pQ.push(s);
    }
}

// take a state off the frontier and return it.  Used by all algorithms.
state *frontier::pop()
{
    if (this->type == "queue")
    {
        state *s = this->q.front();
        this->q.pop();
        return s;
    }
    else if (this->type == "stack")
    {
        state *s = this->stk.top();
        this->stk.pop();
        return s;
    }
    else if (this->type == "pQueue")
    {
        state *s = this->pQ.top();
        this->pQ.pop();
        return s;
    }
    else
    {
        std::cout << "pop() failed to find type";
        return NULL;
    }
}

// check size of frontier.  Only useful for iterative deepening algorithms, and statistics.
int frontier::size()
{
    if (this->type == "queue")
    {
        return this->q.size();
    }
    else if (this->type == "stack")
    {
        return this->stk.size();
    }
    else if (this->type == "pQueue")
    {
        return this->pQ.size();
    }
    return 0;
}

// only for priority queue, which is only used in A* (not IDA*).  gets rid of all states with the same representation since there is no descrimination against non-unique states until one is expanded.
void frontier::removeAll(state *s)
{
    if (this->type == "pQueue")
    {
        while (this->pQ.remove(s))
            ;
    }
    else
    {
        std::cout << "frontier.remove() called by incorrect type.\n";
    }
}