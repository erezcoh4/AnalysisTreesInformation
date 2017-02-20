
import sys; sys.path.insert(0, '../AnaTreesPackage/mac')
from anatrees_tools import *
from calc_tools import *
from ROOT import calcEventTopologies


def stream_vertex_to_file( vertex=None , outcsvname='' ):
    
    data = pd.DataFrame({
                        # event features
                        'run':vertex.run
                        ,'subrun':vertex.subrun
                        ,'event':vertex.event
                        
                        # 2-tracks vertex features
                        ,'delta_phi':vertex.delta_phi_ij
                        ,'delta_theta':vertex.delta_theta_ij
                        ,'distance':vertex.distances_ij
                        
                        # single tracks features
                        # sort by length
                        ,'l_short':vertex.ShortestTrack.length
                        ,'l_long':vertex.LongestTrack.length
                        ,'cfdistance_short':vertex.ShortestTrack.cfdistance
                        ,'cfdistance_long':vertex.LongestTrack.cfdistance
                        ,'PIDA_short':vertex.ShortestTrack.pidpida
                        ,'PIDA_long':vertex.LongestTrack.pidpida
                        ,'starty_short':vertex.ShortestTrack.starty
                        ,'starty_long':vertex.LongestTrack.starty
                        ,'cosmicscore_short':vertex.ShortestTrack.cosmicscore
                        ,'cosmicscore_long':vertex.LongestTrack.cosmicscore
                        ,'coscontscore_short':vertex.ShortestTrack.coscontscore
                        ,'coscontscore_long':vertex.LongestTrack.coscontscore
                        ,'cfdistance_short':vertex.ShortestTrack.cfdistance
                        ,'cfdistance_long':vertex.LongestTrack.cfdistance
                        # sort by pida
                        ,'l_large_pida':vertex.LargePIDATrack.length
                        ,'l_small_pida':vertex.SmallPIDATrack.length
                        ,'cfdistance_large_pida':vertex.LargePIDATrack.cfdistance
                        ,'cfdistance_small_pida':vertex.SmallPIDATrack.cfdistance
                        ,'PIDA_large_pida':vertex.LargePIDATrack.pidpida
                        ,'PIDA_small_pida':vertex.SmallPIDATrack.pidpida
                        ,'starty_large_pida':vertex.LargePIDATrack.starty
                        ,'starty_small_pida':vertex.SmallPIDATrack.starty
                        ,'cosmicscore_large_pida':vertex.LargePIDATrack.cosmicscore
                        ,'cosmicscore_small_pida':vertex.SmallPIDATrack.cosmicscore
                        ,'coscontscore_large_pida':vertex.LargePIDATrack.coscontscore
                        ,'coscontscore_small_pida':vertex.SmallPIDATrack.coscontscore
                        ,'cfdistance_large_pida':vertex.LargePIDATrack.cfdistance
                        ,'cfdistance_small_pida':vertex.SmallPIDATrack.cfdistance

                        
                        
                        
                        
                        
                        # features that are only relevant for truth information
                        ,'pdg':[list(track.MCpdgCode for track in vertex.tracks)]
                        ,'truth_P_proton':vertex.protonTruth.truth_P
                        ,'truth_P_muon':vertex.muonTruth.truth_P
                        ,'l_proton':vertex.protonTruth.length
                        ,'l_muon':vertex.muonTruth.length
                        ,'starty_proton':vertex.protonTruth.starty
                        ,'starty_muon':vertex.muonTruth.starty
                        
                        ,'cosmicscore_proton':vertex.protonTruth.cosmicscore
                        ,'cosmicscore_muon':vertex.muonTruth.cosmicscore
                        ,'coscontscore_proton':vertex.protonTruth.coscontscore
                        ,'coscontscore_muon':vertex.muonTruth.coscontscore
                        
                        ,'cfdistance_proton':vertex.protonTruth.cfdistance
                        ,'cfdistance_muon':vertex.muonTruth.cfdistance
                        ,'PIDA_proton':vertex.protonTruth.pidpida
                        ,'PIDA_muon':vertex.muonTruth.pidpida
                        ,'GENIECC1p':vertex.GENIECC1p
                        ,'IsVertexContained':vertex.IsVertexContained
                        ,'IsMuonReconstructed':vertex.muonTrackReconstructed
                        ,'IsProtonReconstructed':vertex.protonTrackReconstructed
                        ,'IsVertexReconstructed':vertex.IsVertexReconstructed
                        },
                        index=[0])
        

    stream_dataframe_to_file( data, outcsvname  )
