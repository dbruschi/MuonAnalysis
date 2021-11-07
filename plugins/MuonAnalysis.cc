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
   unsigned int i=0, nmuon=0;
   float muon_pt, muon_eta, muon_phi, muon_mass, muon_pfRelIso04_all, muon_pfRelIso04_chgPV, muon_pfRelIso04_chgPU, muon_pfRelIso04_nhad, muon_pfRelIso04_pho, muon_pfRelIso03_all, muon_pfRelIso03_chgPV, muon_pfRelIso03_chgPU, muon_pfRelIso03_nhad, muon_pfRelIso03_pho, muon_tkRelIso, muon_dxy, muon_dz;
   bool muon_isTracker, muon_isGlobal, muon_isStandalone, muon_looseId, muon_mediumId, muon_mediumPromptId, muon_tightId, muon_softId, muon_isPF, muon_softMvaId;
   int muon_charge;
   unsigned char muon_highPtId=0, muon_miniIsoId=0, muon_multiIsoId=0, muon_mvaId=0, muon_mvaLowPtId=0, muon_pfIsoId=0, muon_tkIsoId=0;
   reco::Vertex primaryvertex=(iEvent.get(vertexToken_))[0];
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
      muon_dz=muon.muonBestTrack()->dz(primaryvertex.position());
      //muon_pfIsoLoose=muon.passed(muon.PFIsoLoose);
      //muon_pfIsoMedium=muon.passed(muon.PFIsoMedium);
      //muon_pfIsoTight=muon.passed(muon.PFIsoTight);
      std::cout << "Muon n." << i << " Muon pt" << muon_pt << " Muon eta" << muon_eta << " Muon phi" << muon_phi << " Muon mass" << muon_mass;
      std::cout << " isLooseMuon:" << muon_looseId << " isMediumMuon:" << muon_mediumId << " isMediumPromptMuon: "<< muon_mediumPromptId ;
      std::cout << " isTightMuon:" << muon_tightId << "isSoftMuon:" << muon_softId << " muon_highPtId:" << (int)muon_highPtId << " muon_miniIsoId:";
      std::cout << (int)muon_miniIsoId << " muon_multiIsoId:" << (int)muon_multiIsoId << " muon_mvaId:" << (int)muon_mvaId << " muon_mvaLowPtId:" << (int)muon_mvaLowPtId;
      std::cout << " muon_pfIsoId:" << (int)muon_pfIsoId << " muon_softMvaId:" << muon_softMvaId << "muon_tkIsoId:" << (int)muon_tkIsoId;
      std::cout << " isTrackerMuon:" << muon_isTracker << " isGlobalMuon:" << muon_isGlobal << " isStandaloneMuon" << muon_isStandalone << " isPFMuon:"<<  muon_isPF;
      std::cout << " pfRelIso04 charged from PV:" << muon_pfRelIso04_chgPV << " pfRelIso04 charged from PU:" << muon_pfRelIso04_chgPU;
      std::cout << " pfRelIso04 neutral hadrons:" << muon_pfRelIso04_nhad << " pfRelIso04 photons:" << muon_pfRelIso04_pho << " total pfRelIso04:" << muon_pfRelIso04_all;
      std::cout << " pfRelIso03 charged from PV:" << muon_pfRelIso03_chgPV << " pfRelIso03 charged from PU:" << muon_pfRelIso03_chgPU;
      std::cout << " pfRelIso03 neutral hadrons:" << muon_pfRelIso03_nhad << " pfRelIso03 photons:" << muon_pfRelIso03_pho << " total pfRelIso03:" << muon_pfRelIso03_all;
      std::cout << " tkRelIso:" << muon_tkRelIso << "dxy wrt first PV (signed):"<< muon_dxy << " dz wrt first PV:"<< muon_dz <<"\n";
      i++;
}
   nmuon=i+1;
   std::cout<<nmuon<<"\n";
   std::cout<<"First Primary Vertex x:"<<primaryvertex.x()<<" y:"<<primaryvertex.y()<<" z:"<<primaryvertex.z()<<"\n";

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
