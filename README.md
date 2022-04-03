# State Space Search
Student: Michael Nugent    
Course Number: CSCI 164    
Course Name: Artificial Intelligence    
Term: Spring 2022    
Course Professor: David Ruby    
College: CSU, Fresno    

This is a C++ and JavaScript implementation of various state space search algorithms within a puzzle slider domain.  JavaScript is about tens times slower than C++, which means some of the algorithms like DFS will cause the browser to hang up with even minorly complex problems.  The advantage to JavaScript is the visualization of the puzzle slider, and should be used for easier functionality like solution tests.  C++ is better for the actual search algorithms.

Below are the provided instructions of the assignment.    


## Project Instructions
In this assignment you will explore State-Space Search with the 8-Puzzle and 15-Puzzle.  You will need to implement and provide code for the following algorithms:

- [x] Breadth-First Search    
- [x] Depth-First Search    
- [x] Iterative-Deepening Depth-First Search    
- [x] A* w/ Out-Of-Place, and Manhattan Distance Heuristics    
- [x] Iterative Deepening A* w/ Out-Of-Place, and Manhattan Distance Heuristics    

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

8 Puzzle
Start State | Solution String | Algorithm
--- | --- | --- 
"160273485" | DDLUURDLLDRRUULDRD | A* w/ Out-Of-Place
"462301587" | RDLURULLDRDLURULDRRULDDR | A* w/ Out-Of-Place
"821574360"| <ul><li>LLURULDRDRULURDLLURDDRULLDRULDRR</li><li>LURULLDRULDDRURULDLURRDLURDD</li></ul> | <ul><li>?</li><li>A* w/ Out-Of-Place</li></ul>
"840156372"|DLLURDLDRRULLURDDRUULDDR|A* w/ Out-Of-Place
"530478126"|DLDLURDRUULDLURDDRULLDRR|A* w/ Out-Of-Place
"068314257"|DRRULLDRDLURDRUULDLDRR|A* w/ Out-Of-Place
"453207186"|RDLUULDDRUULDRDR|A* w/ Out-Of-Place
"128307645"|LURDRULLDRDLURRDLURD|A* w/ Out-Of-Place
"035684712"|RDLDRURDLULDRUULDDRURULDRD|A* w/ Out-Of-Place
"684317025"|RURULLDDRURULLDDRRUULLDRDR|A* w/ Out-Of-Place
"028514637"|DRRDLURULLDRDLURRDLUURDD|A* w/ Out-Of-Place
"618273540"|LUULDRRULDDRUULLDDRULDRR|A* w/ Out-Of-Place
"042385671"|DDRRULLURDDRUULDLURDRULDDR|A* w/ Out-Of-Place
"420385716"|DLLURDDLURRULLDDRURD|A* w/ Out-Of-Place
"054672813"|DRDLURURDLULDRRDLUURDLDR|A* w/ Out-Of-Place
"314572680"|LULDRUULDRURDLULDRURDLDR|A* w/ Out-Of-Place
"637218045"|RRUULDLURDRDLULDRRULDR|A* w/ Out-Of-Place
"430621875"|DLURDLLDRULURRDDLURD|A* w/ Out-Of-Place
"158274036"|URDRUULDDRULDLURDRUULDRD|A* w/ Out-Of-Place
"130458726"|DLDRUULDDRULDR|A* w/ Out-Of-Place

15 Puzzle - Easy   
Start State | Solution String | Algorithm
--- | --- | ---
"16235A749C08DEBF"|LUURRDDLDR|Iterative Deepening DFS
"0634217859ABDEFC"|<ul><li>RDLURDLDRRRD</li><li>DRULDDRRRD</li></ul>|<ul><li>Iterative Deepening DFS</li><li>A* w/ Out-Of-Place</li></ul>
"012456379BC8DAEF"|RRDRDLLDRR|Iterative Deepening DFS
"51246A38097BDEFC"|RULURRDDRD|Iterative Deepening DFS
"12345678D9CFEBA0"|<ul><li>LURDLURDLLLURRDR</li><li>ULDLLURRDR</li></ul>|<ul><li>Iterative Deepening DFS</li><li>A * w/ Out-Of-Place</li></ul>

15 Puzzle - Difficult
Start State | Solution String | Algorithm
--- | --- | ---
"71A92CE03DB4658F"    
"02348697DF5A1EBC"    
"39A1D0EC7BF86452"    
"EAB480FC19D56237"    
"7DB13C52F46E80A9"   

You can work in groups, but each person needs to submit assignment, and assignment report should list all team member names.  Groups are generally limited to 5.
