#include <utility>
#include "ROOT/RDataFrame.hxx"
#include "ROOT/RVec.hxx"
#include "TLorentzVector.h"

using ROOT::VecOps::RVec;

RVec<float> goodgenvalue(RVec<float> &GenPart_pt, int &GenPart_postFSRLepIdx1, int &GenPart_postFSRLepIdx2, RVec<float> &GenPart_eta, RVec<float> &GenPart_phi, RVec<int> &GenPart_status, RVec<int> &GenPart_pdgId);

RVec<int> goodgenidx(RVec<float> &GenPart_pt, int &GenPart_postFSRLepIdx1, int &GenPart_postFSRLepIdx2, RVec<float> &GenPart_eta, RVec<float> &GenPart_phi, RVec<int> &GenPart_status, RVec<int> &GenPart_pdgId);

RVec<int> matchedtrackidx(RVec<float> &goodgeneta, RVec<float> &goodgenphi, RVec<int> &goodgenidx, RVec<float> &Track_eta, RVec<float> &Track_phi, RVec<float> &Track_chi2, RVec<int> &Track_originalAlgo);

RVec<int> matchedstaidx(RVec<float> &goodgeneta, RVec<float> &goodgenphi, RVec<int> &goodgenidx, RVec<float> &Track_eta, RVec<float> &Track_phi, RVec<bool> &Muon_isStandalone);

bool cleaner(int idx, RVec<float> &Muon_standeta, RVec<float> &Muon_standphi, RVec<bool> &Muon_isStandalone);

RVec<int> maketnppairs(RVec<float> &Muon_pt, RVec<float> &Muon_eta, RVec<float> &Muon_phi, RVec<float> &Muon_mass, RVec<float> &Muon_standeta, RVec<float> &Muon_standphi, RVec<bool> &Muon_isStandalone, RVec<bool> &Muon_isGlobal, RVec<bool> &Muon_mediumId, RVec<float> &Muon_dxyBS, RVec<bool> &Muon_triggered, RVec<float> &Muon_pfRelIso04_all, int option);

RVec<int> maketnppairstrack(RVec<float> &Muon_pt, RVec<float> &Muon_eta, RVec<float> &Muon_phi, RVec<float> &Muon_mass, RVec<float> &Muon_standeta, RVec<float> &Muon_standphi, RVec<bool> &Muon_isStandalone, RVec<bool> &Muon_isGlobal, RVec<bool> &Muon_mediumId, RVec<float> &Muon_dxyBS, RVec<bool> &Muon_triggered, RVec<float> &Muon_pfRelIso04_all, RVec<float> &Track_pt, RVec<float> &Track_eta, RVec<float> &Track_phi, RVec<float> &Track_chi2, RVec<int> &Track_originalAlgo, int option);

RVec<int> goodtnppairs(RVec<int> &tnppairtag, RVec<int> &tnppairprobe, RVec<int> &tnppairflag, RVec<int> &Muon_genPartIdx, RVec<int> &matchedtrackidx, RVec<int> &goodgenidx, int option);

RVec<float> invariantmasses(RVec<int> &maketnppairstag, RVec<int> &maketnppairsprobe, RVec<float> &pt1, RVec<float> &eta1, RVec<float> &phi1, RVec<float> &pt2, RVec<float> &eta2, RVec<float> &phi2, RVec<float> &Muon_mass);

//STANDALONE

RVec<float> goodmuonboolstand(RVec<float> &Muon_standeta, RVec<float> &Muon_standphi, RVec<bool> &Muon_isStandalone, RVec<float> &goodgeneta, RVec<float> &goodgenphi);

RVec<float> goodtrackgen(RVec<int> &goodtnppairtag, RVec<int> &goodtnppairprobe, RVec<int> &matchedtrackidx, RVec<int> &goodgenidx, RVec<float> &goodgenpt);

RVec<float> goodtrack(RVec<int> &goodtnppairtag, RVec<int> &goodtnppairprobe, RVec<float> &Track_eta);

RVec<bool> isgoodtrack(RVec<float> &goodtracketa, RVec<float> &goodtrackphi, RVec<float> &Muon_standeta, RVec<float> &Muon_standphi, RVec<bool> &Muon_isStandalone);

//GLOBAL

RVec<bool> goodmuonboolglobal(RVec<float> &goodgeneta, RVec<float> &goodgenphi, RVec<int> &goodmuonidx, RVec<int> &Muon_genPartIdx, RVec<bool> &Muon_isGlobal, RVec<float> &Muon_standeta, RVec<float> &Muon_standphi, RVec<float> &Muon_eta, RVec<float> &Muon_phi, RVec<bool> &Muon_isStandalone);

RVec<float> goodmuonreal(RVec<float> &value, RVec<int> &goodtnpprobereco);

RVec<bool> isglobal(RVec<int> &goodtnpflagreco, int bit);

//IDIP

RVec<bool> goodmuonboolidip(RVec<int> &goodgenidx, RVec<int> &Muon_genPartIdx, RVec<float> &Muon_eta, RVec<float> &Muon_phi, RVec<float> &Muon_standeta, RVec<float> &Muon_standphi, RVec<bool> &Muon_isStandalone, RVec<bool> &Muon_isGlobal, RVec<bool> &Muon_mediumId, RVec<float> &Muon_dxyBS);

RVec<bool> goodmuonboolid(RVec<int> &goodgenidx, RVec<int> &Muon_genPartIdx, RVec<float> &Muon_eta, RVec<float> &Muon_phi, RVec<float> &Muon_standeta, RVec<float> &Muon_standphi, RVec<bool> &Muon_isStandalone, RVec<bool> &Muon_isGlobal, RVec<bool> &Muon_mediumId, RVec<float> &Muon_dxyBS);

RVec<bool> goodmuonboolip(RVec<int> &goodgenidx, RVec<int> &Muon_genPartIdx, RVec<float> &Muon_eta, RVec<float> &Muon_phi, RVec<float> &Muon_standeta, RVec<float> &Muon_standphi, RVec<bool> &Muon_isStandalone, RVec<bool> &Muon_isGlobal, RVec<bool> &Muon_mediumId, RVec<float> &Muon_dxyBS);

//TRIGGERMATCH

RVec<bool> goodmuonbooltrigger(RVec<int> &goodgenidx, RVec<int> &Muon_genPartIdx, RVec<float> &Muon_eta, RVec<float> &Muon_phi, RVec<float> &Muon_standeta, RVec<float> &Muon_standphi, RVec<bool> &Muon_isStandalone, RVec<bool> &Muon_isGlobal, RVec<bool> &Muon_mediumId, RVec<float> &Muon_dxyBS, RVec<bool> &Muon_triggered);

//ISOLATION

RVec<bool> goodmuonboolisolation(RVec<int> &goodgenidx, RVec<int> &Muon_genPartIdx, RVec<float> &Muon_eta, RVec<float> &Muon_phi, RVec<float> &Muon_standeta, RVec<float> &Muon_standphi, RVec<bool> &Muon_isStandalone, RVec<bool> &Muon_isGlobal, RVec<bool> &Muon_mediumId, RVec<float> &Muon_dxyBS, RVec<bool> &Muon_triggered, RVec<float> &Muon_pfRelIso04_all);

//ISOLATION WITHOUT TRIGGER

RVec<bool> goodmuonboolisolationnotrig(RVec<int> &goodgenidx, RVec<int> &Muon_genPartIdx, RVec<float> &Muon_eta, RVec<float> &Muon_phi, RVec<float> &Muon_standeta, RVec<float> &Muon_standphi, RVec<bool> &Muon_isStandalone, RVec<bool> &Muon_isGlobal, RVec<bool> &Muon_mediumId, RVec<float> &Muon_dxyBS, RVec<bool> &Muon_triggered, RVec<float> &Muon_pfRelIso04_all);
