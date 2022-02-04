#include "ROOT/RDataFrame.hxx"
#include "ROOT/RVec.hxx"
#include "TLorentzVector.h"
#include "Functions.h"
#include <map>

using namespace ROOT;

RVec<float> goodgenvalue(RVec<float> &GenPart_pt, int &GenPart_postFSRLepIdx1, int &GenPart_postFSRLepIdx2, RVec<float> &GenPart_eta, RVec<float> &GenPart_phi, RVec<int> &GenPart_status) {
	RVec<float> v;
	for (auto i=0U;i < GenPart_pt.size(); ++i) {
		if ((i==GenPart_postFSRLepIdx1)||(i==GenPart_postFSRLepIdx2)) {
			bool condition=true;
			for (auto j=0U; j < GenPart_pt.size(); ++j) {
				if (i==j) continue;
				if (GenPart_status[j]!=1) continue;
				TLorentzVector cand1, cand2;
				cand1.SetPtEtaPhiM(3.,GenPart_eta[i],GenPart_phi[i],0.);
				cand2.SetPtEtaPhiM(3.,GenPart_eta[j],GenPart_phi[j],0.);
				if (cand1.DeltaR(cand2)<0.3) condition=false;
			}
			if (condition) v.emplace_back(GenPart_pt[i]);
		}
	}
	return v;
}

RVec<int> goodgenidx(RVec<float> &GenPart_pt, int &GenPart_postFSRLepIdx1, int &GenPart_postFSRLepIdx2, RVec<float> &GenPart_eta, RVec<float> &GenPart_phi, RVec<int> &GenPart_status) {
	RVec<int> v;
	for (auto i=0U;i < GenPart_pt.size(); ++i) {
		if ((i==GenPart_postFSRLepIdx1)||(i==GenPart_postFSRLepIdx2)) {
			bool condition=true;
			for (auto j=0U; j < GenPart_pt.size(); ++j) {
				if (i==j) continue;
				if (GenPart_status[j]!=1) continue;
				TLorentzVector cand1, cand2;
				cand1.SetPtEtaPhiM(3.,GenPart_eta[i],GenPart_phi[i],0.);
				cand2.SetPtEtaPhiM(3.,GenPart_eta[j],GenPart_phi[j],0.);
				if (cand1.DeltaR(cand2)<0.3) condition=false;
			}
			if (condition) v.emplace_back(i);
		}
	}
	return v;
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
		if ((Map.size()>0)&&(Map.begin()->first<0.1)) v.emplace_back(1);
		else v.emplace_back(0); 
	}
	return v;
}

RVec<float> goodtrack(RVec<float> &value, RVec<float> &Track_eta, RVec<float> &Track_phi, RVec<float> &goodgeneta, RVec<float> &goodgenphi, RVec<float> &Track_chi2, RVec<float> Track_originalAlgo, RVec<unsigned int> Track_quality){
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
		if (Map.begin()->first<0.05) v.emplace_back(value[j]);
	}
	return v;
}

RVec<float> goodtrackreal(RVec<float> &value, RVec<float> &Track_eta, RVec<float> &Track_phi, RVec<float> &goodgeneta, RVec<float> &goodgenphi, RVec<float> &Track_chi2, RVec<float> Track_originalAlgo, RVec<unsigned int> Track_quality){
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
		if (Map.begin()->first<0.05) v.emplace_back(value[idx]);
	}
	return v;
}

RVec<bool> isgoodtrack(RVec<float> &goodtracketa, RVec<float> &goodtrackphi, RVec<float> &Muon_standeta, RVec<float> &Muon_standphi, RVec<bool> &Muon_isStandalone) {
	RVec<bool> v;
	for (auto i=0U;i < goodtracketa.size(); ++i) {
		std::map<float,int> Map;
		for (auto j=0U; j < Muon_standeta.size(); ++j) {
			if (!Muon_isStandalone[j]) continue;
			TLorentzVector cand, cand2;
			cand.SetPtEtaPhiM(5.,goodtracketa[i],goodtrackphi[i],0.);
			cand2.SetPtEtaPhiM(5.,Muon_standeta[j],Muon_standphi[j],0.);
			Map.insert({cand.DeltaR(cand2),j});
		}
		if ((Map.size()>0)&&(Map.begin()->first<0.1)) v.emplace_back(true);
		else v.emplace_back(false);
	}
	return v;
}

//TRACKING

RVec<float> goodmuonbooltrack(RVec<float> &goodgeneta, RVec<float> &goodgenphi, RVec<float> &Track_eta, RVec<float> &Track_phi, RVec<float> &Track_originalAlgo, RVec<unsigned int> &Track_quality, RVec<float> &Track_chi2) {
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
		if ((Map.size()>0)&&(Map.begin()->first<0.1)) v.emplace_back(1);
		else v.emplace_back(0); 
	}
	return v;
}

RVec<float> goodmuon(RVec<float> &value, RVec<float> &goodgeneta, RVec<float> &goodgenphi, RVec<float> &Muon_standeta, RVec<float> &Muon_standphi, RVec<bool> &Muon_isStandalone){
	RVec<float> v;
	for (auto j=0U; j < goodgeneta.size(); ++j) {
		std::map<float,int> Map;
		for (auto i=0U;i < Muon_standeta.size(); ++i) {
			if (!Muon_isStandalone[i]) continue;
			TLorentzVector cand, cand2;
			cand.SetPtEtaPhiM(5.,goodgeneta[j],goodgenphi[j],0.);
			cand2.SetPtEtaPhiM(5.,Muon_standeta[i],Muon_standphi[i],0.);
			Map.insert({cand.DeltaR(cand2),i});
		}
		if (Map.size()==0) continue;
		if (Map.begin()->first<0.1) v.emplace_back(value[j]);
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
		if (Map.begin()->first<0.1) v.emplace_back(value[Map.begin()->second]);
	}
	return v;
}

RVec<bool> isgoodmuon(RVec<float> &Track_eta, RVec<float> &Track_phi, RVec<float> &goodmuoneta, RVec<float> &goodmuonphi, RVec<float> &Track_originalAlgo, RVec<unsigned int> &Track_quality, RVec<float> &Track_chi2) {
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
		if ((Map.size()>0)&&(Map.begin()->first<0.1)) v.emplace_back(true);
		else v.emplace_back(false);
	}
	return v;
}

//GLOBAL

RVec<bool> goodmuonboolglobal(RVec<float> &goodgeneta, RVec<float> &goodgenphi, RVec<float> &Muon_standeta, RVec<float> &Muon_standphi, RVec<bool> &Muon_isGlobal) {
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
		if ((Map.size()>0)&&(Map.begin()->first<0.1)&&(Muon_isGlobal[Map.begin()->second])) v.emplace_back(1);
		else v.emplace_back(0); 
	}
	return v;
}

RVec<int> goodmuonidx(RVec<float> &goodgeneta, RVec<float> &goodgenphi, RVec<float> &Muon_standeta, RVec<float> &Muon_standphi, RVec<bool> &Muon_isStandalone){
	RVec<int> v;
	for (auto j=0U; j < goodgeneta.size(); ++j) {
		std::map<float,int> Map;
		float Val;
		for (auto i=0U;i < Muon_isStandalone.size(); ++i) {
			if (!Muon_isStandalone[i]) continue;
			TLorentzVector cand, cand2;
			cand.SetPtEtaPhiM(5.,goodgeneta[j],goodgenphi[j],0.);
			cand2.SetPtEtaPhiM(5.,Muon_standeta[i],Muon_standphi[i],0.);
			Map.insert({cand.DeltaR(cand2),i});
		}
		if (Map.size()==0) continue;
		if (Map.begin()->first<0.1) v.emplace_back(Map.begin()->second);
	}
	return v;
}

RVec<bool> goodstand(RVec<int> &goodmuonidx, RVec<bool> &Muon_isGlobal) {
	RVec<bool> v;
	for (auto i=0U;i < goodmuonidx.size(); ++i) {
		if ((Muon_isGlobal[goodmuonidx[i]])) v.emplace_back(true);
		else v.emplace_back(false);
	}
	return v;
}
