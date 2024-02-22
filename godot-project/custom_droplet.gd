class_name CustomDroplet extends RigidBody3D

func _enter_tree() -> void:
	PhysicsGlobals.register_custom_droplet(self)

func _exit_tree() -> void:
	PhysicsGlobals.deregister_custom_droplet(self)
