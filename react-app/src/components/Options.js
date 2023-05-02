
const Options = ({ perm, goalState, setPerm, setGoalState, solve }) => {


    return (
        <>
            <table className="options">
                <caption>
                    <h2>
                        Options
                    </h2>
                </caption>
                <tbody>
                    <tr>
                        <td>
                            <label htmlFor="initialState">Initial Puzzle State</label>
                        </td>
                        <td>
                            <input type="text" id="initialState" value={perm}
                                onChange={() => setPerm(document.getElementById('initialState').value)}></input>
                        </td>
                        <td rowSpan={2}>
                            <input type="button" onClick={solve} value="Solve"></input>
                        </td>
                    </tr>
                    <tr>
                        <td>
                            <label htmlFor="goalState">Goal Puzzle State</label>
                        </td>
                        <td>
                            <input type="text" id="goalState" value={goalState}
                                onChange={() => setGoalState(document.getElementById("goalState").value)}></input>
                        </td>
                    </tr>
                    <tr className="algoOption">
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
                    <tr className="algoOption">
                        <td>
                            <label htmlFor="dfs">Depth-First Search</label>
                        </td>
                        <td>
                            <input type="radio" name="algorithm" value="dfs" id="dfs"></input>
                        </td>
                    </tr>
                    <tr className="algoOption">
                        <td>
                            <label htmlFor="itDdfs">Iterative-Deepening Depth-First Search</label>
                        </td>
                        <td>
                            <input type="radio" name="algorithm" value="itDdfs" id="itDdfs"></input>
                        </td>
                    </tr>
                    <tr className="algoOption">
                        <td>
                            <label htmlFor="aStar">               A* with a Heuristic
                            </label>
                        </td>
                        <td>
                            <input type="radio" name="algorithm" value="aStar" id="aStar"></input>
                        </td>
                    </tr>
                    <tr className="algoOption">
                        <td>
                            <label htmlFor="iDH">               Iterative Deepening A* with a Heuristic
                            </label>
                        </td>
                        <td>
                            <input type="radio" name="algorithm" value="iDH" id="iDH"></input>
                        </td>
                    </tr>
                    <tr className="huerOption">
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
                    <tr className="huerOption">
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