from anatrees_tools import *
'''
    usage:
    ------
    python mac/ana_pandoranu_tracks.py --option=hits
    options: 
        hits        {"write hits features to csv file"}
        dEdx        {"dEdx profile for tracks"}
'''

features_pandoranu_track_hits = ['run','subrun','event',
                                 'track_id','plane',
                                 'hit','track_length','dEdx',
                                 'MCpdgCode','pscore']
hits_file_name      = anafiles_path + "/hits_data.csv"
dEdx_figure_name    = anafiles_path + "/plots/dEdx.pdf"

# analyze tracks
# -------------------------------------------------------------------
if flags.option=="write hits features to csv file" or 'hits' in flags.option:
    tracks_counter , hits_counter = 0 , 0
    hits_df = pd.DataFrame(columns=features_pandoranu_track_hits)
    ana_pandoranu = AnalyzeTracksFile( anafiles_path , "BNB_5e19POT" , flags.verbose )
    
    # ToDo: filter out events that we are interested in.
    # for example, consider only classified proton tracks
    Nentries = ana_pandoranu.GetEntries()
    for i in range(Nentries):
        
        if flags.verbose and i%(Nentries/5)==0: print "[ %.0f"%(100*float(i)/Nentries),"%]"
        tracks = ana_pandoranu.GetEventTracks(i)
        
        for track in tracks:
            
            tracks_counter = tracks_counter + 1
            
            for plane in range(3):
                
                track_length = track.GetTrackLengthVector( plane )
                track_dEdx = track.GetTrack_dEdxVector( plane )
                
                for hit in range(track_dEdx.size()):
                    
                    hits_counter = hits_counter + 1
                    
                    hit_df = pd.DataFrame({'run':track.run,'subrun':track.subrun,'event':track.event,
                                          'track_id':track.track_id,
                                          'plane':plane,
                                          'hit':hit,
                                          'track_length':track_length.at(hit),
                                          'dEdx':track_dEdx.at(hit)}
                                          , index = [hits_counter])
                    hits_df = hits_df.append( hit_df )

    hits_df.to_csv( hits_file_name )
    print_filename( hits_file_name , "wrote hits file for %d tracks, %d hits (3 planes)"%(ana_pandoranu.GetEntries(),hits_counter) )



# -------------------------------------------------------------------
if flags.option=="dEdx profile for tracks" or 'dEdx' in flags.option:
    
    hits = pd.read_csv(hits_file_name, dtype={'track_length':np.float32,'dEdx':np.float32})
    H = np.histogram2d( hits.track_length , hits.dEdx )
    im = plt.imshow(H,interpolation='none', cmap='hot_r',origin='lower')#, extent=extent)
    im = ax.imshow(np.ma.masked_where(H == 0, H), cmap='hot_r', interpolation='none', origin='lower' )#, extent=extent)
    ax.set_xlabel('track length [cm]',fontsize=25)
    ax.set_ylabel('$dE/dx$ [MeV/cm]',fontsize=25)
    for tick in ax.xaxis.get_major_ticks():
        tick.label.set_fontsize(25)
    for tick in ax.yaxis.get_major_ticks():
        tick.label.set_fontsize(25)
    ax.set_aspect('auto')
    cb = plt.colorbar()
    for t in cb.ax.get_yticklabels():
        t.set_fontsize(20)
    fig.show()
    plt.savefig( dEdx_figure_name )
    print_filename( hits_file_name , "generated dE/dx profile" )











