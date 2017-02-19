/**
 * WebContent.cpp
 * Purpose: implements class WebContent
 *
 * @author Wall.Of.Death
 * @version 1.0 20160705
 */

#include "../include/WebContent.h"

/**
 * WebContent::readWebContent
 * @brief readWebContent() reads the WebContent and returns current data of the webcontent as DataBuffer
 * readWebContent() reads the WebContent, which was set by setWebContentType and returns a
 * DataBuffer-object which contains the current data of the webcontent.
 * The DataBuffer is organized as follows :
 *
 *       string | value
 * -------------|-------
 * temperature  | 24
 * air pressure | 1022
 *
 * In practice readWebContent does not really read the WebContent, but it delegates the
 * communication with web-service to an implementation of the WebContentType-interface.
 * @return returns DataBuffer-object which contains the current data of the webcontent
 */
DataBuffer WebContent::readWebContent() {
    return webContentType->readWebContent();
}
