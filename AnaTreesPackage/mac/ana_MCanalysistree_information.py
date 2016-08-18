'''
    usage:
    ---------
    > python mac/ana_MCanalysistree_information.py --worker=erez -option=GENIE-interactions -var=SRCvars
'''


import ROOT , os , sys , math
from ROOT import TPlots , gROOT, nuInteraction , GENIEinteraction
from rootpy.interactive import wait
sys.path.insert(0, '../../mySoftware/MySoftwarePackage/mac')
import Initiation as init , GeneralPlot as gp, input_flags
import matplotlib.pyplot as plt
import collections
init.createnewdir()
flags = input_flags.get_args()




FileName    = "prodgenie_bnb_nu_cosmic_uboone_v05_08_00_anatree"
AnaPath     = "/Users/erezcohen/Desktop/uBoone/AnaFiles"
AnafileName = AnaPath+"/Ana_"+FileName+".root"
plot        = TPlots( AnafileName , "anaTree" )
anaGENIE    = TPlots( AnafileName , "GENIETree" )
POTscaleFactor = 2.87 # see AnaPath/README
var     = collections.namedtuple( 'variable', ' name nbins x_min x_max xtit src_cut_min src_cut_max' )
var2d   = collections.namedtuple( 'variable', ' namex nbinsx x_min x_max xtit src_x_min src_x_max \
                                                namey nbinsy y_min y_max ytit src_y_min src_y_max' )

if flags.option   == 12:  flags.option   = 'short-tracks'
if flags.variable == 12:  flags.variable = 'NNeighborTracks'

c = anaGENIE.CreateCanvas(flags.variable)


SRCcutString = "Np >= 1 && Npi==0 && \
                Xb > 1.2 && 0.3 < n_miss.P() && n_miss.P() < 1 && \
                theta_pq < 25 && 0.62 < p_over_q && p_over_q < 0.96 && Mmiss<1.1"

ppSRCcutString = SRCcutString + " && Np >= 2 && protons[1].P()>0.3"

SRCcut = ROOT.TCut(SRCcutString)
ppSRCcut = ROOT.TCut(ppSRCcutString)

def draw_var_and_cut( v ):
    
    print "ploting\n",v
    
    hSRC = anaGENIE.H1( v.name , ROOT.TCut("(%f<%s && %s<%f)*%f"%(v.src_cut_min,v.name,v.name,v.src_cut_max,POTscaleFactor)) ,"goff" ,
                    v.nbins , v.x_min , v.x_max )
    h = anaGENIE.H1( v.name , ROOT.TCut("%f"%POTscaleFactor) ,"hist" ,
                    v.nbins , v.x_min , v.x_max , "" , v.xtit )
    plot.Line( v.src_cut_min , 0 , v.src_cut_min , h.GetMaximum() , 2 , 2 )
    plot.Line( v.src_cut_max , 0 , v.src_cut_max , h.GetMaximum() , 2 , 2 )
    percentageSRC=100*float(hSRC.Integral()/h.Integral())
    plot.Text(v.src_cut_min+0.3*(v.src_cut_max-v.src_cut_min),0.7*h.GetMaximum(),"%.1f%% in range"%percentageSRC,2,0.07 )
    h.GetXaxis().SetTitleSize(0.07)
    h.GetXaxis().SetLabelSize(0.07)
    h.GetYaxis().SetTitleSize(0.07)
    h.GetYaxis().SetLabelSize(0.07)
    c.Update()
    return c


def draw_var_and_cut_2d( v ):
    
    print "ploting\n",v
    
    hSRC = anaGENIE.H2( v.namex , v.namey ,
                    flags.cut + ROOT.TCut("((%f<%s && %s<%f)&&(%f<%s && %s<%f))*%f"
                                          %(v.src_x_min,v.namex,v.namex,v.src_x_max,
                                            v.src_y_min,v.namey,v.namey,v.src_y_max,POTscaleFactor)) ,
                    "goff" ,
                    v.nbinsx , v.x_min , v.x_max ,
                    v.nbinsy , v.y_min , v.y_max )
    h = anaGENIE.H2( v.namex , v.namey , flags.cut + ROOT.TCut("%f"%POTscaleFactor) , "colz" ,
                    v.nbinsx , v.x_min , v.x_max ,
                    v.nbinsy , v.y_min , v.y_max ,
                    "" , v.xtit , v.ytit )
    plot.ROI(v.src_x_min , v.src_y_min , v.src_x_max , v.src_y_max)
    percentageSRC=100*float(hSRC.Integral()/h.Integral())
    x_text = v.src_x_min + 0.3*(v.src_x_max-v.src_x_min)
    y_text = v.src_y_min + 0.3*(v.src_y_max-v.src_y_min)
    plot.Text( x_text , y_text , "%.1f%% in range"%percentageSRC , 2, 0.07 )
    h.GetXaxis().SetTitleSize(0.07)
    h.GetXaxis().SetLabelSize(0.07)
    h.GetYaxis().SetTitleSize(0.07)
    h.GetYaxis().SetLabelSize(0.07)
    c.Update()
    return c





if flags.option=="GENIE-interactions":

    if flags.variable == "SRCvars":
        
        c.Divide(2,2)
        
        c.cd(1)
        c = draw_var_and_cut( var( "Xb", 100, 0, 4 , gp.XbTit , 1.2 , 4 ) )
        
        c.cd(2)
        c = draw_var_and_cut( var( "n_miss.P()", 100, 0, 2, gp.PmissTit , 0.3 , 1 ) )
        
        c.cd(3)
        c = draw_var_and_cut_2d( var2d( "p_over_q", 100, 0, 2, "|#vec{p}(lead)|/|#vec{q}|" , 0.62 , 0.96,
                                    "theta_pq", 100, 0, 180, gp.ThetaTit("#vec{p}(lead),#vec{q}") , 0 , 25) )
        
        c.cd(4)
        c = draw_var_and_cut( var( "Mmiss", 100, 0, 2, "M_{miss} [GeV/c]" , 0 , 1.1 ) )


    if flags.variable == "count-events-in-SRC-cuts":
        print "src cuts:\n"
        SRCcut.Print()
        print "number of contained GENIE interactions in SRC cuts:"
        print "in analysis tree: ",anaGENIE.GetEntries(SRCcut)
        print "scaled to 6.6e20 POT: ",(anaGENIE.GetEntries(SRCcut)*POTscaleFactor)
        print "number of contained GENIE interactions in ppSRC cuts:"
        print "in analysis tree: ",anaGENIE.GetEntries(ppSRCcut)
        print "scaled to 6.6e20 POT: ",(anaGENIE.GetEntries(ppSRCcut)*POTscaleFactor)

    wait()
    c.SaveAs(init.dirname()+"/MCanalysistree_"+flags.variable+".pdf")







elif flags.option == "(nu,mu pp)/(nu,mu p)":
    
    Nbins       = 12
    
    
    hnumup = anaGENIE.H1( "n_miss.P()", SRCcut, "goff", Nbins , 0.3 ,0.9 )
    hnumupp = anaGENIE.H1( "n_miss.P()", ppSRCcut, "goff", Nbins , 0.3 , 0.9 )
    
    n_miss = []
    n_missErr = []
    ratio = []
    ratioErr = []
    
    for bin in range(Nbins):
        
        n_miss.append(hnumup.GetXaxis().GetBinCenter(bin))
        n_missErr.append(hnumup.GetXaxis().GetBinWidth(bin)/2.)
        
        if hnumup.GetBinContent(bin)<0.1 or hnumupp.GetBinContent(bin)<0.1 :
            ratio.append( 0 )
            ratioErr.append( 0 )
        
        else:
            r = hnumupp.GetBinContent(bin) / hnumup.GetBinContent(bin)
            ratio.append( 100*r )
            err = r * math.sqrt( 1. / hnumupp.GetBinContent(bin) + 1. / hnumup.GetBinContent(bin) )
            ratioErr.append( 100*err )


    fig = plt.figure(1, figsize = (6,4) )
    plt.errorbar(n_miss, ratio, fmt='ro', xerr=n_missErr, yerr=ratioErr, ecolor='black')
    plt.xlabel(r'$|\vec{p}_{miss}|$ [GeV/c]',fontsize=22)
    plt.ylabel(r'${\frac{^{40}Ar(\nu,\mu pp)}{^{40}Ar(\nu,\mu p)}}$ [%]',fontsize=25)
    ax = fig.add_subplot(111)
    ax.text( 0.4, 5, "preliminary - not corrected for purities and inefficiencies" , fontsize=22 , color='red' , alpha = 0.15 )
    plt.savefig(init.dirname()+"/ppSRC_40Ar_numupp_numup_ratio.pdf")
    plt.show()








