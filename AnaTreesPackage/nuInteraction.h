/**
 * \file nuInteraction.h
 *
 * \ingroup GBDTprotonsPackage
 *
 * \brief Class def header for a class nuInteraction
 *
 * @author erezcohen
 */

/** \addtogroup GBDTprotonsPackage
 
 @{*/
#ifndef nuInteraction_H
#define nuInteraction_H

#include "LArUtil/Geometry.h"
#include "LArUtil/../../UserDev/mySoftware/MySoftwarePackage/myIncludes.h"

/**
 \class nuInteraction
 User defined class nuInteraction ... these comments are used to generate
 doxygen documentation!
 */
using namespace std;






class nuInteraction{
    
public:
    
    /// Default constructor
    nuInteraction();
    ~nuInteraction();
    
    nuInteraction (Int_t , Int_t , Int_t , TVector3, Int_t, Int_t, Float_t, Float_t );
    
    
    void Print();
    
    
    
    // setters
    void             SetRun (Int_t _run)    {run = _run;};
    void          SetSubrun (Int_t _subrun) {subrun = _subrun;};
    void           SetEvent (Int_t _event)  {event = _event;};
    void          SetVertex (TVector3 ver)  {Vertex = ver;};
    void            SetCCNC (Int_t _ccnc)   {ccnc = _ccnc;};
    void             SetPDG (Int_t _mcpdg)  {pdg = _mcpdg;};
    void              SetXb (Float_t _xb)   {Xb = _xb;};
    void              SetQ2 (Float_t _q2)   {Q2 = _q2;};
    

    
    void      SetNuMomentum ( Float_t, Float_t, Float_t, Float_t );
    void  SetLeptonMomentum ( Float_t, Float_t, Float_t, Float_t );
    void      Set_q_vector ();
    
    Int_t       pdg     , ccnc;
    Int_t       run     , subrun    , event;
    
    TVector3    Vertex  ;
    
    Float_t     Xb , Q2 ;
    
    
    TLorentzVector              nu ,  lepton , q;
    std::vector<TLorentzVector> protons , neutrons;
    
};
#endif
/** @} */ // end of doxygen group

