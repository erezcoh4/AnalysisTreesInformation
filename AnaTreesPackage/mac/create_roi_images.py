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


# ----------------------- file names ----------------------------- #
# input (1): larlite file
latlitefilename = "larlite_BNB5e19_mu-p_vertex_maxscoredprotons_score_0.9_917evnts_23Dec2016.root"
print_filename( latlitefilename ,"input (1): larlite input file" )

# input (2): roi map for muon-proton vertices
roi_map_name    = rois_path + '/' + "roi_Sel2muons_BNB_5e19POT_multi_BNB_TrainedOn_MCBNB_MCCOSMIC_pscore_0.90_intersection_mindistance_10cm.csv"
print_filename( roi_map_name , "input (2): roi map" )
# ---------------------------------------------------------------- #

# arguments
image_format = 'png'


# output: where the images will be generated
images_name     = "BNB_5e19POT_TrainedOn_MCBNB_MCCOSMIC_pscore_0.90_mu_p_intersection_maxdistance_10cm"
n_max_entries   = -1 if flags.evnts_frac <= 1 else int(flags.evnts_frac)

# create a directory for the new images
init.create_dir( data_files_path + "/images/" , images_name )
images_path = data_files_path + "/images/" + images_name
print_important( "output: images at %s"%images_path )
# ---------------------------------------------------------------- #


# create the framework
my_fwk = fmwk.ana_processor()
print 'adding input file: ' + data_files_path + "/BNB_DATA/larlite_files/" + latlitefilename
my_fwk.add_input_file( data_files_path + "/BNB_DATA/larlite_files/" + latlitefilename )
print 'added input file '
my_fwk.set_io_mode( fmwk.storage_manager.kREAD )
print 'adding output file: ' + data_files_path + data_files_path + "/AnaFiles/Ana_%s" % latlitefilename
my_fwk.set_ana_output_file( data_files_path + "/AnaFiles/Ana_%s" % latlitefilename )
print 'added ouput file '




#my_proc = fmwk.ana_processor()
#
#my_proc.add_input_file( data_files_path + "/BNB_DATA/larlite_files/" + infilename)
#
#my_proc.set_io_mode(fmwk.storage_manager.kREAD)
#
#my_proc.set_ana_output_file( data_files_path + "/AnaFiles/Ana_" + infilename)
#
#ana_pandoraNu_tracks = fmwk.AnaPandoraNuTracks()
#
#ana_pandoraNu_tracks.SetWorker( flags.worker , flags.verbose , True , images_path , roi_map_path , roi_map_name , n_max_entries )
#
#my_proc.add_process( ana_pandoraNu_tracks )
#
#
#print  "Finished configuring ana_processor. Start event loop!"
#my_proc.run()

# call the analysis unit
ana_process = fmwk.AnalyseEvents()
# set its important arguments (verbosity, worker, etc.)
ana_process.SetArgs( flags.worker , flags.verbose , images_path , n_max_entries )
# load ROIs map
n_roi_per_event = 3
ana_process.LoadROIsMap( roi_map_name , n_roi_per_event )
ana_process.SetImageFormat( image_format )

# add my analysis process
my_fwk.add_process( ana_process )
# run the analysis
my_fwk.run()

print "done!"

sys.exit(0)
