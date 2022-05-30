
#include "../Headers/sSSearch.h"

// opens up possible new states from current state, and adds to frontier if not seen before
void SSsearch::expand()
{
    this->expandCount += 1;

    // order: up, down, left, right

    // find zero (blank) tile index
    int zeroIndex = this->curState->perm.find('0');

    // find coordinate of zero (blank) tile
    int zeroRow = zeroIndex / this->dimension;
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
void SSsearch::expandAStar()
{
    // increment expand count.  may be able to just take size of expanded vector at end.
    this->expandCount += 1;

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
    if (zeroRow > 0 && this->curState->movement != 'D')
    {
        // check up

        // find swap target's index
        int swapTargetIndex = zeroIndex - this->dimension;

        // check if up movement's resultant new state has already been reached.  if not, add to reached and frontier.
        this->checkExpandedAddToFrontier(swapTargetIndex, 'U', zeroIndex);
    }

    if (zeroRow < this->dimension - 1 && this->curState->movement != 'U')
    {
        // check down

        // find swap target's index
        int swapTargetIndex = zeroIndex + this->dimension;

        // this will check if the down move state has already been reached, and add it to reached and frontier, if not.
        this->checkExpandedAddToFrontier(swapTargetIndex, 'D', zeroIndex);
    }

    // check left and right
    if (zeroCol > 0 && this->curState->movement != 'R')
    {
        // check left

        // find swap target's index
        int swapTargetIndex = zeroIndex - 1;

        // this will check if the Left move state has already been reached, and add it to reached and frontier, if not.
        this->checkExpandedAddToFrontier(swapTargetIndex, 'L', zeroIndex);
    }

    if (zeroCol < this->dimension - 1 && this->curState->movement != 'L')
    {
        // check right

        // find swap target's index
        int swapTargetIndex = zeroIndex + 1;

        // this will check if the right move state has already been reached, and add it to reached and frontier, if not.
        this->checkExpandedAddToFrontier(swapTargetIndex, 'R', zeroIndex);
    }
}

// only used by A* (not the same as IDA*).  checks expanded vector instead of reached like the other algorithms.
void SSsearch::checkExpandedAddToFrontier(int swapTargetIndex, char movement, int zeroIndex)
{
    // create array copy of curState
    std::string newStatePerm = this->curState->perm;

    // perform swap so as to represent the new state
    newStatePerm[zeroIndex] = newStatePerm[swapTargetIndex];
    newStatePerm[swapTargetIndex] = '0';

    // search expanded container for new state
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

        // add state to frontier
        this->f.push(s);
    }
}

// determines which heuristic to use based on constructor argument
int SSsearch::heuristic(std::string &perm)
{
    int distance = 0;

    if (this->heuristicType == "oOP")
        return this->oOPDistance(perm);
    else if (this->heuristicType == "mHD")
        return this->manhattanDistance(perm);
    else
    {
        std::cout << "Heuristic type not recognized.";
        return -1;
    }
}

// heuristic: "out of place" - counts the number of tiles that are in a different position than goal state's representation
int SSsearch::oOPDistance(std::string &perm)
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
int SSsearch::manhattanDistance(std::string &perm)
{
    int totalDistance = 0;

    for (int row = 0, index = 0; row < this->dimension; row++)
    {
        for (int col = 0; col < this->dimension; col++)
        {
            if (perm[index] != '0')
            {
                std::pair<int, int> coord = this->finalPosition[perm[index]];
                totalDistance += abs(row - coord.first) + abs(col - coord.second);
            }
            index++;
        }
    }

    return totalDistance;
}

// utility function to condense expand()
void SSsearch::checkReachedAddToFrontier(int swapTargetIndex, char movement, int zeroIndex)
{

    // create array copy of curState and perform swap
    std::string newStatePerm = this->curState->perm;
    newStatePerm[zeroIndex] = newStatePerm[swapTargetIndex];
    newStatePerm[swapTargetIndex] = '0';

    // search reached container for new state
    if (!this->reached.count(newStatePerm))
    {
        // new state not found in reached.

        // add new state to reached and frontier
        state *s = new state(newStatePerm, this->curState, movement, this->curState->level + 1);

        this->reached[newStatePerm] = s;
        this->f.push(s);
    }

    // compare current state's cost so far to reached's
    else if (this->reached[newStatePerm]->costSoFar > this->curState->costSoFar + 1)
    {
        // delete previous state
        delete this->reached[newStatePerm];

        // recreate state with new parent, movement, and level parameters
        state *s = new state(newStatePerm, this->curState, movement, this->curState->level + 1);

        this->reached[newStatePerm] = s;
        this->f.push(s);
    }
}

// ensure input states are composed of sequential and unique characters
bool SSsearch::validateInput(std::string type)
{

    // get input state
    std::string candidateState = (type == "start") ? this->start->perm : this->goalPerm;

    // check for valid domain (i.e. [0-n))
    // clean
    std::vector<char> arr3;
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
            std::cout << "arr3[" << i << "]: " << arr3[i] << "\n";
            std::cout << "curAscii: " << curAscii << "\n";
            std::cout << "Input " << type << " state lacks value " << char(curAscii) << ".\n";
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
                std::cout << "Input " << type << " state lacks value " << char(curAscii) << ".\n";
                return false;
            }
            curAscii += 1;
        }
    }

    // check for uniqueness
    // insert arr3 contents into a set
    std::set<int> unique(arr3.begin(), arr3.end());
    // cout << "unique.size(): " << unique.size() << "\n";

    // verify every member of the arr was unique
    if (unique.size() != candidateState.size())
    {
        std::cout << "Input values of " << type << " state are not unique.\n";
        return false;
    }

    if (!this->determineSolvability(candidateState))
    {
        return false;
    }

    // string passed all 3 tests: sequential, uniqueness, and even inversions,
    std::cout << "Valid " << type << " state.\n";
    return true;
}

// check for solvability through inversions count
bool SSsearch::determineSolvability(std::string &perm)
{

    // get count of inversions
    int inversionCnt = this->countInversions(perm);

    // check for odd or even dimension
    if (this->dimension % 2 == 1)
    {
        // odd

        // check if inversions count is even or odd
        if (inversionCnt % 2 == 1)
        {
            // odd

            // unsolvable
            std::cout << "N: " << this->dimension << " (odd); Inversion Count: " << inversionCnt << " (odd); Puzzle unsolvable.\n";
            return false;
        }
        return true;
    }
    else
    {
        // even

        // check which row blank tile (0) is in
        int zeroIndex = perm.find('0');

        int zeroRow = zeroIndex / this->dimension;

        // cout << "perm: " << perm << "; zeroIndex: " << zeroIndex << "; zeroRow: " << zeroRow << "\n";

        if (zeroRow % 2 == 1)
        {
            // odd

            // odd inversion count fails
            if (inversionCnt % 2 == 1)
            {
                std::cout << "N: " << this->dimension << " (even); Inversion Count: " << inversionCnt << " (odd); Blank Tile's Row: " << zeroRow << " (odd); Puzzle unsolvable.\n";
                return false;
            }
            return true;
        }
        else
        {
            // even

            // even inversion count fails
            if (inversionCnt % 2 == 0)
            {
                std::cout << "N: " << this->dimension << " (even); Inversion Count: " << inversionCnt << " (even); Blank Tile's Row: " << zeroRow << " (even); Puzzle unsolvable.\n";
                return false;
            }
            return true;
        }
    }
}

// count number of inversions
int SSsearch::countInversions(std::string &perm)
{
    std::string noZero = "";
    int i = 0;
    // cout << "perm: " << perm << "\n";
    while (perm[i] != '0')
    {
        // cout << perm[i];
        noZero += perm[i++];
    }
    i++;
    while (i < perm.size())
    {
        noZero += perm[i++];
    }

    int inversionsCnt = 0;
    for (int i = 0; i < noZero.size(); i++)
    {
        int asciiVal = (int)noZero[i];
        for (int j = i + 1; j < noZero.size(); j++)
        {
            if (asciiVal > (int)noZero[j])
            {
                inversionsCnt++;
                // cout << "(" << noZero[i] << ", " << noZero[j] << "), ";
            }
        }
        // cout << "\n";
    }
    return inversionsCnt;
}

// called once an algorithm is run.  builds and returns move sequence from start to goal.
void SSsearch::traceBack()
{
    // current state should be at goal state when this function is called
    this->solution = std::string(1, this->curState->movement);

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

    this->endTime = std::chrono::high_resolution_clock::now();
    this->durationTime = double(std::chrono::duration_cast<std::chrono::milliseconds>(this->endTime - this->startTime).count());
    this->durationTime /= 1000;
}

// Reports results to console.
void SSsearch::reportResults()
{
    std::cout << "Start State: " << this->start->perm << "; Goal state: " << this->goalPerm << "; Solution: " << this->solution << "\n";

    std::cout << "Expanded Count: " << this->expandCount << "; Frontier Size: " << this->f.size() << "; Reached Size: " << this->reached.size() << "\n";
}

// breadth first search
void SSsearch::bfs()
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
void SSsearch::dfs()
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
void SSsearch::itDdfs()
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
            this->reached = std::unordered_map<std::string, state *>({{this->start->perm, this->start}});

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

void SSsearch::aStar()
{
    this->f = frontier("pQueue");

    while (true)
    {

        // check to see if goal state found and taken off frontier
        if (this->curState->perm == this->goalPerm)
        {
            return;
        }

        // explore next states and possibly add to frontier
        this->expandAStar();

        // get next state from priority queue
        this->curState = this->f.pop();

        // make sure state hasn't been expanded already
        while (this->expanded.count(this->curState->perm))
        {
            this->curState = this->f.pop();
        }
    }
}

void SSsearch::iDAStar()
{

    // IDA* uses depth first search, which requires a FILO (stack) frontier
    this->f = frontier("stack");

    // get heuristic h(n) for start state: f(n) = g(n) + h(n)
    this->start->estTtlCost = this->heuristic(this->start->perm);

    // depth boundary is based on start state's h(n): f(n) = g(n) + h(n) = 0 + h(n) = h(n)
    int fMax = this->start->estTtlCost;

    // set minimum for unexpanded states f(n) distances to infinity
    int fMin = INT32_MAX;

    // debug: delete previous fMax progress file
    std::ofstream progress("../Output/Progress.txt");

    // add header to new progress file
    progress << this->start->perm << "\n";
    progress << this->start->estTtlCost << "\n";

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

        // only expand states below the depth boundary
        if (this->curState->estTtlCost <= fMax)
        {

            // explore next states.  possibly add them to frontier.
            this->expand();

            // debug: status
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
            std::ofstream progress("../Outputs/Progress.txt", std::ios_base::app);
            if (progress.is_open())
            {
                progress << "fMax: " << fMax << "\n";
                progress << "States Expanded this iteration: " << statesExpandedCntPerIteration << "\n";
            }
            else
                std::cout << "Failed to open Progress.txt.\n";

            // this will automatically close when the variable loses scope, but doesn't hurt to be explicit?
            progress.close();

            // debug: status
            statesExpandedCntPerIteration = 0;

            // reset minimum rejected node distance to infinite
            fMin = INT32_MAX;

            // record start state attributes to recreate after mass memory freedom
            std::string startPerm = this->start->perm;

            // delete all reached states to free memory
            for (std::unordered_map<std::string, state *>::iterator it = this->reached.begin(); it != this->reached.end(); it++)
            {
                delete it->second;
            }

            // recreate start state
            this->start = new state(startPerm, NULL, 'S', 0);

            // clear reached vector
            this->reached = std::unordered_map<std::string, state *>({{this->start->perm, this->start}});

            // reset current state to start state
            this->curState = this->start;
        }
        else
        {
            // get next state from priority queue
            this->curState = this->f.pop();
        }

        // get heuristic: h(n)
        int hn = this->heuristic(this->curState->perm);

        // set state's g(n) based on level set in expand()
        this->curState->costSoFar = this->curState->level;

        // set estimated total cost: f(n) = g(n) + h(n)
        this->curState->estTtlCost = hn + this->curState->costSoFar;
    }
}

void SSsearch::createFinalPosMatrix()
{

    //  build finalPosition dictionary for manhattan distance
    std::unordered_map<char, std::pair<int, int>> fp;
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
}

SSsearch::SSsearch(std::string start, std::string goal, std::string algo, std::string heuristic) : start(new state(start, NULL, 'S', 0)), goalPerm(goal), curState(this->start), heuristicType(heuristic), algo(algo), startTime(std::chrono::high_resolution_clock::now())
{

    // record that start state has been seen
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
        std::cout << "algorithm parameter string not recognized.\n";

    // get move sequence (aka solution)
    this->traceBack();

    // notify console of results
    this->reportResults();
}

// returns solution (move sequence)
std::string SSsearch::getSolution()
{
    return this->solution;
}

std::string SSsearch::getStart()
{
    return this->start->perm;
}

std::string SSsearch::getGoal()
{
    return this->goalPerm;
}

int SSsearch::getExpanded()
{
    return this->expandCount;
}

std::string SSsearch::getAlgo()
{
    return this->algo;
}

double SSsearch::getClockTime()
{
    return this->durationTime;
}