// Summary:
//    Contains the NL_NRinexUtils_H namespace and several functions for working 
//    with RINEX data.
//
// History:
//    Jan 09/13 - Created by Mark Petovello
//
// Copyright:
//    Position, Location And Navigation (PLAN) Group
//    Department of Geomatics Engineering
//    Schulich School of Engineering
//    University of Calgary
//
// Disclaimer:
//    This source code is not freeware nor shareware and is only provided under 
//    an agreement between authorized users/licensees and the University of 
//    Calgary (Position, Location And Navigation (PLAN) Group, Geomatics 
//    Engineering, Schulich School of Engineering) and may only be used under 
//    the terms and conditions set forth therein.

#ifndef NL_NRinexUtils_H
#define NL_NRinexUtils_H

#include <string>
#include <iostream>

#include "rinex.h"

namespace NRinexUtils
{
   //**
   // Summary:
   //    Get the type of RINEX file (e.g., observations, navigation, atc.)
   //
   // Arguments:
   //    None.
   //
   // Returns:
   //    A char containing the type of file.  This can be one of the typical characters that 
   //    end the file name (e.g., 'O', 'N', 'G', 'M', etc.)
   char GetFileType( const std::string& rinexFilename );


   //**
   // Summary:
   //    Open a generic RINEX file for input.
   //
   // Arguments:
   //    rinexFile - The file to be opened.
   //    rinexFilename - The name of the file.
   //
   // Returns:
   //    True if successful and false otherwise.
   bool OpenRinexFileForInput( NGSrinex::RinexFile &rinexFile, const std::string& rinexFilename );


   //**
   // Summary:
   //    Open a RINEX observation file for input.
   //
   // Arguments:
   //    rinexFile - The file to be opened.
   //    rinexFilename - The name of the file.
   //
   // Returns:
   //    True if successful and false otherwise.
   bool OpenRinexObservationFileForInput( NGSrinex::RinexObsFile &rinexFile, const std::string& rinexFilename );


   //**
   // Summary:
   //    Open a RINEX navigation (GPS ephemeris) file for input.
   //
   // Arguments:
   //    rinexFile - The file to be opened.
   //    rinexFilename - The name of the file.
   //
   // Returns:
   //    True if successful and false otherwise.
   bool OpenRinexNavigationFileForInput( NGSrinex::RinexNavFile &rinexFile, const std::string& rinexFilename );


   //**
   // Summary:
   //    Open a RINEX observation file for output.
   //
   // Arguments:
   //    rinexFile - The file to be opened.
   //    rinexFilename - The name of the file.
   //
   // Returns:
   //    True if successful and false otherwise.
   bool OpenRinexFileForOutput( NGSrinex::RinexFile &rinexFile, const std::string& rinexFilename );
   
};


#endif //NL_NRinexUtils_H