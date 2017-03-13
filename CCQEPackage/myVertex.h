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
#include "MyTrack.h"

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

    
    void       SetCounterID (Int_t fid)                     {counter_id = fid;};
    void        SetVertexID (Int_t fvertex_id)              {vertex_id = fvertex_id;};
    void             SetRSE (Int_t r, Int_t s, Int_t e)     {run=r; subrun=s; event=e;};
    void         AddTrackID (Int_t ftrack_id)               {track_id.push_back(ftrack_id);};
    void    AddGENIETrackID (Int_t ftrack_id)               {GENIEtrack_id.push_back(ftrack_id);};
    void AddNonGENIETrackID (Int_t ftrack_id)               {NonGENIEtrack_id.push_back(ftrack_id);};
    
    void           AddTrack (PandoraNuTrack ftrack);
    
    void        SetPosition (TVector3 fposition)            {position = fposition;};
    void              Print ();
    bool      IncludesTrack (Int_t ftrack_id);
    bool SortTracksByLength ();
    bool   SortTracksByPIDA ();
    bool    RemoveFarTracks (float max_mu_p_distance, Int_t debug );

    
    bool  SetKinematicalTopology (float min_length_long     =50,    // cm
                                  float max_length_short    =30,    // cm
                                  float delta_phi_min       =-360,  // deg.
                                  float delta_phi_max       =360,   // deg.
                                  float PIDA_short_min      =0, float PIDA_short_max=50,
                                  float PIDA_long_min       =0, float PIDA_long_max=50
    );
    
    
    
    vector<size_t>        sort_l (const vector<PandoraNuTrack> &v);
    vector<size_t>     sort_pida (const vector<PandoraNuTrack> &v);

    // -------------------------------------------------------
    // my-tracking in 3 planes
    // -------------------------------------------------------
    bool                           BuildROI (int plane=0);
    bool               BuildLocationInPlane (int plane=0);
    bool              AssociateHitsToTracks (int plane, std::vector<hit> );
    bool             FindClosestHitToVertex (int plane, std::vector<hit> );
    std::vector<hit>    RemoveHitFromVector (std::vector<hit> HitsVector , hit HitToBeRemoved );
    bool FoundCloseHitAlongTrackDirection ( hit , std::vector<hit> ,
                                           float TrackAngle = 0 ,
                                           float DistanceThreshold = 5 , // mm
                                           float AngleThreshold = 1.046 // rad. (0.523 rad. = 60 deg.)
    );

    Int_t               mu_start_wire[3], mu_start_time[3], p_start_wire[3] , p_start_time[3];
    Int_t               mu_end_wire[3], mu_end_time[3], p_end_wire[3] , p_end_time[3];
    Float_t             mu_angle[3], p_angle[3];
    Float_t             AllChargeInVertexROI[3]; // sum of charge of all hits in the vertex-roi per plane
    MyTrack             MyTrackMuonTrack , MyTrackProtonTrack;
    std::vector<MyTrack> my_tracks;
    // -------------------------------------------------------
    
    
    
    
    // SETters
    void         SetTracksRelations ();
    bool         SetIsReconstructed ();
    void               SetGENIEinfo (GENIEinteraction fgenie_interaction){ genie_interaction = fgenie_interaction; };
    void            SetClosestGENIE (GENIEinteraction fgenie_interaction){ closest_genie_interaction = fgenie_interaction; };
    void       SetReconstructedInfo ();
    
    void            SetAssignTracks (PandoraNuTrack fAssignedMuonTrack, PandoraNuTrack fAssignedProtonTrack,
                                     float PmuonFromRange = 0, float PprotonFromRange = 0 );
    
    void        SetEDepAroundVertex ();
    void    SetReconstructedMomenta ( float PmuonFromRange = 0, float PprotonFromRange = 0 );
    void     SetReconstructedBeamPz ();
    void         SetReconstructed_q ();
    void   SetReconstructedFeatures ( float PmuonFromRange = 0, float PprotonFromRange = 0 );
    void        FixTracksDirections ();
    
    
    // GETters
    bool     IsCC1pTopology () {return CC1pTopology;};
    float  GetAngleBetween2tracks ();
    

    
    // variables
    TString             TopologyString , TruthTopologyString ;

    bool                Is1mu1pDetected , GENIECC1p,  CC1pTopology; // , TruthCC1p deprecated
    bool                IsVertexContained, muonTrackReconstructed, protonTrackReconstructed, IsVertexReconstructed;

    Int_t               counter_id, vertex_id,  Ntracks , run , subrun , event;
    Int_t               reconstructed_Np, reconstructed_Nn, reconstructed_Npi, reconstructed_Nmu, reconstructed_Nel;
    
    // location in each plane
    float               vertex_wire[3] , vertex_time[3];
    
    
    float               delta_phi_LongestShortestTracks;
    float               reconstructed_Xb, reconstructed_Q2 ;
    float               reconstructed_theta_pq, reconstructed_p_over_q, reconstructed_Mmiss;
    
    // CC1p reconstructed features
    float               reco_mu_p_distance;
    float               reco_CC1p_BeamPz,   reco_CC1p_theta_pq, reco_CC1p_Pp_3momentum, reco_CC1p_Pmu_3momentum;
    float               reco_CC1p_p_over_q, reco_CC1p_Xb, reco_CC1p_W2;
    float               reco_CC1p_Ev_from_angles, reco_CC1p_Ev_from_angles_Ev_from_mu_p_diff;
    float               dqdx_around_vertex,   dqdx_around_vertex_tracks_associated, dqdx_around_vertex_non_tracks_associated;
    
//    // from my_track objects
//    float               associated_hit_charge_u, associated_hit_charge_v, associated_hit_charge_y;
//    float               total_hit_charge_u, total_hit_charge_v, total_hit_charge_y;
//    float               ratio_associated_hit_charge_to_total_u, ratio_associated_hit_charge_to_total_v, ratio_associated_hit_charge_to_total_y;

    
    
    TVector3            position    ;
    TVector3            reco_CC1p_Pp_3vect, reco_CC1p_Pmu_3vect;
    
    
    
    TLorentzVector      reconstructed_nu, reconstructed_muon, reconstructed_q ;
    TLorentzVector      reco_CC1p_Pnu,  reco_CC1p_Pp,   reco_CC1p_Pmu,  reco_CC1p_q;
    TLorentzVector      reco_CC1p_n_miss;
    
    
    box                 roi[3] , roi_u , roi_v , roi_y;
    

    PandoraNuTrack      muonTrueTrack,  protonTrueTrack;
    PandoraNuTrack      ShortestTrack,  LongestTrack;
    PandoraNuTrack      LargePIDATrack, SmallPIDATrack;
    PandoraNuTrack      AssignedMuonTrack, AssignedProtonTrack;
    
    GENIEinteraction    genie_interaction, closest_genie_interaction;
    
    std::vector<Int_t>  track_id, GENIEtrack_id, NonGENIEtrack_id;
    
    std::vector <std::vector<float> >   tracks_distances;
    std::vector <std::vector<float> >   tracks_delta_phi;
    std::vector <std::vector<float> >   tracks_delta_theta;
    std::vector<float>                  tracks_dis_from_vertex, delta_phi_ij,    distances_ij , delta_theta_ij;
    
    
    std::vector<TLorentzVector> reconstructed_protons;
    std::vector<PandoraNuTrack> tracks, tracks_lengthsorted,  tracks_pidasorted , GENIEtracks, NonGENIEtracks;
    
    
    hit                 ClosestHitToVertex[3];
    
    
    // service for TPC studies
    //....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
    float WireTimeDistance(float w1 , float t1, float w2 , float t2){
        // return the distance between point (w1,t1) and (w2,t2) in mm
        // pitch between wires is 3 mm
        float wire_distance = (w1-w2)*3.0;
        // drift velocity ~ 1.114 mm/us = 0.557 mm/time-tick (2 time ticks = 1 us)
        // so dt x 0.557 = dx [mm]
        float time_distance = (t1-t2)*0.557;
        return sqrt( wire_distance * wire_distance  +  time_distance * time_distance );
    }
    float HitHitDistance(hit h1, hit h2){
        return WireTimeDistance( (float)h1.hit_wire , (float)h1.hit_peakT , (float)h2.hit_wire , (float)h2.hit_peakT );
    }
    
    float WireTimeAngle(float  w1 , float  t1, float w2 , float  t2){ // angle in wire-time plane (NOT IN mm!)
        return atan2( t2-t1  ,  w2-w1 );
    }
    float HitHitAngle(hit h1, hit h2){
        return WireTimeAngle( (float)h1.hit_wire , (float)h1.hit_peakT , (float)h2.hit_wire , (float)h2.hit_peakT );
    }
    
    

};

#endif
/** @} */ // end of doxygen group 

