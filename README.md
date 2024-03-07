# Fluid Simulation in Godot

*Created using the [Godot](https://godotengine.org/) game engine with the [Godot Jolt](https://github.com/godot-jolt/godot-jolt) addon.*

This is a real-time fluid simulation that uses Godot's built-in [RigidBody3D](https://docs.godotengine.org/en/stable/classes/class_rigidbody3d.html) nodes as individual droplets of fluid. The main contribution that this project makes is the inclusion of a "droplet server". This server applies cohesive forces to attract the droplets to each other. This helps to sell the illusion that the fluid has surface tension and viscosity.

## The Main Scene

This project provides an example of how the droplet server could be used in the scene `main.tscn`. In particular, check out the script for generating droplets: `droplet_generator.gd`. When run, this scene spawns a configurable number of droplets in an enclosed room. You can then change the direction of gravity using the arrow keys and watch the "fluid" move around.

https://github.com/thompsop1sou/godot-fluid-sim/assets/114190975/d862cbda-e522-4c06-98f0-8222376e0a62

## The Droplet Server

There are two different versions of the droplet server written in two different programming languages: 
* C#: officially supported by Godot (see [here](https://docs.godotengine.org/en/stable/tutorials/scripting/c_sharp/index.html))
* C++: can be used with [GDExtension](https://docs.godotengine.org/en/stable/tutorials/scripting/gdextension/what_is_gdextension.html) (without needing to edit the source code)

The code for these servers is basically equivalent. In essence, it loops over each pair of droplets and, if any two are close enough together, will apply a force to pull the droplets together.

> **Note:** This project used to contain a version of the droplet server written in GDScript. This version was removed due to inadequate performance.

## Performance

On my own laptop computer with 8 GB of RAM and a 16-core processor, I was able to spawn the following number of droplets before dropping below 60 frames-per-second:
* C#: ~1900 droplets
* C++: ~2400 droplets

## Running the Project

To run this project, you will need a copy of Godot version 4.2 with .NET support. This can be downloaded from Godot's official [download page](https://godotengine.org/download/windows/).

Currently, the C++ server has only been compiled for Windows. If you want to run the project on a different system, you will first need to compile. To do so, you will need to make sure that you have the `godot-cpp` submodule cloned and set up correctly (check out the first few sections in this [doc](https://docs.godotengine.org/en/stable/tutorials/scripting/gdextension/gdextension_cpp_example.html)). Then, run the following from the root directory (replacing `<your_platform>` with `windows`, `linux`, or `macos`):
```
scons platform=<your_platform>
```
