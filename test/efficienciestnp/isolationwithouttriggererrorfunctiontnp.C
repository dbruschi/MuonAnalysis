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
#include "TF1.h"
#include <iostream>

using namespace ROOT;

void isolationtnp(std::vector<std::string> names, const char* name2) {
	gStyle->SetOptStat(0);
	RDataFrame df("demo/Events",names);
	auto d1=df.Define("goodgenpt",goodgenvalue,{"GenPart_pt","GenPart_postFSRLepIdx1","GenPart_postFSRLepIdx2","GenPart_eta","GenPart_phi","GenPart_status","GenPart_pdgId"}).Define("goodgeneta",goodgenvalue,{"GenPart_eta","GenPart_postFSRLepIdx1","GenPart_postFSRLepIdx2","GenPart_eta","GenPart_phi","GenPart_status","GenPart_pdgId"}).Define("goodgenphi",goodgenvalue,{"GenPart_phi","GenPart_postFSRLepIdx1","GenPart_postFSRLepIdx2","GenPart_eta","GenPart_phi","GenPart_status","GenPart_pdgId"}).Define("goodgenidx",goodgenidx,{"GenPart_pt","GenPart_postFSRLepIdx1","GenPart_postFSRLepIdx2","GenPart_eta","GenPart_phi","GenPart_status","GenPart_pdgId"}).Define("goodmuonbool",goodmuonboolisolationnotrig,{"goodgenidx","Muon_genPartIdx","Muon_eta","Muon_phi","Muon_standeta","Muon_standphi","Muon_isStandalone","Muon_isGlobal","Muon_mediumId","Muon_dxyBS","Muon_triggered","Muon_pfRelIso04_all"}).Define("matchedtrackidx",matchedtrackidx,{"goodgeneta","goodgenphi","goodgenidx","Track_eta","Track_phi","Track_chi2","Track_originalAlgo"}).Define("matchedstaidx",matchedstaidx,{"goodgeneta","goodgenphi","goodgenidx","Muon_standeta","Muon_standphi","Muon_isStandalone"});
	d1=d1.Define("zero","0").Define("one","1").Define("two","2").Define("three","3").Define("four","4").Define("tnppairtag",maketnppairstrack,{"Muon_pt","Muon_eta","Muon_phi","Muon_mass","Muon_standeta","Muon_standphi","Muon_isStandalone","Muon_isGlobal","Muon_mediumId", "Muon_dxyBS","Muon_triggered","Muon_pfRelIso04_all","Track_pt","Track_eta","Track_phi","Track_chi2","Track_originalAlgo","zero"}).Define("tnppairprobe",maketnppairstrack,{"Muon_pt","Muon_eta","Muon_phi","Muon_mass","Muon_standeta","Muon_standphi","Muon_isStandalone","Muon_isGlobal","Muon_mediumId", "Muon_dxyBS","Muon_triggered","Muon_pfRelIso04_all","Track_pt","Track_eta","Track_phi","Track_chi2","Track_originalAlgo","one"}).Define("tnppairflag",maketnppairstrack,{"Muon_pt","Muon_eta","Muon_phi","Muon_mass","Muon_standeta","Muon_standphi","Muon_isStandalone","Muon_isGlobal","Muon_mediumId", "Muon_dxyBS","Muon_triggered","Muon_pfRelIso04_all","Track_pt","Track_eta","Track_phi","Track_chi2","Track_originalAlgo","two"});
	d1=d1.Define("goodtnppairtag",goodtnppairs,{"tnppairtag","tnppairprobe","tnppairflag","Muon_genPartIdx","matchedtrackidx","goodgenidx","zero"}).Define("goodtnppairprobe",goodtnppairs,{"tnppairtag","tnppairprobe","tnppairflag","Muon_genPartIdx","matchedtrackidx","goodgenidx","one"});
	d1=d1.Define("goodtrackpt",goodtrackgen,{"goodtnppairtag","goodtnppairprobe","matchedtrackidx","goodgenidx","goodgenpt"}).Define("goodtracketa",goodtrackgen,{"goodtnppairtag","goodtnppairprobe","matchedtrackidx","goodgenidx","goodgeneta"}).Define("goodtrackphi",goodtrackgen,{"goodtnppairtag","goodtnppairprobe","matchedtrackidx","goodgenidx","goodgenphi"}).Define("goodtrackrealpt",goodtrack,{"goodtnppairtag","goodtnppairprobe","Track_pt"}).Define("goodtrackrealeta",goodtrack,{"goodtnppairtag","goodtnppairprobe","Track_eta"}).Define("goodtrackrealphi",goodtrack,{"goodtnppairtag","goodtnppairprobe","Track_phi"});
	d1=d1.Define("goodtrack",isgoodtrack,{"goodtrackrealeta","goodtrackrealphi","Muon_standeta","Muon_standphi","Muon_isStandalone"});
	d1=d1.Define("absgoodgeneta","abs(goodgeneta)").Define("absgoodtracketa","abs(goodtracketa)");
	unsigned int neta=24, npt=9, nim=70;
	float mineta=0, minpt=25., minim=50.;
	float maxeta=2.4, maxpt=60., maxim=120.;
	TFile* errorfunction=new TFile(name2);
	int numeta=(int)((TH1D*)errorfunction->Get("etabins"))->GetBinContent(1);
	float maximumeta=((TH1D*)errorfunction->Get("maxetabin"))->GetBinContent(1);
	float minimumeta=((TH1D*)errorfunction->Get("minetabin"))->GetBinContent(1);
	std::vector<TF1*> functions;
	for (unsigned int i=0; i!=numeta; i++) {
		std::string errorfunctionname("errorfunction");
		errorfunctionname+=std::to_string(i+1);
		functions.push_back((TF1*)errorfunction->Get(errorfunctionname.c_str()));
	}
	auto efficiencies = [&numeta,&maximumeta,&minimumeta,&functions](RVec<float> &pt, RVec<float> &eta){
		RVec<float> v(pt.size());
		for (auto i=0U; i!=pt.size(); i++) {
			if ((eta[i]>maximumeta)||(eta[i]<minimumeta)) v[i]=0.;
			for (unsigned int j=1; j<=numeta; j++) {
				float binetamin=(maximumeta-minimumeta)/numeta*(j-1)+minimumeta, binetamax=(maximumeta-minimumeta)/numeta*j+minimumeta;
				if ((eta[i]>binetamin)&&(eta[i]<binetamax)) v[i]=functions[j-1]->Eval(pt[i]);
			}
		}
		return v;
	};
	d1=d1.Define("goodtrackefc",efficiencies,{"goodtrackpt","absgoodtracketa"});
	auto histo1 = d1.Histo2D({"histo1", "", neta, mineta, maxeta, npt, minpt, maxpt}, "absgoodgeneta", "goodgenpt", "goodmuonbool");
	auto histo2 = d1.Histo2D({"histo2", "", neta, mineta, maxeta, npt, minpt, maxpt}, "absgoodgeneta", "goodgenpt");
	auto histo3 = d1.Histo2D({"histo3", "", neta, mineta, maxeta, npt, minpt, maxpt}, "absgoodtracketa", "goodtrackpt", "goodtrackefc");
	auto histo4 = d1.Histo2D({"histo4", "", neta, mineta, maxeta, npt, minpt, maxpt}, "absgoodtracketa", "goodtrackpt");
/*	auto histo5 = d1.Histo2D({"histo5", "", neta, mineta, maxeta, npt, minpt, maxpt}, "absgoodstandeta", "goodstandpt", "isglobal");
	auto histo6 = d1.Histo2D({"histo6", "", neta, mineta, maxeta, npt, minpt, maxpt}, "absgoodstandeta", "goodstandpt");
	auto histo7 = d1.Histo2D({"histo7", "", neta, mineta, maxeta, npt, minpt, maxpt}, "absgoodglobaleta", "goodglobalpt", "isidip");
	auto histo8 = d1.Histo2D({"histo8", "", neta, mineta, maxeta, npt, minpt, maxpt}, "absgoodglobaleta", "goodglobalpt");
//	auto histo9 = d1.Histo2D({"histo9", "", neta, mineta, maxeta, npt, minpt, maxpt}, "absgoodidipeta", "goodidippt", "istrigger");
//	auto histo10 = d1.Histo2D({"histo10", "", neta, mineta, maxeta, npt, minpt, maxpt}, "absgoodidipeta", "goodidippt");
	auto histo11 = d1.Histo2D({"histo11", "", neta, mineta, maxeta, npt, minpt, maxpt}, "absgoodtriggereta", "goodtriggerpt", "isisolation");
	auto histo12 = d1.Histo2D({"histo12", "", neta, mineta, maxeta, npt, minpt, maxpt}, "absgoodtriggereta", "goodtriggerpt");
	auto histo13 = d1.Histo3D({"passinginvariantmasses", "", neta, mineta, maxeta, npt, minpt, maxpt, nim, minim, maxim}, "absgoodtriggereta","goodtriggerpt","passinginvariantmasses");
	auto histo14 = d1.Histo3D({"failinginvariantmasses", "", neta, mineta, maxeta, npt, minpt, maxpt, nim, minim, maxim}, "absgoodtriggereta","goodtriggerpt","failinginvariantmasses");*/
	TFile* histos=new TFile("histos.root","RECREATE");
	histos->cd();
	TH2D* Histo1=(TH2D*)histo1.GetPtr()->Clone(), *Histo2=(TH2D*)histo2.GetPtr()->Clone(), *Histo3=(TH2D*)histo3.GetPtr()->Clone(), *Histo4=(TH2D*)histo4.GetPtr()->Clone();
	Histo1->Divide(Histo2);
	Histo3->Divide(Histo4);
	histos->Write();
	histos->Close();
}

int main(int argc, char **argv) {
	char** abcd;
	TApplication theApp("App",0,abcd);
	std::vector<std::string> names;
	for (int i=1; i!=(argc-1); i++) {
		names.push_back(std::string(argv[i]));
	}
	isolationtnp(names,argv[argc-1]);
	theApp.Run();
	return 0;
}
