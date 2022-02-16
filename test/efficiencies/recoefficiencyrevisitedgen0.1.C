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

void recoefficiencyrevisited(const char* name) {
	gStyle->SetOptStat(0);
	RDataFrame df("demo/Events",name);
	auto d1=df.Define("goodgenpt",goodgenvalue,{"GenPart_pt","GenPart_postFSRLepIdx1","GenPart_postFSRLepIdx2","GenPart_eta","GenPart_phi","GenPart_status","GenPart_pdgId"}).Define("goodgeneta",goodgenvalue,{"GenPart_eta","GenPart_postFSRLepIdx1","GenPart_postFSRLepIdx2","GenPart_eta","GenPart_phi","GenPart_status","GenPart_pdgId"}).Define("goodgenphi",goodgenvalue,{"GenPart_phi","GenPart_postFSRLepIdx1","GenPart_postFSRLepIdx2","GenPart_eta","GenPart_phi","GenPart_status","GenPart_pdgId"}).Define("goodgenidx",goodgenidx,{"GenPart_pt","GenPart_postFSRLepIdx1","GenPart_postFSRLepIdx2","GenPart_eta","GenPart_phi","GenPart_status","GenPart_pdgId"}).Define("goodmuonbool",goodmuonboolstand,{"Muon_standeta","Muon_standphi","Muon_isStandalone","goodgeneta","goodgenphi"});
	d1=d1.Define("goodtrackpt",goodtrack,{"goodgenpt","Track_eta","Track_phi","goodgeneta","goodgenphi","Track_chi2","Track_originalAlgo","Track_quality"}).Define("goodtracketa",goodtrack,{"goodgeneta","Track_eta","Track_phi","goodgeneta","goodgenphi","Track_chi2","Track_originalAlgo","Track_quality"}).Define("goodtrackphi",goodtrack,{"goodgenphi","Track_eta","Track_phi","goodgeneta","goodgenphi","Track_chi2","Track_originalAlgo","Track_quality"}).Define("goodtrackrealeta",goodtrackreal,{"Track_eta","Track_eta","Track_phi","goodgeneta","goodgenphi","Track_chi2","Track_originalAlgo","Track_quality"}).Define("goodtrackrealphi",goodtrackreal,{"Track_phi","Track_eta","Track_phi","goodgeneta","goodgenphi","Track_chi2","Track_originalAlgo","Track_quality"});
	d1=d1.Define("goodtrack",isgoodtrack,{"goodtrackrealeta","goodtrackrealphi","Muon_standeta","Muon_standphi","Muon_isStandalone"});
	auto histo1 = d1.Histo2D({"histo1", "", 48, -2.4, 2.4, 9, 25., 60.}, "goodgeneta", "goodgenpt", "goodmuonbool");
	auto histo2 = d1.Histo2D({"histo2", "", 48, -2.4, 2.4, 9, 25., 60.}, "goodgeneta", "goodgenpt");
	auto histo3 = d1.Histo2D({"histo3", "", 48, -2.4, 2.4, 9, 25., 60.}, "goodtracketa", "goodtrackpt", "goodtrack");
	auto histo4 = d1.Histo2D({"histo4", "", 48, -2.4, 2.4, 9, 25., 60.}, "goodtracketa", "goodtrackpt");
	TFile* output=new TFile("output.root","RECREATE");
	output->cd();
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
	output->Write();
//	output->Close();
}

int main(int argc, char **argv) {
	char** abcd;
	TApplication theApp("App",0,abcd);
	recoefficiencyrevisited(argv[1]);
	theApp.Run();
	return 0;
}
