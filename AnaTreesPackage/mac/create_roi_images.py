import sys
sys.path.insert(0, '/uboone/app/users/ecohen/larlite/UserDev/GBDTprotons/GBDTprotonPackage/mac')
sys.path.insert(0, '/Users/erezcohen/larlite/UserDev/GBDTprotons/GBDTprotonPackage/mac')
from uboone_tools import *
from larlite import larlite as fmwk
'''
    
    usage:
    ------
    python $AnalysisTreesAna/mac/create_roi_images.py -wuboone -v2 -evf=3
    
    evf represents the maximal number of images it will create (=n_max_entries)
    
'''

# input (1): larlite file
infilename      = "larlite_BNB_5e19POT_TrainedOn_only_MC_BNB_pscore0.99_3942evts_5nov2016.root"

# input (2): roi map for muon-proton vertices
roi_map_path    = '/uboone/app/users/ecohen/AnalysisTreesAna/FeaturesFiles/'
roi_map_name    = "features_Sel2muons_BNB_5e19POT_multi_BNB_TrainedOn_MCBNB_MCCOSMIC_pscore_0.90_intersection_mindistance_10cm.csv"

# output: where the images will be generated
images_name     = "BNB_5e19POT_TrainedOn_MCBNB_MCCOSMIC_pscore_0.90_mu_p_intersection_maxdistance_10cm"
n_max_entries   = -1 if flags.evnts_frac < 1 else int(flags.evnts_frac) # how many images? -1 means we analyze all TTree entries

# create a directory for the new images
init.create_dir( data_files_path + "/images/" , images_name )
images_path = data_files_path + "/images/" + images_name



sys.stderr.write("\n runnnig %s \n\n" % infilename)

my_proc = fmwk.ana_processor()

my_proc.add_input_file( data_files_path + "/BNB_DATA/larlite_files/" + infilename)

my_proc.set_io_mode(fmwk.storage_manager.kREAD)

my_proc.set_ana_output_file( data_files_path + "/AnaFiles/Ana_" + infilename)

ana_pandoraNu_tracks = fmwk.AnaPandoraNuTracks()

ana_pandoraNu_tracks.SetWorker( flags.worker , flags.verbose , True , images_path , roi_map_path , roi_map_name , n_max_entries )

my_proc.add_process( ana_pandoraNu_tracks )


print  "Finished configuring ana_processor. Start event loop!"
my_proc.run()
print "done!"

sys.exit(0)
