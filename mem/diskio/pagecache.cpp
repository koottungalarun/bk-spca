#include <cstdlib>
#include <cstdio>

const int N=1000*1000*125;
/*
 * "cat /proc/meminfo" before and after this program to check disk cache.
 * Use the command "echo 1 > /proc/sys/vm/drop_caches" to clear disk cache.
 * Command ought to be hardwired here?
 */
void write2file(){
	/*
	 * v = 1 GB array
	 */
	double *v = new double[N];
	for(int i=0; i < N; i++)
		v[i] = rand()*1.0/RAND_MAX;
	char cmd[200];
	if(getenv("SCRATCH")==NULL){
		printf("SCRATCH directory must be set\n");
		exit(-1);
	}
	sprintf(cmd, "mkdir %s/diskio-serial", getenv("SCRATCH"));
	system(cmd);
	char fname[200];
	sprintf(fname, "%s/diskio-serial/pagecachechk.dat", getenv("SCRATCH"));
	FILE *fptr;
	fptr = fopen(fname, "w");
	for(int i=0; i < 10; i++)
		fwrite(v, N, sizeof(double), fptr);
	fclose(fptr);
	sprintf(cmd, "rm %s", fname);
	//system(cmd);/* page cache shrinks back only if file is removed */
	delete[] v;
}

int main(){
	write2file();
}

