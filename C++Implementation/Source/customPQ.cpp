
#include "../Headers/customPQ.h"

template<class T>
bool customPQ<T>::remove(T &s1)
{
    // find an occurence of the state in the priority queue
    auto it = find_if(this->c.begin(), this->c.end(), [s1](T s2)
                      { return (s1->perm == s2->perm); });

    // if the state was found, remove it from vector (priority queue)
    if (it != this->c.end())
    {
        this->c.erase(it);
        make_heap(this->c.begin(), this->c.end(), this->comp);
        return true;
    }
    else
    {
        return false;
    }
}

//informs compiler of which template argument to expect
template class customPQ<state*>;