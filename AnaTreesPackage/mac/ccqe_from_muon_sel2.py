'''
--------------------------------------------------------------------------------------------------------------------------------------------
data --> muon inclusive sel. II --> Ncc2 events with CC candidate muon
                                                        |
                                                        v
                                            filter analysis tree files
                                                        |
                                                        v
                                            analysis trees with Ncc2 events --> extract information
                                                                                        |
                                                                                        v
                                                                            all tracks from the Ncc2 events,
                                                                            including dis2muon_track{closest-distance to sel2_muon track}
                                                                            /
                                                                           /
                                                                          /
                                 ----------------------------------------
                                /
                               /
                              |
                              |
                              |
                              |
                              |
                              |
                              v
        all tracks from the Ncc2 events ----> apply GBDTs model (predict multiscore)
                                                        |
                                                        v
                                            all tracks from the Ncc2 events with GBDT scores
                                                        |
                                                        v
                                            select proton tracks with
                                                * proton score > min_score
                                                * max_distance > closest-distance to sel2-muon 
                                                (max_distance determined form res. study)
                                                (but not too tight - we want to allow neutrons to show up eventually as well ?)
                                                        |
                                                        v
                                            selected proton tracks from CC interaction with a proton score m_pscore
                                                        |
                                                        v
                                            group tracks by R/S/E - reactions topology
                                                        |
                                                        v
                                            events with a sel. II muon and Np protons:
                                            - vector<pandoraNuTrack> protons
                                            - vector<Float_t> p_score
                                            - vector<Float_t> dis2muon_track

--------------------------------------------------------------------------------------------------------------------------------------------
            

    usage:
    ------
    python $AnalysisTreesAna/mac/ccqe_from_muon_sel2.py  
            <--option> <--ff=0.01> <--evf=0.01>
    
    options (chronologically):
        (1) extract     {SingleParticleMC / openCOSMIC_MC / MC_BNB / BNB_5e19POT / extBNB}
        (2) selIImuons
        (3) scheme      { openCOSMIC_MC / MC_BNB / BNB_5e19POT / extBNB }
        (4) extractDATA
        (5) applyGBDTs
        (6) groupEvents
        (7) images


--------------------------------------------------------------------------------------------------------------------------------------------

'''
from anatrees_tools import *
print 'running option ',flags.option,' on MCC version: ',MCCversion




# extract all tracks information from BNB-MC and COSMIC-MC analysis trees to train GBDTs
# ---------------------------------------------------------------------------------------
if 'extract' in flags.option:
    data_type = flags.option[6:]
    print_important( "extract tracks info from analysistrees %s, (files %d-%d)"%(data_type,first_anatree_file,last_anatree_file) )
    
    if 'SingleParticleMC' in data_type:
        for DataType,lf in zip(["p_0.02-1.5GeV_isotropic" , "muminus_0-2.0GeV_isotropic" , "piminus_0-2.0GeV_isotropic"],[377,395,377]):
            extract_anatrees_tracks_information_from_files_list( DataType = MCCversion+"_"+data_type,
                                                                first_anatree_file=0 , last_anatree_file=lf ,
                                                                MCmode=True )

    if 'openCOSMIC_MC' in data_type or 'MC_BNB' in data_type or 'BNB_5e19POT' in data_type or 'extBNB' in data_type:
        extract_anatrees_tracks_information_from_files_list( DataType = MCCversion+"_"+data_type ,
                                                            first_anatree_file=first_anatree_file , last_anatree_file=last_anatree_file ,
                                                            MCmode = True ,
                                                            MCCversion=MCCversion , do_pandora_cosmic=False )

# ---------------------------------------------------------------------------------------


# extract all tracks information from BNB-MC and COSMIC-MC analysis trees to train GBDTs
# ---------------------------------------------------------------------------------------
if 'selIImuons' in flags.option:
    # [https://github.com/erezcoh4/neutrinoEventsSelection]
    print_important( "selection II muons" )




if 'scheme' in flags.option:
    data_type = flags.option[6:]
    print_important( "scheme analysis trees to selection II events from %s" % data_type )

    anatrees_listname = MCCversion + "_" + data_type
    rse_map_name = sel2_path + MCCversion + "/" + MCCversion + "_" + data_type + ".list"


    scheme_anatrees_files(anatrees_listname = MCCversion + "_" + data_type,
                          rse_mapname = MCCversion + "_" + data_type + ".list",
                          outfname = "CC1p_candidates_" + MCCversion + "_" + data_type + ".root")






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





