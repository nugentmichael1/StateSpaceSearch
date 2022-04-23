# State Space Search
Student: Michael Nugent    
Course Number: CSCI 164    
Course Name: Artificial Intelligence    
Term: Spring 2022    
Course Professor: David Ruby    
College: CSU, Fresno    
Description: This is a C++ implementation of various state space search algorithms within a puzzle slider domain.  A JavaScript attempt was made, but proved too slow in the algorithms' infancy.  Perhaps now in maturity, it might handle them better since imlemented heuristics, and achieved optimality decreased run-time. Since JavaScript is 7-10 times slower than C++, its advantage is the visualization of and interaction with the puzzle slider, whereas C++ is better for difficult problems.    
    
## Provided Project Instructions
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

- [x] Provide overview of your code.
- [x] Provide results for the following problems:

### 8 Puzzle
Start State | Algorithm | Nodes Expanded | Clock Time | Solution | Solution Length
:-:|:-:|:-:|:-:|:-:|:-:
160273485|IDA*|735|0.006 seconds|DDLUURDLLDRRUULDRD|18
462301587|IDA*|2139|0.008 seconds|RDLURULLDRDLURULDRRULDDR|24
821574360|IDA*|9863|0.029 seconds|LURULLDRRDLLURDRULULDDRUURDD|28
840156372|IDA*|1340|0.004 seconds|DLLURDLDRRULLURDDRUULDDR|24
530478126|IDA*|4265|0.011 seconds|DLDLURDRUULLDRDRULULDDRR|24
068314257|IDA*|276|0.001 seconds|DRRULLDRDLURDRUULDLDRR|22
453207186|IDA*|161|0.001 seconds|RDLUULDDRUULDRDR|16
128307645|IDA*|330|0.001 seconds|LURDRULLDRDLURRDLURD|20
035684712|IDA*|3301|0.01 seconds|RDLDRURDLULDRUULDDRURULDRD|26
684317025|IDA*|640|0.002 seconds|RURULLDRDLURRULLDRDRUULDDR|26
028514637|IDA*|982|0.003 seconds|DRRDLURULLDRDLURRDLUURDD|24
618273540|IDA*|1612|0.006 seconds|LUULDRRULDDRUULLDRDLURDR|24
042385671|IDA*|2429|0.011 seconds|RRDLLDRRULLURRDLDLURULDDRR|26
420385716|IDA*|736|0.002 seconds|DLLURDDLURRULLDDRURD|20
054672813|IDA*|585|0.002 seconds|DRDLURURDDLUULDRURDLDR|22
314572680|IDA*|1728|0.007 seconds|LULDRUULDRURDLULDRURDLDR|24
637218045|IDA*|271|0.001 seconds|RRUULLDRRDLUULDDRRULDR|22
430621875|IDA*|526|0.003 seconds|LDRULDLDRULURRDDLURD|20
158274036|IDA*|2676|0.014 seconds|URDRUULDDRULDLURDRUULDRD|24
130458726|IDA*|167|0.002 seconds|DLDRUULDRDLURD|14

### 15 Puzzle - Easy     
Start State | Algorithm | Nodes Expanded | Clock Time | Solution | Solution Length
:-:|:-:|:-:|:-:|:-:|:-: 
16235A749C08DEBF|IDA*|10|0 seconds|LUURRDDLDR|10
0634217859ABDEFC|IDA*|10|0 seconds|DRULDDRRRD|10
012456379BC8DAEF|IDA*|10|0 seconds|RRDRDLLDRR|10
51246A38097BDEFC|IDA*|10|0 seconds|RULURRDDRD|10
12345678D9CFEBA0|IDA*|11|0 seconds|ULDLLURRDR|10

### 15 Puzzle - Difficult    
Start State | Algorithm | Nodes Expanded | Clock Time | Solution | Solution Length
:-:|:-:|:-:|:-:|:-:|:-:
71A92CE03DB4658F|IDA*|1044282|6.326 seconds|LLLDDRURURDLDRULLULURRRDDLUULDDRUULDLDDRURULLDRRRD|50
02348697DF5A1EBC|IDA*|281477|1.636 seconds|RDDRDLULDRUURDLULDDRRURULLULDDRRRULDRD|38
39A1D0EC7BF86452|IDA*|44472096|249.976 seconds|DLUURRRDDLLDRRULULDDRUULURDLLURDDDLUURDDRUURULLDDRRULDDR|56
EAB480FC19D56237|IDA*|26420007|215.994 seconds|LDRRUULLDDRDRURULLDRULDDLUURURDDLDRRUULDLUURDDDLUURDRD|54
7DB13C52F46E80A9|IDA*|8370351|78.836 seconds|RULLDRRRULLUULDRRURDDLLLURDDRRULULULDRURRDLLLURRDLDDLURDRR|58

You can work in groups, but each person needs to submit assignment, and assignment report should list all team member names.  Groups are generally limited to 5.
