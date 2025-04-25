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

void idip(const char* name) {
	gStyle->SetOptStat(0);
	ROOT::EnableImplicitMT();
	auto d1=df.Define("zero","0").Define("one","1").Define("GenPart_postFSRLepIdx1", PostFSRIdx, {"GenPart_pdgId", "GenPart_status", "GenPart_genPartIdxMother", "GenPart_statusFlags", "GenPart_pt", "zero"}).Define("GenPart_postFSRLepIdx2",PostFSRIdx, {"GenPart_pdgId", "GenPart_status", "GenPart_genPartIdxMother", "GenPart_statusFlags", "GenPart_pt", "one"}).Define("goodgenpt",goodgenvalue,{"GenPart_pt","GenPart_postFSRLepIdx1","GenPart_postFSRLepIdx2","GenPart_eta","GenPart_phi","GenPart_status","GenPart_pdgId"}).Define("goodgeneta",goodgenvalue,{"GenPart_eta","GenPart_postFSRLepIdx1","GenPart_postFSRLepIdx2","GenPart_eta","GenPart_phi","GenPart_status","GenPart_pdgId"}).Define("goodgenphi",goodgenvalue,{"GenPart_phi","GenPart_postFSRLepIdx1","GenPart_postFSRLepIdx2","GenPart_eta","GenPart_phi","GenPart_status","GenPart_pdgId"}).Define("goodgenidx",goodgenidx,{"GenPart_pt","GenPart_postFSRLepIdx1","GenPart_postFSRLepIdx2","GenPart_eta","GenPart_phi","GenPart_status","GenPart_pdgId"}).Define("goodmuonbool",goodmuonboolidip,{"goodgeneta", "goodgenphi", "Muon_eta", "Muon_phi", "Muon_isStandalone", "Muon_isGlobal", "Muon_mediumId", "Muon_dxybs", "Muon_pt", "Muon_standalonePt", "Muon_standaloneEta", "Muon_standalonePhi"});
	d1=d1.Define("goodtrackpt",goodtrack,{"goodgenpt","Track_eta","Track_phi","goodgeneta","goodgenphi","Track_normChiSq","Track_trackOriginalAlgo","Track_qualityMask"}).Define("goodtracketa",goodtrack,{"goodgeneta","Track_eta","Track_phi","goodgeneta","goodgenphi","Track_normChiSq","Track_trackOriginalAlgo","Track_qualityMask"}).Define("goodtrackphi",goodtrack,{"goodgenphi","Track_eta","Track_phi","goodgeneta","goodgenphi","Track_normChiSq","Track_trackOriginalAlgo","Track_qualityMask"}).Define("goodtrackrealeta",goodtrackreal,{"Track_eta","Track_eta","Track_phi","goodgeneta","goodgenphi","Track_normChiSq","Track_trackOriginalAlgo","Track_qualityMask"}).Define("goodtrackrealphi",goodtrackreal,{"Track_phi","Track_eta","Track_phi","goodgeneta","goodgenphi","Track_normChiSq","Track_trackOriginalAlgo","Track_qualityMask"});
	d1=d1.Define("goodtrack",isgoodtrack,{"goodtrackrealeta","goodtrackrealphi","Muon_standaloneEta","Muon_standalonePhi","Muon_isStandalone","Muon_standalonePt"});
	d1=d1.Define("goodstandpt",goodmuon,{"goodgenpt","goodgeneta","goodgenphi","Muon_standaloneEta","Muon_standalonePhi","Muon_isStandalone","Muon_standalonePt"}).Define("goodstandeta",goodmuon,{"goodgeneta","goodgeneta","goodgenphi","Muon_standaloneEta","Muon_standalonePhi","Muon_isStandalone","Muon_standalonePt"}).Define("goodstandphi",goodmuon,{"goodgenphi","goodgeneta","goodgenphi","Muon_standaloneEta","Muon_standalonePhi","Muon_isStandalone","Muon_standalonePt"}).Define("goodmuonidx",goodmuonidx,{"goodgeneta","goodgenphi","Muon_standaloneEta","Muon_standalonePhi","Muon_isStandalone","Muon_standalonePt"});
	d1=d1.Define("goodstand",goodstand,{"goodmuonidx","Muon_isGlobal","Muon_pt","Muon_standalonePt"});
	d1=d1.Define("goodglobalpt",goodmuontnp,{"goodgenpt","goodgeneta","goodgenphi","Muon_eta","Muon_phi","Muon_isGlobal","Muon_pt","Muon_standalonePt","Muon_standaloneEta","Muon_standalonePhi","Muon_isStandalone"}).Define("goodglobaleta",goodmuontnp,{"goodgeneta","goodgeneta","goodgenphi","Muon_eta","Muon_phi","Muon_isGlobal","Muon_pt","Muon_standalonePt","Muon_standaloneEta","Muon_standalonePhi","Muon_isStandalone"}).Define("goodglobalphi",goodmuontnp,{"goodgenphi","goodgeneta","goodgenphi","Muon_eta","Muon_phi","Muon_isGlobal","Muon_pt","Muon_standalonePt","Muon_standaloneEta","Muon_standalonePhi","Muon_isStandalone"}).Define("goodglobalidx",goodmuontnpidx,{"goodgeneta","goodgenphi","Muon_eta","Muon_phi","Muon_isGlobal","Muon_pt","Muon_standalonePt","Muon_standaloneEta","Muon_standalonePhi","Muon_isStandalone"});
	d1=d1.Define("goodglobal",goodglobal,{"goodglobalidx","Muon_mediumId","Muon_dxybs"});
	auto histo1 = d1.Histo2D({"histo1", "", 48, -2.4, 2.4, 9, 25., 60.}, "goodgeneta", "goodgenpt", "goodmuonbool");
	auto histo2 = d1.Histo2D({"histo2", "", 48, -2.4, 2.4, 9, 25., 60.}, "goodgeneta", "goodgenpt");
	auto histo3 = d1.Histo2D({"histo3", "", 48, -2.4, 2.4, 9, 25., 60.}, "goodtracketa", "goodtrackpt", "goodtrack");
	auto histo4 = d1.Histo2D({"histo4", "", 48, -2.4, 2.4, 9, 25., 60.}, "goodtracketa", "goodtrackpt");
	auto histo5 = d1.Histo2D({"histo5", "", 48, -2.4, 2.4, 9, 25., 60.}, "goodstandeta", "goodstandpt", "goodstand");
	auto histo6 = d1.Histo2D({"histo6", "", 48, -2.4, 2.4, 9, 25., 60.}, "goodstandeta", "goodstandpt");
	auto histo7 = d1.Histo2D({"histo7", "", 48, -2.4, 2.4, 9, 25., 60.}, "goodglobaleta", "goodglobalpt", "goodglobal");
	auto histo8 = d1.Histo2D({"histo8", "", 48, -2.4, 2.4, 9, 25., 60.}, "goodglobaleta", "goodglobalpt");
	TFile* output=new TFile("outputidip.root","RECREATE");
	output->cd();
	TH2D* Histo1=(TH2D*)histo1.GetPtr()->Clone(), *Histo2=(TH2D*)histo2.GetPtr()->Clone(), *Histo3=(TH2D*)histo3.GetPtr()->Clone(), *Histo4=(TH2D*)histo4.GetPtr()->Clone(), *Histo5=(TH2D*)histo5.GetPtr()->Clone(), *Histo6=(TH2D*)histo6.GetPtr()->Clone(), *Histo7=(TH2D*)histo7.GetPtr()->Clone(), *Histo8=(TH2D*)histo8.GetPtr()->Clone();
	Histo1->Divide(Histo2);
	Histo3->Divide(Histo4);
	Histo5->Divide(Histo6);
	Histo7->Divide(Histo8);
	TH2D* product=(TH2D*)Histo3->Clone();
	product->Multiply(Histo5);
	product->Multiply(Histo7);
	TCanvas *c1=new TCanvas();
	c1->cd();
	c1->Draw();
	Histo1->SetTitle("P(global&&medium ID&&|d_{xybs}|<0.05 cm|gen)");
	Histo1->GetXaxis()->SetTitle("GenMuon #eta");
	Histo1->GetYaxis()->SetTitle("GenMuon p_{T} [GeV]");
	Histo1->Draw("colz");
	TCanvas *c3=new TCanvas();
	c3->cd();
	c3->Draw();
	product->SetTitle("P(medium ID&&abs(d_{xybs})<0.05|global)*P(global|standalone)*P(standalone|tracker seeded)");
	product->GetXaxis()->SetTitle("GenMuon #eta");
	product->GetYaxis()->SetTitle("GenMuon p_{T} [GeV]");
	product->Draw("colz");
	TH2D* Histo9=(TH2D*)Histo1->Clone();
	Histo9->Divide(product);
	TCanvas *c7=new TCanvas();
	c7->cd();
	c7->Draw();
	Histo9->SetTitle("MC truth vs tag and probe ratio");
	Histo9->GetXaxis()->SetTitle("GenMuon #eta");
	Histo9->GetYaxis()->SetTitle("GenMuon p_{T} [GeV]");
	Histo9->Draw("colz");
	output->Write();
//	output->Close();
}

int main(int argc, char **argv) {
	char** abcd;
	TApplication theApp("App",0,abcd);
	idip(argv[1]);
	theApp.Run();
	return 0;
}
