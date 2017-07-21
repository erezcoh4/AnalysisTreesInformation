'''
    loop over events, find ccqe topologies, plug into pandas dataframe
    ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    python mac/loopover_events.py  --option=Topology -mccv=8 -data=ExampleDebugging -evf=1 -p1
    python mac/loopover_events.py  --option=Topology -mccv=8 -data=MC_BNB_extBNB -evf=0.01 -p1000
    python mac/loopover_events.py  --option=CC1pTopology -mccv=8 -data=MC_BNB_extBNB -evf=1 -p1000
    python mac/loopover_events.py  --option=CC1pTopology -mccv=8 -data=BNB -evf=0.01 -p1000
    
    
    --------------------------------------------------------------------------------------------------------------------------------------------
    
    COMMETS:
    
    relevant flags
    
        -o/--option             what to do              (TruthCC1p / CC1pTopology)
        
        -data                   data-type               SingleParticleMC
                                                        openCOSMIC_MC
                                                        MC_BNB
                                                        BNB_5e19POT
                                                        extBNB
                                                        MC_BNB_InTimeCOSMIC_MC
                                                        MC_BNB_extBNB
                                                            
        -evf                    events fraction         (default 0.01)
        -mccv/--MCCversion      MCC-version             (default 7)
        -p                      print every X events    (default 1)
    
    --------------------------------------------------------------------------------------------------------------------------------------------
    
'''
from ccqe_tools import *


hyperparameters = dict( {
                       'Perform My-Tracking':True
                       ,'print genie interactions':False#True if debug>1 else False
                       ,'print tracks':False #True if debug>1 else False
                       ,'print vertices':True if debug>1 else False
                       } )


counters = dict(); fractions = dict()
for pair_type in pair_types:  #{
    counters[pair_type] = fractions[pair_type] = 0.0
#}

# June-26, 2017: moving to a different data sample definition CC events with 1p > 200MeVc and no pions
if "Topology" in flags.option or "topology" in flags.option: #{
    
    start_event = flags.run
    MCmode = True if 'MC' in flags.DataType or 'ExampleDebugging' in flags.DataType else False
    infilename = tracks_anafile_name( ListName = MCCversion + "_" + flags.DataType  + "_AnalysisTrees" )
    outfilename = tracks_anafile_name( ListName = MCCversion + "_" + flags.DataType  + "_Pairs" )
    print_filename( infilename , "input events file")
    outcsvname = ccqe_candidates_filename( data_name = flags.option + "_" + MCCversion + "_" + flags.DataType  )
    if os.path.isfile(outcsvname): os.remove(outcsvname)
    print_filename( outcsvname , "output csv of track-pairs")
    
    infile  = ROOT.TFile( infilename )
    inttree = infile.Get("eventsTree")
    
    outfile = ROOT.TFile( outfilename ,"recreate")
    outtree = ROOT.TTree("pairsTree","track pairs")
    
    events  = calcEventTopologies( inttree , outtree, flags.option , debug , MCmode )
    events.SetMaxmupDistance (ccqe_pars['max_mu_p_distance'] )
    events.SetOption ( flags.option )
    
    Nevents , Nreduced = events.Nentries , int(flags.evnts_frac*events.Nentries)
    if debug: print_important("running on %d events (out of %d)"%(Nreduced,Nevents))
    events.debug = debug # if events.event==147603 else 0 # verbosity to a specific event
    
    for i in range(start_event,Nreduced): #{
        event_has_CCQE_topology = False
        events.GetEntry(i)
        
        # cluster tracks into groups
        events.extract_information()
        # find events with good topology: pairs of tracks reconstructed at close proximity
        pair_in_event = events.Find2tracksVertices()
        if pair_in_event is not True: #{
            if events.debug and i==0 or i%flags.print_mod==0:#{
                if events.debug>1: print 'no detected pairs in this event!'
                events.Print( hyperparameters['print genie interactions']
                             ,hyperparameters['print tracks']
                             ,hyperparameters['print vertices'] )
            #}
            continue
        #}
        # tag vertices by their topology: mu-p / other pairs / cosmic / CC 1p 0pi
        events.TagVertices()
        # match hits to pandoraNu tracks
        if hyperparameters['Perform My-Tracking'] is True: events.PerformMyTracking()
        # stream to output file and fill out tree
        for vertex in events.vertices: #{
            stream_vertex_to_file( vertex , outcsvname , MCmode=MCmode )
            count_vertex( vertex , counters , fractions )
        #}
        events.FillOutTree()
        if i==0 or i%flags.print_mod==0:#{
            if events.debug: events.Print( hyperparameters['print genie interactions']
                                          ,hyperparameters['print tracks']
                                          ,hyperparameters['print vertices'] )
            text  = "processed %d events, found %d pairs\n"%(i+1,counters['all'])
            for pair_type in pair_types[1:4]:  text += "%s: %.1f"%(pair_type,fractions[pair_type])+"% ,"
            for pair_type in pair_types[4:]:   text += "(%s: %.1f"%(pair_type,fractions[pair_type])+"%)"
            print text
            print_line()
        #}
    #}
    infile.Close()
    print 'finished processing %d events'%(i+1)
    print_filename( outcsvname, "csv file of %d pairs-vertices (%.1f MB)" %(counters['all'],filesize_in_MB(outcsvname)) )
    print_filename( outfilename, "root file (%.2f MB)"%(filesize_in_MB(outfilename)) )
    outtree.Write()
    outfile.Close()
#}




if "CC1p" in flags.option: #{

    start_event = flags.run
    print 'start at event',start_event
    MCmode = True if 'MC' in flags.DataType else False
    infilename = tracks_anafile_name( ListName = MCCversion + "_" + flags.DataType  + "_AnalysisTrees" )
    outfilename = tracks_anafile_name( ListName = MCCversion + "_" + flags.DataType  + "_2TracksClusters" )
    print_filename( infilename , "input events file")
    outcsvname = ccqe_candidates_filename( data_name = flags.option + "_" + MCCversion + "_" + flags.DataType  )
    if os.path.isfile(outcsvname): os.remove(outcsvname)
    print_filename( outcsvname , "output csv of ccqe candidates file")


    infile  = ROOT.TFile( infilename )
    inttree = infile.Get("eventsTree")

    outfile = ROOT.TFile( outfilename ,"recreate")
    outtree = ROOT.TTree("TwoTracksTree","2-tracks clusters")

    events  = calcEventTopologies( inttree , outtree, flags.option , debug , MCmode )
    events.SetMaxmupDistance (ccqe_pars['max_mu_p_distance'] )
    events.debug = debug
    #    events.SetMinLengthLong (ccqe_pars['min_length_long'] )
    #    events.SetMaxLengthShort (ccqe_pars['max_length_short'] )
    #    events.SetDetaPhiMin (ccqe_pars['delta_phi_min'] )
    #    events.SetDetaPhiMax (ccqe_pars['delta_phi_max'] )
    #    events.SetPIDAShortMin (ccqe_pars['PIDA_short_min'])
    #    events.SetPIDAShortMax (ccqe_pars['PIDA_short_max'])
    #    events.SetPIDALongMin (ccqe_pars['PIDA_long_min'])
    #    events.SetPIDALongMax (ccqe_pars['PIDA_long_max'])
    events.SetOption ( flags.option )


    Nevents , Nreduced = events.Nentries , int(flags.evnts_frac*events.Nentries)
    counter , CC1mu1p_counter , GENIECC1p_counter = 0 , 0 , 0

    if debug: print_important("running on %d events (out of %d)"%(Nreduced,Nevents))

    for i in range(start_event,Nreduced): #{
        
        event_has_CC1p_topology = False
        
        # get event
        events.GetEntry(i)
        
        # verbosity to a specific event
        events.debug=debug if events.event == 138443 else 0
        
        # analyze the event
        events.extract_information()
        
        # find vertices with CC1p topologies   # event_has_CC1p_topology = events.FindVerticesWithCC1pTopology()
        event_has_CC1p_topology = events.Find2tracksVertices()
        events.TagGENIECC1p()
        events.PerformMyTracking()
        
        if i%flags.print_mod==0:
            print "processed %d events,found %d 2-tracks clusters, %d 1mu1p, %d true GENIE CC1p"%(i,counter,CC1mu1p_counter,GENIECC1p_counter)
            if events.debug: events.Print( do_print_tracks , do_print_vertices )
        
        if event_has_CC1p_topology is not True: continue
    
        for vertex in events.CC1p_vertices: #{
            
            stream_vertex_to_file( vertex , outcsvname , MCmode=MCmode )
            
            counter += 1
            if vertex.Is1mu1pDetected:  CC1mu1p_counter += 1
            if vertex.GENIECC1p:        GENIECC1p_counter += 1
            if debug and i%flags.print_mod==0: print_line()
        #}
        
        events.FillOutTree()
    #}


    infile.Close()

    print 'processed %d events'%Nreduced
    print_filename( outcsvname,
                   "%d 2-tracks vertices,  %d 1mu1p, %d true GENIE CC1p (%.1f MB)"
                   %(counter,CC1mu1p_counter,GENIECC1p_counter,filesize_in_MB(outcsvname)) )

    print_filename( outfilename, "root file of %d 2-tracks clusters (%.2f MB)"%(counter,filesize_in_MB(outfilename)) )
    outtree.Write()
    outfile.Close()

#}



print 'loopover_events.py done.'