/**
 * \file mutual_vertex.h
 *
 * \ingroup AnaTreesPackage
 * 
 * \brief Class def header for a class mutual_vertex
 *
 * @author erezcohen
 */

/** \addtogroup AnaTreesPackage

    @{*/
#ifndef MUTUAL_VERTEX_H
#define MUTUAL_VERTEX_H

#include <iostream>
#include <track_vertex.h>
#include "LArUtil/GeometryHelper.h"

#define min_distance_from_vertex 5 // in [cm] - this is the minimal distance between vertices that allows them to be defined as mutual vertices

/**
   \class mutual_vertex
   User defined class mutual_vertex ... these comments are used to generate
   doxygen documentation!
 */
class mutual_vertex : public track_vertex {

public:

    mutual_vertex (){}
    ~mutual_vertex (){}
    mutual_vertex ( Int_t f_vertex_id ){ SetVertexID(f_vertex_id); }
    mutual_vertex ( Int_t , track_vertex );

    void          SetPosition ();
    bool        include_track ( Int_t ftrack_id );
    void       AddTrackVertex ( track_vertex );
    void  SetMutualVertexInfo ();
    void    SetVertexTopology ();
    void           CreateROIs ();
    void                Print ();

    
    
    TVector3    position;
    
    Int_t       N_tracks_vertices;
    Int_t       Np      , Nmu   , Npi   , Ne    , Ngamma;
    Float_t     dxmax   , dymax , dzmax;
    
    
    TString     vertex_topology;
    
    std::vector<Int_t>          tracks_id;
    std::vector<track_vertex>   tracks_vertices;
    
    
    box         roi[3];
};

#endif
/** @} */ // end of doxygen group 

