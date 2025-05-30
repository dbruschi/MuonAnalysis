#include "ROOT/RDataFrame.hxx"
#include "ROOT/RVec.hxx"
#include "TLorentzVector.h"
#include "Functions.h"
#include <map>
#include <cmath>

#define DISTANCE 0.3
#define TRACKDISTANCE 0.2

using namespace ROOT;

float deltaPhi(float phi1, float phi2)
{                                                        
  float result = phi1 - phi2;
  while (result > float(M_PI)) result -= float(2*M_PI);
  while (result <= -float(M_PI)) result += float(2*M_PI);
  return result;
}

float deltaR2(float eta1, float phi1, float eta2, float phi2)
{
  float deta = std::abs(eta1-eta2);
  float dphi = deltaPhi(phi1,phi2);
  return deta*deta + dphi*dphi;
}

float deltaR(float eta1, float phi1, float eta2, float phi2)
{
  return std::sqrt(deltaR2(eta1,phi1,eta2,phi2));
}

RVec<Bool_t> hasTriggerMatch(RVec<Float_t> &Muon_eta, RVec<Float_t> &Muon_phi, 
                             RVec<Int_t> &TrigObj_id, RVec<Int_t> &TrigObj_filterBits,
                             RVec<Float_t> &TrigObj_eta, RVec<Float_t> &TrigObj_phi)
{
  RVec<Bool_t> TriggerMatch;
  for (int iMuon = 0; iMuon<Muon_eta.size(); iMuon++ ){
    bool hasTrigMatch = false;
    for (unsigned int iTrig=0; iTrig<TrigObj_id.size(); ++iTrig){
      if (TrigObj_id[iTrig]  != 13 ) continue;
      if (! (( TrigObj_filterBits[iTrig] & 16) || (TrigObj_filterBits[iTrig] & 32) ) ) continue;
      if (deltaR(Muon_eta[iMuon], Muon_phi[iMuon], TrigObj_eta[iTrig], TrigObj_phi[iTrig]) < 0.3) {
          hasTrigMatch = true;
          break;
      }
    }
    TriggerMatch.push_back(hasTrigMatch);
  }
  return TriggerMatch;
}

bool genleptoncompare (std::pair<int, float> i, std::pair<int, float> j) { 
	return (i.second>j.second);
}

int PostFSRIdx(const RVec<int> &pdgId, const RVec<int> &status, const RVec<int> &genPartIdxMother, const RVec<int> &statusFlag, const RVec<float> &pt, int Status) {
	std::vector<int> postfsrmum;
	std::vector<int> postfsrother;

	int GenPart_postFSRLepIdx1_, GenPart_postFSRLepIdx2_;

	for (unsigned int i = 0; i < pdgId.size(); i++) {
		if (genPartIdxMother[i] < 0)
			continue;
		int mompdgId = pdgId[genPartIdxMother[i]];
		if (abs(pdgId[i]) < 11 || abs(pdgId[i]) > 16)
			continue;
		if (abs(mompdgId) == 23 || abs(mompdgId) == 24) {                       //
			if ((status[i]==1)||(status[i]==2))
				if (statusFlag[i] & 1)
					postfsrmum.push_back(i);
		}
		else {
			if ((status[i]==1)||(status[i]==2))
				if (statusFlag[i] & 1)
					postfsrother.push_back(i);
		}
	}

	std::vector<std::pair<int, float> > postfsr(0);
	for (unsigned int h=0; h!=postfsrmum.size(); h++) {
		postfsr.push_back({postfsrmum[h],pt[postfsrmum[h]]});
	}
	for (unsigned int h=0; h!=postfsrother.size(); h++) {
		postfsr.push_back({postfsrother[h],pt[postfsrother[h]]});
	}
	sort(postfsr.begin(),postfsr.begin()+postfsrmum.size(),genleptoncompare);
	sort(postfsr.begin()+postfsrmum.size(),postfsr.end(),genleptoncompare);

	if (postfsr.size()==0) {
		GenPart_postFSRLepIdx1_ = -1;
		GenPart_postFSRLepIdx2_ = -1;
	}
	else if (postfsr.size()==1) {
		GenPart_postFSRLepIdx1_ = (postfsr[0]).first;
		GenPart_postFSRLepIdx2_ = -1;
	}
	else {
		GenPart_postFSRLepIdx1_ = (postfsr[0]).second > (postfsr[1]).second ? (postfsr[0]).first : (postfsr[1]).first;
		GenPart_postFSRLepIdx2_ = (postfsr[0]).second > (postfsr[1]).second ? (postfsr[1]).first : (postfsr[0]).first;
	}
	if (Status==0) return GenPart_postFSRLepIdx1_;
	else if (Status==1) return GenPart_postFSRLepIdx2_;
	return -1;
}

RVec<float> goodgenvalue(RVec<float> &GenPart_pt, int &GenPart_postFSRLepIdx1, int &GenPart_postFSRLepIdx2, RVec<float> &GenPart_eta, RVec<float> &GenPart_phi, RVec<int> &GenPart_status, RVec<int> &GenPart_pdgId) {
	RVec<float> v;
	for (auto i=0U;i < GenPart_pt.size(); ++i) {
		if (((i==GenPart_postFSRLepIdx1)||(i==GenPart_postFSRLepIdx2))&&(abs(GenPart_pdgId[i])==13)) {
			bool condition=true;
			for (auto j=0U; j < GenPart_pt.size(); ++j) {
				if (i==j) continue;
				if (GenPart_status[j]!=1) continue;
				TLorentzVector cand1, cand2;
				cand1.SetPtEtaPhiM(3.,GenPart_eta[i],GenPart_phi[i],0.);
				cand2.SetPtEtaPhiM(3.,GenPart_eta[j],GenPart_phi[j],0.);
				if (cand1.DeltaR(cand2)<DISTANCE) condition=false;
			}
			if (condition) v.emplace_back(GenPart_pt[i]);
		}
	}
	return v;
}

RVec<int> goodgenidx(RVec<float> &GenPart_pt, int &GenPart_postFSRLepIdx1, int &GenPart_postFSRLepIdx2, RVec<float> &GenPart_eta, RVec<float> &GenPart_phi, RVec<int> &GenPart_status, RVec<int> &GenPart_pdgId) {
	RVec<int> v;
	for (auto i=0U;i < GenPart_pt.size(); ++i) {
		if (((i==GenPart_postFSRLepIdx1)||(i==GenPart_postFSRLepIdx2))&&(abs(GenPart_pdgId[i])==13)) {
			bool condition=true;
			for (auto j=0U; j < GenPart_pt.size(); ++j) {
				if (i==j) continue;
				if (GenPart_status[j]!=1) continue;
				TLorentzVector cand1, cand2;
				cand1.SetPtEtaPhiM(3.,GenPart_eta[i],GenPart_phi[i],0.);
				cand2.SetPtEtaPhiM(3.,GenPart_eta[j],GenPart_phi[j],0.);
				if (cand1.DeltaR(cand2)<DISTANCE) condition=false;
			}
			if (condition) v.emplace_back(i);
		}
	}
	return v;
}

bool cleaner(int idx, RVec<float> &Muon_standeta, RVec<float> &Muon_standphi, RVec<bool> &Muon_isStandalone) {
	for (auto i=0U; i < Muon_standeta.size(); i++) {
		if (i==idx) continue;
		if (!Muon_isStandalone[i]) continue;
		TLorentzVector cand, cand2;
		cand.SetPtEtaPhiM(5.,Muon_standeta[idx],Muon_standphi[idx],0.);
		cand2.SetPtEtaPhiM(5.,Muon_standeta[i],Muon_standphi[i],0.);
		if (cand.DeltaR(cand2)<0.1) return false;
	}
	return true;
}

//STANDALONE

RVec<float> goodmuonboolstand(RVec<float> &Muon_standeta, RVec<float> &Muon_standphi, RVec<bool> &Muon_isStandalone, RVec<float> &goodgeneta, RVec<float> &goodgenphi) {
	RVec<float> v;
	for (auto i=0U;i < goodgeneta.size(); ++i) {
		std::map<float,int> Map;
		for (auto j=0U; j < Muon_standeta.size(); ++j) {
			if (!Muon_isStandalone[j]) continue;
			TLorentzVector cand, cand2;
			cand.SetPtEtaPhiM(5.,goodgeneta[i],goodgenphi[i],0.);
			cand2.SetPtEtaPhiM(5.,Muon_standeta[j],Muon_standphi[j],0.);
			Map.insert({cand.DeltaR(cand2),j});
		}
		if ((Map.size()>0)&&(Map.begin()->first<DISTANCE)) {
			if (cleaner(Map.begin()->second,Muon_standeta,Muon_standphi,Muon_isStandalone)) v.emplace_back(1);
			else v.emplace_back(0);
		}
		else v.emplace_back(0); 
	}
	return v;
}

RVec<float> goodtrack(RVec<float> &value, RVec<float> &Track_eta, RVec<float> &Track_phi, RVec<float> &goodgeneta, RVec<float> &goodgenphi, RVec<float> &Track_chi2, RVec<int> Track_originalAlgo, RVec<int> Track_quality){
	RVec<float> v;
	for (auto j=0U; j < goodgeneta.size(); ++j) {
		std::map<float,int> Map;
		for (auto i=0U;i < Track_eta.size(); ++i) {
			if (!((Track_chi2[i]<5.)&&(Track_originalAlgo[i]!=13)&&(Track_originalAlgo[i]!=14))) continue;
			TLorentzVector cand, cand2;
			cand.SetPtEtaPhiM(5.,goodgeneta[j],goodgenphi[j],0.);
			cand2.SetPtEtaPhiM(5.,Track_eta[i],Track_phi[i],0.);
//			if ((cand.DeltaR(cand2)<0.2)&&(Track_chi2[i]<5.)&&(Track_quality[i]&4)&&(Track_originalAlgo[i]!=13)&&(Track_originalAlgo[i]!=14)) condition=true;
			Map.insert({cand.DeltaR(cand2),i});
		}
		if (Map.size()==0) continue;
		int idx=Map.begin()->second;
		if (Map.begin()->first<TRACKDISTANCE) v.emplace_back(value[j]);
	}
	return v;
}

RVec<float> goodtrackreal(RVec<float> &value, RVec<float> &Track_eta, RVec<float> &Track_phi, RVec<float> &goodgeneta, RVec<float> &goodgenphi, RVec<float> &Track_chi2, RVec<int> Track_originalAlgo, RVec<int> Track_quality){
	RVec<float> v;
	for (auto j=0U; j < goodgeneta.size(); ++j) {
		std::map<float,int> Map;
		for (auto i=0U;i < value.size(); ++i) {
			if (!((Track_chi2[i]<5.)&&(Track_originalAlgo[i]!=13)&&(Track_originalAlgo[i]!=14))) continue;
			TLorentzVector cand, cand2;
			cand.SetPtEtaPhiM(5.,goodgeneta[j],goodgenphi[j],0.);
			cand2.SetPtEtaPhiM(5.,Track_eta[i],Track_phi[i],0.);
//			if ((cand.DeltaR(cand2)<0.2)&&(Track_chi2[i]<5.)&&(Track_quality[i]&4)&&(Track_originalAlgo[i]!=13)&&(Track_originalAlgo[i]!=14)) condition=true;
			Map.insert({cand.DeltaR(cand2),i});
		}
		if (Map.size()==0) continue;
		int idx=Map.begin()->second;
		if (Map.begin()->first<TRACKDISTANCE) v.emplace_back(value[idx]);
	}
	return v;
}

RVec<bool> isgoodtrack(RVec<float> &goodtracketa, RVec<float> &goodtrackphi, RVec<float> &Muon_standeta, RVec<float> &Muon_standphi, RVec<bool> &Muon_isStandalone, RVec<float> &Muon_standpt) {
	RVec<bool> v;
	for (auto i=0U;i < goodtracketa.size(); ++i) {
		std::map<float,int> Map;
		for (auto j=0U; j < Muon_standeta.size(); ++j) {
			if (!Muon_isStandalone[j]) continue;
			if (Muon_standpt[j]<15.) continue;
			TLorentzVector cand, cand2;
			cand.SetPtEtaPhiM(5.,goodtracketa[i],goodtrackphi[i],0.);
			cand2.SetPtEtaPhiM(5.,Muon_standeta[j],Muon_standphi[j],0.);
			Map.insert({cand.DeltaR(cand2),j});
		}
		if ((Map.size()>0)&&(Map.begin()->first<DISTANCE)) {
			if (cleaner(Map.begin()->second,Muon_standeta,Muon_standphi,Muon_isStandalone)) v.emplace_back(true);
			else v.emplace_back(false);
		}
		else v.emplace_back(false);
	}
	return v;
}

//TRACKING

RVec<float> goodmuonbooltrack(RVec<float> &goodgeneta, RVec<float> &goodgenphi, RVec<float> &Track_eta, RVec<float> &Track_phi, RVec<int> &Track_originalAlgo, RVec<unsigned int> &Track_quality, RVec<float> &Track_chi2) {
	RVec<float> v;
	for (auto i=0U;i < goodgeneta.size(); ++i) {
		std::map<float,int> Map;
		for (auto j=0U; j < Track_eta.size(); ++j) {
			if (!((Track_chi2[j]<5.)&&(Track_originalAlgo[j]!=13)&&(Track_originalAlgo[j]!=14))) continue;
			TLorentzVector cand, cand2;
			cand.SetPtEtaPhiM(5.,goodgeneta[i],goodgenphi[i],0.);
			cand2.SetPtEtaPhiM(5.,Track_eta[j],Track_phi[j],0.);
			Map.insert({cand.DeltaR(cand2),j});
//			if ((cand.DeltaR(cand2)<0.3)&&(Track_chi2[j]<5.)&&(Track_quality[j]&4)&&(Track_originalAlgo[j]!=13)&&(Track_originalAlgo[j]!=14)) condition=true;
//			if ((cand.DeltaR(cand2)<0.1)&&(Track_chi2[j]<5.)) condition=true;
		}
		if ((Map.size()>0)&&(Map.begin()->first<DISTANCE)) v.emplace_back(1);
		else v.emplace_back(0); 
	}
	return v;
}

RVec<float> goodmuon(RVec<float> &value, RVec<float> &goodgeneta, RVec<float> &goodgenphi, RVec<float> &Muon_standeta, RVec<float> &Muon_standphi, RVec<bool> &Muon_isStandalone, RVec<float> &Muon_standpt){
	RVec<float> v;
	for (auto j=0U; j < goodgeneta.size(); ++j) {
		std::map<float,int> Map;
		for (auto i=0U;i < Muon_standeta.size(); ++i) {
			if (!Muon_isStandalone[i]) continue;
			if (Muon_standpt[i]<15.) continue;
			TLorentzVector cand, cand2;
			cand.SetPtEtaPhiM(5.,goodgeneta[j],goodgenphi[j],0.);
			cand2.SetPtEtaPhiM(5.,Muon_standeta[i],Muon_standphi[i],0.);
			Map.insert({cand.DeltaR(cand2),i});
		}
		if (Map.size()==0) continue;
		if (Map.begin()->first<DISTANCE) {
			if (cleaner(Map.begin()->second,Muon_standeta,Muon_standphi,Muon_isStandalone)) v.emplace_back(value[j]);
		}
	}
	return v;
}

RVec<float> goodmuontnp(RVec<float> &value, RVec<float> &goodgeneta, RVec<float> &goodgenphi, RVec<float> &Muon_eta, RVec<float> &Muon_phi, RVec<bool> &Muon_isGlobal, RVec<float> &Muon_pt, RVec<float> &Muon_standpt, RVec<float> &Muon_standeta, RVec<float> &Muon_standphi, RVec<bool> &Muon_isStandalone){
	RVec<float> v;
	for (auto j=0U; j < goodgeneta.size(); ++j) {
		std::map<float,int> Map;
		for (auto i=0U;i < Muon_eta.size(); ++i) {
			if (!Muon_isGlobal[i]) continue;
			if (Muon_standpt[i]<15.) continue;
			if (Muon_pt[i]<15.) continue;
			TLorentzVector cand, cand2, cand3;
			cand.SetPtEtaPhiM(5.,goodgeneta[j],goodgenphi[j],0.);
			cand2.SetPtEtaPhiM(5.,Muon_eta[i],Muon_phi[i],0.);
			cand3.SetPtEtaPhiM(5.,Muon_standeta[i],Muon_standphi[i],0.);
			if (cand2.DeltaR(cand3)>0.3) continue;
			Map.insert({cand.DeltaR(cand2),i});
		}
		if (Map.size()==0) continue;
		if (Map.begin()->first<DISTANCE) {
			if (cleaner(Map.begin()->second,Muon_standeta,Muon_standphi,Muon_isStandalone)) v.emplace_back(value[j]);
		}
	}
	return v;
}

RVec<float> goodmuonreal(RVec<float> &value, RVec<float> &goodgeneta, RVec<float> &goodgenphi, RVec<float> &Muon_standeta, RVec<float> &Muon_standphi, RVec<bool> &Muon_isStandalone){
	RVec<float> v;
	for (auto j=0U; j < goodgeneta.size(); ++j) {
		std::map<float,int> Map;
		for (auto i=0U;i < value.size(); ++i) {
			if (!Muon_isStandalone[i]) continue;
			TLorentzVector cand, cand2;
			cand.SetPtEtaPhiM(5.,goodgeneta[j],goodgenphi[j],0.);
			cand2.SetPtEtaPhiM(5.,Muon_standeta[i],Muon_standphi[i],0.);
			Map.insert({cand.DeltaR(cand2),i});
		}
		if (Map.size()==0) continue;
		if (Map.begin()->first<DISTANCE) {
			if (cleaner(Map.begin()->second,Muon_standeta,Muon_standphi,Muon_isStandalone)) v.emplace_back(value[Map.begin()->second]);
		}
	}
	return v;
}

RVec<bool> isgoodmuon(RVec<float> &Track_eta, RVec<float> &Track_phi, RVec<float> &goodmuoneta, RVec<float> &goodmuonphi, RVec<int> &Track_originalAlgo, RVec<unsigned int> &Track_quality, RVec<float> &Track_chi2) {
	RVec<bool> v;
	for (auto i=0U;i < goodmuoneta.size(); ++i) {
		std::map<float,int> Map;
		for (auto j=0U; j < Track_eta.size(); ++j) {
			if (!((Track_chi2[j]<5.)&&(Track_originalAlgo[j]!=13)&&(Track_originalAlgo[j]!=14))) continue;
			TLorentzVector cand, cand2;
			cand.SetPtEtaPhiM(5.,Track_eta[j],Track_phi[j],0.);
			cand2.SetPtEtaPhiM(5.,goodmuoneta[i],goodmuonphi[i],0.);
			Map.insert({cand.DeltaR(cand2),j});
//			if ((cand.DeltaR(cand2)<0.1)&&(Track_chi2[j]<5.)) condition=true;
		}
		if ((Map.size()>0)&&(Map.begin()->first<DISTANCE)) v.emplace_back(true);
		else v.emplace_back(false);
	}
	return v;
}

//GLOBAL

RVec<bool> goodmuonboolglobal(RVec<float> &goodgeneta, RVec<float> &goodgenphi, RVec<float> &Muon_standeta, RVec<float> &Muon_standphi, RVec<bool> &Muon_isStandalone, RVec<bool> &Muon_isGlobal) {
	RVec<bool> v;
	for (auto i=0U; i<goodgeneta.size(); i++) {
		std::map<float,int> Map;
		for (auto j=0U; j<Muon_standeta.size(); ++j) {
			//if (!Muon_isGlobal[j]) continue;
			TLorentzVector cand1, cand2;
			cand1.SetPtEtaPhiM(3.,goodgeneta[i],goodgenphi[i],0.);
			cand2.SetPtEtaPhiM(3.,Muon_standeta[j],Muon_standphi[j],0.);
			//if (Muon_pt[j]<15.) continue;
			Map.insert({cand1.DeltaR(cand2),j});
		}
		if ((Map.size()>0)&&(Map.begin()->first<DISTANCE)&&(Muon_isGlobal[Map.begin()->second])) {
			if (cleaner(Map.begin()->second,Muon_standeta,Muon_standphi,Muon_isStandalone)) v.emplace_back(1);
			else v.emplace_back(0);
		}
		else v.emplace_back(0); 
	}
	return v;
}

RVec<int> goodmuonidx(RVec<float> &goodgeneta, RVec<float> &goodgenphi, RVec<float> &Muon_standeta, RVec<float> &Muon_standphi, RVec<bool> &Muon_isStandalone, RVec<float> &Muon_standpt){
	RVec<int> v;
	for (auto j=0U; j < goodgeneta.size(); ++j) {
		std::map<float,int> Map;
		float Val;
		for (auto i=0U;i < Muon_isStandalone.size(); ++i) {
			if (!Muon_isStandalone[i]) continue;
			if (Muon_standpt[i]<15.) continue;
			TLorentzVector cand, cand2;
			cand.SetPtEtaPhiM(5.,goodgeneta[j],goodgenphi[j],0.);
			cand2.SetPtEtaPhiM(5.,Muon_standeta[i],Muon_standphi[i],0.);
			Map.insert({cand.DeltaR(cand2),i});
		}
		if (Map.size()==0) continue;
		if (Map.begin()->first<DISTANCE) {
			if (cleaner(Map.begin()->second,Muon_standeta,Muon_standphi,Muon_isStandalone)) v.emplace_back(Map.begin()->second);
		}
	}
	return v;
}

RVec<int> goodmuontnpidx(RVec<float> &goodgeneta, RVec<float> &goodgenphi, RVec<float> &Muon_eta, RVec<float> &Muon_phi, RVec<bool> &Muon_isGlobal, RVec<float> &Muon_pt, RVec<float> &Muon_standpt, RVec<float> &Muon_standeta, RVec<float> &Muon_standphi, RVec<bool> &Muon_isStandalone){
	RVec<int> v;
	for (auto j=0U; j < goodgeneta.size(); ++j) {
		std::map<float,int> Map;
		float Val;
		for (auto i=0U;i < Muon_isGlobal.size(); ++i) {
			if (!Muon_isGlobal[i]) continue;
			if (Muon_standpt[i]<15.) continue;
			if (Muon_pt[i]<15.) continue;
			TLorentzVector cand, cand2, cand3;
			cand.SetPtEtaPhiM(5.,goodgeneta[j],goodgenphi[j],0.);
			cand2.SetPtEtaPhiM(5.,Muon_eta[i],Muon_phi[i],0.);
			cand3.SetPtEtaPhiM(5.,Muon_standeta[i],Muon_standphi[i],0.);
			if (cand2.DeltaR(cand3)>0.3) continue;
			Map.insert({cand.DeltaR(cand2),i});
		}
		if (Map.size()==0) continue;
		if (Map.begin()->first<DISTANCE) {
			if (cleaner(Map.begin()->second,Muon_standeta,Muon_standphi,Muon_isStandalone)) v.emplace_back(Map.begin()->second);
		}
	}
	return v;
}

RVec<bool> goodstand(RVec<int> &goodmuonidx, RVec<bool> &Muon_isGlobal, RVec<float> &Muon_pt, RVec<float> &Muon_standpt) {
	RVec<bool> v;
	for (auto i=0U;i < goodmuonidx.size(); ++i) {
		if ((Muon_isGlobal[goodmuonidx[i]])&&(Muon_pt[goodmuonidx[i]]>15.)&&(Muon_standpt[goodmuonidx[i]]>15.)) v.emplace_back(true);
		else v.emplace_back(false);
	}
	return v;
}

//IDIP

RVec<bool> goodmuonboolidip(RVec<float> &goodgeneta, RVec<float> &goodgenphi, RVec<float> &Muon_eta, RVec<float> &Muon_phi, RVec<bool> &Muon_isStandalone, RVec<bool> &Muon_isGlobal, RVec<bool> &Muon_mediumId, RVec<float> &Muon_dxyBS, RVec<float> &Muon_pt, RVec<float> &Muon_standpt, RVec<float> &Muon_standeta, RVec<float> &Muon_standphi) {
	RVec<bool> v;
	for (auto i=0U; i<goodgeneta.size(); i++) {
		std::map<float,int> Map;
		for (auto j=0U; j<Muon_standeta.size(); ++j) {
			if (!Muon_isGlobal[j]) continue;
			TLorentzVector cand1, cand2, cand3;
			cand1.SetPtEtaPhiM(3.,goodgeneta[i],goodgenphi[i],0.);
			cand2.SetPtEtaPhiM(3.,Muon_eta[j],Muon_phi[j],0.);
			if (Muon_pt[j]<15.) continue;
			if (!Muon_mediumId[j]) continue;
			if (fabs(Muon_dxyBS[j])>0.05) continue;
			if (Muon_standpt[j]<15.) continue;
			cand3.SetPtEtaPhiM(3.,Muon_standeta[j], Muon_standphi[j], 0.);
			if (cand2.DeltaR(cand3)>0.3) continue;
			Map.insert({cand1.DeltaR(cand2),j});
		}
		if ((Map.size()>0)&&(Map.begin()->first<DISTANCE)) {
			if (cleaner(Map.begin()->second,Muon_standeta,Muon_standphi,Muon_isStandalone)) v.emplace_back(1);
			else v.emplace_back(0);
		}
		else v.emplace_back(0); 
	}
	return v;
}

RVec<bool> goodmuonbooltrigger(RVec<float> &goodgeneta, RVec<float> &goodgenphi, RVec<float> &Muon_eta, RVec<float> &Muon_phi, RVec<bool> &Muon_isStandalone, RVec<bool> &Muon_isGlobal, RVec<bool> &Muon_mediumId, RVec<float> &Muon_dxyBS, RVec<float> &Muon_pt, RVec<float> &Muon_standpt, RVec<float> &Muon_standeta, RVec<float> &Muon_standphi, RVec<bool> &isTriggered) {
	RVec<bool> v;
	for (auto i=0U; i<goodgeneta.size(); i++) {
		std::map<float,int> Map;
		for (auto j=0U; j<Muon_standeta.size(); ++j) {
			if (!Muon_isGlobal[j]) continue;
			TLorentzVector cand1, cand2, cand3;
			cand1.SetPtEtaPhiM(3.,goodgeneta[i],goodgenphi[i],0.);
			cand2.SetPtEtaPhiM(3.,Muon_eta[j],Muon_phi[j],0.);
			if (Muon_pt[j]<15.) continue;
			if (!Muon_mediumId[j]) continue;
			if (fabs(Muon_dxyBS[j])>0.05) continue;
			if (Muon_standpt[j]<15.) continue;
			cand3.SetPtEtaPhiM(3.,Muon_standeta[j], Muon_standphi[j], 0.);
			if (cand2.DeltaR(cand3)>0.3) continue;
			if (!isTriggered[j]) continue;
			Map.insert({cand1.DeltaR(cand2),j});
		}
		if ((Map.size()>0)&&(Map.begin()->first<DISTANCE)) {
			if (cleaner(Map.begin()->second,Muon_standeta,Muon_standphi,Muon_isStandalone)) v.emplace_back(1);
			else v.emplace_back(0);
		}
		else v.emplace_back(0); 
	}
	return v;
}

RVec<bool> goodglobal(RVec<int> &goodglobalidx, RVec<bool> &Muon_mediumId, RVec<float> &Muon_dxyBS) {
	RVec<bool> v;
	for (auto i=0U;i < goodglobalidx.size(); ++i) {
		if ((Muon_mediumId[goodglobalidx[i]])&&(fabs(Muon_dxyBS[goodglobalidx[i]])<0.05)) v.emplace_back(true);
		else v.emplace_back(false);
	}
	return v;
}

//TRIGGERMATCH

RVec<bool> goodmuonbooltrigger(RVec<float> &goodgeneta, RVec<float> &goodgenphi, RVec<float> &Muon_standeta, RVec<float> &Muon_standphi, RVec<bool> &Muon_isStandalone, RVec<bool> &Muon_isGlobal, RVec<bool> &Muon_mediumId, RVec<float> &Muon_dxyBS, RVec<bool> &Muon_triggered) {
	RVec<bool> v;
	for (auto i=0U; i<goodgeneta.size(); i++) {
		std::map<float,int> Map;
		for (auto j=0U; j<Muon_standeta.size(); ++j) {
			//if (!Muon_isGlobal[j]) continue;
			TLorentzVector cand1, cand2;
			cand1.SetPtEtaPhiM(3.,goodgeneta[i],goodgenphi[i],0.);
			cand2.SetPtEtaPhiM(3.,Muon_standeta[j],Muon_standphi[j],0.);
			//if (Muon_pt[j]<15.) continue;
			Map.insert({cand1.DeltaR(cand2),j});
		}
		if ((Map.size()>0)&&(Map.begin()->first<DISTANCE)&&(Muon_isGlobal[Map.begin()->second])&&(Muon_mediumId[Map.begin()->second])&&(fabs(Muon_dxyBS[Map.begin()->second])<0.05)&&(Muon_triggered[Map.begin()->second])) {
			if (cleaner(Map.begin()->second,Muon_standeta,Muon_standphi,Muon_isStandalone)) v.emplace_back(1);
			else v.emplace_back(0);
		}
		else v.emplace_back(0); 
	}
	return v;
}

RVec<float> goodidip(RVec<float> &value, RVec<float> &goodgeneta, RVec<float> &goodgenphi, RVec<float> &Muon_eta, RVec<float> &Muon_phi, RVec<bool> &Muon_isGlobal, RVec<float> &Muon_pt, RVec<float> &Muon_standpt, RVec<float> &Muon_standeta, RVec<float> &Muon_standphi, RVec<bool> &Muon_isStandalone, RVec<bool> &Muon_mediumId, RVec<float> &Muon_dxyBS){
	RVec<float> v;
	for (auto j=0U; j < goodgeneta.size(); ++j) {
		std::map<float,int> Map;
		for (auto i=0U;i < Muon_eta.size(); ++i) {
			if (!Muon_isGlobal[i]) continue;
			if (Muon_standpt[i]<15.) continue;
			if (Muon_pt[i]<15.) continue;
			TLorentzVector cand, cand2, cand3;
			cand.SetPtEtaPhiM(5.,goodgeneta[j],goodgenphi[j],0.);
			cand2.SetPtEtaPhiM(5.,Muon_eta[i],Muon_phi[i],0.);
			cand3.SetPtEtaPhiM(5.,Muon_standeta[i],Muon_standphi[i],0.);
			if (cand2.DeltaR(cand3)>0.3) continue;
			if (!Muon_mediumId[i]) continue;
			if (fabs(Muon_dxyBS[i])>0.05) continue;
			Map.insert({cand.DeltaR(cand2),i});
		}
		if (Map.size()==0) continue;
		if (Map.begin()->first<DISTANCE) {
			if (cleaner(Map.begin()->second,Muon_standeta,Muon_standphi,Muon_isStandalone)) v.emplace_back(value[j]);
		}
	}
	return v;
}

RVec<int> goodidipidx(RVec<float> &goodgeneta, RVec<float> &goodgenphi, RVec<float> &Muon_eta, RVec<float> &Muon_phi, RVec<bool> &Muon_isGlobal, RVec<float> &Muon_pt, RVec<float> &Muon_standpt, RVec<float> &Muon_standeta, RVec<float> &Muon_standphi, RVec<bool> &Muon_isStandalone, RVec<bool> &Muon_mediumId, RVec<float> &Muon_dxyBS){
	RVec<int> v;
	for (auto j=0U; j < goodgeneta.size(); ++j) {
		std::map<float,int> Map;
		float Val;
		for (auto i=0U;i < Muon_isGlobal.size(); ++i) {
			if (!Muon_isGlobal[i]) continue;
			if (Muon_standpt[i]<15.) continue;
			if (Muon_pt[i]<15.) continue;
			TLorentzVector cand, cand2, cand3;
			cand.SetPtEtaPhiM(5.,goodgeneta[j],goodgenphi[j],0.);
			cand2.SetPtEtaPhiM(5.,Muon_eta[i],Muon_phi[i],0.);
			cand3.SetPtEtaPhiM(5.,Muon_standeta[i],Muon_standphi[i],0.);
			if (cand2.DeltaR(cand3)>0.3) continue;
			if (!Muon_mediumId[i]) continue;
			if (fabs(Muon_dxyBS[i])>0.05) continue;
			Map.insert({cand.DeltaR(cand2),i});
		}
		if (Map.size()==0) continue;
		if (Map.begin()->first<DISTANCE) {
			if (cleaner(Map.begin()->second,Muon_standeta,Muon_standphi,Muon_isStandalone)) v.emplace_back(Map.begin()->second);
		}
	}
	return v;
}

//ISOLATION

RVec<bool> goodmuonboolisolation(RVec<float> &goodgeneta, RVec<float> &goodgenphi, RVec<float> &Muon_standeta, RVec<float> &Muon_standphi, RVec<bool> &Muon_isStandalone, RVec<bool> &Muon_isGlobal, RVec<bool> &Muon_mediumId, RVec<float> &Muon_dxyBS, RVec<bool> &Muon_triggered, RVec<float> &Muon_pfRelIso04_all) {
	RVec<bool> v;
	for (auto i=0U; i<goodgeneta.size(); i++) {
		std::map<float,int> Map;
		for (auto j=0U; j<Muon_standeta.size(); ++j) {
			//if (!Muon_isGlobal[j]) continue;
			TLorentzVector cand1, cand2;
			cand1.SetPtEtaPhiM(3.,goodgeneta[i],goodgenphi[i],0.);
			cand2.SetPtEtaPhiM(3.,Muon_standeta[j],Muon_standphi[j],0.);
			//if (Muon_pt[j]<15.) continue;
			Map.insert({cand1.DeltaR(cand2),j});
		}
		if ((Map.size()>0)&&(Map.begin()->first<DISTANCE)&&(Muon_isGlobal[Map.begin()->second])&&(Muon_mediumId[Map.begin()->second])&&(fabs(Muon_dxyBS[Map.begin()->second])<0.05)&&(Muon_triggered[Map.begin()->second])&&(Muon_pfRelIso04_all[Map.begin()->second]<0.15)) {if (cleaner(Map.begin()->second,Muon_standeta,Muon_standphi,Muon_isStandalone)) v.emplace_back(1);
			else v.emplace_back(0);
		}
		else v.emplace_back(0); 
	}
	return v;
}

RVec<float> goodtrigger(RVec<float> &value, RVec<float> &goodgeneta, RVec<float> &goodgenphi, RVec<float> &Muon_eta, RVec<float> &Muon_phi, RVec<bool> &Muon_isGlobal, RVec<float> &Muon_pt, RVec<float> &Muon_standpt, RVec<float> &Muon_standeta, RVec<float> &Muon_standphi, RVec<bool> &Muon_isStandalone, RVec<bool> &Muon_mediumId, RVec<float> &Muon_dxyBS, RVec<bool> &Muon_triggered){
	RVec<float> v;
	for (auto j=0U; j < goodgeneta.size(); ++j) {
		std::map<float,int> Map;
		for (auto i=0U;i < Muon_eta.size(); ++i) {
			if (!Muon_isGlobal[i]) continue;
			if (Muon_standpt[i]<15.) continue;
			if (Muon_pt[i]<15.) continue;
			TLorentzVector cand, cand2, cand3;
			cand.SetPtEtaPhiM(5.,goodgeneta[j],goodgenphi[j],0.);
			cand2.SetPtEtaPhiM(5.,Muon_eta[i],Muon_phi[i],0.);
			cand3.SetPtEtaPhiM(5.,Muon_standeta[i],Muon_standphi[i],0.);
			if (cand2.DeltaR(cand3)>0.3) continue;
			if (!Muon_mediumId[i]) continue;
			if (fabs(Muon_dxyBS[i])>0.05) continue;
			if (!Muon_triggered[i]) continue;
			Map.insert({cand.DeltaR(cand2),i});
		}
		if (Map.size()==0) continue;
		if (Map.begin()->first<DISTANCE) {
			if (cleaner(Map.begin()->second,Muon_standeta,Muon_standphi,Muon_isStandalone)) v.emplace_back(value[j]);
		}
	}
	return v;
}

RVec<int> goodtriggeridx(RVec<float> &goodgeneta, RVec<float> &goodgenphi, RVec<float> &Muon_eta, RVec<float> &Muon_phi, RVec<bool> &Muon_isGlobal, RVec<float> &Muon_pt, RVec<float> &Muon_standpt, RVec<float> &Muon_standeta, RVec<float> &Muon_standphi, RVec<bool> &Muon_isStandalone, RVec<bool> &Muon_mediumId, RVec<float> &Muon_dxyBS, RVec<bool> &Muon_triggered){
	RVec<int> v;
	for (auto j=0U; j < goodgeneta.size(); ++j) {
		std::map<float,int> Map;
		float Val;
		for (auto i=0U;i < Muon_isGlobal.size(); ++i) {
			if (!Muon_isGlobal[i]) continue;
			if (Muon_standpt[i]<15.) continue;
			if (Muon_pt[i]<15.) continue;
			TLorentzVector cand, cand2, cand3;
			cand.SetPtEtaPhiM(5.,goodgeneta[j],goodgenphi[j],0.);
			cand2.SetPtEtaPhiM(5.,Muon_eta[i],Muon_phi[i],0.);
			cand3.SetPtEtaPhiM(5.,Muon_standeta[i],Muon_standphi[i],0.);
			if (cand2.DeltaR(cand3)>0.3) continue;
			if (!Muon_mediumId[i]) continue;
			if (fabs(Muon_dxyBS[i])>0.05) continue;
			if (!Muon_triggered[i]) continue;
			Map.insert({cand.DeltaR(cand2),i});
		}
		if (Map.size()==0) continue;
		if (Map.begin()->first<DISTANCE) {
			if (cleaner(Map.begin()->second,Muon_standeta,Muon_standphi,Muon_isStandalone)) v.emplace_back(Map.begin()->second);
		}
	}
	return v;
}

RVec<bool> isgoodtrigger(RVec<int> &goodtriggeridx, RVec<float> &Muon_pfRelIso04_all) {
	RVec<bool> v;
	for (auto i=0U;i < goodtriggeridx.size(); ++i) {
		if (Muon_pfRelIso04_all[goodtriggeridx[i]]<0.15) v.emplace_back(true);
		else v.emplace_back(false);
	}
	return v;
}

RVec<bool> isTrigIso(RVec<bool> &isTrig, RVec<float> &iso) {
	RVec<bool> v;
	for (auto i=0U; i<isTrig.size(); i++) {
		v.emplace_back((isTrig[i])&&(iso[i]<0.15));
	}
	return v;
}
