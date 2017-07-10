#ifndef CALCEVENTTOPOLOGIES_CXX
#define CALCEVENTTOPOLOGIES_CXX

#include "calcEventTopologies.h"
#include "MyLArTools.cxx"
#include "LArUtil/GeometryHelper.h"
#include "myVertex.h"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
calcEventTopologies::calcEventTopologies(TTree * fInTree){
    
    SetInTree(fInTree);
    lar_tools = new MyLArTools();
    InitInputTree();
    
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
calcEventTopologies::calcEventTopologies(TTree * fInTree, TTree * fOutTree,
                                         std::string foption,
                                         int fdebug,
                                         bool fMCmode,
                                         float fmax_mu_p_distance, TTree * fInEventsTree){
    
    SetInTree(fInTree);
    SetOutTree(fOutTree);
    debug = fdebug;

    SetMCMode(fMCmode);
    SetMaxmupDistance(fmax_mu_p_distance);
    SetOption(foption);
    lar_tools = new MyLArTools();
    
    InitInputTree();
    InitOutputTree();
    
    if (fInEventsTree) SetInEventsTree(fInEventsTree);

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
void calcEventTopologies::InitOutputTree(){
    vertices_ctr = 0;
    
    // Integer branches
    OutTree -> Branch("run"             ,&run               ,"run/I");
    OutTree -> Branch("subrun"          ,&subrun            ,"subrun/I");
    OutTree -> Branch("event"           ,&event             ,"event/I");
    OutTree -> Branch("Ntracks"         ,&Ntracks           ,"Ntracks/I");
    OutTree -> Branch("Nhits"           ,&Nhits             ,"Nhits/I");
    OutTree -> Branch("NCC1pVertices"   ,&NCC1pVertices     ,"NCC1pVertices/I");
    
    OutTree -> Branch("tracks"              ,&tracks); // tracks information...
    OutTree -> Branch("hits"                ,&hits); // hits information...
    OutTree -> Branch("vertices"            ,&vertices); // all vertices...
    //    OutTree -> Branch("CC1p_vertices"       ,&CC1p_vertices); // CC1p-vertices...
    //    OutTree -> Branch("CC_1p_200MeVc_0pi_vertices" ,&CC_1p_200MeVc_0pi_vertices); // CC_1p_200MeVc_0pi-vertices...
    
    if(debug>1) cout << "calcEventTopologies output-tree ready (" << OutTree -> GetTitle() << ")" << endl;
}


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
bool calcEventTopologies::FillOutTree (){
    NCC1pVertices = (int)CC1p_vertices.size();
    Nhits = (int)hits.size();
    Ntracks = (int)tracks.size();
    
    if (debug>3){
        Printf("Filling 2-tracks clusters tree at entry %d with vertices ",(int)OutTree->GetEntries());
        SHOWstdVector(CC1pVerticesID);
    }
    
    OutTree -> Fill();
    return true;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
bool calcEventTopologies::FillGENIEOutTree (){
    if ((int)CC1p_vertices.size()>0){
        return FillOutTree();
    }
    return false;
}


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void calcEventTopologies::InitEvent(){
    
    Debug ( 2 , "calcEventTopologies::InitEvent");
    ClearVertices();
    ClearHitsTracks();
    if (!protons.empty())           protons.clear();
    if (!genie_interactions.empty())genie_interactions.clear();

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
    Debug(3,"InTree -> GetEntry(entry);");
    genie_interactions = *finteractions;
    Debug(3,"got genie interactions");
    
    
    std::vector <PandoraNuTrack> * ftracks = 0;
    InEventsTree -> SetBranchAddress("tracks" , &ftracks);

    std::vector <hit> * fhits = 0;
    InEventsTree -> SetBranchAddress("hits" , &fhits);
    
    InEventsTree -> GetEntry(entry);
    
    Debug(3,"got hits and tracks");
    hits = *fhits;
    tracks = *ftracks;

    
    c_entry = entry;
    delete ftracks;
    delete fhits;
    delete finteractions;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void calcEventTopologies::GetRSEEntry (int entry){
    InTree -> GetEntry(entry);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void calcEventTopologies::GetEntry (int entry){
    InitEvent();
    
    std::vector <PandoraNuTrack> * ftracks = 0;
    InTree -> SetBranchAddress("tracks" , &ftracks);
    
    std::vector <hit> * fhits = 0;
    InTree -> SetBranchAddress("hits" , &fhits);
    
    std::vector <GENIEinteraction> * finteractions = 0;
    if (MCmode){
        InTree -> SetBranchAddress("genie_interactions" , &finteractions);
    }
    
    InTree -> GetEntry(entry);
    
    hits = *fhits;
    tracks = *ftracks;
    
    if (MCmode){ genie_interactions = *finteractions;    }
    
    c_entry = entry;
    delete ftracks;
    delete fhits;
    delete finteractions;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
bool calcEventTopologies::extract_information(){
    ClusterTracksToVertices();
    AnalyzeVertices();
    return (vertices.size()>0);
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
    Debug(3,"calcEventTopologies::ClusterTracksToVertices()");
    
    bool    FoundCloseTracks , AlreadySetPosition;
    float   closest_distance_ij;
    TVector3 vertex_position;
    
    for (int i=0; i < Ntracks; i++){
        
        //        if (!tracks[i].IsFullyContained) continue;
        if (!tracks[i].IsTrackContainedSoft()) continue;
        
        // skip if track was clustered to a vertex by in one of the previous loop steps
        if (TrackAlreadyIncludedInVerticesList(tracks[i].track_id)) continue;
        // is this wrong?! we need to go over all of the tracks, even if they are included in previous cluseters.
        // however we need to avoid from counting twice!

        c_vertex = myVertex( tracks[i].track_id );
        c_vertex.debug = debug ;
        c_vertex.SetRSE( tracks[i].run, tracks[i].subrun, tracks[i].event);
        c_vertex.AddTrack( tracks[i] );
        
        FoundCloseTracks = AlreadySetPosition = false;
        
        for ( int j=0 ; j < Ntracks ; j++ ){ // i+1
            
            // if (!tracks[j].IsFullyContained) continue;
            if (tracks[j].IsTrackContainedSoft() && j!=i){
                
                // if this is the first time we go over these two tracks
                // and they are close enough to define a vertex,
                // we also define the position of their mutual vertex
                if (!AlreadySetPosition){

                    // two close tracks (at a separation distance smaller that max_mu_p_distance)
                    std::string StartOrEnd = "None";
                    closest_distance_ij = tracks[i].ClosestDistanceToOtherTrack(tracks[j],&StartOrEnd);
                    
                    Debug(5, Form( "closest-distance(i=%d,j=%d)=%f",i,j, closest_distance_ij));
                    
                    if ( closest_distance_ij < max_mu_p_distance ){
                        
                        
                        c_vertex.AddTrack( tracks[j] );
                        FoundCloseTracks = true;
                        
                        if (StartOrEnd.compare("Start")==0)     vertex_position = tracks[i].start_pos ;
                        else if (StartOrEnd.compare("End")==0)  vertex_position = tracks[i].end_pos ;
                        else                                    vertex_position = TVector3(-1000,-1000,-1000) ;
                        
                        c_vertex.SetPosition( vertex_position );
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
            
            if (debug>3){
                Printf("FoundCloseTracks");
                SHOW(genie_interactions.size());
            }
            // match the closest GENIE interaction
            GENIEinteraction c_genie_interaction;
            bool MatchedGENIEinteraction = false;
            float closest_genie_interaction_distance = 10000; // [cm]
            
            for (auto genie_interaction : genie_interactions){
                float c_genie_interaction_distance = (genie_interaction.vertex_position - c_vertex.position).Mag();
                if ( c_genie_interaction_distance < closest_genie_interaction_distance ){
                    closest_genie_interaction_distance = c_genie_interaction_distance;
                    c_genie_interaction = genie_interaction;
                    MatchedGENIEinteraction = true;
                    break;
                }
            }
            if (MatchedGENIEinteraction){
                if (debug>3){
                    SHOWTVector3(c_genie_interaction.vertex_position);
                }
                c_vertex.SetClosestGENIE( c_genie_interaction );
            }
            

            // plug into vertices list
            vertices.push_back( c_vertex );
        }
    }
    return true;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
bool calcEventTopologies::ClusterGENIEToVertices( int vertex_id ){

    Debug (2 , "calcEventTopologies::ClusterGENIEToVertices()");
    for (auto genie_interaction : genie_interactions){
        
        float PmuonFromRange = -100 , PprotonFromRange = -100;
        
        if (genie_interaction.IsCC1p){ // we only consider true CC1p vertices

            
            GENIEmuon = genie_interaction.muon;
            protons = genie_interaction.protons;
            GENIEproton = protons[0];
            
            c_vertex = myVertex( vertex_id );
            c_vertex.debug = debug ;
            c_vertex.SetPosition( genie_interaction.vertex_position );
            c_vertex.SetRSE( genie_interaction.run, genie_interaction.subrun, genie_interaction.event );
            c_vertex.GENIECC1p = true;
            
            // containment
            c_vertex.IsVertexContained = genie_interaction.IsVertexContained;
            
            // muon track
            c_vertex.muonTrackReconstructed = genie_interaction.muonTrackReconstructed;
            if (genie_interaction.muonTrackReconstructed)   {
                c_vertex.AddTrack( genie_interaction.muonTrack );
                c_vertex.muonTrueTrack = genie_interaction.muonTrack;
                PmuonFromRange = 0.001 * lar_tools -> Get_muonMomentumFromRange( genie_interaction.muonTrack.length );
                Debug( 3 , "appended muon track" );
            }
            
            // proton track
            c_vertex.protonTrackReconstructed = genie_interaction.protonTrackReconstructed;
            if (genie_interaction.protonTrackReconstructed) {
                c_vertex.AddTrack( genie_interaction.protonTrack );
                c_vertex.protonTrueTrack = genie_interaction.protonTrack ;
                PprotonFromRange = 0.001 * lar_tools -> Get_protonMomentumFromRange( genie_interaction.protonTrack.length );
                Debug( 3 , "appended proton track" );
            }
            
            c_vertex.SetIsReconstructed( max_mu_p_distance );
            c_vertex.SetGENIEinfo( genie_interaction );
            c_vertex.SetClosestGENIE( genie_interaction );
            
            
            if (debug>3) { Printf("---------------\nfound true CC1p:\n---------------");genie_interaction.Print( true ); PrintLine();}
            
            if( genie_interaction.muonTrackReconstructed || genie_interaction.protonTrackReconstructed ){
                c_vertex.SetAssignTracks( genie_interaction.muonTrack , genie_interaction.protonTrack , PmuonFromRange , PprotonFromRange );
                for (int plane=0; plane<3; plane++) c_vertex.BuildROI(plane);
            }
            c_vertex.TruthTopologyString = "True GENIE CC1p";
            Debug( 3 , Form("pushing genie vertex %d in vertices",c_vertex.vertex_id) );
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
        
        if (v.tracks.size()<2) continue;
        
        if (option.find("Topology") != std::string::npos){ //if (option.compare("Topology")==0){
            v.RemoveFarTracks( max_mu_p_distance );
        }
        
        v.SortTracksByPIDA();
        v.SortTracksByLength();
        v.SetTracksRelations();

        if (option.find("Topology") != std::string::npos){ //if (option.compare("Topology")==0){
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
bool calcEventTopologies::FindVerticesWithGoodTopology(){

    bool FoundGoodTopology = false;
    for (auto v:vertices) {
        if (v.CCQETopology){
            CC1p_vertices.push_back(v);
            CC1pVerticesID.push_back(v.vertex_id);
            //            CC_1p_200MeVc_0pi_vertices.push_back(v);
            FoundGoodTopology = true;
        }
    }
    return FoundGoodTopology;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
bool calcEventTopologies::Find2tracksVertices(){
    Debug(3 , "calcEventTopologies::Find2tracksVertices()");
    bool FoundGoodTopology = false;
    std::vector<myVertex> GoodVertices;
    if (debug>3) SHOW(vertices.size());
    for (auto & v:vertices) {
        
        if (debug>3) {SHOW2(v.tracks.size(), v.NearUncontainedTracks( tracks , max_mu_p_distance ).size() );} // PRINTOUT
        
        if (    v.tracks.size() == 2                        // we are looking for clusters of only two tracks that are fully contained
            &&  v.NearUncontainedTracks( tracks , max_mu_p_distance ).size() == 0       // if there is a semi-contained track, with start/end point too close to the vertex, we don't want the vertex...
            ){
            
            // vertices with only two tracks at close proximity and nothing else!
            
            // assign the muon and proton tracks by length
            //        PandoraNuTrack t1 = v.ShortestTrack;
            //        PandoraNuTrack t2 = v.LongestTrack;
            // assign muon and proton tracks by PID-A
            auto AssignedMuonTrack = v.SmallPIDATrack;
            auto AssignedProtonTrack = v.LargePIDATrack;
            
            float PmuonFromRange = 0.001 * lar_tools -> Get_muonMomentumFromRange( AssignedMuonTrack.length );
            float PprotonFromRange = 0.001 * lar_tools -> Get_protonMomentumFromRange( AssignedProtonTrack.length );

            v.SetAssignTracks( AssignedMuonTrack , AssignedProtonTrack , PmuonFromRange , PprotonFromRange );
            
            
            // account for tracks that come out from the same vertex, but are not clustered with it since they are not fully contained.
            // if (MoreThanTwoCloseTracks( v )) continue;
            v.vertex_id = vertices_ctr;
//            CC1p_vertices.push_back( v );
//            CC1pVerticesID.push_back( v.vertex_id );
//            CC_1p_200MeVc_0pi_vertices.push_back( v );
            FoundGoodTopology = true;
            vertices_ctr += 1;
            GoodVertices.push_back(v);
        }
    }
    ClearVertices();
    FillVertices(GoodVertices);
    return FoundGoodTopology;
}


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void calcEventTopologies::ClearHitsTracks(){
    if (!tracks.empty())            tracks.clear();
    if (!hits.empty())              hits.clear();
    if (!protonTracks.empty())      protonTracks.clear();
}


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void calcEventTopologies::ClearVertices(){
    if (!vertices.empty())                      vertices.clear();
    if (!CC1p_vertices.empty())                 CC1p_vertices.clear();
    if (!CC1pVerticesID.empty())                CC1pVerticesID.clear();
    if (!CC_1p_200MeVc_0pi_vertices.empty())    CC_1p_200MeVc_0pi_vertices.clear();
}


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void calcEventTopologies::FillVertices(std::vector<myVertex> fVertices){
    for (auto v:fVertices)  vertices.push_back(v);
}


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
int calcEventTopologies::CountCloseTracksToVertex(myVertex vertex){
    // count the number of tracks close to the vertex,
    // that are not nesecarily fully contained,
    // to account for tracks that come out from the same vertex,
    // but are not clustered with it since they are not fully contained.
    int NCloseTracks = 0;
    for (auto t:tracks) {
        if (t.DistanceFromPoint(vertex.position) < max_mu_p_distance){
            NCloseTracks++;
        }
    }
    return NCloseTracks;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
bool calcEventTopologies::MoreThanTwoCloseTracks(myVertex vertex){
    if ( CountCloseTracksToVertex(vertex) > 2 ) return true;
    return false;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
bool calcEventTopologies::TagGENIECC1p(){
    // for events-tree, in which we only have tracks information
    // and cluster vertices, we need to tag GENIE true-CC1p vertices
    // which will enable us to then ask what is the purity of the sample
    // after selection cuts applied
    // This method is independent of the tracks-sorting algorithm in the vertex
    for (auto & v:CC1p_vertices){
        v.Non1mu1p = true;
        v.GENIECC1p = false;
        v.Is1mu1pDetected = false;
        v.BothTracksAreGENIECC1p = false;
        v.TruthTopologyString = "unknown vertex";
        
        PandoraNuTrack t1 = v.AssignedMuonTrack;
        PandoraNuTrack t2 = v.AssignedProtonTrack;

        if (    (t1.MCpdgCode==13 && t2.MCpdgCode==2212)
            ||  (t1.MCpdgCode==2212 && t2.MCpdgCode==13)  ){
            
        
            Debug(3,"found 1mu-1p detected");
            v.Is1mu1pDetected = true;
            v.Non1mu1p = false;
            v.TruthTopologyString = "1 muon - 1 proton";
            
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
                Debug(4,Form("found the p (track %d) and mu (track %d) in this 1mu-1p",t1.track_id,t2.track_id));
                v.protonTrueTrack = t1;
                v.muonTrueTrack = t2;
                v.muonTrackReconstructed = v.protonTrackReconstructed = v.IsVertexReconstructed = true;
            }
            else if ( t1.MCpdgCode==13 && t2.MCpdgCode==2212 ){
                Debug(4,Form("found the p (track %d) and mu (track %d) in this 1mu-1p",t2.track_id,t1.track_id));
                v.muonTrueTrack = t1;
                v.protonTrueTrack = t2;
                v.muonTrackReconstructed = v.protonTrackReconstructed = v.IsVertexReconstructed = true;
            }
            else{
                Debug(4,Form("could not find p and mu in this 1mu-1p - pdg codes are %d/%d",t2.MCpdgCode,t1.MCpdgCode));
            }
            
            
            
            if ( (t1.truth_start_pos - t2.truth_start_pos).Mag() < 1. // distance between the true position of the two tracks is small
                && (v.closest_genie_interaction.vertex_position - v.position).Mag() < 10 // distance from the closest genie vertex
                && (v.closest_genie_interaction.IsCC1p)
                && (t1.process_primary==1 && t2.process_primary==1)
                && (t1.IsGENIECC1p && t2.IsGENIECC1p)
                ){
                
                Debug(3,"found true CC1p");
                v.GENIECC1p = true;
                v.Is1mu1pDetected = false; // I define Is1mu1pDetected as non true CC1p
                v.TruthTopologyString = "true GENIE CC1p";
                v.BothTracksAreGENIECC1p = true;
                
            }

        }
    }
    return true;
}



//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
bool calcEventTopologies::TagVertices(){
    // tag vertices
    // ------------
    // µp           : event with a muon and a proton detected, and nothing else at close proximity
    // CC 1p 0π     : a subset of µp, in which only one proton with momentum > 200 MeV/c was produced, and no pions
    // non µp       : other pairs
    // cosmic       : pairs from cosmic (at least one of the tracks is cosmic, i.e. without MC information)
    
    for (auto & v:vertices){
        v.Initiate ();
        
        PandoraNuTrack t1 = v.AssignedMuonTrack;
        PandoraNuTrack t2 = v.AssignedProtonTrack;
        
        if ( (t1.MCpdgCode*t2.MCpdgCode)==(13*2212) ){
            
            v.SetAs1mu1p();
            v.MatchGenieInteraction ( genie_interactions , t1 );
            v.SetMuonProton( t1 , t2 );

            
            if (debug>5){
                SHOWTVector3(t1.truth_start_pos);
                SHOWTVector3(t2.truth_start_pos);
                SHOWTVector3(v.closest_genie_interaction.vertex_position);
                SHOW((v.closest_genie_interaction.vertex_position - v.position).Mag());
                SHOW(v.closest_genie_interaction.IsCC_1p_200MeVc_0pi);
                SHOW2(t1.process_primary,t2.process_primary);
                SHOW2(t1.IsGENIECC_1p_200MeVc_0pi,t2.IsGENIECC_1p_200MeVc_0pi);
            }
            if ( (t1.truth_start_pos - t2.truth_start_pos).Mag() < 1. // distance between the true position of the two tracks is small
                && (v.closest_genie_interaction.vertex_position - v.position).Mag() < 10 // distance from the closest genie vertex
                && (v.closest_genie_interaction.IsCC_1p_200MeVc_0pi)
                && (t1.process_primary==1 && t2.process_primary==1)
                && (t1.IsGENIECC_1p_200MeVc_0pi && t2.IsGENIECC_1p_200MeVc_0pi)
                ){
                v.SetAsCC_1p_200MeVc_0pi();
            }
        }
        else if ( t1.MCpdgCode!=-9999 && t2.MCpdgCode!=-9999 ){
            v.SetAsNon1mu1p();
        }
        else {
            v.SetAsCosmic();
        }
    }
    return true;
}




//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
std::vector<hit> calcEventTopologies::get_hits_in_plane(int plane){
    if(!hits_in_plane[plane].empty()) hits_in_plane[plane].clear();
    
    for (auto hit:hits){
        if (hit.hit_plane == plane){
            hits_in_plane[plane].push_back(hit);
        }
    }
    return hits_in_plane[plane];
}


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
int calcEventTopologies::ClosestTrackToHit( int plane , hit c_hit , myVertex vertex ){
    // input:   hit
    // return:  track-id of the closest (pandoraNu) track to the hit
    // point-line distance calculation referenced from [http://mathworld.wolfram.com/Point-LineDistance2-Dimensional.html]
    // distance from point to line = {(x2-x1)*(y1-y0) - (x1-x0)*(y2-y1)))}/sqrt( (x2-x1)^2 + (y2-y1)^2 )    
    
    int closest_track_id=-100;
    float x0 = c_hit.hit_wire, y0 = c_hit.hit_peakT;
    float x1=0 , y1=0 , x2=0 , y2=0 , DistanceToClosestTrack = 10000;
    float TrackLength , DistanceToTrack , DistanceToFlippedTrack;
    
    if (debug>3) {  Printf("-------\nin calcEventTopologies::ClosestTrackToHit(%d,%d,%.1f)",c_hit.hit_plane,c_hit.hit_wire,c_hit.hit_peakT); }
    for (auto t: tracks) {
        
        std::vector<float> x1y1x2y2 = t.GetX1Y1X2Y2forTrack( plane );
        if (debug>3) { Printf("t.track_id:%d (%d)",t.track_id,t.MCpdgCode); PrintBox(t.roi[plane].EnlargeBox( 5 , 10 ) );}
        
        if ( c_hit.InBox( t.roi[plane].EnlargeBox( 5 , 10 ) ) ){
            
            x1 = x1y1x2y2[0]; y1 = x1y1x2y2[1]; x2 = x1y1x2y2[2]; y2 = x1y1x2y2[3];
            if (debug>3) { printf("In box, ");SHOW4(x1 , y1 , x2 , y2); }
            
            //            TrackLength = sqrt( 3.0*(x2-x1)*3.0*(x2-x1) + 0.557*(y2-y1)*0.557*(y2-y1) );
            TrackLength = sqrt( (x2-x1)*(x2-x1) + (y2-y1)*(y2-y1) );
            
            if (TrackLength==0) TrackLength = 0.1;
            
            if (debug>3){Printf("TrackLength %.2f mm",TrackLength);
                SHOW3(3.0*(x2-x1),3.0*(x1-x0),3.0*(x2-x0));
                SHOW3(0.557*(y1-y0),0.557*(y2-y1),0.557*(y2-y0))}
            
            DistanceToTrack = fabs( (x2-x1) * (y1-y0) - (x1-x0) * (y2-y1) ) / TrackLength; // 3.0*0.557*
            if (debug>3){Printf("distance of hit (%d,%.1f) to track %d is %.2f mm",
                                c_hit.hit_wire,c_hit.hit_peakT,t.track_id,DistanceToTrack);}
            
            if (t.track_id!=vertex.AssignedMuonTrack.track_id && t.track_id!=vertex.AssignedProtonTrack.track_id){
            x1 = x1y1x2y2[0]; y2 = x1y1x2y2[1]; x2 = x1y1x2y2[2]; y1 = x1y1x2y2[3];
            DistanceToFlippedTrack = fabs( (x2-x1) * (y1-y0) - (x1-x0) * (y2-y1) ) / TrackLength; // 3.0*0.557*
            if (DistanceToFlippedTrack < DistanceToTrack) DistanceToTrack = DistanceToFlippedTrack;
            if (debug>3){Printf("distance of hit (%d,%.1f) to flipped-track x1/y2 %d is %.2f mm",
                                c_hit.hit_wire,c_hit.hit_peakT,t.track_id,DistanceToFlippedTrack);}
            
            
            x1 = x1y1x2y2[2]; y1 = x1y1x2y2[1]; x2 = x1y1x2y2[0]; y2 = x1y1x2y2[3];
            DistanceToFlippedTrack = fabs( (x2-x1) * (y1-y0) - (x1-x0) * (y2-y1) ) / TrackLength; // 3.0*0.557*
            if (debug>3){Printf("distance of hit (%d,%.1f) to flipped-track x2/y1 %d is %.2f mm (DistanceToClosestTrack=%.1f)",
                                c_hit.hit_wire,c_hit.hit_peakT,t.track_id,DistanceToFlippedTrack,DistanceToClosestTrack);}
            }
            
            Debug(3,Form("distance of hit (%d,%.1f) to track %d is %.2f mm (DistanceToClosestTrack=%.1f)",
                         c_hit.hit_wire,c_hit.hit_peakT,t.track_id,DistanceToTrack,DistanceToClosestTrack));
            
            if ( DistanceToTrack < DistanceToClosestTrack ){
                DistanceToClosestTrack = DistanceToTrack;
                closest_track_id = t.track_id;
                if (debug>3) Printf("changed closest_track_id to %d (DistanceToClosestTrack=%.1f)",closest_track_id,DistanceToClosestTrack);
            }
            
        }
        
    }
    if (debug>3) SHOW(closest_track_id);
    return closest_track_id;
}


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
bool calcEventTopologies::PerformMyTracking(){
    // perform my own version of tracking in each plane (U/V/Y)
    // to ask what is the fraction of charge deposited in 2-tracks vertex
    // that is associated with the tracks.
    // this will provide a way to supress contributions from
    // CC 1p-reconstructed + X-unreconstructed
    for ( int plane = 0; plane < 3 ; plane++ ){
        hits_in_plane[plane] = get_hits_in_plane(plane);
        if (debug>5) {
            Printf("hits_in_plane %d:", plane);
            for (auto hit:hits_in_plane[plane]) hit.Print();
        }
    }
    
    if (debug>3) SHOW3(run,subrun,event);
    
    for (auto & vertex: CC1p_vertices){
    
        for ( int plane = 0; plane < 3 ; plane++ ){
                
            if(     ( (option.compare("GENIECC1p")==0) && vertex.GENIECC1p && vertex.IsVertexReconstructed ) // GENIE
               ||   ( (option.compare("CC1pTopology")==0) )  // MC / data
            ){
                
                Debug(3,Form("performing tracking for vertex %d in plane %d",vertex.vertex_id,plane));
                
                // (1) define the vertex position and ROI in each plane
                vertex.BuildROI( plane );
                
                if (debug>5){
                    Printf("%d/%d/%d vertex roi in plane %d: ",run,subrun,event,plane); // PRINTOUT
                    PrintBox(vertex.roi[plane]); // PRINTOUT
                }
                
                vertex.BuildLocationInPlane( plane );
                
                
                // (2) collect the total charge deposited in each plane
                vertex.CollectAllHitsInROI( plane , hits_in_plane[plane] );
                
                if (debug>4){
                    Printf("vertex %d, AllHitsInROI[%d]:",vertex.vertex_id, plane);
                    for (auto hit:vertex.AllHitsInROI[plane]) hit.Print();
                }
                
                
                // (3) associate hits to tracks
                vertex.SetDistanceThreshold( 10.0 ); // mm
                vertex.SetAngleThreshold( 0.523 ); // rad.
                
                vertex.SetTracksParameters( plane );
                
                for (auto &t: tracks) t.SetX1Y1X2Y2forTrack( plane , vertex.GetX1Y1X2Y2forTrack( plane , t ) );
                Debug(4,"looking for ClosestTrackToHit( plane , hit , vertex )");
                for (auto &hit:vertex.AllHitsInROI[plane]) {
                    hit.ClosestTrack_track_id = ClosestTrackToHit( plane , hit , vertex );
                }
                
                vertex.AssociateHitsToTracks( plane );
                vertex.CollectAssociatedCharge( plane );
            }
        }
    }
    
    return true;
}


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void calcEventTopologies::Print(bool DoPrintGENIE, bool DoPrintTracks, bool DoVertices){
    Printf("\n\t[%.1f%%]\n",100.*(float)c_entry/Nentries);
    
    PrintLine();    SHOW3( run , subrun , event );      PrintLine();
    
    if(DoPrintGENIE && !genie_interactions.empty()){
        cout << "\033[33m" << "xxxxxxxxxxxxxx\n\n" << genie_interactions.size() << " genie interactions\n\n" << "xxxxxxxxxxxxxx"<< "\033[30m" << endl;
        for (auto g: genie_interactions) {
            g.Print(true);
        }
    }

    if (DoPrintTracks && !tracks.empty()){
        cout << "\033[33m" << "xxxxxxxxxxxxxx\n\n" << tracks.size() << " pandoraNu tracks\n\n" << "xxxxxxxxxxxxxx"<< "\033[30m" << endl;
        for (auto t: tracks) t.Print( true );
    }
    
    if (DoVertices && !vertices.empty()){
        cout << "\033[33m" << "xxxxxxxxxxxxxx\n\n" << vertices.size() << " vertices\n\n" << "xxxxxxxxxxxxxx"<< "\033[30m" << endl;
        for (auto v: vertices) v.Print();
    }
    
    //    if (!CC1p_vertices.empty()){
    //        cout << "\033[33m" << "xxxxxxxxxxxxxx\n\n" << CC1p_vertices.size() << " CC1p vertices\n\n" << "xxxxxxxxxxxxxx"<< "\033[37m" << endl;
    //        for (auto v: CC1p_vertices) v.Print();
    //    }
    
    //    if (!CC_1p_200MeVc_0pi_vertices.empty()){
    //        cout << "\033[33m" << "xxxxxxxxxxxxxx\n\n" << CC_1p_200MeVc_0pi_vertices.size() << " CC-1p(200MeV/c)-0pi vertices\n\n" << "xxxxxxxxxxxxxx"<< "\033[37m" << endl;
    //        for (auto v: CC_1p_200MeVc_0pi_vertices) v.Print();
    //    }
    //    
    EndEventBlock();
}



#endif
