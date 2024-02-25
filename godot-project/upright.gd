extends RigidBody3D


# Applies a rotational force to the RigidBody3D to cause it to stand upright.
func erect() -> void:
	var gravity_dir: Vector3 = _get_gravity_vector().normalized()
	var curr_up_dir: Vector3 = basis.y.normalized()
	print("curr_up_dir: ", curr_up_dir)
	print("gravity_dir: ", gravity_dir)
	var torque_dir: Vector3
	if gravity_dir == curr_up_dir:
		torque_dir = basis.z
	else:
		torque_dir = curr_up_dir.cross(-gravity_dir)
	var torque_mag = curr_up_dir.angle_to(-gravity_dir)
	print("torque_mag: ", rad_to_deg(torque_mag))
	apply_torque(8.0 * torque_mag * torque_dir)

# Gets the gravity vector in the current area from the PhysicsServer3D.
func _get_gravity_vector() -> Vector3:
	var my_state: PhysicsDirectBodyState3D = PhysicsServer3D.body_get_direct_state(get_rid())
	return my_state.total_gravity.normalized()

# Called every frame. 'delta' is the elapsed time since the previous frame.
func _process(delta):
	erect()
