/*  State Space Search
Author/Student: Michael Nugent
Course: CSCI 164 - Artificial Intelligence
Professor: David Ruby
Semester: Spring 2022
School: CSU, Fresno

Program Description: This is a C++ implementation of State Space Search algorithms within a Tile Slider Puzzle domain (8, and 15).  The implemented algorithms are three basics: breadth first search (BFS), depth first search (DFS), and iterative deepening dfs (IDDFS); and two advanced: A* with hueristics (A*), and iterative deepening dfs with hueristics (IDA*).
Input: A list of start states can be provided by file name.  A pair of start state and goal state along with an algorithm can be provided.
Output:  Depends on input.  Can produce a file of solutions to all supplied input states, or just print a single solution to the console.

Compile: g++ main.cpp -o main.o
Run: There are three ways to run the program.  1) Supply three arguments to the command line: start state, goal state, and the desired algorithm.  2)  Supply to the command line just an input batch file of start states.  3) Supply no arguments.
Run1: ./main.o <start state> <goal state> <algorithm>
Run1 example: ./main.o "123450786" "123456780" "BFS"
Run2: ./main.o <MyInputFile.txt>
Run2 example: ./main.o "ProblemsList.txt"
Run3: ./main.o
If no parameters are provided, a set of default states and algorithm are used, which can be modified within the driver (main function) of main.cpp.
*/

#include <fstream>       //read batch sequences or start states
#include <iostream>      //display answers to console
#include <vector>        //reached, priority queue and stack?
#include <string>        //state permutations
#include <math.h>        //square root
#include <algorithm>     //sort
#include <set>           //check uniqueness of characters of start and goal states
#include <queue>         //bfs frontier, A* priority queue
#include <stack>         //dfs, iddfs, IDA*
#include <unordered_map> //pattern database heuristic
#include <chrono>        //clock time
#include <time.h>        //random seed

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

class frontier
{
    queue<state *> q;     // a queue object.  used by breadth first search (bfs)
    stack<state *> stk;   // stack object.  used by depth first search (dfs), iterative deepening depth first search (iddfs), and iterative deepening A* (IDA*)
    customPQ<state *> pQ; // priority queue with custom remove function.  Used by A*
    string type;          // tracks which type frontier should use.

public:
    frontier() {}
    frontier(string type) : type(type) {}

    // push a state onto the frontier.  used by all algorithms.
    void push(state *s)
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
    state *pop()
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
            cout << "pop() failed to find type";
            return NULL;
        }
    }

    // check size of frontier.  Only useful for iterative deepening algorithms, and statistics.
    int size()
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
    void removeAll(state *s)
    {
        if (this->type == "pQueue")
        {
            while (pQ.remove(s))
                ;
        }
        else
        {
            cout << "frontier.remove() called by incorrect type.\n";
        }
    }
};

class SSsearch
{
    // vector<state *> reached; // tracks states seen already to avoid unnecessary expansions and memory use
    unordered_map<string, state *> reached; // replace vector with hash table for fast lookup (constant)
    // vector<state *> expanded;             // only used for A* algorithms, replaces reached
    unordered_map<string, state *>
        expanded;                         // replace vector with hash table for fast lookup
    frontier f;                           // frontier object.  based on algorithm, composed of a stack, queue, or priority queue.
    unsigned int expandCount = 0;         // keep track of states expanded
    state *start;                         // pointer to start state
    state *curState;                      // pointer to current state.
    string goalPerm;                      // goal state representation
    int dimension;                        // usually 3 or 4.  sqrt(permutation length)
    string solution;                      // sequence of movements.
    string heuristicType = "oOP";         // default heuristic
    unordered_map<string, int> patternDB; // hash table to use pattern database heuristic
    string algo;                          // algorithm used
    chrono::high_resolution_clock::time_point startTime;
    chrono::high_resolution_clock::time_point endTime;
    // chrono::duration<int> durationTime;
    double durationTime;
    unordered_map<char, pair<int, int>> finalPosition;

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
        if (zeroRow > 0 && this->curState->movement != 'D')
        {
            // check up

            // find swap target's index
            int swapTargetIndex = zeroIndex - this->dimension;

            // check if up movement's resultant new state has already been reached.  if not, add to reached and frontier.
            this->checkReachedAddToFrontier(swapTargetIndex, 'U', zeroIndex);
        }

        if (zeroRow < this->dimension - 1 && this->curState->movement != 'U')
        {
            // check down

            // find swap target's index
            int swapTargetIndex = zeroIndex + this->dimension;

            // this will check if the down move state has already been reached, and add it to reached and frontier, if not.
            this->checkReachedAddToFrontier(swapTargetIndex, 'D', zeroIndex);
        }

        // check left and right
        if (zeroCol > 0 && this->curState->movement != 'R')
        {
            // check left

            // find swap target's index
            int swapTargetIndex = zeroIndex - 1;

            // this will check if the Left move state has already been reached, and add it to reached and frontier, if not.
            this->checkReachedAddToFrontier(swapTargetIndex, 'L', zeroIndex);
        }

        if (zeroCol < this->dimension - 1 && this->curState->movement != 'L')
        {
            // check right

            // find swap target's index
            int swapTargetIndex = zeroIndex + 1;

            // this will check if the right move state has already been reached, and add it to reached and frontier, if not.
            this->checkReachedAddToFrontier(swapTargetIndex, 'R', zeroIndex);
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
        // this->expanded.emplace_back(this->curState);
        this->expanded[this->curState->perm] = this->curState;

        // identify coordinate of empty tile within puzzle slider
        int zeroIndex = this->curState->perm.find('0');
        int zeroRow = zeroIndex / this->dimension;
        int zeroCol = zeroIndex % this->dimension;

        // debug
        // ofstream aStarDebug("aStarDebug.txt", ios_base::app);
        // if (aStarDebug.is_open())
        // {
        //     aStarDebug << "perm: " << this->curState->perm << "; zeroIndex: " << zeroIndex << "; zeroRow: " << zeroRow << "; zeroCol: " << zeroCol << "\n";
        // }

        // ofstream progress("Progress.txt", ios_base::app);
        // if (progress.is_open())
        // {
        //     progress << "fMax: " << fMax << "\n";
        //     progress << "States Expanded this iteration: " << statesExpandedCntPerIteration << "\n";
        // }
        // else
        //     cout << "Failed to open Progress.txt.\n";

        // this will automatically close when the variable loses scope, but doesn't hurt to be explicit?
        // progress.close();

        // order: up, down, left, right

        // up & down
        if (zeroRow > 0)
        //  && this->curState->movement != 'D')
        {
            // check up

            // find swap target's index
            int swapTargetIndex = zeroIndex - this->dimension;

            // check if up movement's resultant new state has already been reached.  if not, add to reached and frontier.
            this->checkExpandedAddToFrontier(swapTargetIndex, 'U', zeroIndex);
        }

        if (zeroRow < this->dimension - 1)
        //  && this->curState->movement != 'U')
        {
            // check down

            // find swap target's index
            int swapTargetIndex = zeroIndex + this->dimension;

            // this will check if the down move state has already been reached, and add it to reached and frontier, if not.
            this->checkExpandedAddToFrontier(swapTargetIndex, 'D', zeroIndex);
        }

        // check left and right
        if (zeroCol > 0)
        //  && this->curState->movement != 'R')
        {
            // check left

            // find swap target's index
            int swapTargetIndex = zeroIndex - 1;

            // this will check if the Left move state has already been reached, and add it to reached and frontier, if not.
            this->checkExpandedAddToFrontier(swapTargetIndex, 'L', zeroIndex);
        }

        if (zeroCol < this->dimension - 1)
        //  && this->curState->movement != 'L')
        {
            // check right

            // find swap target's index
            int swapTargetIndex = zeroIndex + 1;

            // this will check if the right move state has already been reached, and add it to reached and frontier, if not.
            this->checkExpandedAddToFrontier(swapTargetIndex, 'R', zeroIndex);
        }
    }

    // only used by A* (not the same as IDA*).  checks expanded vector instead of reached like the other algorithms.
    void checkExpandedAddToFrontier(int swapTargetIndex, char movement, int zeroIndex)
    {
        // create array copy of curState
        string newStatePerm = this->curState->perm;

        // perform swap so as to represent the new state
        newStatePerm[zeroIndex] = newStatePerm[swapTargetIndex];
        newStatePerm[swapTargetIndex] = '0';

        // debug
        // ofstream aStarDebug("aStarDebug.txt", ios_base::app);
        // if (aStarDebug.is_open())
        // {
        //     aStarDebug << "newStatePerm == curState->perm\n"
        //                << newStatePerm << "\n"
        //                << this->curState->perm << "\n";
        // }

        // search expanded container for new state
        // vector<state *>::iterator it = find_if(this->expanded.begin(), this->expanded.end(), [newStatePerm](state *s)
        //                                        { return (s->perm == newStatePerm); });
        // if (it == this->expanded.end())

        // debug
        // ofstream aStarDebug("aStarDebug.txt", ios_base::app);
        // if (aStarDebug.is_open())
        // {
        //     aStarDebug << "1 -- expanded.count(newStatePerm): "
        //                << this->expanded.count(newStatePerm) << "\n"
        //                << "2 -- expanded.count(newStatePerm): "
        //                << this->expanded.count(newStatePerm) << "\n";
        // }

        if (!this->expanded.count(newStatePerm))
        {
            // new state not found in expanded.  add to frontier and expanded.

            // estimated cost from new state to goal state based on heuristic
            int heuristicCost = heuristic(newStatePerm);

            // cost so far to new state
            int newCostSoFar = this->curState->costSoFar + 1;

            // total estimated cost: f(n) = g(n) + h(n) = newCostSoFar + heuristicCost
            int ttlEstCost = newCostSoFar + heuristicCost;

            // create state to add to expanded and frontier
            state *s = new state(newStatePerm, this->curState, movement, newCostSoFar, ttlEstCost);

            // add state to expanded vector
            // this->expanded.emplace_back(s);
            // this->expanded[newStatePerm] = this->curState;

            // add state to frontier
            this->f.push(s);
        }
    }

    // determines which heuristic to use based on constructor argument
    int heuristic(string &perm)
    {
        int distance = 0;

        if (this->heuristicType == "oOP")
            return this->oOPDistance(perm);
        else if (this->heuristicType == "mHD")
            return this->manhattanDistance(perm);
        // else if (this->heuristicType == "pDB")
        //     return this->patternDatabase();
        else
        {
            cout << "Heuristic type not recognized.";
            return -1;
        }
    }

    // heuristic: "out of place" - counts the number of tiles that are in a different position than goal state's representation
    int oOPDistance(string &perm)
    {
        int distance = 0;

        for (int i = 0; i < perm.length(); i++)
        {
            // for every tile that is not in the correct position, add 1 to the distance
            if (perm[i] != this->goalPerm[i])
                distance++;
        }

        return distance;
    }

    // heuristic.  calculates distances of each tile from final state location and sums all together.
    int manhattanDistance(string &perm)
    {
        int totalDistance = 0;

        // int index = 0;
        // vector<vector<char>> matrixRep(this->dimension, vector<char>(this->dimension));
        // for (int row = 0; row < this->dimension; row++)
        // {
        //     for (int col = 0; col < this->dimension; col++)
        //     {
        //         matrixRep[row][col] = perm[index++];
        //     }
        // }

        for (int row = 0, index = 0; row < this->dimension; row++)
        {
            for (int col = 0; col < this->dimension; col++)
            {
                pair<int, int> coord = this->finalPosition[perm[index++]];
                totalDistance += abs(row - coord.first) + abs(col - coord.second);
            }
        }

        // for (int i = 0; i < perm.length(); i++)
        // {

        //     // determine target index
        //     int currentValue = perm[i];
        //     string::iterator it = find(this->goalPerm.begin(), this->goalPerm.end(), currentValue);
        //     int targetIndex = it - this->goalPerm.begin();

        //     // determine coordinates of current position
        //     int currentRow = i / this->dimension;
        //     int currentCol = i % this->dimension;

        //     // determine coordinates of target position
        //     int targetRow = targetIndex / this->dimension;
        //     int targetCol = targetIndex % this->dimension;

        //     // determine how many moves must be made per dimension
        //     int horizontal = abs(currentCol - targetCol);
        //     int vertical = abs(currentRow - targetRow);

        //     // add both dimension distances to total distance
        //     totalDistance += horizontal + vertical;
        // }

        return totalDistance;
    }

    /*    failed attempt at pattern database heuristic

         // heuristic
         int patternDatabase()
         {
             // cout << "Pattern database size: " << this->patternDB.size() << "\n";
             if (this->patternDB.size() == 0)
             {
                 // create database
                 this->createPatternDatabase();
             }

            return 0;
        }

        void createPatternDatabase()
        {
            // take bottom row and right-most column.  Find number of moves to get solution with just regards to those tiles.
            // Permutation = 16!/9! (not sure how blank tile in non-fringe areas contributes to search space)

            int totalPermutations = 1;
            int n = pow(this->dimension, 2);
            int r = this->dimension * 2 - 1;

            cout << "dimension: " << this->dimension << "\n";

            // calculate total permutations, simulate factorial
            for (int i = n; i > n - r; i--)
            {
                cout << "i: " << i << " n: " << n << "; r: " << r << "\n";
                totalPermutations *= i;
            }

            cout << "totalPermutations: " << totalPermutations << "\n";

            // cout << "sizeof(int): " << sizeof(long int) << "\n";

            for (int i = 0; i < totalPermutations; i++)
            {
                string perm("");
                int distance = 0;

                // build permutation
                for (int j = 0; j < n; j++)
                {
                }

                // add permutation's distance to table
                // this->patternDB.insert({perm, distance});
            }
            cout << "Finished createPatternDatabase()\n";
            // for (int i = 0; i < this->dimension; i++)
            // {
            //     // perm += this->
            // }
        }
        */

    // utility function to condense expand()
    void checkReachedAddToFrontier(int swapTargetIndex, char movement, int zeroIndex)
    {
        // cout << "checkReachedAddToFrontier: ";
        // this->curState->print();

        // create array copy of curState and perform swap
        string newStatePerm = this->curState->perm;
        newStatePerm[zeroIndex] = newStatePerm[swapTargetIndex];
        newStatePerm[swapTargetIndex] = '0';

        // search reached container for new state
        // vector<state *>::iterator it = find_if(this->reached.begin(), this->reached.end(), [newStatePerm](state *s)
        //                                        { return (s->perm == newStatePerm); });
        // if (it == this->reached.end())
        if (!this->reached.count(newStatePerm))
        {
            // new state not found in reached.

            // add new state to reached and frontier
            state *s = new state(newStatePerm, this->curState, movement, this->curState->level + 1);
            // this->reached.emplace_back(s);
            this->reached[newStatePerm] = s;
            this->f.push(s);
            // cout << "added state to frontier\n";
        }
    }

    // ensure input states are composed of sequential and unique characters
    bool validateInput(string type)
    {

        // get input state
        string candidateState = (type == "start") ? this->start->perm : this->goalPerm;

        // check for valid domain (i.e. [0-n))
        // clean
        vector<char> arr3;
        for (int i = 0; i < candidateState.length(); i++)
        {
            int asciiVal = int(candidateState[i]);
            // cout << "asciiVal: " << asciiVal << "\n";

            if (asciiVal >= 48 && asciiVal <= 57)
            {
                // it's a number.  push original
                arr3.emplace_back(asciiVal);
            }
            else if (asciiVal >= 65 && asciiVal <= 90)
            {
                // it's an uppercase letter.  push ascii number
                arr3.emplace_back(asciiVal);
            }
            else if (asciiVal >= 97 && asciiVal <= 122)
            {
                // it's a lower case letter.  make upper and push ascii
                arr3.emplace_back(asciiVal - 32);
            }
            else
            {
                // not a valid character
            }
        }
        sort(arr3.begin(), arr3.end());

        int curAscii = 48;
        for (int i = 0; i < arr3.size() && i < 10; i++)
        {
            if (arr3[i] != curAscii)
            {
                cout << "arr3[" << i << "]: " << arr3[i] << "\n";
                cout << "curAscii: " << curAscii << "\n";
                cout << "Input " << type << " state lacks value " << char(curAscii) << ".\n";
                return false;
            }
            curAscii += 1;
        }
        if (arr3.size() >= 10)
        {
            curAscii = 65;
            for (int i = 10; i < arr3.size() && i < 26; i++)
            {
                if (arr3[i] != curAscii)
                {
                    cout << "Input " << type << " state lacks value " << char(curAscii) << ".\n";
                    return false;
                }
                curAscii += 1;
            }
        }

        // check for uniqueness
        // insert arr3 contents into a set
        set<int> unique(arr3.begin(), arr3.end());
        // cout << "unique.size(): " << unique.size() << "\n";

        // verify every member of the arr was unique
        if (unique.size() == candidateState.size())
        {
            cout << "State string of " << type << " is valid.\n";
            return true;
        }
        else
        {
            cout << "Input values of " << type << " state are not unique.\n";
            return false;
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

            // debug: helps identify sequence origin
            // cur->print();

            // record movement used to get to this state from parent
            this->solution += cur->movement;

            // update current state pointer to parent of current
            cur = cur->parent;
        }

        // debug: print start state
        // cur->print();

        // reverse the solution string
        reverse(this->solution.begin(), this->solution.end());

        this->endTime = chrono::high_resolution_clock::now();
        this->durationTime = double(chrono::duration_cast<chrono::milliseconds>(this->endTime - this->startTime).count());
        this->durationTime /= 1000;
    }

    // Reports results to console.
    void reportResults()
    {
        cout << "Start State: " << this->start->perm << "; Goal state: " << this->goalPerm << "; Solution: " << this->solution << "\n";

        cout << "Expanded Count: " << this->expandCount << "; Frontier Size: " << this->f.size() << "; Reached Size: " << this->reached.size() << "\n";
    }

    // breadth first search
    void bfs()
    {

        this->f = frontier("queue");

        while (true)
        {

            if (this->curState->perm == this->goalPerm)
            {
                return;
            }

            // else expand current state
            this->expand();

            this->curState = this->f.pop();
        }
    }

    // depth first search
    void dfs()
    {
        this->f = frontier("stack");
        while (true)
        {
            // check to see if in goal state
            if (this->curState->perm == this->goalPerm)
            {
                return;
            }

            this->expand();

            this->curState = this->f.pop();
        }
    }

    // iterative deepening depth first search
    void itDdfs()
    {

        // initialize the depth boundary to 1.  this will progressively increase until a solution is found.
        int depthBound = 1;

        // initialize frontier with a stack implementation
        this->f = frontier("stack");

        while (true)
        {

            // check to see if in goal state
            if (this->curState->perm == this->goalPerm)
            {
                return;
            }

            // check curState for appropriate depth before expansion.
            if (this->curState->level < depthBound)
            {
                // expand current state
                this->expand();
            }

            // check whether frontier has states
            if (this->f.size() == 0)
            {
                // frontier stack is empty.

                //  increment depth boundary by 1 and reset everything
                depthBound += 1;

                // clear reached array of all but the start state
                // this->reached = vector<state *>(1, this->start);
                this->reached = unordered_map<string, state *>({{this->start->perm, this->start}});

                // technically this is already empty by this point.  Might remove.
                this->f = frontier("stack");

                // set current state back to start so iteration can begin again
                this->curState = this->start;
            }
            else
            {
                // frontier still has states left.

                // pop a state off the stack.
                this->curState = this->f.pop();
            }
        }
    }

    void aStar()
    {
        this->f = frontier("pQueue");

        // this->expanded.reserve(362880);

        // debug
        ofstream aStarDebug("aStarDebug.txt");
        if (aStarDebug.is_open())
            aStarDebug.close();
        else
        {
            cout << "aStarDebug failed to open.";
            return;
        }

        while (true)
        {
            // cout << "A* iteration\n";

            // check to see if goal state found and taken off frontier
            if (this->curState->perm == this->goalPerm)
            {
                return;
            }

            // explore next states and possibly add to frontier
            this->expandAStar();

            // remove all occurences of recently expanded state from frontier
            // this->f.removeAll(this->curState);

            // debug:
            cout << "A*: most recently expanded state\n";
            this->curState->print();
            cout << "Expanded count:" << this->expandCount << "\n";
            cout << "Priority Queue size: " << this->f.size() << "\n";

            // get next state from priority queue
            this->curState = this->f.pop();
            // make sure state hasn't been expanded already
            // while (this->expanded.count(this->curState->perm))
            // {
            //     // if(this->f.size()==0) return;
            //     // cout << "A* while loop; f.size(): " << this->f.size() << "\n";
            //     this->curState = this->f.pop();
            // }
        }
    }

    void iDAStar()
    {

        // IDA* uses depth first search, which requires a FILO (stack) frontier
        this->f = frontier("stack");

        // get heuristic h(n) for start state: f(n) = g(n) + h(n)
        this->start->estTtlCost = this->heuristic(this->start->perm);

        // depth boundary is based on start state's h(n): f(n) = g(n) + h(n) = 0 + h(n) = h(n)
        int fMax = this->start->estTtlCost;

        // set minimum for unexpanded states f(n) distances to infinity
        int fMin = INT32_MAX;

        // // debug
        // cout << "iDAStar() variables initialization\n";
        // cout << "this->curState->estTtlCost: " << this->curState->estTtlCost;
        // cout << "; fMax: " << fMax << "; fMin: " << fMin << "\n";

        // debug: delete previous fMax progress file
        ofstream progress("Progress.txt");

        // add header to new progress file
        progress << this->start->perm << "\n";

        // close just to conserve resources.
        progress.close();

        // debug:
        int statesExpandedCntPerIteration = 0;

        while (true)
        {

            // check if goal state found
            if (this->curState->perm == this->goalPerm)
            {
                return;
            }

            // get heuristic: h(n)
            int hn = this->heuristic(this->curState->perm);

            // set state's g(n) based on level set in expand()
            this->curState->costSoFar = this->curState->level;

            // set estimated total cost: f(n) = g(n) + h(n)
            this->curState->estTtlCost = hn + this->curState->costSoFar;

            // only expand states below the depth boundary
            if (this->curState->estTtlCost <= fMax)
            {
                // cout << "Frontier (stack) size: " << this->f.size() << "\n";

                // explore next states.  possibly add them to frontier.
                this->expand();
                statesExpandedCntPerIteration++;
            }
            else if (this->curState->estTtlCost < fMin)
            {
                // track minimum estimated cost from set of all states that exceeded depth boundary
                fMin = this->curState->estTtlCost;
            }

            if (this->f.size() == 0)
            {

                // set new depth boundary
                fMax = fMin;

                // this is to keep progress report on hard problems since they take so long.  updates a file with the latest depth iteration reached.
                ofstream progress("Progress.txt", ios_base::app);
                if (progress.is_open())
                {
                    progress << "fMax: " << fMax << "\n";
                    progress << "States Expanded this iteration: " << statesExpandedCntPerIteration << "\n";
                }
                else
                    cout << "Failed to open Progress.txt.\n";

                // this will automatically close when the variable loses scope, but doesn't hurt to be explicit?
                progress.close();

                // debug
                // cout << "Last state of iteration's stats:\n";
                // this->curState->print();
                // cout << "States expanded this iteration: " << statesExpandedCntPerIteration << "\n";

                statesExpandedCntPerIteration = 0;

                // reset minimum rejected node distance to infinite
                fMin = INT32_MAX;

                // cout << "start state";
                // this->start->print();

                string startPerm = this->start->perm;
                // clear reached vector
                // delete all states to free memory
                // int index = 0;
                // cout << "reached size: " << this->reached.size() << "\n";
                for (unordered_map<string, state *>::iterator it = this->reached.begin(); it != this->reached.end(); it++)
                {
                    // cout << "For Loop: free memory: " << index++ << "\n";
                    delete it->second;
                }

                this->start = new state(startPerm, NULL, 'S', 0);
                // this->reached = vector<state *>(1, this->start);
                this->reached = unordered_map<string, state *>({{this->start->perm, this->start}});

                // reset current state to start state
                this->curState = this->start;
                // cout << "curState print (start)";
                // this->curState->print();
            }
            else
            {
                // get next state from priority queue
                this->curState = this->f.pop();
            }
        }
    }

    void createFinalPosMatrix()
    {
        // unordered_map<char, pair<int, int>> finalPosition({{'0', {this->dimension - 1, this->dimension - 1}},
        //                                                    {'1', {0, 0}},
        //                                                    {'2', {0, 1}},
        //                                                    {'3', {0, 2}},
        //                                                    {}});

        // debug
        //  cout << "this->dimension: " << this->dimension << "\n";

        //  build finalPosition dictionary for manhattan distance
        unordered_map<char, pair<int, int>> fp;
        char indexChar = char(49); //'1' character
        for (int row = 0; row < this->dimension; row++)
        {
            for (int col = 0; col < this->dimension; col++)
            {
                fp[indexChar] = {row, col};
                if (indexChar == char(57))
                    indexChar = char(65);
                else
                    indexChar++;
            }
        }

        fp['0'] = {this->dimension - 1,
                   this->dimension - 1};

        this->finalPosition = fp;

        for (int i = 0; i < this->goalPerm.size(); i++)
        {
            char cur = this->goalPerm[i];
            cout << cur << ": " << this->finalPosition[cur].first << ", " << this->finalPosition[cur].second << "\n";
        }
    }

public:
    SSsearch(string start, string goal, string algo, string heuristic) : start(new state(start, NULL, 'S', 0)), goalPerm(goal), curState(this->start), heuristicType(heuristic), algo(algo), startTime(chrono::high_resolution_clock::now())
    {

        // this->expanded.reserve(this->expanded.max_size()/2);

        // record that start state has been seen
        // this->reached.emplace_back(this->start);
        this->reached[this->start->perm] = this->start;

        // set dimension (e.g. 3x3 or 4x4)
        this->dimension = sqrt(this->start->perm.length());

        this->createFinalPosMatrix();

        // verify start and goal states are in a valid format
        if (!this->validateInput("start"))
            return;
        if (!this->validateInput("goal"))
            return;

        // check start and goal are of same length
        if (this->start->perm.length() != this->goalPerm.length())
            return;

        // call appropriate search algorithm
        if (algo == "BFS")
            this->bfs();
        else if (algo == "DFS")
            this->dfs();
        else if (algo == "IDDFS")
            this->itDdfs();
        else if (algo == "A*")
            this->aStar();
        else if (algo == "IDA*")
            this->iDAStar();
        else
            cout << "algorithm parameter string not recognized.\n";

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

    string getStart()
    {
        return this->start->perm;
    }
    string getGoal()
    {
        return this->goalPerm;
    }
    int getExpanded()
    {
        return this->expandCount;
    }
    string getAlgo()
    {
        return this->algo;
    }
    // chrono::duration<int> getClockTime(){
    double getClockTime()
    {
        return this->durationTime;
    }
};

// tests solutions.  Walks through move a sequence from a start state and identifies its final state.
class test
{
    string start;
    string sequence;
    string currentState;
    int zeroIndex;
    int dimension;

    bool legalMove(char move)
    {
        int zeroCol = this->zeroIndex % this->dimension;
        int zeroRow = this->zeroIndex / this->dimension;

        if (move == 'U')
        {
            if (zeroRow == 0)
                return false;

            return true;
        }
        else if (move == 'D')
        {
            if (zeroRow == this->dimension - 1)
                return false;

            return true;
        }
        else if (move == 'L')
        {
            if (zeroCol == 0)
                return false;

            return true;
        }
        else if (move == 'R')
        {
            if (zeroCol == this->dimension - 1)
                return false;
            return true;
        }
        else
        {
            cout << "Incorrect move character provided.\n";
            return false;
        }
    }

    void makeMove(char &move)
    {
        int targetIndex;

        if (move == 'U')
        {
            targetIndex = this->zeroIndex - this->dimension;
        }
        else if (move == 'D')
        {
            targetIndex = this->zeroIndex + this->dimension;
        }
        else if (move == 'L')
        {
            targetIndex = this->zeroIndex - 1;
        }
        else if (move == 'R')
        {
            targetIndex = this->zeroIndex + 1;
        }
        else
        {
            cout << "Incorrect move character encountered.\n";
        }

        // make swap
        this->currentState[this->zeroIndex] = this->currentState[targetIndex];
        this->currentState[targetIndex] = '0';
        this->zeroIndex = targetIndex;
    }

public:
    test(string start, string sequence) : start(start), sequence(sequence)
    {
        this->dimension = sqrt(this->start.length());
    }
    string outputState()
    {

        if (start.length() % this->dimension != 0)
        {
            cout << "Input start state string is not a squared length.  Cannot calculate output state.\n";
            return "";
        }

        this->zeroIndex = start.find('0');

        this->currentState = this->start;

        for (int i = 0; i < this->sequence.length(); i++)
        {
            if (this->legalMove(this->sequence[i]))
            {
                this->makeMove(sequence[i]);
            }
            else
            {
                cout << "Error: illegal move.  Cannot compute final state.\n";
                cout << "Move#: " << i << "; Move Char: " << this->sequence[i] << "\n";
                return "";
            }
        }

        return this->currentState;
    }
};

class randomWalk
{
    string start;
    string end;
    string currentState;
    int movesCnt;
    int dimension;
    char prevMove;
    int zeroIndex;
    vector<char> legalMoves;
    unordered_map<char, char> prevMoveOpposite;

    void move(char &nextMove)
    {
        int targetIndex;

        if (nextMove == 'U')
        {
            targetIndex = this->zeroIndex - this->dimension;
        }
        else if (nextMove == 'D')
        {
            targetIndex = this->zeroIndex + this->dimension;
        }
        else if (nextMove == 'L')
        {
            targetIndex = this->zeroIndex - 1;
        }
        else if (nextMove == 'R')
        {
            targetIndex = this->zeroIndex + 1;
        }
        else
        {
            cout << "Incorrect move character encountered.\n";
        }

        // make swap
        this->currentState[this->zeroIndex] = this->currentState[targetIndex];
        this->currentState[targetIndex] = '0';
        this->zeroIndex = targetIndex;
    }

    void findLegalMoves()
    {
        this->legalMoves = vector<char>(0);
        int zeroCol = this->zeroIndex % this->dimension;
        int zeroRow = this->zeroIndex / this->dimension;

        if (zeroRow > 0)
        {
            this->legalMoves.emplace_back('U');
        }
        if (zeroRow < this->dimension - 1)
        {
            this->legalMoves.emplace_back('D');
        }
        if (zeroCol > 0)
        {
            this->legalMoves.emplace_back('L');
        }
        if (zeroCol < this->dimension - 1)
        {
            this->legalMoves.emplace_back('R');
        }
    }

    void removeOppositePrevMove()
    {
        char oppositeMove = this->prevMoveOpposite[this->prevMove];
        vector<char>::iterator it = find(this->legalMoves.begin(), this->legalMoves.end(), oppositeMove);

        this->legalMoves.erase(it);
    }

public:
    randomWalk(string &start, int movesCnt) : start(start), currentState(start), movesCnt(movesCnt)
    {
        // acquire dimension size from square root of string permutation size
        this->dimension = sqrt(this->start.size());

        // find zero position, and record
        this->zeroIndex = find(this->start.begin(), this->start.end(), '0') - this->start.begin();

        // dictionary to determine oppositve of previous move
        this->prevMoveOpposite = unordered_map<char, char>({{'U', 'D'},
                                                            {'D', 'U'},
                                                            {'L', 'R'},
                                                            {'R', 'L'}});

        // get legal moves possible for current state
        this->findLegalMoves();

        // seed random number generator
        srand(time(NULL));

        int selection = rand() % this->legalMoves.size();

        char nextMove = this->legalMoves[selection];

        this->move(nextMove);

        this->prevMove = nextMove;

        for (int i = 0; i < this->movesCnt - 1; i++)
        {

            // get set of move choices
            this->findLegalMoves();

            // remove the opposite of previous move so as not to undo it
            this->removeOppositePrevMove();

            selection = rand() % this->legalMoves.size();

            nextMove = this->legalMoves[selection];

            // make move
            this->move(nextMove);

            this->prevMove = nextMove;
        }

        this->end = this->currentState;
    }

    string getEnd()
    {
        return this->end;
    }
};

bool verifySolution(string &start, string &goal, string &solution);

// void printToFile(string &start, string &goal, string &solution, string &fileName);
void printToFile(SSsearch &s, string &fileName);

vector<string> getProblems(string &inputFile);

int main(int argc, char *argv[])
{
    // default variable initilizations
    string startState = "123450786";
    string goalState = "123456780";
    string algo = "IDA*";
    string heuristic = "mHD";

    // run 1
    if (argc > 3)
    {
        startState = argv[1];
        goalState = argv[2];
        algo = argv[3];
    }

    // run 2
    else if (argc == 2)
    {
        string inputFile(argv[1]);
        string outputFile("StateSpaceSearchResults.txt");

        // create output file.  erases previous output files, too.
        ofstream oFile(outputFile);

        // check that file was opened
        if (oFile.is_open())
        {
            // add header to file.
            oFile << "Start State | Algorithm | Nodes Expanded | Clock Time | Solution\n";
            oFile << ":-:|:-:|:-:|:-:|:-:\n";
            oFile.close();
        }
        else
        {
            cout << "Failed to open " << outputFile << ".\n";
        }

        // gets list of start states from an input file
        vector<string> startStates = getProblems(inputFile);

        // run through all the start states and perform a search for goal state from initial state
        for (auto a : startStates)
        {
            // default goal state
            string goalState = "123456780";

            // goal state for 4x4 dimension
            if (a.length() == 16)
            {
                goalState = "123456789ABCDEF0";
            }

            // initialize search object, which automatically starts the search
            SSsearch s(a, goalState, algo, heuristic);

            // get solution.
            string solution = s.getSolution();

            // verify the solution is accurate with the tester class
            if (verifySolution(a, goalState, solution))
            {
                // add result to output file
                printToFile(s, outputFile);
            }
        }

        return 0;
    }

    // run 3
    else
    {

        int moves = 40;
        string rWalkStart = "123456789ABCDEF0";
        randomWalk r(rWalkStart, moves);

        startState = r.getEnd();

        //  Test Start States
        // startState = "120345786";
        //  startState = "203145786";
        //  startState = "123405786";
        //  startState = "123450786";
        //  startState = "023145786";
        // startState = "71A92CE03DB4658F";

        //  Test Goal States
        // goalState = "123456780";
        goalState = "123456789ABCDEF0";

        algo = "A*";
    }

    // used by run 1 and 3

    // create search object, whcih runs search algorithm upon initialization
    SSsearch s(startState, goalState, algo, heuristic);

    // get solution found by search algorithm
    string solution = s.getSolution();

    // verify the solution is accurate
    verifySolution(startState, goalState, solution);

    return 0;
}

bool verifySolution(string &startState, string &goalState, string &solution)
{
    // tester object.  takes start state and move sequence.
    test t1(startState, solution);

    // get final state from tester algorithm.
    string outputState = t1.outputState();

    // output to console to inform user
    cout << "-- Solution Test --\nOutput State: " << outputState << "\n";
    cout << "Goal State:   " << goalState << "\n";
    cout << "Solution ";

    // check if final state provided by move sequence matches the provided goal state
    if (outputState == goalState)
    {
        cout << "Passes\n\n";
        return true;
    }
    else
    {
        cout << "Fails\n\n";
        return false;
    }
}

// void printToFile(string &start, string &goal, string &solution, string &fileName)
void printToFile(SSsearch &s, string &fileName)
{
    ofstream resultsFile(fileName, ofstream::app);

    if (resultsFile.is_open())
    {
        // oFile << "Start State | Algorithm | Nodes Expanded | Clock Time | Solution\n";
        resultsFile << s.getStart() << "|" << s.getAlgo() << "|" << s.getExpanded() << "|" << s.getClockTime() << " seconds|" << s.getSolution() << "\n";

        // github readme table format: goal | start | solution
        // resultsFile << goal << "|";
        // resultsFile << "\"" << start << "\"";
        // resultsFile << "|" << solution << "\n";

        // close file
        resultsFile.close();
    }
    else
    {
        cout << "Results file failed to open.\n";
    }
}

vector<string> getProblems(string &inputFile)
{
    ifstream problems(inputFile);

    vector<string> startStates;

    if (problems.is_open())
    {
        string line;

        while (!problems.eof())
        {
            getline(problems, line);
            if (line[0] == '\"')
            {
                startStates.emplace_back(line.begin() + 1, line.end() - 2);
            }
        }
    }
    else
    {
        cout << "Failed to open problems file.\n";
    }

    return startStates;
}

/* Notes for hard problems:
Make pattern database heuristic its own class, and feed the same object to each search implementation so as to not waste the creation time.  "Ameliorate" the initial cost over multiple problems.
*/