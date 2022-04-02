/*  State Space Search
Author/Student: Michael Nugent
Course: CSCI 164 - Artificial Intelligence
Professor: David Ruby
Semester: Spring 2022
School: CSU, Fresno

Program Description: This is a C++ implementation of State Space Search algorithms within a Tile Slider Puzzle domain (8, and 15).  The implemented algorithms are three basics: breadth first search (bfs), depth first search (dfs), and iterative deepening dfs; and two advanced: A* with hueristics, and iterative deepening dfs with hueristics.

Compile: g++ main.cpp -o main.o
Run: ./main.o <start state> <goal state> <algorithm>
Run example: ./main.o "123450786" "123456780" "bfs"
Run default: If no parameters are provided, a set of default states and algorithm are used, which can be modified within the driver (main function) of main.cpp.
*/

#include <fstream>   //read batch sequences or start states
#include <iostream>  //display answers
#include <vector>    //reached
#include <string>    //states
#include <math.h>    //square root
#include <algorithm> //sort
#include <set>       //check uniqueness of characters of start and goal states
#include <queue>     //bfs frontier, priority queue A*
#include <stack>     //dfs, iddfs

using namespace std;

struct state
{
    string perm;
    state *parent;
    char movement;
    int level = 0;
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

    // bool operator<(state &rhs)
    // {
    //     return this->estTtlCost < rhs.estTtlCost;
    // }
};

class pQCompare
{
public:
    bool operator()(state *lhs, state *rhs)
    {
        return lhs->estTtlCost > rhs->estTtlCost;
    }
};

class frontier
{
    queue<state *> q;
    stack<state *> stk;
    priority_queue<state *, vector<state *>, pQCompare> pQ;
    string type;

public:
    frontier() {}
    frontier(string type) : type(type) {}
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
            // cout << "Pushed onto pQueue\n";
        }
    }
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
            // double check top is correct
            state *s = this->pQ.top();
            this->pQ.pop(); // need to create the comparison function
            // cout << "Popped off pQueue\n";
            return s;
        }
        else
        {
            cout << "pop() failed to find type";
            return NULL;
        }
    }
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
};

class SSsearch
{
    vector<state *> reached;
    vector<state *> expanded; // only used for A* algorithms, replaces reached
    // vector<state> frontier;
    frontier f;
    unsigned int expandCount = 0;
    state *start;
    state *curState;
    string goalPerm;
    int dimension;   // usually 3 or 4.
    string solution; // sequence of movements.
    string heuristicType = "oOP";

    // opens up possible new states from current state, and adds to frontier if not seen before
    void expand()
    {
        this->expandCount += 1;

        // order: up, down, left, right

        // debug: itDdfs issue
        // if (this->curState->perm == "123045786")
        //     cout << "\nExpanding 123045786\n\n";

        // find zero (blank) tile index
        int zeroIndex = this->curState->perm.find('0');
        // console.log("zeroIndex", this.zeroIndex);
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

            // debug:
            // cout << "check right: curState \n";
            // this->curState->print();

            // find swap target's index
            int swapTargetIndex = zeroIndex + 1;

            // this will check if the right move state has already been reached, and add it to reached and frontier, if not.
            this->checkReachedAddToFrontier(swapTargetIndex, 'R', zeroIndex);
        }

        // console.log("reached", this.reached);
        // console.log("frontier", this.frontier)
    }

    void expandAStar()
    {
        // increment expand count.  may be able to just take size of expanded vector at end.
        this->expandCount += 1;

        // debug:
        cout << "expandAStar()";
        this->curState->print();

        // premptively add curState to expanded tracker
        this->expanded.emplace_back(this->curState);

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

        // console.log("reached", this.reached);
        // console.log("frontier", this.frontier)
    }

    void checkExpandedAddToFrontier(int swapTargetIndex, char movement, int zeroIndex)
    {
        // create array copy of curState and perform swap
        string newStatePerm = this->curState->perm;
        newStatePerm[zeroIndex] = newStatePerm[swapTargetIndex];
        newStatePerm[swapTargetIndex] = '0';

        // search expanded container for new state
        vector<state *>::iterator it = find_if(this->expanded.begin(), this->expanded.end(), [newStatePerm](state *s)
                                               { return (s->perm == newStatePerm); });
        if (it == this->expanded.end())
        {
            // new state not found in expanded.  add to frontier and expanded.

            // estimated cost from new state to goal state based on heuristic
            int heuristicCost = heuristic(newStatePerm);

            // cost so far to new state
            int newCostSoFar = this->curState->costSoFar + 1;

            // total estimated cost: f(n) = g(n) + h(n) = newCostSoFar + heuristicCost
            int ttlEstCost = newCostSoFar + heuristicCost;

            // add new state to expanded and frontier
            state *s = new state(newStatePerm, this->curState, movement, newCostSoFar, ttlEstCost);

            this->expanded.emplace_back(s);
            this->f.push(s);
        }
    }

    int heuristic(string &perm)
    {
        int distance = 0;

        if (this->heuristicType == "oOP")
            return this->oOPDistance(perm);
        else if (this->heuristicType == "mHD")
            return this->manhattanDistance(perm);
        else
        {
            cout << "Heuristic type not recognized.";
            return -1;
        }
    }

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

            // if (i < targetIndex)
            // {
            //     // first find how many tiles down it must travel
            // }
            // else if (i > targetIndex)
            // {
            // }
            // else if (i == targetIndex)
            //     totalDistance += 0;
            // else
            // {
            //     cout << "Something wrong in manhattan distance calculation.  Current Index and Target Index have no relationship.\n";
            //     return -1;
            // }
        }

        return totalDistance;
    }

    // utility function to clean up expand()
    void checkReachedAddToFrontier(int swapTargetIndex, char movement, int zeroIndex)
    {

        // create array copy of curState and perform swap
        string newStatePerm = this->curState->perm;
        newStatePerm[zeroIndex] = newStatePerm[swapTargetIndex];
        newStatePerm[swapTargetIndex] = '0';

        // search reached container for new state
        vector<state *>::iterator it = find_if(this->reached.begin(), this->reached.end(), [newStatePerm](state *s)
                                               { return (s->perm == newStatePerm); });
        if (it == this->reached.end())
        {
            // new state not found in reached.

            // debug: curState pointer verification
            //  cout << "this->curState: " << this->curState << "\n";
            //  cout << "curState: " << curState << "\n";

            // add new state to reached and frontier
            state *s = new state(newStatePerm, this->curState, movement, this->curState->level + 1);
            this->reached.emplace_back(s);
            this->f.push(s);

            // debug: check new state properties
            // cout << "New State pushed\n";
            // s->print();
        }
        // debug: ensure reached states are disregarded
        //  else{
        //      cout << "Found state in reached.  Skip.\n";
        //  }
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
            cout << "asciiVal: " << asciiVal << "\n";

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
        cout << "unique.size(): " << unique.size() << "\n";

        // debug: check unique's contents
        // cout << "unique:\n";
        // for (auto a : unique)
        //    cout << a << " - ";

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

        // debug:
        // cout << "Inside traceBack()\n";
        // cout << "this->curState->print()--\n";
        this->curState->print();

        // current state should be at goal state when this function is called
        this->solution = string(1, this->curState->movement);
        state *cur = this->curState->parent;

        // case where start state is goal state
        if (!cur)
            return;

        // debug:
        // int i = 0;

        // while (cur->parent != NULL && i < 10)
        while (cur->parent != NULL)
        {
            // debug
            // cout << "While Loop: cur\n";
            cur->print();

            // debug: track movement sequence generation
            // cout << "cur->movement: " << cur->movement << "\n";
            this->solution += cur->movement;
            cur = cur->parent;
            // debug:
            // i++;
        }

        // reverse the solution string
        reverse(this->solution.begin(), this->solution.end());
    }

public:
    SSsearch(string start, string goal, string algo) : start(new state(start, NULL, 'S', 0)), goalPerm(goal), curState(this->start)
    {

        // record that start state has been seen
        this->reached.emplace_back(this->start);

        // this.frontier.push(this.start);

        // might need to take floor of this
        this->dimension = sqrt(this->start->perm.length());

        cout << "search called\n";

        // verify start and goal states are in a valid format
        if (!this->validateInput("start"))
            return;
        if (!this->validateInput("goal"))
            return;

        // debug:
        // cout << "made it past valideInput() checks\n";

        // check start and goal are of same length
        if (this->start->perm.length() != this->goalPerm.length())
            return;

        // debug:
        // cout << "Start state and goal states are the same length\n";

        // call appropriate search algorithm
        if (algo == "bfs")
            this->bfs();
        else if (algo == "dfs")
            this->dfs();
        else if (algo == "itDdfs")
            this->itDdfs();
        else if (algo == "a*")
            this->aStar();
        else if (algo == "a*ID")
            this->aStarItDeep();
        else
            cout << "algorithm parameter string not recognized.\n";

        cout
            << "Reached goal state: " << this->curState->perm << "\n";

        this->traceBack();

        cout << "Expanded Count: " << this->expandCount << "\n";
        cout << "Solution: " << this->solution << "\n";
        cout << "Frontier Size: " << this->f.size() << "\n";
        cout << "Reached Size: " << this->reached.size() << "\n";
    }

    void setHeuristicOOP()
    {
        this->heuristicType = "oOP";
    }

    void setHeuristicMHD()
    {
        this->heuristicType = "mHD";
    }

    // returns solution (move sequence)
    string getSolution()
    {
        return this->solution;
    }

    // breadth first search
    void bfs()
    {
        // debug:
        // cout << "Inside bfs()\n";

        this->f = frontier("queue");

        while (true)
        {
            // debug:
            // cout << "About to check for goal state\n";
            // check to see if in goal state
            // cout << "curState->perm == this->goalPerm : " << this->curState->perm << " == " << this->goalPerm << "\n";
            if (this->curState->perm == this->goalPerm)
            {
                return;
            }

            // debug
            // cout << "Inside while loop of bfs()\n";

            // console.log("frontier", this.frontier);

            // else expand current state
            // console.log('expand')
            this->expand();

            // debug
            //  cout << "Finished expand()\n";
            //  cout << "frontier size: " << f.size() << "\n";
            this->curState = this->f.pop();

            // debug: check curState
            // cout << "curState from latest f.pop()\n";
            // this->curState->print();
        }
        // debug:
        // cout << "Left while loop of bfs()\n";
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

        int depthBound = 1;

        this->f = frontier("stack");

        while (true)
        {
            // debug
            // cout << "curState->perm == this->goalPerm : " << this->curState->perm << " == " << this->goalPerm << "\n";

            // check to see if in goal state
            if (this->curState->perm == this->goalPerm)
            {
                return;
            }

            // check curState for appropriate depth before expansion.
            if (this->curState->level < depthBound)
            { // expand current state
                this->expand();
            }

            // ensure frontier has states
            if (this->f.size() == 0)
            {

                //  increment depth boundary by 1 and reset everything
                depthBound += 1;

                // debug:
                cout << "Increased depth boundary to: " << depthBound << "\n";

                // debug: limit depth for analysis purposes
                //  if (depthBound > 10)
                //      return;

                // should clear reached array of all but the start state
                this->reached = vector<state *>(1, this->start);

                this->f = frontier("stack"); // technically this was already empty by this point.

                this->curState = this->start;
            }
            else
            {
                this->curState = this->f.pop();
            }
        }
    }

    void aStar()
    {
        this->f = frontier("pQueue");
        // int i = 0;
        while (true)
        {
            // check to see if goal state found and taken off frontier
            if (this->curState->perm == this->goalPerm)
            {
                return;
            }

            // explore next states and possibly add to frontier
            this->expandAStar();

            this->curState = this->f.pop();

            // debug
            // cout << "curState estTtlCost: " << this->curState->estTtlCost << "\n";
            // i++;
        }
    }

    void aStarItDeep()
    {
    }
};

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
        // debug:
        //  cout << "zeroIndex: " << zeroIndex << "\n";

        this->currentState = this->start;

        for (int i = 0; i < this->sequence.length(); i++)
        {
            if (this->legalMove(this->sequence[i]))
            {
                this->makeMove(sequence[i]);

                // debug
                // cout << "State @ i=" << i << ": " << this->currentState << "\n";
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

int main(int argc, char *argv[])
{
    string startState;
    string goalState;
    string algo;

    if (argc > 3)
    {
        startState = argv[1];
        goalState = argv[2];
        algo = argv[3];
    }
    else
    {
        // startState = "120345786";
        // startState = "203145786";
        // startState = "123405786";
        // startState = "123450786";
        // startState = "023145786";

        // assignment start states
        //  startState = "160273485";
        //  startState = "462301587";
        startState = "821574360";
        //  startState = "840156372";
        // startState = "530478126";
        // startState = "068314257";
        // startState = "453207186";
        // startState = "128307645";
        // startState = "035684712";
        // startState = "684317025";
        // startState = "028514637";
        // startState = "618273540";
        // startState = "042385671";
        // startState = "";
        // startState = "";
        // startState = "";
        // startState = "";
        // startState = "";
        // startState = "";
        // startState = "";

        goalState = "123456780";
        // goalState = "123456789ABCDEF0";

        algo = "a*";
    }

    SSsearch s(startState, goalState, algo);
    // s.setHeuristicOOP();
    s.setHeuristicMHD();
    string solution = s.getSolution();

    test t1(startState, solution);
    string outputState = t1.outputState();
    cout << "-- Solution Test --\nOutput State: " << outputState << "\n";
    if (outputState == goalState)
        cout << "is equal to\n";
    else
        cout << "is not equal to\n";
    cout << "Goal State:   " << goalState << "\n";
}
