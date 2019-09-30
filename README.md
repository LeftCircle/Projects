This program uses pygame to create a simple physics engine.
The code is written in python, and creates nodes that interact with physical forces.
The nodes can be affected by spring forces, gravity, drag, and collisions. The nodes
can collide with the edges of the boundary and with other nodes. This is a relativley
basic collision system, as the nodes are simply pushed such that they no longer overlap.
In doing so, Kinetic and potential energy are not necessarily conserved.

The spring forces between two nodes are calculated using RK4. The spring forces are extremely
accurate (error O(h^5)), when there are only two particles being linked together. However, the
forces on nodes that have multiple links are not being calculated correctly. Currently, the spring
portion of the code calculates and updates the position for one spring, then moves onto the next,
updating the positions of the particles each time. Instead, there should be a temporary variable
which stores the physical effects of all springs, then updates individual nodes based off of the
effects of all springs at once, instead of one spring at a time.

Regardless of the physics errors, this is a decent implimentation of a physics engine for real time
applications. The objects still appear to be acting according to physics, other than the particles
that are not effected by gravity.