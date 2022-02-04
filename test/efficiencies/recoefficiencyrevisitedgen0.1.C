#include "ROOT/RDataFrame.hxx"
#include "ROOT/RVec.hxx"
#include "TFile.h"
#include "TLorentzVector.h"
#include "TStyle.h"
#include "TCanvas.h"
#include "TH2D.h"
#include "TApplication.h"

using namespace ROOT;

auto goodgenvalue(RVec<float> &GenPart_pt, int &GenPart_postFSRLepIdx1, int &GenPart_postFSRLepIdx2, RVec<float> &GenPart_eta, RVec<float> &GenPart_phi, RVec<int> &GenPart_status) {
	RVec<float> v;
	for (auto i=0U;i < GenPart_pt.size(); ++i) {
		if ((i==GenPart_postFSRLepIdx1)||(i==GenPart_postFSRLepIdx2)) {
			bool condition=true;
			for (auto j=0U; j < GenPart_pt.size(); ++j) {
				if (i==j) continue;
				if (GenPart_status[j]!=1) continue;
				TLorentzVector cand1, cand2;
				cand1.SetPtEtaPhiM(3.,GenPart_eta[i],GenPart_phi[i],0.);
				cand2.SetPtEtaPhiM(3.,GenPart_eta[j],GenPart_phi[j],0.);
				if (cand1.DeltaR(cand2)<0.3) condition=false;
			}
			if (condition) v.emplace_back(GenPart_pt[i]);
		}
	}
	return v;
}

auto goodgenidx(RVec<float> &GenPart_pt, int &GenPart_postFSRLepIdx1, int &GenPart_postFSRLepIdx2, RVec<float> &GenPart_eta, RVec<float> &GenPart_phi, RVec<int> &GenPart_status) {
	RVec<int> v;
	for (auto i=0U;i < GenPart_pt.size(); ++i) {
		if ((i==GenPart_postFSRLepIdx1)||(i==GenPart_postFSRLepIdx2)) {
			bool condition=true;
			for (auto j=0U; j < GenPart_pt.size(); ++j) {
				if (i==j) continue;
				if (GenPart_status[j]!=1) continue;
				TLorentzVector cand1, cand2;
				cand1.SetPtEtaPhiM(3.,GenPart_eta[i],GenPart_phi[i],0.);
				cand2.SetPtEtaPhiM(3.,GenPart_eta[j],GenPart_phi[j],0.);
				if (cand1.DeltaR(cand2)<0.3) condition=false;
			}
			if (condition) v.emplace_back(i);
		}
	}
	return v;
}

auto goodmuonbool(RVec<float> &Muon_standeta, RVec<float> &Muon_standphi, RVec<bool> &Muon_isStandalone, RVec<float> &goodgeneta, RVec<float> &goodgenphi) {
	RVec<float> v;
	for (auto i=0U;i < goodgeneta.size(); ++i) {
		bool condition=false;
		for (auto j=0U; j < Muon_standeta.size(); ++j) {
			TLorentzVector cand, cand2;
			cand.SetPtEtaPhiM(5.,goodgeneta[i],goodgenphi[i],0.);
			cand2.SetPtEtaPhiM(5.,Muon_standeta[j],Muon_standphi[j],0.);
			if ((cand.DeltaR(cand2)<0.1)&&(Muon_isStandalone[j])) {
				condition=true;
			}
		}
		if (condition) v.emplace_back(1);
		else v.emplace_back(0); 
	}
	return v;
}

auto goodtrack(RVec<float> &value, RVec<float> &Track_eta, RVec<float> &Track_phi, RVec<float> &goodgeneta, RVec<float> &goodgenphi, RVec<float> &Track_chi2, RVec<float> Track_originalAlgo, RVec<unsigned int> Track_quality){
	RVec<float> v;
	for (auto j=0U; j < goodgeneta.size(); ++j) {
		float Val;
		bool condition=false;
		for (auto i=0U;i < Track_eta.size(); ++i) {
			TLorentzVector cand, cand2;
			cand.SetPtEtaPhiM(5.,goodgeneta[j],goodgenphi[j],0.);
			cand2.SetPtEtaPhiM(5.,Track_eta[i],Track_phi[i],0.);
//			if ((cand.DeltaR(cand2)<0.2)&&(Track_chi2[i]<5.)&&(Track_quality[i]&4)&&(Track_originalAlgo[i]!=13)&&(Track_originalAlgo[i]!=14)) condition=true;
			if ((cand.DeltaR(cand2)<0.05)&&(Track_chi2[i]<5.)&&(Track_originalAlgo[i]!=13)&&(Track_originalAlgo[i]!=14)) {condition=true;
				Val=value[j];
			}
		}
		if (condition) v.emplace_back(Val);
	}
	return v;
}

auto goodtrackreal(RVec<float> &value, RVec<float> &Track_eta, RVec<float> &Track_phi, RVec<float> &goodgeneta, RVec<float> &goodgenphi, RVec<float> &Track_chi2, RVec<float> Track_originalAlgo, RVec<unsigned int> Track_quality){
	RVec<float> v;
	for (auto j=0U; j < goodgeneta.size(); ++j) {
		float Val;
		bool condition=false;
		for (auto i=0U;i < value.size(); ++i) {
			TLorentzVector cand, cand2;
			cand.SetPtEtaPhiM(5.,goodgeneta[j],goodgenphi[j],0.);
			cand2.SetPtEtaPhiM(5.,Track_eta[i],Track_phi[i],0.);
//			if ((cand.DeltaR(cand2)<0.2)&&(Track_chi2[i]<5.)&&(Track_quality[i]&4)&&(Track_originalAlgo[i]!=13)&&(Track_originalAlgo[i]!=14)) condition=true;
			if ((cand.DeltaR(cand2)<0.05)&&(Track_chi2[i]<5.)&&(Track_originalAlgo[i]!=13)&&(Track_originalAlgo[i]!=14)) {condition=true;
				Val=value[i];
			}
		}
		if (condition) v.emplace_back(Val);
	}
	return v;
}


auto isgoodtrack(RVec<float> &goodtracketa, RVec<float> &goodtrackphi, RVec<float> &Muon_eta, RVec<float> &Muon_phi, RVec<bool> &Muon_isStandalone) {
	RVec<bool> v;
	for (auto i=0U;i < goodtracketa.size(); ++i) {
		bool condition=false;
		for (auto j=0U; j < Muon_eta.size(); ++j) {
			TLorentzVector cand, cand2;
			cand.SetPtEtaPhiM(5.,goodtracketa[i],goodtrackphi[i],0.);
			cand2.SetPtEtaPhiM(5.,Muon_eta[j],Muon_phi[j],0.);
			if ((cand.DeltaR(cand2)<0.1)&&(Muon_isStandalone[j])) condition=true;
		}
		if (condition) v.emplace_back(true);
		else v.emplace_back(false);
	}
	return v;
}

void recoefficiencyrevisited(const char* name) {
	gStyle->SetOptStat(0);
	RDataFrame df("demo/Events",name);
	auto d1=df.Define("goodgenpt",goodgenvalue,{"GenPart_pt","GenPart_postFSRLepIdx1","GenPart_postFSRLepIdx2","GenPart_eta","GenPart_phi","GenPart_status"}).Define("goodgeneta",goodgenvalue,{"GenPart_eta","GenPart_postFSRLepIdx1","GenPart_postFSRLepIdx2","GenPart_eta","GenPart_phi","GenPart_status"}).Define("goodgenphi",goodgenvalue,{"GenPart_phi","GenPart_postFSRLepIdx1","GenPart_postFSRLepIdx2","GenPart_eta","GenPart_phi","GenPart_status"}).Define("goodgenidx",goodgenidx,{"GenPart_pt","GenPart_postFSRLepIdx1","GenPart_postFSRLepIdx2","GenPart_eta","GenPart_phi","GenPart_status"}).Define("goodmuonbool",goodmuonbool,{"Muon_standeta","Muon_standphi","Muon_isStandalone","goodgeneta","goodgenphi"});
	d1=d1.Define("goodtrackpt",goodtrack,{"goodgenpt","Track_eta","Track_phi","goodgeneta","goodgenphi","Track_chi2","Track_originalAlgo","Track_quality"}).Define("goodtracketa",goodtrack,{"goodgeneta","Track_eta","Track_phi","goodgeneta","goodgenphi","Track_chi2","Track_originalAlgo","Track_quality"}).Define("goodtrackphi",goodtrack,{"goodgenphi","Track_eta","Track_phi","goodgeneta","goodgenphi","Track_chi2","Track_originalAlgo","Track_quality"}).Define("goodtrackrealeta",goodtrackreal,{"Track_eta","Track_eta","Track_phi","goodgeneta","goodgenphi","Track_chi2","Track_originalAlgo","Track_quality"}).Define("goodtrackrealphi",goodtrackreal,{"Track_phi","Track_eta","Track_phi","goodgeneta","goodgenphi","Track_chi2","Track_originalAlgo","Track_quality"});
	d1=d1.Define("goodtrack",isgoodtrack,{"goodtrackrealeta","goodtrackrealphi","Muon_standeta","Muon_standphi","Muon_isStandalone"});
	auto histo1 = d1.Histo2D({"histo1", "", 48, -2.4, 2.4, 9, 25., 60.}, "goodgeneta", "goodgenpt", "goodmuonbool");
	auto histo2 = d1.Histo2D({"histo2", "", 48, -2.4, 2.4, 9, 25., 60.}, "goodgeneta", "goodgenpt");
	auto histo3 = d1.Histo2D({"histo3", "", 48, -2.4, 2.4, 9, 25., 60.}, "goodtracketa", "goodtrackpt", "goodtrack");
	auto histo4 = d1.Histo2D({"histo4", "", 48, -2.4, 2.4, 9, 25., 60.}, "goodtracketa", "goodtrackpt");
//	TFile* output=new TFile("output.root","RECREATE");
//	output->cd();
	TH2D* Histo1=(TH2D*)histo1.GetPtr()->Clone(), *Histo2=(TH2D*)histo2.GetPtr()->Clone(), *Histo3=(TH2D*)histo3.GetPtr()->Clone(), *Histo4=(TH2D*)histo4.GetPtr()->Clone();
	Histo1->Divide(Histo2);
	Histo3->Divide(Histo4);
	TCanvas *c1=new TCanvas();
	c1->cd();
	c1->Draw();
	Histo1->SetTitle("P(standalone|gen)");
	Histo1->GetXaxis()->SetTitle("GenMuon #eta");
	Histo1->GetYaxis()->SetTitle("GenMuon p_{T} [GeV]");
	Histo1->Draw("colz");
	TCanvas *c3=new TCanvas();
	c3->cd();
	c3->Draw();
	Histo3->SetTitle("P(standalone|tracker-seeded track)");
	Histo3->GetXaxis()->SetTitle("GenMuon #eta");
	Histo3->GetYaxis()->SetTitle("GenMuon p_{T} [GeV]");
	Histo3->Draw("colz");
	TH2D* Histo5=(TH2D*)Histo1->Clone();
	Histo5->Divide(Histo3);
	TCanvas *c5=new TCanvas();
	c5->cd();
	c5->Draw();
	Histo5->SetTitle("MC truth vs tag and probe");
	Histo5->GetXaxis()->SetTitle("GenMuon #eta");
	Histo5->GetYaxis()->SetTitle("GenMuon p_{T} [GeV]");
	Histo5->Draw("colz");
//	output->Write();
//	output->Close();
}

int main(int argc, char **argv) {
	char** abcd;
	TApplication theApp("App",0,abcd);
	recoefficiencyrevisited(argv[1]);
	theApp.Run();
	return 0;
}
