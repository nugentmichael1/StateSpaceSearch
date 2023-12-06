import React from 'react'

function Instructions() {
    return (
        <div id='Instructions'>
            <h2>Instructions</h2>

            <h3>
                Warning
            </h3>
            <p>
                JavaScript is not powerful enough to handle difficult problems and will hang up your browser.  Use the <a href='https://github.com/nugentmichael1/StateSpaceSearch'>C++ version</a> for anything complex.  This site's main purpose is to visualize the domain.
            </p>

            <h3>
                Start State
            </h3>
            <p>
                Either edit the string in the "Initial Puzzle State" text box, or use your mouse to click on the tiles of the "Puzzle Slider" until a suitable start state is acquired.  You can change the text box's permutation to represent a 4x4 matrix through hexidecimal (ie 123456789ABCDEF0)
            </p>
            <h3>
                Goal State
            </h3>
            <p>
                Ensure the goal state's length matches the initial state's (ie if start state = 2A94B0E715386DFC, then goal state = 123456789ABCDEF0).
            </p>
            <h3>
                Algorithms
            </h3>
            <p>Choose Breadth-First, Depth-First, or Iterative-Deepening Depth-First search.  The A* and Iterative Deepening A* were not implemented on this site, but are in the C++ program.</p>
            <h3>Hueristic</h3>
            <p>Ignore.  Not implemented on this site.</p>
            <h3>Solve</h3>
            <p>If the problem is simple enough the JavaScript can handle it, and the "Solve" button will populate the "Solution String" text box with the moves sequence.  If the problem is not simple, the C++ program can be used to produce a move sequence, which can then be copy-pasted into the "Solution String" text box.  The "Go" button of the "Solution String" textbox will perform the provided steps sequence automatically, and slowly enough to watch.</p>
        </div>

    )
}

export default Instructions

// <table id='warningTable'>
//                 {/* <caption>
//                 Warning
//             </caption> */}
//                 <thead>
//                     <tr>
//                         <td>
//                             Warning!!!
//                         </td>
//                     </tr>
//                 </thead>
//                 <tbody>

//                 </tbody>
//             </table>