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

void unfoldingmatrix(const char* name) {
	gStyle->SetOptStat(0);
	RDataFrame df("demo/Events",name);
	auto d1=df.Define("goodgenpt",goodgenvalue,{"GenPart_pt","GenPart_postFSRLepIdx1","GenPart_postFSRLepIdx2","GenPart_eta","GenPart_phi","GenPart_status","GenPart_pdgId"}).Define("goodgeneta",goodgenvalue,{"GenPart_eta","GenPart_postFSRLepIdx1","GenPart_postFSRLepIdx2","GenPart_eta","GenPart_phi","GenPart_status","GenPart_pdgId"}).Define("goodgenphi",goodgenvalue,{"GenPart_phi","GenPart_postFSRLepIdx1","GenPart_postFSRLepIdx2","GenPart_eta","GenPart_phi","GenPart_status","GenPart_pdgId"}).Define("goodgenidx",goodgenidx,{"GenPart_pt","GenPart_postFSRLepIdx1","GenPart_postFSRLepIdx2","GenPart_eta","GenPart_phi","GenPart_status","GenPart_pdgId"});
	d1=d1.Define("zero","0").Define("one","1").Define("two","2").Define("tnppairtag",maketnppairstrack,{"Muon_pt","Muon_eta","Muon_phi","Muon_mass","Muon_standeta","Muon_standphi","Muon_isStandalone","Muon_isGlobal","Muon_mediumId", "Muon_dxyBS","Muon_triggered","Muon_pfRelIso04_all","Track_pt","Track_eta","Track_phi","Track_chi2","Track_originalAlgo","zero"}).Define("tnppairprobe",maketnppairstrack,{"Muon_pt","Muon_eta","Muon_phi","Muon_mass","Muon_standeta","Muon_standphi","Muon_isStandalone","Muon_isGlobal","Muon_mediumId", "Muon_dxyBS","Muon_triggered","Muon_pfRelIso04_all","Track_pt","Track_eta","Track_phi","Track_chi2","Track_originalAlgo","one"}).Define("tnppairflag",maketnppairstrack,{"Muon_pt","Muon_eta","Muon_phi","Muon_mass","Muon_standeta","Muon_standphi","Muon_isStandalone","Muon_isGlobal","Muon_mediumId", "Muon_dxyBS","Muon_triggered","Muon_pfRelIso04_all","Track_pt","Track_eta","Track_phi","Track_chi2","Track_originalAlgo","two"});
	d1=d1.Define("tnppairtagreco",maketnppairs,{"Muon_pt","Muon_eta","Muon_phi","Muon_mass","Muon_standeta","Muon_standphi","Muon_isStandalone","Muon_isGlobal","Muon_mediumId","Muon_dxyBS","Muon_triggered","Muon_pfRelIso04_all","zero"}).Define("tnppairprobereco",maketnppairs,{"Muon_pt","Muon_eta","Muon_phi","Muon_mass","Muon_standeta","Muon_standphi","Muon_isStandalone","Muon_isGlobal","Muon_mediumId","Muon_dxyBS","Muon_triggered","Muon_pfRelIso04_all","one"}).Define("tnppairflagreco",maketnppairs,{"Muon_pt","Muon_eta","Muon_phi","Muon_mass","Muon_standeta","Muon_standphi","Muon_isStandalone","Muon_isGlobal","Muon_mediumId","Muon_dxyBS","Muon_triggered","Muon_pfRelIso04_all","two"});
	d1=d1.Define("goodtnppairtagminiaod",goodtnppairs,{"tnppairtagreco","tnppairprobereco","tnppairflagreco","Muon_genPartIdx","Muon_genPartIdx","goodgenidx","zero"}).Define("goodtnppairprobeminiaod",goodtnppairs,{"tnppairtagreco","tnppairprobereco","tnppairflagreco","Muon_genPartIdx","Muon_genPartIdx","goodgenidx","one"}).Define("goodtnppairflagminiaod",goodtnppairs,{"tnppairtagreco","tnppairprobereco","tnppairflagreco","Muon_genPartIdx","Muon_genPartIdx","goodgenidx","two"});
	d1=d1.Define("isglobalminiaod",isglobal,{"goodtnppairflagminiaod","one"});
	d1=d1.Define("iddenominatortag","goodtnppairtagminiaod[isglobalminiaod]").Define("iddenominatorprobe","goodtnppairprobeminiaod[isglobalminiaod]").Define("iddenominatorflag","goodtnppairflagminiaod[isglobalminiaod]");

	d1=d1.Define("goodglobalpt",goodtrackgen,{"iddenominatortag","iddenominatorprobe","Muon_genPartIdx","goodgenidx","goodgenpt"}).Define("goodglobaleta",goodtrackgen,{"iddenominatortag","iddenominatorprobe","Muon_genPartIdx","goodgenidx","goodgeneta"}).Define("goodglobalrealpt",goodtrackunfolding,{"iddenominatortag","iddenominatorprobe","Muon_genPartIdx","goodgenidx","Muon_pt"}).Define("goodglobalrealeta",goodtrackunfolding,{"iddenominatortag","iddenominatorprobe","Muon_genPartIdx","goodgenidx","Muon_eta"});
	unsigned int neta=12, npt=3, nim=70;
	float mineta=-2.4, minpt=25., minim=50.;
	float maxeta=2.4, maxpt=60., maxim=120.;
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
	d1=d1.Define("unfoldingmc",unfolding,{"goodgeneta","goodgenpt"}).Define("unfoldingtnp",unfolding,{"goodglobalrealeta","goodglobalrealpt"});
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
	unfoldingmatrix(argv[1]);
	theApp.Run();
	return 0;
}
