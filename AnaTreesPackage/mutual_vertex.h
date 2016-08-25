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
#define min_distance_from_vertex 5 // in [cm] - this is the minimal distance between vertices that allows them to be defined as mutual vertices 

/**
   \class mutual_vertex
   User defined class mutual_vertex ... these comments are used to generate
   doxygen documentation!
 */
class mutual_vertex : public track_vertex{

public:

  mutual_vertex(){}
  ~mutual_vertex(){}
    mutual_vertex( track_vertex , track_vertex );

};

#endif
/** @} */ // end of doxygen group 

