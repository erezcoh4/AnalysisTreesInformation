'''
    loop over events, find ccqe topologies, plug into pandas dataframe
    ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    > python mac/loopover_events.py  -data=MC_BNB --option=CC1pTopology -evf=0.001 -p100
    
    
    --------------------------------------------------------------------------------------------------------------------------------------------
    
    COMMETS:
    
    relevant flags
    
        -o/--option             what to do              (truthCC1p / CC1pTopology)
        -data                   data-type               (SingleParticleMC / openCOSMIC_MC / MC_BNB BNB_5e19POT / extBNB)
        -evf                    events fraction         (default 0.01)
        -mccv/--MCCversion      MCC-version             (default 7)
        -p                      print every X events    (default 1)
    
    --------------------------------------------------------------------------------------------------------------------------------------------
    
'''
import sys; sys.path.insert(0, '../AnaTreesPackage/mac')
from anatrees_tools import *
from calc_tools import *
from ROOT import calcEventTopologies





# global constants for
ccqe_pars = dict({
                 # for MC, take soft selection cuts here to study the purity and effficiency as a function of the cuts
                 'max_mu_p_distance':15.0   # [cm]      # nominal 2 cm
                 ,'min_length_long':0.0     # [cm]      # nominal 10 cm
                 ,'max_length_short':200    # [cm]      # nominal 10 cm
                 ,'delta_phi_min':0.0       # [deg.]    # nominal 150 deg.
                 ,'delta_phi_max':360.0     # [deg.]
                 ,'PIDA_short_min':-1                   # nominal 16
                 ,'PIDA_short_max':1e13                 # nominal 24
                 ,'PIDA_long_min':-1                    # nominal 5
                 ,'PIDA_long_max':1e13                  # nominal 13
                 })







if "CC1p" in flags.option: #{

    MCmode = True if 'MC' in flags.DataType else False
    infilename = tracks_anafile_name( ListName = MCCversion + "_" + flags.DataType  + "_AnalysisTrees" )
    print_filename( infilename , "input events file")
    outcsvname = ccqe_candidates_filename( data_name = flags.option + "_" + MCCversion + "_" + flags.DataType  )
    if os.path.isfile(outcsvname): os.remove(outcsvname)
    print_filename( outcsvname , "output csv of ccqe candidates file")


    infile  = ROOT.TFile( infilename )
    inttree = infile.Get("eventsTree")

    events  = calcEventTopologies( inttree , flags.verbose , MCmode )
    events.SetMaxmupDistance (ccqe_pars['max_mu_p_distance'] )
    events.SetMinLengthLong (ccqe_pars['min_length_long'] )
    events.SetMaxLengthShort (ccqe_pars['max_length_short'] )
    events.SetDetaPhiMin (ccqe_pars['delta_phi_min'] )
    events.SetDetaPhiMax (ccqe_pars['delta_phi_max'] )
    events.SetPIDAShortMin (ccqe_pars['PIDA_short_min'])
    events.SetPIDAShortMax (ccqe_pars['PIDA_short_max'])
    events.SetPIDALongMin (ccqe_pars['PIDA_long_min'])
    events.SetPIDALongMax (ccqe_pars['PIDA_long_max'])
    

    events.SetOption ( flags.option )


    Nevents , Nreduced = events.Nentries , int(flags.evnts_frac*events.Nentries)
    ccqe , counter = pd.DataFrame() , 0

    if debug: print_important("running on %d events (out of %d)"%(Nreduced,Nevents))

    for i in range(Nreduced): #{

        event_has_CC1p_topology=False
        
        # get event
        events.GetEntry(i)
        
        # analyze the event
        events.extract_information()
        if debug and i%flags.print_mod==0: events.Print()
        
        # find vertices with CC1p topologies
        if 'truth' in flags.option or 'Truth' in flags.option:
            event_has_CC1p_topology = events.FindTruthCC1pVertices()
        elif 'topology' in flags.option or 'Topology' in flags.option:
            event_has_CC1p_topology = events.FindVerticesWithCC1pTopology()
    
        if event_has_CC1p_topology is not True: continue
    
        # get the vertices with CC1p topologies and stream them into pandas.DataFrame
        CC1p_vertices = [v for v  in events.CC1p_vertices]

        for vertex in events.CC1p_vertices: #{
            ccqe = pd.DataFrame({'run':vertex.run
                                ,'subrun':vertex.subrun
                                ,'event':vertex.event
                                ,'Ntracks':vertex.Ntracks
                                ,'track_id':[list(track.track_id for track in vertex.tracks)]
                                ,'track_length':[set_float_precision(np.array(list(track.length for track in vertex.tracks)),1)]
                                ,'track_phi':[set_float_precision(np.array(list((180./np.pi)*track.phi for track in vertex.tracks)),1)]
                                ,'delta_phi':vertex.delta_phi_ij
                                ,'delta_theta':vertex.delta_theta_ij
                                # for more than two tracks [set_float_precision(np.array(list(phi for phi in vertex.delta_phi_ij)),1)]
                                ,'distance':vertex.distances_ij
                                # for more than two tracks [set_float_precision(np.array(list(distance for distance in vertex.distances_ij)),1)]
                                ,'l_short':vertex.ShortestTrack.length
                                ,'l_long':vertex.LongestTrack.length
                                ,'PIDA_short':vertex.ShortestTrack.pidpida
                                ,'PIDA_long':vertex.LongestTrack.pidpida
                                ,'track_startx':[set_float_precision(np.array(list(track.startx for track in vertex.tracks)),1)]
                                ,'track_starty':[set_float_precision(np.array(list(track.starty for track in vertex.tracks)),1)]
                                ,'track_startz':[set_float_precision(np.array(list(track.startz for track in vertex.tracks)),1)]
                                ,'track_endx':[set_float_precision(np.array(list(track.endx for track in vertex.tracks)),1)]
                                ,'track_endy':[set_float_precision(np.array(list(track.endy for track in vertex.tracks)),1)]
                                ,'track_endz':[set_float_precision(np.array(list(track.endz for track in vertex.tracks)),1)]
                                
                                # features that are only relevant for truth information
                                ,'pdg':[list(track.MCpdgCode for track in vertex.tracks)]
                                ,'track_truth_ccnc':[list(track.truth_ccnc for track in vertex.tracks)]
                                ,'l_proton':vertex.ShortestTrack.length
                                ,'l_muon':vertex.LongestTrack.length
                                ,'PIDA_proton':vertex.ShortestTrack.pidpida
                                ,'PIDA_muon':vertex.LongestTrack.pidpida
                                ,'truth_cc1p':vertex.TruthCC1p
                                },
                                index=[0])

            stream_dataframe_to_file( ccqe, outcsvname  )
            counter += 1
            if debug and i%flags.print_mod==0: print_line()
        #}
    #}


    infile.Close()
    print_filename( outcsvname , "wrote %d CC1p vertices features (%.1f MB)"%(counter,filesize_in_MB(outcsvname) ) )
#}


print 'loopover_events.py done.'