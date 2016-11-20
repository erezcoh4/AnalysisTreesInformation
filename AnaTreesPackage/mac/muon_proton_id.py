from anatrees_tools import *
'''
    usage:
    ------
    python $AnalysisTreesAna/mac/muon_proton_id.py  --option=intersect
    
    options: 
        extractMC   {"extract tracks information from MC AnalysisTrees"}
        extractDATA {"extract tracks information from AnalysisTrees"} --DataType=BNB -ff=0.01 -evf=0.01
        intersect   {"intersect GBDT protons with Sel2 muons"}
        scheme      {"scheme analysis trees events"} --DataType=BNB -ff=0.01
        find_mu_p   {"find common muon-proton vertices"} --DataType=BNB
'''


TracksListName          = "BNB_5e19POT"
GBDTmodelName           = "multi_BNB_TrainedOn_MCBNB_MCCOSMIC"  # options: 'BNB_TrainedOn_only_MC_BNB'
maxscore                = 'protons'
p_score                 = 0.9





# (1) extract all tracks information from BNB-MC and COSMIC-MC analysis trees to train GBDTs
# -------------------------------------------------------------------
if flags.option=="extract tracks information from MC AnalysisTrees" or 'extractMC' in flags.option:
    splitjobs_files = 1000 # splitting ti jobs: 0-10, 10-20, 20-30,....
    first_anatree_file = flags.run
    last_anatree_file = first_anatree_file + splitjobs_files
    print_important( "extract tracks information from MC AnalysisTrees" )
    # extract_anatrees_tracks_information_from_files_list( "openCOSMIC_MC" , "extract all tracks information" , MCmode = True )
    extract_anatrees_tracks_information_from_files_list( "MC_BNB" , "extract all tracks information" , first_anatree_file , last_anatree_file , MCmode = True )



# (2) train, build, test the GBDT models
# -------------------------------------------------------------------
# use GBDTprotons/GBDTprotonsPackage


# (3) extract all tracks information from analysis trees to classify proton tracks
# -------------------------------------------------------------------
if flags.option=="extract tracks information from AnalysisTrees" or 'extractDATA' in flags.option:
    splitjobs_files = 1000 # splitting ti jobs: 0-1000, 1000-2000, 2000-3000, ....
    first_anatree_file = flags.run
    last_anatree_file = first_anatree_file + splitjobs_files
    print_important( "extract tracks information from AnalysisTrees (files %d-%d)"%(first_anatree_file,last_anatree_file) )
    extract_anatrees_tracks_information_from_files_list( "BNB_5e19POT" , "extract all tracks information" , first_anatree_file , last_anatree_file )



# (4) Classify proton tracks
# -------------------------------------------------------------------
# use GBDTprotons/GBDTprotonsPackage




# (5) Select events with a muon from neutrino interaction using Sel2
# -------------------------------------------------------------------
# use neutrinoEventSelection
# [https://github.com/erezcoh4/neutrinoEventsSelection]



# (6) intersect GBDT protons list with Sel2 muons
# -------------------------------------------------------------------
if flags.option=="intersect GBDT protons with Sel2 muons" or 'intersect' in flags.option:
    
    intersectlists_GBDTprotons_Sel2muons( GBDTmodelName, TracksListName , p_score )




# (7) scheme all AnalysisTrees to a smaller file containing only the events we are interested in
# -------------------------------------------------------------------
if flags.option=="scheme analysis trees events" or 'scheme' in flags.option:
    if flags.verbose: print_important( "scheme analysis trees events" )
    scheme_list_of_files_rse( GBDTmodelName, TracksListName , p_score )



# (8) loop over the schemed AnalysisTrees file, and for each event check if the selected muon and the classified proton track are close enough
# -------------------------------------------------------------------
if flags.option=="find common muon-proton vertices" or 'find_mu_p' in flags.option:
    print_important( "find common muon-proton vertices" )
    SchemedResultFileName   = schemed_anatrees_file_name( "GOOD"+flags.DataType+"_filesana.list" , Sel2muons_intersection_list_name( GBDTmodelName ,TracksListName , p_score ) )
    IntersectionListName    = mu_p_intersection_path + "/" + Sel2muons_intersection_list_csv_name( GBDTmodelName , TracksListName , p_score )
    mupRSEFileName          = mu_p_intersection_path + "/" + good_mu_p_rse_list_name( GBDTmodelName, TracksListName, p_score  )
    extract_anatrees_tracks_information_from_a_file( flags.DataType ,
                                                    SchemedResultFileName ,
                                                    Option = "find common muon-proton vertices" ,
                                                    MCmode = False,
                                                    TreeName = "anatree",
                                                    AddEventsList = True,
                                                    EventsListName = IntersectionListName,
                                                    mupRSEFileName = mupRSEFileName )



# (9) filter out from art-file intersected events
# -------------------------------------------------------------------
#   lar -c $my_fcl_files/my_argofilter.fcl -S <files-list>
#   BNB art-files list:
#   /uboone/app/users/ecohen/AnalysisTreesAna/lists/ArtFilesLists/prod_bnb_reco_neutrino2016_beamfilter_goodruns_v5.lis



# (10) create ROI images
# -------------------------------------------------------------------



# (11) analyze
# -------------------------------------------------------------------















