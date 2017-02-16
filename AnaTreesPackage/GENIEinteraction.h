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
#include "../../mySoftware/MySoftwarePackage/myIncludes.h"
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
    GENIEinteraction(Int_t fNprimaries, TLorentzVector fnu);
    ~GENIEinteraction();
    
    
    
    void Print();
    
    
    
    // running
    bool         AddPrimary ( Int_t,Float_t,Float_t,Float_t,Float_t,Float_t,Int_t,Float_t,Int_t,Int_t,Int_t,PandoraNuTrack );
    vector<size_t> sort_by_momentum_magnitude(const vector<TVector3> &v);
    bool       SortNucleons ();
    bool  ComputeKinematics ();
    bool   ComputePmissPrec ();
    bool           FindCC1p ();
    
    void            SetRSE (int frun , int fsubrun , int fevent)    {run=frun; subrun=fsubrun;event=fevent;};
    void           SetCCNC (int fccnc)                              {ccnc = fccnc;};
    
    
    Float_t                 Xb , Q2 ;
    Float_t                 theta_pq , p_over_q , Mmiss;
    
    Int_t                   run , subrun , event , ccnc;
    Int_t                   Nprimaries , Np , Nn , Npi , Nmu , Nel , Ntot;
    
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
    TLorentzVector              nu      ,  muon , q;
    TLorentzVector              n_miss  , Pcm   , Prec;
    std::vector<TLorentzVector> protons , neutrons;
    std::vector<TVector3>       p3vect  , n3vect;
    
    
    bool                        IsCC1p; // topology of the interaction: CC1p;
    std::vector<PandoraNuTrack> tracks; // pandoraNu tracks that are associated with the genie interacion
    
    PandoraNuTrack muonTrack;
    std::vector<PandoraNuTrack> protonTracks;

    
};
#endif
/** @} */ // end of doxygen group

