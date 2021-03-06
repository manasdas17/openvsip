//
// Copyright (c) 2009 by CodeSourcery
// Copyright (c) 2013 Stefan Seefeld
// All rights reserved.
//
// This file is part of OpenVSIP. It is made available under the
// license contained in the accompanying LICENSE.GPL file.

/// Description
///   Benchmark for vector minus

#include <iostream>

#include <vsip/initfin.hpp>
#include <vsip/support.hpp>
#include <vsip/math.hpp>
#include <vsip/random.hpp>
#include "../benchmark.hpp"

using namespace vsip;


/***********************************************************************
  Definitions - vector element-wise minus
***********************************************************************/

template <typename T>
struct t_vminus1 : Benchmark_base
{
  char const* what() { return "t_vminus1"; }
  int ops_per_point(length_type)  { return 1; }
  int riob_per_point(length_type) { return 1*sizeof(T); }
  int wiob_per_point(length_type) { return 1*sizeof(T); }
  int mem_per_point(length_type)  { return 2*sizeof(T); }

  void operator()(length_type size, length_type loop, float& time) OVXX_NOINLINE
  {
    Vector<T>   A(size, T());
    Vector<T>   C(size);

    Rand<T> gen(0, 0);
    A = gen.randu(size);

    A.put(0, T(16));

    timer t1;
    for (index_type l=0; l<loop; ++l)
      C = -A;
    time = t1.elapsed();
    
    if (!equal(C.get(0), -T(16)))
    {
      std::cout << "Oops!  C.get(0) is " << C.get(0) << "\n";
      std::cout << "t_vminus1: ERROR" << std::endl;
      abort();
    }

    for (index_type i=0; i<size; ++i)
      test_assert(equal(C.get(i), -A.get(i)));
  }

  void diag()
  {
    length_type const size = 256;

    Vector<T>   A(size, T());
    Vector<T>   C(size);

    std::cout << ovxx::assignment::diagnostics(C, -A) << std::endl;
  }
};


#ifdef VSIP_IMPL_SOURCERY_VPP
template <typename T, vsip::storage_format_type F>
struct t_vminus2 : Benchmark_base
{
  // compile-time typedefs
  typedef Layout<1, row1_type, dense, F> LP;
  typedef impl::Strided<1, T, LP> block_type;

  // benchmark attributes
  char const* what() { return "t_vminus2"; }
  int ops_per_point(length_type)  { return 2; }
  int riob_per_point(length_type) { return 1*sizeof(T); }
  int wiob_per_point(length_type) { return 1*sizeof(T); }
  int mem_per_point(length_type)  { return 2*sizeof(T); }

  void operator()(length_type size, length_type loop, float& time) OVXX_NOINLINE
  {
    Vector<T, block_type> A(size, T());
    Vector<T, block_type> C(size);

    Rand<T> gen(0, 0);
    A = gen.randu(size);

    A.put(0, T(3));
    
    timer t1;
    for (index_type l=0; l<loop; ++l)
      C = -A;
    time = t1.elapsed();
    
    for (index_type i=0; i!=size; ++i)
      if (!equal(C.get(i), -A.get(i)))
      {
	std::cout << "Check: C.get(" << i << ") is " << C.get(i) << "\n";
	break;
      }
  }

  void diag()
  {
    length_type const size = 256;

    Vector<T, block_type> A(size, T());
    Vector<T, block_type> C(size);

    std::cout << ovxx::assignment::diagnostics(C, -A) << std::endl;
  }
};
#endif // VSIP_IMPL_SOURCERY_VPP

void defaults(Loop1P&) {}

int
benchmark(Loop1P& loop, int what)
{
  switch (what)
  {
  case  1: loop(t_vminus1<float>()); break;
  case  2: loop(t_vminus1<complex<float> >()); break;
#ifdef VSIP_IMPL_SOURCERY_VPP
  case  3: loop(t_vminus2<complex<float>, interleaved_complex>()); break;
  case  4: loop(t_vminus2<complex<float>, split_complex>()); break;
#endif

  case  0:
    std::cout
      << "vminus -- vector minus\n"
      << "                F  - float\n"
      << "                CF - complex<float>\n"
      << "   -1 -- vector element-wise minus -- F/F \n"
      << "   -2 -- vector element-wise minus -- CF/CF\n"
      << "   -3 -- Vector<complex<float>> (INTER)\n"
      << "   -4 -- Vector<complex<float>> (SPLIT)\n"
      ;
  default:
    return 0;
  }
  return 1;
}
