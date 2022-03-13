const Solution = ({solution, eNodes}) => {
  return (
    <>
    <table>
        <caption>Solution</caption>
        <tbody>
            <tr>
                <td>Solution String:</td>
                <td>{solution}</td>
            </tr>
            <tr>
                <td>Expanded Nodes:</td>
                <td>{eNodes}</td>
            </tr>
        </tbody>
    </table>
    </>
  )
}

export default Solution