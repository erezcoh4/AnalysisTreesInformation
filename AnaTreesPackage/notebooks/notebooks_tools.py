features_path = "/Users/erezcohen/Desktop/uBoone/AnalysisTreesAna/FeaturesFiles/"

import sys, pandas as pd, numpy as np, matplotlib.pyplot as plt, ast
import matplotlib as mpl , seaborn as sns; sns.set(style="white", color_codes=True , font_scale=1)
sys.path.insert(0, '/Users/erezcohen/larlite/UserDev/mySoftware/MySoftwarePackage/mac')
import GeneralPlot as gp , Initiation as init
from plot_tools import *
generic = lambda x: ast.literal_eval(x)
from matplotlib.colors import LogNorm
from matplotlib.ticker import NullFormatter,MultipleLocator, FormatStrFormatter
from matplotlib import ticker





# ------------------------------------------------------------------------
# Feb-16, 2017
def cbar(im=None,title=None):
    cbar = plt.colorbar(im)
    tick_locator = ticker.MaxNLocator(nbins=5)
    cbar.locator = tick_locator
    cbar.update_ticks()
    for t in cbar.ax.get_yticklabels(): t.set_fontsize(20)
    cbar.ax.get_yaxis().labelpad = 35
    cbar.ax.set_ylabel(title, rotation=270,fontsize=30)
# ------------------------------------------------------------------------




# ------------------------------------------------------------------------
# Feb-09, 2017
def add_retreat_label(xtext=None,ytext=None,label = 'cohen.erez7@gmail.com, mini-retreat (Feb-2017)'):
    if xtext is not None and ytext is not None:
        plt.text(xtext,ytext,label)
    else:
        plt.figtext(0.1,0.05,label)
# ------------------------------------------------------------------------

# ------------------------------------------------------------------------
# Feb-09, 2017
def plot_2d_histogram(data=None , data_name = 'MCC8-extBNB, contained pandoraCosmic tracks',
                      varx='startx' , vary='starty' , mul_x=1, mul_y=1,
                      x_label = '$x_{start}$ [cm]', y_label='$y_{start}$ [cm]',
                      cmap='hot_r',norm='normal',
                      binsx=50, binsy=50,  figsize=(16,10) , ticks_color='black', fontsize=25,
                      do_add_retreat_label=False,do_save_fig=False,do_add_title=False,
                      ax=None):
    
    
    plot2d = plot_2d_hist( mul_x*data[varx].astype(float) , mul_y*data[vary].astype(float) ,
                          bins=(binsx,binsy) , cmap=cmap,
                          xlabel=x_label , ylabel=y_label, norm=norm,
                          figsize=figsize , fontsize=fontsize , ticks_color=ticks_color , ax=ax);
    cb = plt.colorbar()
    cb.ax.tick_params(labelsize=fontsize,labelcolor=ticks_color)
                          
    counts, xedges, yedges, Image, ax = plot2d
    if do_add_title:
        ax.set_title(data_name,fontsize=25,y=1.02)


    if do_add_retreat_label:
        add_retreat_label()
    if do_save_fig:
        plt.savefig('/Users/erezcohen/Desktop/uBoone/analysis/retreats/Feb2017/%s_%s_vs_%s.png'%(namestr(data, globals()),varx,vary))
# ------------------------------------------------------------------------


# ------------------------------------------------------------------------
# Feb-09, 2017
def plot_histstep(x,bins=None,color=None,label=None,linestyle='-',normed=1):
    h,bins,_ = plt.hist( x , bins=bins, histtype='step', linewidth=0 ,normed=normed)
    mid = 0.5*(bins[1:] + bins[:-1])+0.5*(bins[1] - bins[0])
    plt.plot( mid , h , linestyle=linestyle, drawstyle='steps' , color=color, linewidth=3 , label=label )
    return h,bins,mid
# ------------------------------------------------------------------------

# ------------------------------------------------------------------------
# Feb-09, 2017
def namestr(obj, namespace):
    return [name for name in namespace if namespace[name] is obj][0]
# ------------------------------------------------------------------------



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


