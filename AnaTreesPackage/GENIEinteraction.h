/**
 * \file GENIEinteraction.h
 *
 * \ingroup GBDTprotonsPackage
 *
 * \brief Class def header for a class GENIEinteraction
 *
 * @author erezcohen
 */

/** \addtogroup GBDTprotonsPackage
 
 @{*/
#ifndef GENIEinteraction_H
#define GENIEinteraction_H

#include "LArUtil/Geometry.h"
#include "LArUtil/../../UserDev/mySoftware/MySoftwarePackage/myIncludes.h"
#include "PandoraNuTrack.h"

/**
 \class GENIEinteraction
 User defined class GENIEinteraction ... these comments are used to generate
 doxygen documentation!
 */
using namespace std;






class GENIEinteraction{
    
public:
    
    /// Default constructor
    GENIEinteraction();
    GENIEinteraction(Int_t fmcevent_id, Int_t fNprimaries);
    ~GENIEinteraction();
    
    
    
    void              Print (bool DoPrintTracks=false);
    
    
    
    // running
    bool         AddPrimary (Int_t fpdg=-1000
                             ,Float_t fEng=-1000
                             ,Float_t fPx=-1000
                             ,Float_t fPy=-1000
                             ,Float_t fPz=-1000
                             ,Float_t fP=-1000
                             ,Int_t fstatus_code=-1000
                             ,Float_t fmass=-1000
                             ,Int_t fND=-1000
                             ,Int_t fmother=-1000
                             ,Int_t track_reconstructed=-1000
                             ,Int_t ftrackID=-1000
                             ,PandoraNuTrack fprimarPandoraNuTrack=PandoraNuTrack()
                             );

    vector<size_t> sort_by_momentum_magnitude(const vector<TVector3> &v);
    bool       SortNucleons ();
    bool  ComputeKinematics ();
    bool   ComputePmissPrec ();
    bool           FindCC1p ();
    
    
    // SETters
    void      SetNuMomentum (Float_t, Float_t, Float_t, Float_t );
    
    void  SetVertexPosition (TVector3 fpos);
    void             SetRSE (int frun , int fsubrun , int fevent)   {run=frun; subrun=fsubrun;event=fevent;};
    void            SetCCNC (int fccnc)                             {ccnc = fccnc;};
    void SetVertexContained (bool fcontained)                       {IsVertexContained = fcontained;};
    
    
    
    Float_t                 Xb , Q2 ;
    Float_t                 theta_pq , p_over_q , Mmiss;
    Float_t                 reco_mu_p_distance;
    
    Int_t                   run , subrun , event , ccnc, mcevent_id;
    Int_t                   Nprimaries , Nnu , Np , Nn , Npi , Nmu , Nel , Ntot , Ngamma;
    Int_t                   Nnu_e, Nnu_mu, Nmu_minus, Nmu_plus, Npi_minus, Npi_plus, Npi_0, Ne_plus, Ne_minus;
    Int_t                   pos_wire_u, pos_wire_v, pos_wire_y;
    Int_t                   pos_time_u, pos_time_v, pos_time_y;

    TVector3                vertex_position;
    
    std::vector<Int_t>      pdg;          //particle type (pdg) of the GENIE particle
    std::vector<Float_t>    Eng;           //Energy of the GENIE particle in GeV
    std::vector<Float_t>    Px;            //Px of the GENIE particle in GeV
    std::vector<Float_t>    Py;            //Py of the GENIE particle in GeV
    std::vector<Float_t>    Pz;            //Pz of the GENIE particle in GeV
    std::vector<Float_t>    P;             //Magnitude of the momentum vector of the GENIE particle in GeV
    std::vector<Int_t>      status_code;   //particle status code of the GENIE particle
    //  only status_code==1 particles are to be tracked in detector MC,
    //  see genie.hepforge.org/doxygen/html/GHepStatus_8h_source.html
    std::vector<Float_t>    mass;          //mass of the GENIE particle in GeV
    std::vector<Int_t>      trackID;       //trackID of the GENIE particle (different from the GEANT-assigned track ID)
    std::vector<Int_t>      ND;            //number of daughters of the GENIE particle
    std::vector<Int_t>      mother;        //mother trackID of the GENIE particle

    
    TLorentzVector              momentum, Plead ;
    TLorentzVector              nu      , muon  , q ;
    TLorentzVector              n_miss  , Pcm   , Prec;
    std::vector<TLorentzVector> protons , neutrons;
    std::vector<TVector3>       p3vect  , n3vect;
    
    
    // booleans on the genie interaction
    bool                       IsVertexContained,  IsCC1p; // topology of the interaction: CC1p;
    bool                       muonTrackReconstructed, protonTrackReconstructed;
    std::vector<PandoraNuTrack> tracks; // pandoraNu tracks that are associated with the genie interacion
    
    PandoraNuTrack              muonTrack, protonTrack;
    std::vector<PandoraNuTrack> protonTracks;

    
};
#endif
/** @} */ // end of doxygen group

