#ifndef ANALYZETRACKSFILE_CXX
#define ANALYZETRACKSFILE_CXX

#include "AnalyzeTracksFile.h"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
AnalyzeTracksFile::AnalyzeTracksFile( TString fPath, TString fName, Int_t fdebug ){
    
    SetTracksFilesPath(fPath);
    SetTracksFileName(fName);
    SetDebug(fdebug);
    InitInputTree();
    
}



//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void AnalyzeTracksFile::InitInputTree(){
    
    TracksFile = new TFile(TracksFileName);
    TracksTree = (TTree*)TracksFile->Get("TracksTree");

    TracksTree -> SetBranchAddress("run"        , &run);
    TracksTree -> SetBranchAddress("subrun"     , &subrun);
    TracksTree -> SetBranchAddress("event"      , &event);
    TracksTree -> SetBranchAddress("Ntracks"    , &Ntracks);
    
    tracks_pointer = 0 ;
    TracksTree -> SetBranchAddress("tracks"     , &tracks_pointer);

}


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
std::vector<PandoraNuTrack> AnalyzeTracksFile::GetEventTracks(Int_t entry){
    
    TracksTree -> GetEntry(entry);
    tracks = *tracks_pointer;
    if (debug > 2) {
        SHOW3( run , subrun , event );
        if(!tracks.empty()){
            cout << "\033[33m" << "xxxxxxxxxxxxxx\n\n" << tracks.size() << " pandoraNu tracks\n\n" << "xxxxxxxxxxxxxx\n\n"<< "\033[37m" << endl;
            for (auto t: tracks) {
                t.Print();
            }
        }
    }
    return tracks;
}


#endif
