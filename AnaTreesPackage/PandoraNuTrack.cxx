#ifndef PANDORANUTRACK_CXX
#define PANDORANUTRACK_CXX

#include "PandoraNuTrack.h"
#include "LArUtil/GeometryHelper.h"


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
PandoraNuTrack::PandoraNuTrack():
MCpdgCode(-100),
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
coscontscore(-100)
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
void PandoraNuTrack::Set_dEdx (std::vector <Float_t> * ftrack_dx, std::vector <Float_t> * ftrack_length, std::vector <Float_t> * fdEdx ){
    for (int plane = 0; plane < 3; plane++) {
        track_dx[plane] = ftrack_dx[plane];
        track_length[plane] = ftrack_length[plane];
        dEdx[plane] = fdEdx[plane];
    }
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
void PandoraNuTrack::Print(){

    cout << "\033[31m" << "track " << track_id << "\n~~~~~~~~~~~~~~~~~~~~~ "<< "\033[0m" << endl;
    SHOW3(run , subrun , event);
    SHOWTVector3(start_pos);
    SHOWTVector3(end_pos);
    PrintPhys(length,"cm");
    PrintPhys(momentum,"MeV/c");
    SHOW(distlenratio);
    PrintPhys(theta,"radians");
    PrintPhys(phi,"radians");
    PrintPhys(start_dqdx,"ADC/cm");
    PrintPhys(end_dqdx,"ADC/cm");
    PrintPhys(tot_dqdx,"ADC/cm");
    SHOW(MCpdgCode);
    SHOW3( cosmicscore, coscontscore , pidpida )
    
    for (int plane = 0 ; plane < 3; plane++) {
//        Printf("Calorimetry, plane %d:",plane);
//        for (size_t trkhit=0; trkhit < track_length[plane].size(); trkhit++) {
//            SHOW2( track_length[plane].at(trkhit) ,  dEdx[plane].at(trkhit) );
//        }
        PrintPhys( CalorimetryPDG[plane] , Form(" for plane %d",plane) );
        PrintBox(roi[plane]);
    }
    cout << "\033[33m" << NNeighborTracks << " neighboring tracks" ;
    for (size_t i = 0 ; i < NNeighborTracks ; i++ ){
        cout
        << "\ntrack "               << NeighborTracks[i]
        << ", distance: "           << NeighborTracksDistance[i]
        << " cm, relative angle:"   << NeighborTracksAngles[i]
        << " deg.";
    }
    cout << "\033[0m" << endl;

}


#endif
