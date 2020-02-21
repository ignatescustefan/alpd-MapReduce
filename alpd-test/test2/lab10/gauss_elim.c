#include <stdio.h>
#include <mpi/mpi.h>

#define N 2

int main(int argc, char * argv[])
{
	int rank, size;

	int n = N;

	double A[N][N] = {{6, 3},{20, -2}};
	double B[N] = {9,24};

	double *a, b = 0;
	double a_buf[N], b_buf = 0;

	int k, j;


	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &size);


	a = A[rank]; b = B[rank];
	
	for (k = 0; k < n; k++) {

		if (rank == k) {
	
			for (j = k + 1; j < n; j++) {
				a[j] = a[j] / a[k];			
			}
			
			b = b / a[k];
			a[k] = 1;

			for (j = 0; j < n; j++) {
				a_buf[j] = a[j];
			}

			b_buf = b;
		}
		
		MPI_Bcast(a_buf, n, MPI_DOUBLE, k, MPI_COMM_WORLD);
		MPI_Bcast(&b_buf, 1, MPI_DOUBLE, k, MPI_COMM_WORLD);

		if (rank > k) {

			for (j = k + 1; j < n; j++) {
				a[j] = a[j] - a[k] * a_buf[j];
			}
			
			b = b - a[k] * b_buf;
			a[k] = 0;
		}

	}

	printf ("%d -> ", rank);
	for (k = 0; k < n; k++) {
		printf ("%.1lf ", a[k]);
	}

	printf (" %.1lf\n", b);
	
	MPI_Finalize();
	return 0;
}