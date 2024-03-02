extends RigidBody3D

# Called when the node enters the scene tree for the first time.
func _ready():
	GlobalDropletServer.add_droplet(self)
