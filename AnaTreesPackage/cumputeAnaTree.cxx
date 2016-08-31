#ifndef cumputeAnaTree_CXX
#define cumputeAnaTree_CXX

#include "cumputeAnaTree.h"


// main event loop
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
bool cumputeAnaTree::extract_information (){ // main event loop....
    
    GetInTimeFlashes();
    
    GetPandoraNuTracks();

    GetPandoraCosmicTracks();
    
    if (MCmode) GetTruthInformation();

    // if we want to collect vertices, these should be uncommented out
    //    CollectTrackVertices();
    //    FindMutualVertices();
    //    FindMuonScattering();
    
    return true;
}


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
cumputeAnaTree::cumputeAnaTree( TTree * fInTree, TTree * fOutTree, TTree * fGENIETree, TString fCSVFileName, int fdebug, bool fMCmode){
    
    SetInTree(fInTree);
    SetOutTree(fOutTree);
    SetGENIETree(fGENIETree);
    SetCSVFileName (fCSVFileName);
    SetDebug(fdebug);
    SetMCMode(fMCmode);
    InitInputTree();
    InitOutputTree();
    InitOutputCSV();
    
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void cumputeAnaTree::InitInputTree(){
    
    // event info
    InTree -> SetBranchAddress("run"                                            , &run);
    InTree -> SetBranchAddress("subrun"                                         , &subrun);
    InTree -> SetBranchAddress("event"                                          , &event);

    // pandoraNu
    InTree -> SetBranchAddress("ntracks_pandoraNu"                              , &ntracks_pandoraNu);
    InTree -> SetBranchAddress("trkId_pandoraNu"                                , &trkId_pandoraNu);
    InTree -> SetBranchAddress("trklen_pandoraNu"                               , &trklen_pandoraNu);
    InTree -> SetBranchAddress("trkstartx_pandoraNu"                            , &trkstartx_pandoraNu);
    InTree -> SetBranchAddress("trkstarty_pandoraNu"                            , &trkstarty_pandoraNu);
    InTree -> SetBranchAddress("trkstartz_pandoraNu"                            , &trkstartz_pandoraNu);
    InTree -> SetBranchAddress("trkendx_pandoraNu"                              , &trkendx_pandoraNu);
    InTree -> SetBranchAddress("trkendy_pandoraNu"                              , &trkendy_pandoraNu);
    InTree -> SetBranchAddress("trkendz_pandoraNu"                              , &trkendz_pandoraNu);
    InTree -> SetBranchAddress("trktheta_pandoraNu"                             , &trktheta_pandoraNu);
    InTree -> SetBranchAddress("trkphi_pandoraNu"                               , &trkphi_pandoraNu);
    InTree -> SetBranchAddress("ntrkhits_pandoraNu"                             , &ntrkhits_pandoraNu);
    InTree -> SetBranchAddress("trkdqdx_pandoraNu"                              , &trkdqdx_pandoraNu);
    InTree -> SetBranchAddress("trkresrg_pandoraNu"                             , &trkresrg_pandoraNu);
    InTree -> SetBranchAddress("trkncosmictags_tagger_pandoraNu"                , &trkncosmictags_tagger_pandoraNu);
    InTree -> SetBranchAddress("trkcosmicscore_tagger_pandoraNu"                , &trkcosmicscore_tagger_pandoraNu);
    InTree -> SetBranchAddress("trkcosmictype_tagger_pandoraNu"                 , &trkcosmictype_tagger_pandoraNu);
    InTree -> SetBranchAddress("trkcosmicscore_containmenttagger_pandoraNu"     , &trkcosmicscore_containmenttagger_pandoraNu);
    InTree -> SetBranchAddress("trkcosmictype_containmenttagger_pandoraNu"      , &trkcosmictype_containmenttagger_pandoraNu);
    InTree -> SetBranchAddress("trkpidchi_pandoraNu"                            , &trkpidchi_pandoraNu);
    InTree -> SetBranchAddress("trkpidpida_pandoraNu"                           , &trkpidpida_pandoraNu);
    InTree -> SetBranchAddress("trkpidbestplane_pandoraNu"                      , &trkpidbestplane_pandoraNu);
    InTree -> SetBranchAddress("trkpidpdg_pandoraNu"                            , &trkpidpdg_pandoraNu);

    
    // optical info
    InTree -> SetBranchAddress("no_hits"                                        , &no_hits);
    InTree -> SetBranchAddress("hit_plane"                                      , &hit_plane);
    InTree -> SetBranchAddress("hit_wire"                                       , &hit_wire);
    InTree -> SetBranchAddress("hit_trkid"                                      , &hit_trkid);
    InTree -> SetBranchAddress("hit_trkKey"                                     , &hit_trkKey);
    InTree -> SetBranchAddress("no_flashes"                                     , &no_flashes);
    InTree -> SetBranchAddress("flash_time"                                     , &flash_time);
    InTree -> SetBranchAddress("flash_timewidth"                                , &flash_timewidth);
    InTree -> SetBranchAddress("flash_pe"                                       , &flash_pe);
    InTree -> SetBranchAddress("flash_ycenter"                                  , &flash_ycenter);
    InTree -> SetBranchAddress("flash_ywidth"                                   , &flash_ywidth);
    InTree -> SetBranchAddress("flash_zcenter"                                  , &flash_zcenter);
    InTree -> SetBranchAddress("flash_zwidth"                                   , &flash_zwidth);
    
    
    // vertex
    InTree -> SetBranchAddress("nvtx_pandoraNu"                                  , &nvtx_pandoraNu);
    InTree -> SetBranchAddress("vtxx_pandoraNu"                                  , &vtxx_pandoraNu);
    InTree -> SetBranchAddress("vtxy_pandoraNu"                                  , &vtxy_pandoraNu);
    InTree -> SetBranchAddress("vtxz_pandoraNu"                                  , &vtxz_pandoraNu);

    
    
    // pandoraCosmic
    InTree -> SetBranchAddress("ntracks_pandoraCosmic"                          , &ntracks_pandoraCosmic);
    InTree -> SetBranchAddress("trkId_pandoraCosmic"                            , &trkId_pandoraCosmic);
    InTree -> SetBranchAddress("trklen_pandoraCosmic"                           , &trklen_pandoraCosmic);
    InTree -> SetBranchAddress("trkstartx_pandoraCosmic"                        , &trkstartx_pandoraCosmic);
    InTree -> SetBranchAddress("trkstarty_pandoraCosmic"                        , &trkstarty_pandoraCosmic);
    InTree -> SetBranchAddress("trkstartz_pandoraCosmic"                        , &trkstartz_pandoraCosmic);
    InTree -> SetBranchAddress("trkendx_pandoraCosmic"                          , &trkendx_pandoraCosmic);
    InTree -> SetBranchAddress("trkendy_pandoraCosmic"                          , &trkendy_pandoraCosmic);
    InTree -> SetBranchAddress("trkendz_pandoraCosmic"                          , &trkendz_pandoraCosmic);
    InTree -> SetBranchAddress("trktheta_pandoraCosmic"                         , &trktheta_pandoraCosmic);
    InTree -> SetBranchAddress("trkphi_pandoraCosmic"                           , &trkphi_pandoraCosmic);
    InTree -> SetBranchAddress("ntrkhits_pandoraCosmic"                         , &ntrkhits_pandoraCosmic);
    InTree -> SetBranchAddress("trkdqdx_pandoraCosmic"                          , &trkdqdx_pandoraCosmic);
    InTree -> SetBranchAddress("trkresrg_pandoraCosmic"                         , &trkresrg_pandoraCosmic);
    InTree -> SetBranchAddress("trkncosmictags_tagger_pandoraCosmic"            , &trkncosmictags_tagger_pandoraCosmic);
    InTree -> SetBranchAddress("trkcosmicscore_tagger_pandoraCosmic"            , &trkcosmicscore_tagger_pandoraCosmic);
    InTree -> SetBranchAddress("trkcosmictype_tagger_pandoraCosmic"             , &trkcosmictype_tagger_pandoraCosmic);
    InTree -> SetBranchAddress("trkcosmicscore_containmenttagger_pandoraCosmic" , &trkcosmicscore_containmenttagger_pandoraCosmic);
    InTree -> SetBranchAddress("trkcosmictype_containmenttagger_pandoraCosmic"  , &trkcosmictype_containmenttagger_pandoraCosmic);
    InTree -> SetBranchAddress("trkpidchi_pandoraCosmic"                        , &trkpidchi_pandoraCosmic);
    InTree -> SetBranchAddress("trkpidpida_pandoraCosmic"                       , &trkpidpida_pandoraCosmic);
    InTree -> SetBranchAddress("trkpidbestplane_pandoraCosmic"                  , &trkpidbestplane_pandoraCosmic);
    InTree -> SetBranchAddress("trkpidpdg_pandoraCosmic"                        , &trkpidpdg_pandoraCosmic);
    
    
    // vertex
    InTree -> SetBranchAddress("nvtx_pandoraCosmic"                             , &nvtx_pandoraCosmic);
    InTree -> SetBranchAddress("vtxx_pandoraCosmic"                             , &vtxx_pandoraCosmic);
    InTree -> SetBranchAddress("vtxy_pandoraCosmic"                             , &vtxy_pandoraCosmic);
    InTree -> SetBranchAddress("vtxz_pandoraCosmic"                             , &vtxz_pandoraCosmic);
    


    
    if (MCmode) {
        
        if (debug>1)  Printf ("\n\nrunning on MC mode...\n\n");
        InTree -> SetBranchAddress("geant_list_size"                            , &geant_list_size);
        InTree -> SetBranchAddress("trkg4id_pandoraCosmic"                      , &trkg4id_pandoraNu);
        InTree -> SetBranchAddress("TrackId"                                    , &TrackId);
        InTree -> SetBranchAddress("pdg"                                        , &pdg);
        
        InTree -> SetBranchAddress("mcevts_truth"                               , &mcevts_truth);
        InTree -> SetBranchAddress("nuPDG_truth"                                , &nuPDG_truth);
        InTree -> SetBranchAddress("ccnc_truth"                                 , &ccnc_truth);
        InTree -> SetBranchAddress("mode_truth"                                 , &mode_truth);
        InTree -> SetBranchAddress("enu_truth"                                  , &enu_truth);
        InTree -> SetBranchAddress("Q2_truth"                                   , &Q2_truth);
        InTree -> SetBranchAddress("W_truth"                                    , &W_truth);
        InTree -> SetBranchAddress("X_truth"                                    , &X_truth);
        InTree -> SetBranchAddress("Y_truth"                                    , &Y_truth);
        InTree -> SetBranchAddress("hitnuc_truth"                               , &pdg);
        InTree -> SetBranchAddress("nuvtxx_truth"                               , &nuvtxx_truth);
        InTree -> SetBranchAddress("nuvtxy_truth"                               , &nuvtxy_truth);
        InTree -> SetBranchAddress("nuvtxz_truth"                               , &nuvtxz_truth);
        InTree -> SetBranchAddress("nu_dcosx_truth"                             , &nu_dcosx_truth);
        InTree -> SetBranchAddress("nu_dcosy_truth"                             , &nu_dcosy_truth);
        InTree -> SetBranchAddress("nu_dcosz_truth"                             , &nu_dcosz_truth);
        InTree -> SetBranchAddress("lep_mom_truth"                              , &lep_mom_truth);
        InTree -> SetBranchAddress("lep_dcosx_truth"                            , &lep_dcosx_truth);
        InTree -> SetBranchAddress("lep_dcosy_truth"                            , &lep_dcosy_truth);
        InTree -> SetBranchAddress("lep_dcosz_truth"                            , &lep_dcosz_truth);
        
        // GENIE information, is outside of the nucleus (after reaction)
        InTree -> SetBranchAddress("genie_no_primaries"                         , &genie_no_primaries);
        InTree -> SetBranchAddress("genie_primaries_pdg"                        , &genie_pdg);
        InTree -> SetBranchAddress("genie_Eng"                                  , &genie_Eng);
        InTree -> SetBranchAddress("genie_Px"                                   , &genie_Px);
        InTree -> SetBranchAddress("genie_Py"                                   , &genie_Py);
        InTree -> SetBranchAddress("genie_Pz"                                   , &genie_Pz);
        InTree -> SetBranchAddress("genie_P"                                    , &genie_P);
        InTree -> SetBranchAddress("genie_status_code"                          , &genie_status_code);
        InTree -> SetBranchAddress("genie_mass"                                 , &genie_mass);
        InTree -> SetBranchAddress("genie_trackID"                              , &genie_trackID);
        InTree -> SetBranchAddress("genie_ND"                                   , &genie_ND);
        InTree -> SetBranchAddress("genie_mother"                               , &genie_mother);

     }
    
    Nentries = InTree -> GetEntries();
    if(debug>1) cout << "cumputeAnaTree input-tree ready (" << InTree -> GetName() <<"), " <<  Nentries << " entries" << endl;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void cumputeAnaTree::InitOutputTree(){
    
    // Integer branches
    OutTree -> Branch("run"         ,&run               ,"run/I");
    OutTree -> Branch("subrun"      ,&subrun            ,"subrun/I");
    OutTree -> Branch("event"       ,&event             ,"event/I");
    OutTree -> Branch("Ntracks"     ,&Ntracks           ,"Ntracks/I"); // number of contained tracks, not ntracks_pandoraNu...
    
    OutTree -> Branch("nu_interactions"     ,&nu_interactions); // neutrino interactions...
    OutTree -> Branch("tracks"              ,&tracks); // tracks information...
    
    if (MCmode){
        OutTree -> Branch("genie_interactions"  ,&genie_interactions); // genie interactions...
        GENIETree -> Branch("genie_interaction"  ,&genie_interaction); // genie interactions in a seperate tree...
    }


    if(debug>1) cout << "cumputeAnaTree output-tree ready (" << OutTree -> GetTitle() << ")" << endl;
}


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void cumputeAnaTree::InitOutputCSV(){
    
    csvfile.open(CSVFileName);
    
    CSVHeader =
    TString("run subrun event ")
    +TString("U_start_wire U_start_time U_end_wire U_end_time ")
    +TString("V_start_wire V_start_time V_end_wire V_end_time ")
    +TString("Y_start_wire Y_start_time Y_end_wire Y_end_time ");
    
    csvfile << CSVHeader << endl;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void cumputeAnaTree::GetEntry (int entry){
    InitEntry();
    InTree -> GetEntry(entry);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void cumputeAnaTree::InitEntry(){
    
    if (!nu_interactions.empty())       nu_interactions.clear();
    if (!genie_interactions.empty())    genie_interactions.clear();
    if (!goodflashidx.empty())          goodflashidx.clear();
    if (!cosmic_tracks.empty())         cosmic_tracks.clear();
    if (!tracks.empty())                tracks.clear();
    if (!tracks_vertices.empty())       tracks_vertices.clear();
    if (!mutual_vertices.empty())       mutual_vertices.clear();
    
    NnuInteractions = Ntracks = ntracks_pandoraNu = 0;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void cumputeAnaTree::InitTrack(){
    totaldqdx  = startdqdx = enddqdx = nhits = 0;
    
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void cumputeAnaTree::GetInTimeFlashes(){
    
    if(debug>3) Printf("GetInTimeFlashes of %d flashes",no_flashes);
    
    // get a list of in-time flashes for the event
    if(no_flashes > 0) {
        
        if (!goodflashidx.empty())  goodflashidx.clear();
        
        if(debug>3) { SHOW(goodflashidx.size()); Printf("looping in if(no_flashes > 0) on no_flashes"); }
        
        for(int i=0; i < no_flashes && i < MAX_hits ; i++){
            
            if(debug>4) SHOW3( i , flash_time[i] , flash_pe[i] );
            
            if( (0.0 < flash_time[i]) && (flash_time[i] < 10.0) && (6.5 < flash_pe[i]) ){
                goodflashidx.push_back( i );
                if(debug>3) { SHOW3( i , goodflashidx.size() , goodflashidx.back() ); }
            }
            
            if ( 10.0 < flash_time[i] ) {
                break;
            }
        }
    }
    if(debug>3) {
        if(!goodflashidx.empty()){
            PrintLine();
            SHOW(goodflashidx.size());
            SHOWstdVector(goodflashidx);
            PrintLine();
        }
    }
    
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void cumputeAnaTree::GetPandoraNuTracks(){
    
    if (ntracks_pandoraNu==0 || ntracks_pandoraNu>100) return;
    if(debug>2) Printf("LoopPanNuTracks on %d tracks",ntracks_pandoraNu);
    
    
    
    // loop over all reconstructed tracks
    for(Int_t j=0; j < ntracks_pandoraNu && j < MAX_tracks; j++){
        
        InitTrack();
        
        
        c_track = PandoraNuTrack(
                              run                                                                                // run
                              ,subrun                                                                            // subrun
                              ,event                                                                             // event
                              ,trkId_pandoraNu[j]                                                                // track id
                              ,TVector3(trkstartx_pandoraNu[j], trkstarty_pandoraNu[j], trkstartz_pandoraNu[j])  // start position
                              ,TVector3(trkendx_pandoraNu[j]  , trkendy_pandoraNu[j]  , trkendz_pandoraNu[j])    // end position
                              ,trklen_pandoraNu[j]                                                               // track length
                              ,trktheta_pandoraNu[j]                                                             // theta
                              ,trkphi_pandoraNu[j]                                                               // phi
                              );
        
        if (!TrackContained( c_track.start_pos   , c_track.end_pos )) continue;
        Ntracks ++ ;
        if(debug>3) Printf("created (contained) track %d...",j);
        
        // get flash info
        // compare reconstructed track to list of flashes in beam to find closest
        float tzcenter = (c_track.start_pos.z() + c_track.end_pos.z())/2.;
        if(debug>3) Printf("with tzcenter = %.2f, goodflashidx.size() = %lu",tzcenter,goodflashidx.size());
        
        if(goodflashidx.size() > 0) {
            int   minzi    = goodflashidx.at(0);
            float minzdiff = TMath::Abs(flash_zcenter[minzi] - tzcenter);
            if(debug>3) {SHOW(minzi);SHOW(minzdiff);}
            for(size_t k=0; k < goodflashidx.size(); k++)
            {
                int   fidx     = goodflashidx.at(k);
                float fzcenter = flash_zcenter[fidx];
                if(debug>3) {SHOW(fidx);SHOW(fzcenter);}
                if(TMath::Abs(fzcenter - tzcenter) < minzdiff)
                {
                    minzi    = fidx;
                    minzdiff = TMath::Abs(fzcenter = tzcenter);
                }
            }
            cftime      = flash_time[minzi];
            cftimewidth = flash_timewidth[minzi];
            cfzcenter   = flash_zcenter[minzi];
            cfzwidth    = flash_zwidth[minzi];
            cfycenter   = flash_ycenter[minzi];
            cfywidth    = flash_ywidth[minzi];
            cftotalpe   = flash_pe[minzi];
            cfdistance  = tzcenter - cfzcenter;
        }
        else {
            cftime = cftimewidth = cfzcenter = cfzwidth = cfycenter = cfywidth = cftotalpe = cfdistance = -9999;
        }
        c_track.SetFlashInfo(cftime , cftimewidth , cfzcenter , cfzwidth , cfycenter , cfywidth , cftotalpe , cfdistance);
        if(debug>3) Printf("Set Flash Info...");
        
        
        // get cosmic scores
        c_track.SetCosScores( trkcosmicscore_tagger_pandoraNu[j][0] , trkcosmicscore_containmenttagger_pandoraNu[j][0] );
        // get pid info
        c_track.Set_pid_info( trkpidpida_pandoraNu[j][trkpidbestplane_pandoraNu[j]] , trkpidchi_pandoraNu[j][trkpidbestplane_pandoraNu[j]] );
        
        
        // get dqdx info: loop over range from end of track to find start and end
        int   rmin[3] , rmax[3];
        if(debug>3) Printf("before for(Int_t fr=0; fr<3;fr++) ...");
        for(Int_t fr=0; fr<3;fr++) {
            
            if(ntrkhits_pandoraNu[j][fr] >= 0) {
                
                nhits     += ntrkhits_pandoraNu[j][fr];
                rmin[fr]   = rmax[fr]   = trkresrg_pandoraNu[j][fr][0];
                totaldqdx += trkdqdx_pandoraNu[j][fr][0];
                int minidx = 0 , maxidx = 0;
                
                for(Int_t ridx=0; ridx < ntrkhits_pandoraNu[j][fr]; ridx++) {
                    if(trkresrg_pandoraNu[j][fr][ridx] < rmin[fr] && trkdqdx_pandoraNu[j][fr][ridx] != 0) {
                        rmin[fr] = trkresrg_pandoraNu[j][fr][ridx];
                        minidx   = ridx;
                    }
                    if(trkresrg_pandoraNu[j][fr][ridx] > rmax[fr]) {
                        rmax[fr] = trkresrg_pandoraNu[j][fr][ridx];
                        maxidx   = ridx;
                    }
                    totaldqdx += trkdqdx_pandoraNu[j][fr][ridx];
                }
                if(maxidx >= 3) {
                    startdqdx   += (trkdqdx_pandoraNu[j][fr][maxidx] + trkdqdx_pandoraNu[j][fr][maxidx-1]
                                    + trkdqdx_pandoraNu[j][fr][maxidx-2]);
                    enddqdx     += (trkdqdx_pandoraNu[j][fr][minidx] + trkdqdx_pandoraNu[j][fr][minidx+1]
                                    + trkdqdx_pandoraNu[j][fr][minidx+2]);
                } else {
                    startdqdx   += trkdqdx_pandoraNu[j][fr][maxidx];
                    enddqdx     += trkdqdx_pandoraNu[j][fr][minidx];
                }
            }
        }
        if(debug>3) Printf("after for(Int_t fr=0; fr<3;fr++) ...");
        c_track.Set_dqdx( startdqdx , enddqdx , totaldqdx , nhits );
        if(debug>3) Printf("Set dq/dx ...");
        c_track.CreateROIs();
        if(debug>3) Printf("Created ROIs...");
        c_track.Calorimetry();
        if(debug>3) Printf("made some Calorimetry ...");
        c_track.Straightness();
        if(debug>3) Printf("calculated the Straightness of the track ...");
        c_track.Momentum();
        if(debug>3) Printf("calculated the Momentum of the track ...");
        c_track.SetCalorimetryPDG( trkpidpdg_pandoraNu[j] );
        if(debug>3) Printf("set track pid pdg ...");

        // if its MC, plug also MC information
        if(MCmode){
            if(debug>3) Printf("plugging also MC information:");
            bool FoundMCtrack = false;
            for(Int_t ig4=0; ig4 < geant_list_size && ig4 < MAX_tracks; ig4++) {
                if(debug>3) Printf("trkg4id_pandoraNu[%d] = %d, TrackId[%d] = %d",j,trkg4id_pandoraNu[j],ig4,TrackId[ig4]);
                if(TrackId[ig4] == trkg4id_pandoraNu[j]){
                    // lets start with only the MC pdg code, for training purposes
                    if(debug>3) Printf("truth pdg is: %d",pdg[ig4]);
                    FoundMCtrack = true;
                    c_track.SetMCpdgCode(pdg[ig4]);
                }
            }
            if (!FoundMCtrack) {
                if(debug>3) Printf("could not find g4 information for this track");
                c_track.SetMCpdgCode(-9999);
            }
        }
        else {
            if(debug>3) Printf("this is data, so no MC information");
            c_track.SetMCpdgCode(-9999);
        }
        
        
        tracks.push_back(c_track);
        if(debug>3) Printf("pushed the track into tracks which now has a size %lu...",tracks.size());
    }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void cumputeAnaTree::GetPandoraCosmicTracks(){
    
    if (ntracks_pandoraCosmic==0 || ntracks_pandoraCosmic>1000) return;
    if(debug>2) Printf("loop over pandoraCosmic %d tracks",ntracks_pandoraCosmic);
    
    
    
    // loop over all reconstructed tracks
    for(Int_t j=0; j < ntracks_pandoraCosmic && j < MAX_cosmic_tracks; j++){
        
        InitTrack();
        
        
        c_cosmic_track = PandoraNuTrack(
                                        run                                                                                // run
                                        ,subrun                                                                            // subrun
                                        ,event                                                                             // event
                                        ,trkId_pandoraNu[j]                                                                // track id
                                        ,TVector3(trkstartx_pandoraNu[j], trkstarty_pandoraNu[j], trkstartz_pandoraNu[j])  // start position
                                        ,TVector3(trkendx_pandoraNu[j]  , trkendy_pandoraNu[j]  , trkendz_pandoraNu[j])    // end position
                                        ,trklen_pandoraNu[j]                                                               // track length
                                        ,trktheta_pandoraNu[j]                                                             // theta
                                        ,trkphi_pandoraNu[j]                                                               // phi
                                        );
        
        if (!TrackContained( c_cosmic_track.start_pos   , c_cosmic_track.end_pos )) continue;
        Ncosmictracks ++ ;
        if(debug>3) Printf("created (contained) track %d...",j);
        
        // get flash info
        // compare reconstructed track to list of flashes in beam to find closest
        float tzcenter = (c_cosmic_track.start_pos.z() + c_cosmic_track.end_pos.z())/2.;
        if(debug>3) Printf("with tzcenter = %.2f, goodflashidx.size() = %lu",tzcenter,goodflashidx.size());
        
        if(goodflashidx.size() > 0) {
            int   minzi    = goodflashidx.at(0);
            float minzdiff = TMath::Abs(flash_zcenter[minzi] - tzcenter);
            if(debug>3) {SHOW(minzi);SHOW(minzdiff);}
            for(size_t k=0; k < goodflashidx.size(); k++)
            {
                int   fidx     = goodflashidx.at(k);
                float fzcenter = flash_zcenter[fidx];
                if(debug>3) {SHOW(fidx);SHOW(fzcenter);}
                if(TMath::Abs(fzcenter - tzcenter) < minzdiff)
                {
                    minzi    = fidx;
                    minzdiff = TMath::Abs(fzcenter = tzcenter);
                }
            }
            cftime      = flash_time[minzi];
            cftimewidth = flash_timewidth[minzi];
            cfzcenter   = flash_zcenter[minzi];
            cfzwidth    = flash_zwidth[minzi];
            cfycenter   = flash_ycenter[minzi];
            cfywidth    = flash_ywidth[minzi];
            cftotalpe   = flash_pe[minzi];
            cfdistance  = tzcenter - cfzcenter;
        }
        else {
            cftime = cftimewidth = cfzcenter = cfzwidth = cfycenter = cfywidth = cftotalpe = cfdistance = -9999;
        }
        c_cosmic_track.SetFlashInfo(cftime , cftimewidth , cfzcenter , cfzwidth , cfycenter , cfywidth , cftotalpe , cfdistance);
        if(debug>3) Printf("Set Flash Info...");
        
        
        // get cosmic scores
        c_cosmic_track.SetCosScores( trkcosmicscore_tagger_pandoraCosmic[j][0] , trkcosmicscore_containmenttagger_pandoraCosmic[j][0] );
        // get pid info
        c_cosmic_track.Set_pid_info( trkpidpida_pandoraCosmic[j][trkpidbestplane_pandoraCosmic[j]] , trkpidchi_pandoraCosmic[j][trkpidbestplane_pandoraCosmic[j]] );
        
        
        // get dqdx info: loop over range from end of track to find start and end
        int   rmin[3] , rmax[3];
        if(debug>3) Printf("before for(Int_t fr=0; fr<3;fr++) ...");
        for(Int_t fr=0; fr<3;fr++) {
            
            if(ntrkhits_pandoraCosmic[j][fr] >= 0) {
                
                nhits     += ntrkhits_pandoraCosmic[j][fr];
                rmin[fr]   = rmax[fr]   = trkresrg_pandoraCosmic[j][fr][0];
                totaldqdx += trkdqdx_pandoraCosmic[j][fr][0];
                int minidx = 0 , maxidx = 0;
                
                for(Int_t ridx=0; ridx < ntrkhits_pandoraCosmic[j][fr]; ridx++) {
                    if(trkresrg_pandoraCosmic[j][fr][ridx] < rmin[fr] && trkdqdx_pandoraCosmic[j][fr][ridx] != 0) {
                        rmin[fr] = trkresrg_pandoraCosmic[j][fr][ridx];
                        minidx   = ridx;
                    }
                    if(trkresrg_pandoraCosmic[j][fr][ridx] > rmax[fr]) {
                        rmax[fr] = trkresrg_pandoraCosmic[j][fr][ridx];
                        maxidx   = ridx;
                    }
                    totaldqdx += trkdqdx_pandoraCosmic[j][fr][ridx];
                }
                if(maxidx >= 3) {
                    startdqdx   += (trkdqdx_pandoraCosmic[j][fr][maxidx] + trkdqdx_pandoraCosmic[j][fr][maxidx-1]
                                    + trkdqdx_pandoraCosmic[j][fr][maxidx-2]);
                    enddqdx     += (trkdqdx_pandoraCosmic[j][fr][minidx] + trkdqdx_pandoraCosmic[j][fr][minidx+1]
                                    + trkdqdx_pandoraCosmic[j][fr][minidx+2]);
                } else {
                    startdqdx   += trkdqdx_pandoraCosmic[j][fr][maxidx];
                    enddqdx     += trkdqdx_pandoraCosmic[j][fr][minidx];
                }
            }
        }
        if(debug>3) Printf("after for(Int_t fr=0; fr<3;fr++) ...");
        c_cosmic_track.Set_dqdx( startdqdx , enddqdx , totaldqdx , nhits );
        if(debug>3) Printf("Set dq/dx ...");
        c_cosmic_track.CreateROIs();
        if(debug>3) Printf("Created ROIs...");
        c_cosmic_track.Calorimetry();
        if(debug>3) Printf("made some Calorimetry ...");
        c_cosmic_track.Straightness();
        if(debug>3) Printf("calculated the Straightness of the track ...");
        c_cosmic_track.Momentum();
        if(debug>3) Printf("calculated the Momentum of the track ...");
        c_cosmic_track.SetCalorimetryPDG( trkpidpdg_pandoraCosmic[j] );
        if(debug>3) Printf("set track pid pdg ...");
        
        // if its MC, plug also MC information
        if(MCmode){
            if(debug>3) Printf("plugging also MC information:");
            bool FoundMCtrack = false;
            for(Int_t ig4=0; ig4 < geant_list_size && ig4 < MAX_tracks; ig4++) {
                if(debug>3) Printf("trkg4id_pandoraCosmic[%d] = %d, TrackId[%d] = %d",j,trkg4id_pandoraCosmic[j],ig4,TrackId[ig4]);
                if(TrackId[ig4] == trkg4id_pandoraCosmic[j]){
                    // lets start with only the MC pdg code, for training purposes
                    if(debug>3) Printf("truth pdg is: %d",pdg[ig4]);
                    FoundMCtrack = true;
                    c_cosmic_track.SetMCpdgCode(pdg[ig4]);
                }
            }
            if (!FoundMCtrack) {
                if(debug>3) Printf("could not find g4 information for this track");
                c_cosmic_track.SetMCpdgCode(-9999);
            }
        }
        else {
            if(debug>3) Printf("this is data, so no MC information");
            c_cosmic_track.SetMCpdgCode(-9999);
        }
        
        
        cosmic_tracks.push_back(c_cosmic_track);
        if(debug>3) Printf("pushed the track into tracks which now has a size %lu...",cosmic_tracks.size());
    }
}


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void cumputeAnaTree::CollectTrackVertices(){
    
    // (1) collect all start AND end points of all pandoraNu tracks vertices
    // since we can not rely on pandoraNu to know where is the starting and
    // ending position of the track, we have to do it on our own
    for (auto t:cosmic_tracks) {
        tracks_vertices.push_back( track_vertex( run , subrun , event , t.track_id , -(100 + t.track_id) , "start" , t.start_pos , t.CalorimetryPDG , t.roi ) );
        tracks_vertices.push_back( track_vertex( run , subrun , event , t.track_id ,  (100 + t.track_id) , "end"   , t.end_pos   , t.CalorimetryPDG , t.roi ) );
    }
    
    // (2) compute the distance of each vertex from all other vertices
    // its not really important, since this routine is prformed elsewhere and in another manner
    for (auto & c_track_vertex : tracks_vertices) {
        c_track_vertex.SetDistancesFromVertices ( tracks_vertices );
    }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void cumputeAnaTree::FindMutualVertices(){
    
    
    for (auto track_vertex_i : tracks_vertices) { // loop over vertices i

        if (debug>3) Printf("checking if track_vertex_i.track_id = %d is in mutual_vertices",track_vertex_i.track_id);
        
        // skip this step if we have already included this track vertex in one of the previous loop steps
        track_already_included = false;
        for (auto m_v : mutual_vertices) {
            if (m_v.include_track(track_vertex_i.track_id)) {
                track_already_included = true;
            }
        }
        
        
        if (track_already_included) {
            continue;
        }
        else {
            
            
            // create a new mutual-vertex. Its id = incremented from the previous mutual vertex that was saved
            c_mutual_vertex = mutual_vertex( mutual_vertices.size() + 1 , track_vertex_i );
            
            for ( auto track_vertex_j : tracks_vertices ) { // loop over vertices j
                
                // and take interest if they are not already included in a mutual vertex and they are not track vertex j
                
                if (    ( track_vertex_i.track_id != track_vertex_j.track_id )
                    &&  ( !c_mutual_vertex.include_track(track_vertex_j.track_id) ) ){
                    
                    // if these vertices are close enough, collect them together into a mutual vertex
                    
                    if (( c_mutual_vertex.position - track_vertex_j.position ).Mag() < min_distance_from_vertex ) {
                        
                        c_mutual_vertex.AddTrackVertex (  track_vertex_j );
                        
                    }
                }
            } // loop over vertices j
            
            // plug this mutual vertex into the mutual-vertices vector only if it is indeed mutual - i.e. it includes at least two tracks
            if (c_mutual_vertex.N_tracks_vertices > 1 ) {
                c_mutual_vertex.CreateROIs();
                c_mutual_vertex.SetVertexTopology();
                mutual_vertices.push_back( c_mutual_vertex );
            }
        }
    } // loop over vertices i
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
bool cumputeAnaTree::GetTruthInformation(){
    
    if (debug > 2) Printf("getting truth information");
    
    //loop over neutrino interactions
    if (debug > 2) SHOW(mcevts_truth);
    
    for (Int_t n = 0; n < mcevts_truth && n < kMaxTruth; n++) {
        
        if (debug>2) {
            Printf("mc event %d",n);
            cout << "," << run << "," <<subrun<< "," <<event<< "," <<nuvtxx_truth[n] << "," << nuvtxy_truth[n] << "," << nuvtxz_truth[n]<< "," <<ccnc_truth[n]<< "," <<nuPDG_truth[n]<< "," <<X_truth[n]<< "," << Q2_truth[n] << "..."<< endl;
        }
        
        c_nu_interaction = nuInteraction(
                                        run                                                             // run
                                        ,subrun                                                         // subrun
                                        ,event                                                          // event
                                        ,TVector3(nuvtxx_truth[n] , nuvtxy_truth[n] , nuvtxz_truth[n])  // position
                                        ,ccnc_truth[n]                                                  // CC / NC
                                        ,nuPDG_truth[n]                                                 // pdg code
                                        ,X_truth[n]                                                     // Bjorken x
                                        ,Q2_truth[n]                                                    // momentum transfer
                                        );
        
        if (debug>2) { SHOWTVector3(c_nu_interaction.Vertex ); SHOW(c_nu_interaction.ccnc ); }
        
        if ( VertexContained( c_nu_interaction.Vertex ) ) {
            
            if (debug>3) { cout<<"vertex contained "<< endl;}
            
            // we want CCνµ
            if ( c_nu_interaction.ccnc == 0 && c_nu_interaction.pdg == 14 ){

                if (debug>3) { cout<<"CC nu mu event "<< endl;}

                c_nu_interaction.SetNuMomentum( enu_truth[n] , nu_dcosx_truth[n] , nu_dcosy_truth[n] , nu_dcosz_truth[n] );
                c_nu_interaction.SetLeptonMomentum( lep_mom_truth[n] , lep_dcosx_truth[n] , lep_dcosy_truth[n] , lep_dcosz_truth[n] );
                c_nu_interaction.Set_q_vector();
                nu_interactions.push_back(c_nu_interaction);
                
                GetGENIEInformation();
                
                NnuInteractions++;

                
            }// end of if interaction is CCνµ
            
        }// end of if interaction contained
        
    }//end of neutrino interaction loop

    return true;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
bool cumputeAnaTree::GetGENIEInformation(){
    if (debug > 3) Printf("getting GENIE information");
    
    GENIEinteraction c_genie_interaction( genie_no_primaries , nu_interactions.back().nu );
    
    for ( Int_t primary = 0 ; primary < genie_no_primaries ; primary ++ ) {
        c_genie_interaction.AddPrimary(
                                     genie_pdg[primary]
                                     ,genie_Eng[primary]
                                     ,genie_Px[primary]
                                     ,genie_Py[primary]
                                     ,genie_Pz[primary]
                                     ,genie_P[primary]
                                     ,genie_status_code[primary]
                                     ,genie_mass[primary]
                                     ,genie_trackID[primary]
                                     ,genie_ND[primary]
                                     ,genie_mother[primary]
                                     );
    }
    
    if(debug>3) Printf("added primaries");
    c_genie_interaction.SortNucleons();
    if(debug>3) Printf("sorted nucleons");
    c_genie_interaction.ComputeKinematics();
    if(debug>3) Printf("computed kinematics");
    c_genie_interaction.ComputePmissPrec();
    if(debug>3) Printf("computed p(miss) and p(rec)");
    
    genie_interaction = c_genie_interaction;
    GENIETree -> Fill();
    genie_interactions.push_back( c_genie_interaction );
    return true;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
bool cumputeAnaTree::VertexContained(TVector3 v){
    
    if (debug>4) {Printf("checking if contained: "); SHOW3(v.x(),v.y(),v.z());}
    // check if contained
    if( ( v.x() < 3.45 )    | ( v.x() > 249.8 ) )   return false;
    if( ( v.y() < -110.53 ) | ( v.y() > 112.47 ) )  return false;
    if( ( v.z() < 5.1 )     | ( v.z() > 1031.9 ) )  return false;
    return true;
    
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
bool cumputeAnaTree::TrackContained(TVector3 start , TVector3 end){
    
    if( ! VertexContained( start ) )   return false;
    if( ! VertexContained( end ) )      return false;
    return true;
    
}


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void cumputeAnaTree::FindMuonScattering(){
    foundMuonScattering = false;
    for (auto c_mutual_vertex: mutual_vertices) {
        if ( c_mutual_vertex.Nmu >= 2 ) {
            foundMuonScattering = true;
        }
    }
}


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
Float_t cumputeAnaTree::TrkVtxDistance ( Int_t ivtx , Int_t itrk ){
    
    // pandoraNu vertices
    TVector3 vtx_position = TVector3( vtxx_pandoraNu[ivtx] , vtxy_pandoraNu[ivtx] , vtxz_pandoraNu[ivtx] );
    Float_t dis_vtx_start = 1000 , dis_vtx_end = 1000;
    for ( auto track : tracks ) {
        if ( track.track_id == itrk ) {
            dis_vtx_start = (track.start_pos - vtx_position).Mag();
            dis_vtx_end   = (track.end_pos - vtx_position).Mag();
            break;
        }
    }
    Float_t d = std::min( dis_vtx_start , dis_vtx_end );
    if (debug > 2) Printf("track %d distacne from vertex %d is %.2f cm", itrk , ivtx , d );
    return d;
}


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void cumputeAnaTree::CreateNuSelVtxROI( Int_t ivtx ){
    
    TVector3 vtx_position = TVector3( vtxx_pandoraNu[ivtx] , vtxy_pandoraNu[ivtx] , vtxz_pandoraNu[ivtx] );
    
    
    // load GeometryHelper utility
    auto geomHelper = ::larutil::GeometryHelper::GetME();
    
    double start_xyz[3] = {vtx_position.x() + 20 , vtx_position.y() + 20 , vtx_position.z() + 20 };
    double end_xyz[3]   = {vtx_position.x() + 20 , vtx_position.y() - 20 , vtx_position.z() - 20 };
    
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
        NuSelVtxROI[plane] = box( start_wire , start_time , end_wire , end_time );
        
    }
    

    
}


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
bool cumputeAnaTree::FillOutTree (){
    
    OutTree -> Fill();
    return true;
    
}



//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void cumputeAnaTree::Write2CSV(){
    
    // run              , subrun            , event
    // U_start_wire     , U_start_time      , U_end_wire  , U_end_time
    // V_start_wire     , V_start_time      , V_end_wire  , V_end_time
    // Y_start_wire     , Y_start_time      , Y_end_wire  , Y_end_time
    
    
    
    
    if(!mutual_vertices.empty()){
        for (auto m_v:mutual_vertices){
            csvfile
            << m_v.run                       << " " << m_v.subrun             << " " << m_v.event
            << " " << m_v.roi[0].start_wire  << " " << m_v.roi[0].start_time  << " " << m_v.roi[0].end_wire    << " " << m_v.roi[0].end_time
            << " " << m_v.roi[1].start_wire  << " " << m_v.roi[1].start_time  << " " << m_v.roi[1].end_wire    << " " << m_v.roi[1].end_time
            << " " << m_v.roi[2].start_wire  << " " << m_v.roi[2].start_time  << " " << m_v.roi[2].end_wire    << " " << m_v.roi[2].end_time
            << endl;
            
            if (debug>2) cout << "wrote mutual vertex " << m_v.vertex_id << " to csv output file " << endl;
        }
    }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void cumputeAnaTree::PrintData(int entry){
    
    EndEventBlock();
    printf("\t[%.1f%%]\t",100.*(float)entry/Nentries);
    SHOW(entry);
    SHOW3(run , subrun , event);
    if(!nu_interactions.empty()){
        cout << "\033[33m" << "xxxxxxxxxxxxxx\n\n" << nu_interactions.size() << " neutrino interactions\n\n" << "xxxxxxxxxxxxxx\n\n"<< "\033[37m" << endl;
        for (auto nu_interaction: nu_interactions) {
            nu_interaction.Print();
        }
    }
    if(!genie_interactions.empty()){
        cout << "\033[33m" << "xxxxxxxxxxxxxx\n\n" << genie_interactions.size() << " genie interactions\n\n" << "xxxxxxxxxxxxxx\n\n"<< "\033[37m" << endl;
        for (auto genie_interaction: genie_interactions) {
            genie_interaction.Print();
        }
    }
    if(!cosmic_tracks.empty()){
        cout << "\033[33m" << "xxxxxxxxxxxxxx\n\n" << cosmic_tracks.size() << " pandoraCosmic tracks\n\n" << "xxxxxxxxxxxxxx\n\n"<< "\033[37m" << endl;
        
        for (auto t: cosmic_tracks) {
            t.Print();
        }
    }
    if(!tracks.empty()){
        cout << "\033[33m" << "xxxxxxxxxxxxxx\n\n" << tracks.size() << " pandoraNu tracks\n\n" << "xxxxxxxxxxxxxx\n\n"<< "\033[37m" << endl;
        
        for (auto t: tracks) {
            t.Print();
        }
    }
    if(!tracks_vertices.empty()){
        cout << "\033[33m" << "xxxxxxxxxxxxxx\n\n" << tracks_vertices.size() << " tracks vertices\n\n" << "xxxxxxxxxxxxxx\n\n"<< "\033[37m" << endl;
        
        for (auto v: tracks_vertices) {
            v.Print();
        }
    }
    if(!mutual_vertices.empty()){
        cout << "\033[33m" << "xxxxxxxxxxxxxx\n\n" << mutual_vertices.size() << " mutual vertices\n\n" << "xxxxxxxxxxxxxx\n\n"<< "\033[37m" << endl;
        for (auto v: mutual_vertices) {
            v.Print();
        }
    }
    
    EndEventBlock();
    
}


#endif
