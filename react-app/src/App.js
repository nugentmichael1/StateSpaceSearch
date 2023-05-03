import './App.css';
import { Puzzle } from './components/Puzzle';
import Options from './components/Options';
import { useState } from 'react';
import { legalMoveUser, legalSolutionMove, makeSolutionSwap } from './Engine/Mechanics'
import Solution from './components/Solution';
import search from './Engine/Search'


function App() {

  const [perm, setPerm] = useState("123456780")

  const [goalState, setGoalState] = useState(perm);

  const [solution, setSolution] = useState("");

  const [eNodes, setENodes] = useState();

  const swap = (candidate) => {

    let zeroIndex = perm.indexOf(0);
    let candidateIndex = perm.indexOf(candidate);

    if (legalMoveUser(zeroIndex, candidateIndex, Math.sqrt(perm.length))) {
      let tmp = [...perm];
      tmp[zeroIndex] = perm[candidateIndex];
      tmp[candidateIndex] = 0;

      //convert array to string
      setPerm(tmp.join(""));
    }
  }

  function solve() {
    let s = new search(perm, goalState);
    setENodes(s.expandCount);
    setSolution(s.solution);
  }

  function walkthrough(solutionString) {

    let permutation = perm, i = 0;

    const interval = setInterval(() => {

      //Guard
      if (i === solutionString.length) {
        clearInterval(interval)
        return
      }

      if (legalSolutionMove(solutionString[i], permutation)) {

        const newPermutation = makeSolutionSwap(solutionString[i], permutation)

        permutation = newPermutation

        setPerm(newPermutation)
      }
      else {
        console.log("Illegal move made in walkthrough.", solutionString[i])
        clearInterval(interval)
        return
      }

      i++;

    }, 500)
  }

  return (
    <>
      <h1>State Space Search</h1>
      <Options perm={perm} goalState={goalState} setPerm={setPerm} setGoalState={setGoalState} solve={solve} />
      <Puzzle perm={perm} setPerm={setPerm} swap={swap} />
      <Solution solution={solution} setSolution={setSolution} walkthrough={walkthrough} eNodes={eNodes} />
    </>
  );
}

export default App;
