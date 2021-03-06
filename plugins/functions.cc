#include <algorithm>
#include "functions.h"
#include "TLorentzVector.h"
#include "TSystem.h"

bool genleptoncompare (std::pair<int, float> i, std::pair<int, float> j) { 
	return (i.second>j.second);
}

bool distancesort(std::pair<Double_t,std::pair<UInt_t, Int_t> > i, std::pair<Double_t,std::pair<UInt_t, Int_t> > j) {
	return (i.first<j.first);
}

//implentation in the NanoAOD files of the GenPart collection and selection of the PreFSR generator leptons
////logic for the generator lepton selection included from here: https://github.com/emanca/wproperties-analysis/blob/master/nanotools/src/genLeptonSelector.cpp
void getGenLeptonIdxandFill(const std::vector<reco::GenParticle>& genparticles, Float_t *GenPart_eta_, Float_t *GenPart_mass_, Float_t *GenPart_phi_, Float_t *GenPart_pt_, Int_t* GenPart_genPartIdxMother_, Int_t *GenPart_pdgId_, Int_t *GenPart_status_, Int_t *GenPart_statusFlags_, Int_t& GenPart_preFSRLepIdx1, Int_t& GenPart_preFSRLepIdx2, Int_t& GenPart_postFSRLepIdx1_, Int_t& GenPart_postFSRLepIdx2_, UInt_t& nGenPart_, UInt_t& nGenPartPreFSR_, UInt_t& nGenMuonPreFSR_, UInt_t& nGenPart746_, UInt_t& nGenPartPostFSR_, Int_t *GenPart_PostFSR, Int_t *GenPart_motherPdgId_, Int_t *GenPart_origIdx_) {
	std::vector<int> status746(0);
	std::vector<int> other(0);
	std::vector<int> postfsrmum(0);
	std::vector<int> postfsrother(0);
	unsigned int j=0, origj=0;
	for (auto const& genparticle : genparticles) {
		if (genparticle.numberOfMothers()>0) { 
			int mompdgId = genparticle.mother()->pdgId();
			if (abs(genparticle.pdgId()) >= 11 && abs(genparticle.pdgId()) <= 16) {
				GenPart_eta_[j]=genparticle.eta();
				GenPart_mass_[j]=genparticle.mass();
				GenPart_phi_[j]=genparticle.phi();
				GenPart_pt_[j]=genparticle.pt();
				GenPart_genPartIdxMother_[j]=genparticle.motherRef(0).key(); 
				GenPart_motherPdgId_[j]=mompdgId;
				GenPart_pdgId_[j]=genparticle.pdgId();
				GenPart_status_[j]=genparticle.status();
				GenPart_statusFlags_[j]=(genparticle.statusFlags()).flags_.to_ulong();
				GenPart_origIdx_[j]=origj;
				if (abs(mompdgId) == 23 || abs(mompdgId) == 24) {
					if ((genparticle.status() == 1)||(genparticle.status()==2))
						if (genparticle.statusFlags().flags_.to_ulong() & 1)
							postfsrmum.push_back(j);
					if (genparticle.status() == 746) //status 746 is pre photos FSR in new powheg samples. so if they exist we want those.
						status746.push_back(j);
					else 
						other.push_back(j);
				}
				else {
					if (genparticle.status() == 23)  //in madgraph5_aMC@NLO there are some events without a W/Z, those have status 23 leptons (tested on >1M events)
						other.push_back(j);
					if ((genparticle.status() == 1)||(genparticle.status()==2))
						if (genparticle.statusFlags().flags_.to_ulong() & 1)
							postfsrother.push_back(j);
				}
				j++;
			}
		}
		origj++;
	}
	nGenPart_=j;
	for (unsigned int h=0; h!=nGenPart_; h++) {
		bool condition=false;
		int tmp;
		for (unsigned int H=0; H!=nGenPart_; H++) {
			if (h==H) continue;
			if (GenPart_genPartIdxMother_[h]==GenPart_origIdx_[H]) {condition=true;
				tmp=H;
			}
		}
		if (condition) GenPart_genPartIdxMother_[h]=tmp;
		else GenPart_genPartIdxMother_[h]=-1;
	}
	if (other.size() > 2) {
		std::vector<int> otherTmp;
		for (unsigned int h=0; h!=other.size(); h++) {
			if ((GenPart_statusFlags_[other[h]] >> 8) & 1) otherTmp.push_back(other[h]);
		}
		other = otherTmp;
	}
	std::vector<std::pair<int, float> > prefsrleptons(0);
	std::vector<std::pair<int, float> > postfsr(0);
	for (unsigned int h=0; h!=status746.size(); h++) {
		prefsrleptons.push_back({status746[h],GenPart_pt_[status746[h]]});
	}
	for (unsigned int h=0; h!=other.size(); h++) {
		prefsrleptons.push_back({other[h],GenPart_pt_[other[h]]});
	}
	for (unsigned int h=0; h!=postfsrmum.size(); h++) {
		postfsr.push_back({postfsrmum[h],GenPart_pt_[postfsrmum[h]]});
	}
	for (unsigned int h=0; h!=postfsrother.size(); h++) {
		postfsr.push_back({postfsrother[h],GenPart_pt_[postfsrother[h]]});
	}
	nGenPart746_=status746.size();
	nGenPartPreFSR_=prefsrleptons.size();
	nGenPartPostFSR_=postfsr.size();
	nGenMuonPreFSR_=0;
	for (unsigned int h=0; h!=prefsrleptons.size(); h++) {
		if (abs(genparticles[prefsrleptons[h].first].pdgId())==13) nGenMuonPreFSR_++;
	}
	sort(prefsrleptons.begin(),prefsrleptons.begin()+status746.size(),genleptoncompare); //priority given to status746 particles, therefore they have the first indices
	sort(prefsrleptons.begin()+status746.size(),prefsrleptons.end(),genleptoncompare); //then the other final state prefsr particles are added (what it is done here: https://github.com/WMass/nanoAOD-tools/blob/master/python/postprocessing/wmass/genLepSelection.py)
	sort(postfsr.begin(),postfsr.begin()+postfsrmum.size(),genleptoncompare);
	sort(postfsr.begin()+postfsrmum.size(),postfsr.end(),genleptoncompare);
	for (unsigned int h=0; h!=postfsr.size(); h++) {
		GenPart_PostFSR[h]=postfsr[h].first;
	}
	if (prefsrleptons.size()==0) {
		GenPart_preFSRLepIdx1 = -1;
		GenPart_preFSRLepIdx2 = -1;
	}
	else if (prefsrleptons.size()==1) {
		GenPart_preFSRLepIdx1 = (prefsrleptons[0]).first;
		GenPart_preFSRLepIdx2 = -1;
	}
	else {
		GenPart_preFSRLepIdx1 = (prefsrleptons[0]).second > (prefsrleptons[1]).second ? (prefsrleptons[0]).first : (prefsrleptons[1]).first;
		GenPart_preFSRLepIdx2 = (prefsrleptons[0]).second > (prefsrleptons[1]).second ? (prefsrleptons[1]).first : (prefsrleptons[0]).first;
	}
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
}

void setPileUpEra(std::string& era, std::string& pumc, std::string& pudata, std::string& pudata_down, std::string& pudata_up) {
	if (era=="2016preVFP") {
		pumc=std::string("$CMSSW_BASE/src/MuonAnalysis/MuonAnalysis/data/pileup_mc_2016UL.root"); //might need fixing (specific for my config)
		pudata=std::string("/afs/cern.ch/cms/CAF/CMSCOMM/COMM_DQM/certification/Collisions16/13TeV/PileUp/UltraLegacy/PileupHistogram-goldenJSON-13tev-2016-preVFP-69200ub-99bins.root");
		if(gSystem->AccessPathName(gSystem->ExpandPathName(pudata.c_str()))) {
			pudata=std::string("$CMSSW_BASE/src/MuonAnalysis/MuonAnalysis/data/PileupHistogram-goldenJSON-13tev-2016-preVFP-69200ub-99bins.root"); //file above copied in files/ directory (file above doesn't exist when submitting with crab)
		}
		pudata_down=std::string("/afs/cern.ch/cms/CAF/CMSCOMM/COMM_DQM/certification/Collisions16/13TeV/PileUp/UltraLegacy/PileupHistogram-goldenJSON-13tev-2016-preVFP-66000ub-99bins.root");
		if(gSystem->AccessPathName(gSystem->ExpandPathName(pudata_down.c_str()))) {
			pudata_down=std::string("$CMSSW_BASE/src/MuonAnalysis/MuonAnalysis/data/PileupHistogram-goldenJSON-13tev-2016-preVFP-66000ub-99bins.root");
		}
		pudata_up=std::string("/afs/cern.ch/cms/CAF/CMSCOMM/COMM_DQM/certification/Collisions16/13TeV/PileUp/UltraLegacy/PileupHistogram-goldenJSON-13tev-2016-preVFP-72400ub-99bins.root");
		if(gSystem->AccessPathName(gSystem->ExpandPathName(pudata_up.c_str()))) {
			pudata_up=std::string("$CMSSW_BASE/src/MuonAnalysis/MuonAnalysis/data/PileupHistogram-goldenJSON-13tev-2016-preVFP-72400ub-99bins.root");
		}
	}
	if (era=="2016postVFP") {
		pumc=std::string("$CMSSW_BASE/src/MuonAnalysis/MuonAnalysis/data/pileup_mc_2016UL.root"); //might need fixing (specific for my config)
		pudata=std::string("/afs/cern.ch/cms/CAF/CMSCOMM/COMM_DQM/certification/Collisions16/13TeV/PileUp/UltraLegacy/PileupHistogram-goldenJSON-13tev-2016-postVFP-69200ub-99bins.root");
		if(gSystem->AccessPathName(gSystem->ExpandPathName(pudata.c_str()))) {
			pudata=std::string("$CMSSW_BASE/src/MuonAnalysis/MuonAnalysis/data/PileupHistogram-goldenJSON-13tev-2016-postVFP-69200ub-99bins.root"); //file above copied in files/ directory (file above doesn't exist when submitting with crab)
		}
		pudata_down=std::string("/afs/cern.ch/cms/CAF/CMSCOMM/COMM_DQM/certification/Collisions16/13TeV/PileUp/UltraLegacy/PileupHistogram-goldenJSON-13tev-2016-postVFP-66000ub-99bins.root");
		if(gSystem->AccessPathName(gSystem->ExpandPathName(pudata_down.c_str()))) {
			pudata_down=std::string("$CMSSW_BASE/src/MuonAnalysis/MuonAnalysis/data/PileupHistogram-goldenJSON-13tev-2016-postVFP-66000ub-99bins.root");
		}
		pudata_up=std::string("/afs/cern.ch/cms/CAF/CMSCOMM/COMM_DQM/certification/Collisions16/13TeV/PileUp/UltraLegacy/PileupHistogram-goldenJSON-13tev-2016-postVFP-72400ub-99bins.root");
		if(gSystem->AccessPathName(gSystem->ExpandPathName(pudata_up.c_str()))) {
			pudata_up=std::string("$CMSSW_BASE/src/MuonAnalysis/MuonAnalysis/data/PileupHistogram-goldenJSON-13tev-2016-postVFP-72400ub-99bins.root");
		}
	}
}
