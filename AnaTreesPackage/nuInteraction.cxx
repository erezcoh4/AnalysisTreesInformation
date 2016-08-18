#ifndef nuInteraction_CXX
#define nuInteraction_CXX

#include "nuInteraction.h"
#include "LArUtil/GeometryHelper.h"


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
nuInteraction::nuInteraction():
Vertex(TVector3(-100,-100,-100)),
pdg(-100),
Xb(-100),
Q2(-100)
{}


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
nuInteraction::~nuInteraction(){ }

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
nuInteraction::nuInteraction (
                                        Int_t frun
                                        ,Int_t fsubrun
                                        ,Int_t fevent
                                        ,TVector3 fVertex
                                        ,Int_t fccnc
                                        ,Int_t fpdg
                                        ,Float_t fXb
                                        ,Float_t fQ2
                                        ){

    SetRun(frun);
    SetSubrun(fsubrun);
    SetEvent(fevent);
    SetVertex(fVertex);
    SetCCNC(fccnc);
    SetPDG(fpdg);
    SetXb(fXb);
    SetQ2(fQ2);
    
    
}



//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void nuInteraction::SetNuMomentum( Float_t magnitude , Float_t dcosx , Float_t dcosy , Float_t dcosz ){
    nu.SetVectMag( TVector3( magnitude*dcosx , magnitude*dcosy , magnitude*dcosz ) , 0 );
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void nuInteraction::SetLeptonMomentum( Float_t magnitude , Float_t dcosx , Float_t dcosy , Float_t dcosz ){
    lepton.SetVectMag( TVector3( magnitude*dcosx , magnitude*dcosy , magnitude*dcosz ) , 0.105 );
}






//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void nuInteraction::Set_q_vector(){
    q = nu - lepton;
}




//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void nuInteraction::Print(){

    cout << "\033[31m" << "neutrino interaction " << "\n~~~~~~~~~~~~~~~~~~~~~ "<< "\033[0m" << endl;
    SHOW3(run , subrun , event);
    SHOW(ccnc);
    SHOW(pdg);
    SHOWTVector3(Vertex);
    SHOWTLorentzVector(nu);
    SHOWTLorentzVector(lepton);
    SHOWTLorentzVector(q);
    SHOW(Xb);
    SHOW(Q2);

}


#endif
