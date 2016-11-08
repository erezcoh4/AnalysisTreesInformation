from anatrees_tools import *
from gbdt_tools import *
'''
    usage:
    ------
    python $AnalysisTreesAna/mac/muon_proton_id.py --DataType=BNB --option=intersect
    options: 
        intersect   {"intersect GBDT protons with Sel2 muons"}
        scheme      {"scheme analysis trees events"} --DataType=BNB -wuboone -v2 -ff=0.01 -evf=0.1
'''



TracksListName          = "BNB_5e19POT"
GBDTmodelName           = "BNB_TrainedOn_only_MC_BNB"
p_score                 = 0.99




# (1) extract all tracks information from analysis trees to classify proton tracks
# -------------------------------------------------------------------
if flags.option=="extract tracks information from AnalysisTrees" or 'extract' in flags.option:
    print "extract"
    


# (2) Classify proton tracks
# -------------------------------------------------------------------




# (3) Select events with a muon from neutrino interaction using Sel2
# -------------------------------------------------------------------



# (4) intersect GBDT protons list with Sel2 muons
# -------------------------------------------------------------------
if flags.option=="intersect GBDT protons with Sel2 muons" or 'intersect' in flags.option:
    
    intersectlists_GBDTprotons_Sel2muons( GBDTmodelName, TracksListName , p_score )



# (5) scheme all AnalysisTrees to a smaller file containing only the events we are interested in
# -------------------------------------------------------------------
if flags.option=="scheme analysis trees events" or 'scheme' in flags.option:
    
    scheme_list_of_files_rse( GBDTmodelName, TracksListName , p_score )


# (6) loop over the schemed AnalysisTrees file, and for each event check if the selected muon and the classified proton track are close enough
# -------------------------------------------------------------------
if flags.option=="loop over muon-proton candidates" or 'loop' in flags.option:
    print "loop"


# (7) filter out from art-file intersected events
# -------------------------------------------------------------------
#   (A) scp <intersecte_list.csv> $uboone
#
#   (B) lar -c $my_fcl_files/my_argofilter.fcl -S <files-list>
#
#   BNB art-files list:
#   /uboone/app/users/ecohen/AnalysisTreesAna/lists/ArtFilesLists/prod_bnb_reco_neutrino2016_beamfilter_goodruns_v5.lis



# (8) create ROI images
# -------------------------------------------------------------------



# (9) perform analysis
# -------------------------------------------------------------------
