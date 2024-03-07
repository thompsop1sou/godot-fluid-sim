extends RigidBody3D

# When the droplet enters the tree, add it to the server
func _enter_tree() -> void:
	CsDropletServerAutoload.AddDroplet(get_rid())

# When the droplet exits the tree, remove it from the server
func _exit_tree() -> void:
	CsDropletServerAutoload.RemoveDroplet(get_rid())
