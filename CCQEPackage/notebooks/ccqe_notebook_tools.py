import sys;
sys.path.insert(0, '/Users/erezcohen/larlite/UserDev/AnalysisTreesInformation/AnaTreesPackage/notebooks');
sys.path.insert(0, '/Users/erezcohen/larlite/UserDev/AnalysisTreesInformation/AnaTreesPackage/mac')
sys.path.insert(0, '/Users/erezcohen/larlite/UserDev/MyLarLite/MyPackage/')
from notebooks_tools import *
from calc_tools import *
from mpl_toolkits.mplot3d import Axes3D
from matplotlib import cm
from matplotlib.ticker import LinearLocator, FormatStrFormatter
import matplotlib.patches as patches
from scipy.interpolate import interp1d
import ROOT
from larlite import *
#from ROOT import MyLArTools
from ROOT import PandoraNuTrack
from ROOT import cumputeAnaTree
from ROOT import ImportantTools
from ROOT import AnalyzeTracksFile
from ROOT import calcEventTopologies
from ROOT import AnalyzeVertex
import math, copy
#lar_tools = MyLArTools()
LAr_density = 1.396 # [g/cm^3]
Mmuon = 105.6 # [MeV/c^2]




figures_size = (8,4)
figures_fontsize = 15
markers_size = 6

#---------------------------------------------------------------------------------------------
# April-30, 2017
def get_fraction_in_cut( data=None , cut_var='distance', mul=1 , xmin=0.1, xmax=10 , Nbins=10 ,  cut_type= 'max' ):
    x_array = np.linspace(xmin,xmax,Nbins)
    frac , frac_err = [] , []
    denominator = len(data)
    
    for x in x_array:
        if cut_type is 'max':
            reduced = data[mul*data[cut_var]<x]
        elif cut_type is 'min':
            reduced = data[mul*data[cut_var]>x]
        numerator = float(len(reduced))
        
        frac.append(100 * numerator / denominator)
        frac_err.append( frac[-1] * np.sqrt(1./numerator + 1./denominator) ) if numerator>0 else frac_err.append( frac[-1]/np.sqrt(denominator) )

    return np.array(x_array), np.array(frac) , np.array(frac_err)
#---------------------------------------------------------------------------------------------


#---------------------------------------------------------------------------------------------
# April-30, 2017
def get_fraction_in_symmetriccut( data=None , cut_var='delta_phi', mul=1,xcenter=0.1, delta_x_min=0, delta_x_max=100 , Nbins=10 ):
    delta_x_array = np.linspace(delta_x_min,delta_x_max,Nbins)
    frac , frac_err = [] , []
    denominator = len(data)
    
    for delta_x in delta_x_array:
        reduced = data[np.abs(mul*data[cut_var]-xcenter)<delta_x]
        numerator = float(len(reduced))
        frac.append(100 * numerator / denominator)
        frac_err.append( frac[-1] * np.sqrt(1./numerator + 1./denominator) ) if numerator>0 else frac_err.append( frac[-1]/np.sqrt(denominator) )
    
    return np.array(delta_x_array), np.array(frac) , np.array(frac_err)
#---------------------------------------------------------------------------------------------


#---------------------------------------------------------------------------------------------
# April-30, 2017
def plot_feature_2tracks_clusters(samples=None,labels=None,colors=None,
                                  var='l_long',
                                  x_label='$l_{long}$ [cm]',mul=1,
                                  bins=np.linspace(0,300,100),fontsize=figures_fontsize,
                                  fig=None,do_add_legend=False,figsize=figures_size,legend_fontsize=25,
                                  Nh=2,Nw=2,i=None,ticks_color='white',legend_loc='upper center'):
    if i is not None and fig is not None:
        ax = fig.add_subplot(Nh,Nw,i)
    else:
        fig,ax = plt.subplots(figsize=figsize)
    max_h=0
    for sample,label,color in zip(samples,labels,colors):
        h,bins,_=plt.hist(mul*sample[var],normed=1,label=label,bins=bins,histtype='step',linewidth=3,color=color)
        if np.max(h)>max_h:
            max_h=np.max(h)
    if do_add_legend:
        ax.legend(fontsize=legend_fontsize,loc=legend_loc)
    set_axes(ax,x_label=x_label,fontsize=fontsize,ticks_color=ticks_color)
    ax.set_xlim(np.min(bins),np.max(bins))
    ax.set_ylim(0,1.05*max_h)
    ax.grid(linestyle='--',alpha=0.75)
    ax.xaxis.set_major_locator(LinearLocator(5));ax.yaxis.set_major_locator(LinearLocator(4))
    ax.xaxis.set_major_formatter(ticker.FormatStrFormatter('%.0f'))
    ax.yaxis.set_major_formatter(ticker.FormatStrFormatter('%.2f'))
    plt.tight_layout()
    return ax
#---------------------------------------------------------------------------------------------


#---------------------------------------------------------------------------------------------
# April-30, 2017
def plot_cut_samples (samples=None,labels=None,colors=None,
                      cut_name='maximal distance between tracks',mul=1,
                      cut_var ='distance',
                      cut_type= 'max',
                      x_label = 'maximal tracks distance [cm]', y_label='% of sample',
                      xcenter=0,figsize=figures_size,fontsize=35,
                      xmin=0.1, xmax=10 , Nbins=10, do_add_legend=True, legend_loc='best',legend_fontsize=35,
                      ax=None,ticks_color='white'):
    do_return_fig = False
    if ax is None:
        do_return_fig = True
        fig,ax=plt.subplots(figsize=figsize)
    for sample,label,color in zip(samples,labels,colors):
        if cut_type=='max' or cut_type=='min':
            x , frac , frac_err = get_fraction_in_cut( data=sample , cut_var=cut_var , mul=mul , cut_type=cut_type , xmin=xmin, xmax=xmax , Nbins=Nbins )
        elif cut_type=='symmetric':
            x , frac , frac_err = get_fraction_in_symmetriccut( data=sample , cut_var=cut_var , mul=mul , xcenter=xcenter, delta_x_min=xmin, delta_x_max=xmax , Nbins=Nbins )
        plt.errorbar(x , y=frac, yerr=frac_err , fmt='o' , markersize=markers_size , label=label, color=color)
    if do_add_legend:
        ax.legend(fontsize=legend_fontsize,loc=legend_loc)
    ax.set_ylim(0,101)
    ax.set_xlim(xmin,xmax)
    set_axes(ax,x_label=x_label,y_label=y_label,fontsize=fontsize,ticks_color=ticks_color,yticks=[20,40,60,80,100])
    ax.grid(linestyle='--',alpha=0.75)
    plt.tight_layout()
    return ax
#---------------------------------------------------------------------------------------------




#---------------------------------------------------------------------------------------------
# April-30, 2017
def plot_feature_2tracks_clusters_2d(samples=None,plottype='hist',labels=None,cmaps=None,colors=None,
                                     varx='l_long', vary='l_short',
                                     x_label='$l_{long}$ [cm]',y_label='$l_{short}$ [cm]',mulx=1,muly=1,
                                     bins=50,fontsize=figures_fontsize,
                                     fig=None,do_add_legend=False,figsize=figures_size,ticks_color='white'):
    
    if plottype=='hist': fig = plt.figure(figsize=figsize)
    elif plottype=='scatter': fig,ax = plt.subplots(figsize=figsize)
    
    for i,sample,label,cmap,color in zip(range(len(samples)),samples,labels,cmaps,colors):
        if len(sample)<1: continue
        
        if plottype=='hist':
            ax=fig.add_subplot(2,2,i+1)
            plot_2d_hist(mulx*sample[varx],muly*sample[vary],bins=bins,ax=ax,cmap=cmap,ticks_color=ticks_color)
            ax.set_title(label,y=1.02,fontsize=fontsize)
            set_axes(ax,x_label=x_label if i==2 or i==3 else '',y_label=y_label if i==0 or i==2 else '',fontsize=fontsize)
            ax.grid(linestyle='--',alpha=0.75)
            ax.xaxis.set_major_locator(LinearLocator(5));ax.yaxis.set_major_locator(LinearLocator(4))
        
        
        elif plottype=='scatter':
            ax.scatter(mulx*sample[varx],muly*sample[vary],color=color)

    if plottype=='scatter':
        ax.set_title(label,y=1.02,fontsize=fontsize)
        set_axes(ax,x_label=x_label,y_label=y_label,fontsize=fontsize,ticks_color='white')
        ax.grid(linestyle='--',alpha=0.75)
        ax.xaxis.set_major_locator(LinearLocator(5));ax.yaxis.set_major_locator(LinearLocator(4))
    plt.tight_layout()
#---------------------------------------------------------------------------------------------



#---------------------------------------------------------------------------------------------
# April 19 2017
def muon_momontum_from_range(length , debug=0 ):
    
    muon_rangegrampercm = [9.833E-1, 1.786E0, 3.321E0, 6.598E0, 1.058E1, 3.084E1, 4.250E1,
                           6.732E1, 1.063E2, 1.725E2, 2.385E2, 4.934E2,
                           6.163E2, 8.552E2, 1.202E3, 1.758E3, 2.297E3,
                           4.359E3, 5.354E3, 7.298E3, 1.013E4, 1.469E4,
                           1.910E4, 3.558E4, 4.326E4, 5.768E4, 7.734E4, 1.060E5, 1.307E5]
        
    muon_KE_MeV = [10, 14, 20, 30, 40, 80, 100, 140, 200, 300, 400, 800, 1000,
                                          1400, 2000, 3000, 4000, 8000, 10000, 14000, 20000, 30000,
                                          40000, 80000, 100000, 140000, 200000, 300000, 400000]
                           
    Range_grampercm_muon_LAr , p_MeVc_muon = [] , []
                           
    for r,KE in zip(muon_rangegrampercm,muon_KE_MeV):
        Range_grampercm_muon_LAr.append( r / LAr_density )
        E = Mmuon + KE
        p_MeVc_muon.append(np.sqrt(E*E - Mmuon*Mmuon))
                           
    p_vs_range_muon_spline_interpolation = interp1d( Range_grampercm_muon_LAr , p_MeVc_muon)

    momentum_from_range = p_vs_range_muon_spline_interpolation(length)
    if debug>0:
        print 'length:',length,'cm, momentum from range:',momentum_from_range,'MeV/c'

    return momentum_from_range
#---------------------------------------------------------------------------------------------

# ------------------------------------------------
# April 19 2017
def find_next_event_with_a_muon_track( analysistree=None, start_entry=0 ,debug=False ):
    '''
    operation: 
        look for the first analysis-tree entry with a muon track after start_entry
    return: 
        hits and tracks in this entry. 
        i - the entry number (for next iteration)
    ''' 
    for i in range(start_entry , analysistree.Nentries):
        if i%(float(analysistree.Nentries)/20)==0: print '%.1f'%(float(100.*i)/analysistree.Nentries) + '%'
        analysistree.GetEntry(i)
        tracks = [t for t in analysistree.tracks]
        for t in tracks: #{
            if np.abs(t.MCpdgCode)==13:#{
                hits = [hit for hit in analysistree.hits]
                if debug: #{
                    print 'tracks.size():',len(tracks)
                    print 'hits.size():',len(hits)
                #}
                return hits, tracks , i
            #}
        #}
    #}
    print 'did not an entry > %d with a muon track after %d,%d,%d'%(start_entry)
    return None,None,start_entry
# ------------------------------------------------

# ------------------------------------------------
def find_a_straight_line( x_array , y_array ):
    [x0,x1] = x_array
    [y0,y1] = y_array
    slope = (y1-y0)/((x1-x0) if np.abs(x1-x0)>0.001 else (y1-y0)/(0.001*np.sign(x1-x0)))
    intercept = y1 - slope*x1
    return slope,intercept
# ------------------------------------------------



# ------------------------------------------------
def sample_in_FV(sample=None, max_FV_y = 110,
                 min_FV_z = 5, max_FV_z = 1045,
                 min_FV_x = 3, max_FV_x = 250):
    sample_in_FV = sample[ 
                              (np.abs(sample['starty_assigned_muon']) < max_FV_y)        
                            & (np.abs(sample['starty_assigned_proton']) < max_FV_y)
                            & (np.abs(sample['endy_assigned_muon']) < max_FV_y)        
                            & (np.abs(sample['endy_assigned_proton']) < max_FV_y)
                            
                            & ((sample['startz_assigned_muon'] > min_FV_z) & (sample['startz_assigned_muon'] < max_FV_z) )
                            & ((sample['startz_assigned_proton'] > min_FV_z) & (sample['startz_assigned_proton'] < max_FV_z) )                            
                            & ((sample['endz_assigned_muon'] > min_FV_z) & (sample['endz_assigned_muon'] < max_FV_z) )
                            & ((sample['endz_assigned_proton'] > min_FV_z) & (sample['endz_assigned_proton'] < max_FV_z) )

                            & ((sample['startx_assigned_muon'] > min_FV_x) & (sample['startx_assigned_muon'] < max_FV_x) )
                            & ((sample['startx_assigned_proton'] > min_FV_x) & (sample['startx_assigned_proton'] < max_FV_x) )                            
                            & ((sample['endx_assigned_muon'] > min_FV_x) & (sample['endx_assigned_muon'] < max_FV_x) )
                            & ((sample['endx_assigned_proton'] > min_FV_x) & (sample['endx_assigned_proton'] < max_FV_x) )
                           ]
    return sample_in_FV
# ------------------------------------------------



# ------------------------------------------------
def get_CC1p_tracks( events=None , i=0 , debug=False ,
                            ):
        events.GetEntry(i)
        vertices = [v for v in events.vertices]
        tracks = [t for t in events.tracks]
        hits = [hit for hit in events.hits]
        if debug: print len(vertices),'vertices',len(tracks),'tracks',len(hits),'hits'
            
        TwoTracksClusters = dict()
        TwoTracksClusters['Nclusters'] = len(vertices)

        for i,v in enumerate(vertices):            
            TwoTracksClusters['vertex %d'%i] = v
            
        return hits, tracks , TwoTracksClusters 
# ------------------------------------------------

            
    
    

# ------------------------------------------------
def get_CC1p_tracks_from_rse( events=None ,run=0 , subrun=0 , event=0, debug=False ):
    # return hits and only those tracks which are close enough to the vertex in this R/S/E
    for i in range(events.Nentries):
        events.GetEntry(i)
        
        vertices = [v for v in events.vertices]

        TwoTracksClusters = dict()
        TwoTracksClusters['Nclusters'] = len(vertices)
        
        for j,v in enumerate(vertices):            
            if v.run==run and v.subrun==subrun and v.event==event:
                tracks = [t for t in events.tracks]
                hits = [hit for hit in events.hits]
                if debug: print len(vertices),'vertices',len(tracks),'tracks',len(hits),'hits'                
                TwoTracksClusters['vertex %d'%j] = v
                return hits, v        
    print 'did not find %d,%d,%d'%(run , subrun , event)
# ------------------------------------------------

    
    


# ------------------------------------------------
def get_event_from_rse( events=None,run=0 , subrun=0 , event=0 ,debug=False ):
    # return hits and tracks in this R/S/E
    for i in range(events.Nentries):
        if i%(float(events.Nentries)/20)==0: print '%.1f'%(float(100.*i)/events.Nentries) + '%'
        events.GetRSEEntry(i)
        if events.run==run and events.subrun==subrun and events.event==event:
            events.GetEntry(i)
            tracks = [t for t in events.tracks]
            if debug: print 'tracks.size():',len(tracks)
            hits = [hit for hit in events.hits]
            if debug: print 'hits.size():',len(hits)
            return hits, tracks
    print 'did not find %d,%d,%d'%(run , subrun , event)
# ------------------------------------------------




# ------------------------------------------------
def set_color_label(t,MCmode=True,do_add_PfromRange=False):
    
    pdg = t.MCpdgCode
    t_length = t.length
    
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
        color = 'slateblue'
        label = '$\\pi^-$'
    elif pdg==211:
        color = 'slateblue'
        label = '$\\pi^+$'
    elif pdg==111:
        color = 'slateblue'
        label = '$\\pi^0$'
    elif pdg==22:
        color = 'cyan'
        label = '$\\gamma$'
    else:
        color = 'purple'
        label = '%d'%pdg
        
        
    if do_add_PfromRange:
        Ptruth = t.truth_P
        if np.abs(pdg)==13:
            PfromRange = 0.001*muon_momontum_from_range(t_length)
            Pratio = 1-(Ptruth-PfromRange)/Ptruth
            if Pratio<0.8:
                print 'muon Ptruth=%.2f GeV/c, PfromRange=%.2f GeV/c (length=%.1f cm, truth start-end=%.1f)'%(Ptruth,PfromRange,t_length,(t.truth_start_pos-t.truth_end_pos).Mag())
            label = label+"($\\frac{p_{truth}-p_{rec.}}{p_{truth}}=%.2f$)"%Pratio

        
    return color , label
# ------------------------------------------------




    
    
    
# ------------------------------------------------
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
# ------------------------------------------------



# ------------------------------------------------
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



