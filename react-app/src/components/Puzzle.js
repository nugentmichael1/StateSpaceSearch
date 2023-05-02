

const PuzzleRow = ({ perm, row, swap }) => {

    // console.log(row);
    let tds = [];
    for (let i = 0; i < perm.length; i++) {
        let number = (perm[i] !== "0") ? perm[i] : "";
        let id = row + "-" + i;
        tds.push(<td key={i} id={id} onClick={() => swap(number)}>{number}</td>);
    }

    return (
        <>
            <tr>
                {tds}
            </tr>
        </>
    )
}

const Puzzle = ({ perm, swap }) => {

    // console.log(perm);

    let pSize = perm.length;

    let pDimension = Math.sqrt(pSize);

    var board = [];
    for (let i = 0; i < pDimension; i++) {
        let rowPerm = perm.slice(i * pDimension, (i + 1) * pDimension);
        board.push(<PuzzleRow perm={rowPerm} key={i} row={i} swap={swap} />)
    }

    return (
        <>
            {/* Puzzle size: {pDimension} x {pDimension} */}
            <table className="Puzzle">
                <tbody>
                    {board}
                </tbody>
            </table>
        </>
    )
};

export { Puzzle }