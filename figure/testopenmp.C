// -----------------------------------------------------------
// This is a test program for openmp using multiple threads
// To compile: g++ testopenmp.C -o test -fopenmp
// -----------------------------------------------------------

# include <iostream>
# include <time.h>
# include <unistd.h>

using namespace std;

int main()
{
	# pragma omp parallel for
	for (int i = 0; i < 5; ++i)
	{
		time_t rawtime;
		time (&rawtime);
		cout << "pid: " << i << " current time: " << ctime(&rawtime) << endl;
		 int k = 0;
		//  while (k <= 1000)	{k++;	/*cout << i << "-" << k << endl;*/}
		sleep(2);
	}

	return 0;
}

