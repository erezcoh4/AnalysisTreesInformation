#ifndef ANALYZEVERTEX_CXX
#define ANALYZEVERTEX_CXX

#include "AnalyzeVertex.h"


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
AnalyzeVertex::AnalyzeVertex( TTree * fInTree ) {
    SetInTree(fInTree);
    InitInputTree();
}


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void AnalyzeVertex::InitInputTree(){
    
    InTree -> SetBranchAddress("Ntracks"        , &Ntracks);
    InTree -> SetBranchAddress("Nhits"          , &Nhits);
    InTree -> SetBranchAddress("NCC1pVertices"  , &NCC1pVertices);
    
    Nentries = InTree -> GetEntries();
    if(debug>1) cout << "AnalyzeVertex input-tree ready (" << InTree -> GetName() <<"), " <<  Nentries << " entries" << endl;
}


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void AnalyzeVertex::InitEvent(){
    
    Debug ( 2 , "AnalyzeVertex::InitEvent");
    
    if (!tracks.empty())            tracks.clear();
    if (!hits.empty())              hits.clear();
    if (!vertices.empty())          vertices.clear();
    
}


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void AnalyzeVertex::GetEntry (int entry){
    InitEvent();
    
    std::vector <PandoraNuTrack> * ftracks = 0;
    InTree -> SetBranchAddress("tracks" , &ftracks);
    
    std::vector <hit> * fhits = 0;
    InTree -> SetBranchAddress("hits" , &fhits);
    
    std::vector <myVertex> * fvertices = 0;
    InTree -> SetBranchAddress("CC1p_vertices" , &fvertices);
    
    InTree -> GetEntry(entry);
    
    hits = *fhits;
    tracks = *ftracks;
    vertices = *fvertices;
    
    delete ftracks;
    delete fhits;
    delete fvertices;
}


#endif
