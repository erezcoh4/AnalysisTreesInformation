#ifndef CALCEVENTTOPOLOGIES_CXX
#define CALCEVENTTOPOLOGIES_CXX

#include "calcEventTopologies.h"
#include "MyLArTools.cxx"
#include "LArUtil/GeometryHelper.h"

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
    lar_tools = new MyLArTools();
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
    
    Debug ( 2 , "calcEventTopologies::InitEvent");
    
    if (!tracks.empty())            tracks.clear();
    if (!protonTracks.empty())      protonTracks.clear();
    if (!protons.empty())           protons.clear();
    if (!genie_interactions.empty())genie_interactions.clear();
    if (!vertices.empty())          vertices.clear();
    if (!CC1p_vertices.empty())     CC1p_vertices.clear();
    if (!CC1pVerticesID.empty())    CC1pVerticesID.clear();

    Ntracks = 0;
    FoundTruthCC1p = false;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void calcEventTopologies::GetGENIEEntry (int entry){
    
    Debug ( 2 , "calcEventTopologies::GetGENIEEntry" );
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
    
    std::vector <GENIEinteraction> * finteractions = 0;
    InTree -> SetBranchAddress("genie_interactions" , &finteractions);

    
    InTree -> GetEntry(entry);
    tracks = *ftracks;
    genie_interactions = *finteractions;
    
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
    bool    track_already_included_in_vertex=false, FoundCloseTracks , AlreadySetPosition;
    float   closest_distance_ij;
    TVector3 vertex_position;
    
    for (int i=0; i < Ntracks; i++){
        
        // skip if track was clustered to a vertex by in one of the previous loop steps
        if (TrackAlreadyIncludedInVerticesList(tracks[i].track_id)) continue;
        // is this wrong?! we need to go over all of the tracks, even if they are included in previous cluseters.
        // however we need to avoid from counting twice!
        
        // Debug(5, Form("looking for close tracks to track %d", tracks[i].track_id));

        c_vertex = myVertex( tracks[i].track_id );
        c_vertex.SetRSE( tracks[i].run, tracks[i].subrun, tracks[i].event);
        c_vertex.AddTrack( tracks[i] );
        
        FoundCloseTracks = AlreadySetPosition = false;
        
        for ( int j=0 ; j < Ntracks ; j++ ){ // i+1
            if (j!=i){
                
                // if this is the first time we go over these two tracks
                // and they are close enough to define a vertex,
                // we also define the position of their mutual vertex
                if (!AlreadySetPosition){

                    // two close tracks (at a separation distance smaller that max_mu_p_distance)
                    std::string StartOrEnd = "None";
                    closest_distance_ij = tracks[i].ClosestDistanceToOtherTrack(tracks[j],&StartOrEnd);
                    
                    // Debug(5, Form( "closest-distance(i=%d,j=%d)=%f",i,j, closest_distance_ij));
                    
                    if ( closest_distance_ij < max_mu_p_distance ){
                        
                        
                        c_vertex.AddTrack( tracks[j] );
                        FoundCloseTracks = true;
                        
                        if (StartOrEnd.compare("Start")==0)     vertex_position = tracks[i].start_pos ;
                        else if (StartOrEnd.compare("End")==0)  vertex_position = tracks[i].end_pos ;
                        else                                    vertex_position = TVector3(-1000,-1000,-1000) ;
                        
                        c_vertex.SetPosition( vertex_position );
                        c_vertex.SetMyLArTools (lar_tools);
                        AlreadySetPosition = true;
                    }
                }
                
                // else, namely if we have already clustered a vertex
                // and positioned it in space,
                // we only need to check wether the new track (j) is close enough to this vertex
                // to be also associated with it
                else {
                    // Debug(3, Form("cheking track %d distance from vertex %d ",tracks[j].track_id,c_vertex.vertex_id));
                    if ( tracks[j].DistanceFromPoint(c_vertex.position) < max_mu_p_distance ){
                        
                        // SHOWTVector3(c_vertex.position);
                        // Printf("track %d close enough...",tracks[j].track_id);
                        // SHOW(tracks[j].DistanceFromPoint(c_vertex.position));
                        
                        c_vertex.AddTrack( tracks[j] );
                    }
                }
            }
        }
        
        if (FoundCloseTracks){
            vertices.push_back( c_vertex );
        }
    }
    return true;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
bool calcEventTopologies::ClusterGENIEToVertices(){
    Debug (2 , "calcEventTopologies::ClusterGENIEToVertices()");
    for (auto genie_interaction : genie_interactions){
        
        if (genie_interaction.IsCC1p){
      
            GENIEmuon = genie_interaction.muon;
            protons = genie_interaction.protons;
            GENIEproton = protons[0];
            
            c_vertex = myVertex( genie_interaction.muonTrack.track_id );
            c_vertex.SetMyLArTools (lar_tools);
            c_vertex.SetPosition( genie_interaction.vertex_position );
            c_vertex.SetRSE( genie_interaction.run, genie_interaction.subrun, genie_interaction.event );
            c_vertex.GENIECC1p = true;
            
            c_vertex.AddTrack( genie_interaction.protonTrack );
            c_vertex.AddTrack( genie_interaction.muonTrack );


            
            c_vertex.IsVertexContained = genie_interaction.IsVertexContained;
            c_vertex.protonTrackReconstructed = genie_interaction.protonTrackReconstructed;
            c_vertex.muonTrackReconstructed = genie_interaction.muonTrackReconstructed;
            c_vertex.protonTrueTrack = genie_interaction.protonTrack ;
            c_vertex.muonTrueTrack = genie_interaction.muonTrack;
            
            c_vertex.SetIsReconstructed();
            c_vertex.SetGENIEinfo( genie_interaction );
            c_vertex.SetAssignTracks( genie_interaction.muonTrack , genie_interaction.protonTrack );
            c_vertex.TruthTopologyString = "true GENIE CC1p";
            
            vertices.push_back( c_vertex );
            FoundTruthCC1p = true;
        }
    }
    
    return FoundTruthCC1p;
    
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
bool calcEventTopologies::AnalyzeVertices(){
    Debug( 2 , "calcEventTopologies::AnalyzeVertices()" );
    
    for (auto & v:vertices){
        
        if (option.compare("CC1pTopology")==0){
            v.RemoveFarTracks( max_mu_p_distance , debug  );
            
        }
        
        v.SortTracksByPIDA();
        v.SortTracksByLength();
        v.SetTracksRelations();

        if (option.compare("CC1pTopology")==0){
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

// deprecated - delete!
////....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//bool calcEventTopologies::FindGENIECC1pVertices(){
//    Debug(4,"calcEventTopologies::FindGENIECC1pVertices");
//    // method for finding GENIE CC1p when looping over
//    // GENIE interactions
//    // not for looping over events / 2-tracks clusters
//    
//    bool FoundTruthCC1p = false;
//    for (auto & v:vertices) {
//        if (v.GENIECC1p){
//            v.TruthTopologyString = "true GENIE CC1p";
//            CC1p_vertices.push_back(v);
//            CC1pVerticesID.push_back(v.vertex_id);
//            FoundTruthCC1p = true;
//        }
//    }
//    // if (debug>4) SHOW(FoundTruthCC1p);
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
    for (auto & v:vertices) {
        if ( v.tracks.size() == 2 ){
            
            // vertices with only two tracks at close proximity
            // nothing else!
            // later on apply event-selection cuts
            
            // assign the muon and proton tracks by length
            //        PandoraNuTrack t1 = v.ShortestTrack;
            //        PandoraNuTrack t2 = v.LongestTrack;
            // assign muon and proton tracks by PID-A
            v.SetAssignTracks( v.SmallPIDATrack , v.LargePIDATrack );

            
            CC1p_vertices.push_back( v );
            CC1pVerticesID.push_back( v.vertex_id );
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
        
        PandoraNuTrack t1 = v.AssignedMuonTrack;
        PandoraNuTrack t2 = v.AssignedProtonTrack;;
        
        if (     t1.mcevent_id == t2.mcevent_id && t1.IsGENIECC1p && t2.IsGENIECC1p
            && ((t1.MCpdgCode==13 && t2.MCpdgCode==2212) || (t1.MCpdgCode==2212 && t2.MCpdgCode==13) ) ){
            
            Debug(3,"found CC1p");
            v.GENIECC1p = true;
            v.TruthTopologyString = "true GENIE CC1p";
            
            // match the proper GENIE interaction
            GENIEinteraction c_genie_interaction;
            bool MatchedGENIEinteraction = false;
            for (auto genie_interaction : genie_interactions){
                if (genie_interaction.mcevent_id == t1.mcevent_id){
                    c_genie_interaction = genie_interaction;
                    MatchedGENIEinteraction = true;
                    break;
                }
            }
            if (MatchedGENIEinteraction){
                v.SetGENIEinfo( c_genie_interaction );
            }
            

            if ( t1.MCpdgCode==2212 && t2.MCpdgCode==13 ){
                Debug(4,Form("found the p (track %d) and mu (track %d) in this CC1p",t1.track_id,t2.track_id));
                v.protonTrueTrack = t1;
                v.muonTrueTrack = t2;
                v.muonTrackReconstructed = v.protonTrackReconstructed = v.IsVertexReconstructed = true;
            }
            else if ( t1.MCpdgCode==13 && t2.MCpdgCode==2212 ){
                Debug(4,Form("found the p (track %d) and mu (track %d) in this CC1p",t2.track_id,t1.track_id));
                v.muonTrueTrack = t1;
                v.protonTrueTrack = t2;
                v.muonTrackReconstructed = v.protonTrackReconstructed = v.IsVertexReconstructed = true;
            }
            else{
                Debug(4,Form("could not find p and mu in this CC1p - pdg codes are %d/%d",t2.MCpdgCode,t1.MCpdgCode));
            }
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
        for (auto t: tracks) t.Print( true );
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
