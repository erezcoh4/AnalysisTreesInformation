#ifndef CALCEVENTTOPOLOGIES_CXX
#define CALCEVENTTOPOLOGIES_CXX

#include "calcEventTopologies.h"
#include "MyLArTools.cxx"
#include "LArUtil/GeometryHelper.h"

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
                                         float fmax_mu_p_distance){
    
    SetInTree(fInTree);
    SetOutTree(fOutTree);
    
    SetDebug(fdebug);
    SetMCMode(fMCmode);
    SetMaxmupDistance(fmax_mu_p_distance);
    SetOption(foption);
    lar_tools = new MyLArTools();
    
    InitInputTree();
    InitOutputTree();
    
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
    OutTree -> Branch("CC1p_vertices"       ,&CC1p_vertices); // CC1p-vertices...
    
    if(debug>1) cout << "calcEventTopologies output-tree ready (" << OutTree -> GetTitle() << ")" << endl;
}


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
bool calcEventTopologies::FillOutTree (){
    NCC1pVertices = (int)CC1p_vertices.size();
    Nhits = (int)hits.size();
    Ntracks = (int)tracks.size();
    OutTree -> Fill();
    return true;
}



//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void calcEventTopologies::InitEvent(){
    
    Debug ( 2 , "calcEventTopologies::InitEvent");
    
    if (!tracks.empty())            tracks.clear();
    if (!hits.empty())              hits.clear();
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
    
    if (MCmode){
        genie_interactions = *finteractions;
    }
    
    c_entry = entry;
    delete ftracks;
    delete fhits;
    delete finteractions;
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
    bool    FoundCloseTracks , AlreadySetPosition;
    float   closest_distance_ij;
    TVector3 vertex_position;
    
    for (int i=0; i < Ntracks; i++){
        
        if (!tracks[i].IsFullyContained) continue;
        
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
            if (!tracks[j].IsFullyContained) continue;
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
                        //                        c_vertex.SetMyLArTools (lar_tools);
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
                c_vertex.SetClosestGENIE( c_genie_interaction );
            }
            

            // plug into vertices list
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
            // c_vertex.SetMyLArTools (lar_tools);
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
            
            float PmuonFromRange = 0.001 * lar_tools -> Get_muonMomentumFromRange( genie_interaction.muonTrack.length );
            float PprotonFromRange = 0.001 * lar_tools -> Get_protonMomentumFromRange( genie_interaction.protonTrack.length );
            
            c_vertex.SetAssignTracks( genie_interaction.muonTrack , genie_interaction.protonTrack , PmuonFromRange , PprotonFromRange );
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
            float PmuonFromRange = 0.001 * lar_tools -> Get_muonMomentumFromRange( v.SmallPIDATrack.length );
            float PprotonFromRange = 0.001 * lar_tools -> Get_protonMomentumFromRange( v.LargePIDATrack.length );

            v.SetAssignTracks( v.SmallPIDATrack , v.LargePIDATrack , PmuonFromRange , PprotonFromRange );
            
            // account for tracks that come out from the same vertex, but are not clustered with it since they are not fully contained.
            // if (MoreThanTwoCloseTracks( v )) continue;
            
            CC1p_vertices.push_back( v );
            CC1pVerticesID.push_back( v.vertex_id );
            FoundCC1pTopology = true;
            
        }
    }
    return FoundCC1pTopology;
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
        v.GENIECC1p = v.Is1mu1pDetected = false;
        
        PandoraNuTrack t1 = v.AssignedMuonTrack;
        PandoraNuTrack t2 = v.AssignedProtonTrack;

        if (    (t1.MCpdgCode==13 && t2.MCpdgCode==2212)
            ||  (t1.MCpdgCode==2212 && t2.MCpdgCode==13)  ){
            
        
            Debug(3,"found 1mu-1p detected");
            v.Is1mu1pDetected = true;
            
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
                //                && t1.IsGENIECC1p && t2.IsGENIECC1p
                && (v.closest_genie_interaction.vertex_position - v.position).Mag() < 10 // distance from the closest genie vertex
                && v.closest_genie_interaction.IsCC1p
                && (t1.process_primary==1 && t2.process_primary==1)
                ){
                
                Debug(3,"found true CC1p");
                v.GENIECC1p = true;
                v.Is1mu1pDetected = false; // I define Is1mu1pDetected as non true CC1p
                v.TruthTopologyString = "true GENIE CC1p";
                
            }

        }
        
        
        
    }
    return true;
}


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
std::vector<hit> calcEventTopologies::get_hits_in_plane(int plane){
    std::vector<hit> hits_in_this_plane;
    for (auto hit:hits){
        if (hit.hit_plane == plane){
            hits_in_this_plane.push_back(hit);
        }
    }
    return hits_in_this_plane;
}




//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
float calcEventTopologies::CollectAllChargeInROI( std::vector<hit> hits_in_this_plane , box roi ){
    float total_charge_in_roi = 0;
    for (auto hit: hits_in_this_plane){
        if ( hit_in_box(hit,roi) ){
            total_charge_in_roi += hit.hit_charge;
        }
    }
    Debug(4,Form("total charge in roi: %.1f [ADC]",total_charge_in_roi));
    return total_charge_in_roi;
}




//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
bool calcEventTopologies::PerformMyTracking(){
    // perform my own version of tracking in each plane (U/V/Y)
    // to ask what is the fraction of charge deposited in 2-tracks vertex
    // that is associated with the tracks.
    // this will provide a way to supress contributions from
    // CC 1p-reconstructed + X-unreconstructed
    for ( int plane = 0; plane < 3 ; plane++ ){
        
        hits_in_plane = get_hits_in_plane(plane);
        
        for (auto & vertex: CC1p_vertices){

            // (1) define the vertex position and ROI in each plane
            vertex.BuildROI( plane );
            vertex.BuildLocationInPlane( plane );
            
            // (2) collect the total charge deposited in each plane
            vertex.AllChargeInVertexROI[plane] = CollectAllChargeInROI( hits_in_plane , vertex.roi[plane] );

            // (3) associate hits to tracks
            vertex.AssociateHitsToTracks( plane , hits_in_plane );
            
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
