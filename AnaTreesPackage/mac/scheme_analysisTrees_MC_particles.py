from anatrees_tools import *
from ROOT import TPlots, TSchemeDATA
scheme  = TSchemeDATA()


DataName = "MC_BNB"
main_path = "/Users/erezcohen/Desktop/uBoone/AnalysisTreesAna"
anafiles_path = main_path + "/TracksAnaFiles"

purity = 0.99
cutPurity = ROOT.TCut('tracks.purtruth_Y>%f'%purity)



scheme.SchemeOnTCut( anafiles_path, "Tracks_" + DataName + "_AnalysisTrees.root", "TracksTree", "Tracks_" + DataName + "_MCprotons_trkpurity%f_AnalysisTrees.root"%purity,
                    ROOT.TCut('tracks.MCpdgCode==2212') + cutPurity)
ana_MCp = TPlots( anafiles_path + "/Tracks_" + DataName + "_MCprotons_trkpurity%f_AnalysisTrees.root" , 'TracksTree' )
print 'generated ana_MCp, Nentries=',ana_MCp.GetEntries()

scheme.SchemeOnTCut( anafiles_path, "Tracks_" + DataName + "_AnalysisTrees.root", "TracksTree", "Tracks_" + DataName + "_MCmuons_trkpurity%f_AnalysisTrees.root"%purity,
                    ROOT.TCut('MCpdgCode==13 || MCpdgCode==-13') + cutPurity)

ana_MCmu = TPlots( anafiles_path + "/Tracks_" + DataName + "_MCmuons_trkpurity%f_AnalysisTrees.root" , 'TracksTree' )
print 'generated ana_MCmu, Nentries=',ana_MCmu.GetEntries()

scheme.SchemeOnTCut( anafiles_path, "Tracks_" + DataName + "_AnalysisTrees.root", "TracksTree","Tracks_" + DataName + "_MCpions_trkpurity%f_AnalysisTrees.root"%purity,
                    ROOT.TCut('MCpdgCode==211 || MCpdgCode==-211 || MCpdgCode==111') + cutPurity)

ana_MCpi = TPlots( anafiles_path + "/Tracks_" + DataName + "_MCpions_trkpurity%f_AnalysisTrees.root" , 'TracksTree' )
print 'generated ana_MCpi, Nentries=',ana_MCpi.GetEntries()

scheme.SchemeOnTCut( anafiles_path, "Tracks_" + DataName + "_AnalysisTrees.root", "TracksTree","Tracks_" + DataName + "_MCem_trkpurity%f_AnalysisTrees.root"%purity,
                    ROOT.TCut('MCpdgCode==11 || MCpdgCode==-11 || MCpdgCode==22') + cutPurity)

ana_MCem = TPlots( anafiles_path + "/Tracks_" + DataName + "_MCem_trkpurity%f_AnalysisTrees.root" , 'TracksTree' )
print 'generated ana_MCem, Nentries=',ana_MCem.GetEntries()

