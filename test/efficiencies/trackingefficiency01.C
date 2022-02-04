#include "ROOT/RDataFrame.hxx"
#include "ROOT/RVec.hxx"
#include "TFile.h"
#include "TLorentzVector.h"
#include "TStyle.h"
#include "TCanvas.h"
#include "TH2D.h"
#include "TApplication.h"
#include "Functions.h"

using namespace ROOT;

void trackingefficiency(const char* name) {
	gStyle->SetOptStat(0);
	RDataFrame df("demo/Events",name);
	auto d1=df.Define("goodgenpt",goodgenvalue,{"GenPart_pt","GenPart_postFSRLepIdx1","GenPart_postFSRLepIdx2","GenPart_eta","GenPart_phi","GenPart_status"}).Define("goodgeneta",goodgenvalue,{"GenPart_eta","GenPart_postFSRLepIdx1","GenPart_postFSRLepIdx2","GenPart_eta","GenPart_phi","GenPart_status"}).Define("goodgenphi",goodgenvalue,{"GenPart_phi","GenPart_postFSRLepIdx1","GenPart_postFSRLepIdx2","GenPart_eta","GenPart_phi","GenPart_status"}).Define("goodgenidx",goodgenidx,{"GenPart_pt","GenPart_postFSRLepIdx1","GenPart_postFSRLepIdx2","GenPart_eta","GenPart_phi","GenPart_status"}).Define("goodmuonbool",goodmuonbooltrack,{"goodgeneta","goodgenphi","Track_eta","Track_phi","Track_originalAlgo","Track_quality", "Track_chi2"});
	d1=d1.Define("goodmuonpt",goodmuon,{"goodgenpt","goodgeneta","goodgenphi","Muon_standeta","Muon_standphi","Muon_isStandalone"}).Define("goodmuoneta",goodmuon,{"goodgeneta","goodgeneta","goodgenphi","Muon_standeta","Muon_standphi","Muon_isStandalone"}).Define("goodmuonphi",goodmuon,{"goodgenphi","goodgeneta","goodgenphi","Muon_standeta","Muon_standphi","Muon_isStandalone"}).Define("goodmuonrealeta",goodmuonreal,{"Muon_standeta","goodgeneta","goodgenphi","Muon_standeta","Muon_standphi","Muon_isStandalone"}).Define("goodmuonrealphi",goodmuonreal,{"Muon_standphi","goodgeneta","goodgenphi","Muon_standeta","Muon_standphi","Muon_isStandalone"});
	d1=d1.Define("goodmuon",isgoodmuon,{"Track_eta","Track_phi","goodmuonrealeta","goodmuonrealphi","Track_originalAlgo","Track_quality","Track_chi2"});
	auto histo1 = d1.Histo2D({"histo1", "", 48, -2.4, 2.4, 9, 25., 60.}, "goodgeneta", "goodgenpt", "goodmuonbool");
	auto histo2 = d1.Histo2D({"histo2", "", 48, -2.4, 2.4, 9, 25., 60.}, "goodgeneta", "goodgenpt");
	auto histo3 = d1.Histo2D({"histo3", "", 48, -2.4, 2.4, 9, 25., 60.}, "goodmuoneta", "goodmuonpt", "goodmuon");
	auto histo4 = d1.Histo2D({"histo4", "", 48, -2.4, 2.4, 9, 25., 60.}, "goodmuoneta", "goodmuonpt");
//	auto histo1 = d1.Histo2D({"histo1", "", 60, -3.0, 3.0, 11, 15., 70.}, "goodgeneta", "goodgenpt", "goodmuonbool");
//	auto histo2 = d1.Histo2D({"histo2", "", 60, -3.0, 3.0, 11, 15., 70.}, "goodgeneta", "goodgenpt");
//	auto histo3 = d1.Histo2D({"histo3", "", 60, -3.0, 3.0, 11, 15., 70.}, "goodmuoneta", "goodmuonpt", "goodmuon");
//	auto histo4 = d1.Histo2D({"histo4", "", 60, -3.0, 3.0, 11, 15., 70.}, "goodmuoneta", "goodmuonpt");
//	TFile* output=new TFile("output.root","RECREATE");
//	output->cd();
	TH2D* Histo1=(TH2D*)histo1.GetPtr()->Clone(), *Histo2=(TH2D*)histo2.GetPtr()->Clone(), *Histo3=(TH2D*)histo3.GetPtr()->Clone(), *Histo4=(TH2D*)histo4.GetPtr()->Clone();
	Histo1->Divide(Histo2);
	Histo3->Divide(Histo4);
	TCanvas *c1=new TCanvas();
	c1->cd();
	c1->Draw();
	Histo1->SetTitle("P(tracker-seeded track|gen)");
	Histo1->GetXaxis()->SetTitle("GenMuon #eta");
	Histo1->GetYaxis()->SetTitle("GenMuon p_{T} [GeV]");
	Histo1->Draw("colz");
	TCanvas *c3=new TCanvas();
	c3->cd();
	c3->Draw();
	Histo3->SetTitle("P(tracker-seeded track|standalone)");
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
	trackingefficiency(argv[1]);
	theApp.Run();
	return 0;
}
