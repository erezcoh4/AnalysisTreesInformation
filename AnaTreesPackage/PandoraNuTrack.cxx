#ifndef PANDORANUTRACK_CXX
#define PANDORANUTRACK_CXX

#include "PandoraNuTrack.h"
#include "LArUtil/GeometryHelper.h"


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
PandoraNuTrack::PandoraNuTrack():
MCpdgCode(-1000),
run(-100),
subrun(-100),
event(-100),
track_id(-100),
start_pos(TVector3(-100,-100,-100)),
end_pos(TVector3(-100,-100,-100)),
length(-100),
theta(-100),
phi(-100),
start_dqdx(-100),
end_dqdx(-100),
tot_dqdx(-100),
avg_dqdx(-100),
pidpida(-100),
pidchi(-100),
cosmicscore(-100),
coscontscore(-100),
process_primary(-100),
IsGENIECC1p(-100),
mcevent_id(-100),
truth_ccnc(-100)
{}


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
PandoraNuTrack::~PandoraNuTrack(){ }

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
PandoraNuTrack::PandoraNuTrack (
                                Int_t frun
                                ,Int_t fsubrun
                                ,Int_t fevent
                                ,Short_t ftrack_id
                                ,TVector3 fstart_pos
                                ,TVector3 fend_pos
                                ,Float_t flength
                                ,Float_t ftheta
                                ,Float_t fphi
                                ){

    SetRun(frun);
    SetSubrun(fsubrun);
    SetEvent(fevent);
    SetTrackID(ftrack_id);
    SetStartPos(fstart_pos);
    SetEndPos(fend_pos);
    SetLength(flength);
    SetTheta(ftheta);
    SetPhi(fphi);
    NNeighborTracks = 0;
    IsGENIECC1p = false;
    
}





//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void PandoraNuTrack::SetFlashInfo(Float_t fcftime, Float_t fcftimewidth, Float_t fcfzcenter, Float_t fcfzwidth, Float_t fcfycenter, Float_t fcfywidth, Float_t fcftotalpe, Float_t fcfdistance){
    cftime         = fcftime;
    cftimewidth    = fcftimewidth;
    cfzcenter      = fcfzcenter;
    cfzwidth       = fcfzwidth;
    cfycenter      = fcfycenter;
    cfywidth       = fcfywidth;
    cftotalpe      = fcftotalpe;
    cfdistance     = fcfdistance;
}


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void PandoraNuTrack::Set_dqdx (Float_t fstart_dqdx, Float_t fend_dqdx, Float_t ftot_dqdx, Int_t fnhits){
    Set_start_dqdx(fstart_dqdx);
    Set_end_dqdx(fend_dqdx);
    Set_tot_dqdx(ftot_dqdx);
    Set_nhits(fnhits);
    Set_avg_dqdx( (nhits>0) ? tot_dqdx/nhits : 0 );
    
    // fix start and end points by charge: If start charge is greater, flip track
    is_flipped = 0;
    if(fstart_dqdx > fend_dqdx) FlipTrack();
 
}


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void PandoraNuTrack::SetCalorimetry_Y ( std::vector <Float_t> fresidual_range_Y, std::vector <Float_t> fdEdx_Y, std::vector <Float_t> fEdep_Y, std::vector <Float_t> fdqdx_Y){
    residual_range_Y = fresidual_range_Y;
    NEdepYsteps = residual_range_Y.size();
    dEdx_Y = fdEdx_Y;
    Edep_Y = fEdep_Y;
    dqdx_Y = fdqdx_Y;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void PandoraNuTrack::FlipTrack(int debug){
    
    if(debug>0) Printf("flipping track %i",track_id);
    
    // flip start and end positions
    TVector3 tmp_pos = start_pos;
    start_pos   = end_pos;
    end_pos     = tmp_pos;
    
    // change angles
    theta       = 3.1416 - theta;
    phi         = (phi > 0 ? phi-3.1416 : phi+3.1416);
    
    Float_t tmp_dqdx = start_dqdx;
    start_dqdx  = end_dqdx;
    end_dqdx    = tmp_dqdx;
    
    is_flipped  = 1;
}





//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void PandoraNuTrack::CreateROIs(){

    // load GeometryHelper utility
    auto geomHelper = ::larutil::GeometryHelper::GetME();

    double start_xyz[3] = {start_pos.x() , start_pos.y() , start_pos.z() };
    double end_xyz[3] = {end_pos.x() , end_pos.y() , end_pos.z() };

    // shift in time-axis due to the truncation of the waveforms
    // (the first 2400 ADCs are removed from the waveform, The extra couple ticks could be due to a shift in the signal deconvolution)
    double time_shift =  802;

    
    // there is a bug here! it sometimes won't generate the ROIs correctly for muon tracks - why?? it seems to generate protons ROIs fine...
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

        
        start_box[plane] = box( start_wire - 10 , start_time - 20 , start_wire + 10 , start_time + 20 );
        end_box[plane] = box( end_wire - 10 , end_time - 20 , end_wire + 10 , end_time + 20 );
    }
    
}





//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void PandoraNuTrack::Calorimetry(){
    
    // make some additional caloritric variables
    dqdx_diff     = end_dqdx - start_dqdx;
    dqdx_ratio    = end_dqdx/start_dqdx;
    
    // ask about the directionality of the track
    TopBottDir = (start_pos.y() > end_pos.y()) ? "Top->Bottom"              : "Bottom->Top"         ;
    ForBackDir = (start_pos.z() > end_pos.z()) ? "backward (against BNB)"   : "forward (along BNB)" ;
    LefRghtDir = (start_pos.x() > end_pos.x()) ? "Left -> Right"            : "Right -> Left"       ;
    
}




//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void PandoraNuTrack::Straightness(){
    
    // get straightness (distance from start to end of track over reco'd length)
    distlenratio = ((end_pos-start_pos).Mag())/length;
    
    
}




//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void PandoraNuTrack::Momentum(){
    
    // this is true only if this track was a proton
    
    
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void PandoraNuTrack::AddNeighborTrack( Int_t ftrack_id , Float_t fClosestDistance , Float_t fangle ){
    
    NNeighborTracks++ ;
    
    NeighborTracks.push_back( ftrack_id );
    
    NeighborTracksDistance.push_back( fClosestDistance );

    NeighborTracksAngles.push_back( fangle );
    
    

}



//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void PandoraNuTrack::Print(bool DoPrintPos, bool DoPrintPandoraNuFeatures, bool DoPrintPlanes){

    cout << "\033[31m" << "~~~~~~~~~~~~~~~~~~~~~\n" << "track " << track_id << "\n~~~~~~~~~~~~~~~~~~~~~ "<< "\033[0m" << endl;
    SHOW3(run , subrun , event);
    if (DoPrintPos){
        SHOWTVector3(start_pos);
        SHOWTVector3(end_pos);
    }
    if (DoPrintPandoraNuFeatures){
        PrintPhys(length,"cm");
        PrintPhys(momentum,"MeV/c");
        SHOW(distlenratio);
        PrintPhys(theta,"radians");
        PrintPhys(phi,"radians");
        PrintPhys(start_dqdx,"ADC/cm");
        PrintPhys(end_dqdx,"ADC/cm");
        PrintPhys(tot_dqdx,"ADC/cm");
        SHOW3( cosmicscore, coscontscore , pidpida )
        SHOW2( cfdistance , cfdistance_start );
    }
    if (DoPrintPlanes){
        SHOW3( purtruth_U , purtruth_V   , purtruth_Y );
        for (int plane = 0 ; plane < 3; plane++) {
            PrintPhys( CalorimetryPDG[plane] , Form(" for plane %d",plane) );
            PrintBox(roi[plane]);
        }
    }
    //    cout << "\033[33m" << NNeighborTracks << " neighboring tracks" ;
    //    for (size_t i = 0 ; i < NNeighborTracks ; i++ ){
    //        cout
    //        << "\ntrack "               << NeighborTracks[i]
    //        << ", distance: "           << NeighborTracksDistance[i]
    //        << " cm, relative angle:"   << NeighborTracksAngles[i]
    //        << " deg.";
    //    }
    //    cout << "\033[0m" << endl;
    if ( truth_Eng>0 && truth_P>0 ){
        cout << "MC information: " << endl;
        SHOW ( mcevent_id );
        SHOW2 ( process_primary , MCpdgCode );
        PrintPhys (truth_P , "GeV/c");
        SHOW3(truth_Eng , truth_theta , truth_phi);
        SHOWTVector3(truth_start_pos);
        SHOWTVector3(truth_end_pos);
        SHOW2( truth_ccnc, IsGENIECC1p );
    }


}


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
Float_t PandoraNuTrack::ClosestDistanceToOtherTrack( PandoraNuTrack other_track, std::string * fStartOrEnd ){
    Float_t MinDistanceToOtherTrack = 10000;
    std::string StartOrEnd = "None";
    Float_t DistanceStartStart = (start_pos - other_track.start_pos).Mag();
    if (MinDistanceToOtherTrack>DistanceStartStart)     {MinDistanceToOtherTrack = DistanceStartStart; StartOrEnd = "Start";}
    
    Float_t DistanceStartEnd = (start_pos - other_track.end_pos).Mag();
    if (MinDistanceToOtherTrack>DistanceStartEnd)       {MinDistanceToOtherTrack = DistanceStartEnd; StartOrEnd = "Start";}
    
    Float_t DistanceEndStart = (end_pos - other_track.start_pos).Mag();
    if (MinDistanceToOtherTrack>DistanceEndStart)       {MinDistanceToOtherTrack = DistanceEndStart; StartOrEnd = "End";}
    
    Float_t DistanceEndEnd = (end_pos - other_track.end_pos).Mag();
    if (MinDistanceToOtherTrack>DistanceEndEnd)         {MinDistanceToOtherTrack = DistanceEndEnd; StartOrEnd = "End";}
    
    
    if (fStartOrEnd!=nullptr) *fStartOrEnd = StartOrEnd;

    return MinDistanceToOtherTrack;
}


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
Float_t PandoraNuTrack::DistanceFromPoint( TVector3 position, std::string * fStartOrEnd  ){
    Float_t DistanceStart, DistanceEnd , distance = 1000;
    std::string StartOrEnd = "None";

    DistanceStart = ( start_pos - position).Mag();
    DistanceEnd = ( end_pos - position).Mag();
    if ( DistanceStart < DistanceEnd ){
        StartOrEnd = "Start";
        distance = DistanceStart;
    }
    else{
        StartOrEnd = "End";
        distance = DistanceEnd;
    }

    return distance;
}


#endif
