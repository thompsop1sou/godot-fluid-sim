# Fluid Simulation in Godot

*Created using the [Godot](https://godotengine.org/) game engine with the [Godot Jolt](https://github.com/godot-jolt/godot-jolt) addon.*

This is a real-time fluid simulation that uses Godot's built-in [RigidBody3D](https://docs.godotengine.org/en/stable/classes/class_rigidbody3d.html) nodes as individual droplets of fluid. The main contribution that this project makes is the inclusion of a "droplet server". This server applies cohesive forces to attract the droplets to each other. This helps to sell the illusion that the fluid has surface tension and viscosity.

https://github.com/thompsop1sou/godot-fluid-sim/assets/114190975/d862cbda-e522-4c06-98f0-8222376e0a62

## The Droplet Server

There are two different versions of the droplet server written in two different programming languages: 
* C#: officially supported by Godot (see [here](https://docs.godotengine.org/en/stable/tutorials/scripting/c_sharp/index.html))
* C++: can be used with [GDExtension](https://docs.godotengine.org/en/stable/tutorials/scripting/gdextension/what_is_gdextension.html) (without needing to edit the source code)

The code for these servers is basically equivalent. In essence, they loop over each pair of droplets and, if any two are close enough to each other, will apply a cohesive force to pull the droplets closer.

> **Note:** This project used to contain a version of the droplet server written in GDScript. This version was removed due to inadequate performance.

## Example Scene

The main scene of this project, `main.tscn`, provides an example of how the droplet servers can be used. In particular, check out the `DropletGenerator` node and its script, `droplet_generator.gd`.

When the scene is run, this node/script will generate droplets at regular time intervals in random positions near the center of the scene. While it's running, you can also change the direction of gravity using the arrow keys and watch the "fluid" move around.

The `DropletGenerator` node has several exported properties that can be configured from the inspector:
* `droplet_server_type`: Whether to use the C# or the C++ server (see above).
* `droplet_object_type`: Whether to create droplets from scenes or use the physics and rendering servers with RIDs.
  * Scenes are easier to work with but slower.
  * The physics and rendering servers are harder to work with but faster.
* `generation_interval`: When generating droplets, how much time should elapse between each successive droplet.
* `droplets_to_generate`: How many total droplets to generate.

## Performance

I ran a few quick tests on my own laptop, which has 8 GB of RAM and an 8-core processor. I was able to spawn the following number of droplets (approximately) before dropping below 60 frames-per-second:

| Server Type | Object Type | Built-In Physics | Jolt Physics   |
|-------------|-------------|------------------|----------------|
| C#          | Scene       | ~1000 droplets   | ~2000 droplets |
| C#          | Server RID  | ~1100 droplets   | ~2400 droplets |
| C++         | Scene       | ~1100 droplets   | ~2700 droplets |
| C++         | Server RID  | ~1200 droplets   | ~3200 droplets |

Of course, take these results with a grain of salt. But in general:
* Using the physics/rendering servers is faster than using scenes. **(Small Impact)**
* The C++ droplet server is faster than the C# droplet server. **(Medium Impact)**
* Jolt physics is faster than the built-in physics. **(Large Impact)**

## Running the Project

To run this project, you will need a copy of Godot version 4.2 with .NET support. This can be downloaded from Godot's official [download page](https://godotengine.org/download/windows/).

Currently, the C++ server has only been compiled for Windows. If you want to run the project on a different system, you will first need to compile the source code.

To do this, first make sure that you have the `godot-cpp` submodule cloned and set up correctly (check out the first few sections in this [doc](https://docs.godotengine.org/en/stable/tutorials/scripting/gdextension/gdextension_cpp_example.html)). Then, run the following from the root directory (replacing `<your_platform>` with `windows`, `linux`, or `macos`):
```
scons platform=<your_platform>
```
