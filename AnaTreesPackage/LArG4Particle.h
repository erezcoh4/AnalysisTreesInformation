/**
 * \file LArG4Particle.h
 *
 * \ingroup AnaTreesPackage
 * 
 * \brief Class def header for a class LArG4Particle
 *
 * @author erezcohen
 */

/** \addtogroup AnaTreesPackage

    @{*/
#ifndef LARG4PARTICLE_H
#define LARG4PARTICLE_H

#include <iostream>
//#include "../../mySoftware/MySoftwarePackage/myIncludes.h"
#include "LArUtil/../../UserDev/mySoftware/MySoftwarePackage/myIncludes.h"

/**
   \class LArG4Particle
   User defined class LArG4Particle ... these comments are used to generate
   doxygen documentation!
 */
class LArG4Particle{

public:
              LArG4Particle (){};
              LArG4Particle (Int_t , Int_t , Int_t , Int_t , Int_t , Int_t , Float_t , Float_t , Float_t , Float_t , Float_t , Int_t , TVector3 , TVector3 , Int_t , Int_t );
             ~LArG4Particle (){}
    
    
    // methods
    void             Print ();
    
    
    // setters
    void            SetRSE (Int_t frun, Int_t fsubrun , Int_t fevent) {run=frun; subrun=fsubrun; event=fevent;};
    void           Set_ig4 (Int_t fig4)     {ig4 = fig4;};
    void        SetTrackId (Int_t fTrackId) {TrackId = fTrackId;};
    void            SetPDG (Int_t fpdg)     {pdg = fpdg;};
    void SetPrimaryProcess (Int_t fpp)      {process_primary = fpp;};
    void         SetMother (Int_t fm)       {Mother = fm;};
    void              SetP (Float_t fP)     {P = fP;};
    void            SetEng (Float_t fEng)   {Eng = fEng;};
    void           SetMass (Float_t fMass)  {Mass = fMass;};
    void             SetKE (Float_t fKE)    {KE = fKE;};
    void          SetTheta (Float_t ftheta) {theta = ftheta;};
    void            SetPhi (Float_t fphi)   {phi = fphi;};
    void       SetStartPos (TVector3 fp)    {start_pos = fp; startx=start_pos.x(); starty=start_pos.y(); startz=start_pos.z();};
    void         SetEndPos (TVector3 fp)    {end_pos = fp; endx=end_pos.x(); endy=end_pos.y(); endz=end_pos.z();};
    void    SetTrackLength ();
    void           SetCCNC (Int_t fccnc)    {ccnc = fccnc;};

    // getters
    Int_t           Get_ig4 () {return ig4;};
    Int_t        GetTrackId () {return TrackId;};
    Int_t            GetPDG () {return pdg;};
    Float_t          GetEng () {return Eng;};
    Float_t         GetMass () {return Mass;};
    Float_t           GetKE () {return KE;};
    Float_t        GetTheta () {return theta;};
    Float_t          GetPhi () {return phi;};
    Int_t GetPrimaryProcess () {return process_primary;};
    Int_t           GetCCNC () {return ccnc;}

    
    Int_t       run , subrun    , event , Mother , ccnc;
    Int_t       ig4 , TrackId   , pdg;
    
    
    
    Float_t     P , Eng  , Mass , KE , theta , phi , process_primary;
    Float_t     length;
    
    Float_t     startx  , starty , startz , endx , endy , endz;
    TVector3    start_pos , end_pos;

    
    // reconstrcuted features
    Int_t       reconstructed   , rec_nhits     , rec_is_flipped;
    Float_t     rec_length      , rec_theta     , rec_phi , rec_distlenratio , rec_momentum;
    Float_t     rec_start_dqdx  , rec_end_dqdx  , rec_tot_dqdx , rec_avg_dqdx;
    Float_t     rec_dqdx_diff   , rec_dqdx_ratio;
    Float_t     rec_pidpida     , rec_pidchi    , rec_cosmicscore   , rec_coscontscore;
    Float_t     rec_cftime;
    Float_t     rec_startx      , rec_starty , rec_startz , rec_endx , rec_endy , rec_endz;
    TVector3    rec_start_pos   , rec_end_pos;
    void    SetRecStartPos (TVector3 fp)    {
        rec_start_pos = fp; rec_startx=rec_start_pos.x(); rec_starty=rec_start_pos.y(); rec_startz=rec_start_pos.z();
    };
    void      SetRecEndPos (TVector3 fp)    {
        rec_end_pos = fp; rec_endx=rec_end_pos.x(); rec_endy=rec_end_pos.y(); rec_endz=rec_end_pos.z();
    };

};

#endif
/** @} */ // end of doxygen group

