from anatrees_tools import *
'''
    usage:
    ------
    python mac/ana_pandoranu_tracks.py --option=hits --DataType=MC_BNB
    
    options: 
        hits        {"write hits features to csv file"}
        dEdx        {"dEdx profile for tracks"}
'''



TracksListName = flags.DataType # MC_BNB , BNB_5e19POT


features_pandoranu_track_hits = ['run','subrun','event',
                                 'track_id','plane',
                                 'hit','track_length','dEdx',
                                 'MCpdgCode',
                                 'mscore_p','mscore_max']

hits_file_name      = anafiles_path + "/hits_data.csv"
dEdx_figure_name    = anafiles_path + "/plots/dEdx.pdf"

# analyze tracks
# -------------------------------------------------------------------
if flags.option=="write hits features to csv file" or 'hits' in flags.option:
    tracks_counter , hits_counter = 0 , 0
    hits_df = pd.DataFrame(columns=features_pandoranu_track_hits)
    if flags.verbose: print "starting to AnalyzeTracksFile from file ",TracksListName
    ana_pandoranu = AnalyzeTracksFile( anafiles_path , TracksListName , flags.verbose )
    
    # For Data filter out events that we are interested in.
    # for example, consider only classified proton tracks
    Nentries = ana_pandoranu.GetEntries()
    Nreduced = int(flags.evnts_frac*Nentries)
    if flags.verbose: print "processing %d tracks"%Nreduced

    for i in range(Nreduced):
        
        if flags.verbose and (i%(Nreduced/100)==0): print "[ %.0f"%(100*float(i)/Nreduced),"%]"
        tracks = ana_pandoranu.GetEventTracks(i)
        
#        for track in tracks:
#            
#            tracks_counter = tracks_counter + 1
#            track_length_Y = track.GetTrackLengthVector(2)
#            track_dEdx_Y = track.GetTrack_dEdxVector(2)
#            
#            for hit in range(track_dEdx_Y.size()):
#                
#                hits_counter = hits_counter + 1
#                hit_df = pd.DataFrame({'run':track.run,'subrun':track.subrun,'event':track.event,
#                                      'track_id':track.track_id,
#                                      'plane':2,
#                                      'hit':hit,
#                                      'track_length':track_length_Y.at(hit),
#                                      'dEdx':track_dEdx_Y.at(hit),
#                                      'MCpdgCode':track.MCpdgCode }
#                                      , index = [hits_counter])
#                hits_df = hits_df.append( hit_df )
#    
#        stream_dataframe_to_file( hits_df , hits_file_name )

#    hits_df.to_csv( hits_file_name )
#    print_filename( hits_file_name ,"%d tracks, %d Y-plane hits"%(Nreduced,hits_counter) )



# -------------------------------------------------------------------
if flags.option=="dEdx profile for tracks" or 'dEdx' in flags.option:
    
    print_important( "ipynb notebooks/plot_dEdx_profile.ipynb")
    
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











