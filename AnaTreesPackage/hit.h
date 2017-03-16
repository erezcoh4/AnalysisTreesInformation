/**
 * \file hit.h
 *
 * \ingroup AnaTreesPackage
 * 
 * \brief Class def header for a class hit
 *
 * @author erezcohen
 */

/** \addtogroup AnaTreesPackage

    @{*/
#ifndef HIT_H
#define HIT_H

#include <iostream>
#include "LArUtil/../../UserDev/mySoftware/MySoftwarePackage/myIncludes.h"
#include "box.h"

#define PrintHit(hit) cout << "\033[34m" << #hit << ": plane " << hit.hit_plane << ", wire " << hit.hit_wire << ", peak-time " << hit.hit_peakT << ", charge " << hit.hit_charge << "\033[0m" << endl;
/**
   \class hit
   User defined class hit ... these comments are used to generate
   doxygen documentation!
 */
class hit: public myIncludes{

public:

    /// Default constructor

    /// Default destructor
    ~hit(){}

    
    Short_t hit_plane, hit_wire;
    Float_t hit_peakT, hit_charge;
    
    int ClosestTrack_track_id;
    int hit_id;
    
    hit() { hit_plane = hit_wire = hit_peakT = hit_charge = hit_id = -100; }
    
    hit(Short_t fplane, Short_t fwire, Float_t fpeakT, Float_t fcharge, int fid=-100)
    : hit_plane(fplane), hit_wire(fwire), hit_peakT(fpeakT), hit_charge(fcharge) , hit_id(fid)
    {}
    
    
    
    inline bool operator==(const hit & h) {
        return std::tie( hit_plane, hit_wire , hit_peakT , hit_id) == std::tie(h.hit_plane , h.hit_wire, h.hit_peakT , hit_id);
    }
    inline bool InBox(box b){
        return (b.start_wire < hit_wire && hit_wire < b.end_wire && b.start_time < hit_peakT && hit_peakT < b.end_time);
    }
    bool exist(){
        if (hit_id==hit_plane==hit_wire==(int)hit_peakT) return false;
        return true;
    }

    void Print(){
        cout << "hit " << hit_id << ", " << hit_plane << "/" << hit_wire << "/" << hit_peakT << endl;
    }
    
    
    

};

#endif
/** @} */ // end of doxygen group 

