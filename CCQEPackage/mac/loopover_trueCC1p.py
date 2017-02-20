'''
    loop over true CC1p from GENIE, find ccqe pairs, plug into pandas dataframe
    ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    > python mac/loopover_trueCC1p.py  -data=MC_BNB --option=GENIECC1p -evf=0.001 -p100
    
    
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





if 'GENIE' not in flags.option:
    print 'you have to choose Truth...'
    exit(0)


if "CC1p" in flags.option: #{

    MCmode = True
    infilename = tracks_anafile_name( ListName = MCCversion + "_" + flags.DataType  + "_AnalysisTrees" )
    print_filename( infilename , "input events file")
    outcsvname = ccqe_candidates_filename( data_name = flags.option + "_" + MCCversion + "_" + flags.DataType  )
    if os.path.isfile(outcsvname): os.remove(outcsvname)
    print_filename( outcsvname , "output csv of ccqe candidates file")

    infile  = ROOT.TFile( infilename )
    inttree = infile.Get("GENIETree")

    genie  = calcEventTopologies( inttree , flags.option , flags.verbose , MCmode )
    Nevents , Nreduced = genie.Nentries , int(flags.evnts_frac*genie.Nentries)
    counter , contained_counter , reco_counter = 0 , 0 , 0

    if debug: print_important("running on %d events (out of %d)"%(Nreduced,Nevents))

    for i in range(Nreduced): #{
        
        # get event
        genie.GetGENIEEntry(i)

        # analyze the event
        genie.ClusterGENIEToVertices()
        genie.AnalyzeVertices()

        # find vertices with CC1p topologies
        event_has_CC1p_topology = genie.FindGENIECC1pVertices()


        if debug and i%flags.print_mod==0:
            print "processed so far %d events, found %d CC1p GENIE vertices, %d reconstructed "%(i,counter,reco_counter)
            genie.Print()



        # get the vertices with CC1p topologies and stream them into pandas.DataFrame
        CC1p_vertices = [v for v  in genie.CC1p_vertices]

        for vertex in genie.CC1p_vertices: #{
            
            stream_vertex_to_file( vertex , outcsvname )
            
            # counters
            counter += 1
            if vertex.IsVertexContained: contained_counter += 1
            if vertex.IsVertexReconstructed: reco_counter += 1
            if debug and i%flags.print_mod==0: print_line()
        #}
    #}


    infile.Close()
    print 'processed %d events'%Nreduced
    print_filename(outcsvname,"%d CC1p GENIE vertices, %d reconstructed (%.1f MB)"%(counter,reco_counter,filesize_in_MB(outcsvname)) )
#}


print 'loopover_events.py done.'