
function legalMoveUser(zeroIndex, candidateIndex, dimension) {

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

function legalSolutionMove(direction, permutation) {

    const dimension = Math.sqrt(permutation.length)


    const zeroCol = permutation.indexOf(0) % dimension
    const zeroRow = Math.floor(permutation.indexOf(0) / dimension)


    console.log(direction, zeroCol, zeroRow, dimension)

    if (direction === 'U') {
        if (zeroRow === 0)
            return false;

        return true;
    }
    else if (direction === 'D') {
        if (zeroRow === dimension - 1)
            return false;

        return true;
    }
    else if (direction === 'L') {
        if (zeroCol === 0)
            return false;

        return true;
    }
    else if (direction === 'R') {
        if (zeroCol === dimension - 1)
            return false;
        return true;
    }
    else {
        console.log("Incorrect move character provided.", direction);
        return false;
    }
}

//Solution String walkthrough
function makeSolutionSwap(direction, permutation) {

    console.log("permutation", permutation)

    let targetIndex = null;

    const dimension = Math.sqrt(permutation.length)

    const zeroIndex = permutation.indexOf(0)

    if (direction === 'U') {
        targetIndex = zeroIndex - dimension;
    }
    else if (direction === 'D') {
        targetIndex = zeroIndex + dimension;
    }
    else if (direction === 'L') {
        targetIndex = zeroIndex - 1;
    }
    else if (direction === 'R') {
        targetIndex = zeroIndex + 1;
    }
    else {
        console.log("Incorrect move character encountered.", direction)
        return null
    }

    //swap
    const newPermutation = [...permutation]
    newPermutation[zeroIndex] = permutation[targetIndex]
    newPermutation[targetIndex] = permutation[zeroIndex];

    return newPermutation.join("");
}




export { legalMoveUser, legalSolutionMove, makeSolutionSwap };