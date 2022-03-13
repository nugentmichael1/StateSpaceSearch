


const Options = ({ perm, goalState, setPerm, setGoalState }) => {


    function validateState(type) {

        //get input state
        const candidateState = (type === "initial") ? document.getElementById("initialState").value : document.getElementById("goalState").value;

        //check for valid domain (i.e. [0-n))
        //clean
        let arr3 = []
        for (let i = 0; i < candidateState.length; i++) {
            let asciiVal = candidateState.charCodeAt(i);
            console.log(asciiVal);
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
                return;
            }
            asciiStart += 1;
        }
        if (arr3.length >= 11) {
            asciiStart = 65;
            for (let i = 0; i < arr3.length && i < 26; i++) {
                if (arr3[i] !== asciiStart) {
                    alert("Input " + type + " state lacks value " + String.fromCharCode(asciiStart) + ".");
                    return;
                }
                asciiStart += 1;
            }
        }

        //check for uniqueness
        let unique = new Set(arr3)
        console.log(unique);
        if (unique.size === candidateState.length) {



            (type === "initial") ?
                setPerm(candidateState) :
                setGoalState(candidateState);
        }
        else {
            alert("Input values of " + type + " state are not unique.")
            return false;
        }
    }

    return (
        <>
            <table className="options">
                <caption>Options</caption>
                <tbody>
                    <tr>
                        <td>
                            <label htmlFor="initialState">Initial Puzzle State</label>
                        </td>
                        <td>
                            <input type="text" id="initialState" value={perm}
                                onChange={() => validateState("initial")}></input>
                        </td>
                    </tr>
                    <tr>
                        <td>
                            <label htmlFor="goalState">Goal Puzzle State</label>
                        </td>
                        <td>
                            <input type="text" id="goalState" value={goalState}
                                onChange={() => validateState("goal")}></input>
                        </td>
                    </tr>
                    <tr>
                        <td rowSpan="5">
                            Algorithm
                        </td>
                        <td>
                            <label htmlFor="bfs">Breadth-First Search</label>
                        </td>
                        <td>
                            <input type="radio" name="algorithm" value="bfs" id="bfs"></input>
                        </td>
                    </tr>
                    <tr>
                        <td>
                            <label htmlFor="dfs">Depth-First Search</label>
                        </td>
                        <td>
                            <input type="radio" name="algorithm" value="dfs" id="dfs"></input>
                        </td>
                    </tr>
                    <tr>
                        <td>
                            <label htmlFor="itDeep">Iterative-Deepening Depth-First Search</label>
                        </td>
                        <td>
                            <input type="radio" name="algorithm" value="itDeep" id="itDeep"></input>
                        </td>
                    </tr>
                    <tr>
                        <td>
                            <label htmlFor="aStar">               A* with a Heuristic
                            </label>
                        </td>
                        <td>
                            <input type="radio" name="algorithm" value="aStar" id="aStar"></input>
                        </td>
                    </tr>
                    <tr>
                        <td>
                            <label htmlFor="iDH">               Iterative Deepening A* with a Heuristic
                            </label>
                        </td>
                        <td>
                            <input type="radio" name="algorithm" value="iDH" id="iDH"></input>
                        </td>
                    </tr>
                    <tr >
                        <td rowSpan="2">
                            Hueristic
                        </td>
                        <td>
                            <label htmlFor="oop">Out-Of-Place</label>
                        </td>
                        <td>
                            <input type="radio" name="heuristic" value="oop" id="oop"></input>
                        </td>
                    </tr>
                    <tr>
                        <td>
                            <label htmlFor="man">Manhattan Distance</label>
                        </td>
                        <td>
                            <input type="radio" name="heuristic" value="man" id="man"></input>
                        </td>
                    </tr>
                </tbody>
            </table>


        </>
    )
}

export default Options