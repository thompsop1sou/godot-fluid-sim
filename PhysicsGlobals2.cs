using Godot;

public partial class PhysicsGlobals2 : Node
{
	private CustomDroplet2[] _customDroplets = new CustomDroplet2[500];
	private System.Numerics.Vector3[] _customForces = new System.Numerics.Vector3[500];
	private uint _numCustomDroplets = 0;

	public void RegisterCustomDroplet(CustomDroplet2 droplet)
	{
		_customDroplets[_numCustomDroplets] = droplet;
		_customForces[_numCustomDroplets] = System.Numerics.Vector3.Zero;
		_numCustomDroplets++;
	}

	private static System.Numerics.Vector3 ToSystemVector3(Vector3 vec)
	{
		return new System.Numerics.Vector3(vec.X, vec.Y, vec.Z);
	}

	private static Vector3 ToGodotVector3(System.Numerics.Vector3 vec)
	{
		return new Vector3(vec.X, vec.Y, vec.Z);
	}

	// Called every frame. 'delta' is the elapsed time since the previous frame.
	public override void _PhysicsProcess(double delta)
	{
		for (int i = 0; i < _numCustomDroplets; ++i)
		{
			var dropletA = _customDroplets[i];
			System.Numerics.Vector3 dropletAPosition = ToSystemVector3(dropletA.Position);
			for (int j = i + 1; j < _numCustomDroplets; ++j)
			{
				var dropletB = _customDroplets[j];
				System.Numerics.Vector3 dropletBPosition = ToSystemVector3(dropletB.Position);
				if (System.Numerics.Vector3.DistanceSquared(dropletAPosition, dropletBPosition) < 0.5f)
				{
					System.Numerics.Vector3 forceDirec = System.Numerics.Vector3.Normalize(dropletAPosition - dropletBPosition);
					_customForces[i] += -1000f * forceDirec;
					_customForces[j] += 1000f * forceDirec;
				}
			}
		}
		for (int i = 0; i < _numCustomDroplets; ++i)
		{
			var force = ToGodotVector3(_customForces[i]);
			_customDroplets[i].ApplyCentralForce(force);
			_customForces[i] = System.Numerics.Vector3.Zero;
		}
	}
}
