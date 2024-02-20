class_name ServerDroplet extends RigidBody3D

var nearby_droplet_rids: Array[RID]
var nearby_body_rids: Array[RID]
var droplet_force_magnitude: float = 0
var body_force_magnitude: float = 100000

var outer_radius: float = 0.375
var droplet_body_rid: RID
var droplet_area_rid: RID
var droplet_area_shape_rid: RID

func _ready() -> void:
	droplet_body_rid = get_rid()
	PhysicsGlobals.register_droplet(self)
	# Create a physics area
	droplet_area_shape_rid = PhysicsServer3D.sphere_shape_create()
	PhysicsServer3D.shape_set_data(droplet_area_shape_rid, outer_radius)
	droplet_area_rid = PhysicsServer3D.area_create()
	PhysicsServer3D.area_set_monitorable(droplet_area_rid, false)
	PhysicsServer3D.area_add_shape(droplet_area_rid, droplet_area_shape_rid)
	PhysicsServer3D.area_set_space(droplet_area_rid, get_world_3d().space)
	PhysicsServer3D.area_set_transform(droplet_area_rid, transform)
	PhysicsServer3D.area_set_monitor_callback(droplet_area_rid, area_monitor_callback)

func _exit_tree():
	PhysicsGlobals.deregister_droplet(self)
	PhysicsServer3D.free_rid(droplet_area_shape_rid)
	PhysicsServer3D.free_rid(droplet_area_rid)

func area_monitor_callback(status: int, body_rid: RID, instance_id: RID,
							body_shape_idx: int, self_shape_idx: int) -> void:
	if status == PhysicsServer3D.AREA_BODY_ADDED:
		if PhysicsGlobals.droplet_rids.has(body_rid):
			if body_rid != droplet_body_rid:
				nearby_droplet_rids.append(body_rid)
		else:
				nearby_body_rids.append(body_rid)
	elif status == PhysicsServer3D.AREA_BODY_REMOVED:
		if PhysicsGlobals.droplet_rids.has(body_rid):
			if body_rid != droplet_body_rid:
				nearby_droplet_rids.erase(body_rid)
		else:
				nearby_body_rids.erase(body_rid)

# Called every frame. 'delta' is the elapsed time since the previous frame.
func _physics_process(delta: float) -> void:
	var droplet_force = Vector3()
	for droplet_rid in nearby_droplet_rids:
		var droplet_state = PhysicsServer3D.body_get_direct_state(droplet_rid)
		droplet_force += (position - droplet_state.transform.origin).normalized()
	var body_force = Vector3()
	for body_rid in nearby_body_rids:
		var body_state = PhysicsServer3D.body_get_direct_state(body_rid)
		body_force += (position - body_state.transform.origin).normalized()
	apply_central_force(droplet_force_magnitude * droplet_force + body_force_magnitude * body_force)
	PhysicsServer3D.area_set_transform(droplet_area_rid, transform)
