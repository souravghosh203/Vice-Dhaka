# Vice Dhaka native prototype

This is the Unreal Engine 5.9 C++ prototype beside the browser build. It is intentionally separate so the current browser game stays available while the native project is being built out.

## Current native systems

- Physics-driven rickshaw pawn with a rear shoulder camera and a driver camera.
- Blocking Chaos collision on the rickshaw, traffic cars, ground, and building instances.
- Reversed steering input: **A / left turns right**; **D / right turns left**.
- Instanced Dhaka-inspired city blocks with balconies, market awnings, and rooftop water tanks.
- Traffic vehicles that follow city streets and resolve impacts through physics.
- Mission phases for pickup, delivery, timeout, and rewards.
- Clear/monsoon lighting and fog toggle.
- Native HUD for speed, mission state, time, and taka.

## Controls

- **W / S:** drive and reverse
- **A / D:** steer (intentionally reversed)
- **Space:** brake
- **C:** chase / driver camera
- **R:** clear / monsoon weather

## Build status

The native source targets Unreal Engine 5.9. Epic Games Launcher and MSVC are installed, but Unreal Engine has not been downloaded, so this C++ project has not been compiled or run yet. The city and vehicles currently use Unreal's built-in basic meshes as a collision and gameplay blockout; final Dhaka art, character animation, audio, and streamed world assets still need to be added in the editor.
