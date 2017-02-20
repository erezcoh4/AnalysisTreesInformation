'''
    
    find combinations of topological cuts that maximize the 
    classification efficiency and purity of CC1p selecion
    
    '''

import sys,ast; sys.path.insert(0, '../AnaTreesPackage/mac')
from anatrees_tools import *
from calc_tools import *
generic = lambda x: ast.literal_eval(x)



Npoints = flags.NumberOfRuns if flags.NumberOfRuns > 0 and flags.NumberOfRuns < 1000 else 2


# reduce the data
def reduce_dataframe( data=None , var='distance' , type='lower' , ref=0 ): #{
    if type is 'lower':
        return data[data[var] < ref]
    else:
        return data[data[var] > ref]
#}
# --------------------------------------------------------


# true CC1p pairs
if 'erez' in flags.worker :
    ccqe_candidates_path = '/Users/erezcohen/Desktop/uBoone/analysis/ccqe_candidates'
elif 'helion' in flags.worker :
    ccqe_candidates_path = '/extra/Erez/uBoone/analysis/ccqe_candidates'


truthCC1p = pd.read_csv( ccqe_candidates_path + '/ccqe_candidates_TruthCC1p_MCC7_MC_BNB.csv',
                        converters={'track_id':generic,'distances':generic,'delta_phi':generic})
print len(truthCC1p),'true cc1p vertices'


# MC pairs with CC1p topology

MCevents = pd.read_csv( ccqe_candidates_path + '/ccqe_candidates_CC1pTopology_MCC7_MC_BNB.csv',
                       converters={'track_id':generic,'distances':generic,'delta_phi':generic})

print len(MCevents),' vertices with cc1p topology'



# MC pairs with CC1p topology that are true CC1p

MCeventsTrueCC1p = MCevents[MCevents.truth_cc1p==1]
MCeventsNonTrueCC1p = MCevents[MCevents.truth_cc1p==0]
print len(MCeventsTrueCC1p)


cuts_combination_filename = ccqe_candidates_path + "/cuts_combinations.csv"
print_filename(cuts_combination_filename , "cuts combinations" )

dmax            = np.linspace(0,10,Npoints)
llongmin        = np.linspace(10,80,Npoints)
lshortmax       = np.linspace(0,80,Npoints)
dphimin         = np.linspace(0,150,Npoints)
dphimax         = np.linspace(150,360,Npoints)
PIDA_short_min  = np.linspace(5,20,Npoints)
PIDA_short_max  = np.linspace(15,200,Npoints)
PIDA_long_min   = np.linspace(0,10,Npoints)
PIDA_long_max   = np.linspace(5,200,Npoints)

counter=0

for i,d in enumerate(dmax): #{
    
    print '%.0f'%(100*float(i)/len(dmax))+'%'

    reduced_data_d = reduce_dataframe( truthCC1p , 'distance' , 'lower' , d )
    if len(reduced_data_d) < 0.5*len(truthCC1p): continue # do not go on if efficiency is smaller than 50%

    
    for llmin in llongmin: #{

        reduced_data_llmin = reduce_dataframe( reduced_data_d , 'l_long' , 'greater' , llmin )
        if len(reduced_data_llmin) < 0.5*len(truthCC1p): continue # do not go on if efficiency is smaller than 50%


        for lsmax in lshortmax: #{


            reduced_data_lsmax = reduce_dataframe( reduced_data_llmin , 'l_short' , 'lower' , lsmax )
            if len(reduced_data_lsmax) < 0.5*len(truthCC1p): continue # do not go on if efficiency is smaller than 50%


            for dpmin in dphimin: #{
    
            
                reduced_data_dpmin = reduce_dataframe( reduced_data_lsmax , 'delta_phi' , 'greater' , dpmin )
                if len(reduced_data_dpmin) < 0.5*len(truthCC1p): continue # do not go on if efficiency is smaller than 50%
            
            
                for dpmax in dphimax: #{
            
            
                    reduced_data_dpmax = reduce_dataframe( reduced_data_dpmin , 'delta_phi' , 'lower' , dpmax )
                    if len(reduced_data_dpmax) < 0.5*len(truthCC1p): continue # do not go on if efficiency is smaller than 50%
            


                    for PIDA_smin in PIDA_short_min: #{


                        reduced_data_PIDA_smin = reduce_dataframe( reduced_data_dpmax , 'PIDA_short' , 'greater' , PIDA_smin )
                        if len(reduced_data_PIDA_smin) < 0.5*len(truthCC1p): continue # do not go on if efficiency is smaller than 50%


                        for PIDA_smax in PIDA_short_max: #{
            
            
                            reduced_data_PIDA_smax = reduce_dataframe( reduced_data_PIDA_smin , 'PIDA_short' , 'lower' , PIDA_smax )
                            if len(reduced_data_PIDA_smax) < 0.5*len(truthCC1p): continue # do not go on if efficiency is smaller than 50%
                
        
                            for PIDA_lmin in PIDA_long_min: #{
    
    
                                reduced_data_PIDA_lmin = reduce_dataframe( reduced_data_PIDA_smax , 'PIDA_long' , 'greater' , PIDA_lmin )
                                if len(reduced_data_PIDA_lmin) < 0.5*len(truthCC1p): continue # do not go on if efficiency is smaller than 50%
            
            
            
                                for PIDA_lmax in PIDA_long_max: #{
                
                
                                    reduced_data_PIDA_lmax = reduce_dataframe( reduced_data_PIDA_lmin , 'PIDA_long' , 'lower' , PIDA_lmax )
                                    if len(reduced_data_PIDA_lmax) < 0.5*len(truthCC1p): continue # do not go on if efficiency is smaller than 50%
                
                
        
        
                    
                                    reduced_CC1p = reduced_data_PIDA_lmax

                                    reduced_MCevents = MCevents[  (MCevents['distance'] < d)
                                                                & (MCevents['l_long'] > llmin )
                                                                & (MCevents['l_short']  < lsmax )
                                                                & (MCevents['delta_phi'] > dpmin)
                                                                & (MCevents['delta_phi'] < dpmax)
                                                                & (MCevents['PIDA_short'] > PIDA_smin)
                                                                & (MCevents['PIDA_short'] < PIDA_smax)
                                                                & (MCevents['PIDA_long'] > PIDA_lmin)
                                                                & (MCevents['PIDA_long'] < PIDA_lmax)
                                                                ]

                                    reduced_MCevents_trueCC1p = reduced_MCevents[reduced_MCevents['truth_cc1p']==1]
                                             
                                             
                                    # efficiency ( + uncertainty)
                                    den1 = len(truthCC1p) if len(truthCC1p)>0 else 1e20
                                    eff = 100.*float(len(reduced_CC1p))/den1
                                    if eff<60: continue
                                    den2 = len(reduced_CC1p) if len(reduced_CC1p)>0 else 1e20
                                    eff_err = eff*np.sqrt( 1./den1 + 1./den2 )
                                    

                                    # purity ( + uncertainty)
                                    den1 = len(reduced_MCevents) if len(reduced_MCevents)>0 else 1e20
                                    pur =  100.*float(len(reduced_MCevents_trueCC1p))/den1
                                    if pur<40: continue
                                    den2 = len(reduced_MCevents_trueCC1p) if len(reduced_MCevents_trueCC1p)>0 else 1e20
                                    pur_err = pur*np.sqrt( 1./den1 + 1./den2 )
                                             

                                    cuts_combinations = pd.DataFrame({'distance_max':d,
                                                                     'l_long_min':llmin,
                                                                     'l_short_min':lsmax,
                                                                     'delta_phi_min':dpmin,
                                                                     'delta_phi_max':dpmax,
                                                                     'PIDA_short_min':PIDA_smin,
                                                                     'PIDA_short_max':PIDA_smax,
                                                                     'PIDA_long_min':PIDA_lmin,
                                                                     'PIDA_long_max':PIDA_lmax,
                                                                     'efficiency':eff,
                                                                     'eff_err':eff_err,
                                                                     'purity':pur,
                                                                     'pur_err':pur_err,
                                                                     'pur*eff':float(pur*eff)
                                                                     }
                                                                     ,index=[0])
                                             
                                    counter += 1
                                    stream_dataframe_to_file( cuts_combinations , cuts_combination_filename )
                                #}
                            #}
                        #}
                    #}
                #}
            #}
        #}
    #}
    print 'wrote %d new combinations to file....'%counter
#}


print 'done. wrote %d new combinations to file....'%counter
