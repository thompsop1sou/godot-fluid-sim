#ifndef PARALLEL_H
#define PARALLEL_H

#include <functional>
#include <execution>

using std::function;
using std::for_each;
using std::execution::par;

class Parallel
{
private:
    // Data Members
	int m_min;
	int m_max;
	int* m_range;
public:
	// Constructors and Destructor
	Parallel(int p_min, int p_max);
	~Parallel();
	// Getters and Setters
	int get_min() const;
	void set_min(int p_min);
	int get_max() const;
	void set_max(int p_max);
	void set_min_max(int p_min, int p_max);
	// For Loop (essentially a convenient wrapper for std::for_each(std::execution::par, ...))
	bool for_int(int p_start, int p_end, function<void(int)> func) const;
private:
	// Helper Functions
	void reset_range();
};

#endif