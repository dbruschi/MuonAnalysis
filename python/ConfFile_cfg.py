import FWCore.ParameterSet.Config as cms

process = cms.Process("Muon")

process.load("FWCore.MessageService.MessageLogger_cfi")

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(10000) )

process.source = cms.Source("PoolSource",
                                # replace 'myfile.root' with the source file you want to use
                                fileNames = cms.untracked.vstring(
            'file:/gpfs/ddn/srm/cms/store/mc/RunIISummer20UL16MiniAODv2/DYJetsToMuMu_M-50_TuneCP5_13TeV-powhegMiNNLO-pythia8-photos/MINIAODSIM/106X_mcRun2_asymptotic_v17-v1/120000/0B45C79A-BB28-5840-93B5-DC3F5015B8E4.root'
                )
                            )

process.demo = cms.EDAnalyzer('MuonAnalysis',
   muons    = cms.untracked.InputTag('slimmedMuons')
                              )

process.p = cms.Path(process.demo)
