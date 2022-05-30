
#ifndef RANDOMWALK_H
#define RANDOMWALK_H

#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
#include <math.h>
#include <algorithm>
#include <time.h>   //seed random number generator

class randomWalk
{
    std::string start;
    std::string end;
    std::string currentState;
    int movesCnt;
    int dimension;
    char prevMove;
    int zeroIndex;
    std::vector<char> legalMoves;
    std::unordered_map<char, char> prevMoveOpposite;

    void move(char &nextMove);

    void findLegalMoves();

    void removeOppositePrevMove();

public:
    randomWalk(std::string &start, int movesCnt);

    std::string getEnd();
};

#endif