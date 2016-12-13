#ifndef LARG4PARTICLE_CXX
#define LARG4PARTICLE_CXX

#include "LArG4Particle.h"


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
LArG4Particle::LArG4Particle(Int_t frun, Int_t fsubrun , Int_t fevent ,
                             Int_t fig4, Int_t fTrackId, Int_t fpdg, Float_t fEng, Float_t ftheta, Float_t fphi,
                             Int_t fpp){
    SetRSE (frun, fsubrun , fevent);
    Set_ig4 (fig4) ;
    SetTrackId (fTrackId);
    SetPDG (fpdg);
    SetEng (fEng);
    SetTheta (ftheta);
    SetPhi (fphi);
    SetPrimaryProcess (fpp);
    
}


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void LArG4Particle::Print(){
    cout << "\033[31m" << "LArG4Particle " << ig4 << "\n~~~~~~~~~~~~~~~~~~~~~ "<< "\033[0m" << endl;
    SHOW3 ( run , subrun , event ) ;
    SHOW ( process_primary );
    SHOW3 ( ig4 , TrackId , pdg ) ;
    SHOW3 ( Eng , theta , phi ) ;
    
}

#endif
