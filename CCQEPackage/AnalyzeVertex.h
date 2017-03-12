/**
 * \file AnalyzeVertex.h
 *
 * \ingroup CCQEPackage
 * 
 * \brief Class def header for a class AnalyzeVertex
 *
 * @author erezcohen
 */

/** \addtogroup CCQEPackage

    @{*/
#ifndef ANALYZEVERTEX_H
#define ANALYZEVERTEX_H

#include <iostream>
#include "../../mySoftware/MySoftwarePackage/myIncludes.h"
#include "PandoraNuTrack.h"
#include "cumputeAnaTree.h"
#include "myVertex.h"
#include "GENIEinteraction.h"
#include "MyLArTools.h"
#include "LArUtil/Geometry.h"

/**
   \class AnalyzeVertex
   User defined class AnalyzeVertex ... these comments are used to generate
   doxygen documentation!
 */
class AnalyzeVertex: public myIncludes {

public:

    AnalyzeVertex(){};
    ~AnalyzeVertex(){}
    AnalyzeVertex ( TTree * fInTree );

    void                  SetInTree (TTree * tree)       {InTree = tree;};
    void                  InitEvent ();
    void                   GetEntry ( int );
    void              InitInputTree ();

    
    
    Int_t   Nentries, NCC1pVertices , Nhits , Ntracks;

    TTree * InTree;
    std::vector<hit> hits;
    std::vector<PandoraNuTrack> tracks;
    std::vector<myVertex>       vertices;
    
    
    
    

};

#endif
/** @} */ // end of doxygen group 

