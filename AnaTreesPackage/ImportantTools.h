/**
 * \file ImportantTools.h
 *
 * \ingroup AnaTreesPackage
 * 
 * \brief Class def header for a class ImportantTools
 *
 * @author erezcohen
 */

/** \addtogroup AnaTreesPackage

    @{*/
#ifndef IMPORTANTTOOLS_H
#define IMPORTANTTOOLS_H

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

#include "LArUtil/../../UserDev/mySoftware/MySoftwarePackage/myIncludes.h"

/**
   \class ImportantTools
   User defined class ImportantTools ... these comments are used to generate
   doxygen documentation!
 */
class ImportantTools{

public:

  /// Default constructor
  ImportantTools(){}

  /// Default destructor
  ~ImportantTools(){}
    
    
    TTree* SchemeTreeRSEList ( TTree * , TString , Int_t );
    
    TTree* SchemeTreeRSEList ( TChain * chain , TString rse_file_name , Int_t fdebug )
    {return SchemeTreeRSEList ( (TTree *)chain ,  rse_file_name , fdebug );};
    
    std::map < int , std::map < int , int > > LoadRSEMap( TString, bool DoCheck = false);


};

#endif
/** @} */ // end of doxygen group 

