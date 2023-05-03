const Solution = ({ solution, setSolution, walkthrough, eNodes }) => {

  return (
    <>
      <table>
        <caption>Solution</caption>
        <tbody>
          <tr>
            <td>
              <label htmlFor='solutionStringWalkthrough'>
                Solution String:
              </label>
            </td>
            <td>
              <input type='text' id='solutionStringWalkthrough' value={solution} onChange={(e) => { setSolution(e.target.value) }} />
            </td>
            <td><input type="button" value={"Go"} onClick={() => { walkthrough(solution) }} /></td>
          </tr>
          <tr>
            <td>Expanded Nodes:</td>
            <td>{eNodes}</td>
          </tr>
        </tbody>
      </table >
    </>
  )
}

export default Solution