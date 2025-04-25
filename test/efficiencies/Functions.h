#include "ROOT/RDataFrame.hxx"
#include "ROOT/RVec.hxx"
#include "TLorentzVector.h"

using namespace ROOT;

RVec<Bool_t> hasTriggerMatch(RVec<Float_t> &Muon_eta, RVec<Float_t> &Muon_phi, RVec<Int_t> &TrigObj_id, RVec<Int_t> &TrigObj_filterBits, RVec<Float_t> &TrigObj_eta, RVec<Float_t> &TrigObj_phi);

int PostFSRIdx(const RVec<int> &pdgId, const RVec<int> &status, const RVec<int> &genPartIdxMother, const RVec<int> &statusFlag, const RVec<float> &pt, int Status);

RVec<float> goodgenvalue(RVec<float> &GenPart_pt, int &GenPart_postFSRLepIdx1, int &GenPart_postFSRLepIdx2, RVec<float> &GenPart_eta, RVec<float> &GenPart_phi, RVec<int> &GenPart_status, RVec<int> &GenPart_pdgId); 

RVec<int> goodgenidx(RVec<float> &GenPart_pt, int &GenPart_postFSRLepIdx1, int &GenPart_postFSRLepIdx2, RVec<float> &GenPart_eta, RVec<float> &GenPart_phi, RVec<int> &GenPart_status, RVec<int> &GenPart_pdgId);

bool cleaner(int idx, RVec<float> &Muon_standeta, RVec<float> &Muon_standphi, RVec<bool> &Muon_isStandalone);

//STANDALONE

RVec<float> goodmuonboolstand(RVec<float> &Muon_standeta, RVec<float> &Muon_standphi, RVec<bool> &Muon_isStandalone, RVec<float> &goodgeneta, RVec<float> &goodgenphi);

RVec<float> goodtrack(RVec<float> &value, RVec<float> &Track_eta, RVec<float> &Track_phi, RVec<float> &goodgeneta, RVec<float> &goodgenphi, RVec<float> &Track_chi2, RVec<int> Track_originalAlgo, RVec<int> Track_quality);

RVec<float> goodtrackreal(RVec<float> &value, RVec<float> &Track_eta, RVec<float> &Track_phi, RVec<float> &goodgeneta, RVec<float> &goodgenphi, RVec<float> &Track_chi2, RVec<int> Track_originalAlgo, RVec<int> Track_quality);

RVec<bool> isgoodtrack(RVec<float> &goodtracketa, RVec<float> &goodtrackphi, RVec<float> &Muon_standeta, RVec<float> &Muon_standphi, RVec<bool> &Muon_isStandalone, RVec<float> &Muon_standpt);

//TRACKING

RVec<float> goodmuonbooltrack(RVec<float> &goodgeneta, RVec<float> &goodgenphi, RVec<float> &Track_eta, RVec<float> &Track_phi, RVec<int> &Track_originalAlgo, RVec<unsigned int> &Track_quality, RVec<float> &Track_chi2);

RVec<float> goodmuon(RVec<float> &value, RVec<float> &goodgeneta, RVec<float> &goodgenphi, RVec<float> &Muon_standeta, RVec<float> &Muon_standphi, RVec<bool> &Muon_isStandalone, RVec<float> &Muon_standpt);

RVec<float> goodmuontnp(RVec<float> &value, RVec<float> &goodgeneta, RVec<float> &goodgenphi, RVec<float> &Muon_eta, RVec<float> &Muon_phi, RVec<bool> &Muon_isGlobal, RVec<float> &Muon_pt, RVec<float> &Muon_standpt, RVec<float> &Muon_standeta, RVec<float> &Muon_standphi, RVec<bool> &Muon_isStandalone);

RVec<float> goodmuonreal(RVec<float> &value, RVec<float> &goodgeneta, RVec<float> &goodgenphi, RVec<float> &Muon_standeta, RVec<float> &Muon_standphi, RVec<bool> &Muon_isStandalone);

RVec<bool> isgoodmuon(RVec<float> &Track_eta, RVec<float> &Track_phi, RVec<float> &goodmuoneta, RVec<float> &goodmuonphi, RVec<int> &Track_originalAlgo, RVec<unsigned int> &Track_quality, RVec<float> &Track_chi2);

//GLOBAL

RVec<bool> goodmuonboolglobal(RVec<float> &goodgeneta, RVec<float> &goodgenphi, RVec<float> &Muon_standeta, RVec<float> &Muon_standphi, RVec<bool> &Muon_isStandalone, RVec<bool> &Muon_isGlobal);

RVec<int> goodmuonidx(RVec<float> &goodgeneta, RVec<float> &goodgenphi, RVec<float> &Muon_standeta, RVec<float> &Muon_standphi, RVec<bool> &Muon_isStandalone, RVec<float> &Muon_standpt);

RVec<int> goodmuontnpidx(RVec<float> &goodgeneta, RVec<float> &goodgenphi, RVec<float> &Muon_eta, RVec<float> &Muon_phi, RVec<bool> &Muon_isGlobal, RVec<float> &Muon_pt, RVec<float> &Muon_standpt, RVec<float> &Muon_standeta, RVec<float> &Muon_standphi, RVec<bool> &Muon_isStandalone);

RVec<bool> goodstand(RVec<int> &goodmuonidx, RVec<bool> &Muon_isGlobal, RVec<float> &Muon_pt, RVec<float> &Muon_standpt);

//IDIP

RVec<bool> goodmuonboolidip(RVec<float> &goodgeneta, RVec<float> &goodgenphi, RVec<float> &Muon_eta, RVec<float> &Muon_phi, RVec<bool> &Muon_isStandalone, RVec<bool> &Muon_isGlobal, RVec<bool> &Muon_mediumId, RVec<float> &Muon_dxyBS, RVec<float> &Muon_pt, RVec<float> &Muon_standpt, RVec<float> &Muon_standeta, RVec<float> &Muon_standphi);

RVec<bool> goodglobal(RVec<int> &goodglobalidx, RVec<bool> &Muon_mediumId, RVec<float> &Muon_dxyBS);

//TRIGGERMATCH

RVec<bool> goodmuonbooltrigger(RVec<float> &goodgeneta, RVec<float> &goodgenphi, RVec<float> &Muon_eta, RVec<float> &Muon_phi, RVec<bool> &Muon_isStandalone, RVec<bool> &Muon_isGlobal, RVec<bool> &Muon_mediumId, RVec<float> &Muon_dxyBS, RVec<float> &Muon_pt, RVec<float> &Muon_standpt, RVec<float> &Muon_standeta, RVec<float> &Muon_standphi, RVec<bool> &isTriggered);

RVec<float> goodidip(RVec<float> &value, RVec<float> &goodgeneta, RVec<float> &goodgenphi, RVec<float> &Muon_eta, RVec<float> &Muon_phi, RVec<bool> &Muon_isGlobal, RVec<float> &Muon_pt, RVec<float> &Muon_standpt, RVec<float> &Muon_standeta, RVec<float> &Muon_standphi, RVec<bool> &Muon_isStandalone, RVec<bool> &Muon_mediumId, RVec<float> &Muon_dxyBS);

RVec<int> goodidipidx(RVec<float> &goodgeneta, RVec<float> &goodgenphi, RVec<float> &Muon_eta, RVec<float> &Muon_phi, RVec<bool> &Muon_isGlobal, RVec<float> &Muon_pt, RVec<float> &Muon_standpt, RVec<float> &Muon_standeta, RVec<float> &Muon_standphi, RVec<bool> &Muon_isStandalone, RVec<bool> &Muon_mediumId, RVec<float> &Muon_dxyBS);

//ISOLATION

RVec<bool> goodmuonboolisolation(RVec<float> &goodgeneta, RVec<float> &goodgenphi, RVec<float> &Muon_standeta, RVec<float> &Muon_standphi, RVec<bool> &Muon_isStandalone, RVec<bool> &Muon_isGlobal, RVec<bool> &Muon_mediumId, RVec<float> &Muon_dxyBS, RVec<bool> &Muon_triggered, RVec<float> &Muon_pfRelIso04_all);

RVec<float> goodtrigger(RVec<float> &value, RVec<float> &goodgeneta, RVec<float> &goodgenphi, RVec<float> &Muon_eta, RVec<float> &Muon_phi, RVec<bool> &Muon_isGlobal, RVec<float> &Muon_pt, RVec<float> &Muon_standpt, RVec<float> &Muon_standeta, RVec<float> &Muon_standphi, RVec<bool> &Muon_isStandalone, RVec<bool> &Muon_mediumId, RVec<float> &Muon_dxyBS, RVec<bool> &Muon_triggered);

RVec<int> goodtriggeridx(RVec<float> &goodgeneta, RVec<float> &goodgenphi, RVec<float> &Muon_eta, RVec<float> &Muon_phi, RVec<bool> &Muon_isGlobal, RVec<float> &Muon_pt, RVec<float> &Muon_standpt, RVec<float> &Muon_standeta, RVec<float> &Muon_standphi, RVec<bool> &Muon_isStandalone, RVec<bool> &Muon_mediumId, RVec<float> &Muon_dxyBS, RVec<bool> &Muon_triggered);

RVec<bool> isgoodtrigger(RVec<int> &goodtriggeridx, RVec<float> &Muon_pfRelIso04_all);

RVec<bool> isTrigIso(RVec<bool> &isTrig, RVec<float> &iso);
