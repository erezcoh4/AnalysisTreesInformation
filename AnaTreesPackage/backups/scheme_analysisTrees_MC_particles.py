from anatrees_tools import *
from ROOT import TPlots, TSchemeDATA
scheme  = TSchemeDATA()
'''
    usage:
    ------
    python mac/scheme_analysisTrees_MC_particles.py  --option=purity
    
    '''



DataName = "piminus_0-2.0GeV_isotropic"
# MC_BNB / p_0.02-1.5GeV_isotropic / muminus_0-2.0GeV_isotropic / piminus_0-2.0GeV_isotropic



if flags.option == 'high_purity':
    scheme_cut = ROOT.TCut('tracks.purtruth_Y==1')
    scheme_name = 'high_purity'

if flags.option == 'low_purity':
    max_purity = 0.5
    scheme_cut = ROOT.TCut('tracks.purtruth_Y<%f'%max_purity)
    scheme_name = 'low_purity_%.2f'%max_purity

elif flags.option == 'low_pida':
    pidamax = 1
    scheme_cut = ROOT.TCut('tracks.pidpida<%d'%pidamax)
    scheme_name = 'pidamax_%d'%pidamax

elif flags.option == 'high_pida':
    pidamin = 20
    scheme_cut = ROOT.TCut('tracks.pidpida>%d'%pidamin)
    scheme_name = 'pidamin%d'%pidamin


particle_names  = ['pions']
#    ['protons'
#                   ,'muons'
#                   ,'pions']
#                   ,'em']

particle_cuts   = ['MCpdgCode==211 || MCpdgCode==-211 || MCpdgCode==111']
#    ['tracks.MCpdgCode==2212']
#                   ,'MCpdgCode==13 || MCpdgCode==-13'
#                   ,'MCpdgCode==211 || MCpdgCode==-211 || MCpdgCode==111']
#                   ,'MCpdgCode==11 || MCpdgCode==-11 || MCpdgCode==22']

all_name = "Tracks_" + DataName + "_AnalysisTrees.root"


for particle_name , particle_cut in zip(particle_names,particle_cuts):
    schemed_name = "Tracks_" + DataName + "_MC" + particle_name + "_" + scheme_name + "_AnalysisTrees.root"
    scheme.SchemeOnTCut( anafiles_path, all_name ,  'TracksTree' , schemed_name ,  ROOT.TCut(particle_cut) + scheme_cut )
    ana = TPlots( anafiles_path + "/" + schemed_name , 'TracksTree' )
    print 'completed scheming for ' + particle_name + ', Nentries = ',ana.GetEntries()



print 'done.'