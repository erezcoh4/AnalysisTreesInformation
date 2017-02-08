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
        images      {"create ROI images"}
        larlite     {"analyze larlite file"}
'''


TracksListName          = "BNB_5e19POT"
GBDTmodelName           = "multi_BNB_TrainedOn_MCBNB_MCCOSMIC"  # options: 'BNB_TrainedOn_only_MC_BNB'
maxscore                = 'protons'
p_score                 = 0.9
MCCversion              = "MCC%d"%flags.MCCversion


splitjobs_files = 1000 if flags.NumberOfRuns==0 else flags.NumberOfRuns # splitting the jobs: 0-10, 10-20, 20-30,....
first_anatree_file = flags.run
last_anatree_file = first_anatree_file + splitjobs_files


print 'running option ',flags.option,' on MCC version: ',MCCversion


# (1) extract all tracks information from BNB-MC and COSMIC-MC analysis trees to train GBDTs
# -------------------------------------------------------------------
if flags.option=="extract tracks information from MC AnalysisTrees" or 'extractMC' in flags.option:
    print_important( "extract tracks information from MC AnalysisTrees" )
    extract_anatrees_tracks_information_from_files_list(  "MC_BNB" , "extract all tracks information" ,
                                                        first_anatree_file , last_anatree_file , MCmode = True ,
                                                        MCCversion=MCCversion , do_pandora_cosmic=True )


# ---------------
if flags.option=="extract MC-CORSIKA tracks information from AnalysisTrees" or 'extract_CORSIKA' in flags.option:
    print_important( "extract MC-CORSIKA tracks information from AnalysisTrees" )
    extract_anatrees_tracks_information_from_files_list( MCCversion+"_openCOSMIC_MC" , "extract all tracks information" ,
                                                        first_anatree_file , last_anatree_file  ,
                                                        MCmode=True , MCCversion=MCCversion , do_pandora_cosmic=True )


# single particle MC
# ---------------
if flags.option=="extract MC-protons tracks information from AnalysisTrees" or 'extract_protons' in flags.option:
    print_important( "extract MC-porotns tracks information from AnalysisTrees" )
    extract_anatrees_tracks_information_from_files_list( "p_0.02-1.5GeV_isotropic" , "extract all tracks information" , 0 , 377 , MCmode = True ) # 377 files

# ---------------
if flags.option=="extract MC-muons tracks information from AnalysisTrees" or 'extract_muons' in flags.option:
    print_important( "extract MC-muons tracks information from AnalysisTrees" )
    extract_anatrees_tracks_information_from_files_list( "muminus_0-2.0GeV_isotropic" , "extract all tracks information" , 0 , 395 , MCmode = True ) # 397 files


# ---------------
if flags.option=="extract MC-pions tracks information from AnalysisTrees" or 'extract_pions' in flags.option:
    print_important( "extract MC-pions tracks information from AnalysisTrees" )
    extract_anatrees_tracks_information_from_files_list( "piminus_0-2.0GeV_isotropic" , "extract all tracks information" , 0 , 377 , MCmode = True ) # 397 files



# (2) train, build, test the GBDT models
# -------------------------------------------------------------------
# use GBDTprotons/GBDTprotonsPackage




# (3) extract all tracks information from analysis trees to classify proton tracks
# -------------------------------------------------------------------
if flags.option=="extract tracks information from AnalysisTrees" or 'extractDATA' in flags.option:
    print_important("extract AnalysisTrees information (files %d-%d)"%(first_anatree_file,last_anatree_file) )
    extract_anatrees_tracks_information_from_files_list( "BNB_5e19POT" , "extract all tracks information" , first_anatree_file , last_anatree_file )

# ---------------
if flags.option=="extract tracks information from extBNB AnalysisTrees" or 'extractEXTDATA' in flags.option:
    print_important("extract AnalysisTrees information from %s (files %d-%d)"%(MCCversion,first_anatree_file,last_anatree_file) )
    extract_anatrees_tracks_information_from_files_list( MCCversion+"_extBNB" , "extract all tracks information" ,
                                                        first_anatree_file , last_anatree_file ,
                                                        MCCversion=MCCversion , do_pandora_cosmic=True )
    # can also add the option: 'add hard geometrical cuts' (Dec-2016)




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
#    scheme_list_of_files_rse( GBDTmodelName, TracksListName , p_score )

    # scheme CCQE candidates from anatrees for Varuna (Jan. 2017)
#    scheme_anatrees_files( anatrees_list_name="BNB_5e19POT_AnalysisTrees",
#                          rsemap2selectfrom = mu_p_intersection_path + "/Sel2muons_BNB_5e19POT_multi_BNB_TrainedOn_MCBNB_MCCOSMIC_pscore_0.90_intersection_mindistance_10cm.csv",
#                          output_anatree_filename="Preliminary_CCQE_candidates_BNB5e19POT_Jan2017.root" )
    scheme_anatrees_file( input_anatree_file="/uboone/data/users/ecohen/AnalysisTreeData/SchemedFiles/data_AnalysisTrees_Sel2muons_BNB_5e19POT_multi_BNB_TrainedOn_MCBNB_MCCOSMIC_pscore_0.90_intersection.root",
                          rsemap2selectfrom = mu_p_intersection_path + "/Sel2muons_BNB_5e19POT_multi_BNB_TrainedOn_MCBNB_MCCOSMIC_pscore_0.90_intersection_mindistance_10cm.csv",
                          output_anatree_filename="Preliminary_CCQE_candidates_BNB5e19POT_Jan2017.root" )



# (8) loop over the schemed AnalysisTrees file, and for each event check if the selected muon and the classified proton track are close enough
# -------------------------------------------------------------------
if flags.option=="find common muon-proton vertices" or 'find_mu_p' in flags.option:
    print_important( "find common muon-proton vertices" )
    SchemedResultFileName   = schemed_anatrees_file_name( flags.DataType + "_AnalysisTrees" , Sel2muons_intersection_list_name( GBDTmodelName ,TracksListName , p_score ) )
    print_filename( SchemedResultFileName , "Schemed analysistrees File Name" )
    IntersectionListName    = mu_p_intersection_path + "/" + Sel2muons_intersection_list_csv_name( GBDTmodelName , TracksListName , p_score )
    print_filename( IntersectionListName , "mu-p intersection lise" )
    output_mupRSEFileName   = mu_p_intersection_path + "/" + good_mu_p_rse_list_name( GBDTmodelName, TracksListName, p_score  )
    print_filename( output_mupRSEFileName , "output mu-p R/S/E File Name" )
    output_mupROIFileName   = rois_path + "/" + good_mu_p_roi_list_name( GBDTmodelName, TracksListName, p_score  )
    print_filename( output_mupROIFileName , "output mu-p ROIs File Name" )

    extract_anatrees_tracks_information_from_a_file( flags.DataType ,
                                                    SchemedResultFileName ,
                                                    Option = "find common muon-proton vertices" ,
                                                    MCmode = False,
                                                    TreeName = "anatree",
                                                    AddEventsList = True,
                                                    EventsListName = IntersectionListName,
                                                    output_mupRSEFileName = output_mupRSEFileName,
                                                    output_mupROIFileName = output_mupROIFileName)



# (9) filter out from art-file intersected events
# -------------------------------------------------------------------
#   lar -c $my_fcl_files/my_argofilter.fcl -S <files-list>
#   BNB art-files list:
#   /uboone/app/users/ecohen/AnalysisTreesAna/lists/ArtFilesLists/prod_bnb_reco_neutrino2016_beamfilter_goodruns_v5.lis



# (10) create ROI images
# -------------------------------------------------------------------
if flags.option=="create ROI images" or 'roi' in flags.option or 'images' in flags.option or 'ROI' in flags.option:
    print_important('python $AnalysisTreesAna/mac/create_roi_images.py -wuboone -v2 -evf=3')


# (11) further analysis
# -------------------------------------------------------------------
if flags.option=="mix events" or 'mix' in flags.option or 'MIX' in flags.option:
    print_important('python $AnalysisTreesAna/mac/mix_events.py -w%s -v2 -evf=10'%flags.worker)

if flags.option=="analyze larlite file" or 'larlite' in flags.option or 'Larlite' in flags.option:
    print_important('python $AnalysisTreesAna/mac/analyze_larlite_file.py -w%s -v2 -evf=10'%flags.worker)








print 'muon_proton_id.py done.'





