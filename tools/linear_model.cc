
#include "linear_model.h"

using namespace Turk;


#ifdef __linux__
extern "C" {
  extern void dgelsd_(
    int * m, int * n, int * nrhs
    , double * a, int * lda
    , double * b, int * ldb
    , double * s
    , double * rcond, int * rank
    , double * work, int * lwork
    , int * iwork
    , int * info
  );
}
#endif


void linear_model::fit(double * x
  ,double * y
  , const unsigned n_features
  , const unsigned n_obs ) 
{

  // 
  // solve A x = b
  int m = n_obs;
  int n = n_features;
  int nrhs = 1;
  double rcond = -1;
  int rank;
  int lwork = 2048;
  std::vector<double> s(n_features), work(lwork);
  std::vector<int> iwork(lwork);

  std::vector<double> xT(m*n);
  for ( unsigned i=0; i != m; i++ )
    for ( unsigned j=0; j != n; j++ )
      xT[j*m+i] = x[i*n+j];

  // compute optimal size of the work array
  lwork = -1;
#ifdef __linux__
  dgelsd_(&m,&n,&nrhs,&xT[0],&m,y,&m,&s[0],&rcond,&rank,&work[0],&lwork,&iwork[0],&m_info);
#endif
  lwork = work[0];

  work.resize(lwork);
  iwork.resize(lwork);

  // solve the set of equations
#ifdef __linux__
  dgelsd_(&m,&n,&nrhs,&xT[0],&m,y,&m,&s[0],&rcond,&rank,&work[0],&lwork,&iwork[0],&m_info);
#endif

  m_coefs.resize(n_features);
  for ( unsigned i=0; i != n_features; i++ )
    m_coefs[i] = y[i];

  been_fit = true;

}
