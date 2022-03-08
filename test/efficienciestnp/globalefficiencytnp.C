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

void globalefficiencytnp(const char* name) {
	gStyle->SetOptStat(0);
	RDataFrame df("demo/Events",name);
	auto d1=df.Define("goodgenpt",goodgenvalue,{"GenPart_pt","GenPart_postFSRLepIdx1","GenPart_postFSRLepIdx2","GenPart_eta","GenPart_phi","GenPart_status","GenPart_pdgId"}).Define("goodgeneta",goodgenvalue,{"GenPart_eta","GenPart_postFSRLepIdx1","GenPart_postFSRLepIdx2","GenPart_eta","GenPart_phi","GenPart_status","GenPart_pdgId"}).Define("goodgenphi",goodgenvalue,{"GenPart_phi","GenPart_postFSRLepIdx1","GenPart_postFSRLepIdx2","GenPart_eta","GenPart_phi","GenPart_status","GenPart_pdgId"}).Define("goodgenidx",goodgenidx,{"GenPart_pt","GenPart_postFSRLepIdx1","GenPart_postFSRLepIdx2","GenPart_eta","GenPart_phi","GenPart_status","GenPart_pdgId"}).Define("goodmuonbool",goodmuonboolglobal,{"goodgeneta","goodgenphi","goodgenidx","Muon_genPartIdx","Muon_isGlobal","Muon_standeta","Muon_standphi","Muon_eta","Muon_phi","Muon_isStandalone"}).Define("matchedtrackidx",matchedtrackidx,{"goodgeneta","goodgenphi","goodgenidx","Track_eta","Track_phi","Track_chi2","Track_originalAlgo"}).Define("matchedstaidx",matchedstaidx,{"goodgeneta","goodgenphi","goodgenidx","Muon_standeta","Muon_standphi","Muon_isStandalone"});
	d1=d1.Define("zero","0").Define("one","1").Define("two","2").Define("tnppairtag",maketnppairstrack,{"Muon_pt","Muon_eta","Muon_phi","Muon_mass","Muon_standeta","Muon_standphi","Muon_isStandalone","Muon_isGlobal","Muon_mediumId", "Muon_dxyBS","Muon_triggered","Muon_pfRelIso04_all","Track_pt","Track_eta","Track_phi","Track_chi2","Track_originalAlgo","zero"}).Define("tnppairprobe",maketnppairstrack,{"Muon_pt","Muon_eta","Muon_phi","Muon_mass","Muon_standeta","Muon_standphi","Muon_isStandalone","Muon_isGlobal","Muon_mediumId", "Muon_dxyBS","Muon_triggered","Muon_pfRelIso04_all","Track_pt","Track_eta","Track_phi","Track_chi2","Track_originalAlgo","one"}).Define("tnppairflag",maketnppairstrack,{"Muon_pt","Muon_eta","Muon_phi","Muon_mass","Muon_standeta","Muon_standphi","Muon_isStandalone","Muon_isGlobal","Muon_mediumId", "Muon_dxyBS","Muon_triggered","Muon_pfRelIso04_all","Track_pt","Track_eta","Track_phi","Track_chi2","Track_originalAlgo","two"});
	d1=d1.Define("goodtnppairtag",goodtnppairs,{"tnppairtag","tnppairprobe","tnppairflag","Muon_genPartIdx","matchedtrackidx","goodgenidx","zero"}).Define("goodtnppairprobe",goodtnppairs,{"tnppairtag","tnppairprobe","tnppairflag","Muon_genPartIdx","matchedtrackidx","goodgenidx","one"});
	d1=d1.Define("goodtrackpt",goodtrackgen,{"goodtnppairtag","goodtnppairprobe","matchedtrackidx","goodgenidx","goodgenpt"}).Define("goodtracketa",goodtrackgen,{"goodtnppairtag","goodtnppairprobe","matchedtrackidx","goodgenidx","goodgeneta"}).Define("goodtrackphi",goodtrackgen,{"goodtnppairtag","goodtnppairprobe","matchedtrackidx","goodgenidx","goodgenphi"}).Define("goodtrackrealpt",goodtrack,{"goodtnppairtag","goodtnppairprobe","Track_pt"}).Define("goodtrackrealeta",goodtrack,{"goodtnppairtag","goodtnppairprobe","Track_eta"}).Define("goodtrackrealphi",goodtrack,{"goodtnppairtag","goodtnppairprobe","Track_phi"});
	d1=d1.Define("goodtrack",isgoodtrack,{"goodtrackrealeta","goodtrackrealphi","Muon_standeta","Muon_standphi","Muon_isStandalone"});
	d1=d1.Define("tnppairtagreco",maketnppairs,{"Muon_pt","Muon_eta","Muon_phi","Muon_mass","Muon_standeta","Muon_standphi","Muon_isStandalone","Muon_isGlobal","Muon_mediumId","Muon_dxyBS","Muon_triggered","Muon_pfRelIso04_all","zero"}).Define("tnppairprobereco",maketnppairs,{"Muon_pt","Muon_eta","Muon_phi","Muon_mass","Muon_standeta","Muon_standphi","Muon_isStandalone","Muon_isGlobal","Muon_mediumId","Muon_dxyBS","Muon_triggered","Muon_pfRelIso04_all","one"}).Define("tnppairflagreco",maketnppairs,{"Muon_pt","Muon_eta","Muon_phi","Muon_mass","Muon_standeta","Muon_standphi","Muon_isStandalone","Muon_isGlobal","Muon_mediumId","Muon_dxyBS","Muon_triggered","Muon_pfRelIso04_all","two"});
	d1=d1.Define("goodtnppairtagreco",goodtnppairs,{"tnppairtagreco","tnppairprobereco","tnppairflagreco","Muon_genPartIdx","matchedstaidx","goodgenidx","zero"}).Define("goodtnppairprobereco",goodtnppairs,{"tnppairtagreco","tnppairprobereco","tnppairflagreco","Muon_genPartIdx","matchedstaidx","goodgenidx","one"}).Define("goodtnppairflagreco",goodtnppairs,{"tnppairtagreco","tnppairprobereco","tnppairflagreco","Muon_genPartIdx","matchedstaidx","goodgenidx","two"});
	d1=d1.Define("goodstandpt",goodtrackgen,{"goodtnppairtagreco","goodtnppairprobereco","matchedstaidx","goodgenidx","goodgenpt"}).Define("goodstandeta",goodtrackgen,{"goodtnppairtagreco","goodtnppairprobereco","matchedstaidx","goodgenidx","goodgeneta"}).Define("goodstandphi",goodtrackgen,{"goodtnppairtagreco","goodtnppairprobereco","matchedstaidx","goodgenidx","goodgenphi"}).Define("invariantmass",invariantmasses,{"goodtnppairtagreco","goodtnppairprobereco","Muon_pt","Muon_eta","Muon_phi","Muon_standpt","Muon_standeta","Muon_standphi","Muon_mass"});
	d1=d1.Define("isglobal",isglobal,{"goodtnppairflagreco","one"});
	d1=d1.Define("passinginvariantmasses","invariantmass*(isglobal)").Define("failinginvariantmasses","invariantmass*(isglobal==false)");
	d1=d1.Define("absgoodgeneta","abs(goodgeneta)").Define("absgoodtracketa","abs(goodtracketa)").Define("absgoodstandeta","abs(goodstandeta)");
	unsigned int neta=24, npt=9, nim=70;
	float mineta=0, minpt=25., minim=50.;
	float maxeta=2.4, maxpt=60., maxim=120.;
	auto histo1 = d1.Histo2D({"histo1", "", neta, mineta, maxeta, npt, minpt, maxpt}, "absgoodgeneta", "goodgenpt", "goodmuonbool");
	auto histo2 = d1.Histo2D({"histo2", "", neta, mineta, maxeta, npt, minpt, maxpt}, "absgoodgeneta", "goodgenpt");
	auto histo3 = d1.Histo2D({"histo3", "", neta, mineta, maxeta, npt, minpt, maxpt}, "absgoodtracketa", "goodtrackpt", "goodtrack");
	auto histo4 = d1.Histo2D({"histo4", "", neta, mineta, maxeta, npt, minpt, maxpt}, "absgoodtracketa", "goodtrackpt");
	auto histo5 = d1.Histo2D({"histo5", "", neta, mineta, maxeta, npt, minpt, maxpt}, "absgoodstandeta", "goodstandpt", "isglobal");
	auto histo6 = d1.Histo2D({"histo6", "", neta, mineta, maxeta, npt, minpt, maxpt}, "absgoodstandeta", "goodstandpt");
	auto histo7 = d1.Histo3D({"passinginvariantmasses", "", neta, mineta, maxeta, npt, minpt, maxpt, nim, minim, maxim}, "absgoodstandeta","goodstandpt","passinginvariantmasses");
	auto histo8 = d1.Histo3D({"failinginvariantmasses", "", neta, mineta, maxeta, npt, minpt, maxpt, nim, minim, maxim}, "absgoodstandeta","goodstandpt","failinginvariantmasses");
	TFile *file=new TFile("histos.root","RECREATE");
	file->cd();
	TH2D* Histo1=(TH2D*)histo1.GetPtr()->Clone(), *Histo2=(TH2D*)histo2.GetPtr()->Clone(), *Histo3=(TH2D*)histo3.GetPtr()->Clone(), *Histo4=(TH2D*)histo4.GetPtr()->Clone(), *Histo5=(TH2D*)histo5.GetPtr()->Clone(), *Histo6=(TH2D*)histo6.GetPtr()->Clone();
	TH3D* passinginvariantmasses=(TH3D*)histo7.GetPtr()->Clone(), *failinginvariantmasses=(TH3D*)histo8.GetPtr()->Clone();
	Histo1->Divide(Histo2);
	Histo3->Divide(Histo4);
	Histo5->Divide(Histo6);
	TH2D* product=(TH2D*)Histo3->Clone();
	product->Multiply(Histo5);
	TCanvas *c1=new TCanvas();
	c1->cd();
	c1->Draw();
	Histo1->SetTitle("P(global&&#DeltaR_{variables}<0.3&&cleaned|gen)");
//	Histo1->SetTitle("P(global&&gen)/P(standalone&&gen)");
	Histo1->GetXaxis()->SetTitle("GenMuon |#eta|");
	Histo1->GetYaxis()->SetTitle("GenMuon p_{T} [GeV]");
	Histo1->Draw("colz");
	TCanvas *c3=new TCanvas();
	c3->cd();
	c3->Draw();
	product->SetTitle("P(global&&#DeltaR_{variables}<0.3&&cleaned|standalone)*P(standalone|tracker seeded)");
	product->GetXaxis()->SetTitle("GenMuon |#eta|");
	product->GetYaxis()->SetTitle("GenMuon p_{T} [GeV]");
	product->Draw("colz");
	TCanvas *c2=new TCanvas();
	c2->cd();
	c2->Draw();
	Histo3->SetTitle("P(standalone|tracker seeded)");
	Histo3->GetXaxis()->SetTitle("GenMuon |#eta|");
	Histo3->GetYaxis()->SetTitle("GenMuon p_{T} [GeV]");
	Histo3->Draw("colz");
	TCanvas *c4=new TCanvas();
	c4->cd();
	c4->Draw();
	Histo5->SetTitle("P(global&&#DeltaR_{variables}<0.3|standalone)");
	Histo5->GetXaxis()->SetTitle("GenMuon |#eta|");
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
	Histo7->GetXaxis()->SetTitle("GenMuon |#eta|");
	Histo7->GetYaxis()->SetTitle("GenMuon p_{T} [GeV]");
	Histo7->Draw("colz");
	file->Write();
	TFile* output=new TFile("output.root","RECREATE");
	output->cd();
	for (unsigned int i=1; i<=neta; i++) {
		float binetamin=(maxeta-mineta)/neta*(i-1)+mineta, binetamax=(maxeta-mineta)/neta*i+mineta;
		passinginvariantmasses->GetXaxis()->SetRange(i,i);
		failinginvariantmasses->GetXaxis()->SetRange(i,i);
		for (unsigned int j=1; j<=npt; j++) {
			float binptmin=(maxpt-minpt)/npt*(j-1)+minpt, binptmax=(maxpt-minpt)/npt*j+minpt;
			passinginvariantmasses->GetYaxis()->SetRange(j,j);
			failinginvariantmasses->GetYaxis()->SetRange(j,j);
			std::string name("passinginvariantmass");
			name+="_"+std::to_string(i)+"_"+std::to_string(j);
			std::string title("passing invariant mass |#eta|>");
			title+=std::to_string(binetamin)+"&&|#eta|<"+std::to_string(binetamax)+"&&p_{T}>"+std::to_string(binptmin)+"&&p_{T}<"+std::to_string(binptmax);
			TH1D* histo=(TH1D*)passinginvariantmasses->Project3D("z");
			histo->SetName(name.c_str());
			histo->SetTitle(title.c_str());
			histo->SetDirectory(output->GetDirectory(""));
			name="failinginvariantmass";
			name+="_"+std::to_string(i)+"_"+std::to_string(j);
			title="failing invariant mass |#eta|>";
			title+=std::to_string(binetamin)+"&&|#eta|<"+std::to_string(binetamax)+"&&p_{T}>"+std::to_string(binptmin)+"&&p_{T}<"+std::to_string(binptmax);
			histo=(TH1D*)failinginvariantmasses->Project3D("z");
			histo->SetName(name.c_str());
			histo->SetTitle(title.c_str());
			histo->SetDirectory(output->GetDirectory(""));
		}
	}
	output->Write();
	output->Close();
}

int main(int argc, char **argv) {
	char** abcd;
	TApplication theApp("App",0,abcd);
	globalefficiencytnp(argv[1]);
	theApp.Run();
	return 0;
}
