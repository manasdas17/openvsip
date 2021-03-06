//
// Copyright (c) 2010 by CodeSourcery
// Copyright (c) 2013 Stefan Seefeld
// All rights reserved.
//
// This file is part of OpenVSIP. It is made available under the
// license contained in the accompanying LICENSE.GPL file.

/// Description
///   Performance tests for math reductions.

#include <vsip/initfin.hpp>
#include <vsip/matrix.hpp>
#include <vsip/math.hpp>
#include <vsip/math.hpp>
#include <ovxx/timer.hpp>
#include <test.hpp>
#include <iostream>

using namespace ovxx;


#define TIME_REDUCTION(OP)                            \
template <typename                            T,      \
          template <typename, typename> class ViewT,  \
          typename                            BlockT> \
float                                                 \
time_##OP(ViewT<T, BlockT> view)                      \
{                                                     \
  /* warmup (to remove cache-miss effects) */         \
  OP(view);                                           \
  timer t1;					      \
  OP(view);					      \
  double delta = t1.elapsed();			      \
  std::cout << "time: " << delta << std::endl;	      \
  return delta;					      \
}

TIME_REDUCTION(sumval);
TIME_REDUCTION(sumsqval);
TIME_REDUCTION(meanval);
TIME_REDUCTION(meansqval);

#undef test_assert
#define test_assert(x) (x);

#define TEST_REDUCTION(OP, VIEW1, VIEW2, VIEW3)         \
{                                                       \
  double t1 = time_##OP(VIEW1);                         \
  double t2 = time_##OP(VIEW2);			        \
  double t3 = time_##OP(VIEW3);                         \
  double tol = t1 * .1;                                 \
  test_assert(((t1 - tol) < t2) && (t2 < (t1 + tol)) && \
              ((t1 - tol) < t3) && (t3 < (t1 + tol)));  \
}



// The purpose of this test is to measure the relative performance 
// differences between reductions involving differently-dimensioned
// views, which are supposed to be nearly equivalent in terms of
// performance as 2- and 3-D dense views are re-cast into 1-D views
// and then reduced.
int
main(int argc, char **argv)
{
  vsipl init(argc, argv);

  typedef cscalar_f T;

  length_type const L = 16;
  length_type const M = 128;
  length_type const N = 1024;
  Vector<T> vector(L*M*N, T(2.f));
  Matrix<T> matrix(L*M, N, T(2.f));
  Tensor<T> tensor(L, M, N, T(2.f));

  TEST_REDUCTION(sumval, vector, matrix, tensor);
  TEST_REDUCTION(sumsqval, vector, matrix, tensor);
  TEST_REDUCTION(meanval, vector, matrix, tensor);
  TEST_REDUCTION(meansqval, vector, matrix, tensor);
}
