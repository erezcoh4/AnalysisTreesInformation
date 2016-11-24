import sys
sys.path.insert(0, '/uboone/app/users/ecohen/larlite/UserDev/GBDTprotons/GBDTprotonPackage/mac')
sys.path.insert(0, '/Users/erezcohen/larlite/UserDev/GBDTprotons/GBDTprotonPackage/mac')
from uboone_tools import *
from gbdt_tools import *
from larlite import *
from ROOT import PandoraNuTrack
from ROOT import cumputeAnaTree
from ROOT import ImportantTools
from ROOT import AnalyzeTracksFile


# globals
# -------------------------
min_trk_vtx_distance = 10 # [cm], this distance needs to be studied wisely


# paths
# -------------------------

neutrinoSel2_path       = lists_path + "/NeutrinoSelection2"
mu_p_intersection_path  = lists_path + "/muon_proton_intersection"
schemed_anatrees_path   = anatrees_data_path  + "/SchemedFiles"


results_features = ['run', 'subrun' , 'event'
                    , 'Ntracks']





# list names and file names
# -------------------------
def Sel2muons_list_name(DataType = "BNB_5e19POT"):
    if DataType == "BNB_5e19POT":
        neutrinoSel2_list_name = "BeamOnData_pandoraNu_pandoraNu.csv"
    elif DataType == "EXTBNB":
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
def tracks_features_file_name( ListName , first_anatree_file = 0 , last_anatree_file = 0 ):
    if first_anatree_file==last_anatree_file:
        return featuresfiles_path + "/" + "features_" + ListName + ".csv"
    else:
        return featuresfiles_path + "/" + "features_" + ListName + "_anatreefiles_%d_to_%d.csv"%(first_anatree_file,last_anatree_file)

# -------------------------
def tracks_full_features_file_name( ListName , first_anatree_file = 0 , last_anatree_file = 0 ):
    if first_anatree_file==last_anatree_file:
        return featuresfiles_path + "/" + "full_features_" + ListName + ".csv"
    else:
        return featuresfiles_path + "/" + "full_features_" + ListName + "_anatreefiles_%d_to_%d.csv"%(first_anatree_file,last_anatree_file)

# -------------------------
def tracks_anafile_name( ListName , first_anatree_file = 0 , last_anatree_file = 0 ):
    if first_anatree_file==last_anatree_file:
        return anafiles_path + "/" + "Tracks_" + ListName + ".root"
    else:
        return anafiles_path + "/" + "Tracks_" + ListName + "_anatreefiles_%d_to_%d.root"%(first_anatree_file,last_anatree_file)





# methods
# -------------------------
def read_files_from_a_list( ListName , first_anatree_file = 0 , last_anatree_file = 0 ):
    # returns the files
    if flags.verbose: print_filename(ListName,"reading list of files (collecting files %d to %d)..."%(first_anatree_file , last_anatree_file))
    with open( lists_path + "/analysis_trees/" + ListName + ".list") as f:
        files = f.read().splitlines()
    if last_anatree_file > first_anatree_file:
        files = files[ first_anatree_file : last_anatree_file ]
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
    
    # input: (1) proton list
    GBDTpListName = GBDTclassListName( GBDTmodelName, TracksListName, 'protons', p_score ,'roi')
    print_filename( GBDTpListName , "opening GBDT protons list name:")
    # input: (2) selection2 muons list
    Sel2muListName  = Sel2muons_list_name( TracksListName )
    print_filename( Sel2muListName , "opening Sel2 muons list name:")

    df_GBDTp = pd.read_csv( GBDTpListName , delimiter=' ' )
    df_Sel2mu = pd.read_csv( Sel2muListName , delimiter=' ' )
    df_intersection = pd.merge(df_GBDTp, df_Sel2mu, how='inner', on=['run', 'subrun','event'])
    
    # output: intesection of (1) and (2)
    IntersectionListName = mu_p_intersection_path + "/" + Sel2muons_intersection_list_csv_name( GBDTmodelName ,TracksListName ,p_score )
    df_intersection = df_intersection.rename(columns={ 'trkindex[ivtx][itrk]':'itrk-NuSelMuon',
                                             'ivtx':'ivtx-NuSel', 'trackid':'itrk-GBDTproton'} )
    df_intersection[['run','subrun','event',
                    'ivtx-NuSel','itrk-NuSelMuon',
                    'itrk-GBDTproton']].to_csv(IntersectionListName, sep=' ', header=True, index=False )
    print_filename( IntersectionListName , "intersected Sel2/GBDTprotons lists (%d tracks)"%len(df_intersection))
    print_important( "more %s \nscp %s $uboone:~/"%(IntersectionListName,IntersectionListName) )
    print_line()









# -------------------------
def scheme_list_of_files_rse( GBDTmodelName, TracksListName , p_score ):
    '''
    This functionallity schemes (big) analysis trees
    and returns a tree containing only entries with a Run/Subrun/Event
    of a given list (RSE map)
    '''
    # input: (1) analysis trees
    AnaTreesListName = flags.DataType + "_AnalysisTrees" # AnalysisTreesListName   = anatrees_lists_path + "/GOOD" + flags.DataType + "/filesana.list"
    print_filename( AnaTreesListName , "input: (1) analysis trees ")
    # input: (2) intersected mu-p list
    IntersectionListName    = mu_p_intersection_path + "/" + Sel2muons_intersection_list_csv_name( GBDTmodelName ,TracksListName , p_score )
    print_filename( IntersectionListName , "input (2): intersected Sel2/GBDTprotons lists ")
    
    # output: schemed analysis trees file
    SchemedResultFileName   = schemed_anatrees_file_name( AnaTreesListName , Sel2muons_intersection_list_name( GBDTmodelName ,TracksListName , p_score ) )
    it = ImportantTools()
    files = read_files_from_a_list( AnaTreesListName )
    in_chain = get_analysistrees_chain(files)
    OutFile = ROOT.TFile( SchemedResultFileName , "recreate" )
    OutTree = it.SchemeTreeRSEList( in_chain , IntersectionListName , flags.verbose )

    if flags.verbose:
        print_filename(SchemedResultFileName , "schemed anatrees file (%d events, %.2f MB):"%(OutTree.GetEntries(),float(os.path.getsize(SchemedResultFileName)/1048576.0)))
    
    OutTree.Write()
    OutFile.Close()




# -------------------------
def extract_anatrees_tracks_information_from_files_list( DataType, Option,
                                                        first_anatree_file , last_anatree_file ,
                                                        MCmode=False, AddEventsList=False , EventsListName="" ):
    # flags.DataType options:   openCOSMIC_MC / extBNB / MC_BNB / BNB_5e19POT / single particles....
    
    AnaTreesListName = DataType + "_AnalysisTrees"
    files       = read_files_from_a_list( AnaTreesListName , first_anatree_file , last_anatree_file )
    in_chain    = get_analysistrees_chain(files)
    
    extract_anatrees_tracks_information_with_all_features( in_chain , Option, first_anatree_file , last_anatree_file,
                                        MCmode, AddEventsList , EventsListName , AnaTreesListName )

#extract_anatrees_tracks_information

# -------------------------
def extract_anatrees_tracks_information_from_a_file( DataType, InputFileName, Option,
                                                    MCmode=False,
                                                    TreeName="analysistree/anatree",
                                                    AddEventsList=False ,
                                                    EventsListName="",
                                                    output_mupRSEFileName="" ):
    
    AnaTreesListName = DataType + "_AnalysisTrees"
    in_chain = ROOT.TChain( TreeName )
    in_chain.Add( InputFileName )
    extract_anatrees_tracks_information( in_chain, Option, 0, 0 , MCmode, AddEventsList, EventsListName , AnaTreesListName , output_mupRSEFileName )



# -------------------------
def extract_anatrees_tracks_information( in_chain, Option,
                                        first_anatree_file=0, last_anatree_file=1,
                                        MCmode=False,
                                        AddEventsList=False,
                                        EventsListName="", AnaTreesListName="", output_mupRSEFileName="" ):


    if Option != 'extract all tracks information' and Option != 'find common muon-proton vertices':
        print "options:"
        print "\t extract all tracks information"
        print "\t find common muon-proton vertices"
        exit(0)

    FeaturesFileName    = tracks_features_file_name( AnaTreesListName , first_anatree_file , last_anatree_file )
    TracksAnaFileName   = tracks_anafile_name( AnaTreesListName , first_anatree_file , last_anatree_file )

    if Option=="find common muon-proton vertices":
        output_rse_file = open( output_mupRSEFileName , "w" )

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
        if flags.verbose>3:
            print rse_events_list


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
        print_filename( output_mupRSEFileName , "output RSE map for argofiltering muon-proton vertices" )
        output_rse_file.close()


    if MCmode:
        GENIETree.Write()

    TracksTree.Write()
    OutFile.Close()









# ------------------------------------------------------------------------------- #
def open_csv_generate_writer( filename ):
    
    writer = csv.writer(open(filename, 'wb'))
    writer.writerow(results_features)
    return writer



# -------------------------
def extract_anatrees_tracks_information_with_all_features( in_chain, Option,
                                                          first_anatree_file=0, last_anatree_file=1,
                                                          MCmode=False,
                                                          AddEventsList=False,
                                                          EventsListName="", AnaTreesListName="", output_mupRSEFileName="" ):
    
    
    if Option != 'extract all tracks information' and Option != 'find common muon-proton vertices':
        print "options:"
        print "\t extract all tracks information"
        print "\t find common muon-proton vertices"
        exit(0)

    FeaturesFileName    = tracks_features_file_name( AnaTreesListName , first_anatree_file , last_anatree_file )
    TracksAnaFileName   = tracks_anafile_name( AnaTreesListName , first_anatree_file , last_anatree_file )
    resutls_file_name   = tracks_full_features_file_name( AnaTreesListName , first_anatree_file , last_anatree_file )
    writer = open_csv_generate_writer( resutls_file_name )


    if Option=="find common muon-proton vertices":
        output_rse_file = open( output_mupRSEFileName , "w" )
    
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
        if flags.verbose>3:
            print rse_events_list


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
                
                
                tracks = calc.tracks
                print 'len(tracks):',len(tracks) 
                resutls = [calc.run, calc.subrun , calc.event
                           , len(tracks) ]
                writer.writerow( resutls )
                print "writer.writerow( [calc.run, calc.subrun , calc.event] )"



                if Option=="find common muon-proton vertices":
                    
                    output_rse_file.write( "%d %d %d\n"%(calc.run, calc.subrun, calc.event ))

    print_filename( FeaturesFileName , "wrote csv file with %d tracks (%.2f MB)"%(counter,float(os.path.getsize(FeaturesFileName)/1048576.0)) )
    print_filename( resutls_file_name , "wrote csv file with all %d tracks features (%.2f MB)"%(counter,float(os.path.getsize(resutls_file_name)/1048576.0)) )
    print_filename( TracksAnaFileName , "wrote root file (%.2f MB)"%float(os.path.getsize(TracksAnaFileName)/1048576.0) )
    
    if Option=="find common muon-proton vertices":
        print_filename( output_mupRSEFileName , "output RSE map for argofiltering muon-proton vertices" )
        output_rse_file.close()

    if MCmode:  GENIETree.Write()
    
    TracksTree.Write()
    OutFile.Close()




