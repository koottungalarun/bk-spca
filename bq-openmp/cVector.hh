#ifndef __MycVector__
#define __MycVector__
#include <iostream>
#include <fstream>
#include <assert.h>
#include "Vector.hh"

using namespace std;

typedef double z_complex[2];

inline double abs(z_complex& x){
  return sqrt(x[0]*x[0]+x[1]*x[1]);
}

inline void polar(z_complex& x, double& r, double& t){
  r = abs(x);
  t = atan2(x[1], x[0]);
}

inline void conjugate(z_complex& x){
  x[1] *= -1;
}

inline void equate(z_complex& z, double x, double y){
  z[0] = x; z[1] = y;
}

inline void equate(z_complex& x, const z_complex& y){
  x[0] = y[0]; x[1] = y[1];
}

inline void mult(z_complex& x, double y){
  x[0] *= y; x[1] *= y;
}

inline void mult(z_complex& x, const z_complex& y){
  double a  = x[0]*y[0]-x[1]*y[1];
  double b = x[0]*y[1]+x[1]*y[0];
  x[0] = a;
  x[1] = b;
}

inline void div(z_complex& x, const z_complex& y){
  z_complex z;
  z[0] = y[0]/(y[0]*y[0]+y[1]*y[1]);
  z[1] = -y[1]/(y[0]*y[0]+y[1]*y[1]);
  mult(x, z);
}

inline void add(z_complex& x, const z_complex& y){
  x[0] += y[0];
  x[1] += y[1];
}

inline void sub(z_complex& x, const z_complex& y){
  x[0] -= y[0];
  x[1] -= y[1];
}

inline void euler(double t, z_complex& x){
  x[0] = cos(t);
  x[1] = sin(t);
}

class cVector{
private:
  int size;
  z_complex *data;
  int owner;
public:
  cVector(int n){//only constructor to allocate data
    size = n;
    data = (z_complex *)MKL_malloc((sizeof(z_complex))*n, 64);
    owner = 1;
#ifdef DEBUG
    for(int i=0; i < size; i++){
      data[i][0] = -i;
      data[i][1] = i;
    }
#endif
  };

  cVector(){
    size = 0;
    data = NULL;
    owner = 0;
  }
  

  cVector(cVector& v){
    size = v.size;
    data = v.data;
    owner = 0;
  }

  ~cVector(){
    if(owner)
      MKL_free(data);
  }


  void shadow(const cVector& v, int i, int len){
    assert(!owner);
    assert((i+len<=v.size)&&(i>=0)&&(len>0));
    size = len;
    data = v.data + i;
    owner = 0;
  }

  void shadow(const Vector& v){
    assert(v.getSize()%2==0);
    assert(!owner);
    size = v.getSize()/2;
    data = (z_complex *)v.getRawData();
    owner = 0;
  }

  z_complex& operator()(int i){
    assert(i < size);
    return(data[i]);
  }
 
  const z_complex& operator()(int i) const{
    assert(i < size);
    return(data[i]);
  }

  void operator=(const cVector& v){
    assert(size==v.getSize());
    assert(((data < v.data)&&(data+size-1 < v.data))||
	   ((data > v.data)&&(data > v.data+size-1))||
	   (data==v.data));
    if(data==v.data)
      return;
    memcpy((char *)data, (char *)v.data, size*sizeof(z_complex));
  }

  int getSize() const{
    return size;
  }

  z_complex *getRawData() const{
    return data;
  }

  void scale(double x){
    for(int i=0; i < size; i++){
      data[i][0] *= x;
      data[i][1] *= x;
    }
  }

  void scale(const z_complex& fac){
    for(int i=0; i < size; i++)
      mult(data[i], fac);
  }

  void conjugate(){
    for(int i=0; i < size; i++)
      ::conjugate(data[i]);
  }
  
  void add(cVector& v){
    assert(v.getSize()==size);
    for(int i=0; i < size; i++)
      ::add(data[i], v(i));
  }
  void sub(cVector& v){
    assert(v.getSize()==size);
    for(int i=0; i < size; i++)
      ::sub(data[i], v(i));
  }

  double norm() const{
    int dim = 2*size;
    double * vec = (double *)data;
    double ans = 0;
    for(int i=0; i < dim; i++)
      ans += fabs(vec[i]);
    return ans;
  }

  void output(const char* fname, int digits=18) const{
    ofstream ofile(fname);
    assert(!ofile.fail());
    ofile.setf(ios::scientific, ios::floatfield);
    ofile.precision(digits);
    int i;
    for(i=0; i < size; i++)
      ofile<<data[i][0]<<" "<<data[i][1]<<endl;
    ofile.close();
  }

  void input(const char* fname){
    ifstream ifile(fname);
    assert(!ifile.fail());
    int i;
    for(i=0; i < size; i++){
      assert(!ifile.eof());
      ifile >> data[i][0];
      if(ifile.fail()){
	cout<<"Possible underflow in input from "<<fname<<endl;
	ifile.clear();
      }
      ifile >> data[i][1];
      if(ifile.fail()){
	cout<<"Possible underflow in input from "<<fname<<endl;
	ifile.clear();
      }
    }
    ifile.close();
  }
};
#endif

