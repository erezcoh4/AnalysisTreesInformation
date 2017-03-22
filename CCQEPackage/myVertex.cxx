#ifndef MYVERTEX_CXX
#define MYVERTEX_CXX

#include "myVertex.h"
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
myVertex::myVertex(Int_t fID){
    SetVertexID(fID);
    GENIECC1p = CC1pTopology = false;
    average_ratio_associated_hit_charge_to_total = max_ratio_associated_hit_charge_to_total = 0;
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
bool myVertex::SetIsReconstructed( float fmax_mu_p_distance ){
    if (IsVertexContained
        && muonTrackReconstructed && muonTrueTrack.IsTrackContainedSoft()
        && protonTrackReconstructed && protonTrueTrack.IsTrackContainedSoft()
        ){
        reco_mu_p_distance = muonTrueTrack.ClosestDistanceToOtherTrack( protonTrueTrack );
        if (reco_mu_p_distance < fmax_mu_p_distance){
            IsVertexReconstructed = true;
            return true;
        }
    }
    IsVertexReconstructed = false;
    return false;
    
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
    
    reco_CC1p_Pmu_3vect_mcsllhd.SetMagThetaPhi( AssignedMuonTrack.mommsllhd , AssignedMuonTrack.theta , AssignedMuonTrack.phi );
    reco_CC1p_Pmu_mcsllhd.SetVectMag ( reco_CC1p_Pmu_3vect_mcsllhd , 0.1056 );
    
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
    
    reco_CC1p_BeamPz_mcsllhd = reco_CC1p_Pp.Pz() + reco_CC1p_Pmu_mcsllhd.Pz();
    reco_CC1p_Pnu_mcsllhd = TLorentzVector( 0 , 0 , reco_CC1p_BeamPz_mcsllhd , reco_CC1p_BeamPz_mcsllhd );
    
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void myVertex::SetReconstructed_q(){
    // [http://pdg.lbl.gov/2014/reviews/rpp2014-rev-structure-functions.pdf]
    // reconstruct the momentum transfer from minimal features of CC1p
    // and stopping range
    reco_CC1p_q = reco_CC1p_Pnu - reco_CC1p_Pmu;
    reco_CC1p_omega = reco_CC1p_q.E();

    
    // reconstructed 𝜃(p,q) based on these minimal features
    reco_CC1p_theta_pq = r2d * reco_CC1p_Pp.Vect().Angle( reco_CC1p_q.Vect() );
    reco_CC1p_p_over_q = reco_CC1p_Pp.P()/reco_CC1p_q.P();
    
    reco_CC1p_Q2 = - reco_CC1p_q.Mag2();
    reco_CC1p_Q2_from_angles = 4.*reco_CC1p_Pnu.E()*reco_CC1p_Pmu.E()*sin(reco_CC1p_Pmu.Theta())*sin(reco_CC1p_Pmu.Theta());
    reco_CC1p_Q2_from_angles_diff = reco_CC1p_Q2_from_angles - reco_CC1p_Q2;
    reco_CC1p_Q2_from_angles_ratio = fabs(reco_CC1p_Q2)>0.01 ? reco_CC1p_Q2_from_angles / reco_CC1p_Q2 : 100.*reco_CC1p_Q2_from_angles;
    
    
    reco_CC1p_Xb = reco_CC1p_Q2 / (2*0.939*reco_CC1p_q.E());
    reco_CC1p_n_miss = reco_CC1p_Pp - reco_CC1p_q;
    reco_CC1p_y = reco_CC1p_omega/reco_CC1p_Pnu.E();
    reco_CC1p_s = reco_CC1p_Q2/(reco_CC1p_Xb*reco_CC1p_y) + 0.939*0.939 + 0.106*0.106;
    
    
    reco_CC1p_alpha_p = (reco_CC1p_Pp.E()-reco_CC1p_Pp.Pz())/0.931;
    reco_CC1p_alpha_mu = (reco_CC1p_Pmu.E()-reco_CC1p_Pmu.Pz())/0.931;
    reco_CC1p_alpha_q = (reco_CC1p_q.E()-reco_CC1p_q.Pz())/0.931;
    
    // [from clas-note 2002-08]
    float theta_l = reco_CC1p_Pmu.Theta();
    float theta_p = reco_CC1p_Pp.Theta();
    float factor = 0.939 / ( 1. - cos( theta_l ) );
    reco_CC1p_Ev_from_angles = factor * ( cos( theta_l ) + cos( theta_p ) * ( sin( theta_l ) / sin( theta_p ) ) - 1. );
    reco_CC1p_Ev_from_angles_Ev_from_mu_p_diff = reco_CC1p_Ev_from_angles - reco_CC1p_Pnu.E();
    reco_CC1p_Ev_from_angles_Ev_from_mu_p_ratio = fabs(reco_CC1p_Pnu.E())>0.01 ? reco_CC1p_Ev_from_angles / reco_CC1p_Pnu.E() : 100.*reco_CC1p_Ev_from_angles;
    
    reco_CC1p_W2 = 0.939*(0.939 + 2*(reco_CC1p_Pnu.E() - reco_CC1p_Pmu.E())) - 4*reco_CC1p_Pnu.E()*reco_CC1p_Pmu.E()*(1.-cos(theta_l));
    
    
    
    // from MCS
    reco_CC1p_q_mcsllhd = reco_CC1p_Pnu_mcsllhd - reco_CC1p_Pmu_mcsllhd;
    
    reco_CC1p_theta_pq_mcsllhd = r2d * reco_CC1p_Pp.Vect().Angle( reco_CC1p_q_mcsllhd.Vect() );
    reco_CC1p_p_over_q_mcsllhd = reco_CC1p_Pp.P()/reco_CC1p_q_mcsllhd.P();


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
bool myVertex::RemoveFarTracks(float max_mu_p_distance ){
    // narrow down the set of tracks associated to the vertices by looking only
    // at those tracks that are close enough to the vertices
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
    return true;
}


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
bool myVertex::BuildROI(int plane){
    
    // for GENIE vertices in which only one track was reconstructed, build the roi based on this track
    if (muonTrackReconstructed && !protonTrackReconstructed) {AssignedProtonTrack=AssignedMuonTrack;}
    if (!muonTrackReconstructed && protonTrackReconstructed) {AssignedMuonTrack=AssignedProtonTrack;}
    // -------------------------------------------------------------------------------------------------
    
    int wire_min = std::min( {AssignedMuonTrack.roi[plane].start_wire , AssignedMuonTrack.roi[plane].end_wire , AssignedProtonTrack.roi[plane].start_wire, AssignedProtonTrack.roi[plane].end_wire} );
    int wire_max = std::max( {AssignedMuonTrack.roi[plane].start_wire , AssignedMuonTrack.roi[plane].end_wire , AssignedProtonTrack.roi[plane].start_wire, AssignedProtonTrack.roi[plane].end_wire} );
    int time_min = std::min( {AssignedMuonTrack.roi[plane].start_time , AssignedMuonTrack.roi[plane].end_time , AssignedProtonTrack.roi[plane].start_time, AssignedProtonTrack.roi[plane].end_time} );
    int time_max = std::max( {AssignedMuonTrack.roi[plane].start_time , AssignedMuonTrack.roi[plane].end_time , AssignedProtonTrack.roi[plane].start_time, AssignedProtonTrack.roi[plane].end_time} );
    
    roi[plane] = box( wire_min - 10 , time_min - 30 , wire_max + 10 , time_max + 30 );

    switch (plane) {
        case 0:
            roi_u = roi[plane];
            break;
        case 1:
            roi_v = roi[plane];
            break;
        case 2:
        default:
            roi_y = roi[plane];
            break;
    }
    return true;
}


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
bool myVertex::BuildLocationInPlane(int plane){
    
    mu_start_wire[plane] = mu_start_time[plane] = p_start_wire[plane] = p_start_time[plane] = 0;
    mu_end_wire[plane] = mu_end_time[plane] = p_end_wire[plane] = p_end_time[plane] = 0;

    
    switch (plane) {
        case 0:
            mu_start_wire[plane] = AssignedMuonTrack.start_wire_u;
            mu_start_time[plane] = AssignedMuonTrack.start_time_u;
            mu_end_wire[plane] = AssignedMuonTrack.end_wire_u;
            mu_end_time[plane] = AssignedMuonTrack.end_time_u;
            
            p_start_wire[plane] = AssignedProtonTrack.start_wire_u;
            p_start_time[plane] = AssignedProtonTrack.start_time_u;
            p_end_wire[plane] = AssignedProtonTrack.end_wire_u;
            p_end_time[plane] = AssignedProtonTrack.end_time_u;
            
            break;
        case 1:
            mu_start_wire[plane] = AssignedMuonTrack.start_wire_v;
            mu_start_time[plane] = AssignedMuonTrack.start_time_v;
            mu_end_wire[plane] = AssignedMuonTrack.end_wire_v;
            mu_end_time[plane] = AssignedMuonTrack.end_time_v;
            
            p_start_wire[plane] = AssignedProtonTrack.start_wire_v;
            p_start_time[plane] = AssignedProtonTrack.start_time_v;
            p_end_wire[plane] = AssignedProtonTrack.end_wire_v;
            p_end_time[plane] = AssignedProtonTrack.end_time_v;

            break;
        case 2:
        default:
            mu_start_wire[plane] = AssignedMuonTrack.start_wire_y;
            mu_start_time[plane] = AssignedMuonTrack.start_time_y;
            mu_end_wire[plane] = AssignedMuonTrack.end_wire_y;
            mu_end_time[plane] = AssignedMuonTrack.end_time_y;
            
            p_start_wire[plane] = AssignedProtonTrack.start_wire_y;
            p_start_time[plane] = AssignedProtonTrack.start_time_y;
            p_end_wire[plane] = AssignedProtonTrack.end_wire_y;
            p_end_time[plane] = AssignedProtonTrack.end_time_y;
            
            break;
    }
    
    // first fix the position of the vertex
    TVector2 mu_start( mu_start_wire[plane] , mu_start_time[plane] );
    TVector2  mu_end( mu_end_wire[plane]    , mu_end_time[plane] );
    TVector2 p_start(  p_start_wire[plane]  , p_start_time[plane] );
    TVector2   p_end( p_end_wire[plane]     , p_end_time[plane] );
    
    float d_start_start = WireTimeDistance( mu_start.X() , mu_start.Y() , p_start.X() , p_start.Y() );
    float   d_end_start = WireTimeDistance( mu_end.X()   , mu_end.Y()   , p_start.X() , p_start.Y() );
    float   d_start_end = WireTimeDistance( mu_start.X() , mu_start.Y() , p_end.X()   , p_end.Y() );
    float     d_end_end = WireTimeDistance( mu_end.X()   , mu_end.Y()   , p_end.X()   , p_end.Y() );
    float d_min = std::min({ d_start_start , d_end_start , d_start_end , d_end_end });
    
    if (d_min==d_start_start){
        vertex_wire[plane] = 0.5*(mu_start.X() + p_start.X());
        vertex_time[plane] = 0.5*(mu_start.Y() + p_start.Y());
    }
    else if (d_min==d_end_start){
        vertex_wire[plane] = 0.5*(mu_end.X() + p_start.X());
        vertex_time[plane] = 0.5*(mu_end.Y() + p_start.Y());
    }
    else if (d_min==d_start_end){
        vertex_wire[plane] = 0.5*(mu_start.X() + p_end.X());
        vertex_time[plane] = 0.5*(mu_start.Y() + p_end.Y());
    }
    else if (d_min==d_end_end){
        vertex_wire[plane] = 0.5*(mu_end.X() + p_end.X());
        vertex_time[plane] = 0.5*(mu_end.Y() + p_end.Y());
    }
    
    // now flip the tracks that need to be flipped
    if ( WireTimeDistance( vertex_wire[plane] , vertex_time[plane] , mu_start_wire[plane] , mu_start_time[plane] ) > WireTimeDistance( vertex_wire[plane] , vertex_time[plane] , mu_end_wire[plane] , mu_end_time[plane] ) ){
        int tmp_wire = mu_start_wire[plane];
        mu_start_wire[plane] = mu_end_wire[plane];
        mu_end_wire[plane] = tmp_wire;
        int tmp_time = mu_start_time[plane];
        mu_start_time[plane] = mu_end_time[plane];
        mu_end_time[plane] = tmp_time;
    }
    if ( WireTimeDistance( vertex_wire[plane] , vertex_time[plane] , p_start_wire[plane] , p_start_time[plane] ) > WireTimeDistance( vertex_wire[plane] , vertex_time[plane] , p_end_wire[plane] , p_end_time[plane] ) ){
        int tmp_wire = p_start_wire[plane];
        p_start_wire[plane] = p_end_wire[plane];
        p_end_wire[plane] = tmp_wire;
        int tmp_time = p_start_time[plane];
        p_start_time[plane] = p_end_time[plane];
        p_end_time[plane] = tmp_time;
    }
    
    mu_angle[plane] = WireTimeAngle( (float)mu_start_wire[plane] ,  (float)mu_start_time[plane]
                                    , (float)mu_end_wire[plane] , (float)mu_end_time[plane] );
    AssignedMuonTrack.WireTimeAngle[plane] = mu_angle[plane];
    
    p_angle[plane] = WireTimeAngle( (float)p_start_wire[plane] ,  (float)p_start_time[plane]
                                    , (float)p_end_wire[plane] , (float)p_end_time[plane] );
    AssignedProtonTrack.WireTimeAngle[plane] = p_angle[plane];
    
    return true;
    
}



//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
std::vector<PandoraNuTrack> myVertex::RemoveTrackFromVector( vector<PandoraNuTrack> TracksVector , PandoraNuTrack TrackToBeRemoved ){
    auto element = std::find(std::begin(TracksVector), std::end(TracksVector), TrackToBeRemoved );
    if (element!=TracksVector.end()) {
        auto i_TrackToBeRemoved = std::distance( TracksVector.begin(), element );
        TracksVector.erase( TracksVector.begin() + i_TrackToBeRemoved );
        
    }
    return TracksVector;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
std::vector<PandoraNuTrack> myVertex::RemoveTracksFromVector( vector<PandoraNuTrack> TracksVector , vector<PandoraNuTrack> TracksToBeRemoved ){    
    // loop over tracks to be removed
    // and remove each of them
    for (auto t:TracksToBeRemoved) TracksVector = RemoveTrackFromVector( TracksVector , t );
    return TracksVector;
}


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
vector<PandoraNuTrack> myVertex::NearUncontainedTracks( vector<PandoraNuTrack> AllTracksInTheEvent , float fMaxDistance ){
    
    vector<PandoraNuTrack> NonVertexTracks = RemoveTracksFromVector( AllTracksInTheEvent , tracks );
    std::vector<PandoraNuTrack> NearTracks;
    for (auto track:NonVertexTracks) {
        if ( track.DistanceFromPoint(position) < fMaxDistance ){
            NearTracks.push_back(track);
        }
    }
    Debug(4,Form("NumberOfNearUncontainedTracks: %d",(int)NearTracks.size()));
    return NearTracks;
}


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
std::vector<hit> myVertex::RemoveHitFromVector( std::vector<hit> HitsVector , hit HitToBeRemoved ){
//    if (HitsVector.size()<1) return HitsVector;
    auto element = std::find(std::begin(HitsVector), std::end(HitsVector), HitToBeRemoved );
    if (element!=HitsVector.end()) {
        auto i_HitToBeRemoved = std::distance( HitsVector.begin(), element );
        HitsVector.erase( HitsVector.begin() + i_HitToBeRemoved );
    }
    return HitsVector;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
bool myVertex::FindClosestHitToVertex(int plane , std::vector<hit> fPossibleHits ){
    float DistanceToClosestHit = 1000;
    FoundClosestHitToVertex = false;
    ClosestHitToVertex[plane] = hit();
    for (auto hit : fPossibleHits){
        float DistanceFromVertexToHit = WireTimeDistance( (float)vertex_wire[plane] , (float)vertex_time[plane] , (float)hit.hit_wire , (float)hit.hit_peakT );
        if ((DistanceFromVertexToHit < DistanceToClosestHit) && (DistanceFromVertexToHit < 100)) { // do not consider start-hits that are distant from the vertex more than 10 cm
            DistanceToClosestHit = DistanceFromVertexToHit;
            ClosestHitToVertex[plane] = hit;
            FoundClosestHitToVertex = true;
        }
    }
    return FoundClosestHitToVertex;
}


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
bool myVertex::FoundCloseHitAlongTrackDirection( hit StartHit , std::vector<hit> PossibleHits ,
                                                float TrackAngle ){
    Debug(3, "myVertex::FoundCloseHitAlongTrackDirection()" );
    hit NextHit = FindNextHit( StartHit , StartHit , StartHit , PossibleHits , TrackAngle );
    if ( NextHit.exist() ) {
        Debug(3, Form("Found Close Hit Along Track Direction(%d,%.1f)",NextHit.hit_wire,NextHit.hit_peakT) );
        return true;
    }
    return false;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
hit myVertex::FindNextHit( hit StartHit, hit LastHit, hit CurrentHit, std::vector<hit> PossibleHits, float TrackAngle ){
    Debug(3, "myVertex::FindNextHit()" );
    hit NextHit;
    float DistanceToNextHit = 300, DistanceToHit;
    float AngleToHitFromStartHit , AngleToHitFromLastHit , AngleToHitFromCurrentHit;
    
    for (auto hit:PossibleHits) {
        if ( !(hit==CurrentHit) ){ // (!(hit==LastHit) && !(hit==CurrentHit))
            
            DistanceToHit = HitHitDistance ( CurrentHit , hit );
            AngleToHitFromStartHit = HitHitAngle ( StartHit , hit );
            AngleToHitFromLastHit = HitHitAngle ( LastHit , hit );
            AngleToHitFromCurrentHit = HitHitAngle ( CurrentHit , hit );
            
            if (debug>3 && DistanceToHit<100){
                Printf("last-hit(%d,%.1f) current-hit(%d,%.1f) to hit(%d,%.1f)",LastHit.hit_wire,LastHit.hit_peakT,CurrentHit.hit_wire,CurrentHit.hit_peakT,hit.hit_wire,hit.hit_peakT);
                SHOW4(DistanceToHit , AngleToHitFromStartHit , AngleToHitFromCurrentHit , AngleToHitFromLastHit );
                SHOW4( TrackAngle, fabs( AngleToHitFromStartHit - TrackAngle ) , fabs( AngleToHitFromLastHit - TrackAngle ) , fabs( AngleToHitFromStartHit - TrackAngle ) );
            }
            
            if (   ( DistanceToHit < DistanceToNextHit)
                && ( fabs( AngleToHitFromStartHit - TrackAngle ) < AngleThreshold )
                && (    ( fabs( AngleToHitFromLastHit - TrackAngle ) < 1.25*AngleThreshold )
                    ||  ( fabs( AngleToHitFromCurrentHit - TrackAngle ) < 1.25*AngleThreshold ) )
                ){
                DistanceToNextHit = DistanceToHit;
                NextHit = hit;
                if (debug>3) {Printf("candidate next hit at (%d,%.1f)",NextHit.hit_wire,NextHit.hit_peakT);}
            }
        }
    }
    return NextHit;
}


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
std::vector<float> myVertex::FindSlopeAndIntercept( int plane, PandoraNuTrack t ){
    Debug(3 ,Form("myVertex::FindSlopeAndIntercept(plane %d, track %d)",plane,t.track_id));
    float x1=0, x2=0 , y1=0 , y2=0;
    std::vector<float> a = { -1000  , -1000 }; // (a[0] = slope , a[1] = intercept)
    std::vector<float> x1y1x2y2 = GetX1Y1X2Y2forTrack ( plane , t);
    x1 = x1y1x2y2[0]; y1 = x1y1x2y2[1]; x2 = x1y1x2y2[2]; y2 = x1y1x2y2[3];
    
    a[0] = (x2 != x1) ? (float)((y2-y1))/(x2-x1) : 1000;
    a[1] = y2 - a[0]*x2;
    return a;
}



//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
std::vector<float> myVertex::GetX1Y1X2Y2forTrack( int plane , PandoraNuTrack t ){
    // this method is most relevant after fixing the tracks' directions (using the vertex position)
    float x1=0 , y1=0 , x2=0 , y2=0;
            if (t.track_id==AssignedMuonTrack.track_id) {
                x1 = mu_start_wire[plane];
                y1 = mu_start_time[plane];
                x2 = mu_end_wire[plane];
                y2 = mu_end_time[plane];
            }
            
            else if(t.track_id==AssignedProtonTrack.track_id){
                x1 = p_start_wire[plane];
                y1 = p_start_time[plane];
                x2 = p_end_wire[plane];
                y2 = p_end_time[plane];
            }
            else {
                x1 = t.roi[plane].start_wire;
                y1 = t.roi[plane].start_time;
                x2 = t.roi[plane].end_wire;
                y2 = t.roi[plane].end_time;
            }
    if (debug>3){
        Printf("GetX1Y1X2Y2forTrack( int plane=%d , PandoraNuTrack t=%d )",plane,t.track_id);
        SHOW4(x1,y1,x2,y2);
    }
    std::vector<float> x1y1x2y2 = {x1,y1,x2,y2};
    return x1y1x2y2;
}

////....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//int myVertex::ClosestTrackToHit( int plane , hit c_hit ){
//    // input:   hit
//    // return:  track-id of the closest (pandoraNu) track to the hit
//    // point-line distance calculation referenced from [http://mathworld.wolfram.com/Point-LineDistance2-Dimensional.html]
//    // distance from point to line = {(x2-x1)*(y1-y0) - (x1-x0)*(y2-y1)))}/sqrt( (x2-x1)^2 + (y2-y1)^2 )
//    
//    
//    int closest_track_id=-100;
//    float x0 = c_hit.hit_wire, y0 = c_hit.hit_peakT;
//    float x1=0 , y1=0 , x2=0 , y2=0 , DistanceToClosestTrack = 10000;
//    float TrackLength , DistanceToTrack;
//    
//    for (auto t: tracks) {
//        if ( c_hit.InBox( t.roi[plane].EnlargeBox( 5 , 20 ) ) ){
//            
//            std::vector<float> x1y1x2y2 = t.GetX1Y1X2Y2forTrack( plane );
//            x1 = x1y1x2y2[0]; y1 = x1y1x2y2[1]; x2 = x1y1x2y2[2]; y2 = x1y1x2y2[3];
//            
//            TrackLength = WireTimeDistance( x1 , y1 , x2 , y2 );
//            if (TrackLength==0) DistanceToTrack = 1000;
//            else DistanceToTrack = fabs( WireDistance(x1,x2) * TimeDistance(y0,y1) - WireDistance(x0,x1) * TimeDistance(y1,y2) ) / TrackLength;
//            //            else DistanceToTrack = fabs( (x2-x1)*(y1-y0) - (x1-x0)*(y2-y1) ) / ((x2-x1)*(x2-x1) - (y2-y1)*(y2-y1)) ;
//            Debug(5,Form("distance of hit (%d,%.1f) to track %d is %.2f mm",c_hit.hit_wire,c_hit.hit_peakT,t.track_id,DistanceToTrack));
//            
//            if (DistanceToTrack < DistanceToClosestTrack){
//                DistanceToClosestTrack = DistanceToTrack;
//                closest_track_id = t.track_id;
//            }
//            
//        }
//        
//    }
//    return closest_track_id;
//}



//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
hit myVertex::ClosestHitToTrackHorizontally( int plane, float peakTime, std::vector<hit> fPossibleHits, PandoraNuTrack track  ){
    // find the closest hit horizontally - i.e. same time, different wires
    Float_t slope = track.slope[plane] , intercept = track.intercept[plane];
    Debug(5,Form("looking for horizontally closest hit to track %d time=%.1fwire+(%.1f) (wire=(time-(%.1f))/%.1f) at time %.1f",track.track_id,slope,intercept,intercept,slope,peakTime));
    hit ClosestHit;
    float DistanceToClosestHit = 1000, WireAlongTrack;
    for (auto hit:fPossibleHits) {
        if ( fabs( hit.hit_peakT - peakTime ) < 1 ){
            
            if (fabs(slope) > 0.001) {
                WireAlongTrack = ( hit.hit_peakT - intercept ) / slope;
            }
            else {
                WireAlongTrack = (fabs(slope)/slope)*1000*( hit.hit_peakT - intercept ) ;
            }
            float HorizontalDistanceToTrack = fabs(hit.hit_wire - WireAlongTrack) ;
            Debug(5,Form("found at a hit in a good time: (%d,%.2f); wire along track = %.1f, Horizontal-distance=%.1f",hit.hit_wire,hit.hit_peakT,WireAlongTrack,HorizontalDistanceToTrack));
            if (HorizontalDistanceToTrack < DistanceToClosestHit){
                DistanceToClosestHit = HorizontalDistanceToTrack;
                ClosestHit = hit;
            }
        }
    }
    return ClosestHit;
}


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
hit myVertex::ClosestHitToTrackVertically( int plane, int wire, std::vector<hit> fPossibleHits, PandoraNuTrack track ){
    // find the closest hit vertically - i.e. same wire, different peak-times
    Float_t slope = track.slope[plane] , intercept = track.intercept[plane];
    
    Debug(5,Form("looking for vertically closest hit to track %d time=%.1fwire+(%.1f) ",track.track_id,slope,intercept));
    hit ClosestHit;
    float DistanceToClosestHit = 1000;
    for (auto hit:fPossibleHits) {
        if ( hit.hit_wire == wire ){
            float TimeAlongTrack = slope * hit.hit_wire + intercept;
            float VerticalDistanceToTrack = fabs(hit.hit_peakT - TimeAlongTrack);
            if (VerticalDistanceToTrack < DistanceToClosestHit){
                DistanceToClosestHit = VerticalDistanceToTrack;
                ClosestHit = hit;
            }
        }
    }
    return ClosestHit;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
hit myVertex::ClosestHitToTrack1d( int plane, int wire , float peakTime , std::vector<hit> fPossibleHits, PandoraNuTrack track ){
    // input: pandoraNu track
    // return: the closest hit in 1d
    Debug(5,Form("myVertex::ClosestHitToTrack1d( plane=%d, wire=%d , peakTime=%f , PandoraNuTrack %d , anlge = %.1f (PI/2=%.1f))",plane,wire,peakTime,track.track_id,r2d*track.WireTimeAngle[plane],r2d*PI/2));
    if ( fabs(track.WireTimeAngle[plane] - PI/2) < PI/4 || fabs(track.WireTimeAngle[plane] + PI/2) < PI/4){
        Debug(5,"track is vertical");
        // if the track is vertical - look for the closest horizontally
        return ClosestHitToTrackHorizontally( plane, peakTime , fPossibleHits, track );
    }
    else {
        Debug(5,"track is horizontal");
        // if the track is horizontal - look for the closest vertically
        return ClosestHitToTrackVertically( plane, wire , fPossibleHits, track );
    }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
std::vector<hit> myVertex::TrackMyTrack( int plane, hit StartHit , std::vector<hit> fPossibleHits , float TrackAngle , PandoraNuTrack track ){
    
    Debug(2,"myVertex::TrackMyTrack()");
    
    hit LastHit, CurrentHit = StartHit, NextHit;
    std::vector<hit> TrackHits, PossibleHits = fPossibleHits;
    
    // possible hits for tracking are ones that
    // (1) the track is the closest track to them
    // (2) they are the closest hits to the track in 1d (vertically if the track is horizontal / horizontally if the track is vertical)
    
    for (int i=0; i < 1000 ; i++ ) {
        
        TrackHits.push_back( CurrentHit );
        PossibleHits = RemoveHitFromVector( PossibleHits , CurrentHit );
        
        NextHit = FindNextHit( StartHit, LastHit, CurrentHit, PossibleHits, TrackAngle );
        if ( NextHit==hit() ) break; // this means I didn't find a next hit
        
        LastHit = CurrentHit;
        CurrentHit = NextHit;
        if (debug>3){ PrintLine(); printf( "next hit: %lu ", TrackHits.size() ); PrintHit(NextHit); }
        
    }
    return TrackHits;
}


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
std::vector<hit> myVertex::PossibleTracksForTracking( int plane, // std::vector<hit> fPossibleHits,
                                                      float TrackAngle , PandoraNuTrack track ){
    std::vector<hit> PossibleHits;
    for (auto hit:AllHitsInROI[plane]) {
        
        // we want to use hits that are either the closest vertically/horizontally,
        // or a very close (<1cm) the one that is, since sometimes a good track deposits charge in multi-hit/multi-wire pattern...
        auto ClosestHitToTrack_in1d = ClosestHitToTrack1d( plane , hit.hit_wire , hit.hit_peakT , AllHitsInROI[plane]  , track );
        auto DistanceClosestHitToTrack_in1d = HitHitDistance( hit , ClosestHitToTrack_in1d ); // distance in mm
        
        if( debug>5 ){
            PrintHit(hit);
            SHOW2( hit.ClosestTrack_track_id, track.track_id );
            ClosestHitToTrack_in1d.Print();
        }
        if (   ( hit.ClosestTrack_track_id == track.track_id )
            && ( DistanceClosestHitToTrack_in1d < 10 )
            ){
            PossibleHits.push_back( hit );
            Debug(5,"this is a good hit. appendng to PossibleHits");
        }
        Debug(5,"----------------------------------------");
    }
    return PossibleHits;
}





//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
bool myVertex::SetTracksParameters( int plane ){
    
    // find the equation that describes each track in this plane
    for (auto &t:tracks) {
        Debug(3,Form("find slope and intercept of track %d",t.track_id));
        std::vector<float> a = FindSlopeAndIntercept( plane, t );
        t.SetSlopeIntercept( plane, a[0] , a[1] );
        std::vector<float> x1y1x2y2 = GetX1Y1X2Y2forTrack ( plane , t );
        t.SetX1Y1X2Y2forTrack( plane, x1y1x2y2 );
        if (t.track_id==AssignedMuonTrack.track_id)      {AssignedMuonTrack.SetSlopeIntercept( plane, a[0] , a[1] ); AssignedMuonTrack.SetX1Y1X2Y2forTrack( plane, x1y1x2y2 );}
        if (t.track_id==AssignedProtonTrack.track_id)    {AssignedProtonTrack.SetSlopeIntercept( plane, a[0] , a[1] ); AssignedProtonTrack.SetX1Y1X2Y2forTrack( plane, x1y1x2y2 );}
    }
    return true;
}



//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
MyTrack myVertex::FindMyTrack( int plane , PandoraNuTrack pandoraNu_track , float track_WireTimeAngle ) { //, std::vector<hit> fPossibleHits ){

    Debug(3,Form("%d/%d/%d tracking for muon track (%d, pidA=%.1f) in plane %d (angle=%.1f)",
                 run,subrun,event,pandoraNu_track.track_id,pandoraNu_track.pidpida,plane,r2d*track_WireTimeAngle));
    
    // my-tracking for candidate muon/proton
    MyTrack my_track = MyTrack( plane , pandoraNu_track );

    // (A) find the start hit
    std::vector<hit> PossibleStartHits , PossibleHits;
    PossibleStartHits = PossibleHits = PossibleTracksForTracking( plane , track_WireTimeAngle , pandoraNu_track );
    my_track.SetPossibleHits( PossibleHits );
    FoundClosestHitToVertex = FindClosestHitToVertex( plane , PossibleStartHits );
    
    if( debug > 5) {
        Printf("possible start hit for track %d plane %d:",pandoraNu_track.track_id , plane);
        for (auto hit:PossibleStartHits) PrintHit(hit);
        Printf("Closest hit to vertex: ");
        PrintHit( ClosestHitToVertex[plane] );
        PrintLine();
    }
    
    if (FoundClosestHitToVertex){
        
        // (B) make sure there are close hits along track direction
        // and if not, change start-point
        while ( FoundCloseHitAlongTrackDirection( ClosestHitToVertex[plane] , PossibleStartHits , track_WireTimeAngle ) == false ){
            PossibleStartHits = RemoveHitFromVector( PossibleStartHits , ClosestHitToVertex[plane] );
            FindClosestHitToVertex( plane , PossibleStartHits );
            if (PossibleStartHits.size()<1 || FoundClosestHitToVertex==false) break;
            if(debug>2){
                printf("hopping to a new start-point (out of %lu): ",PossibleStartHits.size());
                PrintHit( ClosestHitToVertex[plane] );
            }
        }
        if ( ClosestHitToVertex[plane].exist() ){
            // this means there is no hit that was found to be close to the vertex & with a good angle
            // (C) assign hits to track
            // PossibleHits = RemoveHitFromVector( PossibleHits , ClosestHitToVertex[plane] );
            my_track.SetHits ( TrackMyTrack( plane, ClosestHitToVertex[plane] , PossibleHits , track_WireTimeAngle , pandoraNu_track ) );
        }
    }
    Debug(3,Form("%d/%d/%d finished my-tracking for track %d",run,subrun,event,pandoraNu_track.track_id));
    return my_track;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
bool myVertex::AssociateHitsToTracks( int plane ){//, std::vector<hit> fPossibleHits ){ // input fPossibleHits vector is only hits in this plane
    
    Debug(2,Form("r %d/s %d/e %d myVertex::AssociateHitsToTracks in plane %d",run,subrun,event,plane));
    
    
    bool DoMuonTracking = true , DoProtonTracking = true;

    if (DoMuonTracking){
        MyTrackMuonTrack[plane] = FindMyTrack( plane , AssignedMuonTrack , mu_angle[plane] );
    }
    
    if (DoProtonTracking){
        MyTrackProtonTrack[plane] = FindMyTrack( plane , AssignedProtonTrack , p_angle[plane] );
    }
    
    // remove double counting of hits
    
    
    switch (plane) {
        case 0:
            MyTrackMuon_u = MyTrackMuonTrack[plane];
            MyTrackProton_u = MyTrackProtonTrack[plane];
            break;
        case 1:
            MyTrackMuon_v = MyTrackMuonTrack[plane];
            MyTrackProton_v = MyTrackProtonTrack[plane];
            break;
        case 2:
        default:
            MyTrackMuon_y = MyTrackMuonTrack[plane];
            MyTrackProton_y = MyTrackProtonTrack[plane];
            break;
    }
    
    Debug(3,"finished myVertex::AssociateHitsToTracks\n------------------------------------");
    return true;
}


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void myVertex::CollectAllHitsInROI( int plane, std::vector<hit> hits_in_this_plane ){
    
    std::vector<hit> hits_in_vertex_roi;
    float total_charge_in_roi = 0;
    if (debug>5) {
        Printf("myVertex::CollectAllHitsInROI( int plane, std::vector<hit> hits_in_this_plane )");
        for (auto hit: hits_in_this_plane) hit.Print();
    }
    
    for (auto hit: hits_in_this_plane){
        
        if ( hit.InBox( roi[plane] ) ){
            hits_in_vertex_roi.push_back( hit );
            total_charge_in_roi += hit.hit_charge;
        }
    }
    
    SetAllHitsInROI( plane , hits_in_vertex_roi );
    AllChargeInVertexROI[plane] = total_charge_in_roi;
    
    Debug(4,Form("total charge in roi: %.1f [ADC]",total_charge_in_roi));
}


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void myVertex::SetAllHitsInROI (int plane , std::vector<hit> fhits){

    switch (plane) {
        case 0:
            for (auto hit:fhits) {AllHitsInROI[plane].push_back(hit); AllHitsInROI_u.push_back(hit);}
            break;
        case 1:
            for (auto hit:fhits) {AllHitsInROI[plane].push_back(hit); AllHitsInROI_v.push_back(hit);}
            break;
        case 2:
        default:
            for (auto hit:fhits) {AllHitsInROI[plane].push_back(hit); AllHitsInROI_y.push_back(hit);}
            break;
    }
}


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void myVertex::SetAllHitsInPlane (int plane , std::vector<hit> fhits){
    switch (plane) {
        case 0:
            for (auto hit:fhits) HitsInPlane_u.push_back(hit);
            break;
        case 1:
            for (auto hit:fhits) HitsInPlane_v.push_back(hit);
            break;
        case 2:
        default:
            for (auto hit:fhits) HitsInPlane_y.push_back(hit);
            break;
    }
}



//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
bool myVertex::CollectAssociatedCharge(int plane){
    float MyMuonCharge = MyTrackMuonTrack[plane].GetTotalChargeInHits();
    float MyProtonCharge = MyTrackProtonTrack[plane].GetTotalChargeInHits();
    TracksAssociatedCharge[plane] = MyMuonCharge + MyProtonCharge;
    if (AllChargeInVertexROI[plane]){
        ratio_associated_hit_charge_to_total[plane] = TracksAssociatedCharge[plane]/AllChargeInVertexROI[plane];
    }
    else{
        ratio_associated_hit_charge_to_total[plane] = 1.;
    }
    average_ratio_associated_hit_charge_to_total += ratio_associated_hit_charge_to_total[plane] / 3.;
    if ( ratio_associated_hit_charge_to_total[plane] > max_ratio_associated_hit_charge_to_total )  max_ratio_associated_hit_charge_to_total = ratio_associated_hit_charge_to_total[plane];
    return true;
}


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void myVertex::Print(){
    
    cout << "\033[35m" << "~~~~~~~~~~~~~~\n vertex " << vertex_id << "\n~~~~~~~~~~~~~~"<< "\033[0m" << endl;
    SHOW3( run , subrun , event );
    SHOWTVector3( position );
    for (auto t: tracks) {
        Printf("track %d (pdg %d), vertex distance %.1f cm",t.track_id,t.MCpdgCode,t.DistanceFromPoint( position ));
    }
    
    // tracks
    if (!tracks.empty()){
        cout << "\033[33m" << tracks.size() << " tracks in vertex " << "\033[30m" << endl;
        for (auto t: tracks) {
            if (t.track_id!=-100)   t.Print( true , true, true );
            else                    Printf("unidentified/non-reconstructed track. continuing...");
        }

        // inter-tracks distances
        cout << "\033[33m" << tracks.size()*tracks.size() << " inter-tracks distances " << "\033[30m" << endl;
        for(auto vec : tracks_distances) {
            for(auto x : vec)
            std::cout << setprecision(2) << x << "\t";
            std::cout << std::endl;
        }
    }
    else {
        cout << "\033[33m" << "no reconstructed tracks in vertex " << "\033[30m" << endl;
    }
    //    cout << "topology: " << TopologyString << endl;
    cout << "truth topology: " << TruthTopologyString << endl;
    SHOW(GENIECC1p);
    if (GENIECC1p){
        cout << "This vertex is a GENIE true CC1p" << endl;
        SHOW3( muonTrackReconstructed, protonTrackReconstructed, IsVertexReconstructed );
        
        // muon
        SHOWTLorentzVector( genie_interaction.muon );
        if (!tracks.empty()) SHOWTLorentzVector( reco_CC1p_Pmu );
        
        // proton
        if (genie_interaction.protons.size()>0) SHOWTLorentzVector( genie_interaction.protons[0] );
        if (!tracks.empty()) SHOWTLorentzVector( reco_CC1p_Pp );
        
        // theta (p,q)
        SHOW(genie_interaction.theta_pq);
        if (!tracks.empty()) SHOW(reco_CC1p_theta_pq);
    }

    if (!tracks.empty()) for (int plane = 0 ; plane<3 ; plane ++ ) PrintBox(roi[plane]);
    SHOW3(AllHitsInROI_u.size(),AllHitsInROI_v.size(),AllHitsInROI_y.size());
    
}


#endif
