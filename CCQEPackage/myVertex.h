/**
 * \file myVertex.h
 *
 * \ingroup CCQEPackage
 * 
 * \brief Class def header for a class myVertex
 *
 * @author erezcohen
 */

/** \addtogroup CCQEPackage

    @{*/
#ifndef MYVERTEX_H
#define MYVERTEX_H

#include <iostream>
#include "../../mySoftware/MySoftwarePackage/myIncludes.h"
#include "PandoraNuTrack.h"
#include "GENIEinteraction.h"

#define r2d TMath::RadToDeg()
#define d2r TMath::DegToRad()

/**
   \class myVertex
   User defined class myVertex ... these comments are used to generate
   doxygen documentation!
 */
class myVertex{

public:

  /// Default
    myVertex(){}
    ~myVertex(){}
    myVertex(Int_t fID);

    
    void        SetVertexID (Int_t fvertex_id)              {vertex_id = fvertex_id;};
    void             SetRSE (Int_t r, Int_t s, Int_t e)     {run=r; subrun=s; event=e;};
    void         AddTrackID (Int_t ftrack_id)               {track_id.push_back(ftrack_id);};
    void           AddTrack (PandoraNuTrack ftrack)         {tracks.push_back(ftrack); Ntracks=(int)tracks.size();};
    void        SetPosition (TVector3 fposition)            {position = fposition;};
    void              Print ();
    bool      IncludesTrack (Int_t ftrack_id);
    bool SortTracksByLength ();
    bool   SortTracksByPIDA ();
    bool  SubtractFarTracks ();

    
    //    bool   SetTruthTopology (); // deprecated: GENIE flags CC1p using GENIECC1p
    bool  SetKinematicalTopology (float min_length_long     =50,    // cm
                                  float max_length_short    =30,    // cm
                                  float delta_phi_min       =-360,  // deg.
                                  float delta_phi_max       =360,   // deg.
                                  float PIDA_short_min      =0, float PIDA_short_max=50,
                                  float PIDA_long_min       =0, float PIDA_long_max=50
    );
    
    
    vector<size_t>        sort_l (const vector<PandoraNuTrack> &v);
    vector<size_t>     sort_pida (const vector<PandoraNuTrack> &v);
    
    
    // SETters
    void SetTracksRelations ();
    bool SetIsReconstructed ();
    void SetGENIEinfo (GENIEinteraction fgenie_interaction){ genie_interaction = fgenie_interaction; }
    void SetReconstructedInfo ();
    
    
    // GETters
//    void SetDeltaPhiLongestandShortestTracks ();
//    bool        IsTruthCC1p () {return TruthCC1p;}; // deprecated (GENIE flags true CC1p)
    bool     IsCC1pTopology () {return CC1pTopology;};
    
    
    bool                GENIECC1p,  CC1pTopology; // , TruthCC1p deprecated
    
    Int_t               vertex_id,  Ntracks , run , subrun , event;
    std::vector<Int_t>  track_id;
    
    
    float               delta_phi_LongestShortestTracks;
    TVector3            position    ;
    
    
    TString             TopologyString , TruthTopologyString ;
    
    std::vector<PandoraNuTrack> tracks, tracks_lengthsorted,  tracks_pidasorted;
    PandoraNuTrack      muonTruth,      protonTruth;
    PandoraNuTrack      ShortestTrack,  LongestTrack;
    PandoraNuTrack      LargePIDATrack, SmallPIDATrack;
    
    std::vector <std::vector<float> > tracks_distances;
    std::vector <std::vector<float> > tracks_delta_phi;
    std::vector <std::vector<float> > tracks_delta_theta;
    std::vector<float>  delta_phi_ij,    distances_ij , delta_theta_ij;
    bool                IsVertexContained, muonTrackReconstructed, protonTrackReconstructed, IsVertexReconstructed;
    
    
    
    
    // true - GENIE - variables
    GENIEinteraction    genie_interaction;

    
    // reconstructed physical variables
    Int_t               reconstructed_Np, reconstructed_Nn, reconstructed_Npi, reconstructed_Nmu, reconstructed_Nel;
    TLorentzVector      reconstructed_nu, reconstructed_muon, reconstructed_q ;
    Float_t             reconstructed_Xb, reconstructed_Q2 ;
    Float_t             reconstructed_theta_pq, reconstructed_p_over_q, reconstructed_Mmiss;
    std::vector<TLorentzVector> reconstructed_protons;

};

#endif
/** @} */ // end of doxygen group 

