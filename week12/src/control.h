/***********************************************************************
 * COMPONENT:
 *    CONTROL
 * Author:
 *    Br. Helfrich, <your name here if you made a change>
 * Summary:
 *    This class stores the notion of Bell-LaPadula
 ************************************************************************/

#pragma once

#include <assert.h>
#include <string>

enum Control {
    PUBLIC, CONFIDENTIAL, PRIVILEGED, SECRET
};

static std::string controlToString(Control control)
{
    switch(control) {
        case PUBLIC:
            return std::string("Public");
        case CONFIDENTIAL:
            return std::string("Confidential");
        case PRIVILEGED:
            return std::string("Privileged");
        case SECRET:
            return std::string("Secret");
        default:
            printf("Invalid control!\n");
            assert(false);
    }
}

static bool securityConditionRead(Control assetControl, Control subjectControl)
{
    return subjectControl >= assetControl;
}

static bool securityConditionWrite(Control assetControl, Control subjectControl)
{
    return subjectControl <= assetControl;
}
