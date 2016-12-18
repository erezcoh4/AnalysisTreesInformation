#ifndef LARG4PARTICLE_CXX
#define LARG4PARTICLE_CXX

#include "LArG4Particle.h"


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
LArG4Particle::LArG4Particle(Int_t frun, Int_t fsubrun , Int_t fevent ,
                             Int_t fig4, Int_t fTrackId, Int_t fpdg,
                             Float_t fP, Float_t fEng, Float_t fMass, Float_t ftheta, Float_t fphi,
                             Int_t fpp,
                             TVector3 fstart_pos , TVector3 fend_pos,
                             Int_t fMother):
pdg(-100),
ig4(-100),
TrackId(-100),
Eng(-100),
Mass(-100),
theta(-100),
phi(-100),
process_primary(-100),
Mother(-100),
start_pos(TVector3()),
end_pos(TVector3())
{
    SetRSE (frun, fsubrun , fevent);
    Set_ig4 (fig4) ;
    SetTrackId (fTrackId);
    SetPDG (fpdg);
    SetP (fP);
    SetEng (fEng);
    SetMass (fMass);
    SetKE (fEng - fMass);
    SetTheta (ftheta);
    SetPhi (fphi);
    SetPrimaryProcess (fpp);
    SetMother (fMother);
    SetStartPos (fstart_pos);
    SetEndPos (fend_pos);
    
}


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void LArG4Particle::Print(){
    cout << "\033[31m" << "LArG4Particle " << ig4 << "\n~~~~~~~~~~~~~~~~~~~~~ "<< "\033[0m" << endl;
    SHOW3 ( run , subrun , event ) ;
    SHOW ( process_primary );
    SHOW ( P );
    SHOW2 ( Mass , KE );
    SHOW3 ( ig4 , TrackId , pdg ) ;
    SHOW3 ( Eng , theta , phi ) ;
    SHOWTVector3 ( start_pos ) ;
    SHOWTVector3 ( end_pos ) ;
    SHOW ( Mother );
}

#endif
