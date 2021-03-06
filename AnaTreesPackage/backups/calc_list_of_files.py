'''
    
    usage:
    ------
    python mac/calc_list_of_files.py --DataType=EXTBNB -wuboone -p1 -v2 --option=mu-p-vertex -var=AddEventsList -evf=0.1
    last eddited: Sep-2/2016
    
'''

import ROOT , os , sys, larlite , datetime
sys.path.insert(0, '/uboone/app/users/ecohen/larlite/UserDev/mySoftware/MySoftwarePackage/mac')
sys.path.insert(0, '/Users/erezcohen/larlite/UserDev/mySoftware/MySoftwarePackage/mac')
from ROOT import cumputeAnaTree , AnaTreeTools
import input_flags
flags = input_flags.get_args()

min_trk_vtx_distance = 10 # [cm]

FilesListType   = "GOOD" + flags.DataType
FilesListName   = "filesana"
FilesListsPath  = "/pnfs/uboone/persistent/users/aschu/devel/v05_11_01/hadd"

proton_score    = 0.90
EventsListType  = "mu_p_score_%.2f_intersection"%proton_score
EventsListsPath = "/uboone/data/users/ecohen/Lists/muon_proton_intersection"
EventsListName  = EventsListsPath + "/" + EventsListType + ".csv"

SchemedPath     = "/uboone/data/users/ecohen/AnalysisTreeData"
SchemedfileName= SchemedPath + "/SchemedFiles/" + FilesListType + "_" + FilesListName + "_" + EventsListType + ".root"

AnaPath         = "/uboone/data/users/ecohen/AnalysisTreeData"
AnafileName     = AnaPath + "/ROOTFiles/Ana_" + FilesListType + "_" + FilesListName + "_" + datetime.datetime.now().strftime("%Y%B%d") + ".root"
CSVfileName     = AnaPath + "/CSVFiles/features_" + FilesListType + "_" + FilesListName + "_" + datetime.datetime.now().strftime("%Y%B%d") + ".csv"
MCmode          = True if flags.DataType=='MC' else False



if flags.worker=="erez":
    EventsListsPath = "/Users/erezcohen/Desktop/uBoone/Lists/muon_proton_intersection"


if flags.variable=="AddEventsList":
    
    if flags.verbose>0: print "\nadding list of events:\n" + EventsListName
    import csv
    with open( EventsListName , 'rb') as csvfile:
        reader = csv.reader(csvfile, delimiter=' ', skipinitialspace=True)
        header = next(reader)
        EventsList = [dict(zip(header, map(int, row))) for row in reader]
    if flags.verbose>3: print EventsList

def search(run,subrun,event):
    for e in EventsList:
        if e['run'] == run and e['subrun'] == subrun and e['event'] == event:
            return True , e['ivtx-NuSel'], e['itrk-NuSelMuon'], e['itrk-GBDTproton']
    return False , -1 , -1 , -1


InFile      = ROOT.TFile(SchemedfileName)
InTree      = InFile.Get("anatree")
Nentries    = InTree.GetEntries()


OutFile     = ROOT.TFile(AnafileName,"recreate")
OutTree     = ROOT.TTree("anaTree","physical variables per event")
GENIETree   = ROOT.TTree("GENIETree","genie interactions")



calc = cumputeAnaTree( InTree , OutTree , GENIETree , CSVfileName , flags.verbose , MCmode )



for entry in range(int(flags.evnts_frac*(Nentries))):
    
    do_continue = True
    
    calc.GetEntry( entry )

    if (flags.verbose>2): print calc.run,calc.subrun,calc.event
    
    if flags.variable=="AddEventsList":

        do_continue , ivtx_nuselection , itrk_NuSelMuon , itrk_GBDTproton = search(calc.run,calc.subrun,calc.event)

        if (do_continue and flags.verbose>1):   print "\n\n\nfound r-%d/s-%d/e-%d, extracting information....\n\n\n\n"%(calc.run,calc.subrun,calc.event)

    if do_continue:

        calc.extract_information()

        if (flags.verbose > 0 and flags.verbose < 7 and entry%flags.print_mod == 0):
        
            calc.PrintData( entry )

        if ( flags.option=="mu-p-vertex" and itrk_NuSelMuon != itrk_GBDTproton and calc.TrkVtxDistance( ivtx_nuselection , itrk_GBDTproton ) < min_trk_vtx_distance ):
            
            if (flags.verbose>1):   print "\n\n\ntrack %d is closer to vertex %d than 5 cm! saving the event...\n\n\n"%( ivtx_nuselection , itrk_GBDTproton )
            calc.CreateROIs( ivtx_nuselection , itrk_NuSelMuon , itrk_GBDTproton  )
            calc.FillOutTree()
            calc.Write2CSV( ivtx_nuselection , itrk_NuSelMuon , itrk_GBDTproton )


Nevents = OutTree.GetEntries()

GENIETree.Write()
OutTree.Write()
OutFile.Close()


print "wrote root file (%d events , %.2f MB):\n"%(Nevents,float(os.path.getsize(AnafileName)/1048576.0)) + AnafileName
print "wrote csv file (%.2f MB):\n"%(float(os.path.getsize(CSVfileName)/1048576.0)) + CSVfileName

