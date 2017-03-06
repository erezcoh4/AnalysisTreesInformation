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
import math, copy




def get_event_from_rse( events=None,run=0 , subrun=0 , event=0 ):
    # return hits and tracks in this R/S/E
    for i in range(events.Nentries):
        events.GetEntry(i)
        if events.run==run and events.subrun==subrun and events.event==event:
            tracks = [t for t in events.tracks]
            print 'tracks.size():',len(tracks)
            hits = [hit for hit in events.hits]
            print 'hits.size():',len(hits)
            return hits, tracks
    print 'did not find %d,%d,%d'%(run , subrun , event)



def draw_evd_tracks(hits,tracks,
                    do_print=False,fontsize=20,figsize=(16,6),MCmode=True,
                    vertex_x=None,vertex_y=None,vertex_z=None):
    
    # 3-planes
    fig = plt.figure(figsize=figsize)
    for plane in range(3):
        ax = fig.add_subplot(1,3,plane+1)
        hit_wire , hit_time = [] , []
        # hits
        for hit in hits:
            if hit.hit_plane==plane:
                hit_wire.append(hit.hit_wire)
                hit_time.append(hit.hit_peakT)
        plt.hexbin(hit_wire,hit_time,cmap='hot_r')
        # tracks
        for t in tracks:
            color, label = set_color_label(t.MCpdgCode,MCmode=MCmode)
            if plane==0:
                start_wire,start_time = t.start_wire_u, t.start_time_u
                end_wire,end_time = t.end_wire_u, t.end_time_u

            elif plane==1:
                start_wire,start_time = t.start_wire_v, t.start_time_v
                end_wire,end_time = t.end_wire_v, t.end_time_v

            elif plane==2:
                start_wire,start_time = t.start_wire_y, t.start_time_y
                end_wire,end_time = t.end_wire_y, t.end_time_y
                
            ax.plot( [start_wire,end_wire] , [start_time,end_time] ,label='track %d '%t.track_id+label, color=color,alpha=0.4)
        ax.xaxis.set_major_locator(LinearLocator(4))
        ax.yaxis.set_major_locator(LinearLocator(4))
        set_axes(ax,x_label='wire',y_label='peak-time' if plane==0 else '',fontsize=fontsize)
        if plane!=0: ax.yaxis.set_major_formatter( NullFormatter() )
        if MCmode: ax.legend(loc='best',fontsize=fontsize)
    plt.tight_layout()
    
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
        color = 'red'
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



    
    
    
class Simplex:
    def __init__(self, testfunc, guess, increments, kR = -1, kE = 2, kC = 0.5):
        """Initializes the simplex.
        INPUTS
        ------
        testfunc      the function to minimize
        guess[]       an list containing initial guesses
        increments[]  an list containing increments, perturbation size
        kR            reflection constant
        kE            expansion constant
        kC            contraction constant
        """
        self.testfunc = testfunc
        self.guess = guess
        self.increments = increments
        self.kR = kR
        self.kE = kE
        self.kC = kC
        self.numvars = len(self.guess)


    def minimize(self, epsilon = 0.0001, maxiters = 250, monitor = 1, **kwargs):
        """Walks to the simplex down to a local minima.
        INPUTS
        ------
        epsilon       convergence requirement
        maxiters      maximum number of iterations
        monitor       if non-zero, progress info is output to stdout  

        OUTPUTS
        -------
        an array containing the final values
        lowest value of the error function
        number of iterations taken to get here
        """
        self.simplex = []

        self.lowest = -1
        self.highest = -1
        self.secondhighest = -1

        self.errors = []
        self.currenterror = 0
        # Initialize vertices
        for vertex in range(0, self.numvars + 3): # Two extras to store centroid and reflected point
            self.simplex.append(copy.copy(self.guess))
        # Use initial increments
        for vertex in range(0, self.numvars + 1):
            for x in range(0, self.numvars):
                if x == (vertex - 1):
                    self.simplex[vertex][x] = self.guess[x] + self.increments[x]
            self.errors.append(0)
        self.calculate_errors_at_vertices(**kwargs)

        iter = 0
        for iter in range(0, maxiters):
            # Identify highest, second highest, and lowest vertices
            self.highest = 0
            self.lowest = 0
            for vertex in range(0, self.numvars + 1):
                if self.errors[vertex] > self.errors[self.highest]:
                    self.highest = vertex
                if self.errors[vertex] < self.errors[self.lowest]:
                    self.lowest = vertex
            self.secondhighest = 0
            for vertex in range(0, self.numvars + 1):
                if vertex == self.highest:
                    continue
                if self.errors[vertex] > self.errors[self.secondhighest]:
                    self.secondhighest = vertex
            # Test for convergence
            S = 0.0
            S1 = 0.0
            for vertex in range(0, self.numvars + 1):
                S = S + self.errors[vertex]
            F2 = S / (self.numvars + 1)
            for vertex in range(0, self.numvars + 1):
                S1 = S1 + (self.errors[vertex] - F2)**2
            T = math.sqrt(S1 / self.numvars)
            
            # Optionally, print progress information
            if monitor:
		    print '#%d: Best = %f   Worst = %f' % (iter,self.errors[self.lowest],self.errors[self.highest]),
		    for vertex in range(0, self.numvars + 1):
			    print "[",
			    for x in range(0, self.numvars):
				print "%.2f" % self.simplex[vertex][x],
			    print "]",
		    print

                
            if T <= epsilon:   # We converged!  Break out of loop!
                break;
            else:                   # Didn't converge.  Keep crunching.
                # Calculate centroid of simplex, excluding highest vertex
                for x in range(0, self.numvars):
                    S = 0.0
                    for vertex in range(0, self.numvars + 1):
                        if vertex == self.highest:
                            continue
                        S = S + self.simplex[vertex][x]
                    self.simplex[self.numvars + 1][x] = S / self.numvars

                self.reflect_simplex()

                self.currenterror = self.testfunc(self.guess, **kwargs)

                if self.currenterror < self.errors[self.lowest]:
                    tmp = self.currenterror
                    self.expand_simplex()
                    self.currenterror = self.testfunc(self.guess, **kwargs)
                    if self.currenterror < tmp:
                        self.accept_expanded_point()
                    else:
                        self.currenterror = tmp
                        self.accept_reflected_point()

                elif self.currenterror <= self.errors[self.secondhighest]:
                    self.accept_reflected_point()

                elif self.currenterror <= self.errors[self.highest]:
                    self.accept_reflected_point()

                    self.contract_simplex()
                    self.currenterror = self.testfunc(self.guess, **kwargs)
                    if self.currenterror < self.errors[self.highest]:
                        self.accept_contracted_point()
                    else:
                        self.multiple_contract_simplex(**kwargs)

                elif self.currenterror >= self.errors[self.highest]:
                    self.contract_simplex()
                    self.currenterror = self.testfunc(self.guess, **kwargs)
                    if self.currenterror < self.errors[self.highest]:
                        self.accept_contracted_point()
                    else:
                        self.multiple_contract_simplex(**kwargs)

        # Either converged or reached the maximum number of iterations.
        # Return the lowest vertex and the currenterror.
        for x in range(0, self.numvars):
            self.guess[x] = self.simplex[self.lowest][x]
        self.currenterror = self.errors[self.lowest]
        return self.guess, self.currenterror, iter

    def contract_simplex(self):
        for x in range(0, self.numvars):
            self.guess[x] = self.kC * self.simplex[self.highest][x] + (1 - self.kC) * self.simplex[self.numvars + 1][x]
        return

    def expand_simplex(self):
        for x in range(0, self.numvars):
            self.guess[x] = self.kE * self.guess[x]                 + (1 - self.kE) * self.simplex[self.numvars + 1][x]
        return

    def reflect_simplex(self):
        for x in range(0, self.numvars):
            self.guess[x] = self.kR * self.simplex[self.highest][x] + (1 - self.kR) * self.simplex[self.numvars + 1][x]
            self.simplex[self.numvars + 2][x] = self.guess[x] # REMEMBER THE REFLECTED POINT
        return

    def multiple_contract_simplex(self, **kwargs):
        for vertex in range(0, self.numvars + 1):
            if vertex == self.lowest:
                continue
            for x in range(0, self.numvars):
                self.simplex[vertex][x] = 0.5 * (self.simplex[vertex][x] + self.simplex[self.lowest][x])
        self.calculate_errors_at_vertices(**kwargs)
        return

    def accept_contracted_point(self):
        self.errors[self.highest] = self.currenterror
        for x in range(0, self.numvars):
            self.simplex[self.highest][x] = self.guess[x]
        return

    def accept_expanded_point(self):
        self.errors[self.highest] = self.currenterror
        for x in range(0, self.numvars):
            self.simplex[self.highest][x] = self.guess[x]
        return

    def accept_reflected_point(self):
        self.errors[self.highest] = self.currenterror
        for x in range(0, self.numvars):
            self.simplex[self.highest][x] = self.simplex[self.numvars + 2][x]
        return

    def calculate_errors_at_vertices(self,**kwargs):
        for vertex in range(0, self.numvars + 1):
            if vertex == self.lowest:
                continue
            for x in range(0, self.numvars):
                self.guess[x] = self.simplex[vertex][x]
            self.currenterror = self.testfunc(self.guess, **kwargs)
            self.errors[vertex] = self.currenterror
        return



