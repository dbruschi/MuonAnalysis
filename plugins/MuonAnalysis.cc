// -*- C++ -*-
//
// Package:    MuonAnalysis/MuonAnalysis
// Class:      MuonAnalysis
//
/**\class MuonAnalysis MuonAnalysis.cc MuonAnalysis/MuonAnalysis/plugins/MuonAnalysis.cc

 Description: [one line class summary]

 Implementation:
     [Notes on implementation]
*/
//
// Original Author:  Davide BRUSCHINI
//         Created:  Wed, 03 Nov 2021 15:42:54 GMT
//
//


// system include files
#include <memory>
#include <algorithm>
#include <iostream>
#include <stdexcept>

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/one/EDAnalyzer.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/Run.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Utilities/interface/InputTag.h"
#include "DataFormats/TrackReco/interface/Track.h"
#include "DataFormats/TrackReco/interface/TrackFwd.h"

#include "DataFormats/PatCandidates/interface/Muon.h"
#include "DataFormats/VertexReco/interface/Vertex.h"
#include "DataFormats/BeamSpot/interface/BeamSpot.h"
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "CommonTools/UtilAlgos/interface/TFileService.h"
#include "DataFormats/HepMCCandidate/interface/GenParticle.h"
#include "SimDataFormats/GeneratorProducts/interface/GenEventInfoProduct.h"
#include "SimDataFormats/PileupSummaryInfo/interface/PileupSummaryInfo.h"
#include "PhysicsTools/Utilities/interface/LumiReWeighting.h"
#include "SimDataFormats/GeneratorProducts/interface/LHEEventProduct.h"
#include "FWCore/Common/interface/TriggerNames.h"
#include "DataFormats/Common/interface/TriggerResults.h"
#include "DataFormats/PatCandidates/interface/MET.h"
#include "functions.h"
#include "TSystem.h"
#include "TTree.h"
#include "TLorentzVector.h"

//
// class declaration
//

// If the analyzer does not use TFileService, please remove
// the template argument to the base class so the class inherits
// from  edm::one::EDAnalyzer<>
// This will improve performance in multithreaded jobs.

class MuonAnalysis : public edm::one::EDAnalyzer<edm::one::SharedResources>  {
	public:
		explicit MuonAnalysis(const edm::ParameterSet&);
		~MuonAnalysis();

		static void fillDescriptions(edm::ConfigurationDescriptions& descriptions);


	private:
		virtual void beginJob() override;
		virtual void analyze(const edm::Event&, const edm::EventSetup&) override;
		virtual void endJob() override;

		// ----------member data ---------------------------
		edm::EDGetTokenT<std::vector<pat::Muon> > muonToken_;
		edm::EDGetTokenT<std::vector<reco::Vertex> > vertexToken_;
		edm::EDGetTokenT<reco::BeamSpot> beamspotToken_;
		edm::EDGetTokenT<ROOT::Math::PositionVector3D<ROOT::Math::Cartesian3D<float>,ROOT::Math::DefaultCoordinateSystemTag> > genvertexToken_;
		edm::EDGetTokenT<std::vector<reco::GenParticle> > genparticleToken_;
		edm::EDGetTokenT<GenEventInfoProduct> geneventinfoToken_;
		edm::EDGetTokenT<std::vector<PileupSummaryInfo> > puToken_;
		edm::EDGetTokenT<edm::TriggerResults> hlttrigToken_;
		edm::EDGetTokenT< double > prefweightpreVFP_token;
		edm::EDGetTokenT< double > prefweightpreVFPup_token;
		edm::EDGetTokenT< double > prefweightpreVFPdown_token;
		edm::EDGetTokenT< double > prefweightpreVFPECAL_token;
		edm::EDGetTokenT< double > prefweightpreVFPupECAL_token;
		edm::EDGetTokenT< double > prefweightpreVFPdownECAL_token;
		edm::EDGetTokenT< double > prefweightpreVFPMuon_token;
		edm::EDGetTokenT< double > prefweightpreVFPupMuon_token;
		edm::EDGetTokenT< double > prefweightpreVFPdownMuon_token;
		edm::EDGetTokenT< double > prefweightpreVFPstatupMuon_token;
		edm::EDGetTokenT< double > prefweightpreVFPstatdownMuon_token;
		edm::EDGetTokenT< double > prefweightpreVFPsystupMuon_token;
		edm::EDGetTokenT< double > prefweightpreVFPsystdownMuon_token;
		edm::EDGetTokenT< double > prefweightpostVFP_token;
		edm::EDGetTokenT< double > prefweightpostVFPup_token;
		edm::EDGetTokenT< double > prefweightpostVFPdown_token;
		edm::EDGetTokenT< double > prefweightpostVFPECAL_token;
		edm::EDGetTokenT< double > prefweightpostVFPupECAL_token;
		edm::EDGetTokenT< double > prefweightpostVFPdownECAL_token;
		edm::EDGetTokenT< double > prefweightpostVFPMuon_token;
		edm::EDGetTokenT< double > prefweightpostVFPupMuon_token;
		edm::EDGetTokenT< double > prefweightpostVFPdownMuon_token;
		edm::EDGetTokenT< double > prefweightpostVFPstatupMuon_token;
		edm::EDGetTokenT< double > prefweightpostVFPstatdownMuon_token;
		edm::EDGetTokenT< double > prefweightpostVFPsystupMuon_token;
		edm::EDGetTokenT< double > prefweightpostVFPsystdownMuon_token;
		edm::EDGetTokenT<LHEEventProduct> lheinfoToken_;
		edm::EDGetTokenT<std::vector<pat::MET> > metToken_;
		edm::EDGetTokenT<std::vector<reco::Track> > trackToken_;
		bool isAOD_;
		TTree * tree_;
		UInt_t nMuon_, nTrack_, nGenPart_, nGenPartPreFSR_, nGenMuonPreFSR_, nGenPart746_, nGenPartPostFSR_;
		Float_t Muon_pt_[100], Muon_eta_[100], Muon_phi_[100], Muon_mass_[100], Muon_pfRelIso04_all_[100], Muon_pfRelIso04_chgPV_[100], Muon_pfRelIso04_chgPU_[100];
		Float_t Muon_standpt_[100], Muon_standeta_[100], Muon_standphi_[100];
		Float_t Muon_pfRelIso04_nhad_[100], Muon_pfRelIso04_pho_[100], Muon_pfRelIso03_all_[100];
		Float_t Muon_pfRelIso03_chgPV_[100], Muon_pfRelIso03_chgPU_[100], Muon_pfRelIso03_nhad_[100], Muon_pfRelIso03_pho_[100], Muon_tkRelIso_[100];
		Float_t  Muon_dxy_[100], Muon_dxyErr_[100], Muon_dz_[100], Muon_dzErr_[100], Muon_dxyBS_[100], Muon_dzBS_[100];
		Float_t Muon_closestVtx_X_[100], Muon_closestVtx_Y_[100], Muon_closestVtx_Z_[100];
		Float_t Track_pt_[100], Track_eta_[100], Track_phi_[100], Track_algo_[100], Track_originalAlgo_[100], Track_chi2_[100], Track_ndof_[100], Track_charge_[100];
		Float_t Track_dxy_[100], Track_dxyErr_[100], Track_dz_[100], Track_dzErr_[100], Track_dxyBS_[100], Track_dzBS_[100];
		Float_t Track_closestVtx_X_[100], Track_closestVtx_Y_[100], Track_closestVtx_Z_[100];
		Float_t PV_chi2_, PV_ndof_, PV_score_, PV_x_, PV_y_, PV_z_, BeamSpot_x0_, BeamSpot_y0_, BeamSpot_z0_, BeamSpot_dxdz_, BeamSpot_dydz_, BeamSpot_sigmaZ_;
		Float_t BeamSpot_x0Err_, BeamSpot_y0Err_, BeamSpot_z0Err_, BeamSpot_dxdzErr_, BeamSpot_dydzErr_, BeamSpot_sigmaZErr_, GenVertex_x_, GenVertex_y_, GenVertex_z_;
		Bool_t Muon_isTracker_[100], Muon_isGlobal_[100], Muon_isStandalone_[100], Muon_looseId_[100], Muon_mediumId_[100], Muon_mediumPromptId_[100], Muon_tightId_[100];
		Bool_t Muon_softId_[100], Muon_isPF_[100], Muon_softMvaId_[100], Muon_pfIsoLoose_[100], Muon_pfIsoMedium_[100], Muon_pfIsoTight_[100], Muon_triggered_[100];
		Int_t Muon_charge_[100];
		Int_t Muon_BestTrackAlgo_[100], Muon_InnerTrackAlgo_[100], Muon_GlobalTrackAlgo_[100], Muon_BestTrackOriginalAlgo_[100], Muon_InnerTrackOriginalAlgo_[100];
		Int_t Muon_GlobalTrackOriginalAlgo_[100], Muon_genPartIdx_[100], Muon_genPartPreFSRIdx_[100];
		UChar_t Muon_highPtId_[100], Muon_miniIsoId_[100], Muon_multiIsoId_[100], Muon_mvaId_[100], Muon_mvaLowPtId_[100], Muon_pfIsoId_[100], Muon_tkIsoId_[100];
		Float_t GenPart_eta_[500], GenPart_mass_[500], GenPart_phi_[500], GenPart_pt_[500], Generator_weight_, puWeight_, puWeight_Up_, puWeight_Down_;
		Float_t L1PreFiringWeightpreVFP_Nom, L1PreFiringWeightpreVFP_Up, L1PreFiringWeightpreVFP_Dn, L1PreFiringWeightECALpreVFP_Nom;
		Float_t L1PreFiringWeightECALpreVFP_Up, L1PreFiringWeightECALpreVFP_Dn, L1PreFiringWeightMuonpreVFP_Nom, L1PreFiringWeightMuonpreVFP_Up, L1PreFiringWeightMuonpreVFP_Dn;
		Float_t L1PreFiringWeightMuonpreVFP_statUp, L1PreFiringWeightMuonpreVFP_statDn, L1PreFiringWeightMuonpreVFP_systUp, L1PreFiringWeightMuonpreVFP_systDn;
		Float_t L1PreFiringWeightpostVFP_Nom, L1PreFiringWeightpostVFP_Up, L1PreFiringWeightpostVFP_Dn, L1PreFiringWeightECALpostVFP_Nom, L1PreFiringWeightECALpostVFP_Up, L1PreFiringWeightECALpostVFP_Dn;
		Float_t L1PreFiringWeightMuonpostVFP_Nom, L1PreFiringWeightMuonpostVFP_Up, L1PreFiringWeightMuonpostVFP_Dn; 
		Float_t L1PreFiringWeightMuonpostVFP_statUp, L1PreFiringWeightMuonpostVFP_statDn, L1PreFiringWeightMuonpostVFP_systUp, L1PreFiringWeightMuonpostVFP_systDn;
		Int_t GenPart_genPartIdxMother_[500], GenPart_pdgId_[500], GenPart_status_[500], GenPart_statusFlags_[500];
		Int_t GenPart_preFSRLepIdx1_, GenPart_preFSRLepIdx2_, GenPart_postFSRLepIdx1_, GenPart_postFSRLepIdx2_, GenPart_PostFSR_[500];
		Bool_t HLT_IsoMu24_, HLT_IsoTkMu24_;
		Float_t LHEPdfWeight_[500], LHEScaleWeight_[500];
		UInt_t nLHEPdfWeight_, nLHEScaleWeight_;
		Float_t MET_pt_, MET_phi_, MET_JECUp_pt_, MET_JECDown_pt_, MET_JECUp_phi_, MET_JECDown_phi_, MET_JERUp_pt_, MET_JERDown_pt_;
		Float_t MET_JERUp_phi_, MET_JERDown_phi_, MET_UnclEnUp_pt_, MET_UnclEnDown_pt_, MET_UnclEnUp_phi_, MET_UnclEnDown_phi_;
		Float_t Pileup_nTrueInt_;
		edm::LumiReWeighting* lumiWeights_;
		edm::LumiReWeighting* lumiWeightsup_;
		edm::LumiReWeighting* lumiWeightsdown_;
};

//
// constants, enums and typedefs
//

//
// static data member definitions
//

//
// constructors and destructor
//
MuonAnalysis::MuonAnalysis(const edm::ParameterSet& iConfig)
 :
  muonToken_(consumes<std::vector<pat::Muon> >(iConfig.getUntrackedParameter<edm::InputTag>("muons")))

{
	//now do what ever initialization is needed
	if (iConfig.getUntrackedParameter<std::string>("datatier")=="AOD") {
		isAOD_=true;
		std::cout<<"Running analyzer on AOD. Check this is correct\n";
	}
	else if (iConfig.getUntrackedParameter<std::string>("datatier")=="MINIAOD") {
		isAOD_=false;
		std::cout<<"Running analyzer on MINIAOD. Check this is correct\n";
	}
	else {
		throw std::runtime_error("'datatier' can only be 'MINIAOD' or 'AOD'!");
	}
	vertexToken_=consumes<std::vector<reco::Vertex> >(iConfig.getUntrackedParameter<edm::InputTag>("vertices"));
	beamspotToken_=consumes<reco::BeamSpot>(iConfig.getUntrackedParameter<edm::InputTag>("beamspot"));
	genvertexToken_=consumes<ROOT::Math::PositionVector3D<ROOT::Math::Cartesian3D<float>,ROOT::Math::DefaultCoordinateSystemTag> >(iConfig.getUntrackedParameter<edm::InputTag>("genvertex"));
	genparticleToken_=consumes<std::vector<reco::GenParticle> >(iConfig.getUntrackedParameter<edm::InputTag>("genparticles"));
	geneventinfoToken_=consumes<GenEventInfoProduct>(iConfig.getUntrackedParameter<edm::InputTag>("geneventinfo"));
	puToken_=consumes<std::vector<PileupSummaryInfo> >(iConfig.getUntrackedParameter<edm::InputTag>("pileupinfo"));
	lheinfoToken_=consumes<LHEEventProduct>(iConfig.getUntrackedParameter<edm::InputTag>("lheinfo"));
	hlttrigToken_=consumes<edm::TriggerResults>(iConfig.getUntrackedParameter<edm::InputTag>("hlttrig"));
	prefweightpreVFP_token = consumes< double >(edm::InputTag("prefiringweightpreVFP:nonPrefiringProb"));
	prefweightpreVFPup_token = consumes< double >(edm::InputTag("prefiringweightpreVFP:nonPrefiringProbUp"));
	prefweightpreVFPdown_token = consumes< double >(edm::InputTag("prefiringweightpreVFP:nonPrefiringProbDown"));
	prefweightpreVFPECAL_token = consumes< double >(edm::InputTag("prefiringweightpreVFP:nonPrefiringProbECAL"));
	prefweightpreVFPupECAL_token = consumes< double >(edm::InputTag("prefiringweightpreVFP:nonPrefiringProbECALUp"));
	prefweightpreVFPdownECAL_token = consumes< double >(edm::InputTag("prefiringweightpreVFP:nonPrefiringProbECALDown"));
	prefweightpreVFPMuon_token = consumes< double >(edm::InputTag("prefiringweightpreVFP:nonPrefiringProbMuon"));
	prefweightpreVFPupMuon_token = consumes< double >(edm::InputTag("prefiringweightpreVFP:nonPrefiringProbMuonUp"));
	prefweightpreVFPdownMuon_token = consumes< double >(edm::InputTag("prefiringweightpreVFP:nonPrefiringProbMuonDown"));
	prefweightpreVFPstatupMuon_token = consumes< double >(edm::InputTag("prefiringweightpreVFP:nonPrefiringProbMuonStatUp"));
	prefweightpreVFPstatdownMuon_token = consumes< double >(edm::InputTag("prefiringweightpreVFP:nonPrefiringProbMuonStatDown"));
	prefweightpreVFPsystupMuon_token = consumes< double >(edm::InputTag("prefiringweightpreVFP:nonPrefiringProbMuonSystUp"));
	prefweightpreVFPsystdownMuon_token = consumes< double >(edm::InputTag("prefiringweightpreVFP:nonPrefiringProbMuonSystDown"));
	prefweightpostVFP_token = consumes< double >(edm::InputTag("prefiringweightpostVFP:nonPrefiringProb"));
	prefweightpostVFPup_token = consumes< double >(edm::InputTag("prefiringweightpostVFP:nonPrefiringProbUp"));
	prefweightpostVFPdown_token = consumes< double >(edm::InputTag("prefiringweightpostVFP:nonPrefiringProbDown"));
	prefweightpostVFPECAL_token = consumes< double >(edm::InputTag("prefiringweightpostVFP:nonPrefiringProbECAL"));
	prefweightpostVFPupECAL_token = consumes< double >(edm::InputTag("prefiringweightpostVFP:nonPrefiringProbECALUp"));
	prefweightpostVFPdownECAL_token = consumes< double >(edm::InputTag("prefiringweightpostVFP:nonPrefiringProbECALDown"));
	prefweightpostVFPMuon_token = consumes< double >(edm::InputTag("prefiringweightpostVFP:nonPrefiringProbMuon"));
	prefweightpostVFPupMuon_token = consumes< double >(edm::InputTag("prefiringweightpostVFP:nonPrefiringProbMuonUp"));
	prefweightpostVFPdownMuon_token = consumes< double >(edm::InputTag("prefiringweightpostVFP:nonPrefiringProbMuonDown"));
	prefweightpostVFPsystupMuon_token = consumes< double >(edm::InputTag("prefiringweightpostVFP:nonPrefiringProbMuonSystUp"));
	prefweightpostVFPsystdownMuon_token = consumes< double >(edm::InputTag("prefiringweightpostVFP:nonPrefiringProbMuonSystDown"));
	prefweightpostVFPstatupMuon_token = consumes< double >(edm::InputTag("prefiringweightpostVFP:nonPrefiringProbMuonStatUp"));
	prefweightpostVFPstatdownMuon_token = consumes< double >(edm::InputTag("prefiringweightpostVFP:nonPrefiringProbMuonStatDown"));
	metToken_ = consumes< std::vector<pat::MET> >(edm::InputTag("slimmedMETs"));
	trackToken_ = consumes<std::vector<reco::Track> >(edm::InputTag("generalTracks"));
	std::string pumc("$CMSSW_BASE/src/MuonAnalysis/MuonAnalysis/files/pileup_mc_2016UL.root"); //might need fixing (specific for my config)
	if(gSystem->AccessPathName(gSystem->ExpandPathName(pumc.c_str()))) {
		pumc=std::string("$CMSSW_BASE/pileup_mc_2016UL.root"); //otherwise doesn't work during crab submission due to how files are included
	}
	std::string pudata("/afs/cern.ch/cms/CAF/CMSCOMM/COMM_DQM/certification/Collisions16/13TeV/PileUp/UltraLegacy/PileupHistogram-goldenJSON-13tev-2016-69200ub-99bins.root");
	if(gSystem->AccessPathName(gSystem->ExpandPathName(pudata.c_str()))) {
		pudata=std::string("$CMSSW_BASE/PileupHistogram-goldenJSON-13tev-2016-69200ub-99bins.root"); //file above copied in files/ directory (file above doesn't exist when submitting with crab)
	}
	std::string pudata_down("/afs/cern.ch/cms/CAF/CMSCOMM/COMM_DQM/certification/Collisions16/13TeV/PileUp/UltraLegacy/PileupHistogram-goldenJSON-13tev-2016-66000ub-99bins.root");
        if(gSystem->AccessPathName(gSystem->ExpandPathName(pudata_down.c_str()))) {
                pudata_down=std::string("$CMSSW_BASE/PileupHistogram-goldenJSON-13tev-2016-66000ub-99bins.root");
        }
	std::string pudata_up("/afs/cern.ch/cms/CAF/CMSCOMM/COMM_DQM/certification/Collisions16/13TeV/PileUp/UltraLegacy/PileupHistogram-goldenJSON-13tev-2016-72400ub-99bins.root");
        if(gSystem->AccessPathName(gSystem->ExpandPathName(pudata_up.c_str()))) {
                pudata_up=std::string("$CMSSW_BASE/PileupHistogram-goldenJSON-13tev-2016-72400ub-99bins.root");
        }
	std::string puhistomc("pileup_mc");
	std::string puhistodata("pileup");
	lumiWeights_=new edm::LumiReWeighting(pumc,pudata,puhistomc,puhistodata);
	lumiWeightsup_=new edm::LumiReWeighting(pumc,pudata_up,puhistomc,puhistodata);
	lumiWeightsdown_=new edm::LumiReWeighting(pumc,pudata_down,puhistomc,puhistodata);
	edm::Service<TFileService> fs;
	tree_=fs->make<TTree>("Events","Events");
	tree_->Branch("nMuon", &nMuon_, "nMuon/i");
	tree_->Branch("Muon_pt",&Muon_pt_,"Muon_pt[nMuon]/F");
	tree_->Branch("Muon_eta",&Muon_eta_,"Muon_eta[nMuon]/F");
	tree_->Branch("Muon_phi",&Muon_phi_,"Muon_phi[nMuon]/F");
	tree_->Branch("Muon_mass",&Muon_mass_,"Muon_mass[nMuon]/F");
	tree_->Branch("Muon_standpt",&Muon_standpt_,"Muon_standpt[nMuon]/F");
	tree_->Branch("Muon_standeta",&Muon_standeta_,"Muon_standeta[nMuon]/F");
	tree_->Branch("Muon_standphi",&Muon_standphi_,"Muon_standphi[nMuon]/F");
	tree_->Branch("Muon_pfRelIso04_all",&Muon_pfRelIso04_all_,"Muon_pfRelIso04_all[nMuon]/F");
	tree_->Branch("Muon_pfRelIso04_chgPV",&Muon_pfRelIso04_chgPV_,"Muon_pfRelIso04_chgPV[nMuon]/F");
	tree_->Branch("Muon_pfRelIso04_chgPU",&Muon_pfRelIso04_chgPU_,"Muon_pfRelIso04_chgPU[nMuon]/F");
	tree_->Branch("Muon_pfRelIso04_nhad",&Muon_pfRelIso04_nhad_,"Muon_pfRelIso04_nhad[nMuon]/F");
	tree_->Branch("Muon_pfRelIso04_pho",&Muon_pfRelIso04_pho_,"Muon_pfRelIso04_pho[nMuon]/F");
	tree_->Branch("Muon_pfRelIso03_all",&Muon_pfRelIso03_all_,"Muon_pfRelIso03_all[nMuon]/F");
	tree_->Branch("Muon_pfRelIso03_chgPV",&Muon_pfRelIso03_chgPV_,"Muon_pfRelIso03_chgPV[nMuon]/F");
	tree_->Branch("Muon_pfRelIso03_chgPU",&Muon_pfRelIso03_chgPU_,"Muon_pfRelIso03_chgPU[nMuon]/F");
	tree_->Branch("Muon_pfRelIso03_nhad",&Muon_pfRelIso03_nhad_,"Muon_pfRelIso03_nhad[nMuon]/F");
	tree_->Branch("Muon_pfRelIso03_pho",&Muon_pfRelIso03_pho_,"Muon_pfRelIso03_pho[nMuon]/F");
	tree_->Branch("Muon_tkRelIso",&Muon_tkRelIso_,"Muon_tkRelIso[nMuon]/F");
	tree_->Branch("Muon_dxy",&Muon_dxy_,"Muon_dxy[nMuon]/F");
	tree_->Branch("Muon_dxyErr",&Muon_dxyErr_,"Muon_dxyErr[nMuon]/F");
	tree_->Branch("Muon_dz",&Muon_dz_,"Muon_dz[nMuon]/F");
	tree_->Branch("Muon_dzErr",&Muon_dzErr_,"Muon_dzErr[nMuon]/F");
	tree_->Branch("Muon_dxyBS",&Muon_dxyBS_,"Muon_dxyBS[nMuon]/F");
	tree_->Branch("Muon_dzBS",&Muon_dzBS_,"Muon_dzBS[nMuon]/F");
	tree_->Branch("Muon_isTracker",&Muon_isTracker_,"Muon_isTracker[nMuon]/O");
	tree_->Branch("Muon_isGlobal",&Muon_isGlobal_,"Muon_isGlobal[nMuon]/O"); 
	tree_->Branch("Muon_isStandalone",&Muon_isStandalone_,"Muon_isStandalone[nMuon]/O");
	tree_->Branch("Muon_looseId",&Muon_looseId_,"Muon_looseId[nMuon]/O");
	tree_->Branch("Muon_mediumId",&Muon_mediumId_,"Muon_mediumId[nMuon]/O");
	tree_->Branch("Muon_mediumPromptId",&Muon_mediumPromptId_,"Muon_mediumPromptId[nMuon]/O");
	tree_->Branch("Muon_tightId",&Muon_tightId_,"Muon_tightId[nMuon]/O");
	tree_->Branch("Muon_softId",&Muon_softId_,"Muon_softId[nMuon]/O");
	tree_->Branch("Muon_isPF",&Muon_isPF_,"Muon_isPF[nMuon]/O");
	tree_->Branch("Muon_softMvaId",&Muon_softMvaId_,"Muon_softMvaId[nMuon]/O");
	tree_->Branch("Muon_charge",&Muon_charge_,"Muon_charge[nMuon]/I");
	tree_->Branch("Muon_BestTrackAlgo",&Muon_BestTrackAlgo_,"Muon_BestTrackAlgo[nMuon]/I");
	tree_->Branch("Muon_InnerTrackAlgo",&Muon_InnerTrackAlgo_,"Muon_InnerTrackAlgo[nMuon]/I");
	tree_->Branch("Muon_GlobalTrackAlgo",&Muon_GlobalTrackAlgo_,"Muon_GlobalTrackAlgo[nMuon]/I");
	tree_->Branch("Muon_BestTrackOriginalAlgo",&Muon_BestTrackOriginalAlgo_,"Muon_BestTrackOriginalAlgo[nMuon]/I");
	tree_->Branch("Muon_InnerTrackOriginalAlgo",&Muon_InnerTrackOriginalAlgo_,"Muon_InnerTrackOriginalAlgo[nMuon]/I");
	tree_->Branch("Muon_GlobalTrackOriginalAlgo",&Muon_GlobalTrackOriginalAlgo_,"Muon_GlobalTrackOriginalAlgo[nMuon]/I");
	tree_->Branch("Muon_highPtId",&Muon_highPtId_,"Muon_highPtId[nMuon]/b");
	tree_->Branch("Muon_miniIsoId",&Muon_miniIsoId_,"Muon_miniIsoId[nMuon]/b");
	tree_->Branch("Muon_multiIsoId",&Muon_multiIsoId_,"Muon_multiIsoId[nMuon]/b");
	tree_->Branch("Muon_mvaId",&Muon_mvaId_,"Muon_mvaId[nMuon]/b");
	tree_->Branch("Muon_mvaLowPtId",&Muon_mvaLowPtId_,"Muon_mvaLowPtId[nMuon]/b");
	tree_->Branch("Muon_pfIsoId",&Muon_pfIsoId_,"Muon_pfIsoId[nMuon]/b");
	tree_->Branch("Muon_pfIsoLoose",&Muon_pfIsoLoose_,"Muon_pfIsoLoose[nMuon]/O");
	tree_->Branch("Muon_pfIsoMedium",&Muon_pfIsoMedium_,"Muon_pfIsoMedium[nMuon]/O");
	tree_->Branch("Muon_pfIsoTight",&Muon_pfIsoTight_,"Muon_pfIsoTight[nMuon]/O");
	tree_->Branch("Muon_tkIsoId",&Muon_tkIsoId_,"Muon_tkIsoId[nMuon]/b");
	tree_->Branch("Muon_genPartIdx",&Muon_genPartIdx_,"Muon_genPartIdx[nMuon]/I");
	tree_->Branch("Muon_genPartPreFSRIdx",&Muon_genPartPreFSRIdx_,"Muon_genPartPreFSRIdx[nMuon]/I");
	tree_->Branch("Muon_triggered",&Muon_triggered_,"Muon_triggered[nMuon]/O");
	tree_->Branch("Muon_closestVtx_X",&Muon_closestVtx_X_,"Muon_closestVtx_X[nMuon]/F");
	tree_->Branch("Muon_closestVtx_Y",&Muon_closestVtx_Y_,"Muon_closestVtx_Y[nMuon]/F");
	tree_->Branch("Muon_closestVtx_Z",&Muon_closestVtx_Z_,"Muon_closestVtx_Z[nMuon]/F");
	tree_->Branch("PV_chi2",&PV_chi2_,"PV_chi2/F");
	tree_->Branch("PV_ndof",&PV_ndof_,"PV_ndof/F"); 
	tree_->Branch("PV_score",&PV_score_,"PV_score/F");
	tree_->Branch("PV_x",&PV_x_,"PV_x/F");
	tree_->Branch("PV_y",&PV_y_,"PV_y/F");
	tree_->Branch("PV_z",&PV_z_,"PV_z/F");
	if (isAOD_) {
		tree_->Branch("nTrack",&nTrack_,"nTrack/i");
		tree_->Branch("Track_pt",&Track_pt_,"Track_pt[nTrack]/F");
		tree_->Branch("Track_eta",&Track_eta_,"Track_eta[nTrack]/F");
		tree_->Branch("Track_phi",&Track_phi_,"Track_phi[nTrack]/F");
		tree_->Branch("Track_algo",&Track_algo_,"Track_algo[nTrack]/F");
		tree_->Branch("Track_originalAlgo",&Track_originalAlgo_,"Track_originalAlgo[nTrack]/F");
		tree_->Branch("Track_chi2",&Track_chi2_,"Track_chi2[nTrack]/F");
		tree_->Branch("Track_ndof",&Track_ndof_,"Track_ndof[nTrack]/F");
		tree_->Branch("Track_charge",&Track_charge_,"Track_charge[nTrack]/F");
		tree_->Branch("Track_dxy",&Track_dxy_,"Track_dxy[nTrack]/F");
		tree_->Branch("Track_dxyErr",&Track_dxyErr_,"Track_dxyErr[nTrack]/F");
		tree_->Branch("Track_dz",&Track_dz_,"Track_dz[nTrack]/F");
		tree_->Branch("Track_dzErr",&Track_dzErr_,"Track_dzErr[nTrack]/F");
		tree_->Branch("Track_dxyBS",&Track_dxyBS_,"Track_dxyBS[nTrack]/F");
		tree_->Branch("Track_dzBS",&Track_dzBS_,"Track_dzBS[nTrack]/F");
		tree_->Branch("Track_closestVtx_X",&Track_closestVtx_X_,"Track_closestVtx_X[nTrack]/F");
		tree_->Branch("Track_closestVtx_Y",&Track_closestVtx_Y_,"Track_closestVtx_Y[nTrack]/F");
		tree_->Branch("Track_closestVtx_Z",&Track_closestVtx_Z_,"Track_closestVtx_Z[nTrack]/F");
	}
	tree_->Branch("BeamSpot_x0",&BeamSpot_x0_,"BeamSpot_x0/F");
	tree_->Branch("BeamSpot_x0Err",&BeamSpot_x0Err_,"BeamSpot_x0Err_/F");
	tree_->Branch("BeamSpot_y0",&BeamSpot_y0_,"BeamSpot_y0/F");
	tree_->Branch("BeamSpot_y0Err",&BeamSpot_y0Err_,"BeamSpot_y0Err/F");
	tree_->Branch("BeamSpot_z0",&BeamSpot_z0_,"BeamSpot_z0/F");
	tree_->Branch("BeamSpot_z0Err",&BeamSpot_z0Err_,"BeamSpot_z0Err/F");
	tree_->Branch("BeamSpot_dxdz",&BeamSpot_dxdz_,"BeamSpot_dxdz/F");
	tree_->Branch("BeamSpot_dxdzErr",&BeamSpot_dxdzErr_,"BeamSpot_dxdzErr/F");
	tree_->Branch("BeamSpot_dydz",&BeamSpot_dydz_,"BeamSpot_dydz/F");
	tree_->Branch("BeamSpot_dydzErr",&BeamSpot_dydzErr_,"BeamSpot_dydzErr/F");
	tree_->Branch("BeamSpot_sigmaZ",&BeamSpot_sigmaZ_,"BeamSpot_sigmaZ/F");
	tree_->Branch("BeamSpot_sigmaZErr",&BeamSpot_sigmaZErr_,"BeamSpot_sigmaZErr/F");
	tree_->Branch("GenVertex_x",&GenVertex_x_,"GenVertex_x/F");
	tree_->Branch("GenVertex_y",&GenVertex_y_,"GenVertex_y/F");
	tree_->Branch("GenVertex_z",&GenVertex_z_,"GenVertex_z/F");
	tree_->Branch("nGenPart",&nGenPart_,"nGenPart/i");
	tree_->Branch("nGenPartPreFSR",&nGenPartPreFSR_,"nGenPartPreFSR/i");
	tree_->Branch("nGenPart746",&nGenPart746_,"nGenPart746/i");
	tree_->Branch("nGenMuonPreFSR",&nGenMuonPreFSR_,"nGenMuonPreFSR/i");
	tree_->Branch("nGenPartPostFSR",&nGenPartPostFSR_,"nGenPartPostFSR/i");
	tree_->Branch("GenPart_eta",&GenPart_eta_,"GenPart_eta[nGenPart]/F");
	tree_->Branch("GenPart_mass",&GenPart_mass_,"GenPart_mass[nGenPart]/F");
	tree_->Branch("GenPart_phi",&GenPart_phi_,"GenPart_phi[nGenPart]/F");
	tree_->Branch("GenPart_pt",&GenPart_pt_,"GenPart_pt[nGenPart]/F");
	tree_->Branch("GenPart_genPartIdxMother",&GenPart_genPartIdxMother_,"GenPart_genPartIdxMother[nGenPart]/I");
	tree_->Branch("GenPart_pdgId",&GenPart_pdgId_,"GenPart_pdgId[nGenPart]/I");
	tree_->Branch("GenPart_status",&GenPart_status_,"GenPart_status[nGenPart]/I");
	tree_->Branch("GenPart_statusFlags",&GenPart_statusFlags_,"GenPart_statusFlags[nGenPart]/I");
	tree_->Branch("GenPart_preFSRLepIdx1",&GenPart_preFSRLepIdx1_,"GenPart_preFSRLepIdx1/I");
	tree_->Branch("GenPart_preFSRLepIdx2",&GenPart_preFSRLepIdx2_,"GenPart_preFSRLepIdx2/I");
	tree_->Branch("GenPart_postFSRLepIdx1",&GenPart_postFSRLepIdx1_,"GenPart_postFSRLepIdx1/I");
	tree_->Branch("GenPart_postFSRLepIdx2",&GenPart_postFSRLepIdx2_,"GenPart_postFSRLepIdx2/I");
	tree_->Branch("GenPart_PostFSR", &GenPart_PostFSR_,"GenPart_PostFSR[nGenPartPostFSR]/I");
	tree_->Branch("Generator_weight",&Generator_weight_,"Generator_weight/F");
	tree_->Branch("Pileup_nTrueInt",&Pileup_nTrueInt_,"Pileup_nTrueInt/F");
	tree_->Branch("puWeight",&puWeight_,"puWeight/F");
	tree_->Branch("puWeight_Up",&puWeight_Up_,"puWeight_Up/F");
	tree_->Branch("puWeight_Down",&puWeight_Down_,"puWeight_Down/F");
	tree_->Branch("L1PreFiringWeightpreVFP_Nom",&L1PreFiringWeightpreVFP_Nom,"L1PreFiringWeightpreVFP_Nom/F");
	tree_->Branch("L1PreFiringWeightpreVFP_Up",&L1PreFiringWeightpreVFP_Up,"L1PreFiringWeightpreVFP_Up/F");
	tree_->Branch("L1PreFiringWeightpreVFP_Dn",&L1PreFiringWeightpreVFP_Dn,"L1PreFiringWeightpreVFP_Dn/F");
	tree_->Branch("L1PreFiringWeightECALpreVFP_Nom",&L1PreFiringWeightECALpreVFP_Nom,"L1PreFiringWeightECALpreVFP_Nom/F");
	tree_->Branch("L1PreFiringWeightECALpreVFP_Up",&L1PreFiringWeightECALpreVFP_Up,"L1PreFiringWeightECALpreVFP_Up/F");
	tree_->Branch("L1PreFiringWeightECALpreVFP_Dn",&L1PreFiringWeightECALpreVFP_Dn,"L1PreFiringWeightECALpreVFP_Dn/F");
	tree_->Branch("L1PreFiringWeightMuonpreVFP_Nom",&L1PreFiringWeightMuonpreVFP_Nom,"L1PreFiringWeightMuonpreVFP_Nom/F");
	tree_->Branch("L1PreFiringWeightMuonpreVFP_Up",&L1PreFiringWeightMuonpreVFP_Up,"L1PreFiringWeightMuonpreVFP_Up/F");
	tree_->Branch("L1PreFiringWeightMuonpreVFP_Dn",&L1PreFiringWeightMuonpreVFP_Dn,"L1PreFiringWeightMuonpreVFP_Dn/F");
	tree_->Branch("L1PreFiringWeightMuonpreVFP_statUp",&L1PreFiringWeightMuonpreVFP_statUp,"L1PreFiringWeightMuonpreVFP_statUp/F");
	tree_->Branch("L1PreFiringWeightMuonpreVFP_statDn",&L1PreFiringWeightMuonpreVFP_statDn,"L1PreFiringWeightMuonpreVFP_statDn/F");
	tree_->Branch("L1PreFiringWeightMuonpreVFP_systUp",&L1PreFiringWeightMuonpreVFP_systUp,"L1PreFiringWeightMuonpreVFP_systUp/F");
	tree_->Branch("L1PreFiringWeightMuonpreVFP_systDn",&L1PreFiringWeightMuonpreVFP_systDn,"L1PreFiringWeightMuonpreVFP_systDn/F");
	tree_->Branch("L1PreFiringWeightpostVFP_Nom",&L1PreFiringWeightpostVFP_Nom,"L1PreFiringWeightpostVFP_Nom/F");
	tree_->Branch("L1PreFiringWeightpostVFP_Up",&L1PreFiringWeightpostVFP_Up,"L1PreFiringWeightpostVFP_Up/F");
	tree_->Branch("L1PreFiringWeightpostVFP_Dn",&L1PreFiringWeightpostVFP_Dn,"L1PreFiringWeightpostVFP_Dn/F");
	tree_->Branch("L1PreFiringWeightECALpostVFP_Nom",&L1PreFiringWeightECALpostVFP_Nom,"L1PreFiringWeightECALpostVFP_Nom/F");
	tree_->Branch("L1PreFiringWeightECALpostVFP_Up",&L1PreFiringWeightECALpostVFP_Up,"L1PreFiringWeightECALpostVFP_Up/F");
	tree_->Branch("L1PreFiringWeightECALpostVFP_Dn",&L1PreFiringWeightECALpostVFP_Dn,"L1PreFiringWeightECALpostVFP_Dn/F");
	tree_->Branch("L1PreFiringWeightMuonpostVFP_Nom",&L1PreFiringWeightMuonpostVFP_Nom,"L1PreFiringWeightMuonpostVFP_Nom/F");
	tree_->Branch("L1PreFiringWeightMuonpostVFP_Up",&L1PreFiringWeightMuonpostVFP_Up,"L1PreFiringWeightMuonpostVFP_Up/F");
	tree_->Branch("L1PreFiringWeightMuonpostVFP_Dn",&L1PreFiringWeightMuonpostVFP_Dn,"L1PreFiringWeightMuonpostVFP_Dn/F");
	tree_->Branch("L1PreFiringWeightMuonpostVFP_statUp",&L1PreFiringWeightMuonpostVFP_statUp,"L1PreFiringWeightMuonpostVFP_statUp/F");
	tree_->Branch("L1PreFiringWeightMuonpostVFP_statDn",&L1PreFiringWeightMuonpostVFP_statDn,"L1PreFiringWeightMuonpostVFP_statDn/F");
	tree_->Branch("L1PreFiringWeightMuonpostVFP_systUp",&L1PreFiringWeightMuonpostVFP_systUp,"L1PreFiringWeightMuonpostVFP_systUp/F");
	tree_->Branch("L1PreFiringWeightMuonpostVFP_systDn",&L1PreFiringWeightMuonpostVFP_systDn,"L1PreFiringWeightMuonpostVFP_systDn/F");
	tree_->Branch("nLHEPdfWeight",&nLHEPdfWeight_,"nLHEPdfWeight/i");
	tree_->Branch("LHEPdfWeight",&LHEPdfWeight_,"LHEPdfWeight[nLHEPdfWeight]/F");
	tree_->Branch("nLHEScaleWeight",&nLHEScaleWeight_,"nLHEScaleWeight/i");
	tree_->Branch("LHEScaleWeight",&LHEScaleWeight_,"LHEScaleWeight[nLHEScaleWeight]/F");
	tree_->Branch("HLT_IsoMu24",&HLT_IsoMu24_,"HLT_IsoMu24/O");
	tree_->Branch("HLT_IsoTkMu24",&HLT_IsoTkMu24_,"HLT_IsoTkMu24/O");
	tree_->Branch("MET_pt",&MET_pt_,"MET_pt/F");
	tree_->Branch("MET_phi",&MET_phi_,"MET_phi/F");
	tree_->Branch("MET_JECUp_pt",&MET_JECUp_pt_,"MET_JECUp_pt/F");
	tree_->Branch("MET_JECDown_pt",&MET_JECDown_pt_,"MET_JECDown_pt/F");
	tree_->Branch("MET_JECUp_phi",&MET_JECUp_phi_,"MET_JECUp_phi/F");
	tree_->Branch("MET_JECDown_phi",&MET_JECDown_phi_,"MET_JECDown_phi/F");
	tree_->Branch("MET_JERUp_pt",&MET_JERUp_pt_,"MET_JERUp_pt/F");
	tree_->Branch("MET_JERDown_pt",&MET_JERDown_pt_,"MET_JERDown_pt/F");
	tree_->Branch("MET_JERUp_phi",&MET_JERUp_phi_,"MET_JERUp_phi/F");
	tree_->Branch("MET_JERDown_phi",&MET_JERDown_phi_,"MET_JERDown_phi/F");
	tree_->Branch("MET_UnclEnUp_pt",&MET_UnclEnUp_pt_,"MET_UnclEnUp_pt/F");
	tree_->Branch("MET_UnclEnDown_pt",&MET_UnclEnDown_pt_,"MET_UnclEnDown_pt/F");
	tree_->Branch("MET_UnclEnUp_phi",&MET_UnclEnUp_phi_,"MET_UnclEnUp_phi/F");
	tree_->Branch("MET_UnclEnDown_phi",&MET_UnclEnDown_phi_,"MET_UnclEnDown_phi/F");
	tree_->SetAutoSave(0);
}


MuonAnalysis::~MuonAnalysis()
{

	// do anything here that needs to be done at desctruction time
	// (e.g. close files, deallocate resources etc.)

}


//
// member functions
//

// ------------ method called for each event  ------------
void
MuonAnalysis::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{
	using namespace edm;
	UInt_t i=0; 
	reco::Vertex primaryvertex=(iEvent.get(vertexToken_))[0];
	reco::BeamSpot beamspot=iEvent.get(beamspotToken_);
	ROOT::Math::PositionVector3D<ROOT::Math::Cartesian3D<float>,ROOT::Math::DefaultCoordinateSystemTag> genvertex=iEvent.get(genvertexToken_);
	getGenLeptonIdxandFill(iEvent.get(genparticleToken_), GenPart_eta_, GenPart_mass_, GenPart_phi_, GenPart_pt_, GenPart_genPartIdxMother_, GenPart_pdgId_, GenPart_status_, GenPart_statusFlags_, GenPart_preFSRLepIdx1_, GenPart_preFSRLepIdx2_, GenPart_postFSRLepIdx1_, GenPart_postFSRLepIdx2_, nGenPart_, nGenPartPreFSR_, nGenMuonPreFSR_, nGenPart746_, nGenPartPostFSR_, GenPart_PostFSR_);
	std::vector<LHEEventProduct::WGT> weights=iEvent.get(lheinfoToken_).weights();
	Float_t originalweight=iEvent.get(lheinfoToken_).originalXWGTUP();
	for (std::vector<LHEEventProduct::WGT>::const_iterator it=weights.begin(); it!=weights.end(); it++) {
		if (it->id=="1009") LHEScaleWeight_[0]=it->wgt/originalweight;
		if (it->id=="1007") LHEScaleWeight_[1]=it->wgt/originalweight;
		if (it->id=="1008") LHEScaleWeight_[2]=it->wgt/originalweight;
		if (it->id=="1003") LHEScaleWeight_[3]=it->wgt/originalweight;
		if (it->id=="1001") LHEScaleWeight_[4]=it->wgt/originalweight;
		if (it->id=="1002") LHEScaleWeight_[5]=it->wgt/originalweight;
		if (it->id=="1006") LHEScaleWeight_[6]=it->wgt/originalweight;
		if (it->id=="1004") LHEScaleWeight_[7]=it->wgt/originalweight;
		if (it->id=="1005") LHEScaleWeight_[8]=it->wgt/originalweight;
		if ((it-weights.begin()>=18)&&(it-weights.begin()<=120)) LHEPdfWeight_[(it-weights.begin())-18]=it->wgt/originalweight;
	}
	nLHEScaleWeight_=9;
	nLHEPdfWeight_=103;
	for (const auto& muon : iEvent.get(muonToken_)) {
		// do something with track parameters, e.g, plot the charge.
		// int charge = track.charge();
		Muon_pt_[i]=muon.pt();
		Muon_eta_[i]=muon.eta();
		Muon_phi_[i]=muon.phi();
		Muon_mass_[i]=muon.mass();
		Muon_standpt_[i]=muon.standAloneMuon().isNonnull() ? muon.standAloneMuon()->pt() : -1;
		Muon_standeta_[i]=muon.standAloneMuon().isNonnull() ? muon.standAloneMuon()->eta() : -99;
		Muon_standphi_[i]=muon.standAloneMuon().isNonnull() ? muon.standAloneMuon()->phi() : -99;
		Muon_pfRelIso04_chgPV_[i]=muon.pfIsolationR04().sumChargedHadronPt/Muon_pt_[i];
		Muon_pfRelIso04_nhad_[i]=muon.pfIsolationR04().sumNeutralHadronEt/Muon_pt_[i]; 
		Muon_pfRelIso04_pho_[i]=muon.pfIsolationR04().sumPhotonEt/Muon_pt_[i];
		Muon_pfRelIso04_chgPU_[i]=muon.pfIsolationR04().sumPUPt/Muon_pt_[i];
		Muon_pfRelIso04_all_[i]=(muon.pfIsolationR04().sumChargedHadronPt+std::max(0.,muon.pfIsolationR04().sumNeutralHadronEt+muon.pfIsolationR04().sumPhotonEt-0.5*muon.pfIsolationR04().sumPUPt))/Muon_pt_[i];
		Muon_pfRelIso03_chgPV_[i]=muon.pfIsolationR03().sumChargedHadronPt/Muon_pt_[i];
		Muon_pfRelIso03_nhad_[i]=muon.pfIsolationR03().sumNeutralHadronEt/Muon_pt_[i];
		Muon_pfRelIso03_pho_[i]=muon.pfIsolationR03().sumPhotonEt/Muon_pt_[i];
		Muon_pfRelIso03_chgPU_[i]=muon.pfIsolationR03().sumPUPt/Muon_pt_[i];
		Muon_pfRelIso03_all_[i]=(muon.pfIsolationR03().sumChargedHadronPt+std::max(0.,muon.pfIsolationR03().sumNeutralHadronEt+muon.pfIsolationR03().sumPhotonEt-0.5*muon.pfIsolationR03().sumPUPt))/Muon_pt_[i];
		Muon_tkRelIso_[i]=muon.isolationR03().sumPt/Muon_pt_[i];
		Muon_dxy_[i]=muon.muonBestTrack()->dxy(primaryvertex.position());
		Muon_dxyErr_[i]=muon.muonBestTrack()->dxyError(primaryvertex.position(),primaryvertex.covariance());
		Muon_dz_[i]=muon.muonBestTrack()->dz(primaryvertex.position());
		Muon_dzErr_[i]=muon.muonBestTrack()->dzError();
		Muon_dxyBS_[i]=muon.muonBestTrack()->dxy(beamspot.position());;
		Muon_dzBS_[i]=muon.muonBestTrack()->dz(beamspot.position());
		Muon_isTracker_[i]=muon.isTrackerMuon();
		Muon_isGlobal_[i]=muon.isGlobalMuon();
		Muon_isStandalone_[i]=muon.isStandAloneMuon();
		Muon_looseId_[i]=muon.isLooseMuon();
		Muon_mediumId_[i]=muon.isMediumMuon();
		Muon_mediumPromptId_[i]=muon.passed(muon.CutBasedIdMediumPrompt);
		Muon_tightId_[i]=muon.isTightMuon(primaryvertex);
		Muon_softId_[i]=muon.isSoftMuon(primaryvertex);
		Muon_isPF_[i]=muon.isPFMuon();
		Muon_softMvaId_[i]=0;
		if (muon.passed(reco::Muon::SoftMvaId)) Muon_softMvaId_[i]=1;
		Muon_charge_[i]=muon.charge();
		Muon_BestTrackAlgo_[i]=muon.muonBestTrack()->algo();
		Muon_BestTrackOriginalAlgo_[i]=muon.muonBestTrack()->originalAlgo();
		if (!(muon.innerTrack().isNull())) {
			Muon_InnerTrackAlgo_[i]=muon.innerTrack()->algo();
			Muon_InnerTrackOriginalAlgo_[i]=muon.innerTrack()->originalAlgo();
		}
		else {
			Muon_InnerTrackAlgo_[i]=-1;
			Muon_InnerTrackOriginalAlgo_[i]=-1;
		}
		if (!(muon.globalTrack().isNull())) {
			Muon_GlobalTrackAlgo_[i]=muon.globalTrack()->algo();
			Muon_GlobalTrackOriginalAlgo_[i]=muon.globalTrack()->originalAlgo();
		}
		else {
			Muon_GlobalTrackAlgo_[i]=-1;
			Muon_GlobalTrackOriginalAlgo_[i]=-1;
		}
		Muon_highPtId_[i]=0;
		if (muon.isHighPtMuon(primaryvertex)) Muon_highPtId_[i]=2;
		//if (muon.isTrackerHighPtMuon(primaryvertex)) Muon_highPtId_[i]=1;
		Muon_miniIsoId_[i]=0;
		if (muon.passed(reco::Muon::MiniIsoLoose)) Muon_miniIsoId_[i]=1;
		if (muon.passed(reco::Muon::MiniIsoMedium)) Muon_miniIsoId_[i]=2;
		if (muon.passed(reco::Muon::MiniIsoTight)) Muon_miniIsoId_[i]=3;
		if (muon.passed(reco::Muon::MiniIsoVeryTight)) Muon_miniIsoId_[i]=4;
		Muon_multiIsoId_[i]=0;
		if (muon.passed(reco::Muon::MultiIsoLoose)) Muon_multiIsoId_[i]=1;
		if (muon.passed(reco::Muon::MultiIsoMedium)) Muon_multiIsoId_[i]=2;
		Muon_mvaId_[i]=0;
		if (muon.passed(reco::Muon::MvaLoose)) Muon_mvaId_[i]=1;
		if (muon.passed(reco::Muon::MvaMedium)) Muon_mvaId_[i]=2;
		if (muon.passed(reco::Muon::MvaTight)) Muon_mvaId_[i]=3;
		if (muon.passed(reco::Muon::MvaVTight)) Muon_mvaId_[i]=4;
		if (muon.passed(reco::Muon::MvaVVTight)) Muon_mvaId_[i]=5;
		Muon_mvaLowPtId_[i]=0;
		if (muon.passed(reco::Muon::LowPtMvaLoose)) Muon_mvaLowPtId_[i]=1;
		if (muon.passed(reco::Muon::LowPtMvaMedium)) Muon_mvaLowPtId_[i]=2;
		Muon_pfIsoId_[i]=0;
		if (muon.passed(reco::Muon::PFIsoVeryLoose)) Muon_pfIsoId_[i]=1;
		if (muon.passed(reco::Muon::PFIsoLoose)) Muon_pfIsoId_[i]=2;
		if (muon.passed(reco::Muon::PFIsoMedium)) Muon_pfIsoId_[i]=3;
		if (muon.passed(reco::Muon::PFIsoTight)) Muon_pfIsoId_[i]=4;
		if (muon.passed(reco::Muon::PFIsoVeryTight)) Muon_pfIsoId_[i]=5;
		if (muon.passed(reco::Muon::PFIsoVeryVeryTight)) Muon_pfIsoId_[i]=6;
		Muon_tkIsoId_[i]=0;
		if (muon.passed(reco::Muon::TkIsoLoose)) Muon_tkIsoId_[i]=1;
		if (muon.passed(reco::Muon::TkIsoTight)) Muon_tkIsoId_[i]=2;
		Muon_genPartIdx_[i]=-1;
		Muon_genPartPreFSRIdx_[i]=-1;
		int j=0;
		for (const auto& genparticle : iEvent.get(genparticleToken_)) {
			if (abs(genparticle.pdgId())==13)
				if (genparticle.status()==1) 
					if (muon.genLepton()==&genparticle)
						Muon_genPartIdx_[i]=j;
			j++;
		}
		if (Muon_genPartIdx_[i]>0) {
			if ((Muon_genPartIdx_[i]==GenPart_postFSRLepIdx1_)||(Muon_genPartIdx_[i]==GenPart_postFSRLepIdx2_)) {
				int pdgid=iEvent.get(genparticleToken_)[Muon_genPartIdx_[i]].pdgId();
				if (pdgid==iEvent.get(genparticleToken_)[GenPart_preFSRLepIdx1_].pdgId())
					Muon_genPartPreFSRIdx_[i]=GenPart_preFSRLepIdx1_;
				if (pdgid==iEvent.get(genparticleToken_)[GenPart_preFSRLepIdx2_].pdgId())
					Muon_genPartPreFSRIdx_[i]=GenPart_preFSRLepIdx2_;
			}
		}
		Muon_pfIsoLoose_[i]=muon.passed(muon.PFIsoLoose);
		Muon_pfIsoMedium_[i]=muon.passed(muon.PFIsoMedium);
		Muon_pfIsoTight_[i]=muon.passed(muon.PFIsoTight);
		Muon_triggered_[i]=false;
		if ((muon.triggered("HLT_IsoMu24_v4"))||(muon.triggered("HLT_IsoTkMu24_v4"))) Muon_triggered_[i]=true;
		reco::Vertex muonvtx=primaryvertex;
		for (const auto vtx : iEvent.get(vertexToken_)) {
			if (abs(muon.muonBestTrack()->dz(vtx.position()))<abs(muon.muonBestTrack()->dz(muonvtx.position()))) {
				muonvtx=vtx;
			}
		}
		Muon_closestVtx_X_[i]=muonvtx.x();
		Muon_closestVtx_Y_[i]=muonvtx.y();
		Muon_closestVtx_Z_[i]=muonvtx.z();
		i++;
	}
	nMuon_=i;
	i=0;
	if (isAOD_) {
		for (const auto& track : iEvent.get(trackToken_)) {
			if (track.pt()<15) continue;
			Track_pt_[i]=track.pt();
			Track_eta_[i]=track.eta();
			Track_phi_[i]=track.phi();
			Track_algo_[i]=track.algo();
			Track_originalAlgo_[i]=track.originalAlgo();
			Track_chi2_[i]=track.normalizedChi2();
			Track_ndof_[i]=track.ndof();
			Track_charge_[i]=track.charge();
			Track_dxy_[i]=track.dxy(primaryvertex.position());
			Track_dxyErr_[i]=track.dxyError(primaryvertex.position(),primaryvertex.covariance());
			Track_dz_[i]=track.dz(primaryvertex.position());
			Track_dzErr_[i]=track.dzError();
			Track_dxyBS_[i]=track.dxy(beamspot.position());
			Track_dzBS_[i]=track.dz(beamspot.position());
			reco::Vertex trackvtx=primaryvertex;
			for (const auto vtx : iEvent.get(vertexToken_)) {
				if (abs(track.dz(vtx.position()))<abs(track.dz(trackvtx.position()))) {
					trackvtx=vtx;
				}
			}
			Track_closestVtx_X_[i]=trackvtx.x();
			Track_closestVtx_Y_[i]=trackvtx.y();
			Track_closestVtx_Z_[i]=trackvtx.z();
			i++;
		}
	}
	nTrack_=i;
	PV_x_=primaryvertex.x();
	PV_y_=primaryvertex.y();
	PV_z_=primaryvertex.z();
	PV_chi2_=primaryvertex.normalizedChi2();
	PV_ndof_=primaryvertex.ndof();
	BeamSpot_x0_=beamspot.x0();
	BeamSpot_x0Err_=beamspot.x0Error();
	BeamSpot_y0_=beamspot.y0();
	BeamSpot_y0Err_=beamspot.y0Error();
	BeamSpot_z0_=beamspot.z0();
	BeamSpot_z0Err_=beamspot.z0Error();
	BeamSpot_dxdz_=beamspot.dxdz();
	BeamSpot_dxdzErr_=beamspot.dxdzError();
	BeamSpot_dydz_=beamspot.dydz();
	BeamSpot_dydzErr_=beamspot.dydzError();
	BeamSpot_sigmaZ_=beamspot.sigmaZ();
	BeamSpot_sigmaZErr_=beamspot.sigmaZ0Error();
	GenVertex_x_=genvertex.x();
	GenVertex_y_=genvertex.y();
	GenVertex_z_=genvertex.z();
	Generator_weight_=iEvent.get(geneventinfoToken_).weight();
	float npv = -1;
	for (std::vector<PileupSummaryInfo>::const_iterator pvi = iEvent.get(puToken_).begin(); pvi != iEvent.get(puToken_).end(); ++pvi) {
		int bx = pvi->getBunchCrossing();
		if (bx == 0) {
		npv = pvi->getTrueNumInteractions();
		continue;
		}
	}
	Pileup_nTrueInt_=npv;
	puWeight_=lumiWeights_->weight(npv);
	puWeight_Up_=lumiWeightsup_->weight(npv);
	puWeight_Down_=lumiWeightsdown_->weight(npv);
	L1PreFiringWeightpreVFP_Nom=iEvent.get(prefweightpreVFP_token);
	L1PreFiringWeightpreVFP_Up=iEvent.get(prefweightpreVFPup_token);
	L1PreFiringWeightpreVFP_Dn=iEvent.get(prefweightpreVFPdown_token);
	L1PreFiringWeightECALpreVFP_Nom=iEvent.get(prefweightpreVFPECAL_token);
	L1PreFiringWeightECALpreVFP_Up=iEvent.get(prefweightpreVFPupECAL_token);
	L1PreFiringWeightECALpreVFP_Dn=iEvent.get(prefweightpreVFPdownECAL_token);
	L1PreFiringWeightMuonpreVFP_Nom=iEvent.get(prefweightpreVFPMuon_token);
	L1PreFiringWeightMuonpreVFP_Up=iEvent.get(prefweightpreVFPupMuon_token);
	L1PreFiringWeightMuonpreVFP_Dn=iEvent.get(prefweightpreVFPdownMuon_token);
	L1PreFiringWeightMuonpreVFP_statUp=iEvent.get(prefweightpreVFPstatupMuon_token);
	L1PreFiringWeightMuonpreVFP_statDn=iEvent.get(prefweightpreVFPstatdownMuon_token);
	L1PreFiringWeightMuonpreVFP_systUp=iEvent.get(prefweightpreVFPsystupMuon_token);
	L1PreFiringWeightMuonpreVFP_systDn=iEvent.get(prefweightpreVFPsystdownMuon_token);
	L1PreFiringWeightpostVFP_Nom=iEvent.get(prefweightpostVFP_token);
	L1PreFiringWeightpostVFP_Up=iEvent.get(prefweightpostVFPup_token);
	L1PreFiringWeightpostVFP_Dn=iEvent.get(prefweightpostVFPdown_token);
	L1PreFiringWeightECALpostVFP_Nom=iEvent.get(prefweightpostVFPECAL_token);
	L1PreFiringWeightECALpostVFP_Up=iEvent.get(prefweightpostVFPupECAL_token);
	L1PreFiringWeightECALpostVFP_Dn=iEvent.get(prefweightpostVFPdownECAL_token);
	L1PreFiringWeightMuonpostVFP_Nom=iEvent.get(prefweightpostVFPMuon_token);
	L1PreFiringWeightMuonpostVFP_Up=iEvent.get(prefweightpostVFPupMuon_token);
	L1PreFiringWeightMuonpostVFP_Dn=iEvent.get(prefweightpostVFPdownMuon_token);
	L1PreFiringWeightMuonpostVFP_statUp=iEvent.get(prefweightpostVFPstatupMuon_token);
	L1PreFiringWeightMuonpostVFP_statDn=iEvent.get(prefweightpostVFPstatdownMuon_token);
	L1PreFiringWeightMuonpostVFP_systUp=iEvent.get(prefweightpostVFPsystupMuon_token);
	L1PreFiringWeightMuonpostVFP_systDn=iEvent.get(prefweightpostVFPsystdownMuon_token);
	const edm::TriggerResults &triggerBits = iEvent.get(hlttrigToken_);
	const edm::TriggerNames &names = iEvent.triggerNames(triggerBits);
	HLT_IsoMu24_=false;
	if (triggerBits.accept(names.triggerIndex(std::string("HLT_IsoMu24_v4"))))  HLT_IsoMu24_=true;
	HLT_IsoTkMu24_=false;
	if (triggerBits.accept(names.triggerIndex(std::string("HLT_IsoTkMu24_v4")))) HLT_IsoTkMu24_=true;
	std::vector<pat::MET> mets=iEvent.get(metToken_);
	MET_pt_=mets[0].corPt();
	MET_phi_=mets[0].corPhi();
	MET_JECUp_pt_=mets[0].shiftedPt(pat::MET::METUncertainty::JetEnUp);
	MET_JECDown_pt_=mets[0].shiftedPt(pat::MET::METUncertainty::JetEnDown);
	MET_JECUp_phi_=mets[0].shiftedPhi(pat::MET::METUncertainty::JetEnUp);
	MET_JECDown_phi_=mets[0].shiftedPhi(pat::MET::METUncertainty::JetEnDown);
	MET_JERUp_pt_=mets[0].shiftedPt(pat::MET::METUncertainty::JetResUp);
	MET_JERDown_pt_=mets[0].shiftedPt(pat::MET::METUncertainty::JetResDown);
	MET_JERUp_phi_=mets[0].shiftedPhi(pat::MET::METUncertainty::JetResUp);
	MET_JERDown_phi_=mets[0].shiftedPhi(pat::MET::METUncertainty::JetResDown);
	MET_UnclEnUp_pt_=mets[0].shiftedPt(pat::MET::METUncertainty::UnclusteredEnUp);
        MET_UnclEnDown_pt_=mets[0].shiftedPt(pat::MET::METUncertainty::UnclusteredEnDown);
	MET_UnclEnUp_phi_=mets[0].shiftedPhi(pat::MET::METUncertainty::UnclusteredEnUp);
	MET_UnclEnDown_phi_=mets[0].shiftedPhi(pat::MET::METUncertainty::UnclusteredEnDown);
	tree_->Fill();

#ifdef THIS_IS_AN_EVENTSETUP_EXAMPLE
	ESHandle<SetupData> pSetup;
	iSetup.get<SetupRecord>().get(pSetup);
#endif
}


// ------------ method called once each job just before starting event loop  ------------
void
MuonAnalysis::beginJob()
{
}

// ------------ method called once each job just after ending the event loop  ------------
void
MuonAnalysis::endJob()
{
}

// ------------ method fills 'descriptions' with the allowed parameters for the module  ------------
void
MuonAnalysis::fillDescriptions(edm::ConfigurationDescriptions& descriptions) {
	//The following says we do not know what parameters are allowed so do no validation
	// Please change this to state exactly what you do use, even if it is no parameters
	edm::ParameterSetDescription desc;
	desc.setUnknown();
	descriptions.addDefault(desc);

	//Specify that only 'tracks' is allowed
	//To use, remove the default given above and uncomment below
	//ParameterSetDescription desc;
	//desc.addUntracked<edm::InputTag>("tracks","ctfWithMaterialTracks");
	//descriptions.addDefault(desc);
}

//define this as a plug-in
DEFINE_FWK_MODULE(MuonAnalysis);
