# Fluid Simulation in Godot

*Created using the [Godot](https://godotengine.org/) game engine with the [Godot Jolt](https://github.com/godot-jolt/godot-jolt) addon.*

This is a real-time fluid simulation that uses Godot's built-in [RigidBody3D](https://docs.godotengine.org/en/stable/classes/class_rigidbody3d.html) nodes as individual droplets of fluid. The main contribution that this project makes is the inclusion of a "droplet server". This server applies cohesive forces to attract the droplets to each other. This helps to sell the illusion that the fluid has surface tension and viscosity.

https://github.com/petermthompson/fluid-sim/assets/99611620/ccc4b66a-1de3-4652-b184-680452f732d2

## The Droplet Server

There are three different versions of the droplet server written in three different programming languages: 
1. [GDScript](https://docs.godotengine.org/en/stable/tutorials/scripting/gdscript/gdscript_basics.html): a high-level language built for Godot
2. [C#](https://docs.godotengine.org/en/stable/tutorials/scripting/c_sharp/index.html): also officially supported by Godot
3. [C++/GDExtension](https://docs.godotengine.org/en/stable/tutorials/scripting/gdextension/what_is_gdextension.html): a way to use C++ in Godot without recompiling the engine

The code for each of this servers is basically equivalent. In essence, it loops over each pair of droplets and, if any two are close enough together, will apply a force to pull the droplets together. Currently, both the C# and C++ servers use multithreading to boost performance. Multithreading is not yet implemented for the GDScript server.

## Performance

On my own laptop computer with 8 GB of RAM and a 16-core processor, I was able to spawn the following number of droplets before dropping below 60 frames-per-second:
1. GDScript: ~300 droplets
2. C#: ~1900 droplets
3. C++: ~2400 droplets

As expected the C++ server is the most performant, followed closely by the C# server.

## Running the Project

To run this project, you will need a copy of Godot version 4.2. This can be downloaded from Godot's official [download page](https://godotengine.org/download/windows/).

Currently, the C++ server has only been compiled for Windows. If you want to run the project on a different system, you will first need to compile by running the following from the root directory (replacing `<your_platform>` with `windows`, `linux`, or `macos`):
```
scons platform=<your_platform>
```
