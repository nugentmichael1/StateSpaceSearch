


const Options = ({ perm, goalState, setPerm, setGoalState }) => {


    function validateState(type) {
        let candidateState = (type === "initial") ? document.getElementById("initialState").value : document.getElementById("goalState").value;
        let size = candidateState.length;
        let unique = new Set(candidateState)
        if (unique.size === candidateState.length) {

            //check for valid range (i.e. )

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