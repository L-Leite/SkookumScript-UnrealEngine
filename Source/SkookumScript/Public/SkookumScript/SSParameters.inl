//=======================================================================================
// SkookumScript C++ library.
// Copyright (c) 2001 Agog Labs Inc.,
// All rights reserved.
//
// Formal Parameter List/Interface Class
// # Author(s): Conan Reis
// # Notes:        
//=======================================================================================


//=======================================================================================
// Includes
//=======================================================================================

#include "SkookumScript/SSParameterBase.hpp"


//=======================================================================================
// Inline Methods
//=======================================================================================

//---------------------------------------------------------------------------------------
// #Description
//   Determines if any of the [unary] arguments a default expression set.  
//
// #Author(s) Conan Reis
A_INLINE bool SSParameters::is_defaulted() const
  {
  uint32_t param_count = m_params.get_length();

  if (param_count)
    {
    SSParameterBase ** params_pp     = m_params.get_array();
    SSParameterBase ** params_end_pp = params_pp + param_count;

    while (params_pp < params_end_pp)
      {
      if ((*params_pp)->is_defaultable())
        {
        return true;
        }

      params_pp++;
      }
    }

  return false;
  }

//---------------------------------------------------------------------------------------
// #Description
//   Determines if these parameters are sharable or not.  
//
// #Author(s) Conan Reis
A_INLINE bool SSParameters::is_sharable() const
  {
  // Don't share parameters that have defaults since they may have unique expression
  // source indexes.
  // $Revisit - CReis [Skookum Debug Only]  This check is only needed for serialization
  // with debug information.
  return !is_defaulted();
  }

//---------------------------------------------------------------------------------------
// #Description
//   Creates new structure or finds an existing one to reference
//
// #Modifiers static
// #Author(s) Conan Reis
A_INLINE SSParameters * SSParameters::get_or_create(
  SSClassDescBase * result_type_p,
  // Optional single unary parameter - No parameter if nullptr.
  SSParameterBase * param_p // = nullptr
  )
  {
  SSParameters params(result_type_p, param_p);

  return get_or_create(&params);
  }


#if (SKOOKUM & SS_COMPILED_IN)

//---------------------------------------------------------------------------------------
// #Description
//   Creates new structure or finds an existing one to reference
//
// #Modifiers static
// #Author(s) Conan Reis
A_INLINE SSParameters * SSParameters::get_or_create(
  // Pointer to address to read binary serialization info from and to increment
  // - previously filled using as_binary() or a similar mechanism.
  const void ** binary_pp
  )
  {
  SSParameters params(binary_pp);

  return get_or_create(&params);
  }

#endif
