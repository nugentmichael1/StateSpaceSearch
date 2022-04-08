#include <iostream>      //console output
#include <vector>        //array
#include <algorithm>     //find
#include <unordered_map> //dictionary
#include <string>        //perm
#include <fstream>       //output file
#include <math.h>        //floor
#include <queue>         //bfs frontier
#include <stack>         //IDA* frontier

using namespace std;

// state object.  conceptualizes all the atributes associated with a state.
struct state
{
    string perm;        // permutation (eg "123450786")
    state *parent;      // pointer to parent state: the state found this one.
    char movement;      // the move used to get from parent state to this one. (U,D,L,R) S is for start.
    int level = 0;      // What depth has it reached.  Only used by iterative deepening dfs.  Also used by iterative deepening A* (although it probably shouldn't)
    int costSoFar = 0;  // acts same as level in tile-slider domain, but has different name for readability purposes.
    int estTtlCost = 0; // estimated total cost: f(n) = g(n) + h(n) = costSoFar + hueristic.

    // constructor for non-heuristic algorithms
    state(string perm, state *parent, char movement, int level) : perm(perm), parent(parent), movement(movement), level(level) {}

    // constructor for heuristic algorithms
    state(string perm, state *parent, char movement, int costSoFar, int estTtlCost) : perm(perm), parent(parent), movement(movement), costSoFar(costSoFar), estTtlCost(estTtlCost) {}

    // debug tool
    void print()
    {
        cout << "-- State -- perm: " << perm << "; parent: " << parent << "; movement: " << movement << "; level: " << level << "; costSoFar: " << costSoFar << "; estTtlCost: " << estTtlCost << "\n";
    }
};

// custom comparator for priority queue to use
class pQCompare
{
public:
    bool operator()(state *lhs, state *rhs)
    {
        return lhs->estTtlCost > rhs->estTtlCost;
    }
};

// used by A* to get rid of repeat entries on the frontier once one is expanded
template <class T>
class customPQ : public priority_queue<T, vector<T>, pQCompare>
{
public:
    bool remove(T &s1)
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
};

class SSsearch
{
    vector<state *> expanded;     // A*
    vector<state *> reached;      // tracks states seen already to avoid unnecessary expansions and memory use
    customPQ<state *> f;          // frontier object.  based on algorithm, composed of a stack, queue, or priority queue.
    unsigned int expandCount = 0; // keep track of states expanded
    state *start;                 // pointer to start state
    state *curState;              // pointer to current state.
    string goalPerm;              // goal state representation
    int dimension;                // usually 3 or 4.  sqrt(permutation length)
    string solution;              // sequence of movements.
    state *prevState;             // prevents check for last move

    // opens up possible new states from current state, and adds to frontier if not seen before
    void expand()
    {
        this->expandCount += 1;

        // order: up, down, left, right

        // find zero (blank) tile index
        int zeroIndex = this->curState->perm.find('0');

        // find coordinate of zero (blank) tile
        int zeroRow = floor(zeroIndex / this->dimension);
        int zeroCol = zeroIndex % this->dimension;

        // up & down
        if (zeroRow > 0)
        {
            // check up

            // find swap target's index
            int swapTargetIndex = zeroIndex - this->dimension;

            // check if up movement's resultant new state has already been reached.  if not, add to reached and frontier.
            this->checkReachedAddToFrontier(swapTargetIndex, 'U', zeroIndex);
        }

        if (zeroRow < this->dimension - 1)
        {
            // check down

            // find swap target's index
            int swapTargetIndex = zeroIndex + this->dimension;

            // this will check if the down move state has already been reached, and add it to reached and frontier, if not.
            this->checkReachedAddToFrontier(swapTargetIndex, 'D', zeroIndex);
        }

        // check left and right
        if (zeroCol > 0)
        {
            // check left

            // find swap target's index
            int swapTargetIndex = zeroIndex - 1;

            // this will check if the Left move state has already been reached, and add it to reached and frontier, if not.
            this->checkReachedAddToFrontier(swapTargetIndex, 'L', zeroIndex);
        }

        if (zeroCol < this->dimension - 1)
        {
            // check right

            // find swap target's index
            int swapTargetIndex = zeroIndex + 1;

            // this will check if the right move state has already been reached, and add it to reached and frontier, if not.
            this->checkReachedAddToFrontier(swapTargetIndex, 'R', zeroIndex);
        }
    }

    // utility function to condense expand()
    void checkReachedAddToFrontier(int swapTargetIndex, char movement, int zeroIndex)
    {

        // create array copy of curState and perform swap
        string newStatePerm = this->curState->perm;
        newStatePerm[zeroIndex] = newStatePerm[swapTargetIndex];
        newStatePerm[swapTargetIndex] = '0';

        // debug
        // cout << "Size of frontier: " << this->f.size() << "\n";
        // cout << "Size of reached: " << this->reached.size() << "\n";
        // cout << "reached[0]: \n";
        // this->reached[0]->print();

        // search reached container for new state
        vector<state *>::iterator it = find_if(this->reached.begin(), this->reached.end(), [newStatePerm](state *s)
                                               { return (s->perm == newStatePerm); });
        if (it == this->reached.end())
        {
            // new state not found in reached.

            // add new state to reached and frontier
            state *s = new state(newStatePerm, this->curState, movement, this->curState->level + 1);
            this->reached.emplace_back(s);
            this->f.push(s);
        }
    }

    // called once an algorithm is run.  builds and returns move sequence from start to goal.
    void traceBack()
    {

        this->curState->print();

        // current state should be at goal state when this function is called
        this->solution = string(1, this->curState->movement);

        // second link in the state sequence
        state *cur = this->curState->parent;

        // case where start state is goal state
        if (!cur)
            return;

        // follow parents until start state is reached.
        while (cur->parent != NULL)
        {

            // record movement used to get to this state from parent
            this->solution += cur->movement;

            // update current state pointer to parent of current
            cur = cur->parent;
        }

        // reverse the solution string
        reverse(this->solution.begin(), this->solution.end());
    }

    // Reports results to console.
    void reportResults()
    {
        cout << "Start State: " << this->start->perm << "; Goal state: " << this->goalPerm << "; Solution: " << this->solution << "\n";

        cout << "Expanded Count: " << this->expandCount << "; Frontier Size: " << this->f.size() << "; Reached Size: " << this->reached.size() << "\n";
    }

    // // breadth first search
    // void bfs()
    // {

    //     // this->f = frontier("queue");

    //     while (true)
    //     {

    //         if (this->curState->perm == this->goalPerm)
    //         {
    //             return;
    //         }

    //         // else expand current state
    //         this->expand();

    //         this->curState = this->f.front();
    //         this->f.pop();

    //         // debug:
    //         cout << "curState";
    //         this->curState->print();
    //     }
    // }
    // heuristic.  calculates distances of each tile from final state location and sums all together.
    int manhattanDistance(string &perm)
    {
        int totalDistance = 0;

        for (int i = 0; i < perm.length(); i++)
        {

            // determine target index
            int currentValue = perm[i];
            string::iterator it = find(this->goalPerm.begin(), this->goalPerm.end(), currentValue);
            int targetIndex = it - this->goalPerm.begin();

            // determine coordinates of current position
            int currentRow = i / this->dimension;
            int currentCol = i % this->dimension;

            // determine coordinates of target position
            int targetRow = targetIndex / this->dimension;
            int targetCol = targetIndex % this->dimension;

            // determine how many moves must be made per dimension
            int horizontal = abs(currentCol - targetCol);
            int vertical = abs(currentRow - targetRow);

            // add both dimension distances to total distance
            totalDistance += horizontal + vertical;
        }

        return totalDistance;
    }

    // only used by A* (not the same as IDA*).  checks expanded vector instead of reached like the algorithms.
    void checkExpandedAddToFrontier(int swapTargetIndex, char movement, int zeroIndex)
    {
        // create array copy of curState
        string newStatePerm = this->curState->perm;

        // perform swap so as to represent the new state
        newStatePerm[zeroIndex] = newStatePerm[swapTargetIndex];
        newStatePerm[swapTargetIndex] = '0';

        if (this->prevState->perm == newStatePerm)
            return;

        // search expanded container for new state
        vector<state *>::iterator it = find_if(this->expanded.begin(), this->expanded.end(), [newStatePerm](state *s)
                                               { return (s->perm == newStatePerm); });
        if (it == this->expanded.end())
        {
            // new state not found in expanded.  add to frontier and expanded.

            // estimated cost from new state to goal state based on heuristic
            int heuristicCost = manhattanDistance(newStatePerm);

            // cost so far to new state
            int newCostSoFar = this->curState->costSoFar + 1;

            // total estimated cost: f(n) = g(n) + h(n) = newCostSoFar + heuristicCost
            int ttlEstCost = newCostSoFar + heuristicCost;

            // create state to add to expanded and frontier
            state *s = new state(newStatePerm, this->curState, movement, newCostSoFar, ttlEstCost);

            // add state to expanded vector
            this->expanded.emplace_back(s);

            // add state to frontier
            this->f.push(s);
        }
    }

    // uses heuristic to find optimal solution
    void expandAStar()
    {
        // increment expand count.  may be able to just take size of expanded vector at end.
        this->expandCount += 1;

        cout << "expandAStar(): ";
        this->curState->print();

        // premptively add curState to expanded tracker
        this->expanded.emplace_back(this->curState);

        // identify coordinate of empty tile within puzzle slider
        int zeroIndex = this->curState->perm.find('0');
        int zeroRow = floor(zeroIndex / this->dimension);
        int zeroCol = zeroIndex % this->dimension;

        // order: up, down, left, right

        // up & down
        if (zeroRow > 0)
        {
            // check up

            // find swap target's index
            int swapTargetIndex = zeroIndex - this->dimension;

            // check if up movement's resultant new state has already been reached.  if not, add to reached and frontier.
            this->checkExpandedAddToFrontier(swapTargetIndex, 'U', zeroIndex);
        }

        if (zeroRow < this->dimension - 1)
        {
            // check down

            // find swap target's index
            int swapTargetIndex = zeroIndex + this->dimension;

            // this will check if the down move state has already been reached, and add it to reached and frontier, if not.
            this->checkExpandedAddToFrontier(swapTargetIndex, 'D', zeroIndex);
        }

        // check left and right
        if (zeroCol > 0)
        {
            // check left

            // find swap target's index
            int swapTargetIndex = zeroIndex - 1;

            // this will check if the Left move state has already been reached, and add it to reached and frontier, if not.
            this->checkExpandedAddToFrontier(swapTargetIndex, 'L', zeroIndex);
        }

        if (zeroCol < this->dimension - 1)
        {
            // check right

            // find swap target's index
            int swapTargetIndex = zeroIndex + 1;

            // this will check if the right move state has already been reached, and add it to reached and frontier, if not.
            this->checkExpandedAddToFrontier(swapTargetIndex, 'R', zeroIndex);
        }
    }
    void aStar()
    {
        while (true)
        {
            // check to see if goal state found and taken off frontier
            if (this->curState->perm == this->goalPerm)
            {
                return;
            }

            // explore next states and possibly add to frontier
            this->expandAStar();

            // remove all occurences of recently expanded state from frontier
            while (f.remove(this->curState))
            {
            }
            // this->f.removeAll(this->curState);

            // debug:
            cout << "A*: most recently expanded state\n";
            this->curState->print();
            cout << "Expanded count:" << this->expandCount << "\n";
            cout << "Priority Queue size: " << this->f.size() << "\n";

            this->prevState = this->curState;

            // get next state from priority queue
            this->curState = this->f.top();
            this->f.pop();
        }
    }

    // void iDAStar()
    // {

    //     // get heuristic h(n) for start state: f(n) = g(n) + h(n)
    //     this->start->estTtlCost = this->manhattanDistance(this->start->perm);

    //     // depth boundary is based on start state's h(n): f(n) = g(n) + h(n) = 0 + h(n) = h(n)
    //     int fMax = this->start->estTtlCost;

    //     // set minimum for unexpanded states f(n) distances to infinity
    //     int fMin = INT32_MAX;

    //     // // debug
    //     // cout << "iDAStar() variables initialization\n";
    //     // cout << "this->curState->estTtlCost: " << this->curState->estTtlCost;
    //     // cout << "; fMax: " << fMax << "; fMin: " << fMin << "\n";

    //     // debug: delete previous fMax progress file
    //     ofstream progress("Progress.txt");

    //     // add header to new progress file
    //     progress << this->start->perm << "\n";

    //     // close just to conserve resources.
    //     progress.close();

    //     // debug:
    //     int statesExpandedCntPerIteration = 0;

    //     while (true)
    //     {

    //         // check if goal state found
    //         if (this->curState->perm == this->goalPerm)
    //         {
    //             return;
    //         }

    //         // get heuristic: h(n)
    //         int hn = this->manhattanDistance(this->curState->perm);

    //         // set state's g(n) based on level set in expand()
    //         this->curState->costSoFar = this->curState->level;

    //         // set estimated total cost: f(n) = g(n) + h(n)
    //         this->curState->estTtlCost = hn + this->curState->costSoFar;

    //         // only expand states below the depth boundary
    //         if (this->curState->estTtlCost <= fMax)
    //         {
    //             // explore next states.  possibly add them to frontier.
    //             this->expand();
    //             statesExpandedCntPerIteration++;
    //         }
    //         else if (this->curState->estTtlCost < fMin)
    //         {
    //             // track minimum estimated cost from set of all states that exceeded depth boundary
    //             fMin = this->curState->estTtlCost;
    //         }

    //         if (this->f.size() == 0)
    //         {

    //             // set new depth boundary
    //             fMax = fMin;

    //             // this is to keep progress report on hard problems since they take so long.  updates a file with the latest depth iteration reached.
    //             ofstream progress("Progress.txt", ios_base::app);
    //             if (progress.is_open())
    //             {
    //                 progress << "fMax: " << fMax << "\n";
    //                 progress << "States Expanded this iteration: " << statesExpandedCntPerIteration << "\n";
    //             }
    //             else
    //                 cout << "Failed to open Progress.txt.\n";

    //             // this will automatically close when the variable loses scope, but doesn't hurt to be explicit?
    //             progress.close();

    //             // debug
    //             // cout << "Last state of iteration's stats:\n";
    //             // this->curState->print();
    //             // cout << "States expanded this iteration: " << statesExpandedCntPerIteration << "\n";

    //             statesExpandedCntPerIteration = 0;

    //             // reset minimum rejected node distance to infinite
    //             fMin = INT32_MAX;

    //             // clear reached vector
    //             this->reached = vector<state *>(1, this->start);

    //             // reset current state to start state
    //             this->curState = this->start;
    //         }
    //         else
    //         {
    //             // get next state from priority queue
    //             this->curState = this->f.top();
    //             f.pop();
    //         }
    //     }
    // }

public:
    SSsearch(string start, string goal) : start(new state(start, NULL, 'S', 0)), goalPerm(goal), curState(this->start), prevState(this->start)
    {

        // record that start state has been seen
        this->reached.emplace_back(this->start);

        // set dimension (e.g. 3x3 or 4x4)
        this->dimension = sqrt(this->start->perm.length());

        // call bfs search algorithm
        this->aStar();

        // get move sequence (aka solution)
        this->traceBack();

        // notify console of results
        this->reportResults();
    }

    // returns solution (move sequence)
    string getSolution()
    {
        return this->solution;
    }
};

int main(int argc, char *argv[])
{

    ofstream output("PatternHeuristic.txt");

    unordered_map<int, char> dictionary({
        {0, '0'},
        {1, '1'},
        {2, '2'},
        {3, '3'},
        {4, '4'},
        {5, '5'},
        {6, '6'},
        {7, '7'},
        {8, '8'},
        {9, '9'},
        {10, 'A'},
        {11, 'B'},
        {12, 'C'},
        {13, 'D'},
        {14, 'E'},
        {15, 'F'},
    });

    if (output.is_open())
    {

        for (int i = 0; i < 16; i++)
        {

            vector<int> skip(6);

            skip[0] = i;

            for (int j = 0; j < 16; j++)
            {

                vector<int>::iterator it = find(skip.begin(), skip.begin() + 1, j);

                if (it == skip.begin() + 1)
                {
                    skip[1] = j;

                    for (int k = 0; k < 16; k++)
                    {
                        it = find(skip.begin(), skip.begin() + 2, k);

                        if (it == skip.begin() + 2)
                        {
                            skip[2] = k;

                            for (int l = 0; l < 16; l++)
                            {
                                it = find(skip.begin(), skip.begin() + 3, l);

                                if (it == skip.begin() + 3)
                                {
                                    skip[3] = l;

                                    for (int m = 0; m < 16; m++)
                                    {

                                        it = find(skip.begin(), skip.begin() + 4, m);

                                        if (it == skip.begin() + 4)
                                        {

                                            skip[4] = m;

                                            for (int n = 0; n < 16; n++)
                                            {

                                                it = find(skip.begin(), skip.begin() + 5, n);

                                                if (it == skip.begin() + 5)
                                                {
                                                    skip[5] = n;

                                                    for (int o = 0; o < 16; o++)
                                                    {
                                                        it = find(skip.begin(), skip.end(), o);

                                                        if (it == skip.end())
                                                        {
                                                            vector<int> perm7(7);
                                                            perm7[0] = i;
                                                            perm7[1] = j;
                                                            perm7[2] = k;
                                                            perm7[3] = l;
                                                            perm7[4] = m;
                                                            perm7[5] = n;
                                                            perm7[6] = o;

                                                            string start(16, '*');
                                                            start[12] = dictionary[i];
                                                            start[13] = dictionary[j];
                                                            start[14] = dictionary[k];
                                                            start[15] = dictionary[l];
                                                            start[11] = dictionary[m];
                                                            start[7] = dictionary[n];
                                                            start[3] = dictionary[o];

                                                            string goalState(16, '*');

                                                            vector<int>::iterator it7;

                                                            for (int p = 1; p < 16; p++)
                                                            {
                                                                it7 = find_if(perm7.begin(), perm7.end(), [p](int v)
                                                                              { return p == v; });
                                                                if (it7 != perm7.end())
                                                                {
                                                                    // found value.  put in goalState.
                                                                    goalState[p - 1] = dictionary[p];
                                                                }
                                                            }

                                                            goalState[15] = '0';

                                                            it7 = find_if(perm7.begin(), perm7.end(), [](int v)
                                                                          { return v == 0; });
                                                            if (it7 == perm7.end())
                                                            {
                                                                // found no zero.  add to permutation

                                                                start[0] = 0;
                                                            }
                                                            cout << "Start State: " << start << "\n";
                                                            cout << "Goal State: " << goalState << "\n";

                                                            cout << "Search begin:\n";

                                                            SSsearch s(start, goalState);

                                                            string solution = s.getSolution();

                                                            cout << start << solution.size() << "\n";

                                                            output << start << solution.size() << "\n";
                                                        }
                                                    }
                                                }
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    else
    {
        cout << "Failed to open output file.\n";
    }

    return 0;
}