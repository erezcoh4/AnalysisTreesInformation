'''
    usage:
    ------
    python mac/scheme_list_of_files_rse.py --DataType=EXTBNB -wuboone -v2 -ff=0.01 -evf=0.1
'''

import ROOT , os , sys, larlite , datetime
sys.path.insert(0, '/uboone/app/users/ecohen/larlite/UserDev/mySoftware/MySoftwarePackage/mac')
sys.path.insert(0, '/Users/erezcohen/larlite/UserDev/mySoftware/MySoftwarePackage/mac')
from ROOT import ImportantTools
import input_flags
flags = input_flags.get_args()


FilesListType   = "GOOD" + flags.DataType
FilesListName   = "filesana"
FilesListsPath  = "/pnfs/uboone/persistent/users/aschu/devel/v05_11_01/hadd"

proton_score    = 0.90
EventsListName  = "mu_p_score_%.2f_intersection"%proton_score
EventsListsPath = "/uboone/data/users/ecohen/Lists/muon_proton_intersection"

SchemedPath     = "/uboone/data/users/ecohen/AnalysisTreeData"
SchemedfileName= SchemedPath + "/SchemedFiles/" + FilesListType + "_" + FilesListName + "_" + EventsListName + ".root"

tools           = ImportantTools()


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


OutFile     = ROOT.TFile(SchemedfileName,"recreate")
OutTree     = tools.SchemeTreeRSEList ( in_chain , EventsListsPath + "/" + EventsListName + ".csv" )

print "wrote root file (%d events , %.2f MB):\n"%(OutTree.GetEntries(),float(os.path.getsize(SchemedfileName)/1048576.0)) + SchemedfileName
print "wrote csv file with (%.2f MB):\n"%(float(os.path.getsize(CSVfileName)/1048576.0)) + CSVfileName

OutTree.Write()
OutFile.Close()


