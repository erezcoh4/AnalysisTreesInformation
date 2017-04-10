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
    MyTrack( int p , PandoraNuTrack fpandoraNu_track );
    /// Default destructor
    ~MyTrack(){}
    

    void                   SetPlane (int p)                       {plane = p;};
    void          SetPandoraNuTrack (PandoraNuTrack t)            {pandoraNu_track = t;};
    void                    SetHits (std::vector<hit> fhits)      {for (auto hit:fhits) hits.push_back(hit);};
    void            SetPossibleHits (std::vector<hit> fhits)      {for (auto hit:fhits) possible_hits.push_back(hit);};
    
    float      GetTotalChargeInHits ();
    float GetTotalChargeInHitsInROI (box roi);
    
    int                 plane;
    std::string         color;  // matplotlib colors
    PandoraNuTrack      pandoraNu_track;
    std::vector<hit>    possible_hits, hits;

    
};

#endif
/** @} */ // end of doxygen group 

