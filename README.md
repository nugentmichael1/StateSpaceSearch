# State Space Search
Student: Michael Nugent    
Course Number: CSCI 164    
Course Name: Artificial Intelligence    
Term: Spring 2022    
Course Professor: David Ruby    
College: CSU, Fresno    

This is a C++ and JavaScript implementation of various state space search algorithms within a puzzle slider domain.  JavaScript is about tens times slower than C++, which means some of the algorithms like DFS will cause the browser to hang up with even minorly complex of problems.  The advantage to JavaScript is the visualization of the puzzle slider, and should be used for easier functionality like solution tests.  C++ is better for the actual search algorithms, especially DFS.

Below were the provided instructions of the assignment.    


## Provided Project Instructions
In this assignment you will explore State-Space Search with the 8-Puzzle and 15-Puzzle.  You will need to implement and provide code for the following algorithms:

- Breadth-First Search    
- Depth-First Search    
- Iterative-Deepening Depth-First Search    
- A* w/ Out-Of-Place, and Manhattan Distance Heuristics    
- Iterative Deepening A* w/ Out-Of-Place, and Manhattan Distance Heuristics    

### Develop search code with requirements:

- Input puzzle as a two strings, initial state and goal state.    
- Produce the solution as a string of "udlr" representing the move "up", "down", "left", and "right" for the movement of the blank.    
- Produce the number of expanded nodes required to find solution.    

### In addition, develop code to test a solution with requirements:

- Input initial puzzle state as string.
- Input move sequence as string.
- Produce output state from applying move sequence to initial state.

### Provide link to Github repository with all code, and submit final report as PDF with requirements:

- Provide overview of your code.
- Provide results for the following problems:

Start State | Solution String
--- | --- 
"160273485" | DDLUURDLLDRRUULDRD
"462301587" | RDLURULLDRDLURULDRRULDDR
"821574360"|
"840156372"|
"530478126"|
"068314257"|
"453207186"|
"128307645"|
"035684712"|
"684317025"|
"028514637"|
"618273540"|
"042385671"|
"420385716"|
"054672813"|
"314572680"|
"637218045"|
"430621875"|
"158274036"|
"130458726"|


EASY:    
"16235A749C08DEBF"    
"0634217859ABDEFC"    
"012456379BC8DAEF"    
"51246A38097BDEFC"    
"12345678D9CFEBA0"   

DIFFICULT:    
"71A92CE03DB4658F"    
"02348697DF5A1EBC"    
"39A1D0EC7BF86452"    
"EAB480FC19D56237"    
"7DB13C52F46E80A9"   

You can work in groups, but each person needs to submit assignment, and assignment report should list all team member names.  Groups are generally limited to 5.
