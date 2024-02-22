extends Node

var droplet_rids: Array[RID] = []

func register_droplet(droplet: ServerDroplet):
	droplet_rids.append(droplet.droplet_body_rid)

func deregister_droplet(droplet: ServerDroplet):
	droplet_rids.erase(droplet.droplet_body_rid)

var custom_droplets: Array[CustomDroplet]
var custom_forces: Array[Vector3]
var num_custom_droplets: int = 0

func register_custom_droplet(custom_droplet: CustomDroplet):
	custom_droplets.append(custom_droplet)
	custom_forces.append(Vector3.ZERO)
	num_custom_droplets += 1

func deregister_custom_droplet(custom_droplet: CustomDroplet):
	custom_droplets.erase(custom_droplet)
	custom_forces.pop_back()
	num_custom_droplets -= 1

func _physics_process(_delta: float) -> void:
	var i: int = 0
	var j: int = 0
	while i < num_custom_droplets:
		var droplet_a = custom_droplets[i]
		j = i + 1
		while j < num_custom_droplets:
			var droplet_b = custom_droplets[j]
			if droplet_a.position.distance_squared_to(droplet_b.position) < 1.0:
				var force_direc = (droplet_a.position - droplet_b.position).normalized()
				custom_forces[i] += -300 * force_direc
				custom_forces[j] += 300 * force_direc
				#droplet_a.apply_central_force(-300 * force_direc)
				#droplet_b.apply_central_force(300 * force_direc)
			j += 1
		i += 1
	i = 0
	while i < num_custom_droplets:
		custom_droplets[i].apply_central_force(custom_forces[i])
		custom_forces[i] = Vector3.ZERO
		i += 1
