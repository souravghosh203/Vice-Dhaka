# Vice Dhaka

Vice Dhaka is a Dhaka-inspired open-city game prototype. The playable build is a browser game made with Three.js and a React HUD. It uses original procedural scenery and simple models; it does not include Grand Theft Auto assets.

## Run the game

Requirements: Node.js and npm.

From the project directory, run:

```powershell
npm install
npm start
```

Then open <http://127.0.0.1:8000>. The start command builds the React interface and launches the local web server. Keep the terminal open while playing.

## Browser game controls

| Input | Action |
| --- | --- |
| W / S | Walk forward / backward, or accelerate / reverse in a vehicle |
| A / D | Turn on foot, or steer in a vehicle |
| Shift | Run |
| Space | Jump on foot, or brake in a vehicle |
| J / K | Punch / kick nearby characters |
| E | Enter a nearby parked vehicle or exit the current vehicle |
| C | Change camera angle |
| M | Open the full city map |
| H | Honk while driving |
| Esc | Pause or resume |

An on-screen vehicle button appears near a parked vehicle. It changes to **Exit Vehicle** when the vehicle is stopped. Touch controls are available on narrow screens.

## Project layout

- `index.html` — browser game scene, controls, and game logic.
- `ui.jsx` — React HUD source.
- `react-ui.js` — built HUD bundle loaded by the browser game. Rebuild it with `npm run build` after editing `ui.jsx`.
- `three.min.js` and `THREE-LICENSE.txt` — local Three.js runtime and its license.
- `dev-server.cjs` — local development server.
- `vice-dhaka.html` — earlier standalone browser prototype.
- `ViceDhakaUE/` — experimental Unreal Engine 5.9 C++ scaffold.

## Unreal Engine scaffold

The Unreal C++ project is separate from the playable browser build. It requires Unreal Engine 5.9 to open and build. The source has not been compiled or run because that engine is not installed in this environment. See [`ViceDhakaUE/README.md`](ViceDhakaUE/README.md) for its current systems and status.

## Current scope

The browser prototype includes on-foot movement, sprinting, jumping, basic punch and kick actions, nearby pedestrians, and rideable cycle, motorbike, rickshaw, private car, and sports car models. It also includes Dhaka-inspired districts and landmarks, delivery missions, traffic, a city map, weather controls, and procedural sky and streetside trees.

Character, building, and vehicle visuals are lightweight procedural models. Detailed 3D art, animation, interiors, and a streamed real-world Dhaka map remain future work.
