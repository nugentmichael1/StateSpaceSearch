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

- [x] Breadth-First Search    
- [x] Depth-First Search    
- [x] Iterative-Deepening Depth-First Search    
- [x] A* w/ Out-Of-Place, and Manhattan Distance Heuristics    
- [ ] Iterative Deepening A* w/ Out-Of-Place, and Manhattan Distance Heuristics    

### Develop search code with requirements:

- [x] Input puzzle as a two strings, initial state and goal state.    
- [x] Produce the solution as a string of "udlr" representing the move "up", "down", "left", and "right" for the movement of the blank.    
- [x] Produce the number of expanded nodes required to find solution.    

### In addition, develop code to test a solution with requirements:

- [x] Input initial puzzle state as string.
- [x] Input move sequence as string.
- [x] Produce output state from applying move sequence to initial state.

### Provide link to Github repository with all code, and submit final report as PDF with requirements:

- [ ] Provide overview of your code.
- [ ] Provide results for the following problems:

Start State | Solution String | Algorithm
--- | --- | --- 
"160273485" | DDLUURDLLDRRUULDRD | A* w/ Manhattan Distance
"462301587" | RDLURULLDRDLURULDRRULDDR | A* w/ Manhattan Distance
"821574360"| <ul><li>LLURULDRDRULURDLLURDDRULLDRULDRR</li><li>LURULLDRULDDRURULDLURRDLURDD</li></ul> | <ul><li>?</li><li>A* w/ Manhattan Distance</li></ul>
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

15 Puzzle - Easy   
Start State | Solution String | Algorithm
--- | --- | ---
"16235A749C08DEBF"|LUURRDDLDR|Iterative Deepening DFS
"0634217859ABDEFC"|RDLURDLDRRRD|Iterative Deepening DFS
"012456379BC8DAEF"|RRDRDLLDRR|Iterative Deepening DFS
"51246A38097BDEFC"|RULURRDDRD|Iterative Deepening DFS
"12345678D9CFEBA0"|LURDLURDLLLURRDR|Iterative Deepening DFS

15 Puzzle - Difficult
Start State | Solution String | Algorithm
--- | --- | ---
"71A92CE03DB4658F"    
"02348697DF5A1EBC"    
"39A1D0EC7BF86452"    
"EAB480FC19D56237"    
"7DB13C52F46E80A9"   

You can work in groups, but each person needs to submit assignment, and assignment report should list all team member names.  Groups are generally limited to 5.
