import ROOT , os , sys, larlite , datetime
import math , csv

sys.path.insert(0, '/uboone/app/users/ecohen/larlite/UserDev/mySoftware/MySoftwarePackage/mac')
sys.path.insert(0, '/Users/erezcohen/larlite/UserDev/mySoftware/MySoftwarePackage/mac')
sys.path.insert(0, '../../AnaTreesGBDTprotons/GBDTprotonsPackage/mac')

from ROOT import ImportantTools
from my_tools import *

import input_flags
import pandas as pd



# flags
'''
    flags:
    ------
    --DataType=BNB  { BNB / MCBNB / EXTBNB / MCEXBNB }
    --worker=uboone { worker }
    -v2             { verbosity }
    -ff=0.01        { files fraction to process }
    -evf=0.1        { events fraction to process }
'''
flags = input_flags.get_args()
option = flags.option



# paths
# -------------------------
if flags.worker=="erez":#{
    
    Path            = "/Users/erezcohen/Desktop/uBoone/AnalysisTreesAna"
    PlotPath        = "/Users/erezcohen/Desktop/"

#}

elif flags.worker=="uboone":#{
    
    Path            = "/uboone/app/users/ecohen/AnalysisTreesAna"
    PlotPath        = "/uboone/app/users/ecohen/"

#}

GBDTmodel_path      = Path + "/GBDTmodels"
passedGBDT_path     = Path + "/PassedGBDTFiles"





# models, lists
'''
    
    application on beam-off data, training on cosmic MC
    ----------------------------------------------------
    ListName        = "extBNB_AnalysisTrees"
    ModelName       = "cosmic_trained_only_on_mc"
    TrainingSample  = "200000_tracks_openCOSMIC_MC"
    TestSample      = "55650_tracks_openCOSMIC_MC_AnalysisTrees"
    
    application on beam-on data, training only on MC-BNB
    ----------------------------------------------------
    ListName        = "BNB_5e19POT_AnalysisTrees"
    ModelName       = "BNB_TrainedOn_only_MC_BNB"
    TrainingSample  = "300000_tracks_MC_BNB"
    TestSample      = "87789_tracks_MC_BNB"
    
'''






