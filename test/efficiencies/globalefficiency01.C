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

void globalefficiency(const char* name) {
	gStyle->SetOptStat(0);
	RDataFrame df("demo/Events",name);
	auto d1=df.Define("goodgenpt",goodgenvalue,{"GenPart_pt","GenPart_postFSRLepIdx1","GenPart_postFSRLepIdx2","GenPart_eta","GenPart_phi","GenPart_status"}).Define("goodgeneta",goodgenvalue,{"GenPart_eta","GenPart_postFSRLepIdx1","GenPart_postFSRLepIdx2","GenPart_eta","GenPart_phi","GenPart_status"}).Define("goodgenphi",goodgenvalue,{"GenPart_phi","GenPart_postFSRLepIdx1","GenPart_postFSRLepIdx2","GenPart_eta","GenPart_phi","GenPart_status"}).Define("goodgenidx",goodgenidx,{"GenPart_pt","GenPart_postFSRLepIdx1","GenPart_postFSRLepIdx2","GenPart_eta","GenPart_phi","GenPart_status"}).Define("goodmuonbool",goodmuonboolglobal,{"goodgeneta", "goodgenphi", "Muon_standeta", "Muon_standphi", "Muon_isGlobal"}).Define("goodmuonstand",goodmuonboolglobal,{"goodgeneta", "goodgenphi", "Muon_standeta", "Muon_standphi", "Muon_isGlobal"});
	d1=d1.Define("goodtrackpt",goodtrack,{"goodgenpt","Track_eta","Track_phi","goodgeneta","goodgenphi","Track_chi2","Track_originalAlgo","Track_quality"}).Define("goodtracketa",goodtrack,{"goodgeneta","Track_eta","Track_phi","goodgeneta","goodgenphi","Track_chi2","Track_originalAlgo","Track_quality"}).Define("goodtrackphi",goodtrack,{"goodgenphi","Track_eta","Track_phi","goodgeneta","goodgenphi","Track_chi2","Track_originalAlgo","Track_quality"}).Define("goodtrackrealeta",goodtrackreal,{"Track_eta","Track_eta","Track_phi","goodgeneta","goodgenphi","Track_chi2","Track_originalAlgo","Track_quality"}).Define("goodtrackrealphi",goodtrackreal,{"Track_phi","Track_eta","Track_phi","goodgeneta","goodgenphi","Track_chi2","Track_originalAlgo","Track_quality"});
	d1=d1.Define("goodtrack",isgoodtrack,{"goodtrackrealeta","goodtrackrealphi","Muon_standeta","Muon_standphi","Muon_isStandalone"});
	d1=d1.Define("goodstandpt",goodmuon,{"goodgenpt","goodgeneta","goodgenphi","Muon_standeta","Muon_standphi","Muon_isStandalone"}).Define("goodstandeta",goodmuon,{"goodgeneta","goodgeneta","goodgenphi","Muon_standeta","Muon_standphi","Muon_isStandalone"}).Define("goodstandphi",goodmuon,{"goodgenphi","goodgeneta","goodgenphi","Muon_standeta","Muon_standphi","Muon_isStandalone"}).Define("goodmuonidx",goodmuonidx,{"goodgeneta","goodgenphi","Muon_standeta","Muon_standphi","Muon_isStandalone"});
	d1=d1.Define("goodstand",goodstand,{"goodmuonidx","Muon_isGlobal"});
	auto histo1 = d1.Histo2D({"histo1", "", 48, -2.4, 2.4, 9, 25., 60.}, "goodgeneta", "goodgenpt", "goodmuonbool");
	auto histo2 = d1.Histo2D({"histo2", "", 48, -2.4, 2.4, 9, 25., 60.}, "goodgeneta", "goodgenpt");
	auto histo3 = d1.Histo2D({"histo3", "", 48, -2.4, 2.4, 9, 25., 60.}, "goodtracketa", "goodtrackpt", "goodtrack");
	auto histo4 = d1.Histo2D({"histo4", "", 48, -2.4, 2.4, 9, 25., 60.}, "goodtracketa", "goodtrackpt");
	auto histo5 = d1.Histo2D({"histo5", "", 48, -2.4, 2.4, 9, 25., 60.}, "goodstandeta", "goodstandpt", "goodstand");
	auto histo6 = d1.Histo2D({"histo6", "", 48, -2.4, 2.4, 9, 25., 60.}, "goodstandeta", "goodstandpt");
//	TFile* output=new TFile("output.root","RECREATE");
//	output->cd();
	TH2D* Histo1=(TH2D*)histo1.GetPtr()->Clone(), *Histo2=(TH2D*)histo2.GetPtr()->Clone(), *Histo3=(TH2D*)histo3.GetPtr()->Clone(), *Histo4=(TH2D*)histo4.GetPtr()->Clone(), *Histo5=(TH2D*)histo5.GetPtr()->Clone(), *Histo6=(TH2D*)histo6.GetPtr()->Clone();
	double cont1=Histo1->GetBinContent(20,4), cont2=Histo2->GetBinContent(20,4), cont3=Histo3->GetBinContent(20,4), cont4=Histo4->GetBinContent(20,4), cont5=Histo5->GetBinContent(20,4), cont6=Histo6->GetBinContent(20,4);
	double cont1_2=Histo1->GetBinContent(13,4), cont2_2=Histo2->GetBinContent(13,4), cont3_2=Histo3->GetBinContent(13,4), cont4_2=Histo4->GetBinContent(13,4), cont5_2=Histo5->GetBinContent(13,4), cont6_2=Histo6->GetBinContent(13,4);
	Histo1->Divide(Histo2);
	Histo3->Divide(Histo4);
	Histo5->Divide(Histo6);
	TH2D* product=(TH2D*)Histo3->Clone();
	product->Multiply(Histo5);
	TCanvas *c1=new TCanvas();
	c1->cd();
	c1->Draw();
	Histo1->SetTitle("P(global|gen)");
//	Histo1->SetTitle("P(global&&gen)/P(standalone&&gen)");
	Histo1->GetXaxis()->SetTitle("GenMuon #eta");
	Histo1->GetYaxis()->SetTitle("GenMuon p_{T} [GeV]");
	Histo1->Draw("colz");
	TCanvas *c3=new TCanvas();
	c3->cd();
	c3->Draw();
	product->SetTitle("P(global|standalone)*P(standalone|tracker seeded)");
	product->GetXaxis()->SetTitle("GenMuon #eta");
	product->GetYaxis()->SetTitle("GenMuon p_{T} [GeV]");
	product->Draw("colz");
	TCanvas *c2=new TCanvas();
	c2->cd();
	c2->Draw();
	Histo3->SetTitle("P(standalone|tracker seeded)");
	Histo3->GetXaxis()->SetTitle("GenMuon #eta");
	Histo3->GetYaxis()->SetTitle("GenMuon p_{T} [GeV]");
	Histo3->Draw("colz");
	TCanvas *c4=new TCanvas();
	c4->cd();
	c4->Draw();
	Histo5->SetTitle("P(global|standalone)");
	Histo5->GetXaxis()->SetTitle("GenMuon #eta");
	Histo5->GetYaxis()->SetTitle("GenMuon p_{T} [GeV]");
	Histo5->Draw("colz");
	TH2D* Histo7=(TH2D*)Histo1->Clone();
	Histo7->Divide(product);
//	TH2D* Histo7=(TH2D*)product->Clone();
//	Histo7->Divide(Histo1);
	TCanvas *c7=new TCanvas();
	c7->cd();
	c7->Draw();
	Histo7->SetTitle("MC truth vs tag and probe ratio");
	Histo7->GetXaxis()->SetTitle("#eta");
	Histo7->GetYaxis()->SetTitle("p_{T} [GeV]");
	Histo7->Draw("colz");
	std::cout<<cont1<<" "<<cont2<<" "<<cont3<<" "<<cont4<<" "<<cont5<<" "<<cont6<<"\n";
	std::cout<<cont1_2<<" "<<cont2_2<<" "<<cont3_2<<" "<<cont4_2<<" "<<cont5_2<<" "<<cont6_2<<"\n";
//	output->Write();
//	output->Close();
}

int main(int argc, char **argv) {
	char** abcd;
	TApplication theApp("App",0,abcd);
	globalefficiency(argv[1]);
	theApp.Run();
	return 0;
}
