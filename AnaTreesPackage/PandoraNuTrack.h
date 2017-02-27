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
    
    
    void            Print (bool DoPrintPos=false, bool DoPrintPandoraNuFeatures=false, bool DoPrintPlanes=false);
    
    
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
    void        SetStartPos (TVector3 pos)  {start_pos = pos;  startx=start_pos.x(); starty=start_pos.y(); startz=start_pos.z();};
    void          SetEndPos (TVector3 pos)  {end_pos = pos; endx=end_pos.x(); endy=end_pos.y(); endz=end_pos.z();};
    void          SetLength (Float_t l)     {length = l;};
    void           SetTheta (Float_t t)     {theta = t;};
    void             SetPhi (Float_t ph)    {phi = ph;};
    void     Set_start_dqdx (Float_t dqdx)  {start_dqdx = dqdx;};
    void       Set_end_dqdx (Float_t dqdx)  {end_dqdx = dqdx;};
    void       Set_tot_dqdx (Float_t dqdx)  {tot_dqdx = dqdx;};
    void       Set_avg_dqdx (Float_t dqdx)  {avg_dqdx = dqdx;};
    void          Set_nhits (Int_t n)       {nhits = n;};
    void       SetMCpdgCode (Int_t _mcpdg)  {MCpdgCode = _mcpdg;};
    void  SetCalorimetryPDG (Int_t _pdg[3]) {for (int i=0 ; i < 3 ; i++ ) CalorimetryPDG[i] = _pdg[i];};
    void  SetProcessPrimary (Int_t fpp)     {process_primary = fpp;};
    void            SetCCNC (Int_t fccnc)   {truth_ccnc = fccnc;};
    void        SetCF2Start (Float_t fc)    {cfdistance_start = fc;}; // set the closest flash distance to the start point of the track
    
    
    void   SetTruthStartPos (TVector3 pos)  {
        truth_start_pos = pos;  truth_startx=truth_start_pos.x(); truth_starty=truth_start_pos.y(); truth_startz=truth_start_pos.z();
    };
    void     SetTruthEndPos (TVector3 pos)  {
        truth_end_pos = pos; truth_endx=truth_end_pos.x(); truth_endy=truth_end_pos.y(); truth_endz=truth_end_pos.z();};

    void       SetCosScores (Float_t fcscore, Float_t fccscore)
    {cosmicscore = fcscore; coscontscore = fccscore;};
    
    void       Set_pid_info (Float_t fpida, Float_t fchi)
    {pidpida = fpida; pidchi = fchi;};
    
    void     SetTrackPurity (Float_t fpurtruth_U, Float_t fpurtruth_V, Float_t fpurtruth_Y)
    { purtruth_U = fpurtruth_U; purtruth_V = fpurtruth_V; purtruth_Y = fpurtruth_Y ;};

    void           Set_dqdx (Float_t, Float_t, Float_t, Int_t);
    void       SetFlashInfo (Float_t fcftime, Float_t fcftimewidth, Float_t fcfzcenter, Float_t fcfzwidth, Float_t fcfycenter, Float_t fcfywidth, Float_t fcftotalpe, Float_t fcfdistance);
    
    
    
    void   AddNeighborTrack ( Int_t , Float_t , Float_t );
    void           Set_dEdx (std::vector<Float_t> , std::vector<Float_t> , std::vector<Float_t> , std::vector<Float_t> , std::vector<Float_t> ,
                             std::vector<Float_t> , std::vector<Float_t> , std::vector<Float_t> , std::vector<Float_t> , std::vector<Float_t> ,
                             std::vector<Float_t> , std::vector<Float_t> , std::vector<Float_t> , std::vector<Float_t> , std::vector<Float_t>  );

    void    SetCalorimetry_Y (std::vector<Float_t> , std::vector<Float_t> , std::vector<Float_t> , std::vector<Float_t>   );
    
    void   SetSWtrigger (std::string * fswtrigger_name, bool * fswtrigger_triggered){
        for (size_t i=0 ; i < (int)(sizeof(fswtrigger_triggered)/sizeof(fswtrigger_triggered[0])) ; i++ ) {
            swtrigger_name.push_back(fswtrigger_name[i]);
            swtrigger_triggered.push_back(fswtrigger_triggered[i]);
        }
    };
    
    
    // finders
    Float_t ClosestDistanceToOtherTrack( PandoraNuTrack other_track , std::string * StartOrEnd=nullptr );
    Float_t DistanceFromPoint( TVector3 position , std::string * StartOrEnd=nullptr );


    
    // getters
    TVector3    GetStartPos (){return start_pos;};
    TVector3      GetEndPos (){return end_pos;};
    Float_t       GetLength (){return length;};
    Float_t        GetTheta (){return theta;};
    Float_t          GetPhi (){return phi;};
    box              GetROI (int plane) {return roi[plane];};
    Int_t        GetCaloPDG (int plane) {return CalorimetryPDG[plane];};
    Int_t           GetCCNC (){return truth_ccnc;}
    std::vector<Float_t> GetEdepYInfo (int step)
    {
        std::vector<Float_t> result = {residual_range_Y.at(step), dqdx_Y.at(step), dEdx_Y.at(step), Edep_Y.at(step)};
        return result;
    };
    
    std::vector<Float_t> GetTrackLengthVector (int plane)
    {
        switch (plane) {
            case 0:
                return residual_range_U;
                break;
            case 1:
                return residual_range_V;
                break;
            case 2:
                return residual_range_Y;
                break;
            default:
                return residual_range_Y;
                break;
        }};
    std::vector<Float_t> GetTrack_dEdxVector  (int plane) {
        switch (plane) {
            case 0:
                return dEdx_U;
                break;
            case 1:
                return dEdx_V;
                break;
            case 2:
                return dEdx_Y;
                break;
            default:
                return dEdx_Y;
                break;
        }};
    Int_t GetNSWtrigger () {return (int)swtrigger_name.size();};
    
        

    
    Int_t       CalorimetryPDG[3];
    Int_t       run         , subrun    , event;
    Int_t       nhits       , is_flipped;
    Int_t       NNeighborTracks;
    Int_t       truth_ccnc; // 0=CC interaction , 1=NC interaction
    Short_t     track_id    , process_primary;
    
    Float_t     startx  , starty , startz , endx , endy , endz;
    TVector3    start_pos   , end_pos   ;
    
    Float_t     length      , theta     , phi , distlenratio , momentum;
    Float_t     start_dqdx  , end_dqdx  , tot_dqdx , avg_dqdx;
    Float_t     dqdx_diff   , dqdx_ratio;
    Float_t     pidpida     , pidchi    , cosmicscore   , coscontscore;
    Float_t     cftime      , cftimewidth   , cfzcenter , cfzwidth, cfycenter , cfywidth  , cftotalpe , cfdistance;
    Float_t     cfdistance_start;
    // The trkpurtruth - purity variable is defined as the ratio of the energy of the particle that contributed most to this track in a given plane to the total energy coming from all particles that contribute to this track in that plane
    Float_t     purtruth_U  , purtruth_V    , purtruth_Y;
    
    
    TString     TopBottDir  , ForBackDir    , LefRghtDir;

    
    box         roi[3];
    
    // dE/dx
    Int_t       NEdepYsteps;
    std::vector <Float_t> track_dx_U, residual_range_U, dEdx_U , Edep_U, dqdx_U;
    std::vector <Float_t> track_dx_V, residual_range_V, dEdx_V , Edep_V, dqdx_V;
    std::vector <Float_t> track_dx_Y, residual_range_Y, dEdx_Y , Edep_Y, dqdx_Y;

    
    // tracks which are closer than TracsMinDistance, at the same event, are labled as 'neighbor-tracks'
    vector<Int_t>       NeighborTracks;
    vector<Float_t>     NeighborTracksDistance;
    vector<Float_t>     NeighborTracksAngles;
    
    
    // truth information - only valid for MC data
    Int_t       MCpdgCode;
    Float_t     truth_Eng , truth_P , truth_Mass , truth_KE;
    Float_t     truth_theta         , truth_phi;
    Float_t     truth_startx        , truth_starty , truth_startz , truth_endx , truth_endy , truth_endz;
    TVector3    truth_start_pos     , truth_end_pos;
    
    // true CC1p (GENIE interaction)
    bool        IsGENIECC1p;
    Int_t       mcevent_id;

    
    
    // software trigger
    std::vector<std::string> swtrigger_name;       // the name of the trigger algorithm
    std::vector<bool>        swtrigger_triggered;  // true = event is triggered; false = event is not triggered based on the relative algorithm logic

};
#endif
/** @} */ // end of doxygen group

