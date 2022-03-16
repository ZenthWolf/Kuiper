# Kuiper

2D Physics Tech Demo

Description:

Elastic collider for 2D polygons. Supports both convex and concave polygons, but does not support polygons with holes.

Asteroids will collide with themselves and will generate randomly at the screen edge. They currently may propagate some distance off screen.

At this time, the generation algorithm will leave the field sparse

Controls:

W - Thruster
A/D - Rotational thrusters
Q/E - Volume Down/Up
S - Zeroes ship velocity (For Debug)


Mouse Wheel Up/Down - Camera Zoom (currently unbounded, utilize with care)

Ship movement slowly decays at 0 thrust.

Shift - Toggle Ship Collisions (Defaults to off)

Arrow Keys - camera panning (Screen pans at edge)


Major Branches:

master - The last version of "SpeculativeColl". Controls as above, see entry about the "SpeculativeColl" branch

SimpleColl - A simple collision protocol. Detects line crosses and implements a toy model response. Linear momentum transfers like direct billiard ball collisions,
and angular momentum is treated incredibly simply, but unphysically. Angular momentum transfer tends to build over time, but overall the effect does not exibit
terrible behavior. This branch does not allow for collisions with the ship, nor does it have background music.

RealisticColl - A naive attempt at a corrective response based on detecting vertices inside collision targets. This produces a point to implement a realistic
impulse model for linear and angular momentum. Problems with the corrective model result in multiple collisions and is unlikely to be revisited, as the collision
criterion (vertex required to be interior to the collision target) proved inadequate. Collisions are resolved more realistically, but the multiple collision create
arbitarily large impulses in common edge cases.

SpeculativeColl - Nearly complete. This branch decomposes collision models into convex primitive elements in order to implement well trusted algorithms (SAT) that
otherwise could not be utilized generally. This method requires a speculative update step in order to find contact point of any collision. Collision algorithms are
currently working in debug mode, but work needed to make release mode work smoothly. In the event this method fails, code falls back into a discrete collision
solver.

Future work:

The "SpeculativeColl" branch follows ideas presented by Erin Catto (Box2D creator), but seeks to recreate the ideas rather than implement the trusted code available. Future work should be to use Box2D directly.
