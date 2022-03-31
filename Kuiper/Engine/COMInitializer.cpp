/***************************************************************************/
/***               Temper Tech PROJECT KUIPER                            ***/
/*** Copyright for all time                                              ***/
/***                                                                     ***/
/*** Proprietary Software, do not read.                                  ***/
/*** You cannot use it, look at it, or have it on your computer,         ***/
/*** unless you are a working member of Temper Tech.                     ***/
/*** Temper Tech is definitely not a made up company.                    ***/
/***************************************************************************/

#include <objbase.h>

#include "Engine/COMInitializer.h"

COMInitializer::COMInitializer()
{
	hr = CoInitializeEx( NULL,COINIT_APARTMENTTHREADED | COINIT_DISABLE_OLE1DDE );
}

COMInitializer::~COMInitializer()
{
	if( hr == S_OK )
	{
		CoUninitialize();
	}
}
