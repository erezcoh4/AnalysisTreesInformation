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

#endif
