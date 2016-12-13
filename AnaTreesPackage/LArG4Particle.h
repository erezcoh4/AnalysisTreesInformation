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
#include "../../mySoftware/MySoftwarePackage/myIncludes.h"

/**
   \class LArG4Particle
   User defined class LArG4Particle ... these comments are used to generate
   doxygen documentation!
 */
class LArG4Particle{

public:
              LArG4Particle (){};
              LArG4Particle (Int_t , Int_t , Int_t , Int_t , Int_t , Int_t , Float_t , Float_t , Float_t , Int_t);
             ~LArG4Particle (){}
    
    
    // methods
    void             Print ();
    
    
    // setters
    void            SetRSE (Int_t frun, Int_t fsubrun , Int_t fevent) {run=frun; subrun=fsubrun; event=fevent;};
    void           Set_ig4 (Int_t fig4)     {ig4 = fig4;};
    void        SetTrackId (Int_t fTrackId) {TrackId = fTrackId;};
    void            SetPDG (Int_t fpdg)     {pdg = fpdg;};
    void SetPrimaryProcess (Int_t fpp)      {process_primary = fpp;};
    void            SetEng (Float_t fEng)   {Eng = fEng;};
    void          SetTheta (Float_t ftheta) {theta = ftheta;};
    void            SetPhi (Float_t fphi)   {phi = fphi;};
    
    
    // getters
    Int_t           Get_ig4 () {return ig4;};
    Int_t        GetTrackId () {return TrackId;};
    Int_t            GetPDG () {return pdg;};
    Float_t          GetEng () {return Eng;};
    Float_t        GetTheta () {return theta;};
    Float_t          GetPhi () {return phi;};
    Int_t GetPrimaryProcess () {return process_primary;};
    
    
    Int_t       run , subrun    , event;
    Int_t       ig4 , TrackId   , pdg;
    
    
    
    Float_t     Eng , theta     , phi , process_primary;

};

#endif
/** @} */ // end of doxygen group

