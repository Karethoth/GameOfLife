#include "../src/lifegrid.h"

#include <iostream>
#include <cassert>

using std::cout;
using std::endl;

template<typename T>
int test_report(T result, T expected, std::string test_name)
{
	if(result != expected)
	{
		cout << "ERROR - " << test_name << ": Expected (" << expected << ") but got (" << result << ")" << endl;
		return 1;
	}

	cout << "OK    - " << test_name << endl;
	return 0;
}

void unit_test_report(int errors, std::string test_unit_name)
{
	if(errors)
	{
		cout << test_unit_name << ": Errors: " << errors << endl;
	}
	else
	{
		cout << test_unit_name << ": All tests passed!" << endl;
	}
}

#define TEST_REPORT(func, expected) test_report(func(), expected, #func)
#define UNIT_TEST_REPORT(func) cout << "Testing unit " << #func << endl; unit_test_report(func(), #func)

int test_kernel_compute_state()
{
	int errors = 0;

	// For more comprehensive tests, the kernel matrix could be rotated
	// around step by step with all possible initial permutations

	{
		CellKernel kernel_dead_neighbours_dead{{
			DEAD,  DEAD,  DEAD,
			DEAD,  DEAD,  DEAD,
			DEAD,  DEAD,  DEAD
		}};
		errors += TEST_REPORT(kernel_dead_neighbours_dead.compute_state, DEAD);
	}
	{

		CellKernel kernel_alive_neighbours_dead{{
			DEAD,  DEAD,  DEAD,
			DEAD,  ALIVE, DEAD,
			DEAD,  DEAD,  DEAD
		}};
		errors += TEST_REPORT(kernel_alive_neighbours_dead.compute_state, DEAD);
	}
	{
		CellKernel kernel_dead_neighbours_alive_1{{
			ALIVE, DEAD,  DEAD,
			DEAD,  DEAD,  DEAD,
			DEAD,  DEAD,  DEAD
		}};
		errors += TEST_REPORT(kernel_dead_neighbours_alive_1.compute_state, DEAD);
	}
	{
		CellKernel kernel_dead_neighbours_alive_2{{
			ALIVE, DEAD,  DEAD,
			DEAD,  DEAD,  DEAD,
			DEAD,  DEAD,  ALIVE
		}};
		errors += TEST_REPORT(kernel_dead_neighbours_alive_2.compute_state, DEAD);
	}
	{
		CellKernel kernel_dead_neighbours_alive_3{{
			ALIVE, DEAD,  DEAD,
			ALIVE,  DEAD,  DEAD,
			DEAD,  DEAD,  ALIVE
		}};
		errors += TEST_REPORT(kernel_dead_neighbours_alive_3.compute_state, ALIVE);
	}
	{
		CellKernel kernel_dead_neighbours_alive_4{{
			ALIVE, DEAD,  DEAD,
			ALIVE, DEAD,  ALIVE,
			DEAD,  DEAD,  ALIVE
		}};
		errors += TEST_REPORT(kernel_dead_neighbours_alive_4.compute_state, DEAD);
	}
	{
		CellKernel kernel_alive_neighbours_dead{{
			DEAD,  DEAD,   DEAD,
			DEAD,  ALIVE,  DEAD,
			DEAD,  DEAD,   DEAD
		}};
		errors += TEST_REPORT(kernel_alive_neighbours_dead.compute_state, DEAD);
	}
	{
		CellKernel kernel_alive_neighbours_alive_1{{
			DEAD,  ALIVE,   DEAD,
			DEAD,  ALIVE,  DEAD,
			DEAD,  DEAD,   DEAD
		}};
		errors += TEST_REPORT(kernel_alive_neighbours_alive_1.compute_state, DEAD);
	}
	{
		CellKernel kernel_alive_neighbours_alive_2{{
			DEAD,  ALIVE,  ALIVE,
			DEAD,  ALIVE,  DEAD,
			DEAD,  DEAD,   DEAD
		}};
		errors += TEST_REPORT(kernel_alive_neighbours_alive_2.compute_state, ALIVE);
	}
	{
		CellKernel kernel_alive_neighbours_alive_3{{
			DEAD,  ALIVE,  ALIVE,
			DEAD,  ALIVE,  DEAD,
			ALIVE, DEAD,   DEAD
		}};
		errors += TEST_REPORT(kernel_alive_neighbours_alive_3.compute_state, ALIVE);
	}
	{
		CellKernel kernel_alive_neighbours_alive_4{{
			DEAD,  ALIVE,  ALIVE,
			DEAD,  ALIVE,  DEAD,
			ALIVE, ALIVE,  DEAD
		}};
		errors += TEST_REPORT(kernel_alive_neighbours_alive_4.compute_state, DEAD);
	}
	{
		CellKernel kernel_alive_neighbours_alive_5{{
			DEAD,  ALIVE,  ALIVE,
			DEAD,  ALIVE,  DEAD,
			ALIVE, ALIVE,  ALIVE
		}};
		errors += TEST_REPORT(kernel_alive_neighbours_alive_5.compute_state, DEAD);
	}

	return errors;
}

int main()
{
	UNIT_TEST_REPORT(test_kernel_compute_state);
}

