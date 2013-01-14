#include "aitken.h"
#include <assert.h>

void aitken(const double* seq1, double* seq2, int len){
    int i;
    double si, sip1, sip2;
    assert(len > 2); //length must be greater than 2
    for(i=0; i < len-2; i++){
	si = seq1[i];
	sip1 = seq1[i+1];
	sip2 = seq1[i+2];
	if(si==sip1)
	    seq2[i] = si;
	else //divide by zero is possible
	    seq2[i] = 
	      si - (sip1-si)*(sip1-si)/(sip2-2*sip1+si);
    }
}

double aitkenExtrapolate(double *seq1, double* seq2,
			 int len){
    int n, i, j;
    n = len/2;
    if(len%2==0)
	n--;
    for(i=0; i < n; i++){
	aitken(seq1, seq2, len-2*i);
	for(j=0; j < len-2*(i+1); j++)
	    seq1[j] = seq2[j];
    }
    return (len%2==0)?seq1[1]:seq1[0];
}
