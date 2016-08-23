'''
    
    usage:
    ------
    > python calc_single_file.py --DataType=EXTBNB --worker=uboone -p1 -v2 --option="select muon-proton scattering" -ff=0.001 -evf=0.1
    
    
    option="select muon-proton scattering":
    ---------------------------------------
    Look for (mu,mu'p) scattering with the topology of 3 tracks sharing one vertex where one of them is a proton and the others are muons, using calorimetry information:
    there is a variable called 'trkpidpdg' (3 for each track, one per plane)
    These (mu,mu'p) events are to be used for testing the GBDT classification

'''

import ROOT , os , sys, larlite , datetime
sys.path.insert(0, '../../mySoftware/MySoftwarePackage/mac')
from ROOT import cumputeAnaTree , AnaTreeTools
import input_flags
flags = input_flags.get_args()



ListType        = "GOOD" + flags.DataType
ListName        = "filesana"
ListsPath       = "/pnfs/uboone/persistent/users/aschu/devel/v05_11_01/hadd"
AnaPath         = "/uboone/data/users/ecohen/AnalysisTreeData" if flags.worker=="uboone" else ""
AnafileName     = AnaPath + "/Ana_" + ListType + "_" + ListName + "_" + datetime.datetime.now().strftime("%Y%B%d") + ".root"
tools           = AnaTreeTools()


if debug>0: print "\nreading list of files: " + ListsPath + "/" + ListType + "/" + ListName + ".list"

with open( ListsPath + "/" + ListType + "/" + ListName + ".list" ) as f:
    files = f.read().splitlines()

if debug>4: print files


in_chain = ROOT.TChain("analysistree/anatree");

for i in range(int(files_fraction*len(files))):
    if debug>1: print "file %d size is %.2f MB"%(i,float(os.path.getsize(files[i])/1048576))
    if float(os.path.getsize(files[i])/1048576) > 0.1 :
        in_chain.Add(files[i])
if debug>0: print "input chain entries from",int(files_fraction*len(files)),"files: ", in_chain.GetEntries()


Nentries    = in_chain.GetEntries()


OutFile     = ROOT.TFile(AnafileName,"recreate")
OutTree     = ROOT.TTree("anaTree","physical variables per event")
GENIETree   = ROOT.TTree("GENIETree","genie interactions")



calc = cumputeAnaTree( in_chain , OutTree , GENIETree , debug )

for entry in range(int(evts_fraction*(Nentries))):
        
    calc.extract_information( entry )
        
    if ( flags.option=="select muon-proton scattering" ):
        
        calc.FillOutTree()

        if (flags.verbose > 0 and entry%flags.print_mod == 0):
            calc.PrintData( entry )





print "wrote root file (%d events , %.2f MB):\n"%(OutTree.GetEntries(),float(os.path.getsize(AnafileName)/1048576.0)) + AnafileName


GENIETree.Write()
OutTree.Write()
OutFile.Close()


