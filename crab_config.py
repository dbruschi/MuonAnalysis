import CRABClient
from WMCore.Configuration import Configuration

config = Configuration()

config.section_("General")
config.General.requestName = 'MuonAnalysis_dbruschi'
config.General.workArea = 'crab_projects'
config.General.transferOutputs = True

config.section_("JobType")
config.JobType.pluginName = 'Analysis'
config.JobType.psetName = 'python/ConfFile_cfg.py'

config.section_("Data")
config.Data.inputDataset = '/DYJetsToMuMu_M-50_TuneCP5_13TeV-powhegMiNNLO-pythia8-photos/RunIISummer20UL16MiniAOD-106X_mcRun2_asymptotic_v13-v2/MINIAODSIM'
config.Data.inputDBS = 'global'
config.Data.splitting = 'FileBased'
config.Data.unitsPerJob = 1
config.Data.publication = False
config.Data.outputDatasetTag = 'CRAB3_MuonAnalysis_dbruschi'

config.section_("Site")
config.Site.storageSite = 'T2_IT_Pisa'
