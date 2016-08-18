import ROOT , os , sys
from ROOT import TPlots
import matplotlib.pyplot as plt
sys.path.insert(0, '/Users/erezcohen/larlite/UserDev/mySoftware/MySoftwarePackage/mac')
sys.path.insert(0, '/uboone/app/users/ecohen/larlite/UserDev/mySoftware/MySoftwarePackage/mac')
#import Initiation as init
import GeneralPlot as gp, input_flags
flags = input_flags.get_args()


Path = "/pnfs/uboone/persistent/users/aschu/MC_BNB_Cosmic/"
FileName = "prodgenie_bnb_nu_cosmic_uboone_v05_08_00_anatree"
SavePath = "/uboone/data/users/ecohen/plots" if flags.worker == 'uboone' else "/Users/erezcohen/Desktop"

operation = flags.operation
var = flags.variable
print operation + ", " + var

#InFile = ROOT.TFile(Path+"/"+FileName+".root")
#InTree = InFile.Get("analysistree/anatree")

ana = TPlots(Path+"/"+FileName+".root" , "analysistree/anatree")
NtreeEntries = ana.GetEntries()
print "NtreeEntries: ", NtreeEntries
#Nevents =




if operation == "contained-CCQE":

    contained_CCQE = ROOT.TCut(" \
                               ccnc_truth = 0 && \
                               3.45 < nuvtxx_truth && nuvtxx_truth < 249.8 && \
                               -110.53 < nuvtxy_truth && nuvtxy_truth < 112.47 && \
                               5.1 < nuvtxz_truth && nuvtxz_truth < 1031.9 && \
                               ")
    
    uBooNEtitle = "MicroBooNE expectation for contained CCQE (6.6e20 POT)"
    
    c = ana.CreateCanvas(var)
    
    if var == 'pot':
        
        h = ana.H1( 'pot', flags.cut , "hist same" , 100 , 0 , 3 , "simulated POT" , "protons on target" )
        Npot = h.Integral()
        ana.Text(1.2,0.8*h.GetMaximum(),"")

    
    elif var == 'Q2':
        
        h = ana.H1( 'Q2_truth', contained_CCQE , "hist same" , 100 , 0 , 3 , uBooNEtitle , gp.Q2tit )

        ana.Line(1,0,1,h.GetMaximum())
        ana.Text(1.2,0.8*h.GetMaximum(),"")


    elif var == 'ppSRCPmiss':
    
        CCQEcutSRC = ROOT.TCut( "\
                               1.2 < X_truth && \
                               hitnuc_truth == 2112 && \
                               
                               "
        )
        h = ana.H1( 'Q2_truth', contained_CCQE , "hist same" , 100 , 0 , 3 , uBooNEtitle , gp.Q2tit )
        
        ana.Line(1,0,1,h.GetMaximum())
        ana.Text(1.2,0.8*h.GetMaximum(),"")
    


    c.SaveAs( SavePath+"/uBexpectation_"+var+".pdf")
    print "saved plot:\n"+SavePath+"/uBexpectation_"+var+".pdf"


