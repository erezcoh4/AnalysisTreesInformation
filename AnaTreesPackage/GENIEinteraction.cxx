#ifndef GENIEinteraction_CXX
#define GENIEinteraction_CXX

#include "GENIEinteraction.h"
#include "LArUtil/GeometryHelper.h"


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
GENIEinteraction::GENIEinteraction():
Xb(-100),
Q2(-100),
ccnc(-100)
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
GENIEinteraction::GENIEinteraction( Int_t fmcevent_id, Int_t fNprimaries ){
    // main vertex properties
    mcevent_id = fmcevent_id;
    Nprimaries = fNprimaries;
    
    muonTrackReconstructed = protonTrackReconstructed = false;
    Np = Nn = Npi = Nmu = Nel = Ntot = Nnu = Ngamma = 0;
    ccnc = -100;
    IsCC1p = false;
}


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void GENIEinteraction::SetNuMomentum( Float_t magnitude , Float_t dcosx , Float_t dcosy , Float_t dcosz ){
    nu.SetVectMag( TVector3( magnitude*dcosx , magnitude*dcosy , magnitude*dcosz ) , 0 );
}


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
GENIEinteraction::~GENIEinteraction(){ }

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
bool GENIEinteraction::AddPrimary ( // GENIE information is for outside of the nucleus (after the reaction)
                                   Int_t fpdg
                                   ,Float_t fEng
                                   ,Float_t fPx
                                   ,Float_t fPy
                                   ,Float_t fPz
                                   ,Float_t fP
                                   ,Int_t fstatus_code
                                   ,Float_t fmass
                                   ,Int_t fND
                                   ,Int_t fmother
                                   ,Int_t track_reconstructed
                                   ,Int_t ftrackID
                                   ,PandoraNuTrack fprimarPandoraNuTrack
                                   ){
    
    pdg.push_back(fpdg);
    Eng.push_back(fEng);
    ND.push_back(fND);
    mother.push_back(fmother);
    status_code.push_back(fstatus_code);
    
    momentum.SetVectM( TVector3 ( fPx , fPy , fPz ) , fmass );
    
    if (status_code.back()==1) { // status code 0 particles are unstable or do not exit the nucleus and are thus irrelevant
        
        Ntot++;
        if (track_reconstructed){
            tracks.push_back(fprimarPandoraNuTrack);
            trackID.push_back(ftrackID);
        }
        
        switch (pdg.back()) {
                
            case 14: // ν
                nu = momentum;
                Nnu ++;
                break;
                
            case 13: // µ
                muon = momentum;
                muonTrack = fprimarPandoraNuTrack;
                muonTrackReconstructed = track_reconstructed;
                Nmu++;
                break;
                
                
            case 2212: // p
                p3vect.push_back( momentum.Vect() ) ;
                protonTracks.push_back(fprimarPandoraNuTrack);
                protonTrack = fprimarPandoraNuTrack; // for CC1p, there is only one proton track, protonTrack.
                protonTrackReconstructed = track_reconstructed;
                Np++;
                break;
                
                
            case 2112: // n
                n3vect.push_back( momentum.Vect() ) ;
                Nn++;
                break;
                
            case 211: // π+
            case -211: // π-
            case 111: // π0
                Npi++;
                break;
                
            case 11: // e-
            case -11: // e+
                Nel++;
                break;

            case 22: // photon
                Ngamma++;
                break;

                
            default:
                break;
        }
    }

    
    
    return true;
}


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
bool GENIEinteraction::FindCC1p(){
    
    if ( ccnc==0 && Nmu==1 && Np==1 && Npi==0 && Nn==0 && Nel==0 && Ngamma==0 ){
        IsCC1p = true;
        return true;
    }
    IsCC1p = false;
    return false;
}



//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
bool GENIEinteraction::SortNucleons(){
    

    for (auto i: sort_by_momentum_magnitude( p3vect )){
        protons.push_back( TLorentzVector( p3vect.at(i) , sqrt( p3vect.at(i).Mag2() + 0.938*0.938 ) ) );
    }
    
    for (auto i: sort_by_momentum_magnitude( n3vect )){
        protons.push_back( TLorentzVector( n3vect.at(i) , sqrt( n3vect.at(i).Mag2() + 0.939*0.939 ) ) );
    }

}


//....oooOO0OOooo........oooOO0OOooo........oooOO0OåOooo........oooOO0OOooo......
vector<size_t> GENIEinteraction::sort_by_momentum_magnitude(const vector<TVector3> &v) {
    std::vector<size_t> idx(v.size());
    for (size_t i = 0; i != idx.size(); ++i) idx[i] = i;
    std::sort(idx.begin(), idx.end(), [&v](size_t i1, size_t i2) {return v[i1].Mag() > v[i2].Mag();});
    return idx;
}


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
bool GENIEinteraction::ComputeKinematics(){
    
    q  = nu - muon;
    Q2 = -q.Mag2();
    Xb = Q2 / ( 2 * 0.938 * q.E() );
    
}


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
bool GENIEinteraction::ComputePmissPrec(){
    
    //    SHOW(Np);
    //    SHOW(protons.size());
    if(Np>0){
        
        Plead = protons.at(0);
        
        // The leading proton was a neutron before the reaction!
        n_miss = Plead - q;
        
        for (auto proton: protons) {
            Pcm += proton;
        }
        Pcm  -= q;
        Prec  = Pcm - n_miss;
     
        
        // for SRC
        theta_pq    = (Plead.P()>0 && q.P()>0) ? TMath::RadToDeg() * Plead.Vect().Angle(q.Vect()) : -9999;
        p_over_q    = Plead.P()/q.P();
        
        // A(ν,𝝁p) missing mass M²(miss) = (q + (Mp+Mn) - Plead)² , all 4-vectors
        Mmiss       = (q + TLorentzVector( TVector3() , 0.938+0.939 ) - Plead).Mag();

    }
}




//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void GENIEinteraction::Print(bool DoPrintTracks){

    cout << "\033[31m" << "GENIE interaction " << mcevent_id << "\n~~~~~~~~~~~~~~~~~~~~~ "<< "\033[0m" << endl;
    SHOW(mcevent_id);
    SHOWTLorentzVector(nu);
    SHOWTLorentzVector(muon);
    SHOWTLorentzVector(q);
    SHOWvectorTLorentzVector(protons);
    SHOWvectorTLorentzVector(neutrons);
    SHOW(Xb);
    SHOW(Q2);
    SHOW3(theta_pq,p_over_q,Mmiss);
    
    
    SHOW( Nprimaries );
    SHOW( Np );
    SHOW2 ( Nn , Npi );
    SHOW3( Nmu , Nel , Ngamma );
    SHOW3( ccnc , IsCC1p , IsVertexContained );
    if (IsCC1p){
        SHOW2( muonTrackReconstructed, protonTrackReconstructed );
    }

    if(DoPrintTracks && !tracks.empty()){
        cout << "\033[33m" << "--------------\n" << tracks.size() << " pandoraNu tracks in GENIE interaction " << mcevent_id << "\n--------------" <<  "\033[37m" << endl;
        SHOWstdVector( trackID );
        for (auto t: tracks) {
            t.Print();
        }
    }


}


#endif
