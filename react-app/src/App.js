import './App.css';
import { Puzzle } from './components/Puzzle';
import Options from './components/Options';
import { useState } from 'react';
import { walkthrough, swap } from './Engine/Mechanics'
import Solution from './components/Solution';
import search from './Engine/Search'


function App() {

  const [perm, setPerm] = useState("123456780")

  const [goalState, setGoalState] = useState(perm);

  const [solution, setSolution] = useState("");

  const [eNodes, setENodes] = useState("");


  //Swap function, but provided state variables
  function swapWrapper(candidate) {
    swap(candidate, perm, setPerm)
  }


  function solve() {
    let s = new search(perm, goalState);
    setENodes(s.expandCount);
    setSolution(s.solution);
  }

  //Walkthrough function, but provided state variables
  function walkthroughWrapper(solutionString) {
    walkthrough(solutionString, perm, setPerm)
  }

  return (
    <>
      <h1>State Space Search</h1>
      <Options perm={perm} goalState={goalState} setPerm={setPerm} setGoalState={setGoalState} solve={solve} />
      <Puzzle perm={perm} setPerm={setPerm} swap={swapWrapper} />
      <Solution solution={solution} setSolution={setSolution} walkthrough={walkthroughWrapper} eNodes={eNodes} />
    </>
  );
}

export default App;
