
#ifndef SSSEARCH_H
#define SSSEARCH_H

#include <unordered_map> //hash table for faster lookup on expanded and reached
#include <chrono>        //clock time
#include <set>           //check uniqueness of characters of start and goal states
#include <algorithm>     //sort
#include <fstream>
#include <math.h>

#include "state.h"
#include "frontier.h"

class SSsearch
{
    // vector<state *> reached; // tracks states seen already to avoid unnecessary expansions and memory use
    std::unordered_map<std::string, state *> reached; // replace vector with hash table for fast lookup (constant)
    // vector<state *> expanded;               // only used for A* algorithms, replaces reached
    std::unordered_map<std::string, state *> expanded; // replace vector with hash table for fast lookup
    frontier f;                                        // frontier object.  based on algorithm, composed of a stack, queue, or priority queue.
    unsigned int expandCount = 0;                      // keep track of states expanded
    state *start;                                      // pointer to start state
    state *curState;                                   // pointer to current state.
    std::string goalPerm;                              // goal state representation
    int dimension;                                     // usually 3 or 4.  sqrt(permutation length)
    std::string solution;                              // sequence of movements.
    std::string heuristicType = "oOP";                 // default heuristic
    std::string algo;                                  // algorithm used
    std::chrono::high_resolution_clock::time_point startTime;
    std::chrono::high_resolution_clock::time_point endTime;
    // chrono::duration<int> durationTime;
    double durationTime;
    std::unordered_map<char, std::pair<int, int>> finalPosition;

    // opens up possible new states from current state, and adds to frontier if not seen before
    void expand();

    // uses heuristic to find optimal solution
    void expandAStar();

    // only used by A* (not the same as IDA*).  checks expanded vector instead of reached like the other algorithms.
    void checkExpandedAddToFrontier(int swapTargetIndex, char movement, int zeroIndex);

    // determines which heuristic to use based on constructor argument
    int heuristic(std::string &perm);

    // heuristic: "out of place" - counts the number of tiles that are in a different position than goal state's representation
    int oOPDistance(std::string &perm);

    // heuristic.  calculates distances of each tile from final state location and sums all together.
    int manhattanDistance(std::string &perm);

    // utility function to condense expand()
    void checkReachedAddToFrontier(int swapTargetIndex, char movement, int zeroIndex);

    // ensure input states are composed of sequential and unique characters
    bool validateInput(std::string type);

    // check for solvability through inversions count
    bool determineSolvability(std::string &perm);

    // count number of inversions
    int countInversions(std::string &perm);

    // called once an algorithm is run.  builds and returns move sequence from start to goal.
    void traceBack();

    // Reports results to console.
    void reportResults();

    // breadth first search
    void bfs();

    // depth first search
    void dfs();

    // iterative deepening depth first search
    void itDdfs();

    void aStar();

    void iDAStar();

    void createFinalPosMatrix();

public:
    SSsearch(std::string start, std::string goal, std::string algo, std::string heuristic);

    // returns solution (move sequence)
    std::string getSolution();

    std::string getStart();

    std::string getGoal();

    int getExpanded();

    std::string getAlgo();

    // chrono::duration<int> getClockTime(){
    double getClockTime();
};

#endif