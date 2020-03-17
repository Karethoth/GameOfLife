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

#define TEST_VAL_REPORT(val, expected) test_report(val, expected, #val)
#define TEST_FUNC_REPORT(func, expected) test_report(func(), expected, #func)
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
		errors += TEST_FUNC_REPORT(kernel_dead_neighbours_dead.compute_state, DEAD);
	}
	{

		CellKernel kernel_alive_neighbours_dead{{
			DEAD,  DEAD,  DEAD,
			DEAD,  ALIVE, DEAD,
			DEAD,  DEAD,  DEAD
		}};
		errors += TEST_FUNC_REPORT(kernel_alive_neighbours_dead.compute_state, DEAD);
	}
	{
		CellKernel kernel_dead_neighbours_alive_1{{
			ALIVE, DEAD,  DEAD,
			DEAD,  DEAD,  DEAD,
			DEAD,  DEAD,  DEAD
		}};
		errors += TEST_FUNC_REPORT(kernel_dead_neighbours_alive_1.compute_state, DEAD);
	}
	{
		CellKernel kernel_dead_neighbours_alive_2{{
			ALIVE, DEAD,  DEAD,
			DEAD,  DEAD,  DEAD,
			DEAD,  DEAD,  ALIVE
		}};
		errors += TEST_FUNC_REPORT(kernel_dead_neighbours_alive_2.compute_state, DEAD);
	}
	{
		CellKernel kernel_dead_neighbours_alive_3{{
			ALIVE, DEAD,  DEAD,
			ALIVE,  DEAD,  DEAD,
			DEAD,  DEAD,  ALIVE
		}};
		errors += TEST_FUNC_REPORT(kernel_dead_neighbours_alive_3.compute_state, ALIVE);
	}
	{
		CellKernel kernel_dead_neighbours_alive_4{{
			ALIVE, DEAD,  DEAD,
			ALIVE, DEAD,  ALIVE,
			DEAD,  DEAD,  ALIVE
		}};
		errors += TEST_FUNC_REPORT(kernel_dead_neighbours_alive_4.compute_state, DEAD);
	}
	{
		CellKernel kernel_alive_neighbours_dead{{
			DEAD,  DEAD,   DEAD,
			DEAD,  ALIVE,  DEAD,
			DEAD,  DEAD,   DEAD
		}};
		errors += TEST_FUNC_REPORT(kernel_alive_neighbours_dead.compute_state, DEAD);
	}
	{
		CellKernel kernel_alive_neighbours_alive_1{{
			DEAD,  ALIVE,   DEAD,
			DEAD,  ALIVE,  DEAD,
			DEAD,  DEAD,   DEAD
		}};
		errors += TEST_FUNC_REPORT(kernel_alive_neighbours_alive_1.compute_state, DEAD);
	}
	{
		CellKernel kernel_alive_neighbours_alive_2{{
			DEAD,  ALIVE,  ALIVE,
			DEAD,  ALIVE,  DEAD,
			DEAD,  DEAD,   DEAD
		}};
		errors += TEST_FUNC_REPORT(kernel_alive_neighbours_alive_2.compute_state, ALIVE);
	}
	{
		CellKernel kernel_alive_neighbours_alive_3{{
			DEAD,  ALIVE,  ALIVE,
			DEAD,  ALIVE,  DEAD,
			ALIVE, DEAD,   DEAD
		}};
		errors += TEST_FUNC_REPORT(kernel_alive_neighbours_alive_3.compute_state, ALIVE);
	}
	{
		CellKernel kernel_alive_neighbours_alive_4{{
			DEAD,  ALIVE,  ALIVE,
			DEAD,  ALIVE,  DEAD,
			ALIVE, ALIVE,  DEAD
		}};
		errors += TEST_FUNC_REPORT(kernel_alive_neighbours_alive_4.compute_state, DEAD);
	}
	{
		CellKernel kernel_alive_neighbours_alive_5{{
			DEAD,  ALIVE,  ALIVE,
			DEAD,  ALIVE,  DEAD,
			ALIVE, ALIVE,  ALIVE
		}};
		errors += TEST_FUNC_REPORT(kernel_alive_neighbours_alive_5.compute_state, DEAD);
	}

	return errors;
}

int test_kernel_step_right()
{
	int errors = 0;

	{
		CellKernel kernel_step_right_1{{
			ALIVE, DEAD,  DEAD,
			ALIVE, DEAD,  DEAD,
			ALIVE, DEAD,  DEAD
		}};
		kernel_step_right_1.step_right();

		errors += TEST_VAL_REPORT(kernel_step_right_1.cells[0], DEAD);
		errors += TEST_VAL_REPORT(kernel_step_right_1.cells[3], DEAD);
		errors += TEST_VAL_REPORT(kernel_step_right_1.cells[6], DEAD);
	}
	{
		CellKernel kernel_step_right_2{{
			DEAD,  ALIVE, DEAD,
			DEAD,  ALIVE, DEAD,
			DEAD,  ALIVE, DEAD
		}};
		kernel_step_right_2.step_right();

		errors += TEST_VAL_REPORT(kernel_step_right_2.cells[0], ALIVE);
		errors += TEST_VAL_REPORT(kernel_step_right_2.cells[3], ALIVE);
		errors += TEST_VAL_REPORT(kernel_step_right_2.cells[6], ALIVE);

		errors += TEST_VAL_REPORT(kernel_step_right_2.cells[1], DEAD);
		errors += TEST_VAL_REPORT(kernel_step_right_2.cells[4], DEAD);
		errors += TEST_VAL_REPORT(kernel_step_right_2.cells[7], DEAD);
	}
	{
		CellKernel kernel_step_right_3{{
			DEAD,  DEAD,  ALIVE,
			DEAD,  DEAD,  ALIVE,
			DEAD,  DEAD,  ALIVE
		}};
		kernel_step_right_3.step_right();

		errors += TEST_VAL_REPORT(kernel_step_right_3.cells[0], DEAD);
		errors += TEST_VAL_REPORT(kernel_step_right_3.cells[3], DEAD);
		errors += TEST_VAL_REPORT(kernel_step_right_3.cells[6], DEAD);

		errors += TEST_VAL_REPORT(kernel_step_right_3.cells[1], ALIVE);
		errors += TEST_VAL_REPORT(kernel_step_right_3.cells[4], ALIVE);
		errors += TEST_VAL_REPORT(kernel_step_right_3.cells[7], ALIVE);

		errors += TEST_VAL_REPORT(kernel_step_right_3.cells[2], DEAD);
		errors += TEST_VAL_REPORT(kernel_step_right_3.cells[5], DEAD);
		errors += TEST_VAL_REPORT(kernel_step_right_3.cells[8], DEAD);
	}

	return errors;
}

int main()
{
	UNIT_TEST_REPORT(test_kernel_compute_state);
	UNIT_TEST_REPORT(test_kernel_step_right);
}

