/**
 * SLevel.cpp
 * Purpose: implements class SLevel
 *
 * @author Wall.Of.Death
 * @version 1.0 20160629
 */

#include "../include/SLevel.h"

/**
 * SLevel::severityLevelToString
 * @brief converts enum SeverityLevel into String
 * @param severityLevel_ enum which contains information about the type of log-message
 * @return returns string of always the same length, returns "" on error
 */
string SLevel::severityLevelToString(SeverityLevel severityLevel_) const {
    switch (severityLevel_){
        case EMERGENCY  : return "EMERGENCY"; break;
        case ALERT      : return "ALERT    "; break;
        case CRITICAL   : return "CRITICAL "; break;
        case ERROR      : return "ERROR    "; break;
        case WARNING    : return "WARNING  "; break;
        case NOTICE     : return "NOTICE   "; break;
        case INFO       : return "INFO     "; break;
        case DEBUG      : return "DEBUG    "; break;
        default : return "";
    }
}

/**
 * operator <<(LogWriter& logWriter_, const SLevel severityLevel_)
 * @brief writes timestamp, SeverityLevel and application name to LogWriter-stream
 * @param logWriter_ LogWriter-object before writing data into stream
 * @param severityLevel_ SLevel object which contains information about severityLevel as a string
 * @return returns LogWriter-object after writing data into stream
 *
 * This operator is used for to inform the LogWriter about the SeverityLevel dynamically for
 * every log-message.
 * But furthermore the LogWriter outputs not only the severityLevel, but also a timestamp and
 * the name of the application.
 * After writing
 *  - timestamp
 *  - SeverityLevel
 *  and
 *  - name of the application
 * to the stream a flush is executed, to flush the buffer to the file.
 *
 * For use see the following
 * Example :
 *   Lw << SLevel(ERROR) << "Attention attention an error happend" << endl;
 */
LogWriter& operator<<(LogWriter& logWriter_, const SLevel severityLevel_) {
    logWriter_ << logWriter_.getTimeAsString() << " - ";                // write timestamp
    logWriter_ << severityLevel_.getSeverityLevelAsString() << " - " ;  // write severity level
    logWriter_ << logWriter_.getName() << " - " << flush;               // write application name and flush
    return logWriter_;
}
