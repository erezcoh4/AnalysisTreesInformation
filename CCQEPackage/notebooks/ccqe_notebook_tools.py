import sys;
sys.path.insert(0, '/Users/erezcohen/larlite/UserDev/AnalysisTreesInformation/AnaTreesPackage/notebooks');
sys.path.insert(0, '/Users/erezcohen/larlite/UserDev/AnalysisTreesInformation/AnaTreesPackage/mac')
from notebooks_tools import *
from calc_tools import *
from mpl_toolkits.mplot3d import Axes3D
from matplotlib import cm
from matplotlib.ticker import LinearLocator, FormatStrFormatter
import ROOT
from larlite import *
from ROOT import PandoraNuTrack
from ROOT import cumputeAnaTree
from ROOT import ImportantTools
from ROOT import AnalyzeTracksFile
from ROOT import calcEventTopologies
from ROOT import calcEventTopologies



def get_event_from_rse( events, run , subrun , event ):
    # return Ntracks and tracks in this R/S/E
    for i in range(events.Nentries):
        events.GetEntry(i)
        if events.run==run and events.subrun==subrun and events.event==event:
            Ntracks = events.Ntracks
            tracks = events.tracks
            return events.Ntracks, events.tracks
    print 'did not find %d,%d,%d'%(run , subrun , event)



def set_color_label(pdg,MCmode=True):
    if MCmode==False:
        color = 'purple'
        label = ''
        return color , label
    
    if pdg==-13:
        color = 'black'
        label = '$\\mu^+$'
    elif pdg==13:
        color = 'black'
        label = '$\\mu^-$'
    elif pdg==-11:
        color = 'red'
        label = '$e^+$'
    elif pdg==11:
        color = 'black'
        label = '$e^-$'
    elif pdg==2212:
        color = 'blue'
        label = '$p$'
    elif pdg==2112:
        color = 'salmon'
        label = '$n$'
    elif pdg==-211:
        color = 'yellow'
        label = '$\\pi^-$'
    elif pdg==211:
        color = 'yellow'
        label = '$\\pi^+$'
    elif pdg==111:
        color = 'yellow'
        label = '$\\pi^0$'
    elif pdg==22:
        color = 'cyan'
        label = '$\\gamma$'
    else:
        color = 'black'
        label = '%d'%pdg
    return color , label




def draw_evd_tracks(tracks, do_print=False,fontsize=20,figsize=(16,6),MCmode=True,
                    vertex_x=None,vertex_y=None,vertex_z=None):
    fig = plt.figure(figsize=figsize)
    
    # x-y view
    ax = fig.add_subplot(1,3,1)
    for t in tracks:
        color, label = set_color_label(t.MCpdgCode,MCmode=MCmode)
        plt.plot([t.startx,t.endx],[t.starty,t.endy], color=color)
    ax.grid(linestyle='--',alpha=0.5)
    set_axes(ax,x_label='x [cm]' , y_label='y [cm]',fontsize=fontsize)
    if vertex_y is not None:
        for x,y,z in zip(vertex_x,vertex_y,vertex_z):
            circ=plt.Circle((x,y), 8, color='green',alpha=0.4)
            ax.add_artist(circ)
    ax.xaxis.set_major_locator(LinearLocator(5))
    ax.yaxis.set_major_locator(LinearLocator(5))


    # z-y view
    ax = fig.add_subplot(1,3,(2,3))
    counter=0
    for t in tracks:
        counter+=1
        color, label = set_color_label(t.MCpdgCode,MCmode=MCmode)
        if do_print: print [t.track_id,t.starty,t.endy]
        plt.plot([t.startz,t.endz],[t.starty,t.endy],
                 label='track %d '%t.track_id+label,color=color)
    if MCmode: ax.legend(loc='best',fontsize=fontsize)
    ax.grid(linestyle='--',alpha=0.5)
    set_axes(ax,x_label='z [cm]',fontsize=fontsize)
    ax.yaxis.set_major_formatter( NullFormatter() )
    if vertex_y is not None:
        for x,y,z in zip(vertex_x,vertex_y,vertex_z):
            circ=plt.Circle((z,y), 8, color='green',alpha=0.4)
            ax.add_artist(circ)
    ax.xaxis.set_major_locator(LinearLocator(5))
    ax.yaxis.set_major_locator(LinearLocator(5))
    
    
    # 3d - view
    fig = plt.figure(figsize=(14,6))
    ax = fig.gca(projection='3d')
    for t in tracks:
        color, label = set_color_label(t.MCpdgCode,MCmode=MCmode)
        if do_print: print [t.track_id,t.starty,t.endy]
        plt.plot([t.startz,t.endz],[t.startx,t.endx],[t.starty,t.endy],color=color)

    ax.set_xlabel('z [cm]',labelpad = 20)
    ax.set_ylabel('x [cm]',labelpad = 20)
    ax.set_zlabel('y [cm]',labelpad = 20)

    ax.xaxis.set_major_locator(LinearLocator(5))
    ax.yaxis.set_major_locator(LinearLocator(5))
    ax.zaxis.set_major_locator(LinearLocator(5))
    if vertex_y is not None:
        for x,y,z in zip(vertex_x,vertex_y,vertex_z):
            ax.scatter(z , x , y, s=100, c='green', alpha=0.4)
    plt.tight_layout()

