import sys
sys.path.insert(0, '/uboone/app/users/ecohen/larlite/UserDev/GBDTprotons/GBDTprotonPackage/mac')
sys.path.insert(0, '/Users/erezcohen/larlite/UserDev/GBDTprotons/GBDTprotonPackage/mac')
from uboone_tools import *
from gbdt_tools import *
from larlite import *
from ROOT import PandoraNuTrack
from ROOT import cumputeAnaTree
from ROOT import ImportantTools
from ROOT import AnalyzeTracksFile


# globals
# -------------------------
min_trk_vtx_distance = 10 # [cm], this distance needs to be studied wisely


# paths
# -------------------------

neutrinoSel2_path       = lists_path + "/NeutrinoSelection2"
mu_p_intersection_path  = lists_path + "/muon_proton_intersection"
schemed_anatrees_path   = anatrees_data_path  + "/SchemedFiles"

g4_features_names = [ 'run'         ,'subrun'       ,'event'
                     ,'ig4'         ,'track_id'     ,'pdg'
                     ,'P'           ,'Mass'         ,'Eng'
                     ,'KE'          ,'theta'        ,'phi'
                     ,'primary'
                     ,'startx'      ,'starty'       ,'startz'
                     ,'endx'        ,'endy'         ,'endz'
                     ,'length'      ,'Mother'
                     ]


track_features_names = [ 'run'          ,'subrun'   ,'event'        ,'track_id'
                        ,'flip'         ,'nhits'    ,'length'
                        ,'startx'       ,'starty'   ,'startz'
                        ,'endx'         ,'endy'     ,'endz'
                        ,'theta'        ,'phi'      ,'distlenratio'
                        ,'startdqdx'    ,'enddqdx'  ,'dqdxdiff'     ,'dqdxratio'    ,'totaldqdx'    ,'averagedqdx'
                        ,'cosmicscore'  ,'coscontscore'
                        ,'pidpida'      ,'pidchi'
                        ,'cftime'       ,'cftimewidth','cfzcenter'  ,'cfzwidth'     ,'cfycenter'    ,'cfywidth' ,'cftotalpe'    , 'cfdistance'
                        ,'MCpdgCode'
                        ,'U_start_wire' ,'U_start_time' ,'U_end_wire' ,'U_end_time'
                        ,'V_start_wire' ,'V_start_time' ,'V_end_wire' ,'V_end_time'
                        ,'Y_start_wire' ,'Y_start_time' ,'Y_end_wire' ,'Y_end_time'
                        ,'purtruth_Y'
                        ,'CaloPDG_U'    ,'CaloPDG_V'    ,'CaloPDG_Y'
                        ,'truth_P'      ,'truth_Eng'    ,'truth_KE'   ,'truth_theta' , 'truth_phi'  , 'process_primary'
                        ,'truth_startx' ,'truth_starty' ,'truth_startz'
                        ,'truth_endx'   ,'truth_endy'   ,'truth_endz'
                        ,'residual_range_Y'
                        ,'dqdx_Y'
                        ,'dEdx_Y'
                        ,'Edep_Y'
                        ]


mu_p_features_names = ['run','subrun','event',
                       'ivtx','itrk_NuSelMuon','itrk_GBDTproton',
                       'mu_U_start_wire','mu_U_start_time','mu_U_end_wire','mu_U_end_time'
                       'mu_V_start_wire','mu_V_start_time','mu_V_end_wire','mu_V_end_time'
                       'mu_Y_start_wire','mu_Y_start_time','mu_Y_end_wire','mu_Y_end_time'
                       'p_U_start_wire','p_U_start_time','p_U_end_wire','p_U_end_time'
                       'p_V_start_wire','p_V_start_time','p_V_end_wire','p_V_end_time'
                       'p_Y_start_wire','p_Y_start_time','p_Y_end_wire','p_Y_end_time'
                       'v_U_start_wire','v_U_start_time','v_U_end_wire','v_U_end_time'
                       'v_V_start_wire','v_V_start_time','v_V_end_wire','v_V_end_time'
                       'v_Y_start_wire','v_Y_start_time','v_Y_end_wire','v_Y_end_time'
                       ]


# list names and file names
# ----------------------------------------------------------------------------------------------------
def Sel2muons_list_name(DataType = "BNB_5e19POT"):
    if DataType == "BNB_5e19POT":
        neutrinoSel2_list_name = "BeamOnData_pandoraNu_pandoraNu.csv"
    elif DataType == "EXTBNB":
        neutrinoSel2_list_name = "BeamOffData_pandoraNu_pandoraNu.csv"
    return neutrinoSel2_path + "/" + neutrinoSel2_list_name
# ----------------------------------------------------------------------------------------------------

# ----------------------------------------------------------------------------------------------------
def Sel2muons_intersection_list_name( GBDTmodelName, TracksListName, p_score ):
    classification_name = TracksListName + "_" + GBDTmodelName
    return "Sel2muons_"+classification_name+"_pscore_%.2f_intersection"%p_score
# ----------------------------------------------------------------------------------------------------

# ----------------------------------------------------------------------------------------------------
def Sel2muons_intersection_list_csv_name( GBDTmodelName, TracksListName, p_score ):
    return Sel2muons_intersection_list_name( GBDTmodelName, TracksListName, p_score ) + ".csv"
# ----------------------------------------------------------------------------------------------------

# ----------------------------------------------------------------------------------------------------
def good_mu_p_rse_list_name( GBDTmodelName, TracksListName, p_score ):
    return Sel2muons_intersection_list_name( GBDTmodelName, TracksListName, p_score ) + "_mindistance_%dcm.csv"%min_trk_vtx_distance
# ----------------------------------------------------------------------------------------------------


# ----------------------------------------------------------------------------------------------------
def schemed_anatrees_file_name( anatrees_list_name , scheming_name ):
    return schemed_anatrees_path + "/" + anatrees_list_name + "_" + scheming_name + ".root"
# ----------------------------------------------------------------------------------------------------


# ----------------------------------------------------------------------------------------------------
def tracks_features_file_name( ListName , first_anatree_file = 0 , last_anatree_file = 0 ):
    if first_anatree_file==last_anatree_file:
        return featuresfiles_path + "/" + "features_" + ListName + ".csv"
    else:
        return featuresfiles_path + "/" + "features_" + ListName + "_anatreefiles_%d_to_%d.csv"%(first_anatree_file,last_anatree_file)
# ----------------------------------------------------------------------------------------------------

# ----------------------------------------------------------------------------------------------------
def tracks_full_features_file_name( ListName , first_anatree_file = 0 , last_anatree_file = 0 ):
    if first_anatree_file==last_anatree_file:
        return featuresfiles_path + "/" + "full_features_" + ListName + ".csv"
    else:
        return featuresfiles_path + "/" + "full_features_" + ListName + "_anatreefiles_%d_to_%d.csv"%(first_anatree_file,last_anatree_file)
# ----------------------------------------------------------------------------------------------------

# ----------------------------------------------------------------------------------------------------
def g4_features_file_name( ListName , first_anatree_file = 0 , last_anatree_file = 0 ):
    if first_anatree_file==last_anatree_file:
        return featuresfiles_path + "/" + "g4_features_" + ListName + ".csv"
    else:
        return featuresfiles_path + "/" + "g4_features_" + ListName + "_anatreefiles_%d_to_%d.csv"%(first_anatree_file,last_anatree_file)
# ----------------------------------------------------------------------------------------------------


# ----------------------------------------------------------------------------------------------------
def rois_features_file_name( ListName , first_anatree_file = 0 , last_anatree_file = 0 ):
    if first_anatree_file==last_anatree_file:
        return rois_path + "/" + "rois_" + ListName + ".csv"
    else:
        return rois_path + "/" + "rois_" + ListName + "_anatreefiles_%d_to_%d.csv"%(first_anatree_file,last_anatree_file)
# ----------------------------------------------------------------------------------------------------



# ----------------------------------------------------------------------------------------------------
def tracks_anafile_name( ListName , first_anatree_file = 0 , last_anatree_file = 0 ):
    if first_anatree_file==last_anatree_file:
        return anafiles_path + "/" + "Tracks_" + ListName + ".root"
    else:
        return anafiles_path + "/" + "Tracks_" + ListName + "_anatreefiles_%d_to_%d.root"%(first_anatree_file,last_anatree_file)
# ----------------------------------------------------------------------------------------------------


# methods
# ----------------------------------------------------------------------------------------------------
def read_files_from_a_list( ListName , first_anatree_file = 0 , last_anatree_file = 0 ):
    # returns the files
    if flags.verbose: print_filename(lists_path + "/analysis_trees/" + ListName + ".list","reading list of files (collecting files %d to %d)..."%(first_anatree_file , last_anatree_file))
    with open( lists_path + "/analysis_trees/" + ListName + ".list") as f:
        files = f.read().splitlines()
    if last_anatree_file > first_anatree_file:
        files = files[ first_anatree_file : last_anatree_file ]
    if flags.verbose>4: print files
    return files
# ----------------------------------------------------------------------------------------------------


# ----------------------------------------------------------------------------------------------------
def get_analysistrees_chain(files):
    chain = ROOT.TChain("analysistree/anatree")
    for i in range(int(flags.files_frac*len(files))):
        if flags.verbose>1: print "file %d size is %.2f MB"%(i,float(os.path.getsize(files[i])/1048576))
        if float(os.path.getsize(files[i])/1048576) > 0.1 :
            chain.Add(files[i])
    if flags.verbose: print "input chain entries from ",int(flags.files_frac*len(files))," files: ",chain.GetEntries()
    return chain
# ----------------------------------------------------------------------------------------------------


# ----------------------------------------------------------------------------------------------------
def search_rse( RSE , EventsList ):
    run,subrun,event = RSE[0],RSE[1],RSE[2]
    for e in EventsList:
        if e['run'] == run and e['subrun'] == subrun and e['event'] == event:
            return True , e['ivtx-NuSel'], e['itrk-NuSelMuon'], e['itrk-GBDTproton']
    return False , -1 , -1 , -1
# ----------------------------------------------------------------------------------------------------



# ----------------------------------------------------------------------------------------------------
def intersectlists_GBDTprotons_Sel2muons( GBDTmodelName, TracksListName , p_score ):
    import csv, pandas as pd
    
    # input: (1) proton list
    GBDTpListName = GBDTclassListName( GBDTmodelName, TracksListName, 'protons', p_score ,'roi')
    print_filename( GBDTpListName , "opening GBDT protons list name:")
    # input: (2) selection2 muons list
    Sel2muListName  = Sel2muons_list_name( TracksListName )
    print_filename( Sel2muListName , "opening Sel2 muons list name:")

    df_GBDTp = pd.read_csv( GBDTpListName , delimiter=' ' )
    df_Sel2mu = pd.read_csv( Sel2muListName , delimiter=' ' )
    df_intersection = pd.merge(df_GBDTp, df_Sel2mu, how='inner', on=['run', 'subrun','event'])
    
    # output: intesection of (1) and (2)
    IntersectionListName = mu_p_intersection_path + "/" + Sel2muons_intersection_list_csv_name( GBDTmodelName ,TracksListName ,p_score )
    df_intersection = df_intersection.rename(columns={ 'trkindex[ivtx][itrk]':'itrk-NuSelMuon',
                                             'ivtx':'ivtx-NuSel', 'trackid':'itrk-GBDTproton'} )
    df_intersection[['run','subrun','event',
                    'ivtx-NuSel','itrk-NuSelMuon',
                    'itrk-GBDTproton']].to_csv(IntersectionListName, sep=' ', header=True, index=False )
    print_filename( IntersectionListName , "intersected Sel2/GBDTprotons lists (%d tracks)"%len(df_intersection))
    print_important( "more %s \nscp %s $uboone:~/"%(IntersectionListName,IntersectionListName) )
    print_line()
# ----------------------------------------------------------------------------------------------------




# ----------------------------------------------------------------------------------------------------
def scheme_list_of_files_rse( GBDTmodelName, TracksListName , p_score ):
    '''
    This functionallity schemes (big) analysis trees
    and returns a tree containing only entries with a Run/Subrun/Event
    of a given list (RSE map)
    '''
    # input: (1) analysis trees
    AnaTreesListName = flags.DataType + "_AnalysisTrees" # AnalysisTreesListName   = anatrees_lists_path + "/GOOD" + flags.DataType + "/filesana.list"
    print_filename( AnaTreesListName , "input: (1) analysis trees ")
    # input: (2) intersected mu-p list
    IntersectionListName    = mu_p_intersection_path + "/" + Sel2muons_intersection_list_csv_name( GBDTmodelName ,TracksListName , p_score )
    print_filename( IntersectionListName , "input (2): intersected Sel2/GBDTprotons lists ")
    
    # output: schemed analysis trees file
    SchemedResultFileName   = schemed_anatrees_file_name( AnaTreesListName , Sel2muons_intersection_list_name( GBDTmodelName ,TracksListName , p_score ) )
    it = ImportantTools()
    files = read_files_from_a_list( AnaTreesListName )
    in_chain = get_analysistrees_chain(files)
    OutFile = ROOT.TFile( SchemedResultFileName , "recreate" )
    OutTree = it.SchemeTreeRSEList( in_chain , IntersectionListName , flags.verbose )

    if flags.verbose:
        print_filename(SchemedResultFileName , "schemed anatrees file (%d events, %.2f MB):"%(OutTree.GetEntries(),float(os.path.getsize(SchemedResultFileName)/1048576.0)))
    
    OutTree.Write()
    OutFile.Close()
# ----------------------------------------------------------------------------------------------------


# ----------------------------------------------------------------------------------------------------
def extract_anatrees_tracks_information_from_files_list( DataType, Option,
                                                        first_anatree_file , last_anatree_file ,
                                                        MCmode=False, AddEventsList=False , EventsListName="" ):
    # flags.DataType options:   openCOSMIC_MC / extBNB / MC_BNB / BNB_5e19POT / single particles....
    
    AnaTreesListName = DataType + "_AnalysisTrees"
    files       = read_files_from_a_list( AnaTreesListName , first_anatree_file , last_anatree_file )
    in_chain    = get_analysistrees_chain(files)
    
    extract_anatrees_tracks_information_with_all_features( in_chain , Option,
                                                          first_anatree_file , last_anatree_file,
                                                          MCmode, AddEventsList , EventsListName , AnaTreesListName )
# ----------------------------------------------------------------------------------------------------


# ----------------------------------------------------------------------------------------------------
def extract_anatrees_tracks_information_from_a_file( DataType, InputFileName, Option,
                                                    MCmode=False,
                                                    TreeName="analysistree/anatree",
                                                    AddEventsList=False ,
                                                    EventsListName="",
                                                    output_mupRSEFileName="" ):
    
    in_chain = ROOT.TChain( TreeName )
    in_chain.Add( InputFileName )
    extract_anatrees_tracks_information_with_all_features( in_chain=in_chain, Option=Option,
                                                          MCmode=MCmode, AddEventsList=AddEventsList,
                                                          EventsListName=EventsListName , AnaTreesListName=DataType+"_AnalysisTrees" ,
                                                          output_mupRSEFileName=output_mupRSEFileName )
# ----------------------------------------------------------------------------------------------------



# ----------------------------------------------------------------------------------------------------
def add_event_list( EventsListName ):
    if flags.verbose:
        print_filename( EventsListName , "adding list of R/S/E from" )
    with open( EventsListName , 'rb') as csvfile:
        reader = csv.reader(csvfile, delimiter=' ', skipinitialspace=True)
        header = next(reader)
        rse_events_list = [dict(zip(header, map(int, row))) for row in reader]
    if flags.verbose>3:
        print rse_events_list
    return rse_events_list
# ----------------------------------------------------------------------------------------------------

# ----------------------------------------------------------------------------------------------------
def do_pass_geometrical_cuts( track ):
    
    # sample of free protons candidates from off-beam data using only geometrical cuts
    if flags.verbose>2:
        
        if track.length>10:
            print 'track length: %.1f cm'%track.length
        else:
            print_important( 'track length: %.1f cm'%track.length )

        print 'track position: (%.1f,%.1f,%.1f)=>(%.1f,%.1f,%.1f)'%(track.start_pos.x(),track.start_pos.y(),track.start_pos.z(), track.end_pos.x(),track.end_pos.y(),track.end_pos.z())

        if track.cftime>2 or track.cftime==-9999:
            print 'track closest-flash time: %.1f ns'%track.cftime
        else:
            print_important( 'track closest-flash time: %.1f ns'%track.cftime )
                        
    # (1) track less then some minimum length 10 cm proton has ~ 800 MeV/c
    length_cut = True if (track.length < 10) else False
    
    # (2) Far away from dead regions
    fiducial_cuts = True if (    track.start_pos.x() < 40  or 210 < track.start_pos.x()
                             or  track.end_pos.x()   < 40  or 210 < track.end_pos.x()
                             or  track.start_pos.y() < -50 or 50  < track.start_pos.y()
                             or  track.end_pos.y()   < -50 or 50  < track.end_pos.y()
                             or  track.start_pos.z() < 420 or 620 < track.start_pos.z()
                             or  track.end_pos.z()   < 420 or 620 < track.end_pos.z()    ) else False
                             
    # (3) Flash-matched 
    flashmatched_cut = True if ( track.cftime < 2 and track.cftime != -9999 ) else False
    # ---------------------------------------------------------------------
    if flags.verbose>4:
        print 'length-cut:',length_cut,'\tfiducial_cuts:' , fiducial_cuts , '\tflashmatched_cut:', flashmatched_cut
    
    if (length_cut and fiducial_cuts and flashmatched_cut):
        return True
    else:
        return False
# ----------------------------------------------------------------------------------------------------

# ----------------------------------------------------------------------------------------------------
def stream_g4_features_to_file ( g4particle , writer_g4 ):

    g4_features = [ g4particle.run  , g4particle.subrun     , g4particle.event  ,
                    g4particle.ig4   , g4particle.TrackId    , g4particle.pdg    ,
                    g4particle.P     , g4particle.Mass       , g4particle.Eng    ,
                    g4particle.KE    , g4particle.theta      , g4particle.phi    ,
                    g4particle.process_primary ,
                    g4particle.start_pos.x() , g4particle.start_pos.y()  , g4particle.start_pos.z(),
                    g4particle.end_pos.x()   , g4particle.end_pos.y()    , g4particle.end_pos.z(),
                    g4particle.length        , g4particle.Mother
                    ]
            
    writer_g4.writerow( ['{:.3f}'.format(x) for x in g4_features]  )
# ----------------------------------------------------------------------------------------------------

# ----------------------------------------------------------------------------------------------------
def stream_tracks_features_to_file ( track , writer ):

    roi_U       , roi_V     , roi_Y     = track.GetROI(0)       , track.GetROI(1)       , track.GetROI(2)
    CaloPDG_U   , CaloPDG_V , CaloPDG_Y = track.GetCaloPDG(0)   , track.GetCaloPDG(1)   , track.GetCaloPDG(2)
        
    track_features = [ track.run                , track.subrun          , track.event           , track.track_id
                      , track.is_flipped        , track.nhits           , track.length
                      , track.start_pos.x()     , track.start_pos.y()   , track.start_pos.z()
                      , track.end_pos.x()       , track.end_pos.y()     , track.end_pos.z()
                      , track.theta             , track.phi             , track.distlenratio
                      , track.start_dqdx        , track.end_dqdx        , track.dqdx_diff       , track.dqdx_ratio
                      , track.tot_dqdx          , track.avg_dqdx
                      , track.cosmicscore       , track.coscontscore    , track.pidpida         , track.pidchi
                      , track.cftime            , track.cftimewidth     , track.cfzcenter       , track.cfzwidth
                      , track.cfycenter         , track.cfywidth        , track.cftotalpe       , track.cfdistance
                      , track.MCpdgCode
                      , roi_U.start_wire        , roi_U.start_time      , roi_U.end_wire        , roi_U.end_time
                      , roi_V.start_wire        , roi_V.start_time      , roi_V.end_wire        , roi_V.end_time
                      , roi_Y.start_wire        , roi_Y.start_time      , roi_Y.end_wire        , roi_Y.end_time
                      , track.purtruth_Y
                      , track.CalorimetryPDG[0], track.CalorimetryPDG[1], track.CalorimetryPDG[2]
                      , track.truth_P          , track.truth_Eng        , track.truth_KE        , track.truth_theta     , track.truth_phi       , track.process_primary
                      , track.truth_start_pos.x()   , track.truth_start_pos.y()     , track.truth_start_pos.z()
                      , track.truth_end_pos.x()     , track.truth_end_pos.y()       , track.truth_end_pos.z()
                      ]
        
    track_features = ['{:.3f}'.format(x) for x in track_features]

    residual_range_Y , dqdx_Y , dEdx_Y , Edep_Y = [] , [] , [] , []

    for step in range(track.GetEdepYNsteps()):
        residual_range_Y.append( track.residual_range_Y.at(step) )
        dqdx_Y.append( track.dqdx_Y.at(step) )
        dEdx_Y.append( track.dEdx_Y.at(step) )
        Edep_Y.append( track.Edep_Y.at(step) )
                        

    track_features.append(residual_range_Y)
    track_features.append(dqdx_Y)
    track_features.append(dEdx_Y)
    track_features.append(Edep_Y)

    writer.writerow( track_features )
# ----------------------------------------------------------------------------------------------------

# ----------------------------------------------------------------------------------------------------
def stream_mu_p_vertex_features_to_file ( ivtx, itrk_mu, itrk_p, calc , writer_mu_p ):
    
    muROI_U, muROI_V, muROI_Y= calc.GetROItrk_NuSelMuon(0) , calc.GetROItrk_NuSelMuon(1) , calc.GetROItrk_NuSelMuon(2)
    pROI_U , pROI_V , pROI_Y = calc.GetROItrk_GBDTproton(0) , calc.GetROItrk_GBDTproton(1) , calc.GetROItrk_GBDTproton(2)
    vROI_U , vROI_V , vROI_Y = calc.Getmu_p_VtxROI(0) , calc.Getmu_p_VtxROI(1) , calc.Getmu_p_VtxROI(2)
    
    mu_p_features = [calc.run           , calc.subrun       , calc.event,
                     ivtx               , itrk_mu           , itrk_p    ,
                     muROI_U.start_wire , muROI_U.start_time, muROI_U.end_wire  , muROI_U.end_time,
                     muROI_V.start_wire , muROI_V.start_time, muROI_V.end_wire  , muROI_V.end_time,
                     muROI_Y.start_wire , muROI_Y.start_time, muROI_Y.end_wire  , muROI_Y.end_time,
                     pROI_U.start_wire  , pROI_U.start_time , pROI_U.end_wire   , pROI_U.end_time,
                     pROI_V.start_wire  , pROI_V.start_time , pROI_V.end_wire   , pROI_V.end_time,
                     pROI_Y.start_wire  , pROI_Y.start_time , pROI_Y.end_wire   , pROI_Y.end_time,
                     vROI_U.start_wire  , vROI_U.start_time , vROI_U.end_wire   , vROI_U.end_time,
                     vROI_V.start_wire  , vROI_V.start_time , vROI_V.end_wire   , vROI_V.end_time,
                     vROI_Y.start_wire  , vROI_Y.start_time , vROI_Y.end_wire   , vROI_Y.end_time
                     ]
                     
    writer_mu_p.writerow( ['{:.3f}'.format(x) for x in mu_p_features] )
# ----------------------------------------------------------------------------------------------------



# ----------------------------------------------------------------------------------------------------
def extract_anatrees_tracks_information_with_all_features( in_chain, Option,
                                                          first_anatree_file=0, last_anatree_file=0,
                                                          MCmode=False,
                                                          AddEventsList=False,
                                                          EventsListName="",
                                                          AnaTreesListName="",
                                                          output_mupRSEFileName="" ):
    
    import csv
    if 'extract all tracks information' not in Option and 'find common muon-proton vertices' not in Option:
        print "Problem, can not extract!\noptions are:"
        print "\t extract all tracks information"
        print "\t find common muon-proton vertices"
        print "\t add hard geometrical cuts"
        print "\t exiting..."
        exit(0)

    g4_counter , counter = 0 , 0

    FeaturesFileName    = tracks_features_file_name( AnaTreesListName , first_anatree_file , last_anatree_file )
    TracksAnaFileName   = tracks_anafile_name( AnaTreesListName , first_anatree_file , last_anatree_file )
    resutls_file_name   = tracks_full_features_file_name( AnaTreesListName , first_anatree_file , last_anatree_file )
    g4info_file_name    = g4_features_file_name( AnaTreesListName , first_anatree_file , last_anatree_file )
    rois_file_name      = rois_features_file_name( AnaTreesListName , first_anatree_file , last_anatree_file )

    writer = csv.writer(open(resutls_file_name, 'wb'))
    if first_anatree_file==0:
        writer.writerow( track_features_names )

    if MCmode:
        writer_g4 = csv.writer(open(g4info_file_name, 'wb'))
        if first_anatree_file==0:
            writer_g4.writerow( g4_features_names )

    if Option=="find common muon-proton vertices":
        output_rse_file = open( output_mupRSEFileName , 'w' )
        writer_mu_p = csv.writer(open(rois_file_name, 'wb'))
        if first_anatree_file==0:
            writer_mu_p.writerow( mu_p_features_names )
    
    Nentries    = in_chain.GetEntries()
    Nreduced    = int(flags.evnts_frac*(Nentries))
    OutFile     = ROOT.TFile(TracksAnaFileName,"recreate")
    TracksTree , GENIETree  = ROOT.TTree("TracksTree","pandoraNu tracks") , ROOT.TTree("GENIETree","genie interactions")

    calc = cumputeAnaTree( in_chain, TracksTree, FeaturesFileName, Option, flags.verbose, MCmode, GENIETree )
    
    if AddEventsList:
        rse_events_list = add_event_list( EventsListName )

    if flags.verbose: print_important( "starting run on %d events"%Nreduced )

# - # main events loop
    # main events loop
    for entry in range(Nreduced):
        
        do_continue = True
        calc.GetEntry( entry )
        entry_rse = [calc.run,calc.subrun,calc.event]
        if flags.verbose>3: print entry_rse
        
        if AddEventsList:
            
            do_continue , ivtx_nuselection , itrk_NuSelMuon , itrk_GBDTproton = search_rse( entry_rse , rse_events_list )
            if (do_continue and flags.verbose>1): print_important("found r-%d/s-%d/e-%d, extracting information....\n"%(calc.run,calc.subrun,calc.event))
        
# - # - # do-continue for finding RSE
        if do_continue:
            
            calc.extract_information()
            
            # geant4 particles
            if MCmode:
                for i in range(calc.Ng4particles):
                    g4particle = calc.GetG4Particle(i)
                    stream_g4_features_to_file ( g4particle , writer_g4 )
                    g4_counter = g4_counter + 1
            # end geant4 particles


            if (flags.verbose and entry%flags.print_mod==0):
                
                calc.PrintData( entry )
            
            if "extract all tracks information" in Option:
                
                do_continue , ivtx_nuselection , itrk_NuSelMuon , itrk_GBDTproton = True , 0 , 0 , 0
            
            if "find common muon-proton vertices" in Option:
                
                do_continue = True if ( itrk_NuSelMuon != itrk_GBDTproton and calc.TrkVtxDistance( ivtx_nuselection , itrk_GBDTproton ) < min_trk_vtx_distance ) else False
        
            if flags.verbose>3: print 'loooping over Ntracks=',calc.Ntracks,'contained tracks in this event'

# - # - # - # if Ntracks > 0
            if calc.Ntracks>0 and do_continue:
                
                if "find common muon-proton vertices" in Option:
                    #                calc.Write2CSV( ivtx_nuselection , itrk_NuSelMuon , itrk_GBDTproton )
                    output_rse_file.write( "%d %d %d\n"%(calc.run, calc.subrun, calc.event ))
                    calc.CreateROIsCCQE( ivtx_nuselection , itrk_NuSelMuon , itrk_GBDTproton )
                    stream_mu_p_vertex_features_to_file ( ivtx_nuselection , itrk_NuSelMuon , itrk_GBDTproton , calc , writer_mu_p )

# - # - # - # - # for i in range(calc.Ntracks)
                for i in range(calc.Ntracks):
                    
                    track = calc.GetTrack(i)
                    if flags.verbose>4: print 'grabbed track',i

                    if "add hard geometrical cuts" in Option:
                        
                        do_continue = do_pass_geometrical_cuts(track)
                        if flags.verbose>4: print 'do_continue:', do_continue

                    if do_continue:
                        
                        stream_tracks_features_to_file ( track , writer )
                        if flags.verbose>2: print 'saving track to file from R/S/E ',calc.run,calc.subrun,calc.event
                        counter = counter+1
                    
                    if flags.verbose>2: print_line()
# - # - # - # end if Ntracks > 0
# - # - # - # - # end for i in range(calc.Ntracks)
                calc.FillOutTree()
# - # - # end do-continue for finding RSE
# - # end main events loop

    print_filename( FeaturesFileName , "wrote csv file with %d tracks (%.2f MB)"%(counter,float(os.path.getsize(FeaturesFileName)/1048576.0)) )
    print_filename( resutls_file_name , "wrote csv file with all %d tracks features (%.2f MB)"%(counter,float(os.path.getsize(resutls_file_name)/1048576.0)) )
    print_filename( TracksAnaFileName , "wrote root file (%.2f MB)"%float(os.path.getsize(TracksAnaFileName)/1048576.0) )
    
    if Option=="find common muon-proton vertices":
        print_filename( output_mupRSEFileName , "output RSE map for argofiltering muon-proton vertices" )
        print_filename( rois_file_name , "muon / proton ROIs ")
        output_rse_file.close()

    if MCmode:
        print_filename( resutls_file_name , "wrote csv file with all %d g4 generated tracks (%.2f MB)"%(g4_counter,float(os.path.getsize(g4info_file_name)/1048576.0)) )
        GENIETree.Write()
    
    TracksTree.Write()
    OutFile.Close()
# ----------------------------------------------------------------------------------------------------




