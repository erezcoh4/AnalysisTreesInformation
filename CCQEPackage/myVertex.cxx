#ifndef MYVERTEX_CXX
#define MYVERTEX_CXX

#include "myVertex.h"
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
myVertex::myVertex(Int_t fID){
    SetVertexID(fID);
    AddTrackID(fID);
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
    for (auto i:sort_l( tracks )){
        tracks_lengthsorted.push_back( tracks.at(i) );
    }
    ShortestTrack = tracks_lengthsorted.at( tracks.size() - 1 );
    LongestTrack = tracks_lengthsorted.at( 0 );

    //    cout << "shortest track:" << endl;
    //    ShortestTrack.Print();
    //    cout << "longest track:" << endl;
    //    LongestTrack.Print();
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
void myVertex::SetDeltaPhiLongestandShortestTracks(){
    delta_phi_LongestShortestTracks = r2d*fabs(ShortestTrack.phi - LongestTrack.phi);
}





//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
bool myVertex::SetTruthTopology(){
    // I definde a truth - CC1p vertex by the following:
    // (1) two tracks, and only two tracks
    //      - one is a muon (13)
    //      - the other is a proton (2212)
    // (2) they both come from a truth cc interaction (ccnc=0)
    // (3) the truth start positions of the muon and the proton is as small as the ideal vertex resolution


    TruthCC1p = false;
    TruthTopologyString = "truly not a CC1p";
    if ( tracks.size()!=2 || tracks[0].truth_ccnc!=0 || tracks[1].truth_ccnc!=0) return false;

    if (tracks[0].MCpdgCode==2212 && tracks[1].MCpdgCode==13){
        protonTruth = tracks[0];
        muonTruth = tracks[1];
    }
    else if (tracks[0].MCpdgCode==13 && tracks[1].MCpdgCode==2212){
        muonTruth = tracks[0];
        protonTruth = tracks[1];
    }
    else {
        return false;
    }
    
    if ( (protonTruth.truth_start_pos - muonTruth.truth_start_pos).Mag() < 2 )
    {
        TruthCC1p = true;
        TruthTopologyString = "Truth CC1p";
        return true;
    }
    return false;
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
        && (delta_phi_min < delta_phi_LongestShortestTracks && delta_phi_LongestShortestTracks < delta_phi_max)
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
void myVertex::SetTracksRelations(){
    for(int i = 0; i < Ntracks ; i++){
        
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
void myVertex::Print(){
    SHOW( vertex_id );
    // tracks
    if (!tracks.empty()){
        cout << "\033[33m" << tracks.size() << " tracks in vertex " << "\033[37m" << endl;
        for (auto t: tracks) t.Print();

        // inter-tracks distances
        cout << "\033[33m" << tracks.size()*tracks.size() << " inter-tracks distances " << "\033[37m" << endl;
        cout << "tracks_distances size : " << tracks_distances.size() << endl;
        for(auto vec : tracks_distances) {
            for(auto x : vec)
            std::cout << setprecision(2) << x << "\t";
            std::cout << std::endl;
        }
    }


    cout << "topology: " << TopologyString << endl;
    cout << "truth topology: " << TruthTopologyString << endl;
    
}


#endif
