#include "HTTPRequest.h"

/* --- constructor --- */
/**
 * HTTPRequest::HTTPRequest
 * @brief constuctor of class HTTPRequest, initializes local LogWriter object.
 */
HTTPRequest::HTTPRequest() {
    // create LogWriter-object
    log = LogWriter("HTTPRequest", PATH_OF_LOGFILE);
}

/* --- http - requests --- */
/**
 * HTTPRequest::post
 * @brief executes a http-post-request for the given url
 * @param url_ the URL which is requested
 * @return returns true if request executed successfully, otherwise false
 */
bool HTTPRequest::post(const string &url_, const string &postFields_) {
    bool noError = true;
    CURL *curl;
    CURLcode res;

    // In windows, this will init the winsock stuff
    curl_global_init(CURL_GLOBAL_ALL);

    // get a curl handle
    curl = curl_easy_init();
    if(curl) {
        // Set the URL that is about to receive our POST
        curl_easy_setopt(curl, CURLOPT_URL, url_.c_str());
        // Specify the POST data
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, postFields_.c_str());
        // detect 404 errors
        curl_easy_setopt(curl, CURLOPT_FAILONERROR, true);

        // Perform the request, res will get the return code
        res = curl_easy_perform(curl);
        // Check for errors
        if(res != CURLE_OK) {
            log << SLevel(ERROR) << "Aborted http-post-request, because " <<
                   "curl_easy_perform() failed with " << curl_easy_strerror(res) << endl;
            noError = false;
        }

        // cleanup
        curl_easy_cleanup(curl);
    } else {
        log << SLevel(ERROR) << "Aborted http-post-request, because it was not " <<
               "possible to get a curl handle by using curl_easy_init" << endl;
        noError = false;

    }
    curl_global_cleanup();
    return noError;
}

/**
 * WriteCallback
 * @brief static callback function for libcurl
 * @param contents_ data to write
 * @param size_ size of datatype
 * @param nmemb number of of members with the size 'size_'
 * @param userp user pointer
 * @return returns size of written data (size_ * nmemb_)
 */
static size_t writeCallback(void *contents_, size_t size_, size_t nmemb_, void *userp_) {
    ((string*)userp_)->append((char*)contents_, size_ * nmemb_);
    return size_ * nmemb_;
}

/**
 * HTTPRequest::get
 * @brief executes a http-get-request for the given url
 * @param url_ the URL which is requested
 * @return returns answer of http request as string
 */
string HTTPRequest::get(const string &url_) {
    CURL *curl;
    CURLcode res;
    string readBuffer;

    // get a curl handle
    curl = curl_easy_init();
    if(curl) {
        // Set the URL that is about to receive our POST
        curl_easy_setopt(curl, CURLOPT_URL, url_.c_str());
        // set callback function
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writeCallback);
        // set text buffer for answer
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);
        // detect 404 errors
        curl_easy_setopt(curl, CURLOPT_FAILONERROR, true);
        // perform request
        res = curl_easy_perform(curl);

        // check for errors
        if(res != CURLE_OK) {
            log << SLevel(ERROR) << "Aborted http-get-request, because " <<
                   "curl_easy_perform() failed with " << curl_easy_strerror(res) << endl;
            return "";
        }


        // clean up
        curl_easy_cleanup(curl);
        curl_global_cleanup();

        // return answer of request as string
        return readBuffer;

    } else {
        log << SLevel(ERROR) << "Aborted http-post-request, because it was not " <<
               "possible to get a curl handle by using curl_easy_init" << endl;
        return "";
    }
}
