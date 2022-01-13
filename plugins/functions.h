#include "DataFormats/HepMCCandidate/interface/GenParticle.h"

//implentation in the NanoAOD files of the GenPart collection and selection of the PreFSR generator leptons
//logic for the generator lepton selection included from here: https://github.com/emanca/wproperties-analysis/blob/master/nanotools/src/genLeptonSelector.cpp
void getGenLeptonIdxandFill(const std::vector<reco::GenParticle>& genparticles, Float_t *GenPart_eta_, Float_t *GenPart_mass_, Float_t *GenPart_phi_, Float_t *GenPart_pt_, Int_t* GenPart_genPartIdxMother_, Int_t *GenPart_pdgId_, Int_t *GenPart_status_, Int_t *GenPart_statusFlags_, Int_t& GenPart_preFSRLepIdx1, Int_t& GenPart_preFSRLepIdx2, Int_t& GenPart_postFSRLepIdx1_, Int_t& GenPart_postFSRLepIdx2_,UInt_t& nGenPart_, UInt_t& nGenPartPreFSR_, UInt_t& nGenMuonPreFSR_, UInt_t& nGenMuon746_, UInt_t& nGenPartPostFSR_, Int_t *GenPart_PostFSR, Int_t *GenPart_motherPdgId_, Int_t *GenPart_origIdx_);

bool distancesort(std::pair<Double_t,std::pair<UInt_t, Int_t> > i, std::pair<Double_t,std::pair<UInt_t, Int_t> > j);

void setPileUpEra(std::string& era, std::string& pumc, std::string& pudata, std::string& pudata_down, std::string& pudata_up);
