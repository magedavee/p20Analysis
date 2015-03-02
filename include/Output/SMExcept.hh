/// \file SMExcept.hh \brief exception handling class
#ifndef SMEXCEPT_HH
/// make sure this file is only included once
#define SMEXCEPT_HH

#include <exception>
#include "Stringmap.hh"

/// exception class for error handling
class SMExcept: public std::exception, public Stringmap {
public:
    /// constructor
    SMExcept(const string& tp);
    /// destructor
    ~SMExcept() throw() {}
    /// display error
    virtual const char* what() const throw();
    /// string for holding error message
    mutable string msg;
};

/// replacement for assert(), raising SMExcept instance
void smassert(bool b, const string& tp = "assert_error", const Stringmap& m = Stringmap());

#endif
