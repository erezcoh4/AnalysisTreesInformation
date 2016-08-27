#ifndef TRACK_VERTEX_CXX
#define TRACK_VERTEX_CXX

#include "track_vertex.h"



//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
track_vertex::track_vertex(Int_t fvertex_id , Int_t ftrack_id , TString flocation_in_track , TVector3 fposition , Int_t fCalorimetryPDG[3] ){
    
    SetVertexID         (fvertex_id);
    SetTrackID          (ftrack_id);
    SetLocationInTrack  (flocation_in_track);
    SetPosition         (fposition);
    SetCalorimetryPDG   (fCalorimetryPDG);
}


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void track_vertex::SetDistancesFromVertices ( std::vector<track_vertex> all_vertices_in_this_event ){
    // compute the distacne of THIS track_vertex from all other track vertices in the same event
    
    if (!distance_from_vertex.empty()) distance_from_vertex.clear();
    
    for (auto vertex : all_vertices_in_this_event) {
        
        if ( vertex_id == vertex.vertex_id ){
            distance_from_vertex.push_back ( 99999 ); // its the same vertex. plugging such a default value saves computation time in other routines
        }
        else {
            distance_from_vertex.push_back ( (position - vertex.position ).Mag() );
        }
        
        
    }
    
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void track_vertex::SetCalorimetryPDG (Int_t fCalorimetryPDG[3]){
    
    
    for (int i = 0 ; i < 3 ; i++ ) {
        track_CalorimetryPDG[i] = fCalorimetryPDG[i];
    }
    
    
    // set the 'pdg' code of the track if at least two planes gave the same calorimetry PDG id
    if ( (track_CalorimetryPDG[0]==track_CalorimetryPDG[1]) ||  (track_CalorimetryPDG[0]==track_CalorimetryPDG[2]) ) {
        CalorimetryID = track_CalorimetryPDG[0];
    }
    else if  (track_CalorimetryPDG[1]==track_CalorimetryPDG[2]) {
        CalorimetryID = track_CalorimetryPDG[1];
    }
    else {
        CalorimetryID = -9999;
    }
    
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void track_vertex::Print(){
    
    SHOW3( vertex_id , track_id , location_in_track );
    SHOWTVector3( position );
    SHOWstdVector(distance_from_vertex);
    for (int plane = 0 ; plane < 3; plane++) {
        PrintPhys( track_CalorimetryPDG[plane] , Form(" for plane %d",plane) );
    }
    SHOW(CalorimetryID);
    PrintLine();
}



#endif
