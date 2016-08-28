#ifndef MUTUAL_VERTEX_CXX
#define MUTUAL_VERTEX_CXX

#include "mutual_vertex.h"


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
mutual_vertex::mutual_vertex( Int_t fvertex_id , track_vertex t_vertex ){
    
    N_tracks_vertices = 1;
    SetVertexID( fvertex_id );
    tracks_vertices.push_back( t_vertex );
    tracks_id.push_back( t_vertex.track_id );
    for (int plane = 0 ; plane < 3 ; plane++) {
        tracks_ROIs[plane].push_back( t_vertex.track_ROIs[plane] );
    }
    SetMutualVertexInfo();
    
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
bool mutual_vertex::include_track ( Int_t ftrack_id ){
    for (auto t_id: tracks_id) {
        if (t_id == ftrack_id) {
            return true;
        }
    }
    return false;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void mutual_vertex::AddTrackVertex( track_vertex t_vertex ){
    
    N_tracks_vertices++;
    tracks_vertices.push_back( t_vertex );
    tracks_id.push_back( t_vertex.track_id );
    for (int plane = 0 ; plane < 3 ; plane++) {
        tracks_ROIs[plane].push_back( t_vertex.track_ROIs[plane] );
    }

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
    if (Np > 0)     vertex_topology += Form(" %d protons",Np);
    if (Npi > 0)    vertex_topology += Form(" %d pions",Npi);
    if (Ne > 0)     vertex_topology += Form(" %d electrons",Ne);
    if (Ngamma > 0) vertex_topology += Form(" %d photons",Ngamma);
}


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void mutual_vertex::CreateROIs(){

    auto geomHelper = ::larutil::GeometryHelper::GetME();
    double time_shift =  802;
    double xyz[3] = {position.x() , position.y() , position.z() };
    for (int plane = 0; plane < 3; plane++){
        
        // create an initial roi around the vertex
        auto const& projection2D = geomHelper->Point_3Dto2D( xyz, plane );
        int wire = (int) ( projection2D.w / geomHelper->WireToCm() );
        int time = (int) ( projection2D.t / geomHelper->TimeToCm() ) + time_shift;
        roi[plane] = box( wire-10 , time-10 , wire+10 , time+10 );
        
        // increase the roi to fit the roi of the longest track (in the same plane)
        for (auto t_rois: tracks_ROIs[plane]) {
            
            if ( t_rois.start_wire < roi[plane].start_wire ){
                roi[plane].start_wire = t_rois.start_wire;
            }
            if ( roi[plane].end_wire < t_rois.end_wire ){
                roi[plane].end_wire = t_rois.end_wire;
            }
            
            if ( t_rois.start_time < roi[plane].start_time ){
                roi[plane].start_time = t_rois.start_time;
            }
            if ( roi[plane].end_time < t_rois.end_time ){
                roi[plane].end_time = t_rois.end_time;
            }
            
        }

    }
    
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void mutual_vertex::Print(){
    
    SHOW( vertex_id );
    SHOWTVector3( position );
    
    //    Printf("track vertices:") for (auto c_track_vertex: tracks_vertices) c_track_vertex.Print();
    SHOWstdVector(tracks_id);
    SHOW3( Np  , Nmu   , Npi);
    if (Ne>0 || Ngamma>0) {
        SHOW ( Ne );
        SHOW( Ngamma );
    }
    SHOW(vertex_topology);
    for (int plane = 0 ; plane < 3; plane++) {
        PrintBox(roi[plane]);
    }

    PrintLine();
}


#endif
