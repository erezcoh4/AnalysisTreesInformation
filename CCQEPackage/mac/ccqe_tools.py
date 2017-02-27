
import sys; sys.path.insert(0, '../AnaTreesPackage/mac')
from anatrees_tools import *
from calc_tools import *
from ROOT import calcEventTopologies

r2d = 180./np.pi

# ---------------------------------------------------------------------------
# global constants for CC1p analysis
ccqe_pars = dict({
                 'max_mu_p_distance':11.0   # [cm]      # nominal 2 cm
                 
                 # for MC, take soft selection cuts here to study the purity and effficiency as a function of the cuts
                 ,'min_length_long':0.0     # [cm]      # nominal 10 cm
                 ,'max_length_short':200    # [cm]      # nominal 10 cm
                 ,'delta_phi_min':0.0       # [deg.]    # nominal 150 deg.
                 ,'delta_phi_max':361.0     # [deg.]
                 ,'PIDA_short_min':-1                   # nominal 16
                 ,'PIDA_short_max':1e13                 # nominal 24
                 ,'PIDA_long_min':-1                    # nominal 5
                 ,'PIDA_long_max':1e13                  # nominal 13
                 })

# ---------------------------------------------------------------------------


# ---------------------------------------------------------------------------
def stream_vertex_to_file( vertex=None , outcsvname='' , MCmode=True ):
    
    data = pd.DataFrame({
                        # event features
                        'run':vertex.run
                        ,'subrun':vertex.subrun
                        ,'event':vertex.event
                        
                        # 2-tracks vertex features
                        ,'counter_id':vertex.counter_id
                        ,'delta_phi':r2d*np.abs(vertex.reco_CC1p_Pp.Phi()-vertex.reco_CC1p_Pmu.Phi())
                        ,'delta_theta':r2d*np.abs(vertex.reco_CC1p_Pp.Theta()-vertex.reco_CC1p_Pmu.Theta())
                        ,'distance':vertex.distances_ij
                        ,'2_tracks_angle':vertex.GetAngleBetween2tracks()
                        ,'vertex_x':vertex.position.x()
                        ,'vertex_y':vertex.position.y()
                        ,'vertex_z':vertex.position.z()
                        
                        # single tracks features
                        # sort by length
                        ,'theta_short':vertex.ShortestTrack.theta
                        ,'theta_long':vertex.LongestTrack.theta
                        ,'l_short':vertex.ShortestTrack.length
                        ,'l_long':vertex.LongestTrack.length
                        ,'cfdistance_short':vertex.ShortestTrack.cfdistance
                        ,'cfdistance_long':vertex.LongestTrack.cfdistance
                        ,'PIDA_short':vertex.ShortestTrack.pidpida
                        ,'PIDA_long':vertex.LongestTrack.pidpida
                        ,'starty_short':vertex.ShortestTrack.starty
                        ,'starty_long':vertex.LongestTrack.starty
                        ,'endy_short':vertex.ShortestTrack.endy
                        ,'endy_long':vertex.LongestTrack.endy
                        ,'cosmicscore_short':vertex.ShortestTrack.cosmicscore
                        ,'cosmicscore_long':vertex.LongestTrack.cosmicscore
                        ,'coscontscore_short':vertex.ShortestTrack.coscontscore
                        ,'coscontscore_long':vertex.LongestTrack.coscontscore
                        ,'cfdistance_short':vertex.ShortestTrack.cfdistance
                        ,'cfdistance_long':vertex.LongestTrack.cfdistance
                        
                        # sort by pida
                        ,'theta_large_pida':vertex.LargePIDATrack.theta
                        ,'theta_small_pida':vertex.SmallPIDATrack.theta
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
                        
                        
                        
                        # cc1p reconstructed featues
                        ,'reco_CC1p_Pp':vertex.reco_CC1p_Pp.P()
                        ,'reco_CC1p_Pp_x':vertex.reco_CC1p_Pp.Px()
                        ,'reco_CC1p_Pp_y':vertex.reco_CC1p_Pp.Py()
                        ,'reco_CC1p_Pp_z':vertex.reco_CC1p_Pp.Pz()
                        ,'reco_CC1p_Pmu':vertex.reco_CC1p_Pmu.P()
                        ,'reco_CC1p_Pmu_x':vertex.reco_CC1p_Pmu.Px()
                        ,'reco_CC1p_Pmu_y':vertex.reco_CC1p_Pmu.Py()
                        ,'reco_CC1p_Pmu_z':vertex.reco_CC1p_Pmu.Pz()
                        ,'reco_CC1p_q':vertex.reco_CC1p_q.P()
                        ,'reco_CC1p_q_x':vertex.reco_CC1p_q.Px()
                        ,'reco_CC1p_q_y':vertex.reco_CC1p_q.Py()
                        ,'reco_CC1p_q_z':vertex.reco_CC1p_q.Pz()
                        ,'reco_CC1p_Ev':vertex.reco_CC1p_Pnu.E()
                        ,'reco_CC1p_theta_pq':vertex.reco_CC1p_theta_pq

                        # features that are only relevant for truth information
                        ,'pdg':[list(track.MCpdgCode for track in vertex.tracks)] if MCmode else -1000
                        
                        ,'pdg_large_pida':vertex.LargePIDATrack.MCpdgCode if MCmode else -1000
                        ,'pdg_small_pida':vertex.SmallPIDATrack.MCpdgCode if MCmode else -1000
                        ,'pdg_long':vertex.LongestTrack.MCpdgCode if MCmode else -1000
                        ,'pdg_short':vertex.ShortestTrack.MCpdgCode if MCmode else -1000
                        
                        ,'l_proton':vertex.protonTrueTrack.length if MCmode else -1000
                        ,'l_muon':vertex.muonTrueTrack.length if MCmode else -1000
                        ,'starty_proton':vertex.protonTrueTrack.starty if MCmode else -1000
                        ,'starty_muon':vertex.muonTrueTrack.starty if MCmode else -1000
                        
                        ,'proton_track_is_flipped':vertex.protonTrueTrack.is_flipped if MCmode else -1000
                        ,'muon_track_is_flipped':vertex.muonTrueTrack.is_flipped if MCmode else -1000
                        
                        
                        ,'cosmicscore_proton':vertex.protonTrueTrack.cosmicscore if MCmode else -1000
                        ,'cosmicscore_muon':vertex.muonTrueTrack.cosmicscore if MCmode else -1000
                        ,'coscontscore_proton':vertex.protonTrueTrack.coscontscore if MCmode else -1000
                        ,'coscontscore_muon':vertex.muonTrueTrack.coscontscore if MCmode else -1000
                        
                        ,'cfdistance_proton':vertex.protonTrueTrack.cfdistance if MCmode else -1000
                        ,'cfdistance_muon':vertex.muonTrueTrack.cfdistance if MCmode else -1000
                        ,'PIDA_proton':vertex.protonTrueTrack.pidpida if MCmode else -1000
                        ,'PIDA_muon':vertex.muonTrueTrack.pidpida if MCmode else -1000
                        #                        ,'1mu1p':() if MCmode else -1000
                        ,'GENIECC1p':vertex.GENIECC1p if MCmode else -1000
                        ,'IsVertexContained':vertex.IsVertexContained if MCmode else -1000
                        ,'IsMuonReconstructed':vertex.muonTrackReconstructed if MCmode else -1000
                        ,'IsProtonReconstructed':vertex.protonTrackReconstructed if MCmode else -1000
                        ,'IsVertexReconstructed':vertex.IsVertexReconstructed if MCmode else -1000
                        
                        # features that are only relevant for genie interaction information
                        ,'truth_Ev':vertex.genie_interaction.nu.E() if vertex.GENIECC1p else -1000
                        
                        ,'truth_Pp':vertex.genie_interaction.protons.at(0).P() if vertex.GENIECC1p else -1000
                        ,'truth_Pp_x':vertex.genie_interaction.protons.at(0).Px() if vertex.GENIECC1p else -1000
                        ,'truth_Pp_y':vertex.genie_interaction.protons.at(0).Py() if vertex.GENIECC1p else -1000
                        ,'truth_Pp_z':vertex.genie_interaction.protons.at(0).Pz() if vertex.GENIECC1p else -1000
                        
                        ,'truth_Pmu':vertex.genie_interaction.muon.P() if vertex.GENIECC1p else -1000
                        ,'truth_Pmu_x':vertex.genie_interaction.muon.Px() if vertex.GENIECC1p else -1000
                        ,'truth_Pmu_y':vertex.genie_interaction.muon.Py() if vertex.GENIECC1p else -1000
                        ,'truth_Pmu_z':vertex.genie_interaction.muon.Pz() if vertex.GENIECC1p else -1000
                        
                        ,'truth_q':vertex.genie_interaction.q.P() if vertex.GENIECC1p else -1000
                        ,'truth_q_x':vertex.genie_interaction.q.Px() if vertex.GENIECC1p else -1000
                        ,'truth_q_y':vertex.genie_interaction.q.Py() if vertex.GENIECC1p else -1000
                        ,'truth_q_z':vertex.genie_interaction.q.Pz() if vertex.GENIECC1p else -1000
                        ,'truth_theta_pq':vertex.genie_interaction.theta_pq if vertex.GENIECC1p else -1000
                        
                        },
                        index=[0])
                        #    data=data.append(genie_data)
                        #    data=pd.concat([data,genie_data])
                        
    stream_dataframe_to_file( data, outcsvname  )
# ---------------------------------------------------------------------------

