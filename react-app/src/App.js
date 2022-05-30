import './App.css';
import { Puzzle } from './components/Puzzle';
import Options from './components/Options';
import { useState } from 'react';
import { legalMove } from './Engine/Mechanics'
import Solution from './components/Solution';
import search from './Engine/Search'


function App() {

  const [perm, setPerm] = useState("123456780")

  const [goalState, setGoalState] = useState(perm);

  const [solution, setSolution] = useState();

  const [eNodes, setENodes] = useState();

  const swap = (candidate) => {

    // console.log(perm);

    let zeroIndex = perm.indexOf(0);
    let candidateIndex = perm.indexOf(candidate);

    if (legalMove(zeroIndex, candidateIndex, Math.sqrt(perm.length))) {
      let tmp = [...perm];
      tmp[zeroIndex] = perm[candidateIndex];
      tmp[candidateIndex] = 0;
      // console.log("swap",tmp);
      // console.log("swapPerm",perm)
      setPerm(tmp.join(""));
    }
  }

  function solve() {
    let s = new search(perm, goalState);
    setENodes(s.expandCount);
    setSolution(s.solution);
  }

  return (
    <>
      <h1>State Space Search</h1>
      <Options perm={perm} goalState={goalState} setPerm={setPerm} setGoalState={setGoalState} solve={solve} />
      <Puzzle perm={perm} setPerm={setPerm} swap={swap} />
      <Solution solution={solution} eNodes={eNodes} />
    </>
  );
}

export default App;
