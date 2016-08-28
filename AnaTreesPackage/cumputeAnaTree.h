/**
 * \file cumputeAnaTree.h
 *
 * \ingroup GBDTprotonsPackage
 *
 * \brief Class def header for a class cumputeAnaTree, tools for analysis of AnalysisTree information
 *
 * @author erez c. and katherine w.
 */

/** \addtogroup GBDTprotonsPackage
 
 @{*/
#ifndef cumputeAnaTree_H
#define cumputeAnaTree_H
#include "../../mySoftware/MySoftwarePackage/myIncludes.h"
#include "PandoraNuTrack.h"
#include <ostream>
#include "TChain.h"
#include "GeoAlgo.h"
#include "nuInteraction.h"
#include "GENIEinteraction.h"
#include "track_vertex.h"
#include "mutual_vertex.h"


#define MAX_tracks 100
#define MAX_cosmic_tracks 500
#define MAX_hits 50000
#define kMaxTruth 10
/**
 \class cumputeAnaTree
 User defined class cumputeAnaTree ... these comments are used to generate
 doxygen documentation!
 */

class cumputeAnaTree{
    
public:
    
    // constructors
    cumputeAnaTree(){}
    ~cumputeAnaTree(){}
    
    // construct w/ input and output TTree-s
    cumputeAnaTree (TTree * fInTree, TTree * fOutTree,  TTree * fGENIETree, TString fCSVFileName, int fdebug=0, bool fMCmode=false);
    cumputeAnaTree (TChain * fInChain, TTree * fOutTree, TTree * fGENIETree, TString fCSVFileName, int fdebug=0, bool fMCmode=false){cumputeAnaTree((TTree*) fInChain, fOutTree, fGENIETree, fCSVFileName, fdebug, fMCmode);};
    
    
    
 
    
    // setters
    void        SetInTree (TTree * tree)    {InTree = tree;};
    void       SetOutTree (TTree * tree)    {OutTree = tree;};
    void     SetGENIETree (TTree * tree)    {GENIETree = tree;};
    void   SetCSVFileName (TString name)    {CSVFileName = name;};
    void         SetDebug (int _debug)      {debug = _debug;};
    void        SetMCMode (bool _mc_mode)   {MCmode = _mc_mode;};
    
    // getters
    TTree*          GetInTree (){return InTree;};
    TTree*         GetOutTree (){return OutTree;};
   
    // initializations
    void    InitInputTree ();
    void   InitOutputTree ();
    void    InitOutputCSV ();
    void        InitEntry ();
    void        InitTrack ();
    
    
    // running
    bool    extract_information (int);
    bool            FillOutTree ();
    bool    GetTruthInformation ();
    bool    GetGENIEInformation ();
    void       GetInTimeFlashes ();
    void GetPandoraCosmicTracks ();
    void     GetPandoraNuTracks ();
    void   CollectTrackVertices ();
    void     FindMutualVertices ();
    void     FindMuonScattering ();
    void        WriteTracks2CSV ();
    
    // helpers
    bool        VertexContained ( TVector3 );
    bool         TrackContained (TVector3 , TVector3 );
    void              PrintData (int);

    
    
    
    
  
    // variables
    TTree       * InTree    , * OutTree , * GENIETree;

    int         debug;  // 0 - quiet, 1 - major functionality, > 2 - print out all sorts of shit
    bool        MCmode;
    bool        foundMuonScattering;
    bool        track_already_included;
    
    Short_t     ntracks_pandoraNu ;
    Short_t     trkId_pandoraNu[MAX_tracks] , ntrkhits_pandoraNu[MAX_tracks][3]     , trkncosmictags_tagger_pandoraNu[MAX_tracks];
    Short_t     trkcosmictype_tagger_pandoraNu[MAX_tracks][10]  ;
    Short_t     trkcosmictype_containmenttagger_pandoraNu[MAX_tracks][10]           , trkpidbestplane_pandoraNu[MAX_tracks];
    
    Short_t     ntracks_pandoraCosmic;
    Short_t     trkId_pandoraCosmic[MAX_cosmic_tracks] , ntrkhits_pandoraCosmic[MAX_cosmic_tracks][3]     , trkncosmictags_tagger_pandoraCosmic[MAX_cosmic_tracks];
    Short_t     trkcosmictype_tagger_pandoraCosmic[MAX_cosmic_tracks][10]  ;
    Short_t     trkcosmictype_containmenttagger_pandoraCosmic[MAX_tracks][10]           , trkpidbestplane_pandoraCosmic[MAX_tracks];
    
    Short_t     hit_trkid[MAX_hits]         , hit_trkKey[MAX_hits]                  , hit_plane[MAX_hits]       , hit_wire[MAX_hits];
    
    
    Int_t       run         , subrun    , event , primary ;
    Int_t       Nentries    , entry     , nhits;
    Int_t       Ntracks     , NnuInteractions   , Ncosmictracks;
    Int_t       trkg4id_pandoraNu[MAX_tracks]   , TrackId[MAX_tracks];
    Int_t       trkpidpdg_pandoraNu[MAX_tracks][3];
    Int_t       no_hits     , no_flashes;
    Int_t       mcevts_truth;               //number of neutrino interactions in the spill
    Int_t       nuPDG_truth[kMaxTruth];     //neutrino PDG code (nue=12; anti-nue=-12; numu=14; anti-numu=-14; nutau=16; anti-nutau=-16)
    Int_t       ccnc_truth[kMaxTruth];      //neutrino interaction type: 0=Charged current (CC), 1=Neutral current (NC)
    Int_t       mode_truth[kMaxTruth];      //neutrino nucleus 0=Quasi-elastic or Elastic, 1=Resonant (RES), 2=DIS, 3=Coherent production
    Int_t       hitnuc_truth[kMaxTruth];    //neutrino scattering off of which nucleon (proton or neutron); holds the pdg of the nucleon
    
    // MC information
    Int_t       geant_list_size         , pdg[MAX_tracks];
    
    Float_t     startdqdx , enddqdx     , totaldqdx;
    Float_t     cftime    , cftimewidth , cfzcenter , cfzwidth, cfycenter , cfywidth  , cftotalpe , cfdistance;
    Float_t     trklen_pandoraNu[MAX_tracks]    , trkstartx_pandoraNu[MAX_tracks]   , trkstarty_pandoraNu[MAX_tracks];
    Float_t     trkstartz_pandoraNu[MAX_tracks] , trkendx_pandoraNu[MAX_tracks] , trkendy_pandoraNu[MAX_tracks];
    Float_t     trkendz_pandoraNu[MAX_tracks]   , trktheta_pandoraNu[MAX_tracks], trkphi_pandoraNu[MAX_tracks];
    Float_t     trkdqdx_pandoraNu[MAX_tracks][3][MAX_hits]                      , trkresrg_pandoraNu[MAX_tracks][3][MAX_hits] ;
    Float_t     trkcosmicscore_tagger_pandoraNu[MAX_tracks][10] , trkcosmicscore_containmenttagger_pandoraNu[MAX_tracks][10];
    Float_t     trkpidchi_pandoraNu[MAX_tracks][3]  , trkpidpida_pandoraNu[MAX_tracks][3]   , flash_time[MAX_hits]  ;
    
    Float_t     flash_timewidth[MAX_hits] , flash_pe[MAX_hits];
    Float_t     flash_ycenter[MAX_hits]   , flash_ywidth[MAX_hits]    , flash_zcenter[MAX_hits]   , flash_zwidth[MAX_hits];
    
    
    Int_t       trkg4id_pandoraCosmic[MAX_cosmic_tracks]  ;
    Int_t       trkpidpdg_pandoraCosmic[MAX_cosmic_tracks][3];
    Float_t     trklen_pandoraCosmic[MAX_cosmic_tracks]                 , trkstartx_pandoraCosmic[MAX_cosmic_tracks]            , trkstarty_pandoraCosmic[MAX_cosmic_tracks];
    Float_t     trkstartz_pandoraCosmic[MAX_cosmic_tracks]              , trkendx_pandoraCosmic[MAX_cosmic_tracks]              , trkendy_pandoraCosmic[MAX_cosmic_tracks];
    Float_t     trkendz_pandoraCosmic[MAX_cosmic_tracks]                , trktheta_pandoraCosmic[MAX_cosmic_tracks]             , trkphi_pandoraCosmic[MAX_cosmic_tracks];
    Float_t     trkdqdx_pandoraCosmic[MAX_cosmic_tracks][3][MAX_hits]   , trkresrg_pandoraCosmic[MAX_cosmic_tracks][3][MAX_hits] ;
    Float_t     trkcosmicscore_tagger_pandoraCosmic[MAX_cosmic_tracks][10] , trkcosmicscore_containmenttagger_pandoraCosmic[MAX_cosmic_tracks][10];
    Float_t     trkpidchi_pandoraCosmic[MAX_cosmic_tracks][3]  , trkpidpida_pandoraCosmic[MAX_cosmic_tracks][3]  ;
    
    
    
    Float_t  enu_truth[kMaxTruth];       //true neutrino energy in GeV
    Float_t  Q2_truth[kMaxTruth];        //Momentum transfer squared in GeV^2
    Float_t  W_truth[kMaxTruth];         //hadronic invariant mass in GeV
    Float_t  X_truth[kMaxTruth];         //Bjorken X
    Float_t  Y_truth[kMaxTruth];         //Inelasticity
    Float_t  nuvtxx_truth[kMaxTruth];    //neutrino vertex x in cm
    Float_t  nuvtxy_truth[kMaxTruth];    //neutrino vertex y in cm
    Float_t  nuvtxz_truth[kMaxTruth];    //neutrino vertex z in cm
    Float_t  nu_dcosx_truth[kMaxTruth];  //neutrino x directional cosine for neutrino track Start
    Float_t  nu_dcosy_truth[kMaxTruth];  //neutrino y directional cosine for neutrino track Start
    Float_t  nu_dcosz_truth[kMaxTruth];  //neutrino z directional cosine for neutrino track Start
    Float_t  lep_mom_truth[kMaxTruth];   //lepton momentum in GeV
    Float_t  lep_dcosx_truth[kMaxTruth]; //lepton x directional cosine for lepton track Start
    Float_t  lep_dcosy_truth[kMaxTruth]; //lepton y directional cosine for lepton track Start
    Float_t  lep_dcosz_truth[kMaxTruth]; //lepton z directional cosine for lepton track Start
  
    
    // TVector3
    TVector3 nuIntVertex;

    // GENIE
    Int_t    genie_no_primaries;        //number of primary particles generated by GENIE
    Int_t    genie_pdg[40];             //particle type (pdg) of the GENIE particle
    Float_t  genie_Eng[40];           //Energy of the GENIE particle in GeV
    Float_t  genie_Px[40];            //Px of the GENIE particle in GeV
    Float_t  genie_Py[40];            //Py of the GENIE particle in GeV
    Float_t  genie_Pz[40];            //Pz of the GENIE particle in GeV
    Float_t  genie_P[40];             //Magnitude of the momentum vector of the GENIE particle in GeV
    Int_t    genie_status_code[40];   //particle status code of the GENIE particle
    //  only genie_status_code==1 particles are to be tracked in detector MC,
    //  see genie.hepforge.org/doxygen/html/GHepStatus_8h_source.html
    Float_t  genie_mass[40];          //mass of the GENIE particle in GeV
    Int_t    genie_trackID[40];       //trackID of the GENIE particle (different from the GEANT-assigned track ID)
    Int_t    genie_ND[40];            //number of daughters of the GENIE particle
    Int_t    genie_mother[40];        //mother trackID of the GENIE particle
    
    
    
    TString  CSVFileName    , CSVHeader;
    ofstream    csvfile;


    
    // GeoAlgo
    geoalgo::GeoAlgo geo_algo;
    
    
    std::vector<int> goodflashidx;

    
    PandoraNuTrack c_track;
    std::vector<PandoraNuTrack> tracks;

    PandoraNuTrack c_cosmic_track;
    std::vector<PandoraNuTrack> cosmic_tracks;
    
    nuInteraction c_nu_interaction;
    std::vector<nuInteraction> nu_interactions;
    
    GENIEinteraction genie_interaction;
    std::vector<GENIEinteraction> genie_interactions;


    track_vertex c_track_vertex;
    std::vector<track_vertex> tracks_vertices;

    mutual_vertex c_mutual_vertex;
    std::vector<mutual_vertex> mutual_vertices;



};

#endif
/** @} */ // end of doxygen group

