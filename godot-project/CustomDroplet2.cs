using Godot;
using System;

public partial class CustomDroplet2 : RigidBody3D
{
	// Called when the node enters the scene tree for the first time.
	public override void _Ready()
	{
		GetNode<PhysicsGlobals2>("/root/PhysicsGlobals2").RegisterCustomDroplet(this);
	}
}
