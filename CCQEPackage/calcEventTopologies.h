/**
 * \file calcEventTopologies.h
 *
 * \ingroup CCQEPackage
 *
 * \brief Class def header for a class calcEventTopologies
 *
 * @author erezcohen
 */

/** \addtogroup CCQEPackage
 
 @{*/
#ifndef CALCEVENTTOPOLOGIES_H
#define CALCEVENTTOPOLOGIES_H

#include <iostream>
#include "../../mySoftware/MySoftwarePackage/myIncludes.h"
#include "PandoraNuTrack.h"
#include "myVertex.h"

//#include "GeoAlgo.h"
//#include "nuInteraction.h"
//#include "GENIEinteraction.h"
//#include "track_vertex.h"
//#include "mutual_vertex.h"
//#include "LArG4Particle.h"

/**
 \class calcEventTopologies
 User defined class calcEventTopologies ... these comments are used to generate
 doxygen documentation!
 */
class calcEventTopologies{
    
public:
    
    
    
    /// Default constructor
    calcEventTopologies(){};
    ~calcEventTopologies(){}
    calcEventTopologies( TTree * fInTree,
                        int fdebug=0,
                        bool fMCmode=false,
                        float fmax_mu_p_distance=10);

    
    
    
    
    // setters
    void                  SetInTree (TTree * tree)       {InTree = tree;};
    void                   SetDebug (int _debug)         {debug = _debug;};
    void                  SetMCMode (bool _mc_mode)      {MCmode = _mc_mode;};
    void          SetMaxmupDistance (float fmax)         {max_mu_p_distance = fmax;};
    void           SetMinLengthLong (float fmin)         {min_length_long = fmin;};
    void          SetMaxLengthShort (float fmax)         {max_length_short = fmax;};
    void              SetDetaPhiMin (float fmin)         {delta_phi_min = fmin;};
    void              SetDetaPhiMax (float fmax)         {delta_phi_max = fmax;};
    void            SetPIDAShortMin (float fmin)         {PIDA_short_min = fmin;};
    void            SetPIDAShortMax (float fmax)         {PIDA_short_max = fmax;};
    void             SetPIDALongMin (float fmin)         {PIDA_long_min = fmin;};
    void             SetPIDALongMax (float fmax)         {PIDA_long_max = fmax;};
    void                  SetOption (std::string o="CC1pTopology" ) {option=o;};
    void SetVerticesDeltaPhiLongestandShortestTracks();

    
    // initializations
    void              InitInputTree ();
    void                  InitEvent ();
    
    // running
    bool        extract_information ();
    void                   GetEntry ( int );
    bool    ClusterTracksToVertices ();
    bool            AnalyzeVertices ();
    bool      FindTruthCC1pVertices ();
    bool FindVerticesWithCC1pTopology();
    
    bool TrackAlreadyIncludedInVerticesList (int ftrack_id);
    void                      Print (bool DoTracks=false, bool DoVertices=true);
    
    
    // getters
    PandoraNuTrack         GetTrack ( int i )        {return tracks.at(i);};
    
    TTree * InTree;
    
    int     debug ;
    bool    MCmode;
    Int_t   Nentries,   run, 	subrun,     event;
    Int_t   Ntracks ,   c_entry;
    
    
    float   max_mu_p_distance,  min_length_long, max_length_short, PIDA_short_min,  PIDA_short_max, PIDA_long_min,  PIDA_long_max;
    float   delta_phi_min,      delta_phi_max;
    std::string option;
    
    
    PandoraNuTrack              c_track;
    std::vector<PandoraNuTrack> tracks;
    
    myVertex                    c_vertex;
    std::vector<myVertex>       vertices,   CC1p_vertices;
    std::vector<int>            CC1pVerticesID;


};

#endif
/** @} */ // end of doxygen group

