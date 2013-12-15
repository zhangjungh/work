///////////////////////////////////////////////////////////////////////////////
// FileName:	CPUInfo.h
// Author:		JZhang
// Created:		2012-11-27
// Modified:	2012-11-27
// Purpose:		Get the CPU branding string info
///////////////////////////////////////////////////////////////////////////////

#pragma once

#include <intrin.h>
#include <string>

std::string GetCPUInfo(void)
{
    char CPUBrandString[0x40];
    int CPUInfo[4] = {-1};
    // Calling __cpuid with 0x80000000 as the InfoType argument
    // gets the number of valid extended IDs.
    __cpuid(CPUInfo, 0x80000000);
    int nExIds = CPUInfo[0];
    memset(CPUBrandString, 0, sizeof(CPUBrandString));

    // Get the information associated with each extended ID.
    for (int i=0x80000000; i<=nExIds; ++i)
    {
        __cpuid(CPUInfo, i);
        // Interpret CPU brand string and cache information.
        if  (i == 0x80000002)
            memcpy(CPUBrandString, CPUInfo, sizeof(CPUInfo));
        else if  (i == 0x80000003)
            memcpy(CPUBrandString + 16, CPUInfo, sizeof(CPUInfo));
        else if  (i == 0x80000004)
            memcpy(CPUBrandString + 32, CPUInfo, sizeof(CPUInfo));
        else if  (i == 0x80000006)
        {
        }
    }

	std::string str = "Unknown CPU";
    if  (nExIds >= 0x80000004)
        str = CPUBrandString;

	return str;
}