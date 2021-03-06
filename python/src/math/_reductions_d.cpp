//
// Copyright (c) 2014 Stefan Seefeld
// All rights reserved.
//
// This file is part of OpenVSIP. It is made available under the
// license contained in the accompanying LICENSE.BSD file.

#include "reductions.hpp"

BOOST_PYTHON_MODULE(_reductions_d)
{
  using namespace pyvsip;
  import_array();
  define_reductions<1, double>();
  define_reductions<2, double>();
}
