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

// standard libraries
#include <fstream>  //read batch sequences or start states
#include <iostream> //display answers to console
#include <vector>   //reached, priority queue and stack?
#include <string>   //state permutations

// project headers
#include "../Headers/state.h"
#include "../Headers/pQCompare.h"
#include "../Headers/customPQ.h"
#include "../Headers/frontier.h"
#include "../Headers/sSSearch.h"
#include "../Headers/test.h"
#include "../Headers/randomWalk.h"

bool verifySolution(std::string &start, std::string &goal, std::string &solution);

// void printToFile(string &start, string &goal, string &solution, string &fileName);
void printToFile(SSsearch &s, std::string &fileName);

std::vector<std::string> getProblems(std::string &inputFile);

int main(int argc, char *argv[])
{
    // default variable initilizations
    std::string startState = "123450786";
    std::string goalState = "123456780";
    std::string algo = "IDA*";
    std::string heuristic = "mHD";

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
        std::string inputFile(argv[1]);
        std::string outputFile("../Outputs/StateSpaceSearchResults.md");

        // create output file.  erases previous output files, too.
        std::ofstream oFile(outputFile);

        // check that file was opened
        if (oFile.is_open())
        {
            // add header to file.
            oFile << "Start State | Algorithm | Nodes Expanded | Clock Time | Solution | Solution Length\n";
            oFile << ":-:|:-:|:-:|:-:|:-:|:-:\n";
            oFile.close();
        }
        else
        {
            std::cout << "Failed to open " << outputFile << ".\n";
        }

        // gets list of start states from an input file
        std::vector<std::string> startStates = getProblems(inputFile);

        // run through all the start states and perform a search for goal state from initial state
        for (auto a : startStates)
        {
            // default goal state
            std::string goalState = "123456780";

            // goal state for 4x4 dimension
            if (a.length() == 16)
            {
                goalState = "123456789ABCDEF0";
            }

            // initialize search object, which automatically starts the search
            SSsearch s(a, goalState, algo, heuristic);

            // get solution.
            std::string solution = s.getSolution();

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
        std::string rWalkStart = "123456789ABCDEF0";
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
    std::string solution = s.getSolution();

    // verify the solution is accurate
    verifySolution(startState, goalState, solution);

    return 0;
}

bool verifySolution(std::string &startState, std::string &goalState, std::string &solution)
{
    // tester object.  takes start state and move sequence.
    test t1(startState, solution);

    // get final state from tester algorithm.
    std::string outputState = t1.outputState();

    // output to console to inform user
    std::cout << "-- Solution Test --\nOutput State: " << outputState << "\n";
    std::cout << "Goal State:   " << goalState << "\n";
    std::cout << "Solution ";

    // check if final state provided by move sequence matches the provided goal state
    if (outputState == goalState)
    {
        std::cout << "Passes\n\n";
        return true;
    }
    else
    {
        std::cout << "Fails\n\n";
        return false;
    }
}

// void printToFile(string &start, string &goal, string &solution, string &fileName)
void printToFile(SSsearch &s, std::string &fileName)
{
    std::ofstream resultsFile(fileName, std::ofstream::app);

    if (resultsFile.is_open())
    {
        // oFile << "Start State | Algorithm | Nodes Expanded | Clock Time | Solution\n";
        resultsFile << s.getStart() << "|" << s.getAlgo() << "|" << s.getExpanded() << "|" << s.getClockTime() << " seconds|" << s.getSolution() << "|" << s.getSolution().size() << "\n";

        // close file
        resultsFile.close();
    }
    else
    {
        std::cout << "Results file failed to open.\n";
    }
}

std::vector<std::string> getProblems(std::string &inputFile)
{
    std::ifstream problems(inputFile);

    std::vector<std::string> startStates;

    if (problems.is_open())
    {
        std::string line;

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
        std::cout << "Failed to open problems file.\n";
    }

    return startStates;
}