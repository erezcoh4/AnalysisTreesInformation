import sys
sys.path.insert(0, '/uboone/app/users/ecohen/larlite/UserDev/GBDTprotons/GBDTprotonPackage/mac')
sys.path.insert(0, '/Users/erezcohen/larlite/UserDev/GBDTprotons/GBDTprotonPackage/mac')
from uboone_tools import *
from gbdt_tools import *
from ROOT import ImportantTools , cumputeAnaTree


# globals
# -------------------------
min_trk_vtx_distance = 10 # [cm], this distance needs to be studied wisely


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
    return "Sel2muons_"+classification_name+"_pscore_%.2f_intersection"%p_score

# -------------------------
def Sel2muons_intersection_list_csv_name( GBDTmodelName, TracksListName, p_score ):
    return Sel2muons_intersection_list_name( GBDTmodelName, TracksListName, p_score ) + ".csv"

# -------------------------
def good_mu_p_rse_list_name( GBDTmodelName, TracksListName, p_score ):
    return Sel2muons_intersection_list_name( GBDTmodelName, TracksListName, p_score ) + "_mindistance_%dcm.csv"%min_trk_vtx_distance


# -------------------------
def schemed_anatrees_file_name( anatrees_list_name , scheming_name ):
    return schemed_anatrees_path + "/" + anatrees_list_name + "_" + scheming_name + ".root"


# -------------------------
def tracks_features_file_name( ListName ):
    return featuresfiles_path + "/" + "features_" + ListName + ".csv"

# -------------------------
def tracks_anafile_name( ListName ):
    return anafiles_path + "/" + "Tracks_" + ListName + ".root"





# methods
# -------------------------
def read_files_from_a_list( ListName ):
    if flags.verbose: print "\nreading list of files..."
    with open( lists_path + "/analysis_trees/" + ListName + ".list") as f:
        files = f.read().splitlines()
    if flags.verbose>4: print files
    return files


# -------------------------
def get_analysistrees_chain(files):
    chain = ROOT.TChain("analysistree/anatree")
    for i in range(int(flags.files_frac*len(files))):
        if flags.verbose>1: print "file %d size is %.2f MB"%(i,float(os.path.getsize(files[i])/1048576))
        if float(os.path.getsize(files[i])/1048576) > 0.1 :
            chain.Add(files[i])
    if flags.verbose: print "input chain entries from ",int(flags.files_frac*len(files))," files: ",chain.GetEntries()
    return chain


# -------------------------
def search_rse( RSE , EventsList ):
    run,subrun,event = RSE[0],RSE[1],RSE[2]
    for e in EventsList:
        if e['run'] == run and e['subrun'] == subrun and e['event'] == event:
            return True , e['ivtx-NuSel'], e['itrk-NuSelMuon'], e['itrk-GBDTproton']
    return False , -1 , -1 , -1



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
    
    IntersectionListName = mu_p_intersection_path + "/" + Sel2muons_intersection_list_csv_name( GBDTmodelName ,TracksListName ,p_score )
    df_intersection = df_intersection.rename(columns={ 'trkindex[ivtx][itrk]':'itrk-NuSelMuon',
                                             'ivtx':'ivtx-NuSel', 'trackid':'itrk-GBDTproton'} )
    df_intersection[['run','subrun','event',
                    'ivtx-NuSel','itrk-NuSelMuon',
                    'itrk-GBDTproton']].to_csv(IntersectionListName, sep=' ', header=True, index=False )
    print_filename( IntersectionListName , "wrote Sel2/GBDTprotons intersection list name:")
    print_important( "scp %s $uboone:~/"%IntersectionListName )
    print_line()









# -------------------------
def scheme_list_of_files_rse( GBDTmodelName, TracksListName , p_score ):
    '''
    This functionallity schemes (big) analysis trees
    and returns a tree containing only entries with a Run/Subrun/Event
    of a given list (RSE map)
    '''
    # input: (1) analysis trees, (2) mu-p list
    AnalysisTreesListName   = anatrees_lists_path + "/GOOD" + flags.DataType + "/filesana.list"
    IntersectionListName    = mu_p_intersection_path + "/" + Sel2muons_intersection_list_csv_name( GBDTmodelName ,TracksListName , p_score )
    # output: schemed analysis trees file
    SchemedResultFileName   = schemed_anatrees_file_name( "GOOD"+flags.DataType+"_filesana.list" , Sel2muons_intersection_list_name( GBDTmodelName ,TracksListName , p_score ) )
    it = ImportantTools()
    files = read_files_from_a_list( AnalysisTreesListName )
    in_chain = get_analysistrees_chain(files)
    OutFile = ROOT.TFile( SchemedResultFileName , "recreate" )
    OutTree = it.SchemeTreeRSEList( in_chain , IntersectionListName , flags.verbose )

    if flags.verbose:
        print_filename(SchemedResultFileName , "wrote schemed file (%d events, %.2f MB):"%(OutTree.GetEntries(),float(os.path.getsize(SchemedResultFileName)/1048576.0)))
    
    OutTree.Write()
    OutFile.Close()




# -------------------------
def extract_anatrees_tracks_information_from_files_list( DataType, Option,
                                                        MCmode=False, AddEventsList=False , EventsListName="" ):
    # flags.DataType options:   openCOSMIC_MC / extBNB / MC_BNB / BNB_5e19POT
    
    AnaTreesListName = DataType + "_AnalysisTrees"
    files       = read_files_from_a_list( AnaTreesListName )
    in_chain    = get_analysistrees_chain(files)
    
    extract_anatrees_tracks_information( in_chain , Option, MCmode, AddEventsList , EventsListName , AnaTreesListName )


# -------------------------
def extract_anatrees_tracks_information_from_a_file( DataType, InputFileName, Option,
                                                    MCmode=False,
                                                    TreeName="analysistree/anatree",
                                                    AddEventsList=False ,
                                                    EventsListName="",
                                                    mupRSEFileName="" ):
    
    AnaTreesListName = DataType + "_AnalysisTrees"
    in_chain = ROOT.TChain( TreeName )
    in_chain.Add( InputFileName )
    extract_anatrees_tracks_information( in_chain, Option, MCmode, AddEventsList, EventsListName , AnaTreesListName , mupRSEFileName )



# -------------------------
def extract_anatrees_tracks_information( in_chain, Option, MCmode=False,
                                        AddEventsList=False,
                                        EventsListName="", AnaTreesListName="", mupRSEFileName="" ):


    if Option != 'extract all tracks information' and Option != 'find common muon-proton vertices':
        print "options:"
        print "\t extract all tracks information"
        print "\t find common muon-proton vertices"
        exit(0)

    FeaturesFileName    = tracks_features_file_name( AnaTreesListName )
    TracksAnaFileName   = tracks_anafile_name( AnaTreesListName )

    if Option=="find common muon-proton vertices":
        output_rse_file = open( mupRSEFileName , "w" )

    Nentries    = in_chain.GetEntries()
    Nreduced    = int(flags.evnts_frac*(Nentries))
    if flags.verbose: print_important( "starting run on %d events"%Nreduced )
    OutFile     = ROOT.TFile(TracksAnaFileName,"recreate")
    TracksTree  = ROOT.TTree("TracksTree","pandoraNu tracks")
    GENIETree   = ROOT.TTree("GENIETree","genie interactions")
    
    calc = cumputeAnaTree( in_chain, TracksTree, FeaturesFileName, Option, flags.verbose, MCmode, GENIETree )
    
    if AddEventsList:
        import csv
        if flags.verbose: print_filename( EventsListName , "adding list of R/S/E from" )
        with open( EventsListName , 'rb') as csvfile:
            reader = csv.reader(csvfile, delimiter=' ', skipinitialspace=True)
            header = next(reader)
            rse_events_list = [dict(zip(header, map(int, row))) for row in reader]
        if flags.verbose>3: print rse_events_list

    counter = 0
    
    for entry in range(Nreduced):
        
        do_continue = True
        calc.GetEntry( entry )
        entry_rse = [calc.run,calc.subrun,calc.event]
        if flags.verbose>2: print entry_rse
        
        if AddEventsList:
            
            do_continue , ivtx_nuselection , itrk_NuSelMuon , itrk_GBDTproton = search_rse( entry_rse , rse_events_list )
            if (do_continue and flags.verbose>1): print_important("found r-%d/s-%d/e-%d, extracting information....\n"%(calc.run,calc.subrun,calc.event))
    
        if do_continue:
            
            calc.extract_information()
            
            if (flags.verbose and entry%flags.print_mod==0):
                
                calc.PrintData( entry )
            
            if Option=="extract all tracks information":
                
                do_continue = True
                ivtx_nuselection , itrk_NuSelMuon , itrk_GBDTproton = 0 , 0 , 0
            
            if Option=="find common muon-proton vertices":
                
                do_continue = True if ( itrk_NuSelMuon != itrk_GBDTproton and calc.TrkVtxDistance( ivtx_nuselection , itrk_GBDTproton ) < min_trk_vtx_distance ) else False
            
            if do_continue:
                
                counter = counter+1
                calc.CreateROIs( ivtx_nuselection , itrk_NuSelMuon , itrk_GBDTproton )
                calc.FillOutTree()
                calc.Write2CSV( ivtx_nuselection , itrk_NuSelMuon , itrk_GBDTproton )
                
                if Option=="find common muon-proton vertices":
                    
                    output_rse_file.write( "%d %d %d\n"%(calc.run, calc.subrun, calc.event ))

    print_filename( FeaturesFileName , "wrote csv file with %d tracks (%.2f MB)"%(counter,float(os.path.getsize(FeaturesFileName)/1048576.0)) )
    print_filename( TracksAnaFileName , "wrote root file (%.2f MB)"%float(os.path.getsize(TracksAnaFileName)/1048576.0) )

    if Option=="find common muon-proton vertices":
        print_filename( mupRSEFileName , "output RSE map for argofiltering muon-proton vertices" )
        output_rse_file.close()


    if MCmode: GENIETree.Write()
    TracksTree.Write()
    OutFile.Close()







