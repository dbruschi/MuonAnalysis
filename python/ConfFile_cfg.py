import FWCore.ParameterSet.Config as cms
from PhysicsTools.PatUtils.l1PrefiringWeightProducer_cfi import l1PrefiringWeightProducer
from PhysicsTools.PatUtils.tools.runMETCorrectionsAndUncertainties import runMetCorAndUncFromMiniAOD
import os

process = cms.Process("Muon")

process.load("FWCore.MessageService.MessageLogger_cfi")

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(50000) )

process.source = cms.Source("PoolSource",
                                # replace 'myfile.root' with the source file you want to use
                                fileNames = cms.untracked.vstring(
            'file:/gpfs/ddn/srm/cms/store/mc/RunIISummer20UL16MiniAODv2/DYJetsToMuMu_M-50_TuneCP5_13TeV-powhegMiNNLO-pythia8-photos/MINIAODSIM/106X_mcRun2_asymptotic_v17-v1/120000/0B45C79A-BB28-5840-93B5-DC3F5015B8E4.root'
#            'file:/gpfs/ddn/srm/cms/store/mc/RunIISummer20UL16MiniAODv2/DYJetsToLL_M-50_TuneCP5_13TeV-amcatnloFXFX-pythia8/MINIAODSIM/106X_mcRun2_asymptotic_v17-v1/270000/2EE70A27-7660-984C-BBFE-45D340A578A7.root'
                )
                            )
useGTforConditions=True

process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_cff')

from Configuration.AlCa.GlobalTag import GlobalTag
process.GlobalTag = GlobalTag(process.GlobalTag, '106X_mcRun2_asymptotic_v17', '')


if useGTforConditions == False:
    from CondCore.CondDB.CondDB_cfi import *
    import os

    CondDBSetup = CondDB.clone()
    CondDBSetup.__delattr__('connect')

    erajec="Summer19UL16_V7_MC"
    process.jec = cms.ESSource("PoolDBESSource",CondDBSetup,
    #                           connect = cms.string( "frontier://FrontierPrep/CMS_COND_PHYSICSTOOLS"),
                               connect = cms.string('sqlite_file:'+erajec+'.db'),
                               toGet =  cms.VPSet(
            cms.PSet(
                record = cms.string("JetCorrectionsRecord"),
                tag = cms.string("JetCorrectorParametersCollection_"+erajec+"_AK4PF"),
                label= cms.untracked.string("AK4PF")
            ),
            cms.PSet(
                record = cms.string("JetCorrectionsRecord"),
                tag = cms.string("JetCorrectorParametersCollection_"+erajec+"_AK4PFchs"),
                label= cms.untracked.string("AK4PFchs")
            ),
            )
                               )
    process.es_prefer_jec = cms.ESPrefer("PoolDBESSource",'jec')


    erajer="Summer20UL16_JRV3_MC"

    process.jer = cms.ESSource("PoolDBESSource",
            CondDBSetup,
            toGet = cms.VPSet(
                # Resolution
                cms.PSet(
                    record = cms.string('JetResolutionRcd'),
                    tag    = cms.string('JR_'+erajer+'_PtResolution_AK4PFchs'),
                    label  = cms.untracked.string('AK4PFchs_pt')
                ),
                # Scale factors
                cms.PSet(
                    record = cms.string('JetResolutionScaleFactorRcd'),
                    tag    = cms.string('JR_'+erajer+'_SF_AK4PFchs'),
                    label  = cms.untracked.string('AK4PFchs')
                ),
                ),
#            connect = cms.string( "frontier://FrontierPrep/CMS_COND_PHYSICSTOOLS"),
            connect = cms.string('sqlite_file:'+erajer+'.db'),

        )

    process.es_prefer_jer = cms.ESPrefer('PoolDBESSource', 'jer')

runMetCorAndUncFromMiniAOD(process,
                           isData=False,
#                           pfCandColl=cms.InputTag("packedPFCandidates"), #maybe not needed
#                           recoMetFromPFCs=True, #maybe not needed
#                           CHS = False, #This is an important step and determines what type of jets to be reclustered. Maybe not needed
#                           reclusterJets = False #maybe not needed
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
   hlttrig  = cms.untracked.InputTag('TriggerResults','','HLT'),
   genvertex= cms.untracked.InputTag('genParticles','xyz0'),   
   genparticles = cms.untracked.InputTag('prunedGenParticles'),
   geneventinfo = cms.untracked.InputTag('generator'),
   pileupinfo = cms.untracked.InputTag('slimmedAddPileupInfo'),
   lheinfo  = cms.untracked.InputTag('externalLHEProducer'),
   slimmedmet = cms.untracked.InputTag('slimmedMETs'),
   datatier = cms.untracked.string('MINIAOD')
                              )

process.TFileService = cms.Service("TFileService", fileName = cms.string("nanoaod.root") )

#process.p = cms.Path(process.fullPatMetSequence*process.prefiringweightpreVFP*process.prefiringweightpostVFP*process.demo) 
process.p = cms.Path(process.prefiringweightpreVFP*process.prefiringweightpostVFP*process.demo) #uncomment previous line to reevaluate MET
