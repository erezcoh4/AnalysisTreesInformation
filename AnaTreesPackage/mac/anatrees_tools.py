import sys
sys.path.insert(0, '/uboone/app/users/ecohen/larlite/UserDev/GBDTprotons/GBDTprotonPackage/mac')
sys.path.insert(0, '/Users/erezcohen/larlite/UserDev/GBDTprotons/GBDTprotonPackage/mac')
from uboone_tools import *
from gbdt_tools import *
from ROOT import ImportantTools




# paths
# -------------------------

neutrinoSel2_path       = lists_path + "/NeutrinoSelection2"
mu_p_intersection_path  = lists_path + "/muon_proton_intersection"
schemed_anatrees_path   = anatrees_data_path  + "/SchemedFiles"







# list names and file names
# -------------------------
def Sel2muons_list_name():
    
    if flags.DataType == "BNB":
        neutrinoSel2_list_name = "BeamOnData_pandoraNu_pandoraNu.csv"
    elif flags.DataType == "EXTBNB":
        neutrinoSel2_list_name = "BeamOffData_pandoraNu_pandoraNu.csv"
    return neutrinoSel2_path + "/" + neutrinoSel2_list_name

# -------------------------
def Sel2muons_intersection_list_name( GBDTmodelName, TracksListName, p_score ):
    classification_name = TracksListName + "_" + GBDTmodelName
    return "Sel2muons_"+classification_name+"_pscore_%.2f_intersection.csv"%p_score


# -------------------------
def schemed_anatrees_file_name( DataType ):
    return schemed_anatrees_path + "/GOOD" + DataType + "_filesana_" + EventsListName + ".root"




# methods
# -------------------------
def intersectlists_GBDTprotons_Sel2muons( GBDTmodelName, TracksListName , p_score ):
    import csv, pandas as pd
    
    GBDTpListName   = GBDTprotonsListName( GBDTmodelName, TracksListName, p_score ,'ROIs')
    print_filename( GBDTpListName , "opening GBDT protons list name:")
    Sel2muListName  = Sel2muons_list_name()
    print_filename( Sel2muListName , "opening Sel2 muons list name:")

    df_GBDTp = pd.read_csv( GBDTpListName , delimiter=' ' )
    df_Sel2mu = pd.read_csv( Sel2muListName , delimiter=' ' )
    df_intersection = pd.merge(df_GBDTp, df_Sel2mu, how='inner', on=['run', 'subrun','event'])
    
    IntersectionListName = mu_p_intersection_path + "/" + Sel2muons_intersection_list_name( GBDTmodelName ,TracksListName ,p_score )
    df_intersection = df_intersection.rename(columns={ 'trkindex[ivtx][itrk]':'itrk-NuSelMuon',
                                             'ivtx':'ivtx-NuSel', 'trackid':'itrk-GBDTproton'} )
    df_intersection[['run','subrun','event',
                    'ivtx-NuSel','itrk-NuSelMuon',
                    'itrk-GBDTproton']].to_csv(IntersectionListName, sep=' ', header=False, index=False )
    print_filename( IntersectionListName , "wrote Sel2/GBDTprotons intersection list name:")
    print_important( "scp %s $uboone:~/"%IntersectionListName )
    print_line()






# -------------------------
def get_analysistrees_chain(files):
    in_chain = ROOT.TChain("analysistree/anatree")
    for i in range(int(flags.files_frac*len(files))):
        if flags.verbose>1: print "file %d size is %.2f MB"%(i,float(os.path.getsize(files[i])/1048576))
        if float(os.path.getsize(files[i])/1048576) > 0.1 :
            in_chain.Add(files[i])
    if flags.verbose: print "input chain entries from ",int(flags.files_frac*len(files))," files: ",in_chain.GetEntries()
    return in_chain





# -------------------------
def scheme_list_of_files_rse( GBDTmodelName, TracksListName , p_score ):
    '''
    This functionallity schemes (big) analysis trees
    and returns a tree containing only entries with a Run/Subrun/Event
    of a given list (RSE map)
    '''
    AnalysisTreesListName   = anatrees_lists_path + "/GOOD" + flags.DataType + "/filesana.list" # input analysis trees
    IntersectionListName    = mu_p_intersection_path + "/" + Sel2muons_intersection_list_name( GBDTmodelName ,TracksListName , p_score ) # input mu-p list
    SchemedResultFileName   = schemed_anatrees_file_name( flags.DataType )  # output schemed analysis trees file
    it = ImportantTools()

    with open( AnalysisTreesListName ) as f:
        files = f.read().splitlines()
    if flags.verbose>4: print "shceming files: ",files

    in_chain = get_analysistrees_chain(files)
    OutFile = ROOT.TFile( SchemedResultFileName , "recreate" )
    OutTree = it.SchemeTreeRSEList( in_chain , IntersectionListName )

    OutTree.Write()
    OutFile.Close()
    if flags.verbose:
        print_filename(SchemedResultFileName , "wrote schemed file (%d events, %.2f MB):"%(OutTree.GetEntries(),float(os.path.getsize(SchemedResultFileName)/1048576.0)))







