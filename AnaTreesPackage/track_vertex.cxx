#ifndef TRACK_VERTEX_CXX
#define TRACK_VERTEX_CXX

#include "track_vertex.h"



//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
track_vertex::track_vertex(Int_t fvertex_id , Int_t ftrack_id , TString flocation_in_track , TVector3 fposition ){
    
    SetVertexID         (fvertex_id);
    SetTrackID          (ftrack_id);
    SetLocationInTrack  (flocation_in_track);
    SetPosition         (fposition);

}





//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void track_vertex::SetDistancesFromVertices ( std::vector<track_vertex> all_vertices_in_this_event ){
    // compute the distacne of THIS track_vertex from all other track vertices in the same event
    
    if (!distance_from_vertex.empty()) distance_from_vertex.clear();
    
    for (auto vertex : all_vertices_in_this_event) {
        
        if ( this->vertex_id == vertex.vertex_id ){
            distance_from_vertex.push_back ( 9999 ); // its the same vertex. plugging such a default value saves computation time in other routines
        }
        else {
            distance_from_vertex.push_back ( (this->position - other_vertex.position ).Mag() );
        }
        
        
    }
    
}




//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void track_vertex::Print(){
    
    SHOW3( vertex_id , track_id , location_in_track );
    SHOWTVector3( position );
    SHOWstdVector(distance_from_vertex);
    for (int plane = 0 ; plane < 3; plane++) {
        PrintPhys( track_CalorimetPDG[plane] , Form(" for plane %d",plane) );
    }
    PrintLine();
}



#endif
