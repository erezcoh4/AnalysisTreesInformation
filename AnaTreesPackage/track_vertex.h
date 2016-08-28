/**
 * \file track_vertex.h
 *
 * \ingroup AnaTreesPackage
 * 
 * \brief Class def header for a class track_vertex
 *
 * @author erezcohen
 */

/** \addtogroup AnaTreesPackage

    @{*/
#ifndef TRACK_VERTEX_H
#define TRACK_VERTEX_H

#include <iostream>
#include "../../mySoftware/MySoftwarePackage/myIncludes.h"
#include "PandoraNuTrack.h"

/**
   \class track_vertex
   User defined class track_vertex ... these comments are used to generate
   doxygen documentation!
 */
class track_vertex{

public:

    track_vertex (){}
    ~track_vertex(){}
    track_vertex (Int_t fvertex_id , Int_t ftrack_id , TString flocation_in_track , TVector3 fposition , Int_t fCalorimetryPDG[3] , box ftrack_ROIs[3] );

    void        SetVertexID (Int_t fvertex_id)              {vertex_id = fvertex_id;};
    void         SetTrackID (Int_t ftrack_id)               {track_id = ftrack_id;};
    void SetLocationInTrack (TString flocation_in_track)    {location_in_track = flocation_in_track;};
    void        SetPosition (TVector3 fposition)            {position = fposition;};
    
    void  SetCalorimetryPDG (Int_t fCalorimetryPDG[3]);
    void            SetROIs (box ftrack_ROIs[3]);
    
    
    void SetDistancesFromVertices ( std::vector<track_vertex> other_vertices );
    void                    Print ();
    
    Int_t       vertex_id     ,  track_id;
    Int_t       track_CalorimetryPDG[3] , CalorimetryID;
    TString     location_in_track ;
    
    
    TVector3    position    , position_of_same_track_other_vertex;
    
    box                     track_ROIs[3];
    // CONTINUE HERE: MAKE COMPUTEANATREE PLUG TRACK_ROIS INTO THE VERTEX , TO FEED MUTUAL VERTEX
    std::vector<Float_t>    distance_from_vertex;   // distance from all other track_vertices in this event
};

#endif
/** @} */ // end of doxygen group 

