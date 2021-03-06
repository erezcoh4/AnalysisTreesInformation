#ifndef cumputeAnaTree_CXX
#define cumputeAnaTree_CXX

#include "cumputeAnaTree.h"


// main event loop
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
bool cumputeAnaTree::extract_information(bool fDo){ // main event loop....
    //    GetSoftwareTrigger();
    //    if (debug>3) Printf("Got Software Trigger");
    
    GetInTimeFlashes();
    Debug(3,"Got In Time Flashes");
    
    GetPandoraNuTracks();
    Debug(3,"Got PandoraNu Tracks");
    CollectHits();
    //    AssociateHitsTracks();
    Debug(3,"collected hits");

    if (DoPandoraCosmic){
        GetPandoraCosmicTracks();
        Debug(3,"Got PandoraCosmic Tracks");
    }
    
    if (MCmode){
        GetTruthInformation();
        Debug(3,"Got Truth Information");
        TagCC1pTracks();
        TagCC_1p_200MeVc_0pi_Tracks();
    }

    // if we want to collect vertices, these should be uncommented in
    if (option.Contains("find common muon-proton vertices")){
        CollectTrackVertices();
        FindMutualVertices();
        FindMuonScattering();
    }
    
    return true;
}


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
cumputeAnaTree::cumputeAnaTree( TTree * fOutTree, // TTree * fInTree,
                               TString foption, int fdebug,
                               bool fMCmode, TTree * fGENIETree,
                               bool fDoPandoraCosmic, bool fDo_dEdx, int fMCCV){
    
    //    SetInTree(fInTree);
    SetOutTree(fOutTree);
    SetOption(foption);
    SetDebug(fdebug);
    SetMCMode(fMCmode);
    if (MCmode) SetGENIETree(fGENIETree);
    SetDoPandoraCosmic(fDoPandoraCosmic);
    SetDo_dEdx(fDo_dEdx);
    SetMCCV(fMCCV);
    //    InitInputTree();
    InitOutputTree();
    if (debug>1) Printf("option:%s, debug:%d, MCmode:%d, DoPandoraCosmic:%d",option.Data(),debug,MCmode,DoPandoraCosmic);
    

    for ( Float_t ymax_cosmictrack = 120.0; ymax_cosmictrack > 110.0 ; ymax_cosmictrack-=0.5 ){
        ymax_cosmictracks.push_back(ymax_cosmictrack);
    }
    Nymax_cosmic = (int)ymax_cosmictracks.size();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void cumputeAnaTree::InitInputTree(){
    
    // event info
    InTree -> SetBranchAddress("run"                                            , &run);
    InTree -> SetBranchAddress("subrun"                                         , &subrun);
    InTree -> SetBranchAddress("event"                                          , &event);
    InTree -> SetBranchAddress("pot"                                            , &pot);

    // Kalman-Hit
    InTree -> SetBranchAddress("ntracks_trackkalmanhit"                         , &ntracks_trackkalmanhit);
    
    
    // pandoraNu
    // ----------------------------------------------------------------------------------------------
    InTree -> SetBranchAddress("ntracks_pandoraNu"                              , &ntracks_pandoraNu);
    InTree -> SetBranchAddress("trkId_pandoraNu"                                , &trkId_pandoraNu);
    InTree -> SetBranchAddress("trklen_pandoraNu"                               , &trklen_pandoraNu);
    InTree -> SetBranchAddress("trkstartx_pandoraNu"                            , &trkstartx_pandoraNu);
    InTree -> SetBranchAddress("trkstarty_pandoraNu"                            , &trkstarty_pandoraNu);
    InTree -> SetBranchAddress("trkstartz_pandoraNu"                            , &trkstartz_pandoraNu);

    InTree -> SetBranchAddress("trkmomrange_pandoraNu"                          , &trkmomrange_pandoraNu);
    InTree -> SetBranchAddress("trkmommsllhd_pandoraNu"                         , &trkmommsllhd_pandoraNu);
    
    InTree -> SetBranchAddress("trkendx_pandoraNu"                              , &trkendx_pandoraNu);
    InTree -> SetBranchAddress("trkendy_pandoraNu"                              , &trkendy_pandoraNu);
    InTree -> SetBranchAddress("trkendz_pandoraNu"                              , &trkendz_pandoraNu);
    InTree -> SetBranchAddress("trktheta_pandoraNu"                             , &trktheta_pandoraNu);
    InTree -> SetBranchAddress("trkphi_pandoraNu"                               , &trkphi_pandoraNu);
    InTree -> SetBranchAddress("ntrkhits_pandoraNu"                             , &ntrkhits_pandoraNu);
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

    // calorimetery
    InTree -> SetBranchAddress("trkxyz_pandoraNu"                               , &trkxyz_pandoraNu);
    InTree -> SetBranchAddress("trkdqdx_pandoraNu"                              , &trkdqdx_pandoraNu);
    InTree -> SetBranchAddress("trkdedx_pandoraNu"                              , &trkdedx_pandoraNu);
    
    
    // Hits
    InTree -> SetBranchAddress("no_hits"                                        , &no_hits);
    InTree -> SetBranchAddress("hit_plane"                                      , &hit_plane);
    InTree -> SetBranchAddress("hit_wire"                                       , &hit_wire);
    InTree -> SetBranchAddress("hit_peakT"                                      , &hit_peakT);
    InTree -> SetBranchAddress("hit_charge"                                     , &hit_charge);
    InTree -> SetBranchAddress("hit_trkid"                                      , &hit_trkid);
    InTree -> SetBranchAddress("hit_trkKey"                                     , &hit_trkKey);
    
    
    // optical info
    if (MCCV==7){
        InTree -> SetBranchAddress("no_flashes"                                     , &no_flashes);
        InTree -> SetBranchAddress("flash_time"                                     , &flash_time);
        InTree -> SetBranchAddress("flash_timewidth"                                , &flash_timewidth);
        InTree -> SetBranchAddress("flash_pe"                                       , &flash_pe);
        InTree -> SetBranchAddress("flash_ycenter"                                  , &flash_ycenter);
        InTree -> SetBranchAddress("flash_ywidth"                                   , &flash_ywidth);
        InTree -> SetBranchAddress("flash_zcenter"                                  , &flash_zcenter);
        InTree -> SetBranchAddress("flash_zwidth"                                   , &flash_zwidth);
    }
    else if (MCCV==8){
        InTree -> SetBranchAddress("nfls_simpleFlashBeam"                           , &nfls_simpleFlashBeam);
        InTree -> SetBranchAddress("flsTime_simpleFlashBeam"                        , &flash_time);
        InTree -> SetBranchAddress("flsPe_simpleFlashBeam"                          , &flash_pe);
        InTree -> SetBranchAddress("flsYcenter_simpleFlashBeam"                     , &flash_ycenter);
        InTree -> SetBranchAddress("flsYwidth_simpleFlashBeam"                      , &flash_ywidth);
        InTree -> SetBranchAddress("flsZcenter_simpleFlashBeam"                     , &flash_zcenter);
        InTree -> SetBranchAddress("flsZwidth_simpleFlashBeam"                      , &flash_zwidth);
    }
    
    
    // vertex
    InTree -> SetBranchAddress("nvtx_pandoraNu"                                 , &nvtx_pandoraNu);
    InTree -> SetBranchAddress("vtxx_pandoraNu"                                 , &vtxx_pandoraNu);
    InTree -> SetBranchAddress("vtxy_pandoraNu"                                 , &vtxy_pandoraNu);
    InTree -> SetBranchAddress("vtxz_pandoraNu"                                 , &vtxz_pandoraNu);


    
    // pandoraCosmic
    // ----------------------------------------------------------------------------------------------
    if (DoPandoraCosmic){
        
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
        
        // calorimetery
        InTree -> SetBranchAddress("trkxyz_pandoraNu"                               , &trkxyz_pandoraCosmic);
        InTree -> SetBranchAddress("trkdqdx_pandoraNu"                              , &trkdqdx_pandoraCosmic);
        InTree -> SetBranchAddress("trkdedx_pandoraNu"                              , &trkdedx_pandoraCosmic);
        
        // vertex
        InTree -> SetBranchAddress("nvtx_pandoraCosmic"                             , &nvtx_pandoraCosmic);
        InTree -> SetBranchAddress("vtxx_pandoraCosmic"                             , &vtxx_pandoraCosmic);
        InTree -> SetBranchAddress("vtxy_pandoraCosmic"                             , &vtxy_pandoraCosmic);
        InTree -> SetBranchAddress("swtrigger_name"                                 , &swtrigger_name);
        InTree -> SetBranchAddress("swtrigger_triggered"                            , &swtrigger_triggered);
        InTree -> SetBranchAddress("vtxz_pandoraCosmic"                             , &vtxz_pandoraCosmic);
    }


    
    // MC information
    // ----------------------------------------------------------------------------------------------
    if (MCmode) {
        
        if (debug>1)  Printf ("\n\nrunning on MC mode...\n\n");
        // geant information
        InTree -> SetBranchAddress("geant_list_size"                            , &geant_list_size);
        InTree -> SetBranchAddress("no_primaries"                               , &no_primaries);
        InTree -> SetBranchAddress("trkg4id_pandoraNu"                          , &trkg4id_pandoraNu);
        InTree -> SetBranchAddress("TrackId"                                    , &TrackId);
        InTree -> SetBranchAddress("pdg"                                        , &pdg);
        InTree -> SetBranchAddress("P"                                          , &P);
        InTree -> SetBranchAddress("Mass"                                       , &Mass);
        InTree -> SetBranchAddress("Eng"                                        , &Eng);
        InTree -> SetBranchAddress("theta"                                      , &theta);
        InTree -> SetBranchAddress("phi"                                        , &phi);
        InTree -> SetBranchAddress("pathlen"                                    , &pathlen);
        InTree -> SetBranchAddress("process_primary"                            , &process_primary);
        
        InTree -> SetBranchAddress("Mother"                                     , &Mother);
        InTree -> SetBranchAddress("EndPointx"                                  , &EndPointx);
        InTree -> SetBranchAddress("EndPointy"                                  , &EndPointy);
        InTree -> SetBranchAddress("EndPointz"                                  , &EndPointz);
        InTree -> SetBranchAddress("StartPointx"                                , &StartPointx);
        InTree -> SetBranchAddress("StartPointy"                                , &StartPointy);
        InTree -> SetBranchAddress("StartPointz"                                , &StartPointz);
        
        // purity
        InTree -> SetBranchAddress("trkpurtruth_pandoraNu"                      , &trkpurtruth);

        
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
    // OutTree -> ResetBranchAddresses	();
    OutTree -> Branch("run"             ,&run               ,"run/I");
    OutTree -> Branch("subrun"          ,&subrun            ,"subrun/I");
    OutTree -> Branch("event"           ,&event             ,"event/I");
    // OutTree -> Branch("pot"             ,&pot               ,"pot/D");
    
    OutTree -> Branch("Ntracks"         ,&Ntracks           ,"Ntracks/I"); // number of contained tracks, not ntracks_pandoraNu...
    OutTree -> Branch("Ng4particles"    ,&Ng4particles      ,"Ng4particles/I"); // number of g4 particles
    
    // OutTree -> Branch("nu_interactions"     ,&nu_interactions); // neutrino interactions...
    OutTree -> Branch("tracks"              ,&tracks); // tracks information...
    OutTree -> Branch("g4particles"         ,&g4particles); // g4 information...
    
    OutTree -> Branch("hits"  ,&hits); // reconstructed hits, to build an event-display and understand things...

    if (MCmode){
        OutTree -> Branch("genie_interactions"  ,&genie_interactions); // genie interactions...
        
        // GENIETree -> ResetBranchAddresses	();
        GENIETree -> Branch("run"             ,&run               ,"run/I");
        GENIETree -> Branch("subrun"          ,&subrun            ,"subrun/I");
        GENIETree -> Branch("event"           ,&event             ,"event/I");
        // GENIETree -> Branch("hits"            ,&hits);
        // GENIETree -> Branch("pot"             ,&pot               ,"pot/D");
        GENIETree -> Branch("genie_interactions"  ,&genie_interactions); // genie interactions in a seperate tree...
    }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void cumputeAnaTree::GetEntry (int entry){
    InitEntry();
    InTree -> GetEntry(entry);
}


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void cumputeAnaTree::GetSoftwareTrigger(){
    for (size_t trigger = 0; trigger < (size_t)(sizeof(swtrigger_name)/sizeof(*swtrigger_name)); trigger++) {
        if (debug>2){
            cout << "SW Trigger name: " << swtrigger_name[trigger] << endl;
            cout << " -> was triggered? " << swtrigger_triggered[trigger] << endl;
        }
    }
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
    if (!g4particles.empty())           g4particles.clear();
    if (!hits.empty())                  hits.clear();
    Ntracks = 0;
    Ncosmictracks = mcevts_truth = 0;
    NnuInteractions = ntracks_pandoraNu = ntracks_pandoraCosmic = Ng4particles = 0;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void cumputeAnaTree::InitTrack(){
    totaldqdx  = startdqdx = enddqdx = nhits = 0;
    
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void cumputeAnaTree::GetInTimeFlashes(){
    
    if (MCCV==8) no_flashes = nfls_simpleFlashBeam;
    
    if(debug>3) Printf("GetInTimeFlashes of %d flashes",no_flashes);
    
    // get a list of in-time flashes for the event
    if(no_flashes > 0) {
        
        if (!goodflashidx.empty())  goodflashidx.clear();
        
        if(debug>3) { SHOW(goodflashidx.size()); Printf("looping in if(no_flashes > 0) on no_flashes"); }
        
        for(int i=0; i < no_flashes && i < kMaxHits ; i++){
            
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
    Debug(3,Form("LoopPanNuTracks on %d tracks",ntracks_pandoraNu));
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
        c_track.IsStartContained = VertexContained( c_track.start_pos );
        c_track.IsEndContained = VertexContained( c_track.end_pos );
        c_track.IsFullyContained = TrackContained( c_track.start_pos , c_track.end_pos );
        
        if (!c_track.IsStartContained && !c_track.IsEndContained){
            Debug(3,Form("track %d is not contained at all {(%.1f,%.1f,%.1f)->(%.1f,%.1f,%.1f)}...",j,c_track.start_pos.x(),c_track.start_pos.y(),c_track.start_pos.z(),c_track.end_pos.x(),c_track.end_pos.y(),c_track.end_pos.z()));
            continue;
        }
        Ntracks ++ ;
        Debug(4,Form("created (contained) track %d...",j));
        
        // get flash info
        // compare reconstructed track to list of flashes in beam to find closest
        float tzcenter = (c_track.start_pos.z() + c_track.end_pos.z())/2.;
        float tzstart = c_track.start_pos.z() , cfdistance_start = -9999;
        Debug(4,Form("with tzcenter = %.2f, goodflashidx.size() = %lu",tzcenter,goodflashidx.size()));
        
        if(goodflashidx.size() > 0) {
            
            // minimal distance between the closest flash and the track - center
            int   minzi    = goodflashidx.at(0);
            float minzdiff = TMath::Abs(flash_zcenter[minzi] - tzcenter);
            if(debug>3) {SHOW(minzi);SHOW(minzdiff);}
            
            // closest flash to the start-point of the track
            // for later - we would maybe use it to find the closest flash to a vertex
            int   cfdistance_start_idx = goodflashidx.at(0);
            cfdistance_start = TMath::Abs(flash_zcenter[cfdistance_start_idx] - tzstart);

            for(size_t k=0; k < goodflashidx.size(); k++)
            {
                int   fidx     = goodflashidx.at(k);
                float fzcenter = flash_zcenter[fidx];
                if(debug>3) {SHOW(fidx);SHOW(fzcenter);}
                if(TMath::Abs(fzcenter - tzcenter) < minzdiff)
                {
                    minzi    = fidx;
                    minzdiff = TMath::Abs(fzcenter - tzcenter);
                }
                
                // closest flash to the start-point of the track
                int   cfdistance_start_idx = goodflashidx.at(k);
                float distnace = TMath::Abs(flash_zcenter[cfdistance_start_idx] - tzstart);
                if( distnace < cfdistance_start)
                {
                    minzi    = fidx;
                    cfdistance_start = distnace;
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
        c_track.SetCF2Start(cfdistance_start);
        Debug(4,"Set Flash Info...");
        
        
        // get cosmic scores
        c_track.SetCosScores( trkcosmicscore_tagger_pandoraNu[j][0] , trkcosmicscore_containmenttagger_pandoraNu[j][0] );
        // get pid info
        c_track.Set_pid_info( trkpidpida_pandoraNu[j][trkpidbestplane_pandoraNu[j]] , trkpidchi_pandoraNu[j][trkpidbestplane_pandoraNu[j]] );
        
        
        // get dqdx info: loop over range from end of track to find start and end
        int   rmin[3] , rmax[3];
        float dx, dQtotal=0;
        Debug(4,"before for(Int_t fr=0; fr<3;fr++) ...");
        TVector3 current_hit_position , last_hit_position;
        for(Int_t fr=0; fr<3;fr++) {
            
            if(ntrkhits_pandoraNu[j][fr] >= 0) {
                
                nhits     += ntrkhits_pandoraNu[j][fr];
                rmin[fr]   = rmax[fr]   = trkresrg_pandoraNu[j][fr][0];
                totaldqdx += trkdqdx_pandoraNu[j][fr][0];
                current_hit_position = TVector3(trkxyz_pandoraNu[j][fr][0][0],trkxyz_pandoraNu[j][fr][0][1],trkxyz_pandoraNu[j][0][fr][2]);
                last_hit_position = current_hit_position;
                int minidx = 0 , maxidx = 0;
                
                for(Int_t ridx=0; ridx < ntrkhits_pandoraNu[j][fr]; ridx++) {
                    current_hit_position = TVector3(trkxyz_pandoraNu[j][fr][ridx][0],trkxyz_pandoraNu[j][fr][ridx][1],trkxyz_pandoraNu[j][fr][ridx][2]);
                    
                    if(trkresrg_pandoraNu[j][fr][ridx] < rmin[fr] && trkdqdx_pandoraNu[j][fr][ridx] != 0) {
                        rmin[fr] = trkresrg_pandoraNu[j][fr][ridx];
                        minidx   = ridx;
                    }
                    if(trkresrg_pandoraNu[j][fr][ridx] > rmax[fr]) {
                        rmax[fr] = trkresrg_pandoraNu[j][fr][ridx];
                        maxidx   = ridx;
                    }
                    totaldqdx += trkdqdx_pandoraNu[j][fr][ridx]; // in [ADC/cm]
                    dx = (current_hit_position-last_hit_position).Mag(); // in [cm]
//                    SHOW(trkdqdx_pandoraNu[j][fr][ridx]);
//                    SHOW(dx);
                    dQtotal += trkdqdx_pandoraNu[j][fr][ridx] * dx; // in [ADC]
//                    SHOW(dQtotal);
                    last_hit_position = current_hit_position;
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
        Debug(4,"after for(Int_t fr=0; fr<3;fr++) ...");
        c_track.Set_dqdx( startdqdx , enddqdx , totaldqdx , nhits );
        c_track.dQtotal = dQtotal;
        Debug(4,"Set dq/dx ...");
 
        if (Do_dEdx) GetEnergyDeposition( j );
        
        c_track.CreateROIs();
        Debug(4,"Created ROIs...");
        c_track.Calorimetry();
        Debug(4,"made some Calorimetry ...");
        c_track.Straightness();
        Debug(4,"calculated the Straightness of the track ...");
        c_track.SetMomentum( trkmomrange_pandoraNu[j] , trkmommsllhd_pandoraNu[j] );
        // Debug(4,"calculated the Momentum of the track ...");
        c_track.SetCalorimetryPDG( trkpidpdg_pandoraNu[j] );
        Debug(4,"set track pid pdg ...");

 
        
        // if its MC, plug also MC information
        if(MCmode){
            if(debug>3) Printf("plugging also MC information:");
            bool FoundMCtrack = false;
            for(Int_t ig4=0; ig4 < geant_list_size && ig4 < MAX_tracks && !FoundMCtrack ; ig4++) {
                
                if(debug>3) Printf("trkg4id_pandoraNu[%d] = %d, TrackId[%d] = %d",j,trkg4id_pandoraNu[j],ig4,TrackId[ig4]);
                if(TrackId[ig4] == trkg4id_pandoraNu[j]){
                    // pdg code, for training and testing purposes
                    if(debug>3) Printf("truth pdg is: %d",pdg[ig4]);
                    FoundMCtrack = true;
                    c_track.SetMCpdgCode(pdg[ig4]);
                    c_track.SetProcessPrimary(process_primary[ig4]);
                    // energy, momentum....
                    c_track.truth_P     = P[ig4];
                    c_track.truth_Eng   = Eng[ig4];
                    c_track.truth_Mass  = Mass[ig4];
                    c_track.truth_KE    = Eng[ig4] - Mass[ig4];
                    // angles
                    c_track.truth_theta = theta[ig4];
                    c_track.truth_phi   = phi[ig4];
                    // length
                    c_track.truth_length= pathlen[ig4];
                    // positions
                    c_track.SetTruthStartPos( TVector3( StartPointx[ig4] , StartPointy[ig4] , StartPointz[ig4] ) );
                    c_track.SetTruthEndPos( TVector3( EndPointx[ig4] , EndPointy[ig4] , EndPointz[ig4] ) );
                    // does it match a truth neutrino interaction?
                    for (Int_t n = 0; n < mcevts_truth && n < kMaxTruth ; n++) {
                        if (   fabs(nuvtxx_truth[n]-StartPointx[ig4])<2
                            && fabs(nuvtxy_truth[n]-StartPointy[ig4])<2
                            && fabs(nuvtxz_truth[n]-StartPointz[ig4])<2 ){
                            c_track.truth_ccnc = ccnc_truth[n];
                        }
                    }
                }
            }
            if (!FoundMCtrack) {
                if(debug>3) Printf("could not find g4 information for this track");
                c_track.SetMCpdgCode(-9999);
            }
            // purity in each plane
            c_track.SetTrackPurity(trkpurtruth[j][0] , trkpurtruth[j][1] , trkpurtruth[j][2]);
        }
        else {
            if(debug>3) Printf("this is data, so no MC information");
            c_track.SetMCpdgCode(-9999);
            c_track.SetTrackPurity(-9999,-9999,-9999);
        }
        //        // software trigger
        //        c_track.SetSWtrigger(swtrigger_name,swtrigger_triggered);
        
        tracks.push_back(c_track);
        Debug(5,Form("pushed the track into tracks which now has a size %lu...",tracks.size()));
    }
    
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void cumputeAnaTree::CollectHits(){
    for(Int_t j=0 ; j<no_hits && j<kMaxHits ; j++) {
        c_hit = hit ( hit_plane[j] , hit_wire[j] , hit_peakT[j] , hit_charge[j] , j );
        c_hit.hit_trkKey = hit_plane[j];
        hits.push_back(c_hit);
    }
}

////....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//bool cumputeAnaTree::AssociateHitsTracks(){
//    // get dq/dx in two boxes around the track start & end points
//    Debug( 2, Form("associating %d hits to %d tracks start/end boxes",no_hits,Ntracks) );
//    
//    for (auto & c_track : tracks){
//
//        Float_t dqdx_around_start[3] = {0,0,0}, dqdx_around_end[3] = {0,0,0};
//        Float_t dqdx_around_start_total=0, dqdx_around_end_total=0;
//        Float_t dqdx_around_start_track_associated[3] = {0,0,0}, dqdx_around_end_track_associated[3] = {0,0,0};
//        Float_t dqdx_around_start_track_associated_total=0, dqdx_around_end_track_associated_total=0;
//        Float_t dqdx_around_start_no_tracks_associated_total=0, dqdx_around_end_no_tracks_associated_total=0;
//        
//        
//        for(Int_t j=0 ; j<no_hits && j<kMaxHits ; j++) {
//            
//            
//            if ( WireTimeInBox( hit_wire[j] , hit_peakT[j] , c_track.start_box[ hit_plane[j] ] ) ){
//                Debug(6, Form("hit %d (p %d,w %d,t %.2f) inside start-box of track %d, hit_trkid:%d, hit_trkKey:%d",
//                               j,hit_plane[j],hit_wire[j],hit_peakT[j],c_track.track_id,hit_trkid[j], hit_trkKey[j] ));
//                
//                dqdx_around_start[ hit_plane[j] ] += hit_charge[j];
//                dqdx_around_start_total += hit_charge[j];
//                
//                // associate this hit with the track
//                if (c_track.IsWireTimeAlongTrack( hit_plane[j] , hit_wire[j] , hit_peakT[j] ) ){
//                    
//                    Debug( 6 , Form("asociated hit with track %d", c_track.track_id));
//                    dqdx_around_start_track_associated[ hit_plane[j] ] += hit_charge[j];
//                    dqdx_around_start_track_associated_total += hit_charge[j];
//                    
//                }
//                
//            }
//            if ( WireTimeInBox( hit_wire[j] , hit_peakT[j] , c_track.end_box[ hit_plane[j] ] ) ){
//                Debug(6, Form("hit %d (p %d,w %d,t %.2f) inside end-box of track %d, hit_trkid:%d, hit_trkKey:%d",
//                              j,hit_plane[j],hit_wire[j],hit_peakT[j],c_track.track_id,hit_trkid[j], hit_trkKey[j] ));
//
//                dqdx_around_end[ hit_plane[j] ] += hit_charge[j];
//                dqdx_around_end_total += hit_charge[j];
//                
//                // associate this hit with the track
//                if (c_track.IsWireTimeAlongTrack( hit_plane[j] , hit_wire[j] , hit_peakT[j] ) ){
//                    
//                    Debug( 6 , Form("asociated hit with track %d", c_track.track_id));
//                    dqdx_around_end_track_associated[ hit_plane[j] ] += hit_charge[j];
//                    dqdx_around_end_track_associated_total += hit_charge[j];
//                }
//            }
//            
//        }
//        
//        for (int plane=0; plane<3 ;plane++){
//            
//            c_track.dqdx_around_start[plane] = dqdx_around_start[plane];
//            c_track.dqdx_around_start_total = dqdx_around_start_total;
//            c_track.dqdx_around_start_track_associated[plane] = dqdx_around_start_track_associated[plane];
//            c_track.dqdx_around_start_track_associated_total = dqdx_around_start_track_associated_total;
//            
//            c_track.dqdx_around_end[plane] = dqdx_around_end[plane];
//            c_track.dqdx_around_end_total = dqdx_around_end_total;
//            c_track.dqdx_around_end_track_associated[plane] = dqdx_around_end_track_associated[plane];
//            c_track.dqdx_around_end_track_associated_total = dqdx_around_end_track_associated_total;
//            
//        }
//        
//    }
//    Debug(4,"set dq/dx around start and end points...");
//
//}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
bool cumputeAnaTree::WireTimeInBox(int wire, int time, box box){
    if ( box.start_wire < wire && wire < box.end_wire && box.start_time < time && time < box.end_time ){
        return true;
    }
    return false;
}


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void cumputeAnaTree::GetEnergyDeposition( int j ){
    if(debug>3) Printf("starting GetEnergyDeposition...");
    
    for (int plane = 0 ; plane < 3 ; plane ++) {
        if (!residual_range[plane].empty())  residual_range[plane].clear();
        if (!dqdx[plane].empty())            dqdx[plane].clear();
        if (!dEdx[plane].empty())            dEdx[plane].clear();
        if (!Edep[plane].empty())            Edep[plane].clear();
        for(Int_t trkhit=0; trkhit < ntrkhits_pandoraNu[j][plane] ; trkhit++) {
            residual_range[plane].push_back( trkresrg_pandoraNu[j][plane][trkhit] ); // in [cm]
            dqdx[plane].push_back( trkdqdx_pandoraNu[j][plane][trkhit] ); // in [ADC/cm]
            dEdx[plane].push_back( trkdedx_pandoraNu[j][plane][trkhit] ); // in [MeV/cm]
            Edep[plane].push_back( (trkhit==0) ? 0 : (Edep[plane].back() + dEdx[plane].back()*(residual_range[plane].at(trkhit)-residual_range[plane].at(trkhit-1))) ); // in [MeV/cm]
        }
    }
    c_track.SetCalorimetry_Y( residual_range[2] , dEdx[2] , Edep[2] , dqdx[2] );

     if(debug>3) Printf("got dE/dx ...");

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
    // information about these branches can be found at
    // [https://cdcvs.fnal.gov/redmine/projects/dunetpc/repository/revisions/develop/entry/dune/FDSensOpt/NueAna_module.cc]
    // and/or summarized in T.Y slides
    // [https://indico.fnal.gov/getFile.py/access?contribId=1&resId=0&materialId=slides&confId=11056]
    if (debug > 3) Printf("getting geant4 information");
    
    if (debug > 3) SHOW2(geant_list_size , no_primaries);

    for(Int_t ig4=0; ig4 < geant_list_size &&  ig4 < kMaxPrimaries; ig4++){
        
        // threshold out uncontained generated particles tracks or ones with kinetic energy < 10 MeV
        if ( Eng[ig4]-Mass[ig4] < 0.01 )    continue;
        if (!TrackContained( TVector3(StartPointx[ig4] , StartPointy[ig4] , StartPointz[ig4] ) , TVector3(EndPointx[ig4] , EndPointy[ig4] , EndPointz[ig4] ) )) continue;
        // Katherine cuts on tracks shorter than 1.5 cm
        // keep contained tracks with kinetic energy > 10 MeV (we are mainly interested in protons eventually...)
        
        Ng4particles ++ ;
        c_g4particle = LArG4Particle(run ,subrun ,   event ,
                                     ig4 , TrackId[ig4], pdg[ig4],
                                     P[ig4], Eng[ig4], Mass[ig4],
                                     theta[ig4], phi[ig4],
                                     process_primary[ig4],
                                     TVector3(StartPointx[ig4] , StartPointy[ig4] , StartPointz[ig4] ),
                                     TVector3(EndPointx[ig4] , EndPointy[ig4] , EndPointz[ig4] ),
                                     Mother[ig4] ,  ccnc_truth[0]);
        
        // match generated to reconstructed for the same track
        // in a reverse logic to the way we matched reconstructed to generated
        // (1) find track-id match of MC and pandoraNu
        // (2) extract the info from pandoraNu
        if(!tracks.empty()){
            
            for (auto t: tracks) {
                if (t.track_id == TrackId[ig4]){
                    if(debug>3) Printf("t.track_id == TrackId[%d] = %d",ig4,TrackId[ig4]);
                    c_g4particle.reconstructed = 1;
                    c_g4particle.rec_nhits = t.nhits;
                    c_g4particle.rec_is_flipped = t.is_flipped;
                    c_g4particle.SetRecStartPos(t.start_pos);
                    c_g4particle.SetRecEndPos(t.end_pos);
                    c_g4particle.rec_length  = t.length;
                    c_g4particle.rec_theta = t.theta;
                    c_g4particle.rec_phi = t.phi;
                    c_g4particle.rec_distlenratio = t.distlenratio ;
                    c_g4particle.rec_momentum = t.momentum;
                    c_g4particle.rec_start_dqdx = t.start_dqdx;
                    c_g4particle.rec_end_dqdx = t.end_dqdx;
                    c_g4particle.rec_tot_dqdx = t.tot_dqdx;
                    c_g4particle.rec_avg_dqdx = t.avg_dqdx;
                    c_g4particle.rec_dqdx_diff = t.dqdx_diff;
                    c_g4particle.rec_dqdx_ratio = t.dqdx_ratio;
                    c_g4particle.rec_pidpida = t.pidpida;
                    c_g4particle.rec_pidchi = t.pidchi;
                    c_g4particle.rec_cosmicscore = t.cosmicscore;
                    c_g4particle.rec_coscontscore = t.coscontscore;
                    c_g4particle.rec_cftime = t.cftime;
                }
            }
        }
        g4particles.push_back(c_g4particle);
        
    }
    if (debug > 3) SHOW(Ng4particles);
    
    
    if (debug > 3) Printf("getting genie information");
    //loop over neutrino interactions
    if (debug > 3) SHOW(mcevts_truth);
    for (Int_t n = 0; n < mcevts_truth && n < kMaxTruth ; n++) {

        GetGENIEInformation( n );

        if (debug>5) {
            Printf("blah mc event %d",n);
            cout
            << ", run " << run
            << ", subrun " << subrun
            << ", event " << event
            << ", nuvtxx_truth " << nuvtxx_truth[n]
            << ", nuvtxy_truth " << nuvtxy_truth[n]
            << ", nuvtxz_truth " << nuvtxz_truth[n]
            << ", ccnc_truth " << ccnc_truth[n]
            << ", nuPDG_truth " << nuPDG_truth[n]
            << ", X_truth " << X_truth[n]
            << ", Q2_truth " << Q2_truth[n]
            << "..."<< endl;
        }
        
        c_nu_interaction = nuInteraction(
                                        run                                                             // run
                                        ,subrun                                                         // subrun
                                        ,event                                                          // event
                                        ,TVector3(nuvtxx_truth[n] , nuvtxy_truth[n] , nuvtxz_truth[n])  // position
                                        ,ccnc_truth[0]                                                  // CC / NC
                                        ,nuPDG_truth[n]                                                 // pdg code
                                        ,X_truth[n]                                                     // Bjorken x
                                        ,Q2_truth[n]                                                    // momentum transfer
                                        );
        
        
        if (debug>2) { SHOWTVector3(c_nu_interaction.Vertex ); SHOW(c_nu_interaction.ccnc ); }
        
        
        if ( VertexContained( c_nu_interaction.Vertex ) ) {
            
            if (debug>3) { cout<<"vertex contained "<< endl;}
            
            // we want CCνµ
            //            if ( c_nu_interaction.ccnc == 0 && c_nu_interaction.pdg == 14 ){
            
            if (debug>3) { cout<<"CC nu mu event "<< endl;}
            
            c_nu_interaction.SetNuMomentum( enu_truth[n] , nu_dcosx_truth[n] , nu_dcosy_truth[n] , nu_dcosz_truth[n] );
            c_nu_interaction.SetLeptonMomentum( lep_mom_truth[n] , lep_dcosx_truth[n] , lep_dcosy_truth[n] , lep_dcosz_truth[n] );
            c_nu_interaction.Set_q_vector();
            nu_interactions.push_back(c_nu_interaction );
            NnuInteractions++;
            //            }// end of if interaction is CCνµ
            
        }// end of if interaction contained
        
    }//end of neutrino interaction loop

    return true;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
bool cumputeAnaTree::GetGENIEInformation(int n){
    if (debug > 3) Printf("getting GENIE information");
    
    c_genie_interaction = GENIEinteraction(n,                                   // MC event-id
                                           genie_no_primaries                   // No. of primaries
                                           );
    c_genie_interaction.SetNuMomentum( enu_truth[n] , nu_dcosx_truth[n] , nu_dcosy_truth[n] , nu_dcosz_truth[n] );
    
    c_genie_interaction.SetRSE( run , subrun , event );
    c_genie_interaction.SetCCNC( ccnc_truth[n] );
    
    c_genie_interaction.SetVertexPosition( TVector3(nuvtxx_truth[n] , nuvtxy_truth[n] , nuvtxz_truth[n]) );
    c_genie_interaction.SetVertexContained( VertexContainedSoft( c_genie_interaction.vertex_position ) );
    
    for ( Int_t primary = 0 ; primary < genie_no_primaries ; primary ++ ) {
        PandoraNuTrack primary_pandoraNutrack;
        bool track_reconstructed = false;
        
        if (c_genie_interaction.IsVertexContained && genie_status_code[primary]==1 ){ // status_code=0 are unstable/intermediate particles
            
            if(!tracks.empty()){
                for (auto t: tracks) {
                    // in order to match genie primary to a pandoraNu track
                    // one can not use track-id, since geant4 and genie assign differnet track-ids...
                    // so we match the momentum and energy to within 1 MeV
                    if (t.MCpdgCode == genie_pdg[primary]   &&
                        t.truth_P == genie_P[primary]       &&
                        t.truth_start_pos == c_genie_interaction.vertex_position
                        ){
                        primary_pandoraNutrack = t;
                        track_reconstructed = true;
                        if (debug>3) Printf("track %d reconstructed as primary no. %d! ",t.track_id,primary);
                        break;
                    }
                }
            }
        }
        if (debug>4 && track_reconstructed) primary_pandoraNutrack.Print();
        
        c_genie_interaction.AddPrimary(
                                       genie_pdg[primary]
                                       ,genie_Eng[primary]
                                       ,genie_Px[primary]
                                       ,genie_Py[primary]
                                       ,genie_Pz[primary]
                                       ,genie_P[primary]
                                       ,genie_status_code[primary]
                                       ,genie_mass[primary]
                                       //                                       ,genie_trackID[primary] // useless
                                       ,genie_ND[primary]
                                       ,genie_mother[primary]
                                       ,track_reconstructed
                                       ,primary_pandoraNutrack.track_id
                                       ,primary_pandoraNutrack
                                       );
    }
    
    if(debug>3) Printf("added primaries");
    c_genie_interaction.SortNucleons();
    if(debug>3) Printf("sorted nucleons");
    c_genie_interaction.ComputeKinematics();
    if(debug>3) Printf("computed kinematics");
    c_genie_interaction.ComputePmissPrec();
    if(debug>3) Printf("computed p(miss) and p(rec)");
    c_genie_interaction.FindCC1p();
    if(debug>3) Printf("finished FindCC1p()");
    c_genie_interaction.FindCC_1p_200MeVc();
    if(debug>3) Printf("finished FindCC_1p_200MeVc()");
    c_genie_interaction.FindCC_1p_200MeVc_0pi();
    if(debug>3) Printf("finished FindCC_1p_200MeVc_0pi()");

    
    
    genie_interaction = c_genie_interaction; // for genie-interactions tree...
    genie_interactions.push_back( c_genie_interaction );
    return true;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
bool cumputeAnaTree::VertexContained(TVector3 v){
    
    if (debug>4) {Printf("checking if contained: "); SHOW3(v.x(),v.y(),v.z());}
    // check if contained
    
    // Katherine' Fiducial volume definitions
    if( ( v.x() < 3.45 )    | ( v.x() > 249.8 ) )   return false;
    if( ( v.y() < -110.53 ) | ( v.y() > 112.47 ) )  return false;
    if( ( v.z() < 5.1 )     | ( v.z() > 1031.9 ) )  return false;
    return true;
    
    //    // Tingjun' Fiducial volume definitions
    //    // [https://cdcvs.fnal.gov/redmine/projects/ccinclusive/repository/revisions/761e07f65665275fecce7447d79a1ed9183fdaea/entry/SelectionII/NeutrinoSelection.C]
    //    if( ( v.x() < 20. )             | ( v.x() > (256.35 - 20.) ) )  return false;
    //    if( ( v.y() < (-233./2 + 20.) ) | ( v.y() > (233./2 - 20.) ) )  return false;
    //    if( ( v.z() < (10.) )           | ( v.z() > (1036.8 - 10.) ) )  return false;
    //    return true;

    
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
bool cumputeAnaTree::VertexContainedSoft(TVector3 v){
    if (debug>4) {Printf("checking if softly contained, in the active volume of the detector: "); SHOW3(v.x(),v.y(),v.z());}
    // check if contained
    if( ( v.x() < 0 )    | ( v.x() > 260 ) )   return false;
    if( ( v.y() < -120 ) | ( v.y() > 120 ) )  return false;
    if( ( v.z() < 0 )    | ( v.z() > 1050 ) )  return false;
    return true;
    
}


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
bool cumputeAnaTree::TrackContained(TVector3 start , TVector3 end){
    
    if( ! VertexContained( start ) )   return false;
    if( ! VertexContained( end ) )      return false;
    return true;
    
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
bool cumputeAnaTree::TrackContainedSoft(TVector3 start , TVector3 end){
    //    for cosmic tracks - it is a softer containement requirement
    if( ! VertexContainedSoft( start ) )   return false;
    if( ! VertexContainedSoft( end ) )      return false;
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
void cumputeAnaTree::CreateROIsCCQE( Int_t ivtx , Int_t itrk_NuSelMuon, Int_t itrk_GBDTproton ){
    
    TVector3 vtx_position = TVector3( vtxx_pandoraNu[ivtx] , vtxy_pandoraNu[ivtx] , vtxz_pandoraNu[ivtx] );
    
    bool found_muon_track = false , found_proton_track = false;
    for (auto t:tracks) {
        
        if ( found_muon_track && found_proton_track ) break;
        
        if ( !found_muon_track && t.track_id == itrk_NuSelMuon)   {
            found_muon_track = true;
            for (int plane = 0 ; plane < 3 ; plane++ ){
                ROItrk_NuSelMuon[plane] = t.roi[plane];}
        }
        
        if ( !found_proton_track && t.track_id == itrk_GBDTproton)  {
            for (int plane = 0 ; plane < 3 ; plane++ ){
                found_proton_track = true;
                ROItrk_GBDTproton[plane] = t.roi[plane];}
        }
    }
    
    
    auto    geomHelper = ::larutil::GeometryHelper::GetME();
    double  vtx_xyz[3] = {vtx_position.x() , vtx_position.y() , vtx_position.z() - 20 };
    double  time_shift = 802;
    
    
    for (int plane = 0; plane < 3; plane++){
        
        box pROI = ROItrk_GBDTproton[plane];
        box muROI = ROItrk_NuSelMuon[plane];
        
        // geoHelper is a set of utility functions that help with geometric stuff..
        auto const& projection2D = geomHelper->Point_3Dto2D( vtx_xyz , plane );
        int wire = (int) ( projection2D.w / geomHelper->WireToCm() );
        int time = (int) ( projection2D.t / geomHelper->TimeToCm() ) + time_shift;
        int start_wire = wire, start_time = time, end_wire = wire, end_time = time;

        start_wire = std::min( { (int) ( projection2D.w / geomHelper->WireToCm() ) , pROI.start_wire , muROI.start_wire , pROI.end_wire , muROI.end_wire } );
        start_time = std::min( { (int) ( projection2D.t / geomHelper->TimeToCm() ) , pROI.start_time , muROI.start_time , pROI.end_time , muROI.end_time } );
        
        end_wire = std::max( { (int) ( projection2D.w / geomHelper->WireToCm() ) , pROI.start_wire , muROI.start_wire , pROI.end_wire , muROI.end_wire } );
        end_time = std::max( { (int) ( projection2D.t / geomHelper->TimeToCm() ) , pROI.start_time , muROI.start_time , pROI.end_time , muROI.end_time } );
        
//        
//        // start-wire
//        if ( start_wire > ROItrk_GBDTproton[plane].start_wire ){
//            start_wire = ROItrk_GBDTproton[plane].start_wire;
//        }
//        if ( start_wire > ROItrk_NuSelMuon[plane].start_wire ){
//            start_wire = ROItrk_NuSelMuon[plane].start_wire;
//        }
//        // start-time
//        if ( start_time > ROItrk_GBDTproton[plane].start_time ){
//            start_time = ROItrk_GBDTproton[plane].start_time;
//        }
//        if ( start_time > ROItrk_NuSelMuon[plane].start_time ){
//            start_time = ROItrk_NuSelMuon[plane].start_time;
//        }
//        
//        // end-wire
//        if ( end_wire < ROItrk_GBDTproton[plane].end_wire ){
//            end_wire = ROItrk_GBDTproton[plane].end_wire;
//        }
//        if ( end_wire < ROItrk_NuSelMuon[plane].end_wire ){
//            end_wire = ROItrk_NuSelMuon[plane].end_wire;
//        }
//        // end-time
//        if ( end_time < ROItrk_GBDTproton[plane].end_time ){
//            end_time = ROItrk_GBDTproton[plane].end_time;
//        }
//        if ( end_time < ROItrk_NuSelMuon[plane].end_time ){
//            end_time = ROItrk_NuSelMuon[plane].end_time;
//        }
        mu_p_VtxROI[plane] = box( start_wire , start_time , end_wire , end_time );
        
    }
    
    
}


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void cumputeAnaTree::TagCC1pTracks(){
    if(debug>3) Printf("cumputeAnaTree::TagCC1pTracks()");
    
    // Match pandoraNu tracks to true-CC1p (GENIE interactions),
    // and if they come from GENIE-CC1p, tag them as comming form
    // this CC1p.
    // Add the CC1p id (mcevts_truth number) so that we know later if
    // the proton and the muon came from the same CC1p

    // loop over all tracks reconstructed in this event
    for (auto & pandoraNu_track:tracks){
        //        SHOW(pandoraNu_track.track_id);
        
        if(!genie_interactions.empty()){
            // loop over all tracks in all GENIE neutrino interactions
            for (auto & genie_interaction:genie_interactions){
                //                SHOW(genie_interaction.mcevent_id);
                
                for (auto & genie_interaction_reconstructed_track: genie_interaction.tracks) {
                    //                    SHOW(genie_interaction_reconstructed_track.track_id);
                    
                    // match reconstructed track from the GENIE neutrino interaction to the pandoraNu track at interest
                    if (genie_interaction_reconstructed_track.track_id == pandoraNu_track.track_id){
                        //                        Printf("matched reconstructed to GENIE track!");
                        //                        SHOW(genie_interaction.IsCC1p);
                        
                        // if match, flag the track as true (GENIE) CC1p or not
                        if (genie_interaction.IsCC1p==true){
                            pandoraNu_track.IsGENIECC1p = true;
                            pandoraNu_track.mcevent_id = genie_interaction.mcevent_id;
                            
                            genie_interaction_reconstructed_track.IsGENIECC1p = true;
                            genie_interaction_reconstructed_track.mcevent_id = genie_interaction.mcevent_id;
                        }
                    }
                }
            }
        }
    }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void cumputeAnaTree::TagCC_1p_200MeVc_0pi_Tracks(){
    if(debug>3) Printf("cumputeAnaTree::TagCC1pTracks()");
    
    // Match pandoraNu tracks to true-CC_1p_200MeVc_0pi (GENIE interactions),
    // and if they come from GENIE-CC_1p_200MeVc_0pi, tag them as comming form
    // this CC_1p_200MeVc_0pi.
    // Add the CC_1p_200MeVc_0pi id (mcevts_truth number) so that we know later if
    // the proton and the muon came from the same CC_1p_200MeVc_0pi
    
    // loop over all tracks reconstructed in this event
    for (auto & pandoraNu_track:tracks){
        if(!genie_interactions.empty()){
            // loop over all tracks in all GENIE neutrino interactions
            for (auto & genie_interaction:genie_interactions){
                for (auto & genie_interaction_reconstructed_track: genie_interaction.tracks) {
                    // match reconstructed track from the GENIE neutrino interaction to the pandoraNu track at interest
                    if (genie_interaction_reconstructed_track.track_id == pandoraNu_track.track_id){
                        // if match, flag the track as true (GENIE) CC1p or not
                        if (genie_interaction.IsCC_1p_200MeVc_0pi==true){
                            pandoraNu_track.IsGENIECC_1p_200MeVc_0pi = true;
                            pandoraNu_track.mcevent_id = genie_interaction.mcevent_id;
                            
                            genie_interaction_reconstructed_track.IsGENIECC_1p_200MeVc_0pi = true;
                            genie_interaction_reconstructed_track.mcevent_id = genie_interaction.mcevent_id;
                        }
                    }
                }
            }
        }
    }
}



//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
bool cumputeAnaTree::FillOutTree (bool fDo){
    
    Debug(3, "xxxxxx\nFilling eventsTree....\nxxxxxxxx");
    OutTree -> Fill();
    return true;
    
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
bool cumputeAnaTree::FillGENIETree (bool fDo){
    
    Debug(3, "xxxxxx\nFilling GENIETree....\nxxxxxxxx");
//    if (!genie_interactions.empty()){
    GENIETree -> Fill();
    Debug(3, "xxxxxx\nFilled GENIETree....\nxxxxxxxx");
    return true;
//    }
    Debug(3, "genie_interactions is empty, returning without filling GENIETree");
    return false;
}


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
bool cumputeAnaTree::IsGoodTrack ( int fTrackID ){
    for (auto t: tracks) {
        if (t.track_id==fTrackID) return true;
    }
    return false;
}


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void cumputeAnaTree::GetPandoraCosmicTracks(){

    if (ntracks_pandoraCosmic==0 || ntracks_pandoraCosmic>100) return;
    if(debug>2) Printf("loop over pandoraCosmic %d tracks",ntracks_pandoraCosmic);


    // loop over all reconstructed tracks
    for(Int_t j=0; j < ntracks_pandoraCosmic && j < MAX_cosmic_tracks; j++){

        InitTrack();

        c_cosmic_track = PandoraNuTrack(
                                        run                                     // run
                                        ,subrun                                 // subrun
                                        ,event                                  // event
                                        ,trkId_pandoraCosmic[j]                 // track id
                                        ,TVector3(trkstartx_pandoraCosmic[j],
                                                  trkstarty_pandoraCosmic[j],
                                                  trkstartz_pandoraCosmic[j])   // start position
                                        ,TVector3(trkendx_pandoraCosmic[j]  ,
                                                  trkendy_pandoraCosmic[j]  ,
                                                  trkendz_pandoraCosmic[j])    // end position
                                        ,trklen_pandoraCosmic[j]                // track length
                                        ,trktheta_pandoraCosmic[j]              // theta
                                        ,trkphi_pandoraCosmic[j]                // phi
                                        );

        if ( !TrackContainedSoft( c_cosmic_track.start_pos   , c_cosmic_track.end_pos )) continue;
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
        // c_cosmic_track.Momentum();
        // if(debug>3) Printf("calculated the Momentum of the track ...");
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

        //        // software trigger
        //        c_cosmic_track.SetSWtrigger(swtrigger_name,swtrigger_triggered);

        cosmic_tracks.push_back(c_cosmic_track);
        if(debug>3) Printf("pushed the track into tracks which now has a size %lu...",cosmic_tracks.size());
    }
    
    if(debug>2) Printf("checking y_max for pandoraCosmic %d tracks",Ncosmictracks);
    if (!Ncosmictracks_ymax.empty()) Ncosmictracks_ymax.clear();
    for (auto ymax_cosmictrack: ymax_cosmictracks){
        Ncosmictracks_ymax.push_back(0);
        for(auto c_cosmic_track:cosmic_tracks){
            if (c_cosmic_track.starty < ymax_cosmictrack){
                Ncosmictracks_ymax.back()++;
            }
        }
    }
    if(debug>2) {SHOWstdVector(ymax_cosmictracks); SHOWstdVector(Ncosmictracks_ymax);}
    
}



//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void cumputeAnaTree::PrintData(int entry){
    
    // printf("\t[%.1f%%]\t",100.*(float)entry/Nentries);
    // SHOW(entry);
    PrintLine();    SHOW3( run , subrun , event );      PrintLine();
    //    if(!nu_interactions.empty()){
    //        cout << "\033[33m" << "xxxxxxxxxxxxxx\n\n" << nu_interactions.size() << " neutrino interactions\n\n" << "xxxxxxxxxxxxxx"<< "\033[37m" << endl;
    //        for (auto nu_interaction: nu_interactions) {
    //            nu_interaction.Print();
    //        }
    //    }
    if(!genie_interactions.empty()){
        cout << "\033[33m" << "xxxxxxxxxxxxxx\n\n" << genie_interactions.size() << " genie interactions\n\n" << "xxxxxxxxxxxxxx"<< "\033[37m" << endl;
        for (auto genie_interaction: genie_interactions) {
            genie_interaction.Print(true);
        }
    }
    if(!cosmic_tracks.empty()){
        cout << "\033[33m" << "xxxxxxxxxxxxxx\n\n" << cosmic_tracks.size() << " pandoraCosmic tracks\n\n" << "xxxxxxxxxxxxxx"<< "\033[37m" << endl;
        for (auto t: cosmic_tracks) {
            t.Print();
        }
    }
    
//    if(!hits.empty()){
//        cout << "\033[33m" << "xxxxxxxxxxxxxx\n\n" << hits.size() << " hits\n\n" << "xxxxxxxxxxxxxx"<< "\033[30m" << endl;
//        for (auto hit: hits) {
//            hit.Print();
//        }
//    }

    if(!tracks.empty()){
        cout << "\033[33m" << "xxxxxxxxxxxxxx\n\n" << tracks.size() << " pandoraNu tracks\n\n" << "xxxxxxxxxxxxxx"<< "\033[37m" << endl;
        
        for (auto t: tracks) {
            t.Print(false,true,true);
        }
    }
    if(!tracks_vertices.empty()){
        cout << "\033[33m" << "xxxxxxxxxxxxxx\n\n" << tracks_vertices.size() << " tracks vertices\n\n" << "xxxxxxxxxxxxxx"<< "\033[37m" << endl;
        
        for (auto v: tracks_vertices) {
            v.Print();
        }
    }
    if(!mutual_vertices.empty()){
        cout << "\033[33m" << "xxxxxxxxxxxxxx\n\n" << mutual_vertices.size() << " mutual vertices\n\n" << "xxxxxxxxxxxxxx"<< "\033[37m" << endl;
        for (auto v: mutual_vertices) {
            v.Print();
        }
    }
    if(!g4particles.empty()){
        cout << "\033[33m" << "xxxxxxxxxxxxxx\n\n" << g4particles.size() << " g4 particles \n\n" << "xxxxxxxxxxxxxx"<< "\033[37m" << endl;
        for (auto v: g4particles) {
            v.Print();
        }
    }
    
    EndEventBlock();
    
}


#endif
