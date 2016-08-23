/**
 * \file PandoraNuTrack.h
 *
 * \ingroup GBDTprotonsPackage
 *
 * \brief Class def header for a class PandoraNuTrack
 *
 * @author erezcohen
 */

/** \addtogroup GBDTprotonsPackage
 
 @{*/
#ifndef PANDORANUTRACK_H
#define PANDORANUTRACK_H

#include "LArUtil/Geometry.h"
#include "../../mySoftware/MySoftwarePackage/myIncludes.h"

/**
 \class PandoraNuTrack
 User defined class PandoraNuTrack ... these comments are used to generate
 doxygen documentation!
 */
using namespace std;

#define PrintBox(box) cout << "\033[34m" << #box << ": (" << box.start_wire << "," << box.start_time << ") => (" << box.end_wire << "," << box.end_time << ")" << "\033[0m" << endl;
#define TracsMinDistance 10 // [cm]

struct box {
    
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
    
};








class PandoraNuTrack{
    
public:
    
    /// Default constructor
    PandoraNuTrack();
    ~PandoraNuTrack();
    
    PandoraNuTrack (Int_t , Int_t , Int_t , Short_t , TVector3, TVector3, Float_t, Float_t, Float_t);
    
    
    void Print();
    
    
    void        FlipTrack (int debug=0);
    void       CreateROIs ();
    void      Calorimetry ();
    void     Straightness ();
    void         Momentum ();
    
    
    
    // setters
    void             SetRun (Int_t _run)    {run = _run;};
    void          SetSubrun (Int_t _subrun) {subrun = _subrun;};
    void           SetEvent (Int_t _event)  {event = _event;};
    void         SetTrackID (Short_t _id)   {track_id = _id;};
    void        SetStartPos (TVector3 pos)  {start_pos = pos;};
    void          SetEndPos (TVector3 pos)  {end_pos = pos;};
    void          SetLength (Float_t l)     {length = l;};
    void           SetTheta (Float_t t)     {theta = t;};
    void             SetPhi (Float_t ph)    {phi = ph;};
    void     Set_start_dqdx (Float_t dqdx)  {start_dqdx = dqdx;};
    void       Set_end_dqdx (Float_t dqdx)  {end_dqdx = dqdx;};
    void       Set_tot_dqdx (Float_t dqdx)  {tot_dqdx = dqdx;};
    void       Set_avg_dqdx (Float_t dqdx)  {avg_dqdx = dqdx;};
    void          Set_nhits (Int_t n)       {nhits = n;};
    void       SetCosScores (Float_t fcscore, Float_t fccscore) {cosmicscore = fcscore; coscontscore = fccscore;};
    void       Set_pid_info (Float_t fpida, Float_t fchi)       {pidpida = fpida; pidchi = fchi;};
    void       SetMCpdgCode (Int_t _mcpdg)  {MCpdgCode = _mcpdg;};
    void  SetCalorimetryPDG (Int_t _pdg[3]) {for (int i=0 ; i < 3 ; i++ ) CalorimetPDG[i] = _pdg[i];};
    void           Set_dqdx (Float_t, Float_t, Float_t, Int_t);
    void       SetFlashInfo (Float_t fcftime, Float_t fcftimewidth, Float_t fcfzcenter, Float_t fcfzwidth, Float_t fcfycenter, Float_t fcfywidth, Float_t fcftotalpe, Float_t fcfdistance);
    
    void   AddNeighborTrack ( Int_t , Float_t , Float_t );

    
    
    // getters
    TVector3    GetStartPos (){return start_pos;};
    TVector3      GetEndPos (){return end_pos;};
    Float_t       GetLength (){return length;};
    Float_t        GetTheta (){return theta;};
    Float_t          GetPhi (){return phi;};
    
        
        
        

    
    Int_t       MCpdgCode   , CalorimetPDG[3];
    Int_t       run         , subrun    , event;
    Int_t       nhits       , is_flipped;
    Int_t       NNeighborTracks;
    Short_t     track_id;
    
    TVector3    start_pos   , end_pos   ;
    
    Float_t     length      , theta     , phi , distlenratio , momentum;
    Float_t     start_dqdx  , end_dqdx  , tot_dqdx , avg_dqdx;
    Float_t     dqdx_diff   , dqdx_ratio;
    Float_t     pidpida     ,  pidchi   , cosmicscore   , coscontscore;
    Float_t     cftime      , cftimewidth , cfzcenter , cfzwidth, cfycenter , cfywidth  , cftotalpe , cfdistance;
    
    TString     TopBottDir  , ForBackDir , LefRghtDir;

    
    box         roi[3];
    
    // tracks which are closer than TracsMinDistance, at the same event, are labled as 'neighbor-tracks'
    vector<Int_t>       NeighborTracks;
    vector<Float_t>     NeighborTracksDistance;
    vector<Float_t>     NeighborTracksAngles;
};
#endif
/** @} */ // end of doxygen group

