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

master - The last version of "CollisionManager". Controls as above, see entry about the "CollisionManager" branch

AlienObject - Branch testing 3-D rendering and lighting for a single background object.
