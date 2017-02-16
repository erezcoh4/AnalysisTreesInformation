#ifndef CALCEVENTTOPOLOGIES_CXX
#define CALCEVENTTOPOLOGIES_CXX

#include "calcEventTopologies.h"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
calcEventTopologies::calcEventTopologies(TTree * fInTree,
                                         int fdebug,
                                         bool fMCmode,
                                         float fmax_mu_p_distance){
    
    SetInTree(fInTree);
    SetDebug(fdebug);
    SetMCMode(fMCmode);
    SetMaxmupDistance(fmax_mu_p_distance);
    InitInputTree();
}




//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void calcEventTopologies::InitInputTree(){

    InTree -> SetBranchAddress("run"        , &run);
    InTree -> SetBranchAddress("subrun"     , &subrun);
    InTree -> SetBranchAddress("event"      , &event);
    InTree -> SetBranchAddress("Ntracks"    , &Ntracks);

    Nentries = InTree -> GetEntries();
    if(debug>1) cout << "calcEventTopologies input-tree ready (" << InTree -> GetName() <<"), " <<  Nentries << " entries" << endl;
}


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void calcEventTopologies::InitEvent(){
    
    if (!tracks.empty())            tracks.clear();
    if (!vertices.empty())          vertices.clear();
    if (!CC1p_vertices.empty())     CC1p_vertices.clear();
    if (!CC1pVerticesID.empty())    CC1pVerticesID.clear();

    Ntracks = 0;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void calcEventTopologies::GetEntry (int entry){
    InitEvent();
    std::vector <PandoraNuTrack> * ftracks = 0;
    InTree -> SetBranchAddress("tracks" , &ftracks);
    InTree -> GetEntry(entry);
    tracks = *ftracks;
    c_entry = entry;
}


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
bool calcEventTopologies::extract_information(){

    // cluster vertices with multiple tracks
    ClusterTracksToVertices();
    
    // analyse vertices
    AnalyzeVertices();
    
    if (vertices.size()>0){
        return true;
    }
    
    else {
        return false;
    }
    
}


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
bool calcEventTopologies::TrackAlreadyIncludedInVerticesList(int ftrack_id){
    for (auto v:vertices){
        if ( v.IncludesTrack( ftrack_id ) ) return true;
    }
    return false;
}


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
bool calcEventTopologies::ClusterTracksToVertices(){
    bool    track_already_included_in_vertex=false;
    float   closest_distance_ij;

    for (int i=0; i < Ntracks; i++){
        
        // skip if track was clustered to a vertex by in one of the previous loop steps
        if (TrackAlreadyIncludedInVerticesList(tracks[i].track_id)) continue;
        
        c_vertex = myVertex( tracks[i].track_id );
        c_vertex.SetRSE( tracks[i].run, tracks[i].subrun, tracks[i].event);
        c_vertex.AddTrack( tracks[i] );
        
        
        for ( int j=i+1 ; j < Ntracks ; j++ ){
            
            // two close tracks (at a separation distance smaller that max_mu_p_distance)
            closest_distance_ij = tracks[i].ClosestDistanceToOtherTrack(tracks[j]);
            if ( closest_distance_ij < max_mu_p_distance ){
                if (debug>5) Printf( "closest_distance_ij=%f, generating a new vertex", closest_distance_ij);
                c_vertex.AddTrack( tracks[j] );
            }
        }
        
        vertices.push_back( c_vertex );
    }
    return true;
}


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
bool calcEventTopologies::AnalyzeVertices(){
    for (auto & v:vertices){
        v.SortTracksByLength();
        v.SetTracksRelations();
        v.SetDeltaPhiLongestandShortestTracks();
        if (MCmode) {
            // Printf("setting truth topology for vertex %d",v.vertex_id);
            v.SetTruthTopology();
        }
        if (option.compare("CC1pTopology")==0){
            // Printf("setting Kinematical topology for vertex %d",v.vertex_id);
            v.SetKinematicalTopology(min_length_long,   max_length_short,
                                     delta_phi_min,     delta_phi_max,
                                     PIDA_short_min,    PIDA_short_max,
                                     PIDA_long_min,     PIDA_long_max
                                     );
        }
    }
    
    return true;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
bool calcEventTopologies::FindTruthCC1pVertices(){
    
    if (debug>4) Printf("calcEventTopologies::FindTruthCC1pVertices");
    bool FoundTruthCC1p = false;
    for (auto v:vertices) {
        if (v.TruthCC1p){
            CC1p_vertices.push_back(v);
            CC1pVerticesID.push_back(v.vertex_id);
            FoundTruthCC1p = true;
        }
    }
    if (debug>4) SHOW(FoundTruthCC1p);
    return FoundTruthCC1p;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
bool calcEventTopologies::FindVerticesWithCC1pTopology(){

    bool FoundCC1pTopology = false;
    for (auto v:vertices) {
        if (v.CC1pTopology){
            CC1p_vertices.push_back(v);
            CC1pVerticesID.push_back(v.vertex_id);
            FoundCC1pTopology = true;
        }
    }
    return FoundCC1pTopology;
}



//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void calcEventTopologies::Print(bool DoTracks, bool DoVertices){
    Printf("\n\t[%.1f%%]\n",100.*(float)c_entry/Nentries);
    SHOW3(run,subrun,event);
    if (DoTracks && !tracks.empty()){
        cout << "\033[33m" << tracks.size() << " pandoraNu tracks" << "\033[37m" << endl;
        for (auto t: tracks) t.Print();
    }
    if (DoVertices && !vertices.empty()){
        cout << "\033[33m" << vertices.size() << " vertices" <<"\033[37m" << endl;
        for (auto v: vertices) v.Print();
    }
    EndEventBlock();
}



#endif
