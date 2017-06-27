'''
    loop over true CC1p from GENIE, find ccqe pairs, plug into pandas dataframe
    ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    python mac/loopover_trueCC1p.py  -data=MC_BNB_extBNB --option=GENIEmup -mccv=8 -evf=1 -p1000
    
    
    --------------------------------------------------------------------------------------------------------------------------------------------
    
    COMMETS:
    
    relevant flags
    
        -o/--option             what to do              (TruthCC1p / CC1pTopology)
        -data                   data-type               (SingleParticleMC / openCOSMIC_MC / MC_BNB BNB_5e19POT / extBNB / MC_BNB_extBNB)
        -evf                    events fraction         (default 0.01)
        -mccv/--MCCversion      MCC-version             (default 7)
        -p                      print every X events    (default 1)
    
    --------------------------------------------------------------------------------------------------------------------------------------------
    
'''
from ccqe_tools import *
do_print_tracks , do_print_vertices = True if debug>2 else False, True if debug>2 else False



if 'GENIE' not in flags.option:
    print 'you have to choose GENIE...'
    exit(0)


# ----- June 19, 2017 ----- #
# find mu-p pairs from GENIE level
if "mup" in flags.option: #{
    
    start_event = flags.run
    print 'start at event',start_event
    MCmode = True
    infilename = tracks_anafile_name( ListName = MCCversion + "_" + flags.DataType  + "_AnalysisTrees" )
    print_filename( infilename , "input events file")
    outcsvname = ccqe_candidates_filename( data_name = flags.option + "_" + MCCversion + "_" + flags.DataType  )
    outfilename = tracks_anafile_name( ListName = MCCversion + "_" + flags.DataType  + "_GENIEmup" )
    
    if os.path.isfile(outcsvname): os.remove(outcsvname)
    print_filename( outcsvname , "output csv of ccqe candidates file")
    
    infile  = ROOT.TFile( infilename )
    inttree = infile.Get("GENIETree")
    inteventstree = infile.Get("eventsTree")
    
    outfile = ROOT.TFile( outfilename ,"recreate")
    outtree = ROOT.TTree("GENIETwoTracksTree","2-tracks clusters")
    
    genie = calcEventTopologies( inttree , outtree, flags.option , debug , MCmode , ccqe_pars['max_mu_p_distance'], inteventstree )
    genie.debug = int(flags.verbose)
    
    Nevents , Nreduced = genie.Nentries , int(flags.evnts_frac*genie.Nentries)
    counter , contained_counter , reco_counter , reco_close_counter = 0 , 0 , 0 , 0
    
    if debug: print_important("running on %d events (out of %d)"%(Nreduced,Nevents))
    
    for i in range(start_event,Nreduced): #{
        
        # get event
        genie.GetGENIEEntry(i)
        
        # analyze the event
        genie.ClusterGENIEToVertices( counter )
        genie.AnalyzeVertices()
        genie.mup_vertices = genie.vertices
        genie.PerformMyTracking()
        
        if i%flags.print_mod==0 or genie.debug>4:
            print "processed %d events, found %d muon-proton, %d reconstructed"%(i,counter,reco_counter)
            if genie.debug: genie.Print( do_print_tracks , do_print_vertices  )
        
        for vertex in genie.CC1p_vertices: #{
            stream_vertex_to_file( vertex , outcsvname , MCmode=MCmode )
            
            # counters
            counter += 1
            if vertex.IsVertexContained: contained_counter += 1
            if vertex.IsVertexReconstructed: reco_counter += 1
            if vertex.IsVertexReconstructed and vertex.reco_mu_p_distance < ccqe_pars['max_mu_p_distance'] : reco_close_counter += 1
            if debug and i%flags.print_mod==0: print_line()
        #}
        genie.FillGENIEOutTree()
    #}

    infile.Close()
    print 'processed %d events'%Nreduced
    print_filename(outcsvname,
                   "%d CC1p GENIE vertices, %d reconstructed, %d closer than %.0f cm (%.1f MB)"
                   %(counter,reco_counter,reco_close_counter,ccqe_pars['max_mu_p_distance'],filesize_in_MB(outcsvname)) )
                   outtree.Write()
                   outfile.Close()

#}





if "CC1p" in flags.option: #{

    start_event = flags.run
    print 'start at event',start_event
    MCmode = True
    infilename = tracks_anafile_name( ListName = MCCversion + "_" + flags.DataType  + "_AnalysisTrees" )
    print_filename( infilename , "input events file")
    outcsvname = ccqe_candidates_filename( data_name = flags.option + "_" + MCCversion + "_" + flags.DataType  )
    outfilename = tracks_anafile_name( ListName = MCCversion + "_" + flags.DataType  + "_GENIE2TracksClusters" )

    if os.path.isfile(outcsvname): os.remove(outcsvname)
    print_filename( outcsvname , "output csv of ccqe candidates file")

    infile  = ROOT.TFile( infilename )
    inttree = infile.Get("GENIETree")
    inteventstree = infile.Get("eventsTree")
    
    outfile = ROOT.TFile( outfilename ,"recreate")
    outtree = ROOT.TTree("GENIETwoTracksTree","2-tracks clusters")

    genie = calcEventTopologies( inttree , outtree, flags.option , debug , MCmode , ccqe_pars['max_mu_p_distance'], inteventstree )
    genie.debug = int(flags.verbose)
    
    Nevents , Nreduced = genie.Nentries , int(flags.evnts_frac*genie.Nentries)
    counter , contained_counter , reco_counter , reco_close_counter = 0 , 0 , 0 , 0

    if debug: print_important("running on %d events (out of %d)"%(Nreduced,Nevents))

    for i in range(start_event,Nreduced): #{
        
        # get event
        genie.GetGENIEEntry(i)
        genie.debug = int(flags.verbose) if genie.genie_interactions.size()>0 and genie.genie_interactions[0].event == 138443 else 0

        # analyze the event
        genie.ClusterGENIEToVertices( counter )
        genie.AnalyzeVertices()
        genie.CC1p_vertices = genie.vertices
        genie.PerformMyTracking( )

        if i%flags.print_mod==0 or genie.debug>4:
            print "processed %d events, found %d CC1p, %d reconstructed"%(i,counter,reco_counter)
            if genie.debug: genie.Print( do_print_tracks , do_print_vertices  )


        for vertex in genie.CC1p_vertices: #{
            stream_vertex_to_file( vertex , outcsvname , MCmode=MCmode )
            
            # counters
            counter += 1
            if vertex.IsVertexContained: contained_counter += 1
            if vertex.IsVertexReconstructed: reco_counter += 1
            if vertex.IsVertexReconstructed and vertex.reco_mu_p_distance < ccqe_pars['max_mu_p_distance'] : reco_close_counter += 1
            if debug and i%flags.print_mod==0: print_line()
        #}
        genie.FillGENIEOutTree()
    #}


    infile.Close()
    print 'processed %d events'%Nreduced

    print_filename(outcsvname,
                   "%d CC1p GENIE vertices, %d reconstructed, %d closer than %.0f cm (%.1f MB)"
                   %(counter,reco_counter,reco_close_counter,ccqe_pars['max_mu_p_distance'],filesize_in_MB(outcsvname)) )
    outtree.Write()
    outfile.Close()

#}


print 'loopover_events.py done.'