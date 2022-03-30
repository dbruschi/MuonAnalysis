#include <string>
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

using namespace ROOT;

RVec<float> genzpt(RVec<float> &pt, RVec<float> &eta, RVec<float> &phi, RVec<bool> &goodmuonbool){
	RVec<float> v;
	for (unsigned int i=0; i!=pt.size(); i++) {
		TLorentzVector cand1, cand2;
		cand1.SetPtEtaPhiM(pt[i],eta[i],phi[i],0);
		for (unsigned int j=0; j!=pt.size(); j++) {
			if (i==j) continue;
			cand2.SetPtEtaPhiM(pt[j],eta[j],phi[j],0);
			if (goodmuonbool[i]) v.emplace_back((cand1+cand2).Pt());
		}
	}
	return v;
}

RVec<float> zpt(RVec<float> &tagpt, RVec<float> &tageta, RVec<float> &tagphi, RVec<float> &probept, RVec<float> &probeeta, RVec<float> &probephi){
	RVec<float> v;
	for (unsigned int i=0; i!=tagpt.size(); i++) {
		TLorentzVector cand1, cand2;
		cand1.SetPtEtaPhiM(tagpt[i],tageta[i],tagphi[i],0);
		cand2.SetPtEtaPhiM(probept[i],probeeta[i],probephi[i],0);
		v.emplace_back((cand1+cand2).Pt());
	}
	return v;
}

void zptcomparison(std::vector<std::string> names) {
	gStyle->SetOptStat(0);
	RDataFrame df("demo/Events",names);
	auto d1=df.Define("goodgenpt",goodgenvalue,{"GenPart_pt","GenPart_postFSRLepIdx1","GenPart_postFSRLepIdx2","GenPart_eta","GenPart_phi","GenPart_status","GenPart_pdgId"}).Define("goodgeneta",goodgenvalue,{"GenPart_eta","GenPart_postFSRLepIdx1","GenPart_postFSRLepIdx2","GenPart_eta","GenPart_phi","GenPart_status","GenPart_pdgId"}).Define("goodgenphi",goodgenvalue,{"GenPart_phi","GenPart_postFSRLepIdx1","GenPart_postFSRLepIdx2","GenPart_eta","GenPart_phi","GenPart_status","GenPart_pdgId"}).Define("goodgenidx",goodgenidx,{"GenPart_pt","GenPart_postFSRLepIdx1","GenPart_postFSRLepIdx2","GenPart_eta","GenPart_phi","GenPart_status","GenPart_pdgId"}).Define("goodmuonbool",goodmuonboolglobal,{"goodgeneta","goodgenphi","goodgenidx","Muon_genPartIdx","Muon_isGlobal","Muon_standeta","Muon_standphi","Muon_eta","Muon_phi","Muon_isStandalone"});
	d1=d1.Define("zero","0").Define("one","1").Define("two","2");
	d1=d1.Define("tnppairtagreco",maketnppairs,{"Muon_pt","Muon_eta","Muon_phi","Muon_mass","Muon_standeta","Muon_standphi","Muon_isStandalone","Muon_isGlobal","Muon_mediumId","Muon_dxyBS","Muon_triggered","Muon_pfRelIso04_all","zero"}).Define("tnppairprobereco",maketnppairs,{"Muon_pt","Muon_eta","Muon_phi","Muon_mass","Muon_standeta","Muon_standphi","Muon_isStandalone","Muon_isGlobal","Muon_mediumId","Muon_dxyBS","Muon_triggered","Muon_pfRelIso04_all","one"}).Define("tnppairflagreco",maketnppairs,{"Muon_pt","Muon_eta","Muon_phi","Muon_mass","Muon_standeta","Muon_standphi","Muon_isStandalone","Muon_isGlobal","Muon_mediumId","Muon_dxyBS","Muon_triggered","Muon_pfRelIso04_all","two"});
	d1=d1.Define("goodtnppairtagminiaod",goodtnppairs,{"tnppairtagreco","tnppairprobereco","tnppairflagreco","Muon_genPartIdx","Muon_genPartIdx","goodgenidx","zero"}).Define("goodtnppairprobeminiaod",goodtnppairs,{"tnppairtagreco","tnppairprobereco","tnppairflagreco","Muon_genPartIdx","Muon_genPartIdx","goodgenidx","one"}).Define("goodtnppairflagminiaod",goodtnppairs,{"tnppairtagreco","tnppairprobereco","tnppairflagreco","Muon_genPartIdx","Muon_genPartIdx","goodgenidx","two"});
	d1=d1.Define("goodprobept",goodtrackgen,{"goodtnppairtagminiaod","goodtnppairprobeminiaod","Muon_genPartIdx","goodgenidx","goodgenpt"}).Define("goodprobeeta",goodtrackgen,{"goodtnppairtagminiaod","goodtnppairprobeminiaod","Muon_genPartIdx","goodgenidx","goodgeneta"}).Define("goodprobephi",goodtrackgen,{"goodtnppairtagminiaod","goodtnppairprobeminiaod","Muon_genPartIdx","goodgenidx","goodgenphi"}).Define("goodtagpt",goodtrackgen,{"goodtnppairtagminiaod","goodtnppairtagminiaod","Muon_genPartIdx","goodgenidx","goodgenpt"}).Define("goodtageta",goodtrackgen,{"goodtnppairtagminiaod","goodtnppairtagminiaod","Muon_genPartIdx","goodgenidx","goodgeneta"}).Define("goodtagphi",goodtrackgen,{"goodtnppairtagminiaod","goodtnppairtagminiaod","Muon_genPartIdx","goodgenidx","goodgenphi"});
	d1=d1.Define("genzpt",genzpt,{"goodgenpt","goodgeneta","goodgenphi","goodmuonbool"}).Define("tnpzpt",zpt,{"goodtagpt","goodtageta","goodtagphi","goodprobept","goodprobeeta","goodprobephi"});
	d1=d1.Define("Goodgeneta","goodgeneta[goodmuonbool]").Define("absGoodgeneta","abs(Goodgeneta)").Define("Goodgenpt","goodgenpt[goodmuonbool]");
	d1=d1.Define("absgoodprobeeta","abs(goodprobeeta)");
	int neta=24, npt=9, nqt=25;
	float mineta=0, minpt=25., minqt=0.;
	float maxeta=2.4, maxpt=60., maxqt=100.;
	auto c=d1.Filter("goodgeneta.size()>=2");
	auto histo1 = c.Histo1D({"histo1", "", nqt, minqt, maxqt}, "genzpt");
	auto histo2 = c.Histo1D({"histo2", "", nqt, minqt, maxqt}, "tnpzpt");
	auto histo3 = c.Histo3D({"histo3", "", neta, mineta, maxeta, npt, minpt, maxpt, nqt, minqt, maxqt}, "absGoodgeneta", "Goodgenpt", "genzpt");
	auto histo4 = c.Histo3D({"histo4", "", neta, mineta, maxeta, npt, minpt, maxpt, nqt, minqt, maxqt}, "absgoodprobeeta", "goodprobept", "tnpzpt");
	TH1D* Histo1=(TH1D*)histo1.GetPtr()->Clone(), *Histo2=(TH1D*)histo2.GetPtr()->Clone();
	TH3D* Histo3=(TH3D*)histo3.GetPtr()->Clone(), *Histo4=(TH3D*)histo4.GetPtr()->Clone();
	Histo1->Scale(1./Histo1->Integral(1,nqt+1));
	Histo2->Scale(1./Histo2->Integral(1,nqt+1));
	TH1D* ratio=(TH1D*)Histo1->Clone();
	ratio->SetName("ratio");
	ratio->Divide(Histo2);
	TCanvas *c1=new TCanvas();
	c1->cd();
	c1->Draw();
	Histo1->SetTitle("MC truth");
	Histo1->GetXaxis()->SetTitle("Z p_{T}");
	Histo1->Draw();
	TCanvas *c2=new TCanvas();
	c2->cd();
	c2->Draw();
	Histo2->SetTitle("TNP");
	Histo2->GetXaxis()->SetTitle("Z p_{T}");
	Histo2->Draw();
	TCanvas *c3=new TCanvas();
	c3->cd();
	c3->Draw();
	ratio->GetXaxis()->SetTitle("Z p_{T}");
	ratio->SetTitle("MC truth vs TNP");
	ratio->Draw();
	TFile* output=new TFile("zqt.root","RECREATE");
	output->cd();
	for (unsigned int i=1; i<=neta; i++) {
		float binetamin=(maxeta-mineta)/neta*(i-1)+mineta, binetamax=(maxeta-mineta)/neta*i+mineta;
		Histo3->GetXaxis()->SetRange(i,i);
		Histo4->GetXaxis()->SetRange(i,i);
		for (unsigned int j=1; j<=npt; j++) {
			float binptmin=(maxpt-minpt)/npt*(j-1)+minpt, binptmax=(maxpt-minpt)/npt*j+minpt;
			Histo3->GetYaxis()->SetRange(j,j);
			Histo4->GetYaxis()->SetRange(j,j);
			std::string name("MCtruth");
			name+="_"+std::to_string(i)+"_"+std::to_string(j);
			std::string title("MC truth Z q_{T} |#eta|>");
			title+=std::to_string(binetamin)+"&&|#eta|<"+std::to_string(binetamax)+"&&p_{T}>"+std::to_string(binptmin)+"&&p_{T}<"+std::to_string(binptmax);
			TH1D* histo=(TH1D*)Histo3->Project3D("z");
			histo->SetName(name.c_str());
			histo->SetTitle(title.c_str());
			histo->Scale(1./histo->Integral(1,nqt+1));
			histo->SetDirectory(output->GetDirectory(""));
			name="TNP";
			name+="_"+std::to_string(i)+"_"+std::to_string(j);
			title="TNP Z q_{T} |#eta|>";
			title+=std::to_string(binetamin)+"&&|#eta|<"+std::to_string(binetamax)+"&&p_{T}>"+std::to_string(binptmin)+"&&p_{T}<"+std::to_string(binptmax);
			TH1D* histo_=(TH1D*)Histo4->Project3D("z");
			histo_->SetName(name.c_str());
			histo_->SetTitle(title.c_str());
			histo_->Scale(1./histo_->Integral(1,nqt+1));
			histo_->SetDirectory(output->GetDirectory(""));
			TH1D* Ratio=(TH1D*)histo->Clone();
			std::string Rationame("ratio"), Ratiotitle("Z q_{T} MC truth vs TNP |#eta|>");
			Rationame+=std::to_string(i)+"_"+std::to_string(j);
			Ratiotitle+=std::to_string(binetamin)+"&&|#eta|<"+std::to_string(binetamax)+"&&p_{T}>"+std::to_string(binptmin)+"&&p_{T}<"+std::to_string(binptmax);
			Ratio->SetName(Rationame.c_str());
			Ratio->SetTitle(Ratiotitle.c_str());
			Ratio->SetDirectory(output->GetDirectory(""));
			Ratio->Divide(histo_);
		}
	}
	output->Write();
	output->Close();
}

int main(int argc, char **argv) {
	char** abcd;
	TApplication theApp("App",0,abcd);
	std::vector<std::string> names;
	for (int i=1; i!=argc; i++) {
		names.push_back(std::string(argv[i]));
	}
	zptcomparison(names);
	theApp.Run();
	return 0;
}
