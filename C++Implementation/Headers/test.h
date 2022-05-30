
#ifndef TEST_H
#define TEST_H

#include <string>
#include <iostream>
#include <math.h>

// tests solutions.  Walks through move-sequence and returns final state.
class test
{
    std::string start;
    std::string sequence;
    std::string currentState;
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
            std::cout << "Incorrect move character provided.\n";
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
            std::cout << "Incorrect move character encountered.\n";
        }

        // make swap
        this->currentState[this->zeroIndex] = this->currentState[targetIndex];
        this->currentState[targetIndex] = '0';
        this->zeroIndex = targetIndex;
    }

public:
    test(std::string start, std::string sequence) : start(start), sequence(sequence)
    {
        this->dimension = sqrt(this->start.length());
    }
    std::string outputState()
    {

        if (start.length() % this->dimension != 0)
        {
            std::cout << "Input start state string is not a squared length.  Cannot calculate output state.\n";
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
                std::cout << "Error: illegal move.  Cannot compute final state.\n";
                std::cout << "Move#: " << i << "; Move Char: " << this->sequence[i] << "\n";
                return "";
            }
        }

        return this->currentState;
    }
};

#endif