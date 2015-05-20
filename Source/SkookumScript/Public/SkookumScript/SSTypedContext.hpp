//=======================================================================================
// SkookumScript C++ library.
// Copyright (c) 2001 Agog Labs Inc.,
// All rights reserved.
//
// Class Type Scope Context
// # Author(s): Conan Reis
// # Notes:        
//=======================================================================================


#ifndef __SSTYPEDCONTEXT_HPP
#define __SSTYPEDCONTEXT_HPP


//=======================================================================================
// Includes
//=======================================================================================

#include <AgogCore/AList.hpp>
#include "SkookumScript/SSTyped.hpp"


//=======================================================================================
// Global Structures
//=======================================================================================

// Pre-declarations
class SSClassUnaryBase;
class SSExpressionBase;
class SSInstance;
class SSParameters;

#ifdef A_PLAT_PC
  template<class _ElementType> class AVCompactArrayBase;
  template<class _ElementType, class _KeyType = _ElementType> class AVCompactSortedLogical;
#else
  #include <AgogCore/AVCompactSorted.hpp>
#endif


//---------------------------------------------------------------------------------------
// Notes      Class Type Scope Context - used to track variable scope & type when
//            parsing/compiling.
// Author(s)  Conan Reis
struct SSTypeContext
  {
  // Nested Structures

    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    // Variables (and member variables that have their type changed) and their class types
    // for a specific scope level.
    struct ScopeVars : public AListNode<ScopeVars>
      {
      // Public Data Members

        // Available/type-modified variables at a particular scope.
        tSSTypedNames m_vars;

        // Variables that are no longer available - used to ensure unique names across scopes.
        tSSTypedNames m_var_history;

      // Methods

        ~ScopeVars() { empty(); }

        void empty() { m_vars.free_all(); m_var_history.free_all(); }

      };
  
    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    // Tracking info for a set of captured variables.
    struct CapturedVars : public AListNode<CapturedVars>
      {
      // Public Data Members

        // Variables captured by this closure - note that data members do not need to be
        // captured - just 'this'.
        tSSTypedNames m_vars;

        // Parent scope at which the capture was created
        tSSTypedNames * m_scope_p;

      // Methods

        CapturedVars(tSSTypedNames * scope_p) : m_scope_p(scope_p) {}

      };
  

  // Public Data Members

    // Class type scope - used to retrieve member data and methods
    SSClassUnaryBase * m_obj_scope_p;

    // If in a routine the parameters that are being used otherwise nullptr.
    // Used to determine the desired type for a return parameter.
    SSParameters * m_params_p;

    // Scope name - member/etc.
    ASymbol m_scope_name;

    // Initial variable scope on the stack - namely the arguments & temporaries created
    // within the topmost code block or single path nested code blocks.
    ScopeVars m_top_scope;

    // Stack of typed variables - local variables and any member variables that have had
    // their types changed.  Class types of variables can change throughout the life of a
    // code block.  A new variable scope is placed on the stack each time there are
    // alternate code paths to follow.
    AList<ScopeVars> m_scope_stack;

    // The current scope 
    ScopeVars * m_current_scope_p;
    tSSTypedNames * m_current_vars_p;

    // Captured temporary variables stack
    AList<CapturedVars> m_capture_stack;

    // Innermost capture context - if non-nullptr then there are one or more capture contexts.
    CapturedVars * m_capture_current_p;

  // Methods

    SS_NEW_OPERATORS(SSTypeContext);

    SSTypeContext();
    ~SSTypeContext();

    SSClassDescBase * finalize_generic(const SSClassDescBase & type) const;

    // Member Data Methods - variables belonging to m_obj_scope_p

      SSClassDescBase * get_member_type(const ASymbol & var_name, eSSScope * scope_p = nullptr) const;

    // Local Data Methods - variables from arguments and code block temporaries

      SSClassDescBase * get_rparam_type(const ASymbol & var_name) const;
      SSClassDescBase * get_local_variable_type(const ASymbol & var_name) const;

      void append_local(const ASymbol & var_name, SSClassDescBase * type_p);
      void archive_locals(const AVCompactArrayBase<ASymbol> & var_names);
      bool find_local_variable(const ASymbol & var_name) const;
      void free_locals(const AVCompactArrayBase<ASymbol> & var_names);
      void free_all_locals();
      bool is_locals() const;
      void merge(tSSTypedNames * merge_vars_p) const;
      void merge_locals(tSSTypedNames * merge_vars_p, bool first_path_b) const;
      void capture_locals();
      void capture_locals_stop(AVCompactSortedLogical<ASymbol> * captured_p);
      void nest_locals();
      void unnest_locals(eAHistory history = AHistory_remember);
      void accept_nest();

    // Combined Member & Local Methods

      void              change_variable_type(const ASymbol & var_name, SSClassDescBase * type_p);
      void              change_variable_types(const tSSTypedNames & vars);
      eSSScope          find_variable(const ASymbol & var_name) const;
      SSClassDescBase * get_variable_type(const ASymbol & var_name, bool skip_current_scope_b = false) const;
      bool              is_previous_variable(const ASymbol & var_name) const;
      bool              is_variable(const ASymbol & var_name) const;

  protected:

  // Internal Methods

    void capture_local(SSTypedName * var_p, CapturedVars * top_nesting_p) const;

  };  // SSTypeContext


//=======================================================================================
// Inline Functions
//=======================================================================================

#ifndef A_INL_IN_CPP
  #include "SkookumScript/SSTypedContext.inl"
#endif


#endif  // __SSTYPEDCONTEXT_HPP

