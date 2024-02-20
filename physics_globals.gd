extends Node

var droplet_rids: Array[RID] = []

func register_droplet(droplet: ServerDroplet):
	droplet_rids.append(droplet.droplet_body_rid)

func deregister_droplet(droplet: ServerDroplet):
	droplet_rids.erase(droplet.droplet_body_rid)
