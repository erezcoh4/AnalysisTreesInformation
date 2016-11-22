from anatrees_tools import *
from ROOT import TPlots, TSchemeDATA
scheme  = TSchemeDATA()
'''
    usage:
    ------
    python mac/scheme_analysisTrees_MC_particles.py  --option=low_pida
    
    '''



DataName = "MC_BNB"
#main_path = "/Users/erezcohen/Desktop/uBoone/AnalysisTreesAna"
#anafiles_path = main_path + "/TracksAnaFiles"




if flags.option == 'purity':
    purity = 0.9999
    scheme_cut = ROOT.TCut('tracks.purtruth_Y>%f'%purity)
    scheme_name = 'trkpurity%f'%purity

elif flags.option == 'low_pida':
    pidamax = 1
    scheme_cut = ROOT.TCut('tracks.pidpida<%d'%pidamax)
    scheme_name = 'pidamax_%d'%pidamax

elif flags.option == 'high_pida':
    pidamin = 20
    scheme_cut = ROOT.TCut('tracks.pidpida>%d'%pidamin)
    scheme_name = 'pidamin%d'%pidamin


particle_names  = ['protons']
#                   ,'muons'
#                   ,'pions'
#                   ,'em']

particle_cuts   = ['tracks.MCpdgCode==2212']
#                   ,'MCpdgCode==13 || MCpdgCode==-13'
#                   ,'MCpdgCode==211 || MCpdgCode==-211 || MCpdgCode==111' 
#                   ,'MCpdgCode==11 || MCpdgCode==-11 || MCpdgCode==22']

all_name = "Tracks_" + DataName + "_AnalysisTrees.root"


for particle_name , particle_cut in zip(particle_names,particle_cuts):
    schemed_name = "Tracks_" + DataName + "_MC" + particle_name + "_" + scheme_name + "_AnalysisTrees.root"
    scheme.SchemeOnTCut( anafiles_path, all_name ,  'TracksTree' , schemed_name ,  ROOT.TCut(particle_cut) + scheme_cut )
    ana = TPlots( anafiles_path + "/" + schemed_name , 'TracksTree' )
    print 'completed scheming for ' + particle_name + ', Nentries = ',ana.GetEntries()



print 'done.'