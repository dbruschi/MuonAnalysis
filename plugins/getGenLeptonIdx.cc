//implentation in the NanoAOD files of the GenPart collection and selection of the PreFSR generator leptons
//logic for the generator lepton selection included from here: https://github.com/emanca/wproperties-analysis/blob/master/nanotools/src/genLeptonSelector.cpp

#include "getGenLeptonIdx.h"

void getGenLeptonIdxandFill(const std::vector<reco::GenParticle>& genparticles, Float_t *GenPart_eta_, Float_t *GenPart_mass_, Float_t *GenPart_phi_, Float_t *GenPart_pt_, Int_t* GenPart_genPartIdxMother_, Int_t *GenPart_pdgId_, Int_t *GenPart_status_, Int_t *GenPart_statusFlags_, Int_t& GenPart_preFSRLepIdx1, Int_t& GenPart_preFSRLepIdx2, UInt_t& nGenPart_) {
    std::vector<std::pair<int, const reco::GenParticle&> > status746;
    std::vector<std::pair<int, const reco::GenParticle&> > other;
    unsigned int j=0;
    for (auto const& genparticle : genparticles) {
      GenPart_eta_[j]=genparticle.eta();
      GenPart_mass_[j]=genparticle.mass();
      GenPart_phi_[j]=genparticle.phi();
      GenPart_pt_[j]=genparticle.pt();
      GenPart_genPartIdxMother_[j]=(genparticle.numberOfMothers()>0) ? genparticle.motherRef(0).key():-1;
      GenPart_pdgId_[j]=genparticle.pdgId();
      GenPart_status_[j]=genparticle.status();
      GenPart_statusFlags_[j]=(genparticle.statusFlags()).flags_.to_ulong();
      if (genparticle.numberOfMothers()>0) { 
        int mompdgId = genparticle.mother()->pdgId();
        if (abs(genparticle.pdgId()) >= 11 && abs(genparticle.pdgId()) <= 16) {
          if (abs(mompdgId) == 23 || abs(mompdgId) == 24) {                       //
            if (genparticle.status() == 746) //status 746 is pre photos FSR in new powheg samples. so if they exist we want those.
              status746.push_back(std::make_pair(j, std::ref(genparticle)));
            else
              other.push_back(std::make_pair(j, std::ref(genparticle)));
          }
          else if (genparticle.status() == 23) //in madgraph5_aMC@NLO there are some events without a W/Z, those have status 23 leptons (tested on >1M events)
            other.push_back(std::make_pair(j, std::ref(genparticle)));
        }
      }
      j++;
    }
    nGenPart_=j;

    std::vector<std::pair<int, const reco::GenParticle&> > *pointer=&other;
    if (other.size() > 2) {
      std::vector<std::pair<int, const reco::GenParticle&> > otherTmp;
      for (auto const& gp : other) {
        if ((gp.second.statusFlags().flags_.to_ulong() >> 8) & 1) otherTmp.push_back(gp);
      }
      pointer = &otherTmp;
    }
    std::pair<int, int> prefsrlepidx(-1, -1);
    if (status746.size() == 2)
    {
      prefsrlepidx.first = status746[0].second.pt() > status746[1].second.pt() ? status746[0].first : status746[1].first;
      prefsrlepidx.second = status746[0].second.pt() > status746[1].second.pt() ? status746[1].first : status746[0].first;
    }
    else if (status746.size() == 1 || (*pointer).size() == 1)
    {
      prefsrlepidx.first = status746[0].second.pt() > (*pointer)[0].second.pt() ? status746[0].first : (*pointer)[0].first;
      prefsrlepidx.second = status746[0].second.pt() > (*pointer)[0].second.pt() ? (*pointer)[0].first : status746[0].first;
    }
    else if (status746.size() == 0 and (*pointer).size() == 2)
    {
      prefsrlepidx.first = (*pointer)[0].second.pt() > (*pointer)[1].second.pt() ? (*pointer)[0].first : (*pointer)[1].first;
      prefsrlepidx.second = (*pointer)[0].second.pt() > (*pointer)[1].second.pt() ? (*pointer)[1].first : (*pointer)[0].first;
    } 
    GenPart_preFSRLepIdx1=prefsrlepidx.first;
    GenPart_preFSRLepIdx2=prefsrlepidx.second;
}
