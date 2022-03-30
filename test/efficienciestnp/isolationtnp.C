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

void isolationtnp(std::vector<std::string> names) {
	gStyle->SetOptStat(0);
	RDataFrame df("demo/Events",names);
	auto d1=df.Define("goodgenpt",goodgenvalue,{"GenPart_pt","GenPart_postFSRLepIdx1","GenPart_postFSRLepIdx2","GenPart_eta","GenPart_phi","GenPart_status","GenPart_pdgId"}).Define("goodgeneta",goodgenvalue,{"GenPart_eta","GenPart_postFSRLepIdx1","GenPart_postFSRLepIdx2","GenPart_eta","GenPart_phi","GenPart_status","GenPart_pdgId"}).Define("goodgenphi",goodgenvalue,{"GenPart_phi","GenPart_postFSRLepIdx1","GenPart_postFSRLepIdx2","GenPart_eta","GenPart_phi","GenPart_status","GenPart_pdgId"}).Define("goodgenidx",goodgenidx,{"GenPart_pt","GenPart_postFSRLepIdx1","GenPart_postFSRLepIdx2","GenPart_eta","GenPart_phi","GenPart_status","GenPart_pdgId"}).Define("goodmuonbool",goodmuonboolisolation,{"goodgenidx","Muon_genPartIdx","Muon_eta","Muon_phi","Muon_standeta","Muon_standphi","Muon_isStandalone","Muon_isGlobal","Muon_mediumId","Muon_dxyBS","Muon_triggered","Muon_pfRelIso04_all"}).Define("matchedtrackidx",matchedtrackidx,{"goodgeneta","goodgenphi","goodgenidx","Track_eta","Track_phi","Track_chi2","Track_originalAlgo"}).Define("matchedstaidx",matchedstaidx,{"goodgeneta","goodgenphi","goodgenidx","Muon_standeta","Muon_standphi","Muon_isStandalone"});
	d1=d1.Define("zero","0").Define("one","1").Define("two","2").Define("three","3").Define("four","4").Define("tnppairtag",maketnppairstrack,{"Muon_pt","Muon_eta","Muon_phi","Muon_mass","Muon_standeta","Muon_standphi","Muon_isStandalone","Muon_isGlobal","Muon_mediumId", "Muon_dxyBS","Muon_triggered","Muon_pfRelIso04_all","Track_pt","Track_eta","Track_phi","Track_chi2","Track_originalAlgo","zero"}).Define("tnppairprobe",maketnppairstrack,{"Muon_pt","Muon_eta","Muon_phi","Muon_mass","Muon_standeta","Muon_standphi","Muon_isStandalone","Muon_isGlobal","Muon_mediumId", "Muon_dxyBS","Muon_triggered","Muon_pfRelIso04_all","Track_pt","Track_eta","Track_phi","Track_chi2","Track_originalAlgo","one"}).Define("tnppairflag",maketnppairstrack,{"Muon_pt","Muon_eta","Muon_phi","Muon_mass","Muon_standeta","Muon_standphi","Muon_isStandalone","Muon_isGlobal","Muon_mediumId", "Muon_dxyBS","Muon_triggered","Muon_pfRelIso04_all","Track_pt","Track_eta","Track_phi","Track_chi2","Track_originalAlgo","two"});
	d1=d1.Define("goodtnppairtag",goodtnppairs,{"tnppairtag","tnppairprobe","tnppairflag","Muon_genPartIdx","matchedtrackidx","goodgenidx","zero"}).Define("goodtnppairprobe",goodtnppairs,{"tnppairtag","tnppairprobe","tnppairflag","Muon_genPartIdx","matchedtrackidx","goodgenidx","one"});
	d1=d1.Define("goodtrackpt",goodtrackgen,{"goodtnppairtag","goodtnppairprobe","matchedtrackidx","goodgenidx","goodgenpt"}).Define("goodtracketa",goodtrackgen,{"goodtnppairtag","goodtnppairprobe","matchedtrackidx","goodgenidx","goodgeneta"}).Define("goodtrackphi",goodtrackgen,{"goodtnppairtag","goodtnppairprobe","matchedtrackidx","goodgenidx","goodgenphi"}).Define("goodtrackrealpt",goodtrack,{"goodtnppairtag","goodtnppairprobe","Track_pt"}).Define("goodtrackrealeta",goodtrack,{"goodtnppairtag","goodtnppairprobe","Track_eta"}).Define("goodtrackrealphi",goodtrack,{"goodtnppairtag","goodtnppairprobe","Track_phi"});
	d1=d1.Define("goodtrack",isgoodtrack,{"goodtrackrealeta","goodtrackrealphi","Muon_standeta","Muon_standphi","Muon_isStandalone"});
	d1=d1.Define("tnppairtagreco",maketnppairs,{"Muon_pt","Muon_eta","Muon_phi","Muon_mass","Muon_standeta","Muon_standphi","Muon_isStandalone","Muon_isGlobal","Muon_mediumId","Muon_dxyBS","Muon_triggered","Muon_pfRelIso04_all","zero"}).Define("tnppairprobereco",maketnppairs,{"Muon_pt","Muon_eta","Muon_phi","Muon_mass","Muon_standeta","Muon_standphi","Muon_isStandalone","Muon_isGlobal","Muon_mediumId","Muon_dxyBS","Muon_triggered","Muon_pfRelIso04_all","one"}).Define("tnppairflagreco",maketnppairs,{"Muon_pt","Muon_eta","Muon_phi","Muon_mass","Muon_standeta","Muon_standphi","Muon_isStandalone","Muon_isGlobal","Muon_mediumId","Muon_dxyBS","Muon_triggered","Muon_pfRelIso04_all","two"});
	d1=d1.Define("goodtnppairtagreco",goodtnppairs,{"tnppairtagreco","tnppairprobereco","tnppairflagreco","Muon_genPartIdx","matchedstaidx","goodgenidx","zero"}).Define("goodtnppairprobereco",goodtnppairs,{"tnppairtagreco","tnppairprobereco","tnppairflagreco","Muon_genPartIdx","matchedstaidx","goodgenidx","one"}).Define("goodtnppairflagreco",goodtnppairs,{"tnppairtagreco","tnppairprobereco","tnppairflagreco","Muon_genPartIdx","matchedstaidx","goodgenidx","two"});
	d1=d1.Define("goodstandpt",goodtrackgen,{"goodtnppairtagreco","goodtnppairprobereco","matchedstaidx","goodgenidx","goodgenpt"}).Define("goodstandeta",goodtrackgen,{"goodtnppairtagreco","goodtnppairprobereco","matchedstaidx","goodgenidx","goodgeneta"}).Define("goodstandphi",goodtrackgen,{"goodtnppairtagreco","goodtnppairprobereco","matchedstaidx","goodgenidx","goodgenphi"});
	d1=d1.Define("isglobal",isglobal,{"goodtnppairflagreco","one"});
	d1=d1.Define("goodtnppairtagminiaod",goodtnppairs,{"tnppairtagreco","tnppairprobereco","tnppairflagreco","Muon_genPartIdx","Muon_genPartIdx","goodgenidx","zero"}).Define("goodtnppairprobeminiaod",goodtnppairs,{"tnppairtagreco","tnppairprobereco","tnppairflagreco","Muon_genPartIdx","Muon_genPartIdx","goodgenidx","one"}).Define("goodtnppairflagminiaod",goodtnppairs,{"tnppairtagreco","tnppairprobereco","tnppairflagreco","Muon_genPartIdx","Muon_genPartIdx","goodgenidx","two"});
	d1=d1.Define("isglobalminiaod",isglobal,{"goodtnppairflagminiaod","one"});
	d1=d1.Define("idipdenominatortag","goodtnppairtagminiaod[isglobalminiaod]").Define("idipdenominatorprobe","goodtnppairprobeminiaod[isglobalminiaod]").Define("idipdenominatorflag","goodtnppairflagminiaod[isglobalminiaod]");
	d1=d1.Define("isidip",isglobal,{"idipdenominatorflag","two"});
	d1=d1.Define("goodglobalpt",goodtrackgen,{"idipdenominatortag","idipdenominatorprobe","Muon_genPartIdx","goodgenidx","goodgenpt"}).Define("goodglobaleta",goodtrackgen,{"idipdenominatortag","idipdenominatorprobe","Muon_genPartIdx","goodgenidx","goodgeneta"});
	d1=d1.Define("triggerdenominatortag","idipdenominatortag[isidip]").Define("triggerdenominatorprobe","idipdenominatorprobe[isidip]").Define("triggerdenominatorflag","idipdenominatorflag[isidip]");
	d1=d1.Define("istrigger",isglobal,{"triggerdenominatorflag","three"});
	d1=d1.Define("goodidippt",goodtrackgen,{"triggerdenominatortag","triggerdenominatorprobe","Muon_genPartIdx","goodgenidx","goodgenpt"}).Define("goodidipeta",goodtrackgen,{"triggerdenominatortag","triggerdenominatorprobe","Muon_genPartIdx","goodgenidx","goodgeneta"});
	d1=d1.Define("isolationdenominatortag","triggerdenominatortag[istrigger]").Define("isolationdenominatorprobe","triggerdenominatorprobe[istrigger]").Define("isolationdenominatorflag","triggerdenominatorflag[istrigger]");
	d1=d1.Define("isisolation",isglobal,{"isolationdenominatorflag","four"});
	d1=d1.Define("goodtriggerpt",goodtrackgen,{"isolationdenominatortag","isolationdenominatorprobe","Muon_genPartIdx","goodgenidx","goodgenpt"}).Define("goodtriggereta",goodtrackgen,{"isolationdenominatortag","isolationdenominatorprobe","Muon_genPartIdx","goodgenidx","goodgeneta"});
	d1=d1.Define("invariantmass",invariantmasses,{"isolationdenominatortag","isolationdenominatorprobe","Muon_pt","Muon_eta","Muon_phi","Muon_pt","Muon_eta","Muon_phi","Muon_mass"}).Define("passinginvariantmasses","invariantmass*(isisolation)").Define("failinginvariantmasses","invariantmass*(isisolation==false)");
	d1=d1.Define("absgoodgeneta","abs(goodgeneta)").Define("absgoodtracketa","abs(goodtracketa)").Define("absgoodstandeta","abs(goodstandeta)").Define("absgoodglobaleta","abs(goodglobaleta)").Define("absgoodidipeta","abs(goodidipeta)").Define("absgoodtriggereta","abs(goodtriggereta)");
	unsigned int neta=24, npt=9, nim=70;
	float mineta=0, minpt=25., minim=50.;
	float maxeta=2.4, maxpt=60., maxim=120.;
	auto histo1 = d1.Histo2D({"histo1", "", neta, mineta, maxeta, npt, minpt, maxpt}, "absgoodgeneta", "goodgenpt", "goodmuonbool");
	auto histo2 = d1.Histo2D({"histo2", "", neta, mineta, maxeta, npt, minpt, maxpt}, "absgoodgeneta", "goodgenpt");
	auto histo3 = d1.Histo2D({"histo3", "", neta, mineta, maxeta, npt, minpt, maxpt}, "absgoodtracketa", "goodtrackpt", "goodtrack");
	auto histo4 = d1.Histo2D({"histo4", "", neta, mineta, maxeta, npt, minpt, maxpt}, "absgoodtracketa", "goodtrackpt");
	auto histo5 = d1.Histo2D({"histo5", "", neta, mineta, maxeta, npt, minpt, maxpt}, "absgoodstandeta", "goodstandpt", "isglobal");
	auto histo6 = d1.Histo2D({"histo6", "", neta, mineta, maxeta, npt, minpt, maxpt}, "absgoodstandeta", "goodstandpt");
	auto histo7 = d1.Histo2D({"histo7", "", neta, mineta, maxeta, npt, minpt, maxpt}, "absgoodglobaleta", "goodglobalpt", "isidip");
	auto histo8 = d1.Histo2D({"histo8", "", neta, mineta, maxeta, npt, minpt, maxpt}, "absgoodglobaleta", "goodglobalpt");
	auto histo9 = d1.Histo2D({"histo9", "", neta, mineta, maxeta, npt, minpt, maxpt}, "absgoodidipeta", "goodidippt", "istrigger");
	auto histo10 = d1.Histo2D({"histo10", "", neta, mineta, maxeta, npt, minpt, maxpt}, "absgoodidipeta", "goodidippt");
	auto histo11 = d1.Histo2D({"histo11", "", neta, mineta, maxeta, npt, minpt, maxpt}, "absgoodtriggereta", "goodtriggerpt", "isisolation");
	auto histo12 = d1.Histo2D({"histo12", "", neta, mineta, maxeta, npt, minpt, maxpt}, "absgoodtriggereta", "goodtriggerpt");
	auto histo13 = d1.Histo3D({"passinginvariantmasses", "", neta, mineta, maxeta, npt, minpt, maxpt, nim, minim, maxim}, "absgoodtriggereta","goodtriggerpt","passinginvariantmasses");
	auto histo14 = d1.Histo3D({"failinginvariantmasses", "", neta, mineta, maxeta, npt, minpt, maxpt, nim, minim, maxim}, "absgoodtriggereta","goodtriggerpt","failinginvariantmasses");
	TH2D* Histo1=(TH2D*)histo1.GetPtr()->Clone(), *Histo2=(TH2D*)histo2.GetPtr()->Clone(), *Histo3=(TH2D*)histo3.GetPtr()->Clone(), *Histo4=(TH2D*)histo4.GetPtr()->Clone(), *Histo5=(TH2D*)histo5.GetPtr()->Clone(), *Histo6=(TH2D*)histo6.GetPtr()->Clone(), *Histo7=(TH2D*)histo7.GetPtr()->Clone(), *Histo8=(TH2D*)histo8.GetPtr()->Clone(), *Histo9=(TH2D*)histo9.GetPtr()->Clone(), *Histo10=(TH2D*)histo10.GetPtr()->Clone(), *Histo11=(TH2D*)histo11.GetPtr()->Clone(), *Histo12=(TH2D*)histo12.GetPtr()->Clone();
	TH3D* passinginvariantmasses=(TH3D*)histo13.GetPtr()->Clone(), *failinginvariantmasses=(TH3D*)histo14.GetPtr()->Clone();
	Histo1->Divide(Histo2);
	Histo3->Divide(Histo4);
	Histo5->Divide(Histo6);
	Histo7->Divide(Histo8);
	Histo9->Divide(Histo10);
	Histo11->Divide(Histo12);
	TH2D* product=(TH2D*)Histo3->Clone();
	product->Multiply(Histo5);
	product->Multiply(Histo7);
	product->Multiply(Histo9);
	product->Multiply(Histo11);
	TCanvas *c1=new TCanvas();
	c1->cd();
	c1->Draw();
	Histo1->SetTitle("P(passesglobal&&IDIP&&trigger match&&pfRelIso<0.15|gen)");
	Histo1->GetXaxis()->SetTitle("GenMuon |#eta|");
	Histo1->GetYaxis()->SetTitle("GenMuon p_{T} [GeV]");
	Histo1->Draw("colz");
	TCanvas *c3=new TCanvas();
	c3->cd();
	c3->Draw();
	product->SetTitle("P(pfRelIso<0.15|trigger)*P(trigger|idip)*P(idip|passesglobal)*P(passesglobal|standalone)*P(standalone|tracker seeded)");
	product->GetXaxis()->SetTitle("GenMuon |#eta|");
	product->GetYaxis()->SetTitle("GenMuon p_{T} [GeV]");
	product->Draw("colz");
	TH2D* Histo13=(TH2D*)Histo1->Clone();
	Histo13->Divide(product);
	TCanvas *c7=new TCanvas();
	c7->cd();
	c7->Draw();
	Histo13->SetTitle("MC truth vs tag and probe ratio");
	Histo13->GetXaxis()->SetTitle("GenMuon |#eta|");
	Histo13->GetYaxis()->SetTitle("GenMuon p_{T} [GeV]");
	Histo13->Draw("colz");
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
	std::vector<std::string> names;
	for (int i=1; i!=argc; i++) {
		names.push_back(std::string(argv[i]));
	}
	isolationtnp(names);
	theApp.Run();
	return 0;
}
