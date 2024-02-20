class_name Droplet extends RigidBody3D

var nearby_droplets: Array[Droplet]
var nearby_bodies: Array[PhysicsBody3D]
var droplet_force_magnitude: float = -300
var body_force_magnitude: float = 0

# Called every physics frame. 'delta' is the elapsed time since the previous frame.
func _physics_process(delta):
	var droplet_force = Vector3()
	for droplet in nearby_droplets:
		droplet_force += (position - droplet.position).normalized()
	var body_force = Vector3()
	for body in nearby_bodies:
		body_force += (position - body.position).normalized()
	apply_central_force(droplet_force_magnitude * droplet_force + body_force_magnitude * body_force)

func _on_area_3d_body_entered(body):
	if body is Droplet:
		if not body == self:
			nearby_droplets.append(body)
	else:
		nearby_bodies.append(body)

func _on_area_3d_body_exited(body):
	if body is Droplet:
		if not body == self:
			nearby_droplets.erase(body)
	else:
		nearby_bodies.erase(body)
