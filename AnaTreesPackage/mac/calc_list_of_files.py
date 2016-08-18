# imports
import ROOT , os , sys, larlite, argparse
from ROOT import *
from ROOT import cumputeAnaTree
from ROOT import AnaTreeTools
import input_flags

flags = input_flags.get_args()
[debug,print_every_entry,files_fraction,evts_fraction,worker] = [flags.verbose,flags.print_mod,flags.files_frac,flags.ev_frac,flags.worker]

Path = "/Users/erezcohen/Desktop/uBoone/AnalysisTreesAna" if worker=="erez" else "/uboone/app/users/ecohen/AnalysisTreesAna"


ListsPath = Path+"/lists"
AnaPath = Path+"/BDTanaFiles"
CSVFilesPath = Path+"/CSVOutFiles"
ListName = "extBNB_AnalysisTrees"#"small_20_files_extBNB_AnalysisTrees" # "extBNB_AnalysisTrees"

tools = AnaTreeTools()


if debug>0: print "\nreading list of files..."
with open(ListsPath + "/" + ListName + ".list") as f:
    files = f.read().splitlines()
if debug>4: print files

CSVfileName = CSVFilesPath+"/"+"features_"+ListName+".csv"
AnafileName = AnaPath + "/"+"BDTana_" + ListName + ".root"

in_chain = ROOT.TChain("analysistree/anatree");

for i in range(int(files_fraction*len(files))):
    if debug>1: print "file %d size is %.2f MB"%(i,float(os.path.getsize(files[i])/1048576))
    if float(os.path.getsize(files[i])/1048576) > 0.1 :
        in_chain.Add(files[i])
if debug>0: print "input chain entries from",int(files_fraction*len(files)),"files: ", in_chain.GetEntries()


Nentries = in_chain.GetEntries()


OutFile = ROOT.TFile(AnafileName,"recreate")
OutTree = ROOT.TTree("GBDTTree","physical variables taken from nuInteraction tracking algorithm")


if debug>1: print in_chain,OutTree,debug
    
calc = cumputeAnaTree( in_chain , OutTree , CSVfileName , debug )

counter = 0
for entry in range(int(evts_fraction*(Nentries))):
        
    calc.get_bdt_tools( entry )
        
        
    if calc.Ntracks>0:
        counter += calc.Ntracks
        
        if (entry%print_every_entry == 0):
            calc.PrintData( entry )
        
        calc.WriteTracks2CSV();

        if debug>1: print "wrote %d more contained tracks (%d tracks in total) to features csv file"%(calc.Ntracks,counter)



    
if debug>0: print "extracted %d events\nout-tree has %d events\n\n" % (int(evts_fraction*(Nentries)),OutTree.GetEntries())

OutTree.Write()
OutFile.Close()


print "wrote csv file with %d tracks (%.2f MB):\n"%(counter,float(os.path.getsize(CSVfileName)/1048576.0)) + CSVfileName
print "wrote root file  (%.2f MB):\n"%float(os.path.getsize(AnafileName)/1048576.0) + AnafileName


