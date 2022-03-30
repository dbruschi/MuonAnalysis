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
			TVector3 v1(cand1.Px(),cand1.Py(),0), v2(cand2.Px(),cand2.Py(),0);
			if (goodmuonbool[i]) v.emplace_back((v1+v2).DeltaPhi(v1));
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
		TVector3 v1(cand1.Px(),cand1.Py(),0), v2(cand2.Px(),cand2.Py(),0);
		v.emplace_back((v1+v2).DeltaPhi(v2));
	}
	return v;
}

RVec<bool> Goodmuonboolnotrig(RVec<int> &goodgenidx, RVec<int> &Muon_genPartIdx, RVec<float> &Muon_eta, RVec<float> &Muon_phi, RVec<float> &Muon_standeta, RVec<float> &Muon_standphi, RVec<bool> &Muon_isStandalone, RVec<bool> &Muon_isGlobal, RVec<bool> &Muon_mediumId, RVec<float> &Muon_dxyBS, RVec<bool> &Muon_triggered, RVec<float> &Muon_pfRelIso04_all) {
	RVec<bool> v;
	for (auto i=0U; i<goodgenidx.size(); i++) {
		int idx=-1;
		for (auto j=0U; j<Muon_genPartIdx.size(); ++j) {
			if (Muon_genPartIdx[j]==goodgenidx[i]) {
				TLorentzVector cand1, cand2;
				cand1.SetPtEtaPhiM(3.,Muon_eta[j],Muon_phi[j],0.);
				cand2.SetPtEtaPhiM(3.,Muon_standeta[j],Muon_standphi[j],0.);
				if (cand1.DeltaR(cand2)<0.3) idx=j;
			}
		}
		if ((idx>-1)&&(Muon_isGlobal[idx])&&(Muon_mediumId[idx])&&(fabs(Muon_dxyBS[idx])<0.05)) {
			if (cleaner(idx,Muon_standeta,Muon_standphi,Muon_isStandalone)) v.emplace_back(1);
			else v.emplace_back(0);
		}
		else v.emplace_back(0); 
	}
	return v;
}

void isolationtnp(std::vector<std::string> names) {
	gStyle->SetOptStat(0);
	ROOT::EnableImplicitMT(128);
	RDataFrame df("demo/Events",names);
	auto d1=df.Define("goodgenpt",goodgenvalue,{"GenPart_pt","GenPart_postFSRLepIdx1","GenPart_postFSRLepIdx2","GenPart_eta","GenPart_phi","GenPart_status","GenPart_pdgId"}).Define("goodgeneta",goodgenvalue,{"GenPart_eta","GenPart_postFSRLepIdx1","GenPart_postFSRLepIdx2","GenPart_eta","GenPart_phi","GenPart_status","GenPart_pdgId"}).Define("goodgenphi",goodgenvalue,{"GenPart_phi","GenPart_postFSRLepIdx1","GenPart_postFSRLepIdx2","GenPart_eta","GenPart_phi","GenPart_status","GenPart_pdgId"}).Define("goodgenidx",goodgenidx,{"GenPart_pt","GenPart_postFSRLepIdx1","GenPart_postFSRLepIdx2","GenPart_eta","GenPart_phi","GenPart_status","GenPart_pdgId"}).Define("goodmuonbool",goodmuonboolisolationnotrig,{"goodgenidx","Muon_genPartIdx","Muon_eta","Muon_phi","Muon_standeta","Muon_standphi","Muon_isStandalone","Muon_isGlobal","Muon_mediumId","Muon_dxyBS","Muon_triggered","Muon_pfRelIso04_all"}).Define("goodmuonboolnorm",Goodmuonboolnotrig,{"goodgenidx","Muon_genPartIdx","Muon_eta","Muon_phi","Muon_standeta","Muon_standphi","Muon_isStandalone","Muon_isGlobal","Muon_mediumId","Muon_dxyBS","Muon_triggered","Muon_pfRelIso04_all"});
	d1=d1.Define("zero","0").Define("one","1").Define("two","2").Define("three","3").Define("four","4");
	d1=d1.Define("tnppairtagreco",maketnppairs,{"Muon_pt","Muon_eta","Muon_phi","Muon_mass","Muon_standeta","Muon_standphi","Muon_isStandalone","Muon_isGlobal","Muon_mediumId","Muon_dxyBS","Muon_triggered","Muon_pfRelIso04_all","zero"}).Define("tnppairprobereco",maketnppairs,{"Muon_pt","Muon_eta","Muon_phi","Muon_mass","Muon_standeta","Muon_standphi","Muon_isStandalone","Muon_isGlobal","Muon_mediumId","Muon_dxyBS","Muon_triggered","Muon_pfRelIso04_all","one"}).Define("tnppairflagreco",maketnppairs,{"Muon_pt","Muon_eta","Muon_phi","Muon_mass","Muon_standeta","Muon_standphi","Muon_isStandalone","Muon_isGlobal","Muon_mediumId","Muon_dxyBS","Muon_triggered","Muon_pfRelIso04_all","two"});
	d1=d1.Define("goodtnppairtagminiaod",goodtnppairs,{"tnppairtagreco","tnppairprobereco","tnppairflagreco","Muon_genPartIdx","Muon_genPartIdx","goodgenidx","zero"}).Define("goodtnppairprobeminiaod",goodtnppairs,{"tnppairtagreco","tnppairprobereco","tnppairflagreco","Muon_genPartIdx","Muon_genPartIdx","goodgenidx","one"}).Define("goodtnppairflagminiaod",goodtnppairs,{"tnppairtagreco","tnppairprobereco","tnppairflagreco","Muon_genPartIdx","Muon_genPartIdx","goodgenidx","two"});
	d1=d1.Define("isglobalminiaod",isglobal,{"goodtnppairflagminiaod","one"});
	d1=d1.Define("idipdenominatortag","goodtnppairtagminiaod[isglobalminiaod]").Define("idipdenominatorprobe","goodtnppairprobeminiaod[isglobalminiaod]").Define("idipdenominatorflag","goodtnppairflagminiaod[isglobalminiaod]");
	d1=d1.Define("isidip",isglobal,{"idipdenominatorflag","two"});
	d1=d1.Define("isolationdenominatortag","idipdenominatortag[isidip]").Define("isolationdenominatorprobe","idipdenominatorprobe[isidip]").Define("isolationdenominatorflag","idipdenominatorflag[isidip]");
	d1=d1.Define("isisolation",isglobal,{"isolationdenominatorflag","four"});
	d1=d1.Define("goodprobept",goodtrackgen,{"isolationdenominatortag","isolationdenominatorprobe","Muon_genPartIdx","goodgenidx","goodgenpt"}).Define("goodprobeeta",goodtrackgen,{"isolationdenominatortag","isolationdenominatorprobe","Muon_genPartIdx","goodgenidx","goodgeneta"}).Define("goodprobephi",goodtrackgen,{"isolationdenominatortag","isolationdenominatorprobe","Muon_genPartIdx","goodgenidx","goodgenphi"}).Define("goodtagpt",goodtrackgen,{"isolationdenominatortag","isolationdenominatortag","Muon_genPartIdx","goodgenidx","goodgenpt"}).Define("goodtageta",goodtrackgen,{"isolationdenominatortag","isolationdenominatortag","Muon_genPartIdx","goodgenidx","goodgeneta"}).Define("goodtagphi",goodtrackgen,{"isolationdenominatortag","isolationdenominatortag","Muon_genPartIdx","goodgenidx","goodgenphi"});
	d1=d1.Define("genzpt",genzpt,{"goodgenpt","goodgeneta","goodgenphi","goodmuonboolnorm"}).Define("tnpzpt",zpt,{"goodtagpt","goodtageta","goodtagphi","goodprobept","goodprobeeta","goodprobephi"});
	d1=d1.Define("Goodgeneta","goodgeneta[goodmuonboolnorm]").Define("absGoodgeneta","abs(Goodgeneta)").Define("Goodgenpt","goodgenpt[goodmuonboolnorm]").Define("Goodmuonbool","goodmuonbool[goodmuonboolnorm]");
	d1=d1.Define("absgoodprobeeta","abs(goodprobeeta)");
	int neta=24, npt=9, nqt=100;
	float mineta=0, minpt=25., minqt=-3.2;
	float maxeta=2.4, maxpt=60., maxqt=3.2;
	auto c=d1.Filter("goodgeneta.size()>=2");
	auto histo13 = c.Histo3D({"histo13", "", nqt, minqt, maxqt, neta, mineta, maxeta, npt, minpt, maxpt}, "genzpt","absGoodgeneta","Goodgenpt","Goodmuonbool");
	auto histo14 = c.Histo3D({"histo14", "", nqt, minqt, maxqt, neta, mineta, maxeta, npt, minpt, maxpt}, "genzpt","absGoodgeneta","Goodgenpt");
	auto histo15 = c.Histo3D({"histo15", "", nqt, minqt, maxqt, neta, mineta, maxeta, npt, minpt, maxpt}, "tnpzpt","absgoodprobeeta","goodprobept","isisolation");
	auto histo16 = c.Histo3D({"histo16", "", nqt, minqt, maxqt, neta, mineta, maxeta, npt, minpt, maxpt}, "tnpzpt","absgoodprobeeta","goodprobept");
	TFile* histos=new TFile("deltaphihistos.root","RECREATE");
	histos->cd();
	TH3D* Histo13=(TH3D*)histo13.GetPtr()->Clone(), *Histo14=(TH3D*)histo14.GetPtr()->Clone(), *Histo15=(TH3D*)histo15.GetPtr()->Clone(), *Histo16=(TH3D*)histo16.GetPtr()->Clone();
	for (unsigned int i=1; i<=nqt; i++) {
		float binqtmin=(maxqt-minqt)/nqt*(i-1)+minqt, binqtmax=(maxqt-minqt)/nqt*i+minqt;
		Histo13->GetXaxis()->SetRange(i,i);
		Histo14->GetXaxis()->SetRange(i,i);
		Histo15->GetXaxis()->SetRange(i,i);
		Histo16->GetXaxis()->SetRange(i,i);
		std::string name("MCtruth");
		name+="_"+std::to_string(i);
		std::string title("MCtruth #Delta#phi_{Zq_{T},probe}>");
		title+=std::to_string(binqtmin)+"&&#Delta#phi_{Zq_{T},probe}<"+std::to_string(binqtmax);
		std::string name2("TNP");
		name2+="_"+std::to_string(i);
		std::string title2("TNP #Delta#phi_{Zq_{T},probe}>");
		title2+=std::to_string(binqtmin)+"&&#Delta#phi_{Zq_{T},probe}<"+std::to_string(binqtmax);
		std::string name3("MCvsTNP");
		name3+="_"+std::to_string(i);
		std::string title3("MCvsTNP #Delta#phi_{Zq_{T},probe}>");
		title3+=std::to_string(binqtmin)+"&&#Delta#phi_{Zq_{T},probe}<"+std::to_string(binqtmax);
		TH2D* histo1=(TH2D*)Histo13->Project3D("zy");
		TH2D* histo2=(TH2D*)Histo14->Project3D("zy");
		TH2D* histo3=(TH2D*)Histo15->Project3D("zy");
		TH2D* histo4=(TH2D*)Histo16->Project3D("zy");
		histo1->Divide(histo2);
		histo3->Divide(histo4);
		histo1->SetName(name.c_str());
		histo1->SetTitle(title.c_str());
		histo3->SetName(name2.c_str());
		histo3->SetTitle(title2.c_str());
		TH2D* histo5=(TH2D*)histo1->Clone();
		histo5->Divide(histo3);
		histo5->SetName(name3.c_str());
		histo5->SetTitle(title3.c_str());
		histo5->SetMaximum(1.1);
		histo5->SetMinimum(0.9);
		histo1->SetOption("colz");
		histo3->SetOption("colz");
		histo5->SetOption("colz");
		histo1->SetDirectory(histos->GetDirectory(""));
		histo3->SetDirectory(histos->GetDirectory(""));
		histo5->SetDirectory(histos->GetDirectory(""));
	}
	histos->Write();
	histos->Close();
	std::cout<<"FINISHED\n";
}

int main(int argc, char **argv) {
	std::cout<<"RUN ON CMSANALYSIS\n";
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
