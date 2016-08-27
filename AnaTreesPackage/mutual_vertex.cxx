#ifndef MUTUAL_VERTEX_CXX
#define MUTUAL_VERTEX_CXX

#include "mutual_vertex.h"


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
mutual_vertex::mutual_vertex( Int_t fvertex_id , track_vertex t_vertex ){
    
    N_tracks_vertices = 1;
    SetVertexID( fvertex_id );
    tracks_vertices.push_back(t_vertex);
    tracks_id.push_back(t_vertex.track_id);
    SetMutualVertexInfo();
    
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
bool mutual_vertex::include_track ( Int_t ftrack_id ){
    Printf("looking for track %d in",ftrack_id);
    SHOWstdVector(tracks_id);
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
    tracks_vertices.push_back(t_vertex);
    tracks_id.push_back(t_vertex.track_id);
    SetMutualVertexInfo();
    
}



//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void mutual_vertex::SetMutualVertexInfo(){
    
    Np = Nmu = Npi = Ne = Ngamma = 0;
    
    
    // average position over all tracks vertices that contribute to this mutual vertex
    position = TVector3();
    
    // for ROIs, the maximal distance form the mutual vertex would be the maximal x/y/z position of the longest track associated with this vertex
    dxmax = dymax = dzmax = 0;
    
    for (auto c_track_vertex: tracks_vertices) {
        
        position += c_track_vertex.position;
        
        if (fabs(c_track_vertex.position_of_same_track_other_vertex.x() - position.x()) > dxmax) {
            dxmax = c_track_vertex.position_of_same_track_other_vertex.x() - position.x();
        }
        if (fabs(c_track_vertex.position_of_same_track_other_vertex.y() - position.y()) > dymax) {
            dymax = c_track_vertex.position_of_same_track_other_vertex.y() - position.y();
        }
        if (fabs(c_track_vertex.position_of_same_track_other_vertex.z() - position.z()) > dzmax) {
            dzmax = c_track_vertex.position_of_same_track_other_vertex.z() - position.z();
        }
        
        
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
    
    // load GeometryHelper utility
    auto geomHelper = ::larutil::GeometryHelper::GetME();
    
    double start_xyz[3] = { position.x() - dxmax , position.y() - dymax , position.z() - dzmax };
    double end_xyz[3]   = { position.x() + dxmax , position.y() + dymax , position.z() + dzmax };
    
    // shift in time-axis due to the truncation of the waveforms
    // (the first 2400 ADCs are removed from the waveform, The extra couple ticks could be due to a shift in the signal deconvolution)
    double time_shift =  802;
    
    
    for (int plane = 0; plane < 3; plane++){
        
        // geoHelper is a set of utility functions that help with geometric stuff..
        auto const& start_projection2D = geomHelper->Point_3Dto2D(start_xyz, plane);
        auto const& end_projection2D = geomHelper->Point_3Dto2D(end_xyz, plane);
        
        int start_wire = (int) ( start_projection2D.w / geomHelper->WireToCm() );
        int start_time = (int) ( start_projection2D.t / geomHelper->TimeToCm() ) + time_shift;
        
        // 802: shift in time-axis due to the truncation of the waveforms
        // (the first 2400 ADCs are removed from the waveform, The extra couple ticks could be due to a shift in the signal deconvolution)
        int end_wire = (int) ( end_projection2D.w / geomHelper->WireToCm() );
        int end_time = (int) ( end_projection2D.t / geomHelper->TimeToCm() ) + time_shift;
        roi[plane] = box( start_wire , start_time , end_wire , end_time );
        
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
