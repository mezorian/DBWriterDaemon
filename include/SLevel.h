/**
 * SLevel.cpp
 * Purpose: define class SLevel and enum SeverityLevel
 *
 * @author Wall.Of.Death
 * @version 1.0 20160629
 */

#ifndef SLEVEL_H
#define SLEVEL_H

// declare class LogWriter for to avoid include problems
class LogWriter;

#include <string>
#include "LogWriter.h"

using namespace std;

/**
 * @brief The SeverityLevel enum
 * The list of severities is also defined by RFC 3164
 * source : https://en.wikipedia.org/wiki/Syslog#Severity_level
 *  Val | Severity      | Description             | Examples
 *  ------------------------------------------------------------------------------
 *   0  | Emergency     | System  is  unusable    | This  level  should  not
 *      |               |                         | be  used  by  applications.
 *   1  | Alert         | Should  be  corrected   | Loss  of  the  primary  ISP
 *      |               | immediately             | connection.
 *   2  | Critical      | Critical  conditions    | A  failure  in  the  system's
 *      |               |                         | primary  application.
 *   3  | Error         | Error  conditions       | An  application  has  exceeded
 *      |		        |                         | its  file  storage  limit  and
 *      | 		        |                         | attempts  to  write  are
 *      |               |                         | failing.
 *   4  | Warning       | May  indicate  that  an | A  non-root  file  system  has
 *      |               | error  will  occur  if  | only  2GB   remaining.
 *      |               | action  is  not  taken. |
 *   5  | Notice        | Events  that  are  un-  |
 *      |               | usual,  but  not  error |
 *      |               | conditions.             |
 *   6  | Informational | Normal  operational     | An  application  has  started,
 *      |               | messages  that  require | paused  or  ended  successfully.
 *      |               | no  action.             |
 *   7  | Debug         | Information  useful  to |
 *      |               | developers  for  debug- |
 *      |               | ging  the  application. |
 *
 */
enum SeverityLevel {EMERGENCY, ALERT, CRITICAL, ERROR, WARNING, NOTICE, INFO, DEBUG};

/**
 * @brief The SLevel class
 *
 * SLevel class is a wrapper class, which wrapps the SeverityLevel enum.
 * This is necessary because it is not possible to overload the << operator for an enum.
 */
class SLevel {
    public:
        // constructor
        SLevel(SeverityLevel severityLevel_)      { severityLevel = severityLevel_;                    }

        // getters
        SeverityLevel getSeverityLevel  () const  { return severityLevel;                              }
        string getSeverityLevelAsString () const  { return severityLevelToString(getSeverityLevel());  }

        // operators
        friend LogWriter& operator<<(LogWriter& logWriter_, const SLevel severityLevel_);


    private:
        SeverityLevel severityLevel;
        string severityLevelToString(SeverityLevel severityLevel_) const;
};

#endif // SLEVEL_H
