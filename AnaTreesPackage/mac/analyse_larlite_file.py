'''
    
    usage:
    ------
    python $AnalysisTreesAna/mac/analyse_larlite_file.py -wuboone -v2 -evf=10
    
    evf represents the maximal number of images it will create (=n_max_entries)
'''
import sys
sys.path.insert(0, '/uboone/app/users/ecohen/larlite/UserDev/GBDTprotons/GBDTprotonPackage/mac')
sys.path.insert(0, '/Users/erezcohen/larlite/UserDev/GBDTprotons/GBDTprotonPackage/mac')
from uboone_tools import *
from larlite import larlite as fmwk
from uboone_tools import *
from gbdt_tools import *
from larlite import *


features_names = [
                  'run','subrun','event',
                  'ivtx','itrk_NuSelMuon','itrk_GBDTproton',
                  'mu_P','mu_theta','mu_phi',
                  'mu_start_x','mu_start_y','mu_start_z',
                  'mu_end_x','mu_end_y','mu_end_z',
                  'p_P','p_theta','p_phi',
                  'p_start_x','p_start_y','p_start_z',
                  'p_end_x','p_end_y','p_end_z',
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

# ----------------------- file names ----------------------------- #
my_fwk = fmwk.ana_processor()

# input (1): larlite file
latlitefilename = "larlite_BNB5e19_mu-p_maxp_pscore0.9_Dec2016"
my_fwk.add_input_file( data_files_path + "/BNB_DATA/larlite_files/" + latlitefilename + ".root")
print_filename( data_files_path + "/BNB_DATA/larlite_files/" + latlitefilename  + ".root" , "added input (1): larlite input file")

# input (2): roi map for muon-proton vertices
roi_map_name    = rois_path + '/' + "ROIs_Sel2muons_BNB_5e19POT_multi_BNB_TrainedOn_MCBNB_MCCOSMIC_pscore_0.90_intersection_mindistance_10cm.csv"
print_filename( roi_map_name , "added input (2): roi map" )

# output (1): ana-file with a simple root tree
my_fwk.set_io_mode( fmwk.storage_manager.kREAD )
my_fwk.set_ana_output_file( ana_files_path + "/BNB_ANA/Ana_" + latlitefilename  + ".root" )
print_filename( ana_files_path + "/BNB_ANA/Ana_" + latlitefilename  + ".root", 'added ouput (1): root file ')
# output (2): csv file
resutls_file_name = ana_files_path + "/BNB_ANA/Features_" + latlitefilename  + ".csv"
#writer = csv.writer(open(resutls_file_name, 'wb'))
#writer.writerow( features_names )
print_filename( resutls_file_name , 'added ouput (2): features csv file ')
# ---------------------------------------------------------------- #
n_max_entries   = -1 if flags.evnts_frac <= 1 else int(flags.evnts_frac)
# ---------------------------------------------------------------- #

# call the analysis unit
ana_process = fmwk.AnalyseEvents()
# set its important arguments (verbosity, worker, etc.)
ana_process.SetArgs( flags.worker , flags.verbose , n_max_entries , True , resutls_file_name)
# load ROIs map
ana_process.LoadROIsMap( roi_map_name  )
# add my analysis process
my_fwk.add_process( ana_process )
# run the analysis
my_fwk.run()

print "done!"

sys.exit(0)
