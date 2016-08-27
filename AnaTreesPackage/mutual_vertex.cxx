#ifndef MUTUAL_VERTEX_CXX
#define MUTUAL_VERTEX_CXX

#include "mutual_vertex.h"


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
mutual_vertex::mutual_vertex( Int_t vertex_id , track_vertex t_vertex ){
    
    N_tracks_vertices = 1;
    
    tracks_vertices.push_back(t_vertex);
    
    SetMutualVertexInfo();
    
}


bool mutual_vertex::include_track_vertex ( track_vertex t_v ){
    
    for (auto c_track_vertex: tracks_vertices) {
        if (c_track_vertex.vertex_id == t_v.vertex_id) return true;
    }
    
    return false;
    
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void mutual_vertex::AddTrackVertex( track_vertex t_vertex ){
    
    N_tracks_vertices++;
    
    tracks_vertices.push_back(t_vertex);
    
    SetMutualVertexInfo();
    
}



//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void mutual_vertex::SetMutualVertexInfo(){
    
    Np = Nmu = Npi = Ne = Ngamma = 0;
    
    // average position over all tracks vertices that contribute to this mutual vertex
    position = TVector3();
    
    for (auto c_track_vertex: tracks_vertices) {
        
        position += c_track_vertex.position;
        
        switch (c_track_vertex.CalorimetryID) {
                
            case 2212: // p
                Np++;
                break;
                
            case -13: // µ+
            case 13:  // µ-
                Nmu++;
                break;
                
            case 111: // π0
            case -211:// π-
            case 211: // π+
                Npi++;
                break;
                
            case -11: // e+
            case 11:  // e-
                Ne++;
                break;
                
            case 22:  //𝞬
                Ngamma++;
                break;
                
            default:
                break;
        }
    }
    position = (1./N_tracks_vertices) * position;
    
}



//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void mutual_vertex::SetVertexTopology(){
    
    // try to use the most basic analysis-trees infromation we've collected to determine event topology
    vertex_topology = "";
    vertex_topology += Form("%d muons",Nmu);
    vertex_topology += Form(" %d protons",Np);
    vertex_topology += Form(" %d pions",Npi);
    if (Ne>0 || Ngamma>0) {
        vertex_topology += Form(" %d electrons",Ne);
        vertex_topology += Form(" %d photons",Ngamma);
    }
}



//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void mutual_vertex::Print(){
    
    SHOW( vertex_id );
    SHOWTVector3( position );
    
    for (auto c_track_vertex: tracks_vertices) {
        c_track_vertex.Print();
    }
    SHOW3( Np  , Nmu   , Npi);
    if (Ne>0 || Ngamma>0) {
        SHOW ( Ne );
        SHOW( Ngamma );
    }
    SHOW(vertex_topology);
    PrintLine();
}


#endif
