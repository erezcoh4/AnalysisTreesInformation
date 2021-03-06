'''
    extract data from analysis trees
    ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    python $AnalysisTreesAna/mac/extract_data_from_anatrees.py --option=extractExampleDebugging -mccv=8 -nruns=2 -p1 -v3
 
 
 --------------------------------------------------------------------------------------------------------------------------------------------

    COMMETS:
    
    (1) this code assumes anatrees lists sit in
    
            /uboone/data/users/ecohen/Lists/analysis_trees/MCC<version>/MCC<version>_<data_type>_AnalysisTrees.list
            
    (2) <data_type> possibilities
    
            ExampleDebugging
            SingleParticleMC 
            openCOSMIC_MC
            InTimeCOSMIC_MC
            MC_BNB
            MC_BNB_InTimeCOSMIC_MC
            BNB_5e19POT
            extBNB
            MC_BNB_extBNB
            
    (3) relevant flags
    
            --option                'extract<data_type>' in most cases
            -ff                     files fraction      (default 0.01)
            -evf                    events fraction     (default 0.01)
            -mccv/--MCCversion      MCC-version         (default 7)
        
        
    
--------------------------------------------------------------------------------------------------------------------------------------------

'''
from anatrees_tools import *
do_pandora_cosmic, do_dEdx = False, False
print 'running option ',flags.option,' on MCC version: ',MCCversion





# extract all tracks information analysis trees
# ---------------------------------------------------------------------------------------
if 'extract' in flags.option:
    data_type = flags.option[7:]
    print_important( "extract tracks info from analysistrees %s, (files %d-%d)"%(data_type,first_file,last_file) )

    MCmode = True if 'isotropic' in data_type or 'MC_BNB' in data_type or 'openCOSMIC_MC' in data_type or 'InTimeCOSMIC_MC' in data_type or 'ExampleDebugging' in data_type else False
    
    if 'SingleParticleMC' in data_type:
        for DataType,last_file in zip(["p_0.02-1.5GeV_isotropic","muminus_0-2.0GeV_isotropic"   ,"piminus_0-2.0GeV_isotropic"]
                                      ,[377                     ,395                            ,377]):
            extract_anatrees_tracks_information_from_files_list(data_type=data_type,
                                                                last_file=last_file ,
                                                                MCmode=MCmode,
                                                                MCCversion=MCCversion,
                                                                MCCV=flags.MCCversion
                                                                )

    else:
        extract_anatrees_tracks_information_from_files_list(data_type=data_type,
                                                            first_file=first_file , last_file=last_file ,
                                                            MCmode=MCmode ,
                                                            MCCversion=MCCversion ,
                                                            do_pandora_cosmic=do_pandora_cosmic ,
                                                            do_dEdx=do_dEdx,
                                                            MCCV=flags.MCCversion)

# ---------------------------------------------------------------------------------------



print 'extract_data_from_anatrees.py done.'





