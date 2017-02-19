/**
 * logwriter.h
 * Purpose: defines class LogWriter
 *
 * @author Wall.Of.Death
 * @version 1.0 20160629
 */

#ifndef LOGWRITER_H
#define LOGWRITER_H

#include <fstream>
#include <string>
#include <ctime>
#include "SLevel.h"

using namespace std;

/**
 * @brief The LogWriter class
 *
 * This class is a lightweight implementation for writing log-files.
 * The path of the log-file is set by the constructor.
 * The application which uses the LogWriter has to use a different
 * name in the constructor.
 * The LogWriter supports common syslog severity-levels defined by RFC 3164.
 * --> See slevel.h for severity-levels.
 * DO ALWAYS END A LOG COMMAND BY "endl"
 * (this ensures that your data is immediately flushed into the file)
 *
 * For use see the following
 * Example :
 *
 * #include "include/logwriter.h"
 *
 * int main() {
 *     LogWriter Lw("Application1", "Log.txt");
 *     Lw << SLevel(ERROR) << "Attention attention an error happend" << endl;
 *     Lw << SLevel(INFO) <<  "Just for infomation the value was " << 45 << endl;
 *
 *     return 0;
 * }
 *
 * Output (Log.txt)
 *   29/06/16-14:50:47 - ERROR     - Application1 - Attention attention an error happend
 *   29/06/16-14:50:47 - INFO      - Application1 - Just for infomation the value was 45
 */
class LogWriter : public ofstream {
    public:
        /* --- constructors --- */
        LogWriter() {};                         // standard-constructor
        LogWriter(string name_, string path_);  //          constructor
        LogWriter(const LogWriter& logWriter_); //     copy-constructor

        /* --- operator --- */
        LogWriter& operator=(const LogWriter& logWriter_);
        bool operator==(const LogWriter& logWriter_) const;
        bool operator!=(const LogWriter& logWriter_) const;

        /* --- miscellaneous --- */
        string getName() { return name; };
        string getTimeAsString();

    private:
        string name;
        string path;
};

#endif // LOGWRITER_H
