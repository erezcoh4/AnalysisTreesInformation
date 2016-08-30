#ifndef IMPORTANTTOOLS_CXX
#define IMPORTANTTOOLS_CXX

#include "ImportantTools.h"


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
TTree* ImportantTools::SchemeTreeRSEList ( TTree * InTree , TString RSEfileName){
    
    std::map < int , std::map < int , int > > RSEMap = LoadRSEMap( RSEfileName , true );
    
    // scheme tree to contain only events from RSE list
    TTree* OutTree = InTree -> CloneTree(0);
    
    Int_t run , subrun , event;
    InTree -> SetBranchAddress("run"    , &run);
    InTree -> SetBranchAddress("subrun" , &subrun);
    InTree -> SetBranchAddress("event"  , &event);
    
    for (int i = 0; i < InTree->GetEntries() ; i++) {
        
        InTree->GetEntry(i);
        
        //Search run
        auto run_iter = RSEMap.find(run);
        if(run_iter == RSEMap.end()) {
            // std::cout << " run " << run << " not found..." << std::endl;
            continue;
        }
        // Grab run-map's value (i.e. subrun map)
        auto const& subrun_map = (*run_iter).second;
        //Search sub run
        auto subrun_iter = subrun_map.find(subrun);
        if(subrun_iter == subrun_map.end()) {
            // std::cout << " subrun " << subrun << " not found..." << std::endl;
            continue;
        }
        // Grab subrun-map's value (i.e. event number)
        auto const& event_number = (*subrun_iter).second;
        
        if (event_number == event){
            OutTree->Fill();
            Printf("filled output tree with run-%d subrun-%d event-%d",run,subrun,event);
        }
    }
    
    return OutTree;
}



//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
std::map < int , std::map < int , int > > ImportantTools::LoadRSEMap( TString RSEfileName, bool DoCheck ){
    
    std::map < int , std::map < int , int > > RSEMap;
    ifstream fin(RSEfileName);
    std::cout << "loading data from file \n" << RSEfileName << endl;
    
    Int_t run , subrun , event;
    // Read one line at a time.
    string line ;
    
    while ( getline(fin, line) ) {
        istringstream ss(line);
        ss >> run >> subrun >>  event ;
        RSEMap[run][subrun] = event;
    }
    // check
    if (DoCheck) {
        for(auto it : RSEMap) {
            std::cout << "run"  << it.first ;
            for(auto inner_it : it.second ) {
                std::cout  << ", subrun " << inner_it.first;
                std::cout  << ", event " << inner_it.second;
            }
            std::cout << std::endl;
        }
    }
    return RSEMap;
}




#endif
