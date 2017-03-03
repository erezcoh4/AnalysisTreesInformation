import sys
sys.path.insert(0, '/uboone/app/users/ecohen/larlite/UserDev/GBDTprotons/GBDTprotonPackage/mac')
sys.path.insert(0, '/Users/erezcohen/larlite/UserDev/GBDTprotons/GBDTprotonPackage/mac')
sys.path.insert(0, '/home/erez/larlite/UserDev/GBDTprotons/GBDTprotonPackage/mac')
sys.path.insert(0, '/uboone/app/users/ecohen/larlite/UserDev/mySoftware/MySoftware/mac')
from uboone_tools import *
from gbdt_tools import *
from my_tools import *
from larlite import *
from ROOT import PandoraNuTrack
from ROOT import cumputeAnaTree
from ROOT import ImportantTools
from ROOT import AnalyzeTracksFile


# globals
# -------------------------
max_trk_vtx_distance = 10 # [cm], this distance needs to be decided form vertex reconstruction resolution
first_file  = flags.run
splitjobs   = 1000 if flags.NumberOfRuns==0 else flags.NumberOfRuns # splitting the jobs: 0-10, 10-20, 20-30,....
last_file   = first_file + splitjobs
MCCversion  = "MCC%d"%flags.MCCversion





# paths
# -------------------------

neutrinoSel2_path       = lists_path + "/NeutrinoSelection2"
mu_p_intersection_path  = lists_path + "/muon_proton_intersection"
schemed_anatrees_path   = anatrees_data_path  + "/SchemedFiles"



g4_counter , counter , cosmic_counter , evts_counter = 0 , 0 , 0 , 0
run , subrun , event , Ntracks , Ng4particles, nu_interactions, tracks, g4particles, genie_interactions = 0,0,0,0,0,0,0,0,0
eventsTree , GENIETree = None, None


event_features_names = [ 'run'  ,'subrun'   ,'event'
                        ,'MCmode'
                        ,'Ntracks_kalmanhit'
                        ,'Ntracks_pandoraNu'            ,'Ncosmictracks_pandoraCosmic'
                        ,'Ntracks_pandoraNu_Contained'  ,'Ncosmictracks_pandoraCosmic_Contained'
                        ,'Ng4particles'
                        ,'nvtx_pandoraCosmic'
                        ,'Ncosmictracks_ymax'
                        ]


g4_features_names = [ 'run'         ,'subrun'       ,'event'
                     ,'ig4'         ,'track_id'     ,'pdg'
                     ,'P'           ,'Mass'         ,'Eng'
                     ,'KE'          ,'theta'        ,'phi'
                     ,'primary'
                     ,'startx'      ,'starty'       ,'startz'
                     ,'endx'        ,'endy'         ,'endz'
                     ,'length'      ,'Mother'       ,'truth_ccnc'
                     ,'reconstructed'
                     ,'rec_nhits'   , 'rec_is_flipped'
                     ,'rec_startx'  , 'rec_starty'  , 'rec_startz'
                     ,'rec_endx'    , 'rec_endy'    , 'rec_endz'
                     ,'rec_length'       , 'rec_theta'     , 'rec_phi'
                     ,'rec_distlenratio' , 'rec_momentum'
                     ,'rec_start_dqdx'   , 'rec_end_dqdx'  , 'rec_tot_dqdx'
                     ,'rec_avg_dqdx'     , 'rec_dqdx_diff' , 'rec_dqdx_ratio'
                     ,'rec_pidpida'      , 'rec_pidchi'    , 'rec_cosmicscore'
                     ,'rec_coscontscore' , 'rec_cftime'
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
                        ,'truth_endx'   ,'truth_endy'   ,'truth_endz' ,'truth_ccnc'
                        ,'residual_range_Y'
                        ,'dqdx_Y'
                        ,'dEdx_Y'
                        ,'Edep_Y'
                        ,'swtrigger_name'
                        ,'swtrigger_triggered'
                        ]


mu_p_features_names = ['run','subrun','event',
                       'ivtx','itrk_NuSelMuon','itrk_GBDTproton',
                       'mu_U_start_wire','mu_U_start_time','mu_U_end_wire','mu_U_end_time',
                       'mu_V_start_wire','mu_V_start_time','mu_V_end_wire','mu_V_end_time',
                       'mu_Y_start_wire','mu_Y_start_time','mu_Y_end_wire','mu_Y_end_time',
                       'p_U_start_wire','p_U_start_time','p_U_end_wire','p_U_end_time',
                       'p_V_start_wire','p_V_start_time','p_V_end_wire','p_V_end_time',
                       'p_Y_start_wire','p_Y_start_time','p_Y_end_wire','p_Y_end_time',
                       'v_U_start_wire','v_U_start_time','v_U_end_wire','v_U_end_time',
                       'v_V_start_wire','v_V_start_time','v_V_end_wire','v_V_end_time',
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
    return Sel2muons_intersection_list_name( GBDTmodelName, TracksListName, p_score ) + "_mindistance_%dcm.csv"%max_trk_vtx_distance
# ----------------------------------------------------------------------------------------------------
# ----------------------------------------------------------------------------------------------------
def good_mu_p_roi_list_name( GBDTmodelName, TracksListName, p_score ):
    return "ROIs_" + Sel2muons_intersection_list_name( GBDTmodelName, TracksListName, p_score ) + "_mindistance_%dcm.csv"%max_trk_vtx_distance
# ----------------------------------------------------------------------------------------------------


# ----------------------------------------------------------------------------------------------------
def schemed_anatrees_file_name( anatrees_list_name , scheming_name ):
    return schemed_anatrees_path + "/" + anatrees_list_name + "_" + scheming_name + ".root"
# ----------------------------------------------------------------------------------------------------


# ----------------------------------------------------------------------------------------------------
def tracks_features_file_name( ListName , first_file = 0 , last_file = 0 ):
    if first_file==last_file:
        return featuresfiles_path + "/" + "features_" + ListName + ".csv"
    else:
        return featuresfiles_path + "/" + "features_" + ListName + "_anatreefiles_%d_to_%d.csv"%(first_file,last_file)
# ----------------------------------------------------------------------------------------------------

# ----------------------------------------------------------------------------------------------------
def events_features_file_name( ListName , first_file = 0 , last_file = 0 ):
    if first_file==last_file:
        return featuresfiles_path + "/" + "events_features_" + ListName + ".csv"
    else:
        return featuresfiles_path + "/" + "events_features_" + ListName + "_anatreefiles_%d_to_%d.csv"%(first_file,last_file)
# ----------------------------------------------------------------------------------------------------

# ----------------------------------------------------------------------------------------------------
def tracks_full_features_file_name( ListName , first_file = 0 , last_file = 0 ):
    if first_file==last_file:
        return featuresfiles_path + "/" + "full_features_" + ListName + ".csv"
    else:
        return featuresfiles_path + "/" + "full_features_" + ListName + "_anatreefiles_%d_to_%d.csv"%(first_file,last_file)
# ----------------------------------------------------------------------------------------------------

# ----------------------------------------------------------------------------------------------------
def cosmic_features_file_name( ListName , first_file = 0 , last_file = 0 ):
    if first_file==last_file:
        return featuresfiles_path + "/" + "cosmic_features_" + ListName + ".csv"
    else:
        return featuresfiles_path + "/" + "cosmic_features_" + ListName + "_anatreefiles_%d_to_%d.csv"%(first_file,last_file)
# ----------------------------------------------------------------------------------------------------


# ----------------------------------------------------------------------------------------------------
def g4_features_file_name( ListName , first_file = 0 , last_file = 0 ):
    if first_file==last_file:
        return featuresfiles_path + "/" + "g4_features_" + ListName + ".csv"
    else:
        return featuresfiles_path + "/" + "g4_features_" + ListName + "_anatreefiles_%d_to_%d.csv"%(first_file,last_file)
# ----------------------------------------------------------------------------------------------------


# ----------------------------------------------------------------------------------------------------
def rois_features_file_name( ListName , first_file = 0 , last_file = 0 ):
    if first_file==last_file:
        return rois_path + "/" + "rois_" + ListName + ".csv"
    else:
        return rois_path + "/" + "rois_" + ListName + "_anatreefiles_%d_to_%d.csv"%(first_file,last_file)
# ----------------------------------------------------------------------------------------------------



# ----------------------------------------------------------------------------------------------------
def tracks_anafile_name( ListName , first_file = 0 , last_file = 0 ):
    if first_file==last_file:
        return eventsfiles_path + "/" + "Events_" + ListName + ".root"
    else:
        return eventsfiles_path + "/" + "Events_" + ListName + "_anatreefiles_%d_to_%d.root"%(first_file,last_file)
# ----------------------------------------------------------------------------------------------------



# ----------------------------------------------------------------------------------------------------
def ccqe_candidates_filename( data_name=None ):
    return ccqe_candidates_path + "/" + "ccqe_candidates_" + data_name + ".csv"
# ----------------------------------------------------------------------------------------------------


# methods
# ----------------------------------------------------------------------------------------------------
def read_files_from_a_list( ListName , first_file = 0 , last_file = 0 , MCCversion="MCC7" ): #{
    # returns the files
    list_full_name = lists_path + "/analysis_trees/" + MCCversion + "/" + ListName + ".list"
    
    if flags.verbose:
        print_filename( list_full_name, "reading list of files (%d to %d)..."%(first_file , last_file))
    
    with open( list_full_name ) as f:
        files = f.read().splitlines()
    if last_file > first_file:
        files = files[ first_file : last_file ]

    return files
#}
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
def scheme_anatrees_single_file( in_anatrees_file=None, in_treename=None, rse_mapname=None, outfname=None , MCCversion="MCC7"):
    '''
        This functionallity schemes a single analysis trees file
        and returns a tree containing only entries with a Run/Subrun/Event
        of a given list (RSE map)
        '''
    print_filename( in_anatrees_file , "input (1): analysis trees ")
    
    it = ImportantTools()
    in_chain = ROOT.TChain(in_treename)
    in_chain.Add( in_anatrees_file )
    if flags.verbose: print "input chain entries from "+in_anatrees_file + ": ",in_chain    .GetEntries()

    rsemap2selectfrom = neutrinoSel2_path + "/" + MCCversion + "/" + MCCversion + "_"+ rse_mapname + ".list"
    print_filename( rsemap2selectfrom , "input (2): RSE list to select from ")
    
    OutFile = ROOT.TFile( outfname , "recreate" )
    OutTree = it.SchemeTreeRSEList( in_chain , rsemap2selectfrom , flags.verbose )

    print_filename(outfname , "schemed anatrees (%d events, %.2f MB):"%(OutTree.GetEntries(),filesize_in_MB(outfname)))

    OutTree.Write()
    OutFile.Close()
# ----------------------------------------------------------------------------------------------------

# ----------------------------------------------------------------------------------------------------
def scheme_anatrees_files( anatrees_listname=None, rse_mapname=None, outfname=None , MCCversion="MCC7"):
    '''
        This functionallity schemes (big) analysis trees
        and returns a tree containing only entries with a Run/Subrun/Event
        of a given list (RSE map)
        '''
    print_filename( anatrees_listname , "input (1): analysis trees ")
    print_filename( rse_mapname , "input (2): RSE list to select from ")
    
    it = ImportantTools()
    files = read_files_from_a_list( ListName = anatrees_listname )
    rsemap2selectfrom = neutrinoSel2_path + "/" + MCCversion + "/" + rse_mapname + ".list"
    
    in_chain = get_analysistrees_chain(files)
    OutFile = ROOT.TFile( outfname , "recreate" )
    OutTree = it.SchemeTreeRSEList( in_chain , rse_mapname , flags.verbose )
    
    print_filename(outfname , "schemed anatrees (%d events, %.2f MB):"%(OutTree.GetEntries(),filesize_in_MB(outfname)))
    
    OutTree.Write()
    OutFile.Close()
# ----------------------------------------------------------------------------------------------------


# ----------------------------------------------------------------------------------------------------
def scheme_list_of_files_rse( GBDTmodelName, TracksListName , p_score ):
    '''
    This functionallity schemes (big) analysis trees
    and returns a tree containing only entries with a Run/Subrun/Event
    of a given list (RSE map)
    '''
    # input: (1) analysis trees
    anatrees_listname = flags.DataType + "_AnalysisTrees" # AnalysisTreesListName = anatrees_lists_path + "/GOOD" + flags.DataType + "/filesana.list"
    print_filename( anatrees_listname , "input: (1) analysis trees ")
    # input: (2) intersected mu-p list
    IntersectionListName    = mu_p_intersection_path + "/" + Sel2muons_intersection_list_csv_name( GBDTmodelName ,TracksListName , p_score )
    print_filename( IntersectionListName , "input (2): intersected Sel2/GBDTprotons lists ")
    
    # output: schemed analysis trees file
    SchemedResultFileName   = schemed_anatrees_file_name( anatrees_listname , Sel2muons_intersection_list_name( GBDTmodelName ,TracksListName , p_score ) )
    it = ImportantTools()
    files = read_files_from_a_list( anatrees_listname )
    in_chain = get_analysistrees_chain(files)
    OutFile = ROOT.TFile( SchemedResultFileName , "recreate" )
    OutTree = it.SchemeTreeRSEList( in_chain , IntersectionListName , flags.verbose )

    if flags.verbose:
        print_filename(SchemedResultFileName , "schemed anatrees file (%d events, %.2f MB):"%(OutTree.GetEntries(),float(os.path.getsize(SchemedResultFileName)/1048576.0)))
    
    OutTree.Write()
    OutFile.Close()
# ----------------------------------------------------------------------------------------------------



# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~
# extract data from anatrees
# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~
## ----------------------------------------------------------------------------------------------------
# previous version
#def extract_anatrees_tracks_information_from_files_list(data_type="BNB_5e19POT",
#                                                        Option="extract all tracks information",
#                                                        first_file=0 , last_file=0 ,
#                                                        MCmode=False,
#                                                        AddEventsList=False ,
#                                                        EventsListName="" ,
#                                                        MCCversion="MCC7" ,
#                                                        do_pandora_cosmic=False ):
#    
#    data_name = MCCversion + "_" + data_type
#    anatrees_listname = data_name + "_AnalysisTrees"
#    print_filename(anatrees_listname , "reading anatrees listname")
#    files       = read_files_from_a_list( ListName=anatrees_listname, first_file=first_file, last_file=last_file, MCCversion=MCCversion )
#    in_chain    = get_analysistrees_chain( files )
#    
#    extract_anatrees_information(in_chain=in_chain , Option=Option,
#                                 first_file=first_file , last_file=last_file, MCmode=MCmode,
#                                 anatrees_listname=anatrees_listname ,
#                                 do_pandora_cosmic=do_pandora_cosmic)
#    if debug: print 'finished extract_anatrees_information'
## ----------------------------------------------------------------------------------------------------




# ----------------------------------------------------------------------------------------------------
def extract_anatrees_tracks_information_from_a_file( DataType, InputFileName, Option,
                                                    MCmode=False,
                                                    TreeName="analysistree/anatree",
                                                    AddEventsList=False ,
                                                    EventsListName="",
                                                    output_mupRSEFileName="",
                                                    output_mupROIFileName=""):
    
    in_chain = ROOT.TChain( TreeName )
    in_chain.Add( InputFileName )
    extract_anatrees_tracks_information_with_all_features( in_chain=in_chain, Option=Option,
                                                          MCmode=MCmode, AddEventsList=AddEventsList,
                                                          EventsListName=EventsListName ,
                                                          anatrees_listname=DataType+"_AnalysisTrees" ,
                                                          output_mupRSEFileName=output_mupRSEFileName ,
                                                          output_mupROIFileName=output_mupROIFileName )
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

        print 'track position: (%.1f,%.1f,%.1f)=>(%.1f,%.1f,%.1f)'%(track.startx,track.starty,track.startz, track.endx,track.endy,track.endz)

        if track.cftime>2 or track.cftime==-9999:
            print 'track closest-flash time: %.1f ns'%track.cftime
        else:
            print_important( 'track closest-flash time: %.1f ns'%track.cftime )
                        
    # (1) track less then some minimum length 10 cm proton has ~ 800 MeV/c
    length_cut = True if (track.length < 10) else False
    
    # (2) Far away from dead regions
    fiducial_cuts = True if (    track.startx < 40  or 210 < track.startx
                             or  track.endx   < 40  or 210 < track.endx
                             or  track.starty < -50 or 50  < track.starty
                             or  track.endy   < -50 or 50  < track.endy
                             or  track.startz < 420 or 620 < track.startz
                             or  track.endz   < 420 or 620 < track.endz    ) else False
                             
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
def stream_event_features_to_file( calc , writer , do_pandora_cosmic=False  ):
    event_features = [ calc.run                 , calc.subrun           , calc.event
                      , calc.MCmode
                      , calc.ntracks_trackkalmanhit
                      , calc.ntracks_pandoraNu  , calc.ntracks_pandoraCosmic
                      , calc.Ntracks            , calc.Ncosmictracks    , calc.Ng4particles
                      , calc.nvtx_pandoraCosmic
                      ]

    event_features = ['{:.0f}'.format(x) for x in event_features]
    
    if do_pandora_cosmic:
        Ncosmictracks_ymax = []
        if calc.Nymax_cosmic>0:
            for i in range(calc.Nymax_cosmic):
                Ncosmictracks_ymax.append( calc.Ncosmictracks_ymax.at(i) )
        event_features.append(Ncosmictracks_ymax)

    writer.writerow( event_features )
# ----------------------------------------------------------------------------------------------------

# ----------------------------------------------------------------------------------------------------
def stream_g4_features_to_file ( g4particle , g4_writer ):

    g4_features = [ g4particle.run      , g4particle.subrun     , g4particle.event  ,
                    g4particle.ig4      , g4particle.TrackId    , g4particle.pdg    ,
                    g4particle.P        , g4particle.Mass       , g4particle.Eng    ,
                    g4particle.KE       , g4particle.theta      , g4particle.phi    ,
                    g4particle.process_primary ,
                    g4particle.startx , g4particle.starty  , g4particle.startz,
                    g4particle.endx   , g4particle.endy    , g4particle.endz,
                    g4particle.length        , g4particle.Mother         , g4particle.ccnc,

                    g4particle.reconstructed    ,
                    g4particle.rec_nhits        , g4particle.rec_is_flipped    ,
                    g4particle.rec_startx, g4particle.rec_starty , g4particle.rec_startz ,
                    g4particle.rec_endx  , g4particle.rec_endy   , g4particle.rec_endz   ,
                    g4particle.rec_length       , g4particle.rec_theta     , g4particle.rec_phi ,
                    g4particle.rec_distlenratio , g4particle.rec_momentum  ,
                    g4particle.rec_start_dqdx   , g4particle.rec_end_dqdx  , g4particle.rec_tot_dqdx ,
                    g4particle.rec_avg_dqdx     , g4particle.rec_dqdx_diff , g4particle.rec_dqdx_ratio ,
                    g4particle.rec_pidpida      , g4particle.rec_pidchi    , g4particle.rec_cosmicscore ,
                    g4particle.rec_coscontscore , g4particle.rec_cftime
                   ]
            
    g4_writer.writerow( ['{:.3f}'.format(x) for x in g4_features]  )
# ----------------------------------------------------------------------------------------------------

# ----------------------------------------------------------------------------------------------------
def stream_tracks_features_to_file ( track , writer , do_dEdx=False , do_SWtrigger=False ):

    roi_U       , roi_V     , roi_Y     = track.GetROI(0)       , track.GetROI(1)       , track.GetROI(2)
    CaloPDG_U   , CaloPDG_V , CaloPDG_Y = track.GetCaloPDG(0)   , track.GetCaloPDG(1)   , track.GetCaloPDG(2)
        
    track_features = [ track.run                , track.subrun          , track.event           , track.track_id
                      , track.is_flipped        , track.nhits           , track.length
                      , track.startx     , track.starty   , track.startz
                      , track.endx       , track.endy     , track.endz
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
                      , track.truth_P          , track.truth_Eng        , track.truth_KE        , track.truth_theta     , track.truth_phi
                      , track.process_primary
                      , track.truth_startx   , track.truth_starty     , track.truth_startz
                      , track.truth_endx     , track.truth_endy       , track.truth_endz
                      , track.truth_ccnc
                      ]
        
    track_features = ['{:.3f}'.format(x) for x in track_features]

    residual_range_Y , dqdx_Y , dEdx_Y , Edep_Y = [] , [] , [] , []

    if do_dEdx:
        for step in range(track.NEdepYsteps):
            residual_range_Y.append( track.residual_range_Y.at(step) )
            dqdx_Y.append( track.dqdx_Y.at(step) )
            dEdx_Y.append( track.dEdx_Y.at(step) )
            Edep_Y.append( track.Edep_Y.at(step) )


    track_features.append(residual_range_Y)
    track_features.append(dqdx_Y)
    track_features.append(dEdx_Y)
    track_features.append(Edep_Y)

    SWtrigName , SSWtrigTriggered = [] , []
    if do_SWtrigger:
        for trigger in range(1):
            SWtrigName.append( track.swtrigger_name.at(trigger) )
            SSWtrigTriggered.append( track.swtrigger_triggered.at(trigger) )

    track_features.append(SWtrigName)
    track_features.append(SSWtrigTriggered)


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
                     
    writer_mu_p.writerow( mu_p_features )
# ----------------------------------------------------------------------------------------------------



# ----------------------------------------------------------------------------------------------------
def extract_anatrees_tracks_information_with_all_features(in_chain,
                                                          Option,
                                                          anatrees_listname="",
                                                          first_file=0, last_file=0,
                                                          MCmode=False,
                                                          AddEventsList=False,
                                                          EventsListName="",
                                                          output_mupRSEFileName="",
                                                          output_mupROIFileName="" ,
                                                          do_pandora_cosmic=False ,
                                                          do_dEdx=False, do_SWtrigger=False ):
    
    import csv
    if 'extract all tracks information' not in Option and 'find common muon-proton vertices' not in Option:
        print "Problem, can not extract!\noptions are:"
        print "\t extract all tracks information"
        print "\t find common muon-proton vertices"
        print "\t add hard geometrical cuts"
        print "\t exiting..."
        exit(0)

    g4_counter , counter , cosmic_counter , evts_counter = 0 , 0 , 0 , 0

    TracksAnaFileName   = tracks_anafile_name( anatrees_listname , first_file , last_file )
    events_file_name    = events_features_file_name( anatrees_listname , first_file , last_file )
    results_file_name   = tracks_full_features_file_name( anatrees_listname , first_file , last_file )
    cosmics_file_name   = cosmic_features_file_name( anatrees_listname , first_file , last_file )
    g4info_file_name    = g4_features_file_name( anatrees_listname , first_file , last_file )

    writer = csv.writer(open(results_file_name, 'wb'))
    if first_file==0:
        writer.writerow( track_features_names )

    if MCmode:
        g4_writer = csv.writer(open(g4info_file_name, 'wb'))
        if first_file==0:
            g4_writer.writerow( g4_features_names )

    if do_pandora_cosmic:
        cosmic_writer = csv.writer(open(cosmics_file_name, 'wb'))
        if first_file==0:
            cosmic_writer.writerow( track_features_names )

    events_writer = csv.writer(open(events_file_name, 'wb'))
    if first_file==0:
        events_writer.writerow( event_features_names )


    if Option=="find common muon-proton vertices":
        output_rse_file = open( output_mupRSEFileName , 'w' )
        writer_mu_p = csv.writer(open(output_mupROIFileName, 'wb'))
        if first_file==0:
            writer_mu_p.writerow( mu_p_features_names )
    
    Nentries    = in_chain.GetEntries()
    Nreduced    = int(flags.evnts_frac*(Nentries))
    OutFile     = ROOT.TFile(TracksAnaFileName,"recreate")
    eventsTree , GENIETree  = ROOT.TTree("eventsTree","events with all pandoraNu tracks") , ROOT.TTree("GENIETree","genie interactions")

    calc = cumputeAnaTree( in_chain, eventsTree,
                          Option, flags.verbose, MCmode, GENIETree , do_pandora_cosmic )
    
    if AddEventsList:
        rse_events_list = add_event_list( EventsListName )

    if flags.verbose: print_important( "starting run on %d events"%Nreduced )

# - # main events loop
    for entry in range(Nreduced): #{
    
        do_continue = True
        calc.GetEntry( entry )
        entry_rse = [calc.run,calc.subrun,calc.event]
        if flags.verbose>3: print entry_rse
        
        if AddEventsList: #{
            
            do_continue , ivtx_nuselection , itrk_NuSelMuon , itrk_GBDTproton = search_rse( entry_rse , rse_events_list )
            if (do_continue and flags.verbose>1): print_important("found r-%d/s-%d/e-%d, extracting information....\n"%(calc.run,calc.subrun,calc.event))
        #}
        
# - # - # do-continue for finding RSE
        if do_continue: #{
    
            if flags.verbose>2: print 'extract information'
            calc.extract_information( True )
            
            # event features
            if flags.verbose>2: print 'stream event features to file'
            stream_event_features_to_file ( calc , events_writer )
            evts_counter += 1
            # end event features
            
            # geant4 particles
            if MCmode:
                for i in range(calc.Ng4particles):
                    g4particle = calc.GetG4Particle(i)
                    stream_g4_features_to_file ( g4particle , g4_writer )
                    g4_counter = g4_counter + 1
            # end geant4 particles


            if (flags.verbose and entry%flags.print_mod==0):
                
                calc.PrintData( entry )
            
            if "extract all tracks information" in Option:
                
                do_continue , ivtx_nuselection , itrk_NuSelMuon , itrk_GBDTproton = True , 0 , 0 , 0
            
            if "find common muon-proton vertices" in Option:
                
                do_continue = True if ( calc.IsGoodTrack( itrk_NuSelMuon ) and calc.IsGoodTrack( itrk_GBDTproton ) # tracks are contained
                                       and itrk_NuSelMuon != itrk_GBDTproton
                                       and calc.TrkVtxDistance( ivtx_nuselection , itrk_GBDTproton ) < max_trk_vtx_distance ) else False
        
            if flags.verbose>3: print 'looping over Ntracks=',calc.Ntracks,'contained tracks in this event'



# - # - # - # if NCosmicTracks > 0
            if do_pandora_cosmic and calc.Ncosmictracks>0 and do_continue:
                
                # - # - # - # - # for i in range(Ncosmictracks)
                for i in range(calc.Ncosmictracks):
                    
                    cosmic_track = calc.GetCosmicTrack(i)
                    if flags.verbose>4: print 'grabbed cosmic track',i
                        
                    stream_tracks_features_to_file ( cosmic_track , cosmic_writer , do_dEdx=do_dEdx , do_SWtrigger=do_SWtrigger )
                    cosmic_counter += 1
                    
                    if flags.verbose>2:
                        print 'saved cosmic track R-%d/S-%d/E-%d (%d)'%(calc.run,calc.subrun,calc.event,cosmic_track.track_id)
                        print_line()
# - # - # - # end if NCosmicTracks > 0
# - # - # - # - # end for i in range(calc.Ntracks)



# - # - # - # if Ntracks > 0
            if calc.Ntracks>0 and do_continue:
                
                if "find common muon-proton vertices" in Option:
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
                        
                        stream_tracks_features_to_file ( track , writer , do_dEdx=do_dEdx , do_SWtrigger=do_SWtrigger )
                        if flags.verbose>2:
                            print 'saving track to file from R/S/E ',calc.run,calc.subrun,calc.event
                            print_line()
                        counter = counter+1
# - # - # - # end if Ntracks > 0
# - # - # - # - # end for i in range(calc.Ntracks)
                calc.FillOutTree()
# - # - # end do-continue for finding RSE

        #}


    #}

# - # end main events loop

    #    print_filename( FeaturesFileName , "wrote csv file with %d tracks (%.2f MB)"%(counter,float(os.path.getsize(FeaturesFileName)/1048576.0)) )
    
    print_filename( events_file_name, "%d events (%.2f MB)"%(evts_counter,filesize_in_MB(events_file_name)) )
    print_filename( results_file_name , "%d tracks features (%.2f MB)"%(counter,filesize_in_MB(results_file_name) ) )
    print_filename( TracksAnaFileName , "root file (%.2f MB)"%filesize_in_MB(TracksAnaFileName) )

    if do_pandora_cosmic:
        print_filename( cosmics_file_name , "%d cosmic tracks features (%.2f MB)"%(cosmic_counter,filesize_in_MB(cosmics_file_name) ) )

    if Option=="find common muon-proton vertices":
        print_filename( output_mupRSEFileName , "output RSE map for argofiltering muon-proton vertices" )
        print_filename( output_mupROIFileName , "muon / proton ROIs ")
        output_rse_file.close()

    if MCmode:
        print_filename( results_file_name , "%d g4 generated particles (%.2f MB)"%(g4_counter,filesize_in_MB(g4info_file_name)) )
        GENIETree.Write()
    
    eventsTree.Write()
    OutFile.Close()
# ----------------------------------------------------------------------------------------------------





# ----------------------------------------------------------------------------------------------------
def create_csv_writers( events_file_name, results_file_name, cosmics_file_name, g4info_file_name,
                        MCmode=False,do_pandora_cosmic=False ):#{

    events_writer, tracks_writer, cosmic_writer, g4_writer = None, None, None, None

    # event features file
    events_writer = csv.writer(open(events_file_name, 'wb'))
    if first_file==0: events_writer.writerow( event_features_names )
    
    # pandoraNu tracks features file
    tracks_writer = csv.writer(open(results_file_name, 'wb'))
    if first_file==0: tracks_writer.writerow( track_features_names )
    
    # pandoraCosmic tracks features file
    if do_pandora_cosmic:
        cosmic_writer = csv.writer(open(cosmics_file_name, 'wb'))
        if first_file==0: cosmic_writer.writerow( track_features_names )
    
    # MC mode - g4 features
    if MCmode:
        g4_writer = csv.writer(open(g4info_file_name, 'wb'))
        if first_file==0: g4_writer.writerow( g4_features_names )

    return events_writer, tracks_writer, cosmic_writer, g4_writer
#}
# ----------------------------------------------------------------------------------------------------





# ----------------------------------------------------------------------------------------------------
# new version, March-01, loop over the input files without chaining them first..
def extract_anatrees_tracks_information_from_files_list(data_type="BNB_5e19POT",
                                                        Option="extract all tracks information",
                                                        first_file=0 , last_file=0 ,
                                                        MCmode=False,
                                                        AddEventsList=False ,
                                                        EventsListName="" ,
                                                        MCCversion="MCC7" ,
                                                        do_pandora_cosmic=False ): #{

    global g4_counter , counter , cosmic_counter , evts_counter
    global eventsTree , GENIETree
    
    data_name = MCCversion + "_" + data_type
    anatrees_listname = data_name + "_AnalysisTrees"
    print_filename(anatrees_listname , "reading anatrees listname")
    files  = read_files_from_a_list( ListName=anatrees_listname, first_file=first_file, last_file=last_file, MCCversion=MCCversion )
    
    # csv writers
    events_file_name  = events_features_file_name( anatrees_listname , first_file , last_file )
    results_file_name = tracks_full_features_file_name( anatrees_listname , first_file , last_file )
    cosmics_file_name = cosmic_features_file_name( anatrees_listname , first_file , last_file )
    g4info_file_name  = g4_features_file_name( anatrees_listname , first_file , last_file )
    events_writer, tracks_writer, cosmic_writer, g4_writer = create_csv_writers( events_file_name, results_file_name,
                                                                                cosmics_file_name, g4info_file_name,
                                                                                MCmode=MCmode,do_pandora_cosmic=do_pandora_cosmic )
    
    
    # output root file
    TracksAnaFileName  = tracks_anafile_name( anatrees_listname , first_file , last_file )
    OutFile = ROOT.TFile(TracksAnaFileName,"recreate")
    eventsTree , GENIETree  = ROOT.TTree("eventsTree","events") , ROOT.TTree("GENIETree","genie interactions")

    calc = cumputeAnaTree( eventsTree, Option, flags.verbose, MCmode, GENIETree , do_pandora_cosmic )



    i_file = 0
    for file in files: #{
        
        if debug: print_filename( file , "reading analysistree data from file (%.2f MB)"%filesize_in_MB(file) )
        if debug and i_file%(len(files)/10)==0: print'%.0f'%(100.*float(i_file)/len(files)) + '%'
        if filesize_in_MB(file) < 0.1 : continue
        
        
        in_chain = ROOT.TChain("analysistree/anatree")
        in_chain.Add( file )
        calc.SetInTree( in_chain )

        extract_anatrees_information(calc,#in_chain = in_chain , Option = Option,i_file=i_file,# eventsTree=eventsTree , GENIETree=GENIETree,
                                     events_writer=events_writer, tracks_writer=tracks_writer,
                                     cosmic_writer=cosmic_writer, g4_writer=g4_writer,
                                     MCmode=MCmode, do_pandora_cosmic=do_pandora_cosmic )
                                     
        if debug: print_filename( file , "finished extracting anatrees information from file %d out of %d"%(i_file,len(files)-1) )
        if debug: print_line()
        i_file += 1
    #}
    
    print_filename( events_file_name, "%d events (%.2f MB)"%(evts_counter,filesize_in_MB(events_file_name)) )
    print_filename( results_file_name,"%d tracks features (%.2f MB)"%(counter,filesize_in_MB(results_file_name) ) )
    print_filename( TracksAnaFileName,"%d events root file (%.2f MB)"%(evts_counter,filesize_in_MB(TracksAnaFileName)) )
    if do_pandora_cosmic: print_filename( cosmics_file_name, "%d cosmic tracks (%.2f MB)"%(cosmic_counter,filesize_in_MB(cosmics_file_name)))


    eventsTree.Write()
    if MCmode: GENIETree.Write()
    OutFile.Close()
    print "closed output file"

#}
# ----------------------------------------------------------------------------------------------------



# ----------------------------------------------------------------------------------------------------
def init_output_trees(MCmode=False): #{
    
    global run , subrun , event , Ntracks , Ng4particles, nu_interactions, tracks, g4particles, genie_interactions
    global eventsTree , GENIETree
    
    eventsTree.Branch("run"             ,run               ,"run/I")
    eventsTree.Branch("subrun"          ,subrun            ,"subrun/I")
    eventsTree.Branch("event"           ,event             ,"event/I")
    eventsTree.Branch("Ntracks"         ,Ntracks           ,"Ntracks/I")
    eventsTree.Branch("Ng4particles"    ,Ng4particles      ,"Ng4particles/I")
    eventsTree.Branch("nu_interactions" ,nu_interactions)
    eventsTree.Branch("tracks"          ,tracks)
    eventsTree.Branch("g4particles"     ,g4particles)
    
    if MCmode: #{
        eventsTree.Branch("genie_interactions"  ,genie_interactions)
        
        GENIETree.Branch("run"     ,run               ,"run/I")
        GENIETree.Branch("subrun"  ,subrun            ,"subrun/I")
        GENIETree.Branch("event"   ,event             ,"event/I")
        GENIETree.Branch("genie_interactions"  ,genie_interactions)
    # }
    
    
    if debug>1: print "cumputeAnaTree output-tree ready (", eventsTree.GetTitle(),")"
#}




# ----------------------------------------------------------------------------------------------------
def extract_anatrees_information(calc=None,#in_chain=None, Option='',i_file=0,# eventsTree=None, GENIETree=None,
                                 events_writer=None, tracks_writer=None, cosmic_writer=None, g4_writer=None,
                                 MCmode=False,do_pandora_cosmic=False,
                                 do_dEdx=False, do_SWtrigger=False ):
    
    global g4_counter , counter , cosmic_counter , evts_counter
    global run , subrun , event , Ntracks , Ng4particles, nu_interactions, tracks, g4particles, genie_interactions
    global eventsTree , GENIETree

    Nentries, Nreduced = calc.Nentries, int(flags.evnts_frac*(calc.Nentries))
    if flags.verbose: print_important( "proceesing %d events"%Nreduced )


    # main events loop
    for entry in range(Nreduced): #{
        
        calc.GetEntry( entry )
        rse = [calc.run,calc.subrun,calc.event]
        
        calc.extract_information( True )
        
        if flags.verbose and entry%flags.print_mod==0: calc.PrintData( entry )
        
        if flags.verbose>2: print 'stream event features to file'
        stream_event_features_to_file ( calc , events_writer )
        evts_counter += 1
        

        if MCmode: #{ geant4 particles
            if flags.verbose>2: print 'stream genie interactions'
            for i in range(calc.Ng4particles): #{
                g4particle = calc.GetG4Particle(i)
                stream_g4_features_to_file ( g4particle , g4_writer )
                g4_counter = g4_counter + 1
            #}
            
            if calc.mcevts_truth > 0: #{
                calc.FillGENIETree()
            #}
        #} end geant4 particles
        
        
        if do_pandora_cosmic and calc.Ncosmictracks>0: #{ if calc.Ncosmictracks>0
            for i in range(calc.Ncosmictracks): #{
                cosmic_track = calc.GetCosmicTrack(i)
                stream_tracks_features_to_file ( cosmic_track , cosmic_writer , do_dEdx=do_dEdx , do_SWtrigger=do_SWtrigger )
                cosmic_counter += 1
                if flags.verbose>4: print 'saved cosmic track (%d)'%cosmic_track.track_id; print_line()
            # } end for Ncosmictracks
        # } end do_pandora_cosmic
            
            
            
        if calc.Ntracks>0: #{  if calc.Ntracks>0
            for i in range(calc.Ntracks): #{ for Ntracks
                track = calc.GetTrack(i)
                stream_tracks_features_to_file ( track , tracks_writer , do_dEdx=do_dEdx , do_SWtrigger=do_SWtrigger )
                counter += 1
                if flags.verbose>4: print 'saving track to file %d',track.track_id ; print_line()
            #} end for Ntracks
        #}  if calc.Ntracks>0
        calc.FillOutTree()
    #} end main events loop
# ----------------------------------------------------------------------------------------------------





