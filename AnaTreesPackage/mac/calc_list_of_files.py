'''
    
    usage:
    ------
    python mac/calc_list_of_files.py --DataType=EXTBNB --worker=uboone -p1 -v2 --option=mu-p-vertex -var=AddEventsList -ff=0.01 -evf=0.1
    
    
    option="select muon-proton scattering":
    ---------------------------------------
    Look for (mu,mu'p) scattering with the topology of 3 tracks sharing one vertex where one of them is a proton and the others are muons, using calorimetry information:
    there is a variable called 'trkpidpdg_pandoraNu' (3 for each track, one per plane)
    These (mu,mu'p) events are to be used for testing the GBDT classification

'''

import ROOT , os , sys, larlite , datetime
sys.path.insert(0, '/uboone/app/users/ecohen/larlite/UserDev/mySoftware/MySoftwarePackage/mac')
sys.path.insert(0, '/Users/erezcohen/larlite/UserDev/mySoftware/MySoftwarePackage/mac')
from ROOT import cumputeAnaTree , AnaTreeTools
import input_flags
flags = input_flags.get_args()

min_trk_vtx_distance = 5 # [cm]

FilesListType   = "GOOD" + flags.DataType
FilesListName   = "filesana"
FilesListsPath  = "/pnfs/uboone/persistent/users/aschu/devel/v05_11_01/hadd"

proton_score    = 0.90
EventsListName  = "mu_p_score_%.2f_intersection"%proton_score
EventsListsPath = "/uboone/data/users/ecohen/Lists/muon_proton_intersection"

AnaPath         = "/uboone/data/users/ecohen/AnalysisTreeData"
AnafileName     = AnaPath + "/ROOTFiles/Ana_" + FilesListType + "_" + FilesListName + "_" + datetime.datetime.now().strftime("%Y%B%d") + ".root"
CSVfileName     = AnaPath + "/CSVFiles/features_" + FilesListType + "_" + FilesListName + "_" + datetime.datetime.now().strftime("%Y%B%d") + ".root"
MCmode          = True if flags.DataType=='MC' else False
tools           = AnaTreeTools()



if flags.worker=="erez":
    EventsListsPath = "/Users/erezcohen/Desktop/uBoone/Lists/muon_proton_intersection"




'''
    choose from a list of events
    e.g. if we want to choose from events with a classified proton of score > XXYY
'''

if flags.variable=="AddEventsList":
    
    if flags.verbose>0: print "\nadding list of events: " + EventsListsPath+"/"+EventsListName+".csv"
    import csv
    
    with open(EventsListsPath+"/"+EventsListName+".csv", 'rb') as csvfile:
        reader = csv.reader(csvfile, delimiter=' ', skipinitialspace=True)
        header = next(reader)
        EventsList = [dict(zip(header, map(int, row))) for row in reader]

    print EventsList

def search(run,subrun,event):
    for e in EventsList:
        if e['run'] == run and e['subrun'] == subrun and e['event'] == event:
            return True , e['ivtx-nuselection'], e['itrk-GBDTprotons']
    return False , -1 , -1







if flags.verbose>0: print "\nreading list of files: " + FilesListsPath + "/" + FilesListType + "/" + FilesListName + ".list"

with open( FilesListsPath + "/" + FilesListType + "/" + FilesListName + ".list" ) as f:
    files = f.read().splitlines()

if flags.verbose>4: print files


in_chain = ROOT.TChain("analysistree/anatree");

for i in range(int(flags.files_frac*len(files))):
    if flags.verbose>1: print "file %d size is %.2f MB"%(i,float(os.path.getsize(files[i])/1048576))
    if float(os.path.getsize(files[i])/1048576) > 0.1 :
        in_chain.Add(files[i])
if flags.verbose>0: print "input chain entries from",int(flags.files_frac*len(files)),"files: ", in_chain.GetEntries()


Nentries    = in_chain.GetEntries()


OutFile     = ROOT.TFile(AnafileName,"recreate")
OutTree     = ROOT.TTree("anaTree","physical variables per event")
GENIETree   = ROOT.TTree("GENIETree","genie interactions")



calc = cumputeAnaTree( in_chain , OutTree , GENIETree , CSVfileName , flags.verbose , MCmode )



for entry in range(int(flags.evnts_frac*(Nentries))):
    
    do_continue = True
    
    calc.GetEntry( entry )

    if (flags.verbose>2 and calc.run>6000): print calc.run,calc.subrun,calc.event
    
    if flags.variable=="AddEventsList":

        do_continue , ivtx_nuselection , itrk_GBDTprotons = search(calc.run,calc.subrun,calc.event)

        if (do_continue and flags.verbose>1):   print "\n\n\nfound r-%d/s-%d/e-%d, extracting information....\n\n\n\n"%(calc.run,calc.subrun,calc.event)

    if do_continue:

        calc.extract_information()

        if (flags.verbose > 0 and flags.verbose < 7 and entry%flags.print_mod == 0):
        
            calc.PrintData( entry )

        if ( flags.option=="mu-p-vertex" and calc.TrkVtxDistance( ivtx_nuselection , itrk_GBDTprotons ) < min_trk_vtx_distance ):
            
            if (flags.verbose>1):   print "\n\n\ntrack %d is closer to vertex %d than 5 cm! saving the event...\n\n\n"%( ivtx_nuselection , itrk_GBDTprotons )
            calc.CreateNuSelVtxROI( ivtx_nuselection )
            calc.FillOutTree()
            calc.Write2CSV()







print "wrote root file (%d events , %.2f MB):\n"%(OutTree.GetEntries(),float(os.path.getsize(AnafileName)/1048576.0)) + AnafileName
print "wrote csv file with (%.2f MB):\n"%(float(os.path.getsize(CSVfileName)/1048576.0)) + CSVfileName


GENIETree.Write()
OutTree.Write()
OutFile.Close()


