/**
 * \file cumputeAnaTree.h
 *
 * \ingroup GBDTprotonsPackage
 *
 * \brief Class def header for a class cumputeAnaTree, tools for analysis of AnalysisTree information
 *
 * @author erez o. c.
 */

/** \addtogroup GBDTprotonsPackage
 
 @{*/
#ifndef cumputeAnaTree_H
#define cumputeAnaTree_H
#include "LArUtil/../../UserDev/mySoftware/MySoftwarePackage/myIncludes.h"
#include "PandoraNuTrack.h"
#include <ostream>
#include "TChain.h"
#include "GeoAlgo.h"
#include "nuInteraction.h"
#include "GENIEinteraction.h"
#include "track_vertex.h"
#include "mutual_vertex.h"
#include "LArG4Particle.h"
#include "hit.h"
#include "box.h"


#define MAX_vertices 20
#define MAX_tracks 1000
#define MAX_cosmic_tracks 100
#define kMaxHits 60000
#define kMaxTruth 10
#define kMaxPrimaries 20000  //maximum number of primary particles
/**
 \class cumputeAnaTree
 User defined class cumputeAnaTree ... these comments are used to generate
 doxygen documentation!
 */



class cumputeAnaTree: public myIncludes {
    
public:
    
    // constructors
    cumputeAnaTree(){}
    ~cumputeAnaTree(){}
    
    // construct w/ input and output TTree-s
    cumputeAnaTree (TTree * fOutTree,
                    TString foption="extract all tracks information", int fdebug=0,
                    bool fMCmode=false, TTree * fGENIETree = nullptr,
                    bool fDoPandoraCosmic=false, bool fDo_dEdx=false, int fMCCV=8 );
    
    
 
    
    // setters
    void             SetInTree (TTree * tree)    {InTree = tree; InitInputTree();};
    void             SetInTree (TChain * ch)     {SetInTree ((TTree*)ch);};
    void            SetOutTree (TTree * tree)    {OutTree = tree;};
    void          SetGENIETree (TTree * tree)    {GENIETree = tree;};
    void        SetCSVFileName (TString name)    {CSVFileName = name;};
    void             SetMCMode (bool _mc_mode)   {MCmode = _mc_mode;};
    void             SetOption (TString foption) {option = foption;};
    void    SetDoPandoraCosmic (bool fDo)        {DoPandoraCosmic = fDo;};
    void            SetDo_dEdx (bool fDo)        {Do_dEdx = fDo;};
    void               SetMCCV (int fMCCV=8)     {MCCV = fMCCV;};
    
    
    // getters
    TTree*             GetInTree ()            {return InTree;};
    TTree*            GetOutTree ()            {return OutTree;};
    void                GetEntry ( int );
    box     GetROItrk_GBDTproton ( int plane ) {return ROItrk_GBDTproton[plane]; };
    box      GetROItrk_NuSelMuon ( int plane ) {return ROItrk_NuSelMuon[plane]; };
    box           Getmu_p_VtxROI ( int plane ) {return mu_p_VtxROI[plane]; };


    // getters of my objects
    PandoraNuTrack        GetTrack ( int i )     {return tracks.at(i);};
    PandoraNuTrack  GetCosmicTrack ( int i )     {return cosmic_tracks.at(i);};
    LArG4Particle    GetG4Particle ( int i )     {return g4particles.at(i);};

    
    // initializations
    void          InitInputTree ();
    void         InitOutputTree ();
    void    SetOutTreeAddresses ();
    void              InitEntry ();
    void              InitTrack ();
    
    
    // running
    bool    extract_information (bool fDo=true);
    bool          FillGENIETree (bool fDo=true);
    bool            FillOutTree (bool fDo=true);
    bool    GetTruthInformation ();
    bool    GetGENIEInformation (int n=0);
    void       GetInTimeFlashes ();
    void GetPandoraCosmicTracks ();
    void     GetPandoraNuTracks ();
    void   CollectTrackVertices ();
    void     FindMutualVertices ();
    void     FindMuonScattering ();
    void         CreateROIsCCQE ( Int_t, Int_t, Int_t );
    void              Write2CSV ( Int_t, Int_t, Int_t  );
    void    GetEnergyDeposition ( int j );
    void     GetSoftwareTrigger ();
    void          TagCC1pTracks ();
    bool          WireTimeInBox (int w, int t, box b); // returns true if the wire/time point is in the box
    bool    AssociateHitsTracks ();
    void            CollectHits ();
    //void              CloseFile (){   InTree->ReleaseBranchAddresses(); };
    
    
    
    // helpers
    bool        VertexContained ( TVector3 );
    bool         TrackContained ( TVector3 , TVector3 );
    bool    VertexContainedSoft ( TVector3 );
    bool     TrackContainedSoft ( TVector3 , TVector3 );
    void              PrintData ( int);
    Float_t      TrkVtxDistance ( Int_t ivtx , Int_t itrk ) ;
    bool            IsGoodTrack ( int fTrackID );
    
    
  
    // variables
    
    TTree       * InTree    , * OutTree , * GENIETree;
    TString     CSVFileName , option    , CSVHeader;
    ofstream    csvfile;

    bool        MCmode;
    bool        foundMuonScattering;
    bool        track_already_included;
    bool        DoPandoraCosmic, Do_dEdx;
    
    // event information
    
    Int_t       run         , subrun    , event ;
    Int_t       Nentries    , entry     ;
    Double_t    pot; // protons on target
    
    
    // hits
    Int_t       no_hits,                no_flashes;
    Short_t     nfls_simpleFlashBeam;
    Short_t     hit_plane[kMaxHits],    hit_wire[kMaxHits];
    Float_t     hit_peakT[kMaxHits];
    Float_t     hit_charge[kMaxHits];     //area (total charge (ADC) deposited for hit in the tdc range)
    Short_t     hit_trkid[kMaxHits];      //is this hit associated with a reco track?
    Short_t     hit_trkKey[kMaxHits];      //is this hit associated with a reco track,  if so associate a unique track key ID?


    
    // PandoraNu
    // -------------------------------------------------------
    Short_t     ntracks_pandoraNu;
    Short_t     trkId_pandoraNu[MAX_tracks] ;
    Short_t     ntrkhits_pandoraNu[MAX_tracks][3];
    Short_t     trkncosmictags_tagger_pandoraNu[MAX_tracks];
    Short_t     trkcosmictype_tagger_pandoraNu[MAX_tracks][10];
    Short_t     trkcosmictype_containmenttagger_pandoraNu[MAX_tracks][10];
    Short_t     trkpidbestplane_pandoraNu[MAX_tracks];
    
    
    
    
    Int_t       nhits       , primary, MCCV;
    Int_t       Ntracks     , NnuInteractions   , Ncosmictracks     , Ng4particles;
    Int_t       Nymax_cosmic;
    std::vector<Float_t>    ymax_cosmictracks;
    std::vector<Int_t>      Ncosmictracks_ymax;
    Int_t       trkg4id_pandoraNu[MAX_tracks]   , TrackId[MAX_tracks];
    Int_t       trkpidpdg_pandoraNu[MAX_tracks][3];
    Int_t       mcevts_truth;               //number of neutrino interactions in the spill
    Int_t       nuPDG_truth[kMaxTruth];     //neutrino PDG code (nue=12; anti-nue=-12; numu=14; anti-numu=-14; nutau=16; anti-nutau=-16)
    Int_t       ccnc_truth[kMaxTruth];      //neutrino interaction type: 0=Charged current (CC), 1=Neutral current (NC)
    Int_t       mode_truth[kMaxTruth];      //neutrino nucleus 0=Quasi-elastic or Elastic, 1=Resonant (RES), 2=DIS, 3=Coherent production
    Int_t       hitnuc_truth[kMaxTruth];    //neutrino scattering off of which nucleon (proton or neutron); holds the pdg of the nucleon

    Float_t     trkpurtruth[MAX_tracks][3]; // The trkpurtruth variable is defined as the ratio of the energy of the particle that contributed most to this track in a given plane to the total energy coming from all particles that contribute to this track in that plane
    
    
    Float_t     startdqdx , enddqdx     , totaldqdx;
    Float_t     cftime    , cftimewidth , cfzcenter , cfzwidth, cfycenter , cfywidth  , cftotalpe , cfdistance;
    
    Float_t     trklen_pandoraNu[MAX_tracks]        , trkstartx_pandoraNu[MAX_tracks]       , trkstarty_pandoraNu[MAX_tracks];
    Float_t     trkstartz_pandoraNu[MAX_tracks]     , trkendx_pandoraNu[MAX_tracks]         , trkendy_pandoraNu[MAX_tracks];
    Float_t     trkendz_pandoraNu[MAX_tracks]       , trktheta_pandoraNu[MAX_tracks]        , trkphi_pandoraNu[MAX_tracks];
    
    Float_t     trkmomrange_pandoraNu[MAX_tracks]   , trkmommsllhd_pandoraNu[MAX_tracks]; // track momentum from range / MCS llhd
    
    // x/y/z positions
    Float_t     trkxyz_pandoraNu[MAX_tracks][3][2000][3];
    // calorimetery
    Float_t     trkdqdx_pandoraNu[MAX_tracks][3][2000]  ,   trkdedx_pandoraNu[MAX_tracks][3][2000] , trkresrg_pandoraNu[MAX_tracks][3][2000]; // (dE/dx in MeV/cm)
    
    // tagging
    Float_t     trkcosmicscore_tagger_pandoraNu[MAX_tracks][10];
    Float_t     trkcosmicscore_containmenttagger_pandoraNu[MAX_tracks][10];
    Float_t     trkpidchi_pandoraNu[MAX_tracks][3]      , trkpidpida_pandoraNu[MAX_tracks][3]       , flash_time[kMaxHits]  ;
    
    Float_t     flash_timewidth[kMaxHits] , flash_pe[kMaxHits];
    Float_t     flash_ycenter[kMaxHits]   , flash_ywidth[kMaxHits]    , flash_zcenter[kMaxHits] , flash_zwidth[kMaxHits];
    
    // vertex information
    Short_t     nvtx_pandoraNu;
    Float_t     vtxx_pandoraNu[MAX_vertices];     //the X location (in cm) for a given vertex
    Float_t     vtxy_pandoraNu[MAX_vertices];     //the Y location (in cm) for a given vertex
    Float_t     vtxz_pandoraNu[MAX_vertices];     //the Z location (in cm) for a given vertex


    
    
    // PandoraCosmic
    // -------------------------------------------------------
    Short_t     ntracks_trackkalmanhit;

    
    // PandoraCosmic
    // -------------------------------------------------------
    Short_t     ntracks_pandoraCosmic;
    Short_t     trkId_pandoraCosmic[MAX_cosmic_tracks];
    Short_t     ntrkhits_pandoraCosmic[MAX_cosmic_tracks][3];
    Short_t     trkncosmictags_tagger_pandoraCosmic[MAX_cosmic_tracks];
    Short_t     trkcosmictype_tagger_pandoraCosmic[MAX_cosmic_tracks][10];
    Short_t     trkcosmictype_containmenttagger_pandoraCosmic[MAX_tracks][10] ;
    Short_t     trkpidbestplane_pandoraCosmic[MAX_tracks];
    
    Int_t       trkg4id_pandoraCosmic[MAX_cosmic_tracks]  ;
    Int_t       trkpidpdg_pandoraCosmic[MAX_cosmic_tracks][3];
    
    Float_t     trklen_pandoraCosmic[MAX_cosmic_tracks]     , trkstartx_pandoraCosmic[MAX_cosmic_tracks]    , trkstarty_pandoraCosmic[MAX_cosmic_tracks];
    Float_t     trkstartz_pandoraCosmic[MAX_cosmic_tracks]  , trkendx_pandoraCosmic[MAX_cosmic_tracks]      , trkendy_pandoraCosmic[MAX_cosmic_tracks];
    Float_t     trkendz_pandoraCosmic[MAX_cosmic_tracks]    , trktheta_pandoraCosmic[MAX_cosmic_tracks]     , trkphi_pandoraCosmic[MAX_cosmic_tracks];
    
    
    // x/y/z positions
    Float_t     trkxyz_pandoraCosmic[MAX_tracks][3][2000][3];
    // calorimetery
    Float_t     trkdqdx_pandoraCosmic[MAX_tracks][3][2000]  ,   trkdedx_pandoraCosmic[MAX_tracks][3][2000] , trkresrg_pandoraCosmic[MAX_tracks][3][2000]; // (dE/dx in MeV/cm)

    
    // tagging
    Float_t     trkcosmicscore_tagger_pandoraCosmic[MAX_cosmic_tracks][10];
    Float_t     trkcosmicscore_containmenttagger_pandoraCosmic[MAX_cosmic_tracks][10];
    Float_t     trkpidchi_pandoraCosmic[MAX_cosmic_tracks][3]           , trkpidpida_pandoraCosmic[MAX_cosmic_tracks][3]  ;
    
    
    // vertex information
    Short_t     nvtx_pandoraCosmic;
    Float_t     vtxx_pandoraCosmic[MAX_vertices];     //the X location (in cm) for a given vertex
    Float_t     vtxy_pandoraCosmic[MAX_vertices];     //the Y location (in cm) for a given vertex
    Float_t     vtxz_pandoraCosmic[MAX_vertices];     //the Z location (in cm) for a given vertex
    
    
    
    
    
    // MC information
    // geant information
    Int_t       geant_list_size , no_primaries;
    Int_t       pdg[kMaxPrimaries]  , process_primary[kMaxPrimaries]        , Mother[kMaxPrimaries];
    Float_t     Eng[kMaxPrimaries]  , P[kMaxPrimaries]  , Mass[kMaxPrimaries]   , KE[kMaxPrimaries];
    Float_t     theta[kMaxPrimaries], phi[kMaxPrimaries], pathlen[kMaxPrimaries];
    Float_t     StartPointx[kMaxPrimaries]  , StartPointy[kMaxPrimaries]    , StartPointz[kMaxPrimaries];
    Float_t     EndPointx[kMaxPrimaries]    , EndPointy[kMaxPrimaries]      , EndPointz[kMaxPrimaries];
    /// ---------------------------------
    
    Float_t     enu_truth[kMaxTruth];       //true neutrino energy in GeV
    Float_t     Q2_truth[kMaxTruth];        //Momentum transfer squared in GeV^2
    Float_t     W_truth[kMaxTruth];         //hadronic invariant mass in GeV
    Float_t     X_truth[kMaxTruth];         //Bjorken X
    Float_t     Y_truth[kMaxTruth];         //Inelasticity
    Float_t     nuvtxx_truth[kMaxTruth];    //neutrino vertex x in cm
    Float_t     nuvtxy_truth[kMaxTruth];    //neutrino vertex y in cm
    Float_t     nuvtxz_truth[kMaxTruth];    //neutrino vertex z in cm
    Float_t     nu_dcosx_truth[kMaxTruth];  //neutrino x directional cosine for neutrino track Start
    Float_t     nu_dcosy_truth[kMaxTruth];  //neutrino y directional cosine for neutrino track Start
    Float_t     nu_dcosz_truth[kMaxTruth];  //neutrino z directional cosine for neutrino track Start
    Float_t     lep_mom_truth[kMaxTruth];   //lepton momentum in GeV
    Float_t     lep_dcosx_truth[kMaxTruth]; //lepton x directional cosine for lepton track Start
    Float_t     lep_dcosy_truth[kMaxTruth]; //lepton y directional cosine for lepton track Start
    Float_t     lep_dcosz_truth[kMaxTruth]; //lepton z directional cosine for lepton track Start
  
    // GENIE
    Int_t    genie_no_primaries;        //number of primary particles generated by GENIE
    Int_t    genie_pdg[40];             //particle type (pdg) of the GENIE particle
    Float_t  genie_Eng[40];           //Energy of the GENIE particle in GeV
    Float_t  genie_Px[40];            //Px of the GENIE particle in GeV
    Float_t  genie_Py[40];            //Py of the GENIE particle in GeV
    Float_t  genie_Pz[40];            //Pz of the GENIE particle in GeV
    Float_t  genie_P[40];             //Magnitude of the momentum vector of the GENIE particle in GeV
    Int_t    genie_status_code[40];   //particle status code of the GENIE particle
    Float_t  genie_mass[40];          //mass of the GENIE particle in GeV
    Int_t    genie_trackID[40];       //trackID of the GENIE particle (different from the GEANT-assigned track ID)
    Int_t    genie_ND[40];            //number of daughters of the GENIE particle
    Int_t    genie_mother[40];        //mother trackID of the GENIE particle
    
    
    // software trigger
    std::string swtrigger_name[1];
    bool        swtrigger_triggered[1];
    

    
    
    
    box ROItrk_NuSelMuon[3] , ROItrk_GBDTproton[3] , mu_p_VtxROI[3];
    
    // energy deposition profile
    std::vector <Float_t>   residual_range[3]    , dEdx[3] , Edep[3] , dqdx[3];


    Float_t Nhits[3];

    
    // GeoAlgo
    geoalgo::GeoAlgo geo_algo;
    
    
    std::vector<int> goodflashidx;

    
    PandoraNuTrack c_track;
    std::vector<PandoraNuTrack> tracks;

    PandoraNuTrack c_cosmic_track;
    std::vector<PandoraNuTrack> cosmic_tracks;
    
    nuInteraction c_nu_interaction;
    std::vector<nuInteraction> nu_interactions;
    
    GENIEinteraction c_genie_interaction , genie_interaction;
    std::vector<GENIEinteraction> genie_interactions;


    track_vertex c_track_vertex;
    std::vector<track_vertex> tracks_vertices;

    mutual_vertex c_mutual_vertex;
    std::vector<mutual_vertex> mutual_vertices;
    
    LArG4Particle c_g4particle;
    std::vector<LArG4Particle> g4particles;

    hit c_hit;
    std::vector<hit> hits;
    
    



};

#endif
/** @} */ // end of doxygen group

