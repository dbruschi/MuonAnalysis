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

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/one/EDAnalyzer.h"

#include "FWCore/Framework/interface/Event.h"
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
#include "functions.h"
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
      edm::EDGetTokenT< double > prefweightpreVFP_token;
      edm::EDGetTokenT< double > prefweightpreVFPup_token;
      edm::EDGetTokenT< double > prefweightpreVFPdown_token;
      edm::EDGetTokenT< double > prefweightpreVFPECAL_token;
      edm::EDGetTokenT< double > prefweightpreVFPupECAL_token;
      edm::EDGetTokenT< double > prefweightpreVFPdownECAL_token;
      edm::EDGetTokenT< double > prefweightpreVFPMuon_token;
      edm::EDGetTokenT< double > prefweightpreVFPupMuon_token;
      edm::EDGetTokenT< double > prefweightpreVFPdownMuon_token;
      edm::EDGetTokenT< double > prefweightpostVFP_token;
      edm::EDGetTokenT< double > prefweightpostVFPup_token;
      edm::EDGetTokenT< double > prefweightpostVFPdown_token;
      edm::EDGetTokenT< double > prefweightpostVFPECAL_token;
      edm::EDGetTokenT< double > prefweightpostVFPupECAL_token;
      edm::EDGetTokenT< double > prefweightpostVFPdownECAL_token;
      edm::EDGetTokenT< double > prefweightpostVFPMuon_token;
      edm::EDGetTokenT< double > prefweightpostVFPupMuon_token;
      edm::EDGetTokenT< double > prefweightpostVFPdownMuon_token;
      TTree * tree_;
      UInt_t nMuon_, nGenPart_, nGenPartPreFSR_, nGenMuonPreFSR_, nGenPart746_, nGenPartPostFSR_;
      Float_t Muon_pt_[100], Muon_eta_[100], Muon_phi_[100], Muon_mass_[100], Muon_pfRelIso04_all_[100], Muon_pfRelIso04_chgPV_[100], Muon_pfRelIso04_chgPU_[100], Muon_pfRelIso04_nhad_[100], Muon_pfRelIso04_pho_[100], Muon_pfRelIso03_all_[100];
      Float_t Muon_pfRelIso03_chgPV_[100], Muon_pfRelIso03_chgPU_[100], Muon_pfRelIso03_nhad_[100], Muon_pfRelIso03_pho_[100], Muon_tkRelIso_[100], Muon_dxy_[100], Muon_dxyErr_[100], Muon_dz_[100], Muon_dzErr_[100], Muon_dxyBS_[100], Muon_dzBS_[100];
      Float_t PV_chi2_, PV_ndof_, PV_score_, PV_x_, PV_y_, PV_z_, BeamSpot_x0_, BeamSpot_y0_, BeamSpot_z0_, GenVertex_x_, GenVertex_y_, GenVertex_z_;
      Bool_t Muon_isTracker_[100], Muon_isGlobal_[100], Muon_isStandalone_[100], Muon_looseId_[100], Muon_mediumId_[100], Muon_mediumPromptId_[100], Muon_tightId_[100], Muon_softId_[100], Muon_isPF_[100], Muon_softMvaId_[100];
      Int_t Muon_charge_[100];
      Int_t Muon_BestTrackAlgo_[100], Muon_InnerTrackAlgo_[100], Muon_GlobalTrackAlgo_[100], Muon_BestTrackOriginalAlgo_[100], Muon_InnerTrackOriginalAlgo_[100], Muon_GlobalTrackOriginalAlgo_[100], Muon_genPartIdx_[100], Muon_genPartPreFSRIdx_[100];
      UChar_t Muon_highPtId_[100], Muon_miniIsoId_[100], Muon_multiIsoId_[100], Muon_mvaId_[100], Muon_mvaLowPtId_[100], Muon_pfIsoId_[100], Muon_tkIsoId_[100];
      Float_t GenPart_eta_[500], GenPart_mass_[500], GenPart_phi_[500], GenPart_pt_[500], Generator_weight_, puWeight_, puWeight_Up_, puWeight_Down_, L1PreFiringWeightpreVFP_Nom, L1PreFiringWeightpreVFP_Up, L1PreFiringWeightpreVFP_Dn, L1PreFiringWeightECALpreVFP_Nom, L1PreFiringWeightECALpreVFP_Up, L1PreFiringWeightECALpreVFP_Dn, L1PreFiringWeightMuonpreVFP_Nom, L1PreFiringWeightMuonpreVFP_Up, L1PreFiringWeightMuonpreVFP_Dn, L1PreFiringWeightpostVFP_Nom, L1PreFiringWeightpostVFP_Up, L1PreFiringWeightpostVFP_Dn, L1PreFiringWeightECALpostVFP_Nom, L1PreFiringWeightECALpostVFP_Up, L1PreFiringWeightECALpostVFP_Dn, L1PreFiringWeightMuonpostVFP_Nom, L1PreFiringWeightMuonpostVFP_Up, L1PreFiringWeightMuonpostVFP_Dn;
      Int_t GenPart_genPartIdxMother_[500], GenPart_pdgId_[500], GenPart_status_[500], GenPart_statusFlags_[500], GenPart_preFSRLepIdx1_, GenPart_preFSRLepIdx2_, GenPart_postFSRLepIdx1_, GenPart_postFSRLepIdx2_, GenPart_PostFSR_[500];
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
    vertexToken_=consumes<std::vector<reco::Vertex> >(iConfig.getUntrackedParameter<edm::InputTag>("vertices"));
    beamspotToken_=consumes<reco::BeamSpot>(iConfig.getUntrackedParameter<edm::InputTag>("beamspot"));
    genvertexToken_=consumes<ROOT::Math::PositionVector3D<ROOT::Math::Cartesian3D<float>,ROOT::Math::DefaultCoordinateSystemTag> >(iConfig.getUntrackedParameter<edm::InputTag>("genvertex"));
    genparticleToken_=consumes<std::vector<reco::GenParticle> >(iConfig.getUntrackedParameter<edm::InputTag>("genparticles"));
    geneventinfoToken_=consumes<GenEventInfoProduct>(iConfig.getUntrackedParameter<edm::InputTag>("geneventinfo"));
    puToken_=consumes<std::vector<PileupSummaryInfo> >(iConfig.getUntrackedParameter<edm::InputTag>("pileupinfo"));
    prefweightpreVFP_token = consumes< double >(edm::InputTag("prefiringweightpreVFP:nonPrefiringProb"));
    prefweightpreVFPup_token = consumes< double >(edm::InputTag("prefiringweightpreVFP:nonPrefiringProbUp"));
    prefweightpreVFPdown_token = consumes< double >(edm::InputTag("prefiringweightpreVFP:nonPrefiringProbDown"));
    prefweightpreVFPECAL_token = consumes< double >(edm::InputTag("prefiringweightpreVFP:nonPrefiringProbECAL"));
    prefweightpreVFPupECAL_token = consumes< double >(edm::InputTag("prefiringweightpreVFP:nonPrefiringProbECALUp"));
    prefweightpreVFPdownECAL_token = consumes< double >(edm::InputTag("prefiringweightpreVFP:nonPrefiringProbECALDown"));
    prefweightpreVFPMuon_token = consumes< double >(edm::InputTag("prefiringweightpreVFP:nonPrefiringProbMuon"));
    prefweightpreVFPupMuon_token = consumes< double >(edm::InputTag("prefiringweightpreVFP:nonPrefiringProbMuonUp"));
    prefweightpreVFPdownMuon_token = consumes< double >(edm::InputTag("prefiringweightpreVFP:nonPrefiringProbMuonDown"));
    prefweightpostVFP_token = consumes< double >(edm::InputTag("prefiringweightpostVFP:nonPrefiringProb"));
    prefweightpostVFPup_token = consumes< double >(edm::InputTag("prefiringweightpostVFP:nonPrefiringProbUp"));
    prefweightpostVFPdown_token = consumes< double >(edm::InputTag("prefiringweightpostVFP:nonPrefiringProbDown"));
    prefweightpostVFPECAL_token = consumes< double >(edm::InputTag("prefiringweightpostVFP:nonPrefiringProbECAL"));
    prefweightpostVFPupECAL_token = consumes< double >(edm::InputTag("prefiringweightpostVFP:nonPrefiringProbECALUp"));
    prefweightpostVFPdownECAL_token = consumes< double >(edm::InputTag("prefiringweightpostVFP:nonPrefiringProbECALDown"));
    prefweightpostVFPMuon_token = consumes< double >(edm::InputTag("prefiringweightpostVFP:nonPrefiringProbMuon"));
    prefweightpostVFPupMuon_token = consumes< double >(edm::InputTag("prefiringweightpostVFP:nonPrefiringProbMuonUp"));
    prefweightpostVFPdownMuon_token = consumes< double >(edm::InputTag("prefiringweightpostVFP:nonPrefiringProbMuonDown"));
    std::string pumc("$CMSSW_BASE/src/MuonAnalysis/MuonAnalysis/files/pileup_mc_2016UL.root"); //might need fixing (specific for my config)
    std::string pudata("/afs/cern.ch/cms/CAF/CMSCOMM/COMM_DQM/certification/Collisions16/13TeV/PileUp/UltraLegacy/PileupHistogram-goldenJSON-13tev-2016-69200ub-99bins.root");
    std::string pudata_down("/afs/cern.ch/cms/CAF/CMSCOMM/COMM_DQM/certification/Collisions16/13TeV/PileUp/UltraLegacy/PileupHistogram-goldenJSON-13tev-2016-66000ub-99bins.root");
    std::string pudata_up("/afs/cern.ch/cms/CAF/CMSCOMM/COMM_DQM/certification/Collisions16/13TeV/PileUp/UltraLegacy/PileupHistogram-goldenJSON-13tev-2016-72400ub-99bins.root");
    std::string puhistomc("pileup_mc");
    std::string puhistodata("pileup");
    lumiWeights_=new edm::LumiReWeighting(pumc,pudata,puhistomc,puhistodata);
    lumiWeightsup_=new edm::LumiReWeighting(pumc,pudata_up,puhistomc,puhistodata);
    lumiWeightsdown_=new edm::LumiReWeighting(pumc,pudata_down,puhistomc,puhistodata);
    edm::Service<TFileService> fs;
    tree_=fs->make<TTree>("Events","Events");
    tree_->Branch("nMuon", &nMuon_, "nMuon/i");
    tree_->Branch("Muon_pt",&Muon_pt_,"Muon_pt[nMuon]/F");
    tree_->Branch("Muon_eta",&Muon_eta_,"Muon_eta_[nMuon]/F");
    tree_->Branch("Muon_phi",&Muon_phi_,"Muon_phi_[nMuon]/F");
    tree_->Branch("Muon_mass",&Muon_mass_,"Muon_mass[nMuon]/F");
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
    tree_->Branch("Muon_tkIsoId",&Muon_tkIsoId_,"Muon_tkIsoId[nMuon]/b");
    tree_->Branch("Muon_genPartIdx",&Muon_genPartIdx_,"Muon_genPartIdx[nMuon]/I");
    tree_->Branch("Muon_genPartPreFSRIdx",&Muon_genPartPreFSRIdx_,"Muon_genPartPreFSRIdx[nMuon]/I");
    tree_->Branch("PV_chi2",&PV_chi2_,"PV_chi2/F");
    tree_->Branch("PV_ndof",&PV_ndof_,"PV_ndof/F"); 
    tree_->Branch("PV_score",&PV_score_,"PV_score/F");
    tree_->Branch("PV_x",&PV_x_,"PV_x/F");
    tree_->Branch("PV_y",&PV_y_,"PV_y/F");
    tree_->Branch("PV_z",&PV_z_,"PV_z/F");
    tree_->Branch("BeamSpot_x0",&BeamSpot_x0_,"BeamSpot_x0/F");
    tree_->Branch("BeamSpot_y0",&BeamSpot_y0_,"BeamSpot_y0/F");
    tree_->Branch("BeamSpot_z0",&BeamSpot_z0_,"BeamSpot_z0/F");
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
	tree_->Branch("L1PreFiringWeightpostVFP_Nom",&L1PreFiringWeightpostVFP_Nom,"L1PreFiringWeightpostVFP_Nom/F");
	tree_->Branch("L1PreFiringWeightpostVFP_Up",&L1PreFiringWeightpostVFP_Up,"L1PreFiringWeightpostVFP_Up/F");
	tree_->Branch("L1PreFiringWeightpostVFP_Dn",&L1PreFiringWeightpostVFP_Dn,"L1PreFiringWeightpostVFP_Dn/F");
	tree_->Branch("L1PreFiringWeightECALpostVFP_Nom",&L1PreFiringWeightECALpostVFP_Nom,"L1PreFiringWeightECALpostVFP_Nom/F");
	tree_->Branch("L1PreFiringWeightECALpostVFP_Up",&L1PreFiringWeightECALpostVFP_Up,"L1PreFiringWeightECALpostVFP_Up/F");
	tree_->Branch("L1PreFiringWeightECALpostVFP_Dn",&L1PreFiringWeightECALpostVFP_Dn,"L1PreFiringWeightECALpostVFP_Dn/F");
	tree_->Branch("L1PreFiringWeightMuonpostVFP_Nom",&L1PreFiringWeightMuonpostVFP_Nom,"L1PreFiringWeightMuonpostVFP_Nom/F");
	tree_->Branch("L1PreFiringWeightMuonpostVFP_Up",&L1PreFiringWeightMuonpostVFP_Up,"L1PreFiringWeightMuonpostVFP_Up/F");
	tree_->Branch("L1PreFiringWeightMuonpostVFP_Dn",&L1PreFiringWeightMuonpostVFP_Dn,"L1PreFiringWeightMuonpostVFP_Dn/F");
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
   UInt_t nmuon=0, i=0; 
   Float_t muon_pt, muon_eta, muon_phi, muon_mass, muon_pfRelIso04_all, muon_pfRelIso04_chgPV, muon_pfRelIso04_chgPU, muon_pfRelIso04_nhad, muon_pfRelIso04_pho, muon_pfRelIso03_all;
   Float_t muon_pfRelIso03_chgPV, muon_pfRelIso03_chgPU, muon_pfRelIso03_nhad, muon_pfRelIso03_pho, muon_tkRelIso, muon_dxy, muon_dxyErr, muon_dz, muon_dzErr, muon_dxyBS, muon_dzBS;
   Float_t pv_chi2, pv_ndof, pv_score, pv_x, pv_y, pv_z, beamspot_x0, beamspot_y0, beamspot_z0, genvertex_x, genvertex_y, genvertex_z;
   Bool_t muon_isTracker, muon_isGlobal, muon_isStandalone, muon_looseId, muon_mediumId, muon_mediumPromptId, muon_tightId, muon_softId, muon_isPF, muon_softMvaId;
   Int_t muon_charge;
   Int_t muon_BestTrackAlgo;
   UChar_t muon_highPtId=0, muon_miniIsoId=0, muon_multiIsoId=0, muon_mvaId=0, muon_mvaLowPtId=0, muon_pfIsoId=0, muon_tkIsoId=0;
   reco::Vertex primaryvertex=(iEvent.get(vertexToken_))[0];
   reco::BeamSpot beamspot=iEvent.get(beamspotToken_);
   ROOT::Math::PositionVector3D<ROOT::Math::Cartesian3D<float>,ROOT::Math::DefaultCoordinateSystemTag> genvertex=iEvent.get(genvertexToken_);
   getGenLeptonIdxandFill(iEvent.get(genparticleToken_), GenPart_eta_, GenPart_mass_, GenPart_phi_, GenPart_pt_, GenPart_genPartIdxMother_, GenPart_pdgId_, GenPart_status_, GenPart_statusFlags_, GenPart_preFSRLepIdx1_, GenPart_preFSRLepIdx2_, GenPart_postFSRLepIdx1_, GenPart_postFSRLepIdx2_, nGenPart_, nGenPartPreFSR_, nGenMuonPreFSR_, nGenPart746_, nGenPartPostFSR_, GenPart_PostFSR_);
   for (const auto& muon : iEvent.get(muonToken_)) {
      // do something with track parameters, e.g, plot the charge.
      // int charge = track.charge();
      muon_pt=muon.pt();
      muon_eta=muon.eta();
      muon_phi=muon.phi();
      muon_mass=muon.mass();
      muon_isTracker=muon.isTrackerMuon();
      muon_isGlobal=muon.isGlobalMuon();
      muon_isStandalone=muon.isStandAloneMuon();
      muon_isPF=muon.isPFMuon();
      muon_looseId=muon.isLooseMuon();
      muon_mediumId=muon.isMediumMuon();
      muon_mediumPromptId=muon.passed(muon.CutBasedIdMediumPrompt);
      muon_tightId=muon.isTightMuon(primaryvertex);
      muon_softId=muon.isSoftMuon(primaryvertex);
      if (muon.isHighPtMuon(primaryvertex)) muon_highPtId=2;
      //if (muon.isTrackerHighPtMuon(primaryvertex)) muon_highPtId=1;
      if (muon.passed(reco::Muon::MiniIsoLoose)) muon_miniIsoId=1;
      if (muon.passed(reco::Muon::MiniIsoMedium)) muon_miniIsoId=2;
      if (muon.passed(reco::Muon::MiniIsoTight)) muon_miniIsoId=3;
      if (muon.passed(reco::Muon::MiniIsoVeryTight)) muon_miniIsoId=4;
      if (muon.passed(reco::Muon::MultiIsoLoose)) muon_multiIsoId=1;
      if (muon.passed(reco::Muon::MultiIsoMedium)) muon_multiIsoId=2;
      if (muon.passed(reco::Muon::MvaLoose)) muon_mvaId=1;
      if (muon.passed(reco::Muon::MvaMedium)) muon_mvaId=2;
      if (muon.passed(reco::Muon::MvaTight)) muon_mvaId=3;
      if (muon.passed(reco::Muon::MvaVTight)) muon_mvaId=4;
      if (muon.passed(reco::Muon::MvaVVTight)) muon_mvaId=5;
      if (muon.passed(reco::Muon::LowPtMvaLoose)) muon_mvaLowPtId=1;
      if (muon.passed(reco::Muon::LowPtMvaMedium)) muon_mvaLowPtId=2;
      if (muon.passed(reco::Muon::PFIsoVeryLoose)) muon_pfIsoId=1;
      if (muon.passed(reco::Muon::PFIsoLoose)) muon_pfIsoId=2;
      if (muon.passed(reco::Muon::PFIsoMedium)) muon_pfIsoId=3;
      if (muon.passed(reco::Muon::PFIsoTight)) muon_pfIsoId=4;
      if (muon.passed(reco::Muon::PFIsoVeryTight)) muon_pfIsoId=5;
      if (muon.passed(reco::Muon::PFIsoVeryVeryTight)) muon_pfIsoId=6;
      if (muon.passed(reco::Muon::SoftMvaId)) muon_softMvaId=1;
      if (muon.passed(reco::Muon::TkIsoLoose)) muon_tkIsoId=1;
      if (muon.passed(reco::Muon::TkIsoTight)) muon_tkIsoId=2;
      muon_pfRelIso04_chgPV=muon.pfIsolationR04().sumChargedHadronPt/muon_pt;
      muon_pfRelIso04_nhad=muon.pfIsolationR04().sumNeutralHadronEt/muon_pt; 
      muon_pfRelIso04_pho=muon.pfIsolationR04().sumPhotonEt/muon_pt;
      muon_pfRelIso04_chgPU=muon.pfIsolationR04().sumPUPt/muon_pt;
      muon_pfRelIso04_all=(muon.pfIsolationR04().sumChargedHadronPt+std::max(0.,muon.pfIsolationR04().sumNeutralHadronEt+muon.pfIsolationR04().sumPhotonEt-0.5*muon.pfIsolationR04().sumPUPt))/muon_pt;
      muon_pfRelIso03_chgPV=muon.pfIsolationR03().sumChargedHadronPt/muon_pt;
      muon_pfRelIso03_nhad=muon.pfIsolationR03().sumNeutralHadronEt/muon_pt;    
      muon_pfRelIso03_pho=muon.pfIsolationR03().sumPhotonEt/muon_pt;
      muon_pfRelIso03_chgPU=muon.pfIsolationR03().sumPUPt/muon_pt;
      muon_pfRelIso03_all=(muon.pfIsolationR03().sumChargedHadronPt+std::max(0.,muon.pfIsolationR03().sumNeutralHadronEt+muon.pfIsolationR03().sumPhotonEt-0.5*muon.pfIsolationR03().sumPUPt))/muon_pt;
      muon_tkRelIso=muon.isolationR03().sumPt/muon_pt;
      muon_dxy=muon.muonBestTrack()->dxy(primaryvertex.position());
      muon_dxyErr=muon.muonBestTrack()->dxyError(primaryvertex.position(),primaryvertex.covariance());
      muon_dz=muon.muonBestTrack()->dz(primaryvertex.position());
      muon_dzErr=muon.muonBestTrack()->dzError();
      muon_dxyBS=muon.muonBestTrack()->dxy(beamspot.position());
      muon_dzBS=muon.muonBestTrack()->dz(beamspot.position());
      muon_charge=muon.charge();
      muon_BestTrackAlgo=muon.muonBestTrack()->algo();
      //muon_pfIsoLoose=muon.passed(muon.PFIsoLoose);
      //muon_pfIsoMedium=muon.passed(muon.PFIsoMedium);
      //muon_pfIsoTight=muon.passed(muon.PFIsoTight);
/*      std::cout << "Muon n." << i << " Muon charge:" << muon_charge << " Muon pt" << muon_pt << " Muon eta" << muon_eta << " Muon phi" << muon_phi << " Muon mass" << muon_mass;
      std::cout << " isLooseMuon:" << muon_looseId << " isMediumMuon:" << muon_mediumId << " isMediumPromptMuon: "<< muon_mediumPromptId ;
      std::cout << " isTightMuon:" << muon_tightId << "isSoftMuon:" << muon_softId << " muon_highPtId:" << (int)muon_highPtId << " muon_miniIsoId:";
      std::cout << (int)muon_miniIsoId << " muon_multiIsoId:" << (int)muon_multiIsoId << " muon_mvaId:" << (int)muon_mvaId << " muon_mvaLowPtId:" << (int)muon_mvaLowPtId;
      std::cout << " muon_pfIsoId:" << (int)muon_pfIsoId << " muon_softMvaId:" << muon_softMvaId << "muon_tkIsoId:" << (int)muon_tkIsoId;
      std::cout << " isTrackerMuon:" << muon_isTracker << " isGlobalMuon:" << muon_isGlobal << " isStandaloneMuon" << muon_isStandalone << " isPFMuon:"<<  muon_isPF;
      std::cout << " pfRelIso04 charged from PV:" << muon_pfRelIso04_chgPV << " pfRelIso04 charged from PU:" << muon_pfRelIso04_chgPU;
      std::cout << " pfRelIso04 neutral hadrons:" << muon_pfRelIso04_nhad << " pfRelIso04 photons:" << muon_pfRelIso04_pho << " total pfRelIso04:" << muon_pfRelIso04_all;
      std::cout << " pfRelIso03 charged from PV:" << muon_pfRelIso03_chgPV << " pfRelIso03 charged from PU:" << muon_pfRelIso03_chgPU;
      std::cout << " pfRelIso03 neutral hadrons:" << muon_pfRelIso03_nhad << " pfRelIso03 photons:" << muon_pfRelIso03_pho << " total pfRelIso03:" << muon_pfRelIso03_all;
      std::cout << " tkRelIso:" << muon_tkRelIso << " dxy wrt first PV (signed):"<< muon_dxy << " dxyError:"<< muon_dxyErr << " dz wrt first PV:"<< muon_dz;
      std::cout << " dzError:"<< muon_dzErr << " dxy wrt BeamSpot (signed):"<< muon_dxyBS << " dz wrt BeamSpot:"<< muon_dzBS<< " besttracktype:" <<muon.muonBestTrackType()<<" besttrackalgo:" << muon_BestTrackAlgo <<"\n";*/
      Muon_pt_[i]=muon_pt;
      Muon_eta_[i]=muon_eta;
      Muon_phi_[i]=muon_phi;
      Muon_mass_[i]=muon_mass;
      Muon_pfRelIso04_all_[i]=muon_pfRelIso04_all;
      Muon_pfRelIso04_chgPV_[i]=muon_pfRelIso04_chgPV;
      Muon_pfRelIso04_chgPU_[i]=muon_pfRelIso04_chgPU;
      Muon_pfRelIso04_nhad_[i]=muon_pfRelIso04_nhad;
      Muon_pfRelIso04_pho_[i]=muon_pfRelIso04_pho;
      Muon_pfRelIso03_all_[i]=muon_pfRelIso03_all;
      Muon_pfRelIso03_chgPV_[i]=muon_pfRelIso03_chgPV;
      Muon_pfRelIso03_chgPU_[i]=muon_pfRelIso03_chgPU;
      Muon_pfRelIso03_nhad_[i]=muon_pfRelIso03_nhad;
      Muon_pfRelIso03_pho_[i]=muon_pfRelIso03_pho;
      Muon_tkRelIso_[i]=muon_tkRelIso;
      Muon_dxy_[i]=muon_dxy;
      Muon_dxyErr_[i]=muon_dxyErr;
      Muon_dz_[i]=muon_dz;
      Muon_dzErr_[i]=muon_dzErr;
      Muon_dxyBS_[i]=muon_dxyBS;
      Muon_dzBS_[i]=muon_dzBS;
      Muon_isTracker_[i]=muon_isTracker;
      Muon_isGlobal_[i]=muon_isGlobal;
      Muon_isStandalone_[i]=muon_isStandalone;
      Muon_looseId_[i]=muon_looseId;
      Muon_mediumId_[i]=muon_mediumId;
      Muon_mediumPromptId_[i]=muon_mediumPromptId;
      Muon_tightId_[i]=muon_tightId;
      Muon_softId_[i]=muon_softId;
      Muon_isPF_[i]=muon_isPF;
      Muon_softMvaId_[i]=muon_softMvaId;
      Muon_charge_[i]=muon_charge;
      Muon_BestTrackAlgo_[i]=muon_BestTrackAlgo;
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
      Muon_highPtId_[i]=muon_highPtId;
      Muon_miniIsoId_[i]=muon_miniIsoId;
      Muon_multiIsoId_[i]=muon_multiIsoId;
      Muon_mvaId_[i]=muon_mvaId;
      Muon_mvaLowPtId_[i]=muon_mvaLowPtId;
      Muon_pfIsoId_[i]=muon_pfIsoId;
      Muon_tkIsoId_[i]=muon_tkIsoId;
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
      i++;
   }
   nMuon_=nmuon=i;
   PV_x_=pv_x=primaryvertex.x();
   PV_y_=pv_y=primaryvertex.y();
   PV_z_=pv_z=primaryvertex.z();
   PV_chi2_=pv_chi2=primaryvertex.normalizedChi2();
   PV_ndof_=pv_ndof=primaryvertex.ndof();
   BeamSpot_x0_=beamspot_x0=beamspot.x0();
   BeamSpot_y0_=beamspot_y0=beamspot.y0();
   BeamSpot_z0_=beamspot_z0=beamspot.z0();
   GenVertex_x_=genvertex_x=genvertex.x();
   GenVertex_y_=genvertex_y=genvertex.y();
   GenVertex_z_=genvertex_z=genvertex.z();
   Generator_weight_=iEvent.get(geneventinfoToken_).weight();
   float npv = -1;
   for (std::vector<PileupSummaryInfo>::const_iterator pvi = iEvent.get(puToken_).begin(); pvi != iEvent.get(puToken_).end(); ++pvi) {
     int bx = pvi->getBunchCrossing();
     if (bx == 0) {
       npv = pvi->getTrueNumInteractions();
       continue;
     }
   }
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
   L1PreFiringWeightpostVFP_Nom=iEvent.get(prefweightpostVFP_token);
   L1PreFiringWeightpostVFP_Up=iEvent.get(prefweightpostVFPup_token);
   L1PreFiringWeightpostVFP_Dn=iEvent.get(prefweightpostVFPdown_token);
   L1PreFiringWeightECALpostVFP_Nom=iEvent.get(prefweightpostVFPECAL_token);
   L1PreFiringWeightECALpostVFP_Up=iEvent.get(prefweightpostVFPupECAL_token);
   L1PreFiringWeightECALpostVFP_Dn=iEvent.get(prefweightpostVFPdownECAL_token);
   L1PreFiringWeightMuonpostVFP_Nom=iEvent.get(prefweightpostVFPMuon_token);
   L1PreFiringWeightMuonpostVFP_Up=iEvent.get(prefweightpostVFPupMuon_token);
   L1PreFiringWeightMuonpostVFP_Dn=iEvent.get(prefweightpostVFPdownMuon_token);
   /*std::cout<<nmuon<<"\n";
   std::cout<<"First Primary Vertex x:"<<pv_x<<" y:"<<pv_y<<" z:"<<pv_z<<" normalized chi2:"<<pv_chi2<<" ndof:"<<pv_ndof<<"\n";
   std::cout<<"BeamSpot x0:"<<beamspot_x0<<" y0:"<<beamspot_y0<<" z0:"<<beamspot_z0<<"\n";
   std::cout<<"GenVertex x:"<<genvertex_x<<" y:"<<genvertex_y<<" z:"<<genvertex_z<<"\n";*/
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
