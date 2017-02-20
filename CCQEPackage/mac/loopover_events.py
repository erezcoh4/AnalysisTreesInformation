'''
    loop over events, find ccqe topologies, plug into pandas dataframe
    ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    > python mac/loopover_events.py  -data=MC_BNB --option=CC1pTopology -evf=0.001 -p100
    
    
    --------------------------------------------------------------------------------------------------------------------------------------------
    
    COMMETS:
    
    relevant flags
    
        -o/--option             what to do              (TruthCC1p / CC1pTopology)
        -data                   data-type               (SingleParticleMC / openCOSMIC_MC / MC_BNB BNB_5e19POT / extBNB)
        -evf                    events fraction         (default 0.01)
        -mccv/--MCCversion      MCC-version             (default 7)
        -p                      print every X events    (default 1)
    
    --------------------------------------------------------------------------------------------------------------------------------------------
    
'''
from ccqe_tools import *




# global constants for
ccqe_pars = dict({
                 # for MC, take soft selection cuts here to study the purity and effficiency as a function of the cuts
                 'max_mu_p_distance':10.0   # [cm]      # nominal 2 cm
                 ,'min_length_long':0.0     # [cm]      # nominal 10 cm
                 ,'max_length_short':200    # [cm]      # nominal 10 cm
                 ,'delta_phi_min':0.0       # [deg.]    # nominal 150 deg.
                 ,'delta_phi_max':361.0     # [deg.]
                 ,'PIDA_short_min':-1                   # nominal 16
                 ,'PIDA_short_max':1e13                 # nominal 24
                 ,'PIDA_long_min':-1                    # nominal 5
                 ,'PIDA_long_max':1e13                  # nominal 13
                 })





if 'Truth' not in flags.option and 'Topology' not in flags.option:
    print 'you have to choose Truth / Topology...'
    exit(0)


if "CC1p" in flags.option: #{

    MCmode = True if 'MC' in flags.DataType else False
    infilename = tracks_anafile_name( ListName = MCCversion + "_" + flags.DataType  + "_AnalysisTrees" )
    print_filename( infilename , "input events file")
    outcsvname = ccqe_candidates_filename( data_name = flags.option + "_" + MCCversion + "_" + flags.DataType  )
    if os.path.isfile(outcsvname): os.remove(outcsvname)
    print_filename( outcsvname , "output csv of ccqe candidates file")


    infile  = ROOT.TFile( infilename )
    inttree = infile.Get("eventsTree")

    events  = calcEventTopologies( inttree , flags.option , flags.verbose , MCmode )
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
    counter , GENIECC1p_counter = 0 , 0

    if debug: print_important("running on %d events (out of %d)"%(Nreduced,Nevents))

    for i in range(Nreduced): #{

        event_has_CC1p_topology=False
        
        # get event
        events.GetEntry(i)
        
        # analyze the event
        events.extract_information()
        
        # find vertices with CC1p topologies
        # event_has_CC1p_topology = events.FindVerticesWithCC1pTopology()
        event_has_CC1p_topology = events.Find2tracksVertices()
        events.TagGENIECC1p()
        

        if debug and i%flags.print_mod==0:
            print "processed so far %d events, found %d CC-2-tracks vertices, %d true GENIE CC1p"%(i,counter,GENIECC1p_counter)
            events.Print(True,True)
        
        if event_has_CC1p_topology is not True: continue
    
        # get the vertices with CC1p topologies and stream them into pandas.DataFrame
        CC1p_vertices = [v for v  in events.CC1p_vertices]

        for vertex in events.CC1p_vertices: #{
            stream_vertex_to_file( vertex , outcsvname )
            counter += 1
            if vertex.GENIECC1p: GENIECC1p_counter += 1
            if debug and i%flags.print_mod==0: print_line()
        #}
    #}


    infile.Close()

    print 'processed %d events'%Nreduced
    print_filename( outcsvname,"%d CC1p vertices/%d true GENIE CC1p (%.1f MB)"%(counter,GENIECC1p_counter,filesize_in_MB(outcsvname) ) )
#}


print 'loopover_events.py done.'