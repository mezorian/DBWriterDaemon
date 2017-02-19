#ifndef HTTPREQUEST_H
#define HTTPREQUEST_H

#include <curl/curl.h>
#include <iostream>
// ---
#include "LogWriter.h"
#include "Config.h"

/**
 * HTTPRequest class
 * @brief The HTTPRequest class provides methods for executing http-post and http-get requests
 */
class HTTPRequest{
    public:
        HTTPRequest();
        bool post(const string& url_, const string& postFields_);
        string get(const string& url_);
    private:
        LogWriter log;

};

#endif // HTTPREQUEST_H
