// Copyright (c) 2017
// Utrecht University (The Netherlands),
// ETH Zurich (Switzerland),
// INRIA Sophia-Antipolis (France),
// Max-Planck-Institute Saarbruecken (Germany),
// and Tel-Aviv University (Israel).  All rights reserved.
//
// This file is part of CGAL (www.cgal.org); you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public License as
// published by the Free Software Foundation; either version 3 of the License,
// or (at your option) any later version.
//
// This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
// WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
//
// $URL$
// $Id$
//
//
// Author(s)     : Mael Rouxel-Labbé

#ifndef CGAL__TEST_CLS_WEIGHTED_POINT_2_H
#define CGAL__TEST_CLS_WEIGHTED_POINT_2_H

#include <CGAL/Origin.h>
#include <CGAL/Point_2.h>
#include <CGAL/use.h>
#include <CGAL/Weighted_point_2.h>

#include <cassert>
#include <iostream>

using CGAL::internal::use;

template <class R>
bool _test_cls_weighted_point_2(const R& )
{
  std::cout << "Testing class Weighted_point_2" ;

  typedef typename  R::RT    RT;
  typedef typename  R::FT    FT;

  RT  n1(-35 );
  RT  n2( 50 );
  RT n4(  5);
  FT iw = -1;

  CGAL::Point_2<R> p0(n1, n2, n4);

  // constructions
  typename R::Weighted_point_2       iwp;
  CGAL::Weighted_point_2<R> wp0; // default
  CGAL::Weighted_point_2<R> wp1(CGAL::ORIGIN); // with origin
  CGAL::Weighted_point_2<R> wp2(p0); // with Point_2
  CGAL::Weighted_point_2<R> wp3(p0, iw); // with Point_2 and weight
  CGAL::Weighted_point_2<R> wp4(iwp); // with R::Weighted_point_2
  CGAL::Weighted_point_2<R> wp5(wp3); // with CGAL::Weighted_point_2< R >
  CGAL::Weighted_point_2<R> wp6(n1, n2); // with coordinates
  use(wp0); use(wp4); use(wp5);

  // assignement
  wp1 = wp6;

  std::cout << ".";

  // accessors
  CGAL::Point_2<R> p1 = wp2.point();
  assert(p1 == p0);

  FT w = wp3.weight();
  assert(w == iw);

  // no need to test the other operations as they use Point_2 operations (which
  // are tested in _test_cls_point_2.h)

  std::cout << "done" << std::endl;
  return true;
}

#endif // CGAL__TEST_CLS_WEIGHTED_POINT_2_H
