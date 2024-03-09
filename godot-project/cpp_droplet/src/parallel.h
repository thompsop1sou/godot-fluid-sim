#ifndef PARALLEL_H
#define PARALLEL_H

#include <functional>
#include <execution>

using std::function;
using std::for_each;
using std::execution::par;
using std::min;
using std::max;

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
	void for_int(int p_start, int p_end, function<void(int)> func);
private:
	// Helper Functions
	void reset_range();
	int index_of(int p_x) const;
};

#endif