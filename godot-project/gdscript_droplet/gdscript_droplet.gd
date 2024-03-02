extends RigidBody3D

# When the droplet enters the tree, add it to the server
func _enter_tree() -> void:
	GDScriptDropletServerAutoload.add_droplet(self)

# When the droplet exits the tree, remove it from the server
func _exit_tree() -> void:
	GDScriptDropletServerAutoload.remove_droplet(self)
