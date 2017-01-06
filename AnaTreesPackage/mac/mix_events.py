from anatrees_tools import *
'''
    usage:
    ----------
    python $AnalysisTreesAna/mac/mix_events.py -v1
    '''


data_name = 'BNB5e19_mu-p_maxp_pscore0.9_Dec2016'
data_filename = ana_files_path + '/BNB_ANA/Features_larlite_%s.csv'%data_name
mixd_filename = ana_files_path + '/BNB_ANA/Features_larlite_%s_mixed.csv'%data_name

data = pd.read_csv( data_filename )

print_filename( data_filename , "input file - data of muon+proton")
mixed_counter = 0
for i,row in data.iterrows():
    if i%100==0: print 'taking muon',i,',so far written',mixed_counter,'mixed muon+proton'
    for j,row in data.iterrows():
        if i is not j:
            mixed_counter += 1
            mixed = pd.DataFrame({
                                 'itrk_NuSelMuon':data.get_value(i,'itrk_NuSelMuon')
                                 ,'mu_P':data.get_value(i,'mu_P')
                                 ,'mu_theta':data.get_value(i,'mu_theta')
                                 ,'mu_phi':data.get_value(i,'mu_phi')
                                 ,'mu_start_x':data.get_value(i,'mu_start_x')
                                 ,'mu_start_y':data.get_value(i,'mu_start_y')
                                 ,'mu_start_z':data.get_value(i,'mu_start_z')
                                 ,'mu_end_x':data.get_value(i,'mu_end_x')
                                 ,'mu_end_y':data.get_value(i,'mu_end_y')
                                 ,'mu_end_z':data.get_value(i,'mu_end_z')
                                 ,'itrk_GBDTproton':data.get_value(j,'itrk_GBDTproton')
                                 ,'p_P':data.get_value(j,'p_P')
                                 ,'p_theta':data.get_value(j,'p_theta')
                                 ,'p_phi':data.get_value(j,'p_phi')
                                 ,'p_start_x':data.get_value(j,'p_start_x')
                                 ,'p_start_y':data.get_value(j,'p_start_y')
                                 ,'p_start_z':data.get_value(j,'p_start_z')
                                 ,'p_end_x':data.get_value(j,'p_end_x')
                                 ,'p_end_y':data.get_value(j,'p_end_y')
                                 ,'p_end_z':data.get_value(j,'p_end_z')
                                 }
                                 ,index=[mixed_counter])
            stream_dataframe_to_file( mixed , mixd_filename )

print_filename( mixd_filename , "output file - mixed muon+proton")
