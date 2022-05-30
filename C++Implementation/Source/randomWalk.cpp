
#include "../Headers/randomWalk.h"

void randomWalk::move(char &nextMove)
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
        std::cout << "Incorrect move character encountered.\n";
    }

    // make swap
    this->currentState[this->zeroIndex] = this->currentState[targetIndex];
    this->currentState[targetIndex] = '0';
    this->zeroIndex = targetIndex;
}

void randomWalk::findLegalMoves()
{
    this->legalMoves = std::vector<char>(0);
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

void randomWalk::removeOppositePrevMove()
{
    char oppositeMove = this->prevMoveOpposite[this->prevMove];
    std::vector<char>::iterator it = find(this->legalMoves.begin(), this->legalMoves.end(), oppositeMove);

    this->legalMoves.erase(it);
}

randomWalk::randomWalk(std::string &start, int movesCnt) : start(start), currentState(start), movesCnt(movesCnt)
{
    // acquire dimension size from square root of string permutation size
    this->dimension = sqrt(this->start.size());

    // find zero position, and record
    this->zeroIndex = find(this->start.begin(), this->start.end(), '0') - this->start.begin();

    // dictionary to determine oppositve of previous move
    this->prevMoveOpposite = std::unordered_map<char, char>({{'U', 'D'},
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

std::string randomWalk::getEnd()
{
    return this->end;
}