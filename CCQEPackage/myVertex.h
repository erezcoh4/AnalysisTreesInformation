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
#define PI TMath::Pi()

//// pitch between wires is 3 mm
//#define WireDistance(w1, w2 ) (w2-w1)*3.0
//
//// drift velocity ~ 1.114 mm/us = 0.557 mm/time-tick (2 time ticks = 1 us)
//// so dt x 0.557 = dx [mm]
//#define TimeDistance( t1, t2 ) (t2-t1)*0.557
//
//// return the distance between point (w1,t1) and (w2,t2) in mm
//#define WireTimeDistance( w1 , t1, w2 , t2) sqrt(  WireDistance( w1, w2) * WireDistance( w1, w2)  +  TimeDistance( t1, t2) * TimeDistance( t1, t2) );




/**
   \class myVertex
   User defined class myVertex ... these comments are used to generate
   doxygen documentation!
 */
class myVertex: public myIncludes {

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
    bool    RemoveFarTracks (float max_mu_p_distance );
	
    vector<PandoraNuTrack>  NearUncontainedTracks ( vector<PandoraNuTrack> AllTracksInTheEvent , float fmax_distance=100 );
    vector<PandoraNuTrack>  RemoveTrackFromVector ( vector<PandoraNuTrack> AllTracks , PandoraNuTrack TrackToBeRemoved );
    vector<PandoraNuTrack> RemoveTracksFromVector ( vector<PandoraNuTrack> AllTracks , vector<PandoraNuTrack> TracksToBeRemoved );
   
    bool  SetKinematicalTopology (float min_length_long     =0,    // cm
                                  float max_length_short    =3000,    // cm
                                  float delta_phi_min       =-360,  // deg.
                                  float delta_phi_max       =360,   // deg.
                                  float PIDA_short_min      =0, float PIDA_short_max=5000,
                                  float PIDA_long_min       =0, float PIDA_long_max=0
    );
    
    
    
    vector<size_t>        sort_l (const vector<PandoraNuTrack> &v);
    vector<size_t>     sort_pida (const vector<PandoraNuTrack> &v);

    // -------------------------------------------------------
    // my-tracking in 3 planes
    // -------------------------------------------------------
    void               SetDistanceThreshold ( float fDistanceThreshold = 50.0) {HitHitDistanceThreshold = fDistanceThreshold;}; // mm
    void                  SetAngleThreshold ( float fAngleThreshold = 1.046) {AngleThreshold = fAngleThreshold;}; // 0.523 rad. = 60 deg.
    bool                           BuildROI ( int plane=0);
    bool               BuildLocationInPlane ( int plane=0);
    bool              AssociateHitsToTracks ( int plane=0);//, std::vector<hit> );
    bool             FindClosestHitToVertex ( int plane, std::vector<hit> );
    std::vector<hit>    RemoveHitFromVector ( std::vector<hit> HitsVector , hit HitToBeRemoved );
    bool   FoundCloseHitAlongTrackDirection ( hit , std::vector<hit> , float TrackAngle = 0  );
    std::vector<hit>           TrackMyTrack ( int plane, hit StartHit , std::vector<hit> hits , float TrackAngle , PandoraNuTrack track );
    //    int                   ClosestTrackToHit ( int plane, hit c_hit );
    hit                 ClosestHitToTrack1d ( int plane, int wire , float peakTime , std::vector<hit> fPossibleHits, PandoraNuTrack track );
    vector<hit>   PossibleTracksForTracking ( int plane, float TrackAngle , PandoraNuTrack track ); // , std::vector<hit> fPossibleHits
    std::vector<float>  GetX1Y1X2Y2forTrack ( int plane , PandoraNuTrack t ); // return {x1,y1,x2,y2}
    vector<float>     FindSlopeAndIntercept ( int plane, PandoraNuTrack t );
    hit       ClosestHitToTrackHorizontally ( int plane, float peakTime, std::vector<hit> fPossibleHits, PandoraNuTrack track );
    hit         ClosestHitToTrackVertically ( int plane, int wire , std::vector<hit> fPossibleHits, PandoraNuTrack track );
    hit                         FindNextHit ( hit StartHit, hit LastHit, hit CurrentHit, std::vector<hit> PossibleHits, float TrackAngle );
    bool                SetTracksParameters ( int plane);
    bool            CollectAssociatedCharge ( int plane);
    MyTrack                     FindMyTrack ( int plane , PandoraNuTrack pandoraNu_track ,
                                             float track_WireTimeAngle ); // , std::vector<hit> fPossibleHits );
    void                  SetAllHitsInPlane ( int plane, std::vector<hit> fhits);
    void                    SetAllHitsInROI ( int plane, std::vector<hit> fhits);
    void                CollectAllHitsInROI ( int plane, std::vector<hit> hits_in_this_plane );
    
    
    bool                FoundClosestHitToVertex;
    
    Int_t               mu_start_wire[3], mu_start_time[3], p_start_wire[3] , p_start_time[3];
    Int_t               mu_end_wire[3], mu_end_time[3], p_end_wire[3] , p_end_time[3];
    
    Float_t             AngleThreshold, HitHitDistanceThreshold;
    Float_t             mu_angle[3], p_angle[3];
    Float_t             AllChargeInVertexROI[3], AllChargeInVertexROI_enlarged_20_100[3], AllChargeInVertexROI_enlarged_40_200[3]; // sum of charge of all hits in the vertex-roi per plane
    Float_t             dQtotROI_20x40_AroundVertex[3], dQassociatedROI_20x40_AroundVertex[3];
    Float_t             TracksAssociatedCharge[3]; // sum of charge of all hits that are associated with my-tracks
    Float_t             ratio_associated_hit_charge_to_total[3] , average_ratio_associated_hit_charge_to_total , max_ratio_associated_hit_charge_to_total;
    Float_t             ratio_associated_hit_charge_to_total_enlarged_20_100[3];
    Float_t             ratio_associated_hit_charge_to_total_enlarged_40_200[3];
    Float_t             ratio_dQassociated_dQtot_ROI_20x40_AroundVertex[3];
    
    MyTrack             MyTrackMuonTrack[3] , MyTrackProtonTrack[3];
    MyTrack             MyTrackMuon_u, MyTrackMuon_v, MyTrackMuon_y, MyTrackProton_u, MyTrackProton_v, MyTrackProton_y;
    std::vector<MyTrack> my_tracks;
    
    hit                 ClosestHitToVertex[3];
    std::vector<hit>    HitsInPlane_u, HitsInPlane_v, HitsInPlane_y;
    std::vector<hit>    AllHitsInROI[3], AllHitsInROI_u,AllHitsInROI_v,AllHitsInROI_y;
    // -------------------------------------------------------
    
    
    
    
    // SETters
    void         SetTracksRelations ();
    bool         SetIsReconstructed ( float fmax_mu_p_distance = 10 );
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

    bool                Is1mu1pDetected , GENIECC1p,  CC1pTopology, BothTracksAreGENIECC1p, Non1mu1p; 
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
    float               reco_CC1p_p_over_q, reco_CC1p_Q2, reco_CC1p_Q2_from_angles;
    float               reco_CC1p_omega;
    float               reco_CC1p_Xb, reco_CC1p_y, reco_CC1p_W2, reco_CC1p_s;
    float               reco_CC1p_Ev_from_angles, reco_CC1p_Ev_from_angles_Ev_from_mu_p_diff, reco_CC1p_Ev_from_angles_Ev_from_mu_p_ratio;
    float               reco_CC1p_alpha_p , reco_CC1p_alpha_q , reco_CC1p_alpha_mu, reco_CC1p_alpha_miss;
    float               reco_CC1p_Q2_from_angles_diff, reco_CC1p_Q2_from_angles_ratio;
    float               reco_CC1p_Ev_with_binding, reco_CC1p_Ev_with_binding_diff, reco_CC1p_Ev_with_binding_ratio;

    
    // momentum correction from p(mu)/theta(mu) and p(p) / theta(p) correlations
    float               reco_CC1p_Ev_corrected;
    float               reco_CC1p_theta_pq_corrected,   reco_CC1p_p_over_q_corrected, reco_CC1p_Q2_corrected;
    float               reco_CC1p_omega_corrected;
    float               reco_CC1p_Xb_corrected, reco_CC1p_y_corrected, reco_CC1p_W2_corrected, reco_CC1p_s_corrected;
    float               reco_CC1p_alpha_q_corrected, reco_CC1p_alpha_p_corrected, reco_CC1p_alpha_miss_corrected, reco_CC1p_alpha_mu_corrected;
    // --- - - --- -- - -- -- -- -- --- -- - --- - -- - -- -- -- --- - -- - --- - - -- - -- -

    // Tp + Eµ
    float               reco_CC1p_BeamE, reco_CC1p_Ev_fromE;
    float               reco_CC1p_theta_pq_fromE,   reco_CC1p_p_over_q_fromE, reco_CC1p_Q2_fromE;
    float               reco_CC1p_omega_fromE;
    float               reco_CC1p_Xb_fromE, reco_CC1p_y_fromE, reco_CC1p_W2_fromE, reco_CC1p_s_fromE;
    float               reco_CC1p_alpha_p_fromE, reco_CC1p_alpha_q_fromE, reco_CC1p_alpha_miss_fromE;
    // --- - - --- -- - -- -- -- -- --- -- - --- - -- - -- -- -- --- - -- - --- - - -- - -- -

    
    float               dqdx_around_vertex,   dqdx_around_vertex_tracks_associated, dqdx_around_vertex_non_tracks_associated;
    float               truth_alpha_q, truth_alpha_p, truth_alpha_mu, truth_alpha_miss;
    
    TVector3            position    ;
    TVector3            reco_CC1p_Pp_3vect, reco_CC1p_Pmu_3vect, reco_CC1p_Pp_3vect_corrected, reco_CC1p_Pmu_3vect_corrected;
    
    
    TLorentzVector      reconstructed_nu, reconstructed_muon, reconstructed_q ;
    TLorentzVector      reco_CC1p_Pnu,  reco_CC1p_Pp,   reco_CC1p_Pmu,  reco_CC1p_q;
    TLorentzVector      reco_CC1p_n_miss;    
    // Tp + Eµ
    TLorentzVector      reco_CC1p_Pnu_fromE, reco_CC1p_q_fromE, reco_CC1p_n_miss_fromE;

    
    
    // momentum correction from p(mu)/theta(mu) and p(p) / theta(p) correlations
    TLorentzVector      reco_CC1p_Pp_corrected,  reco_CC1p_Pmu_corrected,  reco_CC1p_q_corrected;
    TLorentzVector      reco_CC1p_Pnu_corrected, reco_CC1p_n_miss_corrected;
    // --- - - --- -- - -- -- -- -- --- -- - --- - -- - -- -- -- --- - -- - --- - - -- - -- -
    
    
    // from MCS LLHD
    TVector3            reco_CC1p_Pmu_3vect_mcsllhd;
    TLorentzVector      reco_CC1p_Pmu_mcsllhd, reco_CC1p_Pnu_mcsllhd, reco_CC1p_q_mcsllhd;
    float               reco_CC1p_theta_pq_mcsllhd, reco_CC1p_BeamPz_mcsllhd;
    float               reco_CC1p_p_over_q_mcsllhd, reco_CC1p_Q2_mcsllhd, reco_CC1p_Q2_from_angles_mcsllhd;
    // --------
    
    box                 roi[3] , roi_u , roi_v , roi_y, Roi_20x40_AroundVertex[3];
    

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
    
    
    
    
    // service for TPC studies
    //....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
    float WireDistance(float w1, float w2 ){
        // pitch between wires is 3 mm
        return (w2-w1)*3.0;
    }
    float TimeDistance(float t1, float t2 ){
        // drift velocity ~ 1.114 mm/us = 0.557 mm/time-tick (2 time ticks = 1 us)
        // so dt x 0.557 = dx [mm]
        return (t2-t1)*0.557;
    }
    float WireTimeDistance(float w1 , float t1, float w2 , float t2){
        // return the distance between point (w1,t1) and (w2,t2) in mm
        float w_dis = WireDistance( w1, w2);
        float t_dis = TimeDistance( t1, t2);
        return sqrt(  w_dis * w_dis  +  t_dis * t_dis );
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

