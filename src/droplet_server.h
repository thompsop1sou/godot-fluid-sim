#ifndef DROPLET_SERVER_H
#define DROPLET_SERVER_H

#include <godot_cpp/variant/variant.hpp>
#include <godot_cpp/variant/vector3.hpp>
#include <godot_cpp/classes/physics_body3d.hpp>
#include <godot_cpp/classes/rigid_body3d.hpp>

#include <mutex>

#include "vec3.h"

namespace godot
{
	class DropletServer : public Node
	{
		GDCLASS(DropletServer, Node)

	public:
		// Max number of droplets
		static const int MAX_DROPLETS = 4000;

	private:
		// An array of droplets and their properties
		int m_num_droplets;
		RigidBody3D* m_droplets[MAX_DROPLETS];
		Vec3 m_positions[MAX_DROPLETS];
		Vec3 m_forces[MAX_DROPLETS];

		// The magnitude of the attraction force and the distance that it is effective over
		float m_force_magnitude;
		float m_force_effective_distance;
		float m_force_effective_distance_squared;

		// Whether currently in-game
		bool m_in_game;

		// Used to make sure threads are safe
		std::mutex m_mutexes[MAX_DROPLETS];

	protected:
		static void _bind_methods();

	public:
		// Constructor and destructor
		DropletServer();
		~DropletServer();

		// Overridden functions
		void _ready() override;
		void _physics_process(double delta) override;

		// Functions for adding to or removing from the droplets array
		void add_droplet(RigidBody3D* p_droplet);
		void remove_droplet(RigidBody3D* p_droplet);

		// Getters and setters for force magnitude and effective distance
		void set_force_magnitude(float p_force_magnitude);
		float get_force_magnitude() const;
		void set_force_effective_distance(float p_force_effective_distance);
		float get_force_effective_distance() const;
	};
}

#endif