/**
 * \file MyTrack.h
 *
 * \ingroup CCQEPackage
 * 
 * \brief Class def header for a class MyTrack
 *
 * @author erezcohen
 */

/** \addtogroup CCQEPackage

    @{*/
#ifndef MYTRACK_H
#define MYTRACK_H

#include <iostream>
#include "../../mySoftware/MySoftwarePackage/myIncludes.h"
#include "PandoraNuTrack.h"
#include "cumputeAnaTree.h"

/**
   \class MyTrack
   User defined class MyTrack ... these comments are used to generate
   doxygen documentation!
 */
class MyTrack{

public:

    /// Default constructor
    MyTrack(){}
    MyTrack( int p );
    /// Default destructor
    ~MyTrack(){}
    

    void    SetPlane (int p) {plane = p;};
    
    int                 plane;
    std::string         color;  // matplotlib colors
    PandoraNuTrack      pandoraNu_track;
    std::vector<hit>    hits;

    
};

#endif
/** @} */ // end of doxygen group 

