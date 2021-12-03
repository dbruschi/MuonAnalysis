import FWCore.ParameterSet.Config as cms
from PhysicsTools.PatUtils.l1PrefiringWeightProducer_cfi import l1PrefiringWeightProducer

process = cms.Process("Muon")

process.load("FWCore.MessageService.MessageLogger_cfi")

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(-1) )

process.source = cms.Source("PoolSource",
                                # replace 'myfile.root' with the source file you want to use
                                fileNames = cms.untracked.vstring(
            'file:/gpfs/ddn/srm/cms/store/mc/RunIISummer20UL16MiniAODv2/DYJetsToMuMu_M-50_TuneCP5_13TeV-powhegMiNNLO-pythia8-photos/MINIAODSIM/106X_mcRun2_asymptotic_v17-v1/120000/0B45C79A-BB28-5840-93B5-DC3F5015B8E4.root'
#            'file:/gpfs/ddn/srm/cms/store/mc/RunIISummer20UL16MiniAODv2/DYJetsToLL_M-50_TuneCP5_13TeV-amcatnloFXFX-pythia8/MINIAODSIM/106X_mcRun2_asymptotic_v17-v1/270000/2EE70A27-7660-984C-BBFE-45D340A578A7.root'
                )
                            )
process.prefiringweightpreVFP = l1PrefiringWeightProducer.clone(
   TheJets = cms.InputTag("slimmedJets"),
   DataEraECAL = cms.string("UL2016preVFP"),
   DataEraMuon = cms.string("2016preVFP"),
   UseJetEMPt = cms.bool(False),
   PrefiringRateSystematicUnctyECAL = cms.double(0.2),
   PrefiringRateSystematicUnctyMuon = cms.double(0.2)
)

process.prefiringweightpostVFP = l1PrefiringWeightProducer.clone(
   TheJets = cms.InputTag("slimmedJets"),
   DataEraECAL = cms.string("UL2016postVFP"),
   DataEraMuon = cms.string("2016postVFP"),
   UseJetEMPt = cms.bool(False),
   PrefiringRateSystematicUnctyECAL = cms.double(0.2),
   PrefiringRateSystematicUnctyMuon = cms.double(0.2)
)

process.demo = cms.EDAnalyzer('MuonAnalysis',
   muons    = cms.untracked.InputTag('slimmedMuons'),
   vertices = cms.untracked.InputTag('offlineSlimmedPrimaryVertices'),
   beamspot = cms.untracked.InputTag('offlineBeamSpot'),
   hlttrig  = cms.untracked.InputTag("TriggerResults","","HLT"),
   genvertex= cms.untracked.InputTag('genParticles','xyz0'),   
   genparticles = cms.untracked.InputTag('prunedGenParticles'),
   geneventinfo = cms.untracked.InputTag('generator'),
   pileupinfo = cms.untracked.InputTag('slimmedAddPileupInfo'),
   lheinfo  = cms.untracked.InputTag('externalLHEProducer')
                              )

process.TFileService = cms.Service("TFileService", fileName = cms.string("nanoaod.root") )

process.p = cms.Path(process.prefiringweightpreVFP*process.prefiringweightpostVFP*process.demo)
