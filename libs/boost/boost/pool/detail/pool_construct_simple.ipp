// Copyright (C) 2000 Stephen Cleary
//
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org for updates, documentation, and revision history.

// This file was AUTOMATICALLY GENERATED from "pool_construct_simple.m4"
//  Do NOT include directly!
//  Do NOT edit!

template <typename T0>
element_type * construct(const T0 & a0)
{  
  malloc_guard ret(malloc(), this);
  
  if (ret.ptr == 0)
    return ret.ptr;
  new (ret.ptr) element_type(a0);
  ret.free_ptr = false;
  return ret.ptr;
}
template <typename T0, typename T1>
element_type * construct(const T0 & a0, const T1 & a1)
{  
  malloc_guard ret(malloc(), this);
  
  if (ret.ptr == 0)
    return ret.ptr;
  new (ret.ptr) element_type(a0, a1);
  ret.free_ptr = false;
  return ret.ptr;
}
template <typename T0, typename T1, typename T2>
element_type * construct(const T0 & a0, const T1 & a1, const T2 & a2)
{  
  malloc_guard ret(malloc(), this);
  
  if (ret.ptr == 0)
    return ret.ptr;
  new (ret.ptr) element_type(a0, a1, a2);
  ret.free_ptr = false;
  return ret.ptr;
}

