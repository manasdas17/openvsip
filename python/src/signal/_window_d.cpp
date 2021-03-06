//
// Copyright (c) 2014 Stefan Seefeld
// All rights reserved.
//
// This file is part of OpenVSIP. It is made available under the
// license contained in the accompanying LICENSE.BSD file.

#include "window.hpp"

BOOST_PYTHON_MODULE(_window_d)
{
  using namespace pyvsip;

  bpl::def("blackman", blackman<double>);
  bpl::def("cheby", cheby<double>);
  bpl::def("hanning", hanning<double>);
  bpl::def("kaiser", kaiser<double>);

}
