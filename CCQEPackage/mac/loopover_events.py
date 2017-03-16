'''
    loop over events, find ccqe topologies, plug into pandas dataframe
    ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    > python mac/loopover_events.py  --option=CC1pTopology -data=ExampleDebugging -evf=1 -p1
    
    
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
                                                            
        -evf                    events fraction         (default 0.01)
        -mccv/--MCCversion      MCC-version             (default 7)
        -p                      print every X events    (default 1)
    
    --------------------------------------------------------------------------------------------------------------------------------------------
    
'''
from ccqe_tools import *


do_print_tracks , do_print_vertices = True if debug>2 else False, True if debug>4 else False






if 'Truth' not in flags.option and 'Topology' not in flags.option:
    print 'you have to choose Truth / Topology...'
    exit(0)


if "CC1p" in flags.option: #{

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

    events  = calcEventTopologies( inttree , outtree, flags.option , (int)(flags.verbose) , MCmode )
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
    counter , CC1mu1p_counter , GENIECC1p_counter = 0 , 0 , 0

    if debug: print_important("running on %d events (out of %d)"%(Nreduced,Nevents))

    for i in range(Nreduced): #{
        
        event_has_CC1p_topology = False
        
        # get event
        events.GetEntry(i)
        
        # verbosity to a specific event
        events.debug=flags.verbose if events.event == 36674 else 0
        


        # analyze the event
        events.extract_information()
        
        # find vertices with CC1p topologies   # event_has_CC1p_topology = events.FindVerticesWithCC1pTopology()
        event_has_CC1p_topology = events.Find2tracksVertices()
        events.TagGENIECC1p()
        events.PerformMyTracking()
        
        if i%flags.print_mod==0:
            print "processed %d events,found %d 2-tracks clusters, %d 1mu1p, %d true GENIE CC1p"%(i,counter,CC1mu1p_counter,GENIECC1p_counter)
            if debug: events.Print( do_print_tracks , do_print_vertices )
        
        if event_has_CC1p_topology is not True: continue
    
        for vertex in events.CC1p_vertices: #{
            
            # perform my-tracking to look for fraction of charge deposited in vertex to track-associated charge
#            hits = events.hits
#            tracks = [vertex.AssignedMuonTrack , vertex.AssignedProtonTrack]
#            my_tracks_dict , hits_charge_dict = get_my_tracks_in_roi( hits=hits, tracks=tracks , debug=0 )
#            attach_hits_my_tracks_to_vertex( hits_charge_dict=hits_charge_dict , my_tracks_dict=my_tracks_dict , vertex=vertex )

#            if vertex.Is1mu1pDetected and vertex.genie_interaction.protons.size():
#                print 'mcevent_id:',vertex.genie_interaction.mcevent_id
#                print vertex.genie_interaction.Np,'protons in GENIE interaction'
#                print vertex.genie_interaction.protons.size()
#                print vertex.genie_interaction.protons.at(0).P()

            if vertex.GENIECC1p: vertex.SetCounterID( GENIECC1p_counter )
            
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