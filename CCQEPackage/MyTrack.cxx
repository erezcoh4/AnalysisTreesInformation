#ifndef MYTRACK_CXX
#define MYTRACK_CXX

#include "MyTrack.h"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
MyTrack::MyTrack( int p , PandoraNuTrack t ){
    SetPlane(p);
    if (!hits.empty()) hits.clear();
    SetPandoraNuTrack( t );
    color = "";
}


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
float MyTrack::GetTotalChargeInHits (){
    float total_charge=0;
    for (auto hit:hits){
        total_charge += hit.hit_charge;
    }
    return total_charge;
}


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
float MyTrack::GetTotalChargeInHitsInROI (box roi){
    float total_charge=0;
    for (auto hit:hits){
        if (hit.InBox( roi )){
            total_charge += hit.hit_charge;
        }
    }
    return total_charge;
}



#endif
