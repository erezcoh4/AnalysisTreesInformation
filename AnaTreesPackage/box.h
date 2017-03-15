/**
 * \file box.h
 *
 * \ingroup AnaTreesPackage
 * 
 * \brief Class def header for a class box
 *
 * @author erezcohen
 */

/** \addtogroup AnaTreesPackage

    @{*/
#ifndef BOX_H
#define BOX_H

#include <iostream>
#include "LArUtil/../../UserDev/mySoftware/MySoftwarePackage/myIncludes.h"

#define PrintBox(box) cout << "\033[34m" << #box << ": (" << box.start_wire << "," << box.start_time << ") => (" << box.end_wire << "," << box.end_time << ")" << "\033[0m" << endl;

/**
   \class box
   User defined class box ... these comments are used to generate
   doxygen documentation!
 */
class box: public myIncludes{

public:

  /// Default constructor
    
    
    int     start_wire , start_time , end_wire , end_time;
    float   diag_slope;
    
    box() { start_wire = start_time = end_wire = end_time = 0; }
    
    box(int fstart_wire, int fstart_time, int fend_wire, int fend_time)
    : start_wire(fstart_wire), start_time(fstart_time), end_wire(fend_wire), end_time(fend_time)
    {
        // order the box start and end positions
        if (start_wire > end_wire){
            
            int tmp_wire= end_wire;
            end_wire   = start_wire;
            start_wire = tmp_wire;
            
        }
        if (start_time > end_time){
            
            int tmp_time = end_time;
            end_time    = start_time;
            start_time  = tmp_time;
            
        }
        diag_slope = (float)( end_time - start_time ) / ( end_wire - start_wire );
    }
    
    box EnlargeBox(int dw = 10 , int dt = 50){
        return box( start_wire-dw , start_time-dt , end_wire+dw , end_time+dt );
    }
    
    
    
    

  /// Default destructor
  ~box(){}

};

#endif
/** @} */ // end of doxygen group

