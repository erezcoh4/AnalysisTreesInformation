
import sys; sys.path.insert(0, '../AnaTreesPackage/mac'); sys.path.insert(0, 'notebooks');
from anatrees_tools import *
from calc_tools import *
from tracking_tools import *
from ROOT import calcEventTopologies, MyTrack
from matplotlib import ticker
import scipy.optimize as optimize

# constants
r2d = 180./np.pi

# globals
pair_types = ['all','1mu-1p','other pairs','cosmic','CC 1p 0pi']

# -- - - - -- - - - -- - - -- - - -- - - -- - -- - - - - -- - - -- -- -- - - -
# July-9
def count_vertex( vertex , counters , fractions ):#{
    counters['all'] += 1
    if vertex.Is1mu1p:#{
        counters['1mu-1p'] += 1
        if vertex.IsGENIECC_1p_200MeVc_0pi:#{
            counters['CC 1p 0pi'] += 1
        #}
    #}
    elif vertex.Non1mu1p:#{
        counters['other pairs'] += 1
    #}
    elif vertex.IsCosmic:#{
        counters['cosmic'] += 1
    #}
    for pair_type in pair_types: #{
        fractions[pair_type] = 100.0*float(counters[pair_type])/counters['all']
    #}
#}



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
def attach_hits_my_tracks_to_vertex( hits_charge_dict=None, my_tracks_dict=None , vertex=None ):
    
    for plane in range(3):
        
        associated_hit_charge_this_plane = []
        
        my_tracks_this_plane = my_tracks_dict['my_tracks in plane %d'%plane]

        for my_track in my_tracks_this_plane:
            
            if my_track!=0 and my_track is not None:
                
                my_track_object = MyTrack( plane )
                my_track_object.color = my_track['color']
                my_track_object.pandoraNu_track = my_track['pandoraNu_track']
                
                for hit in my_track['hits']:
                    my_track_object.hits.push_back( hit )
                    associated_hit_charge_this_plane.append( hit.hit_charge )
                
                vertex.my_tracks.push_back( my_track_object )

        sum_hits_charge = np.sum(hits_charge_dict['hit_charge in plane %d'%plane])
        ratio_associated_hit_charge_to_total = np.sum( associated_hit_charge_this_plane )/sum_hits_charge if sum_hits_charge>1 else 0

        if plane==0:
            vertex.associated_hit_charge_u = np.sum( associated_hit_charge_this_plane )
            vertex.total_hit_charge_u = np.sum(hits_charge_dict['hit_charge in plane %d'%plane])
            vertex.ratio_associated_hit_charge_to_total_u = ratio_associated_hit_charge_to_total
        elif plane==1:
            vertex.associated_hit_charge_v = np.sum( associated_hit_charge_this_plane )
            vertex.total_hit_charge_v = np.sum(hits_charge_dict['hit_charge in plane %d'%plane])
            vertex.ratio_associated_hit_charge_to_total_v = ratio_associated_hit_charge_to_total
        elif plane==2:
            vertex.associated_hit_charge_y = np.sum( associated_hit_charge_this_plane )
            vertex.total_hit_charge_y = np.sum(hits_charge_dict['hit_charge in plane %d'%plane])
            vertex.ratio_associated_hit_charge_to_total_y = ratio_associated_hit_charge_to_total

# ---------------------------------------------------------------------------




# ---------------------------------------------------------------------------
def stream_vertex_to_file( vertex=None , outcsvname='' , MCmode=True ):

    data = pd.DataFrame({
                        # event features
                        'run':vertex.run
                        ,'subrun':vertex.subrun
                        ,'event':vertex.event
                        
                        # 2-tracks vertex features
                        ,'vertex_id':vertex.vertex_id
                        ,'vertex_x':vertex.position.x()
                        ,'vertex_y':vertex.position.y()
                        ,'vertex_z':vertex.position.z()
                        ,'pida':[list(track.pidpida for track in vertex.tracks)] if vertex.tracks.size()>0 else -1000
                        ,'track_id':[list(track.track_id for track in vertex.tracks)] if vertex.tracks.size()>0 else -1000
                        ,'length':[list(track.length for track in vertex.tracks)] if vertex.tracks.size()>0 else -1000
                        ,'mcevent_id':[list(track.mcevent_id for track in vertex.tracks)] if MCmode and vertex.tracks.size()>0 else -1000
                        ,'startx':[list(track.startx for track in vertex.tracks)] if MCmode else -1000
                        ,'starty':[list(track.starty for track in vertex.tracks)] if MCmode else -1000
                        ,'endy':[list(track.endy for track in vertex.tracks)] if MCmode else -1000
                        ,'startz':[list(track.startz for track in vertex.tracks)] if MCmode else -1000

                        # single tracks features
                        # sort by length
                        ,'theta_short':vertex.ShortestTrack.theta if vertex.tracks.size()>0 else -1000
                        ,'theta_long':vertex.LongestTrack.theta if vertex.tracks.size()>0 else -1000
                        ,'l_short':vertex.ShortestTrack.length if vertex.tracks.size()>0 else -1000
                        ,'l_long':vertex.LongestTrack.length if vertex.tracks.size()>0 else -1000
                        ,'cfdistance_short':vertex.ShortestTrack.cfdistance if vertex.tracks.size()>0 else -1000
                        ,'cfdistance_long':vertex.LongestTrack.cfdistance if vertex.tracks.size()>0 else -1000
                        ,'PIDA_short':vertex.ShortestTrack.pidpida if vertex.tracks.size()>0 else -1000
                        ,'PIDA_long':vertex.LongestTrack.pidpida if vertex.tracks.size()>0 else -1000
                        ,'cfdistance_short':vertex.ShortestTrack.cfdistance if vertex.tracks.size()>0 else -1000
                        ,'cfdistance_long':vertex.LongestTrack.cfdistance if vertex.tracks.size()>0 else -1000

                        # sort by pida
                        ,'theta_large_pida':vertex.LargePIDATrack.theta if vertex.tracks.size()>0 else -1000
                        ,'theta_small_pida':vertex.SmallPIDATrack.theta if vertex.tracks.size()>0 else -1000
                        ,'l_large_pida':vertex.LargePIDATrack.length if vertex.tracks.size()>0 else -1000
                        ,'l_small_pida':vertex.SmallPIDATrack.length if vertex.tracks.size()>0 else -1000
                        ,'cfdistance_large_pida':vertex.LargePIDATrack.cfdistance if vertex.tracks.size()>0 else -1000
                        ,'cfdistance_small_pida':vertex.SmallPIDATrack.cfdistance if vertex.tracks.size()>0 else -1000
                        ,'PIDA_large_pida':vertex.LargePIDATrack.pidpida if vertex.tracks.size()>0 else -1000
                        ,'PIDA_small_pida':vertex.SmallPIDATrack.pidpida if vertex.tracks.size()>0 else -1000
                        ,'cosmicscore_large_pida':vertex.LargePIDATrack.cosmicscore if vertex.tracks.size()>0 else -1000
                        ,'cosmicscore_small_pida':vertex.SmallPIDATrack.cosmicscore if vertex.tracks.size()>0 else -1000
                        ,'coscontscore_large_pida':vertex.LargePIDATrack.coscontscore if vertex.tracks.size()>0 else -1000
                        ,'coscontscore_small_pida':vertex.SmallPIDATrack.coscontscore if vertex.tracks.size()>0 else -1000
                        ,'cfdistance_large_pida':vertex.LargePIDATrack.cfdistance if vertex.tracks.size()>0 else -1000
                        ,'cfdistance_small_pida':vertex.SmallPIDATrack.cfdistance if vertex.tracks.size()>0 else -1000

                        # assigned tracks
                        ,'l_assigned_muon':vertex.AssignedMuonTrack.length if vertex.tracks.size()>0 else -1000
                        ,'l_assigned_proton':vertex.AssignedProtonTrack.length if vertex.tracks.size()>0 else -1000
                        ,'cfdistance_assigned_muon':vertex.AssignedMuonTrack.cfdistance if vertex.tracks.size()>0 else -1000
                        ,'cfdistance_assigned_proton':vertex.AssignedProtonTrack.cfdistance if vertex.tracks.size()>0 else -1000
                        ,'PIDA_assigned_muon':vertex.AssignedMuonTrack.pidpida if vertex.tracks.size()>0 else -1000
                        ,'PIDA_assigned_proton':vertex.AssignedProtonTrack.pidpida if vertex.tracks.size()>0 else -1000
                        ,'starty_assigned_muon':vertex.AssignedMuonTrack.starty if vertex.tracks.size()>0 else -1000
                        ,'starty_assigned_proton':vertex.AssignedProtonTrack.starty if vertex.tracks.size()>0 else -1000
                        ,'endy_assigned_muon':vertex.AssignedMuonTrack.endy if vertex.tracks.size()>0 else -1000
                        ,'endy_assigned_proton':vertex.AssignedProtonTrack.endy if vertex.tracks.size()>0 else -1000
                        ,'startz_assigned_muon':vertex.AssignedMuonTrack.startz if vertex.tracks.size()>0 else -1000
                        ,'startz_assigned_proton':vertex.AssignedProtonTrack.startz if vertex.tracks.size()>0 else -1000
                        ,'endz_assigned_muon':vertex.AssignedMuonTrack.endz if vertex.tracks.size()>0 else -1000
                        ,'endz_assigned_proton':vertex.AssignedProtonTrack.endz if vertex.tracks.size()>0 else -1000
                        ,'startx_assigned_muon':vertex.AssignedMuonTrack.startx if vertex.tracks.size()>0 else -1000
                        ,'startx_assigned_proton':vertex.AssignedProtonTrack.startx if vertex.tracks.size()>0 else -1000
                        ,'endx_assigned_muon':vertex.AssignedMuonTrack.endx if vertex.tracks.size()>0 else -1000
                        ,'endx_assigned_proton':vertex.AssignedProtonTrack.endx if vertex.tracks.size()>0 else -1000
                        ,'momrange_assigned_muon':vertex.AssignedMuonTrack.momrange if vertex.tracks.size()>0 else -1000
                        ,'momrange_assigned_proton':vertex.AssignedProtonTrack.momrange if vertex.tracks.size()>0 else -1000
                        ,'mommsllhd_assigned_muon':vertex.AssignedMuonTrack.mommsllhd if vertex.tracks.size()>0 else -1000
                        ,'mommsllhd_assigned_proton':vertex.AssignedProtonTrack.mommsllhd if vertex.tracks.size()>0 else -1000
                        ,'ratio_momrange_mommsllhd_assigned_muon':(vertex.AssignedMuonTrack.momrange/vertex.AssignedMuonTrack.mommsllhd) if vertex.tracks.size()>0 and vertex.AssignedMuonTrack.mommsllhd!=0 else -1000
                        ,'ratio_momrange_mommsllhd_assigned_proton':(vertex.AssignedProtonTrack.momrange/vertex.AssignedProtonTrack.mommsllhd) if vertex.tracks.size()>0 and vertex.AssignedProtonTrack.mommsllhd!=0 else -1000

                        
                        # cc1p reconstructed featues
                        ,'distance':vertex.distances_ij if vertex.tracks.size()>1 else -1000
                        ,'delta_phi':r2d*np.abs(vertex.reco_CC1p_Pp.Phi()-vertex.reco_CC1p_Pmu.Phi()) if vertex.tracks.size()>1 else -1000
                        ,'delta_theta':r2d*np.abs(vertex.reco_CC1p_Pp.Theta()-vertex.reco_CC1p_Pmu.Theta()) if vertex.tracks.size()>1 else -1000
                        ,'2_tracks_angle':vertex.GetAngleBetween2tracks() if vertex.tracks.size()>1 else -1000
                        ,'reco_CC1p_l_proton':vertex.AssignedProtonTrack.length if MCmode and vertex.tracks.size()>0 else -1000
                        ,'reco_CC1p_l_muon':vertex.AssignedMuonTrack.length if MCmode and vertex.tracks.size()>0 else -1000
                        ,'reco_CC1p_Ep':vertex.reco_CC1p_Pp.E() if vertex.tracks.size()>1 else -1000
                        ,'reco_CC1p_Pp':vertex.reco_CC1p_Pp.P() if vertex.tracks.size()>1 else -1000
                        ,'reco_CC1p_Pp_x':vertex.reco_CC1p_Pp.Px() if vertex.tracks.size()>1 else -1000
                        ,'reco_CC1p_Pp_y':vertex.reco_CC1p_Pp.Py() if vertex.tracks.size()>1 else -1000
                        ,'reco_CC1p_Pp_z':vertex.reco_CC1p_Pp.Pz() if vertex.tracks.size()>1 else -1000
                        ,'reco_CC1p_Pp_theta':vertex.reco_CC1p_Pp.Theta() if vertex.tracks.size()>1 else -1000
                        ,'reco_CC1p_alpha_p':vertex.reco_CC1p_alpha_p if vertex.tracks.size()>1 else -1000
                        ,'reco_CC1p_Emu':vertex.reco_CC1p_Pmu.E() if vertex.muonTrackReconstructed or vertex.tracks.size()>1 else -1000
                        ,'reco_CC1p_Pmu':vertex.reco_CC1p_Pmu.P() if vertex.muonTrackReconstructed or vertex.tracks.size()>1 else -1000
                        ,'reco_CC1p_Pmu_x':vertex.reco_CC1p_Pmu.Px() if vertex.tracks.size()>1 else -1000
                        ,'reco_CC1p_Pmu_y':vertex.reco_CC1p_Pmu.Py() if vertex.tracks.size()>1 else -1000
                        ,'reco_CC1p_Pmu_z':vertex.reco_CC1p_Pmu.Pz() if vertex.tracks.size()>1 else -1000
                        ,'reco_CC1p_Pmu_theta':vertex.reco_CC1p_Pmu.Theta() if vertex.muonTrackReconstructed or vertex.tracks.size()>1 else -1000
                        ,'reco_CC1p_alpha_mu':vertex.reco_CC1p_alpha_mu if vertex.tracks.size()>1 else -1000
                        ,'reco_CC1p_alpha_miss':vertex.reco_CC1p_alpha_miss if vertex.tracks.size()>1 else -1000
                        ,'reco_CC1p_alpha_q':vertex.reco_CC1p_alpha_q if vertex.tracks.size()>1 else -1000
                        ,'reco_CC1p_Pt':( vertex.reco_CC1p_Pmu + vertex.reco_CC1p_Pp ).Pt() if vertex.tracks.size()>0 else -1000
                        ,'reco_CC1p_q':vertex.reco_CC1p_q.P() if vertex.tracks.size()>1 else -1000
                        ,'reco_CC1p_q_x':vertex.reco_CC1p_q.Px() if vertex.tracks.size()>1 else -1000
                        ,'reco_CC1p_q_y':vertex.reco_CC1p_q.Py() if vertex.tracks.size()>1 else -1000
                        ,'reco_CC1p_q_z':vertex.reco_CC1p_q.Pz() if vertex.tracks.size()>1 else -1000
                        ,'reco_CC1p_Ev':vertex.reco_CC1p_Pnu.E() if vertex.tracks.size()>1 else -1000
                        ,'reco_CC1p_W2':vertex.reco_CC1p_W2 if vertex.tracks.size()>1 else -1000
                        ,'reco_CC1p_theta_pq':vertex.reco_CC1p_theta_pq if vertex.tracks.size()>1 else -1000
                        ,'reco_CC1p_theta_pq_mcsllhd':vertex.reco_CC1p_theta_pq_mcsllhd if vertex.tracks.size()>1 else -1000
                        ,'reco_CC1p_p_over_q':vertex.reco_CC1p_p_over_q if vertex.tracks.size()>1 else -1000
                        ,'reco_CC1p_p_over_q_mcsllhd':vertex.reco_CC1p_p_over_q_mcsllhd if vertex.tracks.size()>1 else -1000
                        ,'reco_CC1p_n_miss':vertex.reco_CC1p_n_miss.P() if vertex.tracks.size()>1 else -1000
                        ,'reco_CC1p_Xb':vertex.reco_CC1p_Xb if vertex.tracks.size()>1 else -1000
                        ,'reco_CC1p_y':vertex.reco_CC1p_y if vertex.tracks.size()>1 else -1000
                        ,'reco_CC1p_yXb':(vertex.reco_CC1p_y*vertex.reco_CC1p_Xb) if vertex.tracks.size()>1 else -1000
                        ,'reco_CC1p_s':vertex.reco_CC1p_s if vertex.tracks.size()>1 else -1000
                        ,'reco_CC1p_Q2':vertex.reco_CC1p_Q2 if vertex.tracks.size()>1 else -1000
                        ,'reco_CC1p_Q2_mcsllhd':vertex.reco_CC1p_Q2_mcsllhd if vertex.tracks.size()>1 else -1000
                        ,'reco_CC1p_Q2_from_angles':vertex.reco_CC1p_Q2_from_angles if vertex.tracks.size()>1 else -1000
                        ,'reco_CC1p_Q2_from_angles_mcsllhd':vertex.reco_CC1p_Q2_from_angles_mcsllhd if vertex.tracks.size()>1 else -1000
                        ,'reco_CC1p_Q2_from_angles_diff':vertex.reco_CC1p_Q2_from_angles_diff if vertex.tracks.size()>1 else -1000
                        ,'reco_CC1p_Q2_from_angles_ratio':vertex.reco_CC1p_Q2_from_angles_ratio if vertex.tracks.size()>1 else -1000
                        ,'reco_CC1p_omega':vertex.reco_CC1p_omega if vertex.tracks.size()>1 else -1000
                        ,'reco_CC1p_Ev_from_angles':vertex.reco_CC1p_Ev_from_angles if vertex.tracks.size()>1 else -1000
                        ,'reco_CC1p_Ev_from_angles_Ev_from_mu_p_diff':vertex.reco_CC1p_Ev_from_angles_Ev_from_mu_p_diff if vertex.tracks.size()>1 else -1000
                        ,'reco_CC1p_Ev_from_angles_Ev_from_mu_p_ratio':vertex.reco_CC1p_Ev_from_angles_Ev_from_mu_p_ratio if vertex.tracks.size()>1 else -1000
                        ,'reco_CC1p_Ev_with_binding':vertex.reco_CC1p_Ev_with_binding if vertex.tracks.size()>1 else -1000
                        ,'reco_CC1p_Ev_with_binding_diff':vertex.reco_CC1p_Ev_with_binding_diff if vertex.tracks.size()>1 else -1000
                        ,'reco_CC1p_Ev_with_binding_ratio':vertex.reco_CC1p_Ev_with_binding_ratio if vertex.tracks.size()>1 else -1000
                        
                        
                        ,'reco_CC1p_Ev_fromE':vertex.reco_CC1p_Pnu_fromE.E() if vertex.tracks.size()>1 else -1000
                        ,'reco_CC1p_alpha_miss_fromE':vertex.reco_CC1p_alpha_miss_fromE if vertex.tracks.size()>1 else -1000
                        ,'reco_CC1p_alpha_q_fromE':vertex.reco_CC1p_alpha_q_fromE if vertex.tracks.size()>1 else -1000
                        ,'reco_CC1p_q_fromE':vertex.reco_CC1p_q_fromE.P() if vertex.tracks.size()>1 else -1000
                        ,'reco_CC1p_q_x_fromE':vertex.reco_CC1p_q_fromE.Px() if vertex.tracks.size()>1 else -1000
                        ,'reco_CC1p_q_y_fromE':vertex.reco_CC1p_q_fromE.Py() if vertex.tracks.size()>1 else -1000
                        ,'reco_CC1p_q_z_fromE':vertex.reco_CC1p_q_fromE.Pz() if vertex.tracks.size()>1 else -1000
                        ,'reco_CC1p_W2_fromE':vertex.reco_CC1p_W2_fromE if vertex.tracks.size()>1 else -1000
                        ,'reco_CC1p_theta_pq_fromE':vertex.reco_CC1p_theta_pq_fromE if vertex.tracks.size()>1 else -1000
                        ,'reco_CC1p_p_over_q_fromE':vertex.reco_CC1p_p_over_q_fromE if vertex.tracks.size()>1 else -1000
                        ,'reco_CC1p_n_miss_fromE':vertex.reco_CC1p_n_miss_fromE.P() if vertex.tracks.size()>1 else -1000
                        ,'reco_CC1p_n_miss_x_fromE':vertex.reco_CC1p_n_miss_fromE.Px() if vertex.tracks.size()>1 else -1000
                        ,'reco_CC1p_n_miss_y_fromE':vertex.reco_CC1p_n_miss_fromE.Py() if vertex.tracks.size()>1 else -1000
                        ,'reco_CC1p_n_miss_z_fromE':vertex.reco_CC1p_n_miss_fromE.Pz() if vertex.tracks.size()>1 else -1000
                        ,'reco_CC1p_Xb_fromE':vertex.reco_CC1p_Xb_fromE if vertex.tracks.size()>1 else -1000
                        ,'reco_CC1p_y_fromE':vertex.reco_CC1p_y_fromE if vertex.tracks.size()>1 else -1000
                        ,'reco_CC1p_s_fromE':vertex.reco_CC1p_s_fromE if vertex.tracks.size()>1 else -1000
                        ,'reco_CC1p_Q2_fromE':vertex.reco_CC1p_Q2_fromE if vertex.tracks.size()>1 else -1000
                        ,'reco_CC1p_Q2_fromE_from_angles_diff':(vertex.reco_CC1p_Q2_from_angles-vertex.reco_CC1p_Q2_fromE) if vertex.tracks.size()>1 else -1000
                        ,'reco_CC1p_Q2_fromE_from_angles_ratio':(vertex.reco_CC1p_Q2_from_angles/vertex.reco_CC1p_Q2_fromE) if vertex.tracks.size()>1 and np.abs(vertex.reco_CC1p_Q2_fromE)>0 else -1000

                        ,'reco_CC1p_omega_fromE':vertex.reco_CC1p_omega_fromE if vertex.tracks.size()>1 else -1000
                        ,'reco_CC1p_Ev_fromE_from_angles_Ev_from_mu_p_diff':(vertex.reco_CC1p_Ev_from_angles-vertex.reco_CC1p_Ev_fromE) if vertex.tracks.size()>1 else -1000
                        ,'reco_CC1p_Ev_fromE_from_angles_Ev_from_mu_p_ratio':(vertex.reco_CC1p_Ev_from_angles//vertex.reco_CC1p_Ev_fromE) if vertex.tracks.size()>1 and vertex.reco_CC1p_Ev_fromE>0 else -1000
                        ,'reco_CC1p_Ev_fromE_with_binding_diff':(vertex.reco_CC1p_Ev_with_binding-vertex.reco_CC1p_Ev_fromE) if vertex.tracks.size()>1 else -1000
                        ,'reco_CC1p_Ev_fromE_with_binding_ratio':(vertex.reco_CC1p_Ev_with_binding/vertex.reco_CC1p_Ev_fromE) if vertex.tracks.size()>1 and vertex.reco_CC1p_Ev_fromE>0 else -1000
                        ,'reco_CC1p_Mmiss_fromE':(vertex.reco_CC1p_n_miss_fromE - ROOT.TLorentzVector(0,0,0,0.939)).Mag() if vertex.tracks.size()>1 else -1000


#                        # Pmu reco. correction
#                        ,'reco_CC1p_Pmu_corrected':vertex.reco_CC1p_Pmu_corrected.P() if vertex.tracks.size()>1  else -1000
#                        ,'reco_CC1p_Emu_corrected':vertex.reco_CC1p_Pmu_corrected.E() if vertex.tracks.size()>1  else -1000
#                        ,'reco_CC1p_Pp_corrected':vertex.reco_CC1p_Pp_corrected.P() if vertex.tracks.size()>1  else -1000
#                        ,'reco_CC1p_Ep_corrected':vertex.reco_CC1p_Pp_corrected.E() if vertex.tracks.size()>1  else -1000
#                        ,'reco_CC1p_Ev_corrected':vertex.reco_CC1p_Pnu_corrected.E() if vertex.tracks.size()>1 else -1000
#                        ,'reco_CC1p_alpha_miss_corrected':vertex.reco_CC1p_alpha_miss_corrected if vertex.tracks.size()>1 else -1000
#                        ,'reco_CC1p_alpha_q_corrected':vertex.reco_CC1p_alpha_q_corrected if vertex.tracks.size()>1 else -1000
#                        ,'reco_CC1p_alpha_p_corrected':vertex.reco_CC1p_alpha_p_corrected if vertex.tracks.size()>1 else -1000
#                        ,'reco_CC1p_alpha_mu_corrected':vertex.reco_CC1p_alpha_mu_corrected if vertex.tracks.size()>1 else -1000
#                        ,'reco_CC1p_q_corrected':vertex.reco_CC1p_q_corrected.P() if vertex.tracks.size()>1 else -1000
#                        ,'reco_CC1p_W2_corrected':vertex.reco_CC1p_W2_corrected if vertex.tracks.size()>1 else -1000
#                        ,'reco_CC1p_theta_pq_corrected':vertex.reco_CC1p_theta_pq_corrected if vertex.tracks.size()>1 else -1000
#                        ,'reco_CC1p_p_over_q_corrected':vertex.reco_CC1p_p_over_q_corrected if vertex.tracks.size()>1 else -1000
#                        ,'reco_CC1p_n_miss_corrected':vertex.reco_CC1p_n_miss_corrected.P() if vertex.tracks.size()>1 else -1000
#                        ,'reco_CC1p_Xb_corrected':vertex.reco_CC1p_Xb_corrected if vertex.tracks.size()>1 else -1000
#                        ,'reco_CC1p_y_corrected':vertex.reco_CC1p_y_corrected if vertex.tracks.size()>1 else -1000
#                        ,'reco_CC1p_s_corrected':vertex.reco_CC1p_s_corrected if vertex.tracks.size()>1 else -1000
#                        ,'reco_CC1p_Q2_corrected':vertex.reco_CC1p_Q2_corrected if vertex.tracks.size()>1 else -1000
#                        ,'reco_CC1p_omega_corrected':vertex.reco_CC1p_omega_corrected if vertex.tracks.size()>1 else -1000
#                        ,'reco_CC1p_Q2_corrected_from_angles_diff':(vertex.reco_CC1p_Q2_from_angles-vertex.reco_CC1p_Q2_corrected) if vertex.tracks.size()>1 else -1000
#                        ,'reco_CC1p_Q2_corrected_from_angles_ratio':(vertex.reco_CC1p_Q2_from_angles/vertex.reco_CC1p_Q2_corrected) if vertex.tracks.size()>1 and np.abs(vertex.reco_CC1p_Q2_corrected)>0 else -1000
#                        ,'reco_CC1p_Pt_corrected':( vertex.reco_CC1p_Pmu_corrected + vertex.reco_CC1p_Pp_corrected ).Pt() if vertex.tracks.size()>0 else -1000
#
#                        
#                        # my tracking
#                        ,'associated_hit_charge_u':vertex.TracksAssociatedCharge[0] if vertex.tracks.size()>0 else -1000
#                        ,'associated_hit_charge_v':vertex.TracksAssociatedCharge[1] if vertex.tracks.size()>0 else -1000
#                        ,'associated_hit_charge_y':vertex.TracksAssociatedCharge[2] if vertex.tracks.size()>0 else -1000
#                        ,'total_hit_charge_u':vertex.AllChargeInVertexROI[0] if vertex.tracks.size()>0 else -1000
#                        ,'total_hit_charge_v':vertex.AllChargeInVertexROI[1] if vertex.tracks.size()>0 else -1000
#                        ,'total_hit_charge_y':vertex.AllChargeInVertexROI[2] if vertex.tracks.size()>0 else -1000
#                        ,'total_hit_charge_u_enlarged_20_100':vertex.AllChargeInVertexROI_enlarged_20_100[0] if vertex.tracks.size()>0 else -1000
#                        ,'total_hit_charge_v_enlarged_20_100':vertex.AllChargeInVertexROI_enlarged_20_100[1] if vertex.tracks.size()>0 else -1000
#                        ,'total_hit_charge_y_enlarged_20_100':vertex.AllChargeInVertexROI_enlarged_20_100[2] if vertex.tracks.size()>0 else -1000
#                        ,'ratio_associated_hit_charge_to_total_u':vertex.ratio_associated_hit_charge_to_total[0] if vertex.tracks.size()>0 else -1000
#                        ,'ratio_associated_hit_charge_to_total_v':vertex.ratio_associated_hit_charge_to_total[1] if vertex.tracks.size()>0 else -1000
#                        ,'ratio_associated_hit_charge_to_total_y':vertex.ratio_associated_hit_charge_to_total[2] if vertex.tracks.size()>0 else -1000
#                        ,'ratio_associated_hit_charge_to_total_u_enlarged_20_100':vertex.ratio_associated_hit_charge_to_total_enlarged_20_100[0] if vertex.tracks.size()>0 else -1000
#                        ,'ratio_associated_hit_charge_to_total_v_enlarged_20_100':vertex.ratio_associated_hit_charge_to_total_enlarged_20_100[1] if vertex.tracks.size()>0 else -1000
#                        ,'ratio_associated_hit_charge_to_total_y_enlarged_20_100':vertex.ratio_associated_hit_charge_to_total_enlarged_20_100[2] if vertex.tracks.size()>0 else -1000
#                        ,'ratio_associated_hit_charge_to_total_u_enlarged_40_200':vertex.ratio_associated_hit_charge_to_total_enlarged_40_200[0] if vertex.tracks.size()>0 else -1000
#                        ,'ratio_associated_hit_charge_to_total_v_enlarged_40_200':vertex.ratio_associated_hit_charge_to_total_enlarged_40_200[1] if vertex.tracks.size()>0 else -1000
#                        ,'ratio_associated_hit_charge_to_total_y_enlarged_40_200':vertex.ratio_associated_hit_charge_to_total_enlarged_40_200[2] if vertex.tracks.size()>0 else -1000
#                        ,'average_ratio_associated_hit_charge_to_total':vertex.average_ratio_associated_hit_charge_to_total if vertex.tracks.size()>0 else -1000
#                        ,'ratio_dQassociated_dQtot_ROI_20x40_AroundVertex_u':vertex.ratio_dQassociated_dQtot_ROI_20x40_AroundVertex[0] if vertex.tracks.size()>0 else -1000
#                        ,'ratio_dQassociated_dQtot_ROI_20x40_AroundVertex_v':vertex.ratio_dQassociated_dQtot_ROI_20x40_AroundVertex[1] if vertex.tracks.size()>0 else -1000
#                        ,'ratio_dQassociated_dQtot_ROI_20x40_AroundVertex_y':vertex.ratio_dQassociated_dQtot_ROI_20x40_AroundVertex[2] if vertex.tracks.size()>0 else -1000
#                        
#                        
                        # features that are only relevant for truth information
                        ,'pdg':[list(track.MCpdgCode for track in vertex.tracks)] if MCmode and vertex.tracks.size()>0  else -1000
                        ,'truth_startx':[list(track.truth_startx for track in vertex.tracks)] if MCmode and vertex.tracks.size()>0 else -1000
                        ,'truth_starty':[list(track.truth_starty for track in vertex.tracks)] if MCmode and vertex.tracks.size()>0 else -1000
                        ,'truth_startz':[list(track.truth_startz for track in vertex.tracks)] if MCmode and vertex.tracks.size()>0 else -1000
                        ,'truth_endy':[list(track.truth_endy for track in vertex.tracks)] if MCmode and vertex.tracks.size()>0 else -1000
                        ,'process_primary':[list(track.process_primary for track in vertex.tracks)] if MCmode and vertex.tracks.size()>0 else -1000
                        
                        ,'pdg_large_pida':vertex.LargePIDATrack.MCpdgCode if MCmode and vertex.tracks.size()>0 else -1000
                        ,'pdg_small_pida':vertex.SmallPIDATrack.MCpdgCode if MCmode and vertex.tracks.size()>0 else -1000
                        ,'pdg_long':vertex.LongestTrack.MCpdgCode if MCmode and vertex.tracks.size()>0 else -1000
                        ,'pdg_short':vertex.ShortestTrack.MCpdgCode if MCmode and vertex.tracks.size()>0 else -1000
                        
                        ,'l_proton':vertex.protonTrueTrack.length if MCmode and vertex.tracks.size()>0 else -1000
                        ,'l_muon':vertex.muonTrueTrack.length if MCmode and vertex.tracks.size()>0 else -1000
                        ,'starty_proton':vertex.protonTrueTrack.starty if MCmode and vertex.tracks.size()>0 else -1000
                        ,'starty_muon':vertex.muonTrueTrack.starty if MCmode and vertex.tracks.size()>0  else -1000
                        
                        ,'cosmicscore_proton':vertex.protonTrueTrack.cosmicscore if MCmode and vertex.tracks.size()>0 else -1000
                        ,'cosmicscore_muon':vertex.muonTrueTrack.cosmicscore if MCmode and vertex.tracks.size()>0 else -1000
                        ,'coscontscore_proton':vertex.protonTrueTrack.coscontscore if MCmode and vertex.tracks.size()>0 else -1000
                        ,'coscontscore_muon':vertex.muonTrueTrack.coscontscore if MCmode and vertex.tracks.size()>0 else -1000
                        
                        ,'cfdistance_proton':vertex.protonTrueTrack.cfdistance if MCmode and vertex.tracks.size()>0 else -1000
                        ,'cfdistance_muon':vertex.muonTrueTrack.cfdistance if MCmode and vertex.tracks.size()>0 else -1000
                        ,'PIDA_proton':vertex.protonTrueTrack.pidpida if MCmode and vertex.tracks.size()>0 else -1000
                        ,'PIDA_muon':vertex.muonTrueTrack.pidpida if MCmode and vertex.tracks.size()>0 else -1000
                        
                        # truth topology of the event
                        ,'all':True if MCmode else -1000
                        ,'1mu-1p':vertex.Is1mu1p if MCmode else -1000
                        ,'CC 1p 0pi':vertex.IsGENIECC_1p_200MeVc_0pi if MCmode else -1000
                        ,'other pairs':vertex.Non1mu1p if MCmode else -1000
                        ,'cosmic':vertex.IsCosmic if MCmode else -1000

                        # closest GENIE interaction
                        ,'closest_genie_distance':(vertex.closest_genie_interaction.vertex_position-vertex.position).Mag() if MCmode else -1000
                        ,'closest_genie_Ev':vertex.closest_genie_interaction.nu.E() if MCmode else -1000
                        
                        ,'closest_genie_x':vertex.closest_genie_interaction.vertex_position.x() if MCmode else -1000
                        ,'closest_genie_y':vertex.closest_genie_interaction.vertex_position.y() if MCmode else -1000
                        ,'closest_genie_z':vertex.closest_genie_interaction.vertex_position.z() if MCmode else -1000
                        ,'closest_genie_Nprimaries':vertex.closest_genie_interaction.Nprimaries if MCmode else -1000
                        ,'closest_genie_Np':vertex.closest_genie_interaction.protons.size() if MCmode else -1000
                        ,'closest_genie_Nn':vertex.closest_genie_interaction.neutrons.size() if MCmode else -1000
                        ,'closest_genie_Npi':vertex.closest_genie_interaction.Npi if MCmode else -1000
                        ,'closest_genie_Nmu':vertex.closest_genie_interaction.Nmu if MCmode else -1000
                        ,'closest_genie_Nel':vertex.closest_genie_interaction.Nel if MCmode else -1000
                        ,'closest_genie_Ngamma':vertex.closest_genie_interaction.Ngamma if MCmode else -1000
                        
                        ,'closest_genie_Nnu_e':vertex.closest_genie_interaction.Nnu_e if MCmode else -1000
                        ,'closest_genie_Nnu_mu':vertex.closest_genie_interaction.Nnu_mu if MCmode else -1000
                        ,'closest_genie_Nmu_minus':vertex.closest_genie_interaction.Nmu_minus if MCmode else -1000
                        ,'closest_genie_Nmu_plus':vertex.closest_genie_interaction.Nmu_plus if MCmode else -1000
                        ,'closest_genie_Npi_minus':vertex.closest_genie_interaction.Npi_minus if MCmode else -1000
                        ,'closest_genie_Npi_plus':vertex.closest_genie_interaction.Npi_plus if MCmode else -1000
                        ,'closest_genie_Npi_0':vertex.closest_genie_interaction.Npi_0 if MCmode else -1000
                        ,'closest_genie_Ne_plus':vertex.closest_genie_interaction.Ne_plus if MCmode else -1000
                        ,'closest_genie_Ne_minus':vertex.closest_genie_interaction.Ne_minus if MCmode else -1000

                        
                        # features that are only relevant for genie interaction information
                        ,'truth_Ev':vertex.genie_interaction.nu.E() if MCmode else -1000
                        
                        ,'Nprimaries':vertex.genie_interaction.Nprimaries if MCmode else -1000
                        ,'truth_Np':vertex.genie_interaction.protons.size() if MCmode else -1000
                        ,'truth_Nn':vertex.genie_interaction.neutrons.size() if MCmode else -1000
                        ,'truth_Npi':vertex.genie_interaction.Npi if MCmode else -1000
                        ,'truth_Nmu':vertex.genie_interaction.Nmu if MCmode else -1000
                        ,'truth_Nel':vertex.genie_interaction.Nel if MCmode else -1000
                        ,'truth_Ngamma':vertex.genie_interaction.Ngamma if MCmode else -1000

                        ,'truth_Ep':vertex.genie_interaction.protons.at(0).E() if MCmode and vertex.genie_interaction.protons.size()>0 else -1000
                        ,'truth_Pp':vertex.genie_interaction.protons.at(0).P() if MCmode and vertex.genie_interaction.protons.size()>0 else -1000
                        ,'truth_Pp_theta':vertex.genie_interaction.protons.at(0).Theta() if MCmode and vertex.genie_interaction.protons.size()>0 else -1000
                        ,'truth_Pp_x':vertex.genie_interaction.protons.at(0).Px() if MCmode and vertex.genie_interaction.protons.size()>0 else -1000
                        ,'truth_Pp_y':vertex.genie_interaction.protons.at(0).Py() if MCmode and vertex.genie_interaction.protons.size()>0 else -1000
                        ,'truth_Pp_z':vertex.genie_interaction.protons.at(0).Pz() if MCmode and vertex.genie_interaction.protons.size()>0 else -1000
                        ,'truth_alpha_p':(vertex.genie_interaction.protons.at(0).E()-vertex.genie_interaction.protons.at(0).Pz())/0.931 if MCmode and vertex.genie_interaction.protons.size()>0 else -1000

                        ,'truth_l_muon':(vertex.muonTrueTrack.truth_start_pos-vertex.muonTrueTrack.truth_end_pos).Mag() if MCmode else -1000
                        ,'truth_l_proton':(vertex.protonTrueTrack.truth_start_pos-vertex.protonTrueTrack.truth_end_pos).Mag() if MCmode else -1000

                        ,'truth_Emu':vertex.genie_interaction.muon.E() if MCmode else -1000
                        ,'truth_Pmu':vertex.genie_interaction.muon.P() if MCmode else -1000
                        ,'truth_Pmu_theta':vertex.genie_interaction.muon.Theta() if MCmode else -1000
                        ,'truth_Pmu_x':vertex.genie_interaction.muon.Px() if MCmode else -1000
                        ,'truth_Pmu_y':vertex.genie_interaction.muon.Py() if MCmode else -1000
                        ,'truth_Pmu_z':vertex.genie_interaction.muon.Pz() if MCmode else -1000
                        ,'truth_alpha_mu':(vertex.genie_interaction.muon.E()-vertex.genie_interaction.muon.Pz())/0.931 if MCmode else -1000
                        
                        ,'truth_q':vertex.genie_interaction.q.P() if MCmode else -1000
                        ,'truth_q_x':vertex.genie_interaction.q.Px() if MCmode else -1000
                        ,'truth_q_y':vertex.genie_interaction.q.Py() if MCmode else -1000
                        ,'truth_q_z':vertex.genie_interaction.q.Pz() if MCmode else -1000
                        ,'truth_n_miss':vertex.genie_interaction.n_miss.P() if MCmode else -1000
                        ,'truth_n_miss_x':vertex.genie_interaction.n_miss.Px() if MCmode else -1000
                        ,'truth_n_miss_y':vertex.genie_interaction.n_miss.Py() if MCmode else -1000
                        ,'truth_n_miss_z':vertex.genie_interaction.n_miss.Pz() if MCmode else -1000
                        ,'truth_theta_pq':vertex.genie_interaction.theta_pq if MCmode else -1000
                        ,'truth_p_over_q':vertex.genie_interaction.p_over_q if MCmode else -1000
                        ,'truth_Q2':vertex.genie_interaction.Q2 if MCmode else -1000
                        ,'truth_W2':(0.939*(0.939 + 2*(vertex.genie_interaction.nu.E() - vertex.genie_interaction.muon.E()))
                                     - 4*vertex.genie_interaction.nu.E()*vertex.genie_interaction.muon.E()*(1.-np.cos(vertex.genie_interaction.muon.Theta()))) if MCmode else -1000
                        ,'truth_Xb':vertex.genie_interaction.Xb if MCmode else -1000
                        ,'truth_omega':vertex.genie_interaction.q.E() if MCmode else -1000
                        ,'truth_alpha_p':vertex.truth_alpha_p if vertex.tracks.size()>1 else -1000
                        ,'truth_alpha_q':vertex.truth_alpha_q if vertex.tracks.size()>1 else -1000
                        ,'truth_alpha_mu':vertex.truth_alpha_mu if vertex.tracks.size()>1 else -1000
                        ,'truth_alpha_miss':vertex.truth_alpha_miss if vertex.tracks.size()>1 else -1000

                        ,'truth_Pt':(vertex.genie_interaction.protons.at(0)+vertex.genie_interaction.muon).Pt() if MCmode and vertex.genie_interaction.protons.size()>0 else -1000
                        
                        },
                        index=[0])
                        
                        
    stream_dataframe_to_file( data, outcsvname  )
# ---------------------------------------------------------------------------

