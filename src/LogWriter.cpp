/**
 * LogWriter.cpp
 * Purpose: implements class LogWriter
 *
 * @author Wall.Of.Death
 * @version 1.0 20160629
 */

#include "../include/LogWriter.h"

using namespace std;

/* --- constructors --- */

/**
 * LogWriter::LogWriter(string name_, string path_)
 * @brief constructor of class LogWriter, opens file at path_
 * @param name_ name of application which uses the LogWriter-Class
 * @param path_ path of log-file
 *
 * Constructor sets the attributes name and path and opens
 * the stream to a file at path "path_" in append mode.
 * If the file does not exist, it is created
 */
LogWriter::LogWriter(string name_, string path_){
   name = name_;
   path = path_;
   // open file and append everything to the content which is already in the file
   open(path_.c_str(), std::ios_base::app);
}

/**
 * LogWriter::LogWriter
 * @brief copy constructor of class LogWriter, executes normal constructor
 * @param logWriter_ LogWriter-object to copy
 */
LogWriter::LogWriter(const LogWriter& logWriter_) {
    *this = LogWriter(logWriter_.name, logWriter_.path);
}

/* --- operator --- */
/**
 * LogWriter::operator =
 * @brief assignment-operator, assigns values of LogWriter to *this and opens stream
 * @param logWriter_ LogWriter-object to assign
 * @return *this with assign values of logWriter_
 */
LogWriter& LogWriter::operator=(const LogWriter& logWriter_) {
    // check for identical addresses
    if (this == logWriter_) {
        return *this;
    }

    // assign values
    name = logWriter_.name;
    path = logWriter_.path;

    // open file and append everything to the content which is already in the file
    open(path.c_str(), std::ios_base::app);

    return *this;
}

/**
 * LogWriter::operator ==
 * @brief comparison-operator, which compares two LogWriter-objects
 * @param logWriter1_ value one to compare
 * @param logWriter2_ value two to compare
 * @return returns whether all attributes of logWriter1 and logWriter2 are equal
 */
bool LogWriter::operator==(const LogWriter& logWriter_) const {
    return ( (name == logWriter_.name) &&
             (path == logWriter_.path)    );
}

/**
 * LogWriter::operator !=
 * @brief not equal-operator, which compares two LogWriter-objects
 * @param logWriter1_ value one to compare
 * @param logWriter2_ value two to compare
 * @return returns whether all attributes of logWriter1 and logWriter2 are not equal
 */
bool LogWriter::operator!=(const LogWriter& logWriter_) const {
    return !(*this == logWriter_);
}

/* --- miscellaneous --- */

/**
 * LogWriter::getTimeAsString()
 * @brief returns date and time as a dd/mm/yy-hh:mm:ss formatted string
 * @return returns date and time as a dd/mm/yy-hh:mm:ss formatted string
 *
 * time refers to timezone of the local machine
 */
string LogWriter::getTimeAsString() {
    time_t rawtime;                // create time_t
    struct tm *structTimezoneTime; // create time pointer
    char buffer[80];               // create buffer for string

    time(&rawtime);  // get current time into rawtime (seconds since 1970)
    structTimezoneTime = localtime(&rawtime); // transform rawtime by timezone to localtime

    // time_struct-->string
    // returns string like 29/06/16-11:15:10
    strftime (buffer,sizeof(buffer),"%d/%m/%y-%T",structTimezoneTime);
    return buffer;
}
