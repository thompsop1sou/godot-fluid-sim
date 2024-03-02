#ifndef CPP_DROPLET_SERVER_H
#define CPP_DROPLET_SERVER_H

#include <godot_cpp/variant/variant.hpp>
#include <godot_cpp/variant/vector3.hpp>
#include <godot_cpp/classes/physics_body3d.hpp>
#include <godot_cpp/classes/rigid_body3d.hpp>

#include <mutex>

#include "vec3.h"

namespace godot
{
	class CppDropletServer : public Node
	{
		GDCLASS(CppDropletServer, Node)

	public:
		// Max number of droplets
		static const int MAX_DROPLETS = 4000;

	private:
		// An array of droplets and their properties
		int m_num_droplets;
		RigidBody3D* m_droplets[MAX_DROPLETS];
		Vec3 m_positions[MAX_DROPLETS];
		Vec3 m_forces[MAX_DROPLETS];
		std::mutex m_mutexes[MAX_DROPLETS];

		// The magnitude of the attraction force
		float m_force_magnitude;

		// The distance that the attraction force is effective over
		float m_force_effective_distance;
		float m_force_effective_distance_squared;

		// Whether currently in-game
		bool m_in_game;

	protected:
		// Needed for exposing stuff to Godot
		static void _bind_methods();

	public:
		// Constructor and destructor
		CppDropletServer();
		~CppDropletServer();

		// Overridden functions
		void _ready() override;
		void _physics_process(double delta) override;

		// Adds a droplet to the server's array
		void add_droplet(RigidBody3D* p_droplet);

		// Removes a droplet from the server's array
		void remove_droplet(RigidBody3D* p_droplet);

		// Getters and setters for force magnitude
		float get_force_magnitude() const;
		void set_force_magnitude(const float p_force_magnitude);

		// Getters and setters for force effective distance
		float get_force_effective_distance() const;
		void set_force_effective_distance(const float p_force_effective_distance);
	};
}

#endif