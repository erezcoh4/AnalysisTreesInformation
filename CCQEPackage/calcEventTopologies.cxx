#ifndef CALCEVENTTOPOLOGIES_CXX
#define CALCEVENTTOPOLOGIES_CXX

#include "calcEventTopologies.h"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
calcEventTopologies::calcEventTopologies(TTree * fInTree, std::string foption,
                                         int fdebug,
                                         bool fMCmode,
                                         float fmax_mu_p_distance){
    
    SetInTree(fInTree);
    SetDebug(fdebug);
    SetMCMode(fMCmode);
    SetMaxmupDistance(fmax_mu_p_distance);
    SetOption(foption);
    InitInputTree();
}




//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void calcEventTopologies::InitInputTree(){

    InTree -> SetBranchAddress("run"        , &run);
    InTree -> SetBranchAddress("subrun"     , &subrun);
    InTree -> SetBranchAddress("event"      , &event);
    
    if (option.compare("GENIECC1p")!=0){
        InTree -> SetBranchAddress("Ntracks"    , &Ntracks);
    }
    Nentries = InTree -> GetEntries();
    if(debug>1) cout << "calcEventTopologies input-tree ready (" << InTree -> GetName() <<"), " <<  Nentries << " entries" << endl;
}


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void calcEventTopologies::InitEvent(){
    
    if (debug>2) Printf("calcEventTopologies::InitEvent");
    
    if (!tracks.empty())            tracks.clear();
    if (!protonTracks.empty())      protonTracks.clear();
    if (!protons.empty())           protons.clear();
    if (!genie_interactions.empty())genie_interactions.clear();
    if (!vertices.empty())          vertices.clear();
    if (!CC1p_vertices.empty())     CC1p_vertices.clear();
    if (!CC1pVerticesID.empty())    CC1pVerticesID.clear();

    Ntracks = 0;
}


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void calcEventTopologies::GetGENIEEntry (int entry){
    
    if (debug>1) Printf("calcEventTopologies::GetGENIEEntry");
    InitEvent();
    
    std::vector <GENIEinteraction> * finteractions = 0;
    InTree -> SetBranchAddress("genie_interactions" , &finteractions);
    InTree -> GetEntry(entry);
    genie_interactions = *finteractions;
    c_entry = entry;
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
bool calcEventTopologies::ClusterGENIEToVertices(){

    if (debug>2) Printf("calcEventTopologies::ClusterGENIEToVertices()");
    for (auto genie_interaction : genie_interactions){
        
        if (genie_interaction.IsCC1p){
        
            GENIEmuon = genie_interaction.muon;
            protons = genie_interaction.protons;
            GENIEproton = protons[0];
            
            c_vertex = myVertex( protonTrack.track_id );
            c_vertex.SetRSE( genie_interaction.run, genie_interaction.subrun, genie_interaction.event );
            c_vertex.GENIECC1p = true;
            
            c_vertex.AddTrack( genie_interaction.protonTrack );
            c_vertex.AddTrack( genie_interaction.muonTrack );
            
            c_vertex.IsVertexContained = genie_interaction.IsVertexContained;
            c_vertex.protonTrackReconstructed = genie_interaction.protonTrackReconstructed;
            c_vertex.muonTrackReconstructed = genie_interaction.muonTrackReconstructed;
            
            c_vertex.SetIsReconstructed();
            c_vertex.SetGENIEinfo( genie_interaction );
            // c_vertex.SetReconstructedInfo();
            
            vertices.push_back( c_vertex );
            return true;
        }
    }
    
    return false;
    
}


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
bool calcEventTopologies::AnalyzeVertices(){
    for (auto & v:vertices){
        // narrow down the set of tracks associated to the vertices by looking only
        // at those tracks that are close enough to the vertices
        v.SubtractFarTracks();

        v.SortTracksByPIDA();
        v.SortTracksByLength();
        v.SetTracksRelations();
        if (option.compare("CC1pTopology")==0){
            // Printf("setting Kinematical topology for vertex %d",v.vertex_id);
            v.SetKinematicalTopology(
                                     min_length_long,   max_length_short,
                                     delta_phi_min,     delta_phi_max,
                                     PIDA_short_min,    PIDA_short_max,
                                     PIDA_long_min,     PIDA_long_max
                                     );
        }
    }
    
    return true;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
bool calcEventTopologies::FindGENIECC1pVertices(){
    
    if (debug>4) Printf("calcEventTopologies::FindGENIECC1pVertices");
    bool FoundTruthCC1p = false;
    for (auto v:vertices) {
        if (v.GENIECC1p){
            CC1p_vertices.push_back(v);
            CC1pVerticesID.push_back(v.vertex_id);
            FoundTruthCC1p = true;
        }
    }
    if (debug>4) SHOW(FoundTruthCC1p);
    return FoundTruthCC1p;
}


// deprecated. GENIE flags true CC1p
////....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//bool calcEventTopologies::FindTruthCC1pVertices(){
//    
//    if (debug>4) Printf("calcEventTopologies::FindTruthCC1pVertices");
//    bool FoundTruthCC1p = false;
//    for (auto v:vertices) {
//        if (v.TruthCC1p){
//            CC1p_vertices.push_back(v);
//            CC1pVerticesID.push_back(v.vertex_id);
//            FoundTruthCC1p = true;
//        }
//    }
//    if (debug>4) SHOW(FoundTruthCC1p);
//    return FoundTruthCC1p;
//}

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
bool calcEventTopologies::Find2tracksVertices(){
    
    bool FoundCC1pTopology = false;
    for (auto v:vertices) {
        if (v.tracks.size() == 2){
            // vertices with only two tracks at close proximity
            // nothing else!
            // later on apply event-selection cuts
            CC1p_vertices.push_back(v);
            CC1pVerticesID.push_back(v.vertex_id);
            FoundCC1pTopology = true;
            
        }
    }
    return FoundCC1pTopology;
}




//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
bool calcEventTopologies::TagGENIECC1p(){
    // for events-tree, in which we only have tracks information
    // and cluster vertices, we need to tag GENIE true-CC1p vertices
    // which will enable us to then ask what is the purity of the sample
    // after selection cuts applied
    // This method is independent of the tracks-sorting algorithm in the vertex
    for (auto & v:CC1p_vertices){
        v.GENIECC1p = false;
        PandoraNuTrack reconstructed_proton_track = v.ShortestTrack;
        PandoraNuTrack reconstructed_muon_track = v.LongestTrack;
        
        if (
            reconstructed_proton_track.IsGENIECC1p
            && reconstructed_muon_track.IsGENIECC1p
            && reconstructed_proton_track.mcevent_id == reconstructed_muon_track.mcevent_id
            ){
            v.GENIECC1p = true;
        }
    }
    return true;
}



//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void calcEventTopologies::Print(bool DoPrintTracks, bool DoVertices){
    Printf("\n\t[%.1f%%]\n",100.*(float)c_entry/Nentries);
    
    if (option.compare("GENIECC1p")!=0) SHOW3( run , subrun , event );
    
    
    if (DoPrintTracks && !tracks.empty()){
        cout << "\033[33m" << "xxxxxxxxxxxxxx\n\n" << tracks.size() << " pandoraNu tracks\n\n" << "xxxxxxxxxxxxxx"<< "\033[37m" << endl;
        for (auto t: tracks) t.Print();
    }

    
    if (DoVertices && !vertices.empty()){
        cout << "\033[33m" << "xxxxxxxxxxxxxx\n\n" << vertices.size() << " vertices\n\n" << "xxxxxxxxxxxxxx"<< "\033[37m" << endl;
        for (auto v: vertices) v.Print();
    }
    
    if (!CC1p_vertices.empty()){
        cout << "\033[33m" << "xxxxxxxxxxxxxx\n\n" << CC1p_vertices.size() << " CC1p vertices\n\n" << "xxxxxxxxxxxxxx"<< "\033[37m" << endl;
        for (auto v: CC1p_vertices) v.Print();
    }

    EndEventBlock();
}



#endif
