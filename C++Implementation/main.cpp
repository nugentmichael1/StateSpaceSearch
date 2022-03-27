/*  State Space Search

*/

#include <fstream>   //read batch sequences or start states
#include <iostream>  //display answers
#include <vector>    //reached
#include <string>    //states
#include <math.h>    //square root
#include <algorithm> //sort
#include <set>       //check uniqueness of characters of start and goal states
#include <queue>     //bfs frontier

using namespace std;

struct state
{
    string const test;
    const string perm;
    state *const parent;
    const char movement;
    const int level;

    state(string perm, state *parent, char movement, int level) : perm(perm), parent(parent), movement(movement), level(level) {}
    state operator=(const state s) { return s; }

    // const string getPerm()
    // {
    //     return this->perm;
    // }
};

class search
{
    vector<state> reached;
    vector<state> frontier;
    int expandCount = 0;
    const state start;
    state curState;
    const string goalPerm;
    int dimension;   // usually 3 or 4.
    string solution; // sequence of movements.

public:
    search(string start, string goal, string algo) : start(state(start, NULL, NULL, 0)), goalPerm(goal)
    {

        // create state object from start state permutation
        // this->start = state(start, NULL, NULL, 0);

        // set current state to start state
        this->curState = this->start;

        // set goal permutation
        // this->goalPerm = goal;

        // record that start state has been seen
        this->reached.emplace_back(this->start);

        // this.frontier.push(this.start);

        // might need to take floor of this
        this->dimension = sqrt(this->start.perm.length());

        cout << "search called\n";

        // verify start and goal states are in a valid format
        if (!this->validateInput("start"))
            return;
        if (!this->validateInput("goal"))
            return;

        // check start and goal are of same length
        if (this->start.perm.length() != this->goalPerm.length())
            return;

        // ensure an algorithm was selected by user
        // let a = document.querySelector('input[type=radio][name=algorithm]:checked');
        // if (!a)
        // {
        //     console.log('no algorithm selected');
        //     alert("Select an algorithm.");
        //     return;
        // }
        // let algo = a.value;
        // console.log(algo);

        // call appropriate search algorithm
        if (algo == "bfs")
            this->bfs();
        else if (algo == "dfs")
            this->dfs();
        else if (algo == "itDdfs")
            this->itDdfs();

        cout << "Reached goal state: " << this->curState.perm << "\n";

        cout << "expandedCount: " << this->expandCount << "\n";

        this->traceBack();

        cout << "Solution: " << this->solution << "\n";
        cout << "frontierSize" << this->frontier.size() << "\n";
        cout << "reached size: " << this->reached.size() << "\n";
    }

    void traceBack()
    {
        // current state should be at goal state when this function is called

        this->solution = string(1, this->curState.movement);
        state *cur = this->curState.parent;

        // case where start state is goal state
        if (!cur)
            return;

        while (cur->parent != NULL)
        {
            // debug: track movement sequence generation
            cout << "cur->movement: " << cur->movement << "\n";
            this->solution += cur->movement;
            cur = cur->parent;
        }

        // reverse the solution string
        reverse(this->solution.begin(), this->solution.end());
    }

    // breadth first search
    void bfs()
    {
        while (true)
        {
            // check to see if in goal state
            if (this->curState.perm == this->goalPerm)
            {
                return;
            }

            // console.log("frontier", this.frontier);

            // else expand current state
            // console.log('expand')
            this->expand();

            this->curState = this->frontier.shift();
            // console.log("curState after expand()", this.curState);
        }
    }

    dfs()
    {
        while (true)
        {
            // check to see if in goal state
            if (this.curState.perm == = this.goal)
            {
                return;
            }

            // console.log("dfs,reached",this.reached);
            // console.log("dfs,frontier",this.frontier);
            // console.log("dfs,curState",this.curState);
            this.expand();

            this.curState = this.frontier.pop();
        }
    }

    itDdfs()
    {

        let depthBound = 1;

        while (true)
        {
            // check to see if in goal state
            if (this.curState.perm == = this.goal)
            {
                return;
            }

            // console.log("itDdfs,reached",this.reached);
            // console.log("itDdfs,frontier",this.frontier);
            // console.log("itDdfs,curState",this.curState);

            // expand current state
            this.expand();

            this.curState = this.frontier.pop();

            // reject nodes that are beyond the depth boundary
            while (this.curState.level >= depthBound &&this.frontier.length != = 0)
            {

                this.curState = this.frontier.pop();
            }
            if (this.frontier.length == = 0)
            {
                // increment depth boundary by 1 and reset everything
                depthBound += 1;

                // should clear reached array of all but the start state
                this.reached = [this.start];

                this.frontier = []; // technically this is already empty at this point.

                this.curState = this.start;
            }
        }
    }

    expand()
    {
        this.expandCount += 1;
        // console.log("expand()",this.curState);

        // order: up, down, left, right

        // find zero (blank) tile index
        this.zeroIndex = this.curState.perm.indexOf(0);
        // console.log("zeroIndex", this.zeroIndex);
        let zeroRow = Math.floor(this.zeroIndex / this.dimension)
                          let zeroCol = this.zeroIndex % this.dimension;

        // console.log("zeroRow", zeroRow)
        // console.log("dimension", this.dimension)
        // up & down
        if (zeroRow > 0)
        {
            // check up

            // find swap target's index
            let swapTargetIndex = this.zeroIndex - this.dimension;

            // check if up movement's resultant new state has already been reached.  if not, add to reached and frontier.
            this.checkReachedAddToFrontier(swapTargetIndex, 'U');
        }

        if (zeroRow < this.dimension - 1)
        {
            // check down

            // find swap target's index
            let swapTargetIndex = this.zeroIndex + this.dimension;

            // this will check if the down move state has already been reached, and add it to reached and frontier, if not.
            this.checkReachedAddToFrontier(swapTargetIndex, 'D');
        }

        // check left and right
        if (zeroCol > 0)
        {
            // check left

            // find swap target's index
            let swapTargetIndex = this.zeroIndex - 1;

            // this will check if the Left move state has already been reached, and add it to reached and frontier, if not.
            this.checkReachedAddToFrontier(swapTargetIndex, 'L');
        }

        if (zeroCol < this.dimension - 1)
        {
            // check right

            // find swap target's index
            let swapTargetIndex = this.zeroIndex + 1;

            // this will check if the right move state has already been reached, and add it to reached and frontier, if not.
            this.checkReachedAddToFrontier(swapTargetIndex, 'R');
        }

        // console.log("reached", this.reached);
        // console.log("frontier", this.frontier)
    }

    checkReachedAddToFrontier(swapTargetIndex, movement)
    {

        // create array copy of curState and perform swap
        let newState = [... this.curState.perm];
        newState[this.zeroIndex] = newState[swapTargetIndex];
        newState[swapTargetIndex] = 0;

        // turn array back to string (javaScript specific issue)
        let newStateStr = newState.join("");
        // console.log("newState", newStateStr);

        // search reached container for new state
        if (!this.reached.find(s = > s.perm == = newStateStr))
        {
            // console.log("new state not found in reached.  adding it to reached and frontier.")
            // add new state to reached and frontier
            let s = new state(newStateStr, this.curState, movement, this.curState.level + 1);
            this.reached.push(s);
            this.frontier.push(s);
            // console.log("newState", s);
        }
    }

    // ensure input states are composed of sequential and unique character
    bool validateInput(string type)
    {

        // get input state
        const string candidateState = (type == "start") ? this->start.perm : this->goalPerm;

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

        // debug: check unique's contents
        cout << "unique:\n";
        for (auto a : unique)
            cout << a << " ";

        // verify every member of the arr was unique
        if (unique.size() == candidateState.size())
        {
            return true;
        }
        else
        {
            cout << "Input values of " << type << " state are not unique.\n";
            return false;
        }
    }
};

int main(int argc, char *argv[])
{

    string goalState("123456780");
    string startState("160273485");
}
