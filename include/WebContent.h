/**
 * WebContent.h
 * Purpose: defines class WebContent
 *
 * @author Wall.Of.Death
 * @version 1.0 20160705
 */

#ifndef WEBCONTENT_H
#define WEBCONTENT_H

#include "WebContentType.h"
#include "DataBuffer.h"

/**
 * WebContent class
 * @brief The WebContent class describes a WebContent of the type WebContentType
 *
 * The design of the WebContent-class is based on the design pattern 'strategy'.
 * In respect of this the WebContent-class is the so called 'Context' within the Strategy-Pattern.
 *
 * The WebContent class provides access to different kinds of WebContents, which are defined as implementations
 * of the interface WebContentType.
 * By setWebContentType() one can choose the type of the WebContent one wants to access.
 * The function readWebContent() returns a DataBuffer-object which contains the current data of the webcontent
 */
class WebContent {
    public:
        /* --- constructor --- */
        WebContent() {};

        /* --- getter and setter --- */
        void setWebContentType(WebContentType *val_) { webContentType = val_; };
        WebContentType* getWebContentType()          { return webContentType; };

        /* --- measuring --- */
        DataBuffer readWebContent();

    private:
        WebContentType *webContentType;
};

#endif // WEBCONTENT_H
