/**
 * WebContentType.h
 * Purpose: defines interface WebContentType
 *
 * @author Wall.Of.Death
 * @version 1.0 20160705
 */

#ifndef WEBCONTENTTYPE_H
#define WEBCONTENTTYPE_H

#include "DataBuffer.h"

/**
 * WebContentType class
 * @brief The WebContentType class is an interface which outsources the readWebContent-function from WebContent-Class
 *
 * The design of the WebContentType-class is based on the design pattern 'strategy'.
 * In respect of this the WebContentType-class is the so called 'IStrategy' within the Strategy-Pattern.
 *
 * WebContentType can be implemented by different types of WebContent-classes.
 * If one wants to inherit from WebContentType one has to implement the readWebContent()-function.
 *
 *
 */
class WebContentType {
    public:
        WebContentType() {};
        virtual DataBuffer readWebContent() = 0;
};

#endif // WEBCONTENTTYPE_H
