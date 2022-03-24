class state {
    perm;
    parent;
    movement;
    constructor(perm, parent, movement) {
        this.perm = perm;
        this.parent = parent;
        this.movement = movement;
    }

}

class search {
    reached = []
    frontier = []
    expandCount = 0;
    constructor(start, goal) {
        this.start = new state(start, undefined, undefined);
        this.curState = this.start;
        this.goal = goal;
        this.reached.push(this.start);
        this.frontier.push(this.start);
        //might need to take floor of this
        this.dimension = Math.sqrt(this.start.perm.length);
        console.log("search called");

        //verify start and goal states are in a valid format
        if (!this.validateInput("start")) return;
        if (!this.validateInput("goal")) return;

        //check start and goal are of same length
        if (this.start.perm.length !== this.goal.length) return;

        //ensure an algorithm was selected by user
        let a = document.querySelector('input[type=radio][name=algorithm]:checked');
        if (!a) {
            console.log('no algorithm selected');
            alert("Select an algorithm.");
            return;
        }
        let algo = a.value;
        console.log(algo);

        //call appropriate search algorithm
        if (algo === "bfs") this.bfs();
        else if (algo === "dfs") this.dfs();

        console.log("Reached goal state: ", this.curState);

        console.log("expandedCount", this.expandCount);
        console.log(this.traceBack());

    }

    traceBack() {
        //current state should be at goal state when this function is called
        let solution = [this.curState.movement];
        let cur = this.curState.parent;

        while (cur.parent != undefined) {
            // console.log(cur.movement)
            solution.push(cur.movement);
            cur = cur.parent;
        }

        //reverse the solution string
        solution = solution.reverse();

        this.solution = solution.join("");
    }

    //breadth first search
    bfs() {

        while (true) {
            // check to see if in goal state
            if (this.curState.perm === this.goal) {
                //add to this later
                return;
            }

            // else expand current state
            // console.log('expand')
            this.expand();

            this.curState = this.frontier.shift();
            // console.log("curState after expand()", this.curState);

        }


    }

    expand() {
        this.expandCount += 1;

        //order: up, down, left, right

        //find zero (blank) tile index
        this.zeroIndex = this.curState.perm.indexOf(0);
        // console.log("zeroIndex", this.zeroIndex);
        let zeroRow = Math.floor(this.zeroIndex / this.dimension)
        let zeroCol = this.zeroIndex % this.dimension;

        // console.log("zeroRow", zeroRow)
        // console.log("dimension", this.dimension)
        //up & down
        if (zeroRow > 0) {
            //check up

            //find swap target's index
            let swapTargetIndex = this.zeroIndex - this.dimension;

            //check if up movement's resultant new state has already been reached.  if not, add to reached and frontier.
            this.checkReachedAddToFrontier(swapTargetIndex, 'U');
        }

        if (zeroRow < this.dimension - 1) {
            //check down

            //find swap target's index
            let swapTargetIndex = this.zeroIndex + this.dimension;

            //this will check if the down move state has already been reached, and add it to reached and frontier, if not.
            this.checkReachedAddToFrontier(swapTargetIndex, 'D');

        }

        //check left and right
        if (zeroCol > 0) {
            //check left

            //find swap target's index
            let swapTargetIndex = this.zeroIndex - 1;

            //this will check if the Left move state has already been reached, and add it to reached and frontier, if not.
            this.checkReachedAddToFrontier(swapTargetIndex, 'L');

        }

        if (zeroCol < this.dimension - 1) {
            //check right

            //find swap target's index
            let swapTargetIndex = this.zeroIndex + 1;

            //this will check if the right move state has already been reached, and add it to reached and frontier, if not.
            this.checkReachedAddToFrontier(swapTargetIndex, 'R');
        }

        // console.log("reached", this.reached);
        // console.log("frontier", this.frontier)
    }

    checkReachedAddToFrontier(swapTargetIndex, movement) {

        //create array copy of curState and perform swap
        let newState = [...this.curState.perm];
        newState[this.zeroIndex] = newState[swapTargetIndex];
        newState[swapTargetIndex] = 0;

        //turn array back to string (javaScript specific issue)
        let newStateStr = newState.join("");
        // console.log("newState", newStateStr);

        //search reached container for new state
        if (!this.reached.find(s => s.permStr === newStateStr)) {
            // console.log("new state not found in reached.  adding it to reached and frontier.")
            //add new state to reached and frontier
            let s = new state(newStateStr, this.curState, movement);
            this.reached.push(s);
            this.frontier.push(s);
        }
    }


    //ensure input states are composed of sequential and unique character
    validateInput(type) {

        //get input state
        const candidateState = (type === "start") ? this.start.perm : this.goal;

        /*
        document.getElementById("initialState").value : document.getElementById("goalState").value;
          */


        //check for valid domain (i.e. [0-n))
        //clean
        let arr3 = []
        for (let i = 0; i < candidateState.length; i++) {
            let asciiVal = candidateState.charCodeAt(i);
            // console.log(asciiVal);
            if (asciiVal >= 48 && asciiVal <= 57) {
                //it's a number.  push original
                arr3.push(asciiVal);
            }
            else if (asciiVal >= 65 && asciiVal <= 90) {
                //it's an uppercase letter.  push ascii number
                arr3.push(asciiVal)
            }
            else if (asciiVal >= 97 && asciiVal <= 122) {
                //it's a lower case letter.  make upper and push ascii
                arr3.push(asciiVal - 32)
            }
            else {
                //not a valid character
            }
        }
        arr3.sort();

        let asciiStart = 48;
        for (let i = 0; i < arr3.length && i < 10; i++) {
            if (arr3[i] !== asciiStart) {
                console.log(arr3[i]);
                console.log(asciiStart);
                alert("Input " + type + " state lacks value " + String.fromCharCode(asciiStart) + ".");
                return false;
            }
            asciiStart += 1;
        }
        if (arr3.length >= 11) {
            asciiStart = 65;
            for (let i = 0; i < arr3.length && i < 26; i++) {
                if (arr3[i] !== asciiStart) {
                    alert("Input " + type + " state lacks value " + String.fromCharCode(asciiStart) + ".");
                    return false;
                }
                asciiStart += 1;
            }
        }

        //check for uniqueness
        let unique = new Set(arr3)
        // console.log(unique);
        if (unique.size === candidateState.length) {

            /*
                        (type === "initial") ?
                            setPerm(candidateState) :
                            setGoalState(candidateState);
            */
            return true;
        }
        else {
            alert("Input values of " + type + " state are not unique.")
            return false;
        }
    }
}

export default search