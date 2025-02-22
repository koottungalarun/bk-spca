#include "../../utils/utils.hh"
#include "fft_thrd.hh"
#include <iostream>
#include <cmath>
#include <cstdlib>

void test(int n, long count, int nth){
	long len = 2l*n*count;
	long nbytes = len * sizeof(double);
	double *v = (double *)_mm_malloc(nbytes, 64);
	
	for(long j=0; j < count; j++)
		for(int i=0; i < n; i++){
			double x = 2.0*PI*i/n;
			v[2*i + 2*n*j] = 2*j*cos(x);
			v[2*i + 1 + 2*n*j] = 0;
		}
	
	fft_thrd fft(n, count, nth);
	fft.fwd(v);
	
	for(long j=0; j < count; j++){
		std::cout<<v[2*n*j]<<std::endl;
		std::cout<<v[2*n*j + 1]<<std::endl;
		std::cout<<v[2*n*j + 2]<<std::endl;
		std::cout<<v[2*n*j + 3]<<std::endl;
		std::cout<<"\n\n";
	}
	
	_mm_free(v);
}

int main(){

#ifdef __MIC__
	assrt(getenv("MIC_OMP_NUM_THREADS") != NULL);
	const int nthreads = atoi(getenv("MIC_OMP_NUM_THREADS"));
#else
	const int nthreads = num_cpu();
#endif

	test(10, 24, nthreads);
}

