//=======================================================================================
// SkookumScript C++ library.
// Copyright (c) 2001 Agog Labs Inc.,
// All rights reserved.
//
// Invocation classes: SSInvokeBase, SSInvocation, SSInvokeSync, SSInvokeRace,
// SSInvokeCascade, SSInstantiate and SSCopyInvoke
// 
// #Author(s):  Conan Reis
//=======================================================================================


//=======================================================================================
// Includes
//=======================================================================================


//=======================================================================================
// SSInvokeBase Inline Methods
//=======================================================================================

//---------------------------------------------------------------------------------------
//  Constructor
// # Returns:   itself
// Arg          call_name - name of the invokable (method or coroutine) to call
//              (Default ASymbol::ms_null)
// Arg          scope_p - Optional overriding class scope for specifically qualified /
//              scoped calls - i.e. "actor _attack()" vs. "actor Character@_attack()"
// # Author(s):  Conan Reis
A_INLINE SSInvokeBase::SSInvokeBase(
  const ASymbol & call_name, // = ASymbol::ms_null,
  SSClass *       scope_p    // = nullptr
  ) :
  SSQualifier(call_name, scope_p)
  {
  }

//---------------------------------------------------------------------------------------
// Transfer copy constructor - takes over internal contents of supplied
//             'call_p' and sets it to default values.
// # Returns:  itself
// Arg         call_p - SSInvokeBase object to take over contents of
// # Notes:    This method is useful when the contents of a local stack allocated
//             SSInvokeBase derived object needs to be promoted to a dynamic heap allocated
//             object.
// # Author(s): Conan Reis
A_INLINE SSInvokeBase::SSInvokeBase(SSInvokeBase * call_p) :
  SSQualifier(*call_p),
  m_arguments(&call_p->m_arguments)
  {
  }


#if (SKOOKUM & SS_COMPILED_OUT)

//---------------------------------------------------------------------------------------
// Fills memory pointed to by binary_pp with the information needed to
//             recreate this call (without knowing what type of call it is)
//             and increments the memory address to just past the last byte written.
// Arg         binary_pp - Pointer to address to fill and increment.  Its size *must* be
//             large enough to fit all the binary data.  Use the get_binary_length()
//             method to determine the size needed prior to passing binary_pp to this
//             method.
// # See:      as_binary_typed_length(), from_binary_typed_new(), as_binary(),
//             as_binary_length()
// # Notes:    Used in combination with as_binary_length_typed() and from_binary_new()
//
//             Binary composition:
//               1 byte  - call type
//               n bytes - call binary
//
// # Author(s): Conan Reis
A_INLINE void SSInvokeBase::as_binary_typed(void ** binary_pp) const
  {
  // 1 byte - call type
  **(uint8_t **)binary_pp = static_cast<uint8_t>(get_invoke_type());
  (*(uint8_t **)binary_pp)++;

  // n bytes - call binary
  as_binary(binary_pp);
  }

//---------------------------------------------------------------------------------------
// Returns length of binary version of itself plus it's type information in
//             bytes.
// # Returns:  length of binary version of itself plus type information in bytes
// # See:      as_binary_typed(), from_binary_typed_new(), as_binary(), as_binary_length()
// # Notes:    Used in combination with as_binary_typed()
//
//             Binary composition:
//               1 byte  - call type
//               n bytes - call binary
// # Modifiers: virtual from SSInvokeBase
// # Author(s): Conan Reis
A_INLINE uint32_t SSInvokeBase::as_binary_typed_length() const
  {
  return 1u + as_binary_length();
  }

#endif // (SKOOKUM & SS_COMPILED_OUT)


//=======================================================================================
// SSInvocation Inline Methods
//=======================================================================================

//---------------------------------------------------------------------------------------
//  Constructor
// # Returns:   itself
// Arg          call_p - invocation selector (SSCoroutineCall or SSMethodCall)
// Arg          receiver_p - expression that is the 'target' of this invocation.  If nullptr
//              is given then 'this' - i.e. the topmost scope - is inferred.
//              (Default nullptr)
// # Author(s):  Conan Reis
A_INLINE SSInvocation::SSInvocation(
  SSInvokeBase *     call_p,
  SSExpressionBase * receiver_p // = nullptr
  ) :
  m_receiver_p(receiver_p),
  m_call_p(call_p)
  {
  }


#if (SKOOKUM & SS_COMPILED_IN)

//---------------------------------------------------------------------------------------
// Constructor from binary info
// # Returns:  itself
// Arg         binary_pp - Pointer to address to read binary serialization info from and
//             to increment - previously filled using as_binary() or a similar mechanism.
// # See:      as_binary()
// # Notes:    Binary composition:
//               n bytes - receiver expression typed binary or inferred this (nullptr)
//               n bytes - call typed binary
//
//             Little error checking is done on the binary info as it assumed that it
//             previously validated upon input.
// # Author(s): Conan Reis
A_INLINE SSInvocation::SSInvocation(const void ** binary_pp)
  {
  // n bytes - receiver expression or inferred this (nullptr)
  m_receiver_p = SSExpressionBase::from_binary_typed_new(binary_pp);

  // n bytes - call
  m_call_p = SSInvokeBase::from_binary_typed_new(binary_pp);
  }

#endif // (SKOOKUM & SS_COMPILED_IN)


