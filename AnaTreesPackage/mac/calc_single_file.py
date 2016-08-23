'''
    usage:
    ---------
    > python calc_single_file.py --DataType=MC --worker=uboone -v2 -evf=0.001 -p1
'''


import ROOT , os , sys
from ROOT import cumputeAnaTree , gROOT
sys.path.insert(0, '/Users/erezcohen/larlite/UserDev/mySoftware/MySoftwarePackage/mac')
sys.path.insert(0, '/uboone/app/users/ecohen/larlite/UserDev/mySoftware/MySoftwarePackage/mac')
import input_flags
flags = input_flags.get_args()




Path            = "/pnfs/uboone/persistent/users/aschu/MC_BNB_Cosmic"
FileName        = "prodgenie_bnb_nu_cosmic_uboone_v05_08_00_anatree"
AnafileName     = "/uboone/app/users/ecohen/Ana_"+FileName+".root"

pot_chain       = ROOT.TChain("analysistree/pottree")
pot_chain.Add( Path + "/" + FileName + ".root" )
pot_chain.Draw("pot>>histoPOT","","goff")
h               = gROOT.FindObject("histoPOT")
POTtot          = h.GetEntries()*h.GetMean()
POTscaleFactor  = 6.6e20 / POTtot


in_chain = ROOT.TChain("analysistree/anatree")
in_chain.Add( Path + "/" + FileName + ".root" )
Nentries = in_chain.GetEntries()
Nevents_uBooNE = Nentries * POTscaleFactor

if(flags.verbose > 0):
    print "total number of POT in analysis tree is ", POTtot
    print "total number of evets in analysis tree is ", Nentries
    print "scale factor for MicroBooNE: ",POTscaleFactor
    print "total number of evets in MicroBooNE: ", Nevents_uBooNE


OutFile = ROOT.TFile(AnafileName,"recreate")
OutTree = ROOT.TTree("anaTree","physical variables")
GENIETree = ROOT.TTree("GENIETree","genie interactions")

MCmode = True if flags.DataType == "MC" else False
calc = cumputeAnaTree( in_chain , OutTree , GENIETree , flags.verbose , MCmode)


for entry in range(int(flags.evnts_frac*(calc.Nentries))):
    
    calc.extract_information( entry )
    
    if ( calc.NnuInteractions > 0 ):
        
        calc.FillOutTree()
        
        if (flags.verbose > 0 and entry%flags.print_mod == 0):
            calc.PrintData( entry )
        


print "wrote root file (%d events , %.2f MB):\n"%(OutTree.GetEntries(),float(os.path.getsize(AnafileName)/1048576.0)) + AnafileName


GENIETree.Write()
OutTree.Write()
OutFile.Close()


