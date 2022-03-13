
function legalMove(zeroIndex, candidateIndex, dimension) {

    let zeroRow = Math.floor(zeroIndex / dimension);
    let zeroCol = zeroIndex % dimension;

    let candidateRow = Math.floor(candidateIndex / dimension);
    let candidateCol = candidateIndex % dimension;

    // console.log("0", zeroRow, zeroCol);
    // console.log(candidateIndex, candidateRow, candidateCol);
    if (zeroRow === candidateRow) {
        return (Math.abs(zeroCol - candidateCol) === 1) ? true : false;
    }
    else if (zeroCol === candidateCol) {
        return (Math.abs(zeroRow - candidateRow) === 1) ? true : false;
    }
    else return false;
}

export {legalMove};