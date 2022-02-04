#include "ROOT/RDataFrame.hxx"
#include "ROOT/RVec.hxx"
#include "TLorentzVector.h"

using namespace ROOT;

RVec<float> goodgenvalue(RVec<float> &GenPart_pt, int &GenPart_postFSRLepIdx1, int &GenPart_postFSRLepIdx2, RVec<float> &GenPart_eta, RVec<float> &GenPart_phi, RVec<int> &GenPart_status); 

RVec<int> goodgenidx(RVec<float> &GenPart_pt, int &GenPart_postFSRLepIdx1, int &GenPart_postFSRLepIdx2, RVec<float> &GenPart_eta, RVec<float> &GenPart_phi, RVec<int> &GenPart_status);

//STANDALONE

RVec<float> goodmuonboolstand(RVec<float> &Muon_standeta, RVec<float> &Muon_standphi, RVec<bool> &Muon_isStandalone, RVec<float> &goodgeneta, RVec<float> &goodgenphi);

RVec<float> goodtrack(RVec<float> &value, RVec<float> &Track_eta, RVec<float> &Track_phi, RVec<float> &goodgeneta, RVec<float> &goodgenphi, RVec<float> &Track_chi2, RVec<float> Track_originalAlgo, RVec<unsigned int> Track_quality);

RVec<float> goodtrackreal(RVec<float> &value, RVec<float> &Track_eta, RVec<float> &Track_phi, RVec<float> &goodgeneta, RVec<float> &goodgenphi, RVec<float> &Track_chi2, RVec<float> Track_originalAlgo, RVec<unsigned int> Track_quality);

RVec<bool> isgoodtrack(RVec<float> &goodtracketa, RVec<float> &goodtrackphi, RVec<float> &Muon_eta, RVec<float> &Muon_phi, RVec<bool> &Muon_isStandalone);

//TRACKING

RVec<float> goodmuonbooltrack(RVec<float> &goodgeneta, RVec<float> &goodgenphi, RVec<float> &Track_eta, RVec<float> &Track_phi, RVec<float> &Track_originalAlgo, RVec<unsigned int> &Track_quality, RVec<float> &Track_chi2);

RVec<float> goodmuon(RVec<float> &value, RVec<float> &goodgeneta, RVec<float> &goodgenphi, RVec<float> &Muon_standeta, RVec<float> &Muon_standphi, RVec<bool> &Muon_isStandalone);

RVec<float> goodmuonreal(RVec<float> &value, RVec<float> &goodgeneta, RVec<float> &goodgenphi, RVec<float> &Muon_standeta, RVec<float> &Muon_standphi, RVec<bool> &Muon_isStandalone);

RVec<bool> isgoodmuon(RVec<float> &Track_eta, RVec<float> &Track_phi, RVec<float> &goodmuoneta, RVec<float> &goodmuonphi, RVec<float> &Track_originalAlgo, RVec<unsigned int> &Track_quality, RVec<float> &Track_chi2);

//GLOBAL

RVec<bool> goodmuonboolglobal(RVec<float> &goodgeneta, RVec<float> &goodgenphi, RVec<float> &Muon_standeta, RVec<float> &Muon_standphi, RVec<bool> &Muon_isGlobal);

RVec<int> goodmuonidx(RVec<float> &goodgeneta, RVec<float> &goodgenphi, RVec<float> &Muon_standeta, RVec<float> &Muon_standphi, RVec<bool> &Muon_isStandalone);

RVec<bool> goodstand(RVec<int> &goodmuonidx, RVec<bool> &Muon_isGlobal);
