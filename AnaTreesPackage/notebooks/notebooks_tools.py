features_path = "/Users/erezcohen/Desktop/uBoone/AnalysisTreesAna/FeaturesFiles/"

import sys, pandas as pd, numpy as np, matplotlib.pyplot as plt, ast
import matplotlib as mpl , seaborn as sns; sns.set(style="white", color_codes=True , font_scale=1)
sys.path.insert(0, '/Users/erezcohen/larlite/UserDev/mySoftware/MySoftwarePackage/mac')
import GeneralPlot as gp , Initiation as init
from plot_tools import *
dirname = init.createnewdir()
generic = lambda x: ast.literal_eval(x)
from matplotlib.colors import LogNorm


# hits data frame
def DataFrame_hits( tracks_df ):
    resrng , dEdx , Edep = tracks_df.residual_range_Y , tracks_df.dEdx_Y , tracks_df.Edep_Y
    if len(resrng) == 0:
        print 'there are no tracks...'
        return False
    resrng = np.concatenate(resrng.ravel())
    dEdx = np.concatenate(dEdx.ravel())
    Edep = np.concatenate(Edep.ravel())
    hits_df = pd.DataFrame(data={'residual_range':resrng,'dEdx':dEdx,'Edep':Edep})
    hits_df = hits_df[ (hits_df.dEdx>0.1) & (hits_df.dEdx<100) & (hits_df.residual_range>0.1) & (hits_df.residual_range<200) ]
    return hits_df

# hits data frame for high-purity (=1) and low-purity (<0.6) tracks
def DataFrame_hits_lowhigh_purity( tracks_df ):
    tracks_df_low_purity = tracks_df[tracks_df.purtruth_Y<0.6]
    if len(tracks_df_low_purity) == 0:
        print 'there are no low-purity tracks...'
        return False, False
    hits_df_low_purity = DataFrame_hits( tracks_df_low_purity )
    tracks_df_high_purity = tracks_df[tracks_df.purtruth_Y==1]
    if len(tracks_df_high_purity) == 0:
        print 'there are no high-purity tracks...'
        return False, False
    
    hits_df_high_purity = DataFrame_hits( tracks_df_high_purity )
    return hits_df_low_purity , hits_df_high_purity


# scatter plot
def dEdx_scatter( df_hits , label='' ,color='black' , s=0.1 , alpha=1 , x_text=30, y_text=27):
    x , y = df_hits.residual_range , df_hits.dEdx
    plt.scatter( x ,y ,s=s,color=color,alpha=alpha)
    plt.axis([0,100,0,30])
    plt.xlabel('residual range [cm]')
    plt.ylabel('$dE/dx$ [MeV/cm]')
    plt.text(x_text,y_text,label,color=color)

# scatter plot
def dEdx_scatter_mulitple( df_hits_list , labels ,colors , s=0.1 , alpha=0.8):
    c = 0
    for df_hits,label,color in zip( df_hits_list , labels , colors ):
        dEdx_scatter( df_hits , label=label ,color=color , s=s , alpha=alpha , x_text=30+2*c, y_text=27-c)
        c += 1.5

# dE/dx jointpplot
def dEdx_jointplot( df_hits , cmap='hot_r' , label='protons', xlim=(0,100) , ylim=(0,30)):
    with sns.axes_style("white"):
        g = sns.jointplot(df_hits.residual_range , df_hits.dEdx ,
                          cmap=cmap, kind="hex", stat_func=None,
                          xlim=xlim, ylim=ylim,
                          marginal_kws={'color': 'blue'})
    g.set_axis_labels('residual range [cm]' , '$dE/dx$ [MeV/cm]')
    plt.colorbar()
    g.fig.suptitle(label)

# dE/dx of expected muons/pions/kaons/protons
dEdx_truth_protons = pd.read_csv('/Users/erezcohen/Desktop/uBoone/analysis/BetheBloch/dedx_range_protons.csv')
dEdx_truth_kaons = pd.read_csv('/Users/erezcohen/Desktop/uBoone/analysis/BetheBloch/dedx_range_kaons.csv')
dEdx_truth_pions = pd.read_csv('/Users/erezcohen/Desktop/uBoone/analysis/BetheBloch/dedx_range_pions.csv')
dEdx_truth_muons = pd.read_csv('/Users/erezcohen/Desktop/uBoone/analysis/BetheBloch/dedx_range_muons.csv')

# dE/dx vs expected muons/pions/kaons/protons
def dEdx_vs_expected_particles( hits , xlim = (0,20) , ylim = (0,40) , cmap='hot_r' ):
    
    fig,ax = plt.subplots(figsize=(14,10))
    plt.hist2d(hits.residual_range , hits.dEdx  , bins=[np.linspace(xlim[0],xlim[1],100),np.linspace(ylim[0],ylim[1],100)], cmap=cmap);
    plt.plot(dEdx_truth_protons['residual range [cm]'],dEdx_truth_protons['dE/dx [MeV/cm]'],color='purple',label='expected protons dE/dx')
    plt.plot(dEdx_truth_kaons['residual range [cm]'],dEdx_truth_kaons['dE/dx [MeV/cm]'],color='green',label='expected kaons dE/dx')
    plt.plot(dEdx_truth_pions['residual range [cm]'],dEdx_truth_pions['dE/dx [MeV/cm]'],color='blue',label='expected pions dE/dx')
    plt.plot(dEdx_truth_muons['residual range [cm]'],dEdx_truth_muons['dE/dx [MeV/cm]'],color='black',label='expected muons dE/dx')
    pt.set_axes(ax,'residual range [cm]','dE/dx [MeV/cm]')
    plt.legend(fontsize=25)
    ax.set_xlim(xlim)
    ax.set_ylim(ylim)



# acceptance plots for protons
def uB_acceptace_1d( var='P', x_label='momentum [GeV/c]',
                    bins=np.linspace(0,2,30) ,
                    figsize=(10,10),fontsize=20,
                    legend_loc='lower left',
                    y_lim=None):
    gen_protons = MCBNB_gen_protons[var][MCBNB_gen_protons.length>1.5]
    rec_protons = MCBNB_rec_protons['truth_'+var]
    return acceptace_1d( MCBNB_gen_protons[var][MCBNB_gen_protons.length>1.5] , MCBNB_rec_protons['truth_'+var] ,
                        bins=bins , figsize=figsize,fontsize=fontsize,
                        legend_loc=legend_loc, y_lim=y_lim, x_label=x_label)


