#ifndef MYVERTEX_CXX
#define MYVERTEX_CXX

#include "myVertex.h"
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
myVertex::myVertex(Int_t fID){
    SetVertexID(fID);
    GENIECC1p = CC1pTopology = false;
    TruthTopologyString = "not a GENIE CC1p";
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void myVertex::AddTrack (PandoraNuTrack ftrack){
    
    tracks.push_back(ftrack);
    Ntracks=(int)tracks.size();
    AddTrackID(ftrack.track_id);
    
    if (ftrack.truth_Eng>0 && ftrack.truth_P>0){
        GENIEtracks.push_back(ftrack);
        AddGENIETrackID(ftrack.track_id);
    }
    else{
        NonGENIEtracks.push_back(ftrack);
        AddNonGENIETrackID(ftrack.track_id);
    }

}



//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
bool myVertex::IncludesTrack (Int_t ftrack_id){
    for (auto track:tracks){
        if (track.track_id==ftrack_id) return true;
    }
    return false;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
bool myVertex::SortTracksByLength(){
    std::vector<PandoraNuTrack> tmp_tracks = tracks;
    for (auto i:sort_l( tmp_tracks )){
        tracks_lengthsorted.push_back( tmp_tracks.at(i) );
    }
    ShortestTrack = tracks_lengthsorted.at( tmp_tracks.size() - 1 );
    LongestTrack = tracks_lengthsorted.at( 0 );
    return true;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
bool myVertex::SortTracksByPIDA(){
    std::vector<PandoraNuTrack> tmp_tracks = tracks;
    for (auto i:sort_pida( tmp_tracks )){
        tracks_pidasorted.push_back( tmp_tracks.at(i) );
    }
    SmallPIDATrack = tracks_pidasorted.at( tmp_tracks.size() - 1 );
    LargePIDATrack = tracks_pidasorted.at( 0 );
    return true;
}


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
vector<size_t> myVertex::sort_l(const vector<PandoraNuTrack> &v) {
    std::vector<size_t> idx(v.size());
    for (size_t i = 0; i != idx.size(); ++i) idx[i] = i;
    std::sort(idx.begin(), idx.end(), [&v](size_t i1, size_t i2) {return v[i1].length > v[i2].length;});
    return idx;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
vector<size_t> myVertex::sort_pida(const vector<PandoraNuTrack> &v) {
    std::vector<size_t> idx(v.size());
    for (size_t i = 0; i != idx.size(); ++i) idx[i] = i;
    std::sort(idx.begin(), idx.end(), [&v](size_t i1, size_t i2) {return v[i1].pidpida > v[i2].pidpida;});
    return idx;
}



//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
bool myVertex::SetKinematicalTopology(float min_length_long,
                                      float max_length_short,
                                      float delta_phi_min,  float delta_phi_max,
                                      float PIDA_short_min, float PIDA_short_max,
                                      float PIDA_long_min,  float PIDA_long_max){
    
    // I definde a CC1p vertex topolgy by the following:
    // (1) two tracks, and only two tracks
    //      - one is a long (longer than min_length_muon), with a PIDA ~ proton
    //      - the other is short (shorteer than max_length_proton), with a PIDA ~ muon
    // (2) they are closer than 10 cm
    // (3) their azimuthal angular difference corresponds to QE ~ 180 (delta_phi_min < delta_phi < delta_phi_max)
    
    CC1pTopology = false;
    TopologyString = "just a weird topology";
    if (
        tracks.size() == 2
        && LongestTrack.length > min_length_long
        && ShortestTrack.length < max_length_short
        && (delta_phi_min < delta_phi_ij[0] && delta_phi_ij[0] < delta_phi_max)
        && (PIDA_short_min < ShortestTrack.pidpida && ShortestTrack.pidpida < PIDA_short_max)
        && (PIDA_long_min < LongestTrack.pidpida && LongestTrack.pidpida < PIDA_long_max)
        ){
        CC1pTopology = true;
        TopologyString = "CC1p topology";
        return true;
    }
    
    return false;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
float myVertex::GetAngleBetween2tracks(){
    TVector3 t1_direction, t2_direction;
    t1_direction.SetMagThetaPhi ( 1 , ShortestTrack.theta, ShortestTrack.phi );
    t2_direction.SetMagThetaPhi ( 1 , LongestTrack.theta, LongestTrack.phi );
    return t1_direction.Angle( t2_direction );
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void myVertex::SetTracksRelations(){

    for(int i = 0; i < Ntracks ; i++){
        
        tracks_dis_from_vertex.push_back( tracks[i].DistanceFromPoint( position ) );
        
        std::vector<float> distances_track_i;
        std::vector<float> delta_phi_track_i;
        std::vector<float> delta_theta_track_i;
        
        for(int j = 0; j < Ntracks ; j++){
            distances_track_i.push_back( tracks[i].ClosestDistanceToOtherTrack(tracks[j]) );
            delta_phi_track_i.push_back( r2d*fabs(tracks[i].phi - tracks[j].phi) );
            delta_theta_track_i.push_back( r2d*fabs(tracks[i].theta - tracks[j].theta) );
            
            
            if (i==0 && j!=i) {
                distances_ij.push_back( distances_track_i.back() );
                delta_phi_ij.push_back( delta_phi_track_i.back() );
                delta_theta_ij.push_back( delta_theta_track_i.back() );
            }
        }
        tracks_distances.push_back( distances_track_i );
        tracks_delta_phi.push_back( delta_phi_track_i );
        tracks_delta_theta.push_back( delta_theta_track_i );
        
        
        distances_track_i.clear();
        delta_phi_track_i.clear();
        delta_theta_track_i.clear();
    }


}



//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
bool myVertex::SetIsReconstructed(){
    if (IsVertexContained && muonTrackReconstructed && protonTrackReconstructed){
        IsVertexReconstructed = true;
        reco_mu_p_distance = muonTrueTrack.ClosestDistanceToOtherTrack( protonTrueTrack );
        return true;
    }
    else {
        IsVertexReconstructed = false;
        return false;
    }
}


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void myVertex::SetAssignTracks(PandoraNuTrack fAssignedMuonTrack,
                               PandoraNuTrack fAssignedProtonTrack,
                               float PmuonFromRange, float PprotonFromRange){
    
    AssignedMuonTrack = fAssignedMuonTrack;
    AssignedProtonTrack = fAssignedProtonTrack;
    FixTracksDirections ();
    SetEDepAroundVertex ();
    SetReconstructedFeatures ( PmuonFromRange , PprotonFromRange );

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void myVertex::FixTracksDirections(){
    // for CC1p events, we can fix the directions of the track
    // by looking at the reconstructed vertex position
    // and comparing the start/end point of the track to the vertex position
    
    // first fix the position of the vertex
    float min_distance = (AssignedMuonTrack.start_pos - AssignedProtonTrack.start_pos).Mag();
    position = 0.5*(AssignedMuonTrack.start_pos + AssignedProtonTrack.start_pos);
    
    if (min_distance > (AssignedMuonTrack.end_pos - AssignedProtonTrack.start_pos).Mag()){
        min_distance = (AssignedMuonTrack.end_pos - AssignedProtonTrack.start_pos).Mag();
        position = 0.5*(AssignedMuonTrack.end_pos + AssignedProtonTrack.start_pos);
    }
    
    if (min_distance > (AssignedMuonTrack.start_pos - AssignedProtonTrack.end_pos).Mag()){
        min_distance = (AssignedMuonTrack.start_pos - AssignedProtonTrack.end_pos).Mag();
        position = 0.5*(AssignedMuonTrack.start_pos + AssignedProtonTrack.end_pos);
    }
    
    if (min_distance > (AssignedMuonTrack.end_pos - AssignedProtonTrack.end_pos).Mag()){
        min_distance = (AssignedMuonTrack.end_pos - AssignedProtonTrack.end_pos).Mag();
        position = 0.5*(AssignedMuonTrack.end_pos + AssignedProtonTrack.end_pos);
    }
    
    // then, flip the tracks accordingly
    if ( (AssignedMuonTrack.end_pos - position).Mag() < (AssignedMuonTrack.start_pos - position).Mag() ){
        AssignedMuonTrack.FlipTrack();
        // if (GENIECC1p) Printf("Flipped muon track");
    }
    if ( (AssignedProtonTrack.end_pos - position).Mag() < (AssignedProtonTrack.start_pos - position).Mag() ){
        AssignedProtonTrack.FlipTrack();
        // if (GENIECC1p) Printf("Flipped proton track");
    }
}


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void myVertex::SetEDepAroundVertex(){
    
    dqdx_around_vertex_non_tracks_associated = dqdx_around_vertex_tracks_associated = dqdx_around_vertex = -10000;
    
    dqdx_around_vertex = AssignedMuonTrack.dqdx_around_start_total + AssignedProtonTrack.dqdx_around_start_total;
    
    dqdx_around_vertex_tracks_associated = AssignedMuonTrack.dqdx_around_start_track_associated_total + AssignedProtonTrack.dqdx_around_start_track_associated_total;
    
    if(dqdx_around_vertex_tracks_associated!=0 && dqdx_around_vertex!=0
       && dqdx_around_vertex_tracks_associated!=-10000 && dqdx_around_vertex!=-10000){
        dqdx_around_vertex_non_tracks_associated = dqdx_around_vertex - dqdx_around_vertex_tracks_associated;
    }
    else {
        dqdx_around_vertex_non_tracks_associated = -10000;
    }
}



//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void myVertex::SetReconstructedMomenta( float PmuonFromRange, float PprotonFromRange ){
    // reconstruct the µ and p momenta by using the minimal features possible
    // theta / phi of the reconstructed track
    // and the momentum from stopping range ( Get_protonMomentumFromRange which gives the answer in MeV/c )
    // at a later stage we can maybe use calorimetery
    // p
    reco_CC1p_Pp_3momentum = PprotonFromRange;
    reco_CC1p_Pp_3vect.SetMagThetaPhi( reco_CC1p_Pp_3momentum , AssignedProtonTrack.theta , AssignedProtonTrack.phi );
    reco_CC1p_Pp.SetVectMag ( reco_CC1p_Pp_3vect , 0.9385 );
    
    // µ
    reco_CC1p_Pmu_3momentum = PmuonFromRange;
    reco_CC1p_Pmu_3vect.SetMagThetaPhi( reco_CC1p_Pmu_3momentum , AssignedMuonTrack.theta , AssignedMuonTrack.phi );
    reco_CC1p_Pmu.SetVectMag ( reco_CC1p_Pmu_3vect , 0.1056 );
    
    //        PrintPhys( (AssignedMuonTrack.truth_start_pos - AssignedMuonTrack.truth_end_pos).Mag() , "cm (true length)" );
    //        PrintPhys(AssignedMuonTrack.truth_P,"GeV/c (true momentum)");
    //        PrintPhys( AssignedMuonTrack.length , "cm (reco. length)" );
    //        PrintPhys(reco_CC1p_Pmu_3momentum,"GeV/c (reco. momentum from stopping range)");

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void myVertex::SetReconstructedBeamPz(){
    // reconstruct the beam Pz by using the reconstructed Pµ and Pp
    // in the z direction
    // for only CC1p events
    reco_CC1p_BeamPz = reco_CC1p_Pp.Pz() + reco_CC1p_Pmu.Pz();
    reco_CC1p_Pnu = TLorentzVector( 0 , 0 , reco_CC1p_BeamPz , reco_CC1p_BeamPz );
    
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void myVertex::SetReconstructed_q(){
    // reconstruct the momentum transfer from minimal features of CC1p
    // and stopping range
    reco_CC1p_q = reco_CC1p_Pnu - reco_CC1p_Pmu;
    // reconstructed 𝜃(p,q) based on these minimal features
    reco_CC1p_theta_pq = r2d * reco_CC1p_Pp.Vect().Angle( reco_CC1p_q.Vect() );
    reco_CC1p_p_over_q = reco_CC1p_Pp.P()/reco_CC1p_q.P();
    reco_CC1p_Xb = - reco_CC1p_q.Mag2() / (2*0.939*reco_CC1p_q.E());
    reco_CC1p_n_miss = reco_CC1p_Pp - reco_CC1p_q;
    
    // [from clas-note 2002-08]
    float theta_l = reco_CC1p_Pmu.Theta();
    float theta_p = reco_CC1p_Pp.Theta();
    float factor = 0.939 / ( 1. - cos( theta_l ) );
    reco_CC1p_Ev_from_angles = factor * ( cos( theta_l ) + cos( theta_p ) * ( sin( theta_l ) / sin( theta_p ) ) - 1. );
    reco_CC1p_Ev_from_angles_Ev_from_mu_p_diff = reco_CC1p_Ev_from_angles - reco_CC1p_Pnu.E();

    reco_CC1p_W2 = 0.939*(0.939 + 2*(reco_CC1p_Pnu.E() - reco_CC1p_Pmu.E())) - 4*reco_CC1p_Pnu.E()*reco_CC1p_Pmu.E()*(1.-cos(theta_l));

}


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void myVertex::SetReconstructedFeatures( float PmuonFromRange, float PprotonFromRange ){
    
    // reconstructed distance between µ and p
    reco_mu_p_distance = AssignedMuonTrack.ClosestDistanceToOtherTrack( AssignedProtonTrack );
    
    SetReconstructedMomenta( PmuonFromRange, PprotonFromRange );
    SetReconstructedBeamPz();
    SetReconstructed_q();

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
bool myVertex::RemoveFarTracks(float max_mu_p_distance, Int_t debug){
    // narrow down the set of tracks associated to the vertices by looking only
    // at those tracks that are close enough to the vertices
    float TrackStartDis2Vertex , TrackEndDis2Vertex;
    std::vector<Int_t>          CloseEnoughTracksID;
    std::vector<PandoraNuTrack> CloseEnoughTracks;

    // if (debug>4)     {    SHOWstdVector( track_id ); } // toDelete!
    for (auto t: tracks) {
        if ( t.DistanceFromPoint(position) < max_mu_p_distance ){
            CloseEnoughTracks.push_back( t );
            CloseEnoughTracksID.push_back( t.track_id );
        }
    }
    tracks = CloseEnoughTracks;
    track_id = CloseEnoughTracksID;
    Ntracks = tracks.size();
    // if (debug>4)     {    SHOWstdVector( track_id );  } // toDelete!

}


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void myVertex::Print(){
    //    if(GENIECC1p && IsVertexReconstructed) Printf("in Print");
    //    if(GENIECC1p && IsVertexReconstructed) SHOW2( reco_CC1p_Pmu.Phi() , reco_CC1p_Pmu.Theta());

    
    cout << "\033[35m" << "~~~~~\n" << "vertex " << counter_id << ", vertex-id " << vertex_id << "\n~~~~~~ "<< "\033[0m" << endl;
    // SHOW3( run , subrun , event );
    SHOWTVector3( position );
    for (auto t: tracks) {
        Printf("track %d (pdg %d), vertex distance %.1f cm",t.track_id,t.MCpdgCode,t.DistanceFromPoint( position ));
    }
    
    // tracks
    if (!tracks.empty()){
        cout << "\033[33m" << tracks.size() << " tracks in vertex " << "\033[37m" << endl;
        for (auto t: tracks) {
            if (t.track_id!=-100){
                t.Print( true );
            }
            else{
                Printf("unidentified/non-reconstructed track. continuing...");
            }
        }

        // inter-tracks distances
        cout << "\033[33m" << tracks.size()*tracks.size() << " inter-tracks distances " << "\033[37m" << endl;
        for(auto vec : tracks_distances) {
            for(auto x : vec)
            std::cout << setprecision(2) << x << "\t";
            std::cout << std::endl;
        }
    }
    cout << "topology: " << TopologyString << endl;
    cout << "truth topology: " << TruthTopologyString << endl;
    if (GENIECC1p){
        cout << "This vertex is a GENIE true CC1p" << endl;
        SHOW3( muonTrackReconstructed, protonTrackReconstructed, IsVertexReconstructed );
        SHOWTLorentzVector( genie_interaction.muon );
        SHOWTLorentzVector( reco_CC1p_Pmu );
        if (genie_interaction.protons.size()>0){
            SHOWTLorentzVector( genie_interaction.protons[0] );
        }
        SHOWTLorentzVector( reco_CC1p_Pp );
        SHOW2(genie_interaction.theta_pq,reco_CC1p_theta_pq);
    }

    
    //    Printf("truth muon");
    //    SHOW4( genie_interaction.muon.Phi() , AssignedMuonTrack.truth_phi , AssignedMuonTrack.phi , reco_CC1p_Pmu.Phi() );
    //    SHOW4( genie_interaction.muon.Theta() , AssignedMuonTrack.truth_theta , AssignedMuonTrack.theta  , reco_CC1p_Pmu.Theta() );

}


#endif
