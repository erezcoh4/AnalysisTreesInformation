features_path = "/Users/erezcohen/Desktop/uBoone/AnalysisTreesAna/FeaturesFiles/"
features_path = "/Users/erezcohen/Desktop/uBoone/AnalysisTreesAna/FeaturesFiles/"

import sys, pandas as pd, numpy as np, matplotlib.pyplot as plt, ast
import matplotlib as mpl , seaborn as sns; sns.set(style="white", color_codes=True , font_scale=1)
sys.path.insert(0, '/Users/erezcohen/larlite/UserDev/mySoftware/MySoftwarePackage/mac')
import GeneralPlot as gp , Initiation as init, plot_tools as pt
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


def dEdx_jointplot( df_hits , cmap='hot_r' , label='protons', xlim=(0,100) , ylim=(0,30)):
    with sns.axes_style("white"):
        g = sns.jointplot(df_hits.residual_range , df_hits.dEdx ,
                          cmap=cmap, kind="hex", stat_func=None,
                          xlim=xlim, ylim=ylim,
                          marginal_kws={'color': 'blue'})
    g.set_axis_labels('residual range [cm]' , '$dE/dx$ [MeV/cm]')
    plt.colorbar()
    g.fig.suptitle(label)