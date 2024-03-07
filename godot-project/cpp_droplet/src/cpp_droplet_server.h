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
		// A struct to hold information about each droplet
		struct Droplet
		{
			RID rid;
			Vec3 position;
			Vec3 force;
			std::mutex mut;
		};

		// An array of Droplet structs
		int m_num_droplets;
		Droplet m_droplets[MAX_DROPLETS];

		// The magnitude of the attraction force
		float m_force_magnitude;

		// The distance that the attraction force is effective over
		float m_force_effective_distance;
		float m_force_effective_distance_squared;

		// Whether currently in-game
		bool m_in_game;

		// Reference to the physics server singleton
		PhysicsServer3D* m_physics_server;

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
		void add_droplet(RID droplet_rid);

		// Removes a droplet from the server's array
		void remove_droplet(RID droplet_rid);

		// Getter and setter for force magnitude
		float get_force_magnitude() const;
		void set_force_magnitude(const float p_force_magnitude);

		// Getter and setter for force effective distance
		float get_force_effective_distance() const;
		void set_force_effective_distance(const float p_force_effective_distance);

	private:
		// Confirms that an RID is for a rigid body
		bool is_rigid_body(const RID &droplet_rid) const;
		// Gets the position of a droplet from its RID
		Vector3 get_droplet_position(const RID &droplet_rid) const;
	};
}

#endif