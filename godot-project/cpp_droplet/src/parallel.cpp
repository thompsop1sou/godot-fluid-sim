#include "parallel.h"

// Constructors and Destructor

Parallel::Parallel(int p_min, int p_max) : m_min(0), m_max(0), m_range(nullptr)
{
	// Initialize min and max
	if (p_min > p_max)
	{
		m_min = p_max;
		m_max = p_min;
	}
	else
	{
		m_min = p_min;
		m_max = p_max;
	}
	// Initialize range
	m_range = new int[m_max - m_min];
	for (int i = m_min; i < m_max; i++)
	{
		m_range[index_of(i)] = i;
	}
}

Parallel::~Parallel()
{
	delete [] m_range;
}

// Getters and Setters

int Parallel::get_min() const
{
	return m_min;
}

void Parallel::set_min(int p_min)
{
	if (p_min > m_max)
	{
		m_min = m_max;
	}
	else
	{
		m_min = p_min;
	}
	reset_range();
}

int Parallel::get_max() const
{
	return m_max;
}

void Parallel::set_max(int p_max)
{
	if (p_max < m_min)
	{
		m_max = m_min;
	}
	else
	{
		m_max = p_max;
	}
	reset_range();
}

void Parallel::set_min_max(int p_min, int p_max)
{
	if (p_min > p_max)
	{
		m_min = p_max;
		m_max = p_min;
	}
	else
	{
		m_min = p_min;
		m_max = p_max;
	}
	reset_range();
}

// For Loop (essentially a convenient wrapper for std::for_each(std::execution::par, ...))

void Parallel::for_int(int p_start, int p_end, function<void(int)> func)
{
	// Break early if the range is bad
	if (p_start > p_end)
	{
		return;
	}
	// Reset the range if needed
	if (p_start < m_min || p_end > m_max)
	{
		m_min = min(p_start, m_min);
		m_max = max(p_end, m_max);
		reset_range();
	}
	// Run the for loop
	for_each(par, &m_range[index_of(p_start)], &m_range[index_of(p_end)], func);
}

// Helper Functions

void Parallel::reset_range()
{
	delete [] m_range;
	m_range = new int[m_max - m_min];
	for (int i = m_min; i < m_max; i++)
	{
		m_range[index_of(i)] = i;
	}
}

int Parallel::index_of(int p_x) const
{
	return p_x - m_min;
}