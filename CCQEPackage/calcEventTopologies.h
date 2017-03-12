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
#include "cumputeAnaTree.h"
#include "myVertex.h"
#include "GENIEinteraction.h"
#include "MyLArTools.h"
#include "LArUtil/Geometry.h"



/**
 \class calcEventTopologies
 User defined class calcEventTopologies ... these comments are used to generate
 doxygen documentation!
 */
class calcEventTopologies: public myIncludes {
    
public:
    
    
    
    /// Default constructor
    calcEventTopologies(){};
    ~calcEventTopologies(){}
    calcEventTopologies( TTree * fInTree );
    calcEventTopologies( TTree * fInTree,TTree * fOutTree,
                        std::string o="CC1pTopology",
                        int fdebug=0,
                        bool fMCmode=false,
                        float fmax_mu_p_distance=10);

    
    
    
    // setters
    void                  SetInTree (TTree * tree)       {InTree = tree;};
    void                 SetOutTree (TTree * tree)       {OutTree = tree;};
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
    void             InitOutputTree ();
    void                  InitEvent ();
    
    // running
    bool        extract_information ();
    void                GetRSEEntry ( int ); // get only R/S/E
    void                   GetEntry ( int );
    void              GetGENIEEntry ( int );
    
    int    CountCloseTracksToVertex ( myVertex );
    bool     MoreThanTwoCloseTracks ( myVertex );

    bool    ClusterTracksToVertices ();
    bool     ClusterGENIEToVertices ();
    bool            AnalyzeVertices ();
    bool      FindGENIECC1pVertices ();
    bool      FindTruthCC1pVertices ();
    bool FindVerticesWithCC1pTopology();
    bool        Find2tracksVertices ();
    bool               TagGENIECC1p ();
    bool                FillOutTree ();
    
    bool TrackAlreadyIncludedInVerticesList (int ftrack_id);
    void                      Print (bool DoPrintTracks=false, bool DoVertices=false);
    
    // getters
    PandoraNuTrack         GetTrack ( int i )        {return tracks.at(i);};
    
    TTree * InTree , * OutTree;
   
    int     debug ;
    bool    MCmode,     IsGENIECC1p,    FoundTruthCC1p;
    Int_t   Nentries,   run, 	subrun, event;
    Int_t   c_entry;
    Int_t   NCC1pVertices , Nhits , Ntracks;

    Int_t   Nprimaries, Np,     Nmu,    Npi ,   Nn,   Nel,  ccnc;

    
    float   max_mu_p_distance,  min_length_long, max_length_short, PIDA_short_min,  PIDA_short_max, PIDA_long_min,  PIDA_long_max;
    float   delta_phi_min,      delta_phi_max;
    std::string option;
    
    std::vector<hit> hits;
    
    
    PandoraNuTrack              c_track;
    std::vector<PandoraNuTrack> tracks,     protonTracks;
    
    myVertex                    c_vertex;
    std::vector<myVertex>       vertices,   CC1p_vertices;
    std::vector<int>            CC1pVerticesID;


    std::vector<TLorentzVector> protons;
    TLorentzVector              GENIEproton, GENIEmuon;
    PandoraNuTrack              protonTrack, muonTrack;
    
    
    std::vector<GENIEinteraction> genie_interactions;
    
    
    MyLArTools  * lar_tools;

};

#endif
/** @} */ // end of doxygen group

