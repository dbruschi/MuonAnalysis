#include <utility>
#include "ROOT/RDataFrame.hxx"
#include "ROOT/RVec.hxx"
#include "TLorentzVector.h"
#include "Functionstnp.h"
#include <map>
#include <cmath>
#include <iostream>

#define DISTANCE 0.3
#define TRACKDISTANCE 0.2

using ROOT::VecOps::RVec;

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

RVec<int> matchedtrackidx(RVec<float> &goodgeneta, RVec<float> &goodgenphi, RVec<int> &goodgenidx, RVec<float> &Track_eta, RVec<float> &Track_phi, RVec<float> &Track_chi2, RVec<int> &Track_originalAlgo) {
	RVec<int> v;
	std::map<float,std::pair<int,int> > Map;
	for (unsigned int i=0; i!=Track_eta.size(); i++) {
		v.emplace_back(-1);
	}
	for (unsigned int i=0; i!=goodgeneta.size(); i++) {
		for (unsigned int j=0; j!=Track_eta.size(); j++) {
			if ((Track_originalAlgo[j]==13)||(Track_originalAlgo[j]==14)) continue;
			if (Track_chi2[j]>5.) continue;
			TLorentzVector cand1, cand2;
			cand1.SetPtEtaPhiM(3.,goodgeneta[i],goodgenphi[i],0.);
			cand2.SetPtEtaPhiM(3.,Track_eta[j],Track_phi[j],0.);
			if (cand1.DeltaR(cand2)<DISTANCE) Map.insert({cand1.DeltaR(cand2),{i,j}});
		}
	}
	std::vector<int> dropind, dropgen;
	for (unsigned int i=0; i!=Map.size(); i++) {
		auto it = Map.begin();
		std::advance(it,i);
		int genidx=it->second.first, idx=it->second.second;
		if (std::find(dropind.begin(), dropind.end(), idx) != dropind.end()) continue;
		if (std::find(dropgen.begin(), dropgen.end(), genidx) != dropgen.end()) continue;
		v[idx]=goodgenidx[genidx];
		dropind.emplace_back(idx);
		dropgen.emplace_back(genidx);
	}
	return v;
}

RVec<int> matchedstaidx(RVec<float> &goodgeneta, RVec<float> &goodgenphi, RVec<int> &goodgenidx, RVec<float> &Track_eta, RVec<float> &Track_phi, RVec<bool> &Muon_isStandalone) {
	RVec<int> v;
	std::map<float,std::pair<int,int> > Map;
	for (unsigned int i=0; i!=Track_eta.size(); i++) {
		v.emplace_back(-1);
	}
	for (unsigned int i=0; i!=goodgeneta.size(); i++) {
		for (unsigned int j=0; j!=Track_eta.size(); j++) {
			if (!Muon_isStandalone[j]) continue;
			TLorentzVector cand1, cand2;
			cand1.SetPtEtaPhiM(3.,goodgeneta[i],goodgenphi[i],0.);
			cand2.SetPtEtaPhiM(3.,Track_eta[j],Track_phi[j],0.);
			if (cand1.DeltaR(cand2)<DISTANCE) Map.insert({cand1.DeltaR(cand2),{i,j}});
		}
	}
	std::vector<int> dropind, dropgen;
	for (unsigned int i=0; i!=Map.size(); i++) {
		auto it = Map.begin();
		std::advance(it,i);
		int genidx=it->second.first, idx=it->second.second;
		if (std::find(dropind.begin(), dropind.end(), idx) != dropind.end()) continue;
		if (std::find(dropgen.begin(), dropgen.end(), genidx) != dropgen.end()) continue;
		v[idx]=goodgenidx[genidx];
		dropind.emplace_back(idx);
		dropgen.emplace_back(genidx);
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

RVec<int> maketnppairs(RVec<float> &Muon_pt, RVec<float> &Muon_eta, RVec<float> &Muon_phi, RVec<float> &Muon_mass, RVec<float> &Muon_standeta, RVec<float> &Muon_standphi, RVec<bool> &Muon_isStandalone, RVec<bool> &Muon_isGlobal, RVec<bool> &Muon_mediumId, RVec<float> &Muon_dxyBS, RVec<bool> &Muon_triggered, RVec<float> &Muon_pfRelIso04_all, int option) {
	RVec<int> pairs;
	for (auto i=0U; i < Muon_pt.size(); i++) {
		if (Muon_pt[i]<35.) continue;
		if (abs(Muon_eta[i])>2.4) continue;
		if (!Muon_isGlobal[i]) continue;
		TLorentzVector cand1, cand2;
		cand1.SetPtEtaPhiM(3.,Muon_eta[i],Muon_phi[i],0.);
		cand2.SetPtEtaPhiM(3.,Muon_standeta[i],Muon_standphi[i],0.);
		if (cand1.DeltaR(cand2)>DISTANCE) continue;
		if (!cleaner(i,Muon_standeta,Muon_standphi,Muon_isStandalone)) continue;
		if (!Muon_mediumId[i]) continue;
		if (abs(Muon_dxyBS[i])>0.05) continue;
//		if (!Muon_triggered[i]) continue;
//		if (Muon_pfRelIso04_all[i]>0.15) continue;
		for (auto j=0U; j < Muon_pt.size(); j++) {
			if (i==j) continue;
			unsigned int flags=0;
			if ((cleaner(j,Muon_standeta,Muon_standphi,Muon_isStandalone))&&(Muon_isStandalone[j])) flags|=1;
			else continue;
			TLorentzVector Cand1, Cand2;
			Cand1.SetPtEtaPhiM(3.,Muon_eta[j],Muon_phi[j],0.);
			Cand2.SetPtEtaPhiM(3.,Muon_standeta[j],Muon_standphi[j],0.);
			if ((Muon_isGlobal[j])&&(Cand1.DeltaR(Cand2)<DISTANCE)) flags|=(1<<1);
			if ((Muon_mediumId[j])&&(abs(Muon_dxyBS[j])<0.05)) flags|=(1<<2);
			if (Muon_triggered[j]) flags|=(1<<3);
			if (Muon_pfRelIso04_all[j]<0.15) flags|=(1<<4);
			if (abs(Muon_dxyBS[j])<0.05) flags|=(1<<5);
			if (Muon_mediumId[j]) flags|=(1<<6);
			if (option==0) pairs.push_back(i);
			if (option==1) pairs.push_back(j);
			if (option==2) pairs.push_back(flags);
		}
	}
	return pairs;
}

RVec<int> maketnppairstrack(RVec<float> &Muon_pt, RVec<float> &Muon_eta, RVec<float> &Muon_phi, RVec<float> &Muon_mass, RVec<float> &Muon_standeta, RVec<float> &Muon_standphi, RVec<bool> &Muon_isStandalone, RVec<bool> &Muon_isGlobal, RVec<bool> &Muon_mediumId, RVec<float> &Muon_dxyBS, RVec<bool> &Muon_triggered, RVec<float> &Muon_pfRelIso04_all, RVec<float> &Track_pt, RVec<float> &Track_eta, RVec<float> &Track_phi, RVec<float> &Track_chi2, RVec<int> &Track_originalAlgo, int option) {
	RVec<int> pairs;
	for (auto i=0U; i < Muon_pt.size(); i++) {
		if (Muon_pt[i]<35.) continue;
		if (abs(Muon_eta[i])>2.4) continue;
		if (!Muon_isGlobal[i]) continue;
		TLorentzVector cand1, cand2;
		cand1.SetPtEtaPhiM(3.,Muon_eta[i],Muon_phi[i],0.);
		cand2.SetPtEtaPhiM(3.,Muon_standeta[i],Muon_standphi[i],0.);
		if (cand1.DeltaR(cand2)>DISTANCE) continue;
		if (!cleaner(i,Muon_standeta,Muon_standphi,Muon_isStandalone)) continue;
		if (!Muon_mediumId[i]) continue;
		if (abs(Muon_dxyBS[i])>0.05) continue;
//		if (!Muon_triggered[i]) continue;
//		if (Muon_pfRelIso04_all[i]>0.15) continue;
		for (auto j=0U; j < Track_pt.size(); j++) {
			TLorentzVector cand3;
			cand3.SetPtEtaPhiM(3.,Track_eta[j],Track_phi[j],0.);
			if (cand1.DeltaR(cand3)<DISTANCE) continue;
			if (Track_chi2[j]>5.) continue;
			if ((Track_originalAlgo[j]==13)||(Track_originalAlgo[j]==14)) continue;
			if (option==0) pairs.push_back(i);
			if (option==1) pairs.push_back(j);
			if (option==2) pairs.push_back(0);
		}
	}
	return pairs;
}

RVec<int> goodtnppairs(RVec<int> &tnppairtag, RVec<int> &tnppairprobe, RVec<int> &tnppairflag, RVec<int> &Muon_genPartIdx, RVec<int> &matchedtrackidx, RVec<int> &goodgenidx, int option) {
	RVec<int> pairs;
	for (auto i=0U; i < tnppairtag.size(); i++) {
		bool condition1=false, condition2=false;
		for (auto j=0U; j < goodgenidx.size(); j++) {
			if (Muon_genPartIdx[tnppairtag[i]]==goodgenidx[j]) condition1=true;
			if (matchedtrackidx[tnppairprobe[i]]==goodgenidx[j]) condition2=true;
		}
		if ((condition1)&&(condition2)) {
			if (option==0) pairs.emplace_back(tnppairtag[i]);
			if (option==1) pairs.emplace_back(tnppairprobe[i]);
			if (option==2) pairs.emplace_back(tnppairflag[i]);
		}
	}
	return pairs;
}

RVec<float> invariantmasses(RVec<int> &maketnppairstag, RVec<int> &maketnppairsprobe, RVec<float> &pt1, RVec<float> &eta1, RVec<float> &phi1, RVec<float> &pt2, RVec<float> &eta2, RVec<float> &phi2, RVec<float> &Muon_mass) {
	RVec<float> im;
	for (auto i=0U; i < maketnppairstag.size(); i++) {
		TLorentzVector cand1, cand2;
		cand1.SetPtEtaPhiM(pt1[maketnppairstag[i]],eta1[maketnppairstag[i]],phi1[maketnppairstag[i]],Muon_mass[maketnppairstag[i]]);
		cand2.SetPtEtaPhiM(pt2[maketnppairsprobe[i]],eta2[maketnppairsprobe[i]],phi2[maketnppairsprobe[i]],Muon_mass[maketnppairstag[i]]);
		im.emplace_back((cand1+cand2).Mag());
	}
	return im;
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

RVec<float> goodtrackgen(RVec<int> &goodtnppairtag, RVec<int> &goodtnppairprobe, RVec<int> &matchedtrackidx, RVec<int> &goodgenidx, RVec<float> &goodgenpt) {
	RVec<float> v;
	for (auto i=0U;i < goodtnppairtag.size(); ++i) {
		bool condition=false;
		int idx;
		for (auto j=0U; j < goodgenpt.size(); j++) {
			if (matchedtrackidx[goodtnppairprobe[i]]==goodgenidx[j]) {condition=true;
				idx=j;
			}
		}
		if (condition) v.emplace_back(goodgenpt[idx]);
	}
	return v;
}

RVec<float> goodtrack(RVec<int> &goodtnppairtag, RVec<int> &goodtnppairprobe, RVec<float> &Track_eta) {
	RVec<float> v;
	for (auto i=0U;i < goodtnppairtag.size(); ++i) {
		v.emplace_back(Track_eta[goodtnppairprobe[i]]);
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
		if ((Map.size()>0)&&(Map.begin()->first<DISTANCE)) {
			if (cleaner(Map.begin()->second,Muon_standeta,Muon_standphi,Muon_isStandalone)) v.emplace_back(true);
			else v.emplace_back(false);
		}
		else v.emplace_back(false);
	}
	return v;
}

//GLOBAL

RVec<bool> goodmuonboolglobal(RVec<float> &goodgeneta, RVec<float> &goodgenphi, RVec<int> &goodmuonidx, RVec<int> &Muon_genPartIdx, RVec<bool> &Muon_isGlobal, RVec<float> &Muon_standeta, RVec<float> &Muon_standphi, RVec<float> &Muon_eta, RVec<float> &Muon_phi, RVec<bool> &Muon_isStandalone) {
	RVec<bool> v;
	for (auto i=0U;i < goodgeneta.size(); ++i) {
		bool condition=false;
		int idx=-1;
		for (auto j=0U; j < Muon_genPartIdx.size(); ++j) {
			if (Muon_genPartIdx[j]==goodmuonidx[i]) {condition=true;
				idx=j;
			}
		}
		if ((condition)&&(Muon_isGlobal[idx])) {
			TLorentzVector cand1, cand2;
			cand1.SetPtEtaPhiM(5.,Muon_eta[idx],Muon_phi[idx],0.);
			cand2.SetPtEtaPhiM(5.,Muon_standeta[idx],Muon_standphi[idx],0.);
			if (cand1.DeltaR(cand2)<DISTANCE) {
				if (cleaner(idx,Muon_standeta,Muon_standphi,Muon_isStandalone)) v.emplace_back(true);
				else v.emplace_back(false);
			}
			else v.emplace_back(false);
		}
		else v.emplace_back(false);
	}
	return v;
}

RVec<float> goodmuonreal(RVec<float> &value, RVec<int> &goodtnpprobereco) {
	RVec<float> v;
	for (auto i=0U; i < goodtnpprobereco.size(); i++) {
		v.emplace_back(value[goodtnpprobereco[i]]);
	}
	return v;
}

RVec<bool> isglobal(RVec<int> &goodtnpflagreco, int bit) {
	RVec<bool> v;
	for (auto i=0U; i < goodtnpflagreco.size(); i++) {
		if ((goodtnpflagreco[i]&(1<<bit))>>bit) v.emplace_back(true);
		else v.emplace_back(false);
	}
	return v;
}

//IDIP

RVec<bool> goodmuonboolidip(RVec<int> &goodgenidx, RVec<int> &Muon_genPartIdx, RVec<float> &Muon_eta, RVec<float> &Muon_phi, RVec<float> &Muon_standeta, RVec<float> &Muon_standphi, RVec<bool> &Muon_isStandalone, RVec<bool> &Muon_isGlobal, RVec<bool> &Muon_mediumId, RVec<float> &Muon_dxyBS) {
	RVec<bool> v;
	for (auto i=0U; i<goodgenidx.size(); i++) {
		int idx=-1;
		for (auto j=0U; j<Muon_genPartIdx.size(); ++j) {
			if (Muon_genPartIdx[j]==goodgenidx[i]) {
				TLorentzVector cand1, cand2;
				cand1.SetPtEtaPhiM(3.,Muon_eta[j],Muon_phi[j],0.);
				cand2.SetPtEtaPhiM(3.,Muon_standeta[j],Muon_standphi[j],0.);
				if (cand1.DeltaR(cand2)<DISTANCE) idx=j;
			}
		}
		if ((idx>-1)&&(Muon_isGlobal[idx])&&(Muon_mediumId[idx])&&(fabs(Muon_dxyBS[idx])<0.05)) {
			if (cleaner(idx,Muon_standeta,Muon_standphi,Muon_isStandalone)) v.emplace_back(1);
			else v.emplace_back(0);
		}
		else v.emplace_back(0); 
	}
	return v;
}

RVec<bool> goodmuonboolid(RVec<int> &goodgenidx, RVec<int> &Muon_genPartIdx, RVec<float> &Muon_eta, RVec<float> &Muon_phi, RVec<float> &Muon_standeta, RVec<float> &Muon_standphi, RVec<bool> &Muon_isStandalone, RVec<bool> &Muon_isGlobal, RVec<bool> &Muon_mediumId, RVec<float> &Muon_dxyBS) {
	RVec<bool> v;
	for (auto i=0U; i<goodgenidx.size(); i++) {
		int idx=-1;
		for (auto j=0U; j<Muon_genPartIdx.size(); ++j) {
			if (Muon_genPartIdx[j]==goodgenidx[i]) {
				TLorentzVector cand1, cand2;
				cand1.SetPtEtaPhiM(3.,Muon_eta[j],Muon_phi[j],0.);
				cand2.SetPtEtaPhiM(3.,Muon_standeta[j],Muon_standphi[j],0.);
				if (cand1.DeltaR(cand2)<DISTANCE) idx=j;
			}
		}
		if ((idx>-1)&&(Muon_isGlobal[idx])&&(Muon_mediumId[idx])) {
			if (cleaner(idx,Muon_standeta,Muon_standphi,Muon_isStandalone)) v.emplace_back(1);
			else v.emplace_back(0);
		}
		else v.emplace_back(0); 
	}
	return v;
}

RVec<bool> goodmuonboolip(RVec<int> &goodgenidx, RVec<int> &Muon_genPartIdx, RVec<float> &Muon_eta, RVec<float> &Muon_phi, RVec<float> &Muon_standeta, RVec<float> &Muon_standphi, RVec<bool> &Muon_isStandalone, RVec<bool> &Muon_isGlobal, RVec<bool> &Muon_mediumId, RVec<float> &Muon_dxyBS) {
	RVec<bool> v;
	for (auto i=0U; i<goodgenidx.size(); i++) {
		int idx=-1;
		for (auto j=0U; j<Muon_genPartIdx.size(); ++j) {
			if (Muon_genPartIdx[j]==goodgenidx[i]) {
				TLorentzVector cand1, cand2;
				cand1.SetPtEtaPhiM(3.,Muon_eta[j],Muon_phi[j],0.);
				cand2.SetPtEtaPhiM(3.,Muon_standeta[j],Muon_standphi[j],0.);
				if (cand1.DeltaR(cand2)<DISTANCE) idx=j;
			}
		}
		if ((idx>-1)&&(Muon_isGlobal[idx])&&(fabs(Muon_dxyBS[idx])<0.05)) {
			if (cleaner(idx,Muon_standeta,Muon_standphi,Muon_isStandalone)) v.emplace_back(1);
			else v.emplace_back(0);
		}
		else v.emplace_back(0); 
	}
	return v;
}

//TRIGGERMATCH

RVec<bool> goodmuonbooltrigger(RVec<int> &goodgenidx, RVec<int> &Muon_genPartIdx, RVec<float> &Muon_eta, RVec<float> &Muon_phi, RVec<float> &Muon_standeta, RVec<float> &Muon_standphi, RVec<bool> &Muon_isStandalone, RVec<bool> &Muon_isGlobal, RVec<bool> &Muon_mediumId, RVec<float> &Muon_dxyBS, RVec<bool> &Muon_triggered) {
	RVec<bool> v;
	for (auto i=0U; i<goodgenidx.size(); i++) {
		int idx=-1;
		for (auto j=0U; j<Muon_genPartIdx.size(); ++j) {
			if (Muon_genPartIdx[j]==goodgenidx[i]) {
				TLorentzVector cand1, cand2;
				cand1.SetPtEtaPhiM(3.,Muon_eta[j],Muon_phi[j],0.);
				cand2.SetPtEtaPhiM(3.,Muon_standeta[j],Muon_standphi[j],0.);
				if (cand1.DeltaR(cand2)<DISTANCE) idx=j;
			}
		}
		if ((idx>-1)&&(Muon_isGlobal[idx])&&(Muon_mediumId[idx])&&(fabs(Muon_dxyBS[idx])<0.05)&&(Muon_triggered[idx])) {
			if (cleaner(idx,Muon_standeta,Muon_standphi,Muon_isStandalone)) v.emplace_back(1);
			else v.emplace_back(0);
		}
		else v.emplace_back(0); 
	}
	return v;
}

//ISOLATION

RVec<bool> goodmuonboolisolation(RVec<int> &goodgenidx, RVec<int> &Muon_genPartIdx, RVec<float> &Muon_eta, RVec<float> &Muon_phi, RVec<float> &Muon_standeta, RVec<float> &Muon_standphi, RVec<bool> &Muon_isStandalone, RVec<bool> &Muon_isGlobal, RVec<bool> &Muon_mediumId, RVec<float> &Muon_dxyBS, RVec<bool> &Muon_triggered, RVec<float> &Muon_pfRelIso04_all) {
	RVec<bool> v;
	for (auto i=0U; i<goodgenidx.size(); i++) {
		int idx=-1;
		for (auto j=0U; j<Muon_genPartIdx.size(); ++j) {
			if (Muon_genPartIdx[j]==goodgenidx[i]) {
				TLorentzVector cand1, cand2;
				cand1.SetPtEtaPhiM(3.,Muon_eta[j],Muon_phi[j],0.);
				cand2.SetPtEtaPhiM(3.,Muon_standeta[j],Muon_standphi[j],0.);
				if (cand1.DeltaR(cand2)<DISTANCE) idx=j;
			}
		}
		if ((idx>-1)&&(Muon_isGlobal[idx])&&(Muon_mediumId[idx])&&(fabs(Muon_dxyBS[idx])<0.05)&&(Muon_triggered[idx])&&(Muon_pfRelIso04_all[idx]<0.15)) {
			if (cleaner(idx,Muon_standeta,Muon_standphi,Muon_isStandalone)) v.emplace_back(1);
			else v.emplace_back(0);
		}
		else v.emplace_back(0); 
	}
	return v;
}

//ISOLATION WITHOUT TRIGGER

RVec<bool> goodmuonboolisolationnotrig(RVec<int> &goodgenidx, RVec<int> &Muon_genPartIdx, RVec<float> &Muon_eta, RVec<float> &Muon_phi, RVec<float> &Muon_standeta, RVec<float> &Muon_standphi, RVec<bool> &Muon_isStandalone, RVec<bool> &Muon_isGlobal, RVec<bool> &Muon_mediumId, RVec<float> &Muon_dxyBS, RVec<bool> &Muon_triggered, RVec<float> &Muon_pfRelIso04_all) {
	RVec<bool> v;
	for (auto i=0U; i<goodgenidx.size(); i++) {
		int idx=-1;
		for (auto j=0U; j<Muon_genPartIdx.size(); ++j) {
			if (Muon_genPartIdx[j]==goodgenidx[i]) {
				TLorentzVector cand1, cand2;
				cand1.SetPtEtaPhiM(3.,Muon_eta[j],Muon_phi[j],0.);
				cand2.SetPtEtaPhiM(3.,Muon_standeta[j],Muon_standphi[j],0.);
				if (cand1.DeltaR(cand2)<DISTANCE) idx=j;
			}
		}
		if ((idx>-1)&&(Muon_isGlobal[idx])&&(Muon_mediumId[idx])&&(fabs(Muon_dxyBS[idx])<0.05)&&(Muon_pfRelIso04_all[idx]<0.15)) {
			if (cleaner(idx,Muon_standeta,Muon_standphi,Muon_isStandalone)) v.emplace_back(1);
			else v.emplace_back(0);
		}
		else v.emplace_back(0); 
	}
	return v;
}
