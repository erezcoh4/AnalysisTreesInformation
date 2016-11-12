/**
 * \file AnalyzeTracksFile.h
 *
 * \ingroup AnaTreesPackage
 *
 * \brief Class def header for a class AnalyzeTracksFile
 *
 * @author erezcohen
 */

/** \addtogroup AnaTreesPackage
 
 @{*/
#ifndef ANALYZETRACKSFILE_H
#define ANALYZETRACKSFILE_H

#include <iostream>
#include "../../mySoftware/MySoftwarePackage/myIncludes.h"
#include "PandoraNuTrack.h"

/**
 \class AnalyzeTracksFile
 User defined class AnalyzeTracksFile ... these comments are used to generate
 doxygen documentation!
 */
class AnalyzeTracksFile{
    
public:
    
    /// Default constructor
    AnalyzeTracksFile( TString fPath, TString fName = "BNB_5e19POT" , Int_t fdebug = 0 );
    ~AnalyzeTracksFile(){ TracksFile->Close(); }
    
    
    // setters
    void   SetTracksFilesPath ( TString fPath ) { Path = fPath;};
    void    SetTracksFileName ( TString fName ) { TracksFileName = Path + "/Tracks_" + fName + "_AnalysisTrees.root";};
    void             SetDebug (int _debug)      {debug = _debug;};
    
    // initializations
    void        InitInputTree ();
 
    
    // getters
    Int_t         GetEntries () {return TracksTree -> GetEntries(); };
    std::vector<PandoraNuTrack>  GetEventTracks (Int_t entry);

    TFile * TracksFile;
    TTree * TracksTree;
    
    TString     Path    , TracksFileName;
    
    Int_t       debug;  // 0 - quiet, 1 - major functionality, > 2 - print out all sorts of shit
    
    Int_t       run     , subrun    , event     , Ntracks;

    
    PandoraNuTrack c_track;
    std::vector<PandoraNuTrack> tracks , *tracks_pointer;

};

#endif
/** @} */ // end of doxygen group

