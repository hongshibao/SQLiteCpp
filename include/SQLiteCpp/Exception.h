/**
 * @file    Exception.h
 * @ingroup SQLiteCpp
 * @brief   Encapsulation of the error message from SQLite3 on a std::runtime_error.
 *
 * Copyright (c) 2012-2013 Sebastien Rombauts (sebastien.rombauts@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#pragma once

#include <stdexcept>
#include <string>
#include <sstream>
#include <sqlite3.h>


namespace SQLite
{


/**
 * @brief Encapsulation of the error message from SQLite3, based on std::runtime_error.
 */
class Exception : public std::runtime_error
{
public:
    /**
     * @brief Encapsulation of the error message from SQLite3, based on std::runtime_error.
     *
     * @param[in] aErrorMessage The string message describing the SQLite error
     */
    explicit Exception(const std::string& aErrorMessage) :
        std::runtime_error(aErrorMessage),
        errcode(0),
        extendedErrcode(0),
        errstr(),
        errmsg()
    {
    }
    
    /**
     * @brief Encapsulation of the error message from SQLite3, based on std::runtime_error.
     *
     * @param[in] apSQLite The SQLite object, to obtain detailed error messages from.
     * @param[in] ret Return value from function call that failed.
     */
    explicit Exception(sqlite3* apSQLite, int ret) :
        errcode(ret),
        extendedErrcode(sqlite3_extended_errcode(apSQLite)),
        errstr(sqlite3_errstr(ret)),
        errmsg(sqlite3_errmsg(apSQLite)),
        
        std::runtime_error([apSQLite, ret](){
            std::ostringstream message;
            message << sqlite3_errstr(ret) << ": " << sqlite3_errmsg(apSQLite);
            return message.str();
        }())
    {
    }
    
    const int errcode;
    const int extendedErrcode;
    const std::string errstr;
    const std::string errmsg;
};


}  // namespace SQLite


/// Compatibility with non-clang compilers.
#ifndef __has_feature
    #define __has_feature(x) 0
#endif

// Detect whether the compiler supports C++11 noexcept exception specifications.
#if (  defined(__GNUC__) && ((__GNUC__ == 4 && __GNUC_MINOR__ >= 7) || (__GNUC__ > 4)) \
    && defined(__GXX_EXPERIMENTAL_CXX0X__))
// GCC 4.7 and following have noexcept
#elif defined(__clang__) && __has_feature(cxx_noexcept)
// Clang 3.0 and above have noexcept
#elif defined(_MSC_VER) && _MSC_VER > 1800
// Visual Studio 2015 and above have noexcept
#else
    // Visual Studio 2013 does not support noexcept, and "throw()" is deprecated by C++11
    #define noexcept
#endif
