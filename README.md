# MuonAnalysis

Analyzer to produce custom NanoAOD ntuples from MiniAOD or AOD. Below are the instructions to run the analyzer

# Setup

```
cmsrel CMSSW_10_6_29
cd CMSSW_10_6_29/src
cmsenv
git init
git remote add -f origin https://github.com/dbruschi/cmssw.git
git config core.sparseCheckout true
echo "PhysicsTools/PatAlgos/" >> .git/info/sparse-checkout
git pull origin CMSSW_10_6_X
scram b
mkdir MuonAnalysis
cd MuonAnalysis
git clone https://github.com/dbruschi/MuonAnalysis.git
cd MuonAnalysis
scram b

```

# Run on MiniAOD
CmsRun config files are available in the python/ directory
```
voms-proxy-init -voms cms #to run on files available remotely
cmsRun python/ConfFile_cfg.py
```

# Run on AOD
CmsRun config files are available in the python/ directory
```
voms-proxy-init -voms cms #to run on files available remotely
cmsRun python/ConfFile_cfg_AOD.py
```

# CRAB submission
```
voms-proxy-init -voms cms
export X509_USER_PROXY = {name of the proxy file}
crab submit -c crab_config.py #to be modified to be run on MiniAOD or AOD.
#In particular "config.JobType.psetName = python/ConfFile_cfg.py" or "config.JobType.psetName = python/ConfFile_cfg_AOD.py"
#and config.Data.inputDataset need to be modified
```
