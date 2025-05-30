#include <string>
#include <iostream>
#include "ROOT/RDataFrame.hxx"
#include "ROOT/RVec.hxx"
#include "TFile.h"
#include "TLorentzVector.h"
#include "TStyle.h"
#include "TCanvas.h"
#include "TH2D.h"
#include "TH3D.h"
#include "TApplication.h"
#include "Functionstnp.h"
#include "TMatrixF.h"

using namespace ROOT;

RVec<float> goodtrackunfolding(RVec<int> &goodtnppairtag, RVec<int> &goodtnppairprobe, RVec<int> &matchedtrackidx, RVec<int> &goodgenidx, RVec<float> &Track_pt) {
	RVec<float> v;
	for (auto i=0U;i < goodgenidx.size(); ++i) {
		bool condition=false;
		int idx;
		for (auto j=0U; j < goodtnppairprobe.size(); j++) {
			if (matchedtrackidx[goodtnppairprobe[j]]==goodgenidx[i]) {condition=true;
				idx=goodtnppairprobe[j];
			}
		}
		if (condition) v.emplace_back(Track_pt[idx]);
		else v.emplace_back(-100);
	}
	return v;
}

void unfoldingmatrix(std::vector<std::string> names) {
	gStyle->SetOptStat(0);
	RDataFrame df("demo/Events",names);
	auto d1=df.Define("goodgenpt",goodgenvalue,{"GenPart_pt","GenPart_postFSRLepIdx1","GenPart_postFSRLepIdx2","GenPart_eta","GenPart_phi","GenPart_status","GenPart_pdgId"}).Define("goodgeneta",goodgenvalue,{"GenPart_eta","GenPart_postFSRLepIdx1","GenPart_postFSRLepIdx2","GenPart_eta","GenPart_phi","GenPart_status","GenPart_pdgId"}).Define("goodgenphi",goodgenvalue,{"GenPart_phi","GenPart_postFSRLepIdx1","GenPart_postFSRLepIdx2","GenPart_eta","GenPart_phi","GenPart_status","GenPart_pdgId"}).Define("goodgenidx",goodgenidx,{"GenPart_pt","GenPart_postFSRLepIdx1","GenPart_postFSRLepIdx2","GenPart_eta","GenPart_phi","GenPart_status","GenPart_pdgId"}).Define("matchedstaidx",matchedstaidx,{"goodgeneta","goodgenphi","goodgenidx","Muon_standeta","Muon_standphi","Muon_isStandalone"});
	d1=d1.Define("zero","0").Define("one","1").Define("two","2").Define("tnppairtag",maketnppairstrack,{"Muon_pt","Muon_eta","Muon_phi","Muon_mass","Muon_standeta","Muon_standphi","Muon_isStandalone","Muon_isGlobal","Muon_mediumId", "Muon_dxyBS","Muon_triggered","Muon_pfRelIso04_all","Track_pt","Track_eta","Track_phi","Track_chi2","Track_originalAlgo","zero"}).Define("tnppairprobe",maketnppairstrack,{"Muon_pt","Muon_eta","Muon_phi","Muon_mass","Muon_standeta","Muon_standphi","Muon_isStandalone","Muon_isGlobal","Muon_mediumId", "Muon_dxyBS","Muon_triggered","Muon_pfRelIso04_all","Track_pt","Track_eta","Track_phi","Track_chi2","Track_originalAlgo","one"}).Define("tnppairflag",maketnppairstrack,{"Muon_pt","Muon_eta","Muon_phi","Muon_mass","Muon_standeta","Muon_standphi","Muon_isStandalone","Muon_isGlobal","Muon_mediumId", "Muon_dxyBS","Muon_triggered","Muon_pfRelIso04_all","Track_pt","Track_eta","Track_phi","Track_chi2","Track_originalAlgo","two"});
	d1=d1.Define("tnppairtagreco",maketnppairs,{"Muon_pt","Muon_eta","Muon_phi","Muon_mass","Muon_standeta","Muon_standphi","Muon_isStandalone","Muon_isGlobal","Muon_mediumId","Muon_dxyBS","Muon_triggered","Muon_pfRelIso04_all","zero"}).Define("tnppairprobereco",maketnppairs,{"Muon_pt","Muon_eta","Muon_phi","Muon_mass","Muon_standeta","Muon_standphi","Muon_isStandalone","Muon_isGlobal","Muon_mediumId","Muon_dxyBS","Muon_triggered","Muon_pfRelIso04_all","one"}).Define("tnppairflagreco",maketnppairs,{"Muon_pt","Muon_eta","Muon_phi","Muon_mass","Muon_standeta","Muon_standphi","Muon_isStandalone","Muon_isGlobal","Muon_mediumId","Muon_dxyBS","Muon_triggered","Muon_pfRelIso04_all","two"});
	d1=d1.Define("goodtnppairtagreco",goodtnppairs,{"tnppairtagreco","tnppairprobereco","tnppairflagreco","Muon_genPartIdx","matchedstaidx","goodgenidx","zero"}).Define("goodtnppairprobereco",goodtnppairs,{"tnppairtagreco","tnppairprobereco","tnppairflagreco","Muon_genPartIdx","matchedstaidx","goodgenidx","one"}).Define("goodtnppairflagreco",goodtnppairs,{"tnppairtagreco","tnppairprobereco","tnppairflagreco","Muon_genPartIdx","matchedstaidx","goodgenidx","two"});
	d1=d1.Define("goodstandpt",goodtrackgen,{"goodtnppairtagreco","goodtnppairprobereco","matchedstaidx","goodgenidx","goodgenpt"}).Define("goodstandeta",goodtrackgen,{"goodtnppairtagreco","goodtnppairprobereco","matchedstaidx","goodgenidx","goodgeneta"}).Define("goodstandphi",goodtrackgen,{"goodtnppairtagreco","goodtnppairprobereco","matchedstaidx","goodgenidx","goodgenphi"}).Define("goodstandrealpt",goodtrackunfolding,{"goodtnppairtagreco","goodtnppairprobereco","matchedstaidx","goodgenidx","Muon_standpt"}).Define("goodstandrealeta",goodtrackunfolding,{"goodtnppairtagreco","goodtnppairprobereco","matchedstaidx","goodgenidx","Muon_standeta"}).Define("goodstandrealphi",goodtrackunfolding,{"goodtnppairtagreco","goodtnppairprobereco","matchedstaidx","goodgenidx","Muon_standphi"});
	int neta=48, npt=9, nqt=25;
	float mineta=-2.4, minpt=25., minqt=0.;
	float maxeta=2.4, maxpt=60., maxqt=100.;
	auto unfolding = [&neta,&maxeta,&mineta,&npt,&maxpt,&minpt](RVec<float> &eta, RVec<float> &pt){
		RVec<float> v;
		for (auto i=0U; i!=pt.size(); i++) {
			bool flag=false;
			for (unsigned int I=1; I!=neta+1; I++) {
				float binetamin=(maxeta-mineta)/neta*(I-1)+mineta, binetamax=(maxeta-mineta)/neta*I+mineta;
				for (unsigned int J=1; J!=npt+1; J++) {
					float binptmin=(maxpt-minpt)/npt*(J-1)+minpt, binptmax=(maxpt-minpt)/npt*J+minpt;
					if ((eta[i]>binetamin)&&(eta[i]<binetamax)&&(pt[i]>binptmin)&&(pt[i]<binptmax)) {v.emplace_back((I-1)*npt+(J-1));
					flag=true;}
				}
			}
			if (!flag) v.emplace_back(-10);
		}
		return v;
	};
	d1=d1.Define("unfoldingmc",unfolding,{"goodgeneta","goodgenpt"}).Define("unfoldingtnp",unfolding,{"goodstandrealeta","goodstandrealpt"});
	auto histo1=d1.Histo2D({"histo1","",neta*npt,-0.5,neta*npt+0.5,neta*npt,-0.5,neta*npt+0.5},"unfoldingmc","unfoldingtnp");
	TH2D* Histo1=(TH2D*)histo1.GetPtr()->Clone();
	TMatrixF* matrix=new TMatrixF(neta*npt,neta*npt);
	for (unsigned int i=0; i!=neta*npt; i++) {
		Histo1->GetXaxis()->SetRange(i+1,i+1);
		TH1D* projection=(TH1D*)Histo1->ProjectionY();
		float integral=projection->Integral();
		Histo1->GetXaxis()->SetRange(1,neta*npt);
		for (unsigned int j=0; j!=neta*npt; j++) {
			if (integral>0) (*matrix)[i][j]=Histo1->GetBinContent(i+1,j+1)/integral;
			else (*matrix)[i][j]=0.;
		}
	}
	matrix->Invert();
	matrix->Print();
	matrix->Draw();
}

int main(int argc, char **argv){
	char** abcd;
	TApplication theApp("App",0,abcd);
	std::vector<std::string> names;
	for (int i=1; i!=argc; i++) {
		names.push_back(std::string(argv[i]));
	}
	unfoldingmatrix(names);
	theApp.Run();
	return 0;
}
