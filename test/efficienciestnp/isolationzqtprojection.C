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
			if (goodmuonbool[i]) v.emplace_back(((v1+v2)*v1)/sqrt(v1*v1));
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
		v.emplace_back(((v1+v2)*v2)/sqrt(v2*v2));
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

RVec<float> goodgenprefsrpt(RVec<float> &GenPart_pt, RVec<int> &goodgenidx, RVec<int> &GenPart_pdgId, int &GenPart_preFSRLepIdx1, int &GenPart_preFSRLepIdx2) {
	RVec<float> v;
	for (auto i=0U; i!=goodgenidx.size(); i++) {
		float value;
		if (GenPart_pdgId[GenPart_preFSRLepIdx1]==GenPart_pdgId[GenPart_preFSRLepIdx2]) {
			break;
		}
		if (GenPart_pdgId[goodgenidx[i]]==GenPart_pdgId[GenPart_preFSRLepIdx1]) value=GenPart_pt[GenPart_preFSRLepIdx1];
		if (GenPart_pdgId[goodgenidx[i]]==GenPart_pdgId[GenPart_preFSRLepIdx2]) value=GenPart_pt[GenPart_preFSRLepIdx2];
		v.emplace_back(value);
	}
	return v;
}

void isolationtnp(std::vector<std::string> names) {
	gROOT->SetBatch(kTRUE);
	gStyle->SetOptStat(0);
	ROOT::EnableImplicitMT(128); //RUN ON CMSANALYSIS!
	RDataFrame df("demo/Events",names);
	auto d1=df.Define("goodgenpt",goodgenvalue,{"GenPart_pt","GenPart_postFSRLepIdx1","GenPart_postFSRLepIdx2","GenPart_eta","GenPart_phi","GenPart_status","GenPart_pdgId"}).Define("goodgeneta",goodgenvalue,{"GenPart_eta","GenPart_postFSRLepIdx1","GenPart_postFSRLepIdx2","GenPart_eta","GenPart_phi","GenPart_status","GenPart_pdgId"}).Define("goodgenphi",goodgenvalue,{"GenPart_phi","GenPart_postFSRLepIdx1","GenPart_postFSRLepIdx2","GenPart_eta","GenPart_phi","GenPart_status","GenPart_pdgId"}).Define("goodgenidx",goodgenidx,{"GenPart_pt","GenPart_postFSRLepIdx1","GenPart_postFSRLepIdx2","GenPart_eta","GenPart_phi","GenPart_status","GenPart_pdgId"}).Define("goodmuonbool",goodmuonboolisolationnotrig,{"goodgenidx","Muon_genPartIdx","Muon_eta","Muon_phi","Muon_standeta","Muon_standphi","Muon_isStandalone","Muon_isGlobal","Muon_mediumId","Muon_dxyBS","Muon_triggered","Muon_pfRelIso04_all"}).Define("goodmuonboolnorm",Goodmuonboolnotrig,{"goodgenidx","Muon_genPartIdx","Muon_eta","Muon_phi","Muon_standeta","Muon_standphi","Muon_isStandalone","Muon_isGlobal","Muon_mediumId","Muon_dxyBS","Muon_triggered","Muon_pfRelIso04_all"}).Define("goodgenprefsrpt",goodgenprefsrpt,{"GenPart_pt","goodgenidx","GenPart_pdgId","GenPart_preFSRLepIdx1","GenPart_preFSRLepIdx2"});
	d1=d1.Define("zero","0").Define("one","1").Define("two","2").Define("three","3").Define("four","4");
	d1=d1.Define("tnppairtagreco",maketnppairs,{"Muon_pt","Muon_eta","Muon_phi","Muon_mass","Muon_standeta","Muon_standphi","Muon_isStandalone","Muon_isGlobal","Muon_mediumId","Muon_dxyBS","Muon_triggered","Muon_pfRelIso04_all","zero"}).Define("tnppairprobereco",maketnppairs,{"Muon_pt","Muon_eta","Muon_phi","Muon_mass","Muon_standeta","Muon_standphi","Muon_isStandalone","Muon_isGlobal","Muon_mediumId","Muon_dxyBS","Muon_triggered","Muon_pfRelIso04_all","one"}).Define("tnppairflagreco",maketnppairs,{"Muon_pt","Muon_eta","Muon_phi","Muon_mass","Muon_standeta","Muon_standphi","Muon_isStandalone","Muon_isGlobal","Muon_mediumId","Muon_dxyBS","Muon_triggered","Muon_pfRelIso04_all","two"});
	d1=d1.Define("goodtnppairtagminiaod",goodtnppairs,{"tnppairtagreco","tnppairprobereco","tnppairflagreco","Muon_genPartIdx","Muon_genPartIdx","goodgenidx","zero"}).Define("goodtnppairprobeminiaod",goodtnppairs,{"tnppairtagreco","tnppairprobereco","tnppairflagreco","Muon_genPartIdx","Muon_genPartIdx","goodgenidx","one"}).Define("goodtnppairflagminiaod",goodtnppairs,{"tnppairtagreco","tnppairprobereco","tnppairflagreco","Muon_genPartIdx","Muon_genPartIdx","goodgenidx","two"});
	d1=d1.Define("isglobalminiaod",isglobal,{"goodtnppairflagminiaod","one"});
	d1=d1.Define("idipdenominatortag","goodtnppairtagminiaod[isglobalminiaod]").Define("idipdenominatorprobe","goodtnppairprobeminiaod[isglobalminiaod]").Define("idipdenominatorflag","goodtnppairflagminiaod[isglobalminiaod]");
	d1=d1.Define("isidip",isglobal,{"idipdenominatorflag","two"});
	d1=d1.Define("isolationdenominatortag","idipdenominatortag[isidip]").Define("isolationdenominatorprobe","idipdenominatorprobe[isidip]").Define("isolationdenominatorflag","idipdenominatorflag[isidip]");
	d1=d1.Define("isisolation",isglobal,{"isolationdenominatorflag","four"});
	d1=d1.Define("goodprobept",goodtrackgen,{"isolationdenominatortag","isolationdenominatorprobe","Muon_genPartIdx","goodgenidx","goodgenpt"}).Define("goodprobeeta",goodtrackgen,{"isolationdenominatortag","isolationdenominatorprobe","Muon_genPartIdx","goodgenidx","goodgeneta"}).Define("goodprobephi",goodtrackgen,{"isolationdenominatortag","isolationdenominatorprobe","Muon_genPartIdx","goodgenidx","goodgenphi"}).Define("goodtagpt",goodtrackgen,{"isolationdenominatortag","isolationdenominatortag","Muon_genPartIdx","goodgenidx","goodgenpt"}).Define("goodtageta",goodtrackgen,{"isolationdenominatortag","isolationdenominatortag","Muon_genPartIdx","goodgenidx","goodgeneta"}).Define("goodtagphi",goodtrackgen,{"isolationdenominatortag","isolationdenominatortag","Muon_genPartIdx","goodgenidx","goodgenphi"}).Define("goodprobeprefsrpt",goodtrackgen,{"isolationdenominatortag","isolationdenominatorprobe","Muon_genPartIdx","goodgenidx","goodgenprefsrpt"});
	d1=d1.Define("genzpt",genzpt,{"goodgenpt","goodgeneta","goodgenphi","goodmuonboolnorm"}).Define("tnpzpt",zpt,{"goodtagpt","goodtageta","goodtagphi","goodprobept","goodprobeeta","goodprobephi"});
	d1=d1.Define("difference","(goodprobeprefsrpt-goodprobept)").Define("lowpt","difference[goodprobept>=25&&goodprobept<=30]").Define("highpt","difference[goodprobept>=40]").Define("tnpzptlowpt","tnpzpt[goodprobept>=25&&goodprobept<=30]").Define("tnpzpthighpt","tnpzpt[goodprobept>=40]");
	d1=d1.Define("Goodgeneta","goodgeneta[goodmuonboolnorm]").Define("absGoodgeneta","abs(Goodgeneta)").Define("Goodgenpt","goodgenpt[goodmuonboolnorm]").Define("Goodgenprefsrpt","goodgenprefsrpt[goodmuonboolnorm]").Define("Goodmuonbool","goodmuonbool[goodmuonboolnorm]");
	d1=d1.Define("absgoodprobeeta","abs(goodprobeeta)");
	d1=d1.Define("differencemctruth","(Goodgenprefsrpt-Goodgenpt)").Define("lowptmctruth","differencemctruth[Goodgenpt>=25&&Goodgenpt<=30]").Define("highptmctruth","differencemctruth[Goodgenpt>=40]").Define("genzptlowpt","genzpt[Goodgenpt>=25&&Goodgenpt<=30]").Define("genzpthighpt","genzpt[Goodgenpt>=40]");
	d1=d1.Define("absGoodgenetalowpt","absGoodgeneta[Goodgenpt>=25&&Goodgenpt<=30]").Define("absGoodgenetahighpt","absGoodgeneta[Goodgenpt>=40]");
	d1=d1.Define("absgoodprobeetalowpt","absgoodprobeeta[goodprobept>=25&&goodprobept<=30]").Define("absgoodprobeetahighpt","absgoodprobeeta[goodprobept>=40]");
	int neta=24, npt=9, nqt=100, nqt2=40;
	float mineta=0, minpt=25., minqt=-100.;
	float maxeta=2.4, maxpt=60., maxqt=100.;
	auto c=d1.Filter("goodgeneta.size()>=2");
	auto histo13 = c.Histo3D({"histo13", "", nqt, minqt, maxqt, neta, mineta, maxeta, npt, minpt, maxpt}, "genzpt","absGoodgeneta","Goodgenpt","Goodmuonbool");
	auto histo14 = c.Histo3D({"histo14", "", nqt, minqt, maxqt, neta, mineta, maxeta, npt, minpt, maxpt}, "genzpt","absGoodgeneta","Goodgenpt");
	auto histo15 = c.Histo3D({"histo15", "", nqt, minqt, maxqt, neta, mineta, maxeta, npt, minpt, maxpt}, "tnpzpt","absgoodprobeeta","goodprobept","isisolation");
	auto histo16 = c.Histo3D({"histo16", "", nqt, minqt, maxqt, neta, mineta, maxeta, npt, minpt, maxpt}, "tnpzpt","absgoodprobeeta","goodprobept");
	auto histoA = c.Histo2D({"histoA", "", nqt2, minqt, maxqt, 100,-10,10},"genzptlowpt","lowptmctruth");
	auto histoB = c.Histo2D({"histoB", "", nqt2, minqt, maxqt, 100,-10,10},"genzpthighpt","highptmctruth");
	auto histoC = c.Histo2D({"histoC", "", nqt2, minqt, maxqt, 100,-10,10},"tnpzptlowpt","lowpt");
	auto histoD = c.Histo2D({"histoD", "", nqt2, minqt, maxqt, 100,-10,10},"tnpzpthighpt","highpt");
	auto histoE = c.Histo3D({"histoE", "", nqt2, minqt, maxqt, neta, mineta, maxeta, 100,-10,10},"genzptlowpt","absGoodgenetalowpt","lowptmctruth");
	auto histoF = c.Histo3D({"histoF", "", nqt2, minqt, maxqt, neta, mineta, maxeta, 100,-10,10},"genzpthighpt","absGoodgenetahighpt","highptmctruth");
	auto histoG = c.Histo3D({"histoG", "", nqt2, minqt, maxqt, neta, mineta, maxeta, 100,-10,10},"tnpzptlowpt","absgoodprobeetalowpt","lowpt");
	auto histoH = c.Histo3D({"histoH", "", nqt2, minqt, maxqt, neta, mineta, maxeta, 100,-10,10},"tnpzpthighpt","absgoodprobeetahighpt","highpt");
	TH3D* Histo13=(TH3D*)histo13.GetPtr()->Clone(), *Histo14=(TH3D*)histo14.GetPtr()->Clone(), *Histo15=(TH3D*)histo15.GetPtr()->Clone(), *Histo16=(TH3D*)histo16.GetPtr()->Clone(), *HistoE=(TH3D*)histoE.GetPtr()->Clone(), *HistoF=(TH3D*)histoF.GetPtr()->Clone(), *HistoG=(TH3D*)histoG.GetPtr()->Clone(), *HistoH=(TH3D*)histoH.GetPtr()->Clone();
	TH2D* HistoA=(TH2D*)histoA.GetPtr()->Clone(), *HistoB=(TH2D*)histoB.GetPtr()->Clone(), *HistoC=(TH2D*)histoC.GetPtr()->Clone(), *HistoD=(TH2D*)histoD.GetPtr()->Clone();
	TFile* histos=new TFile("zqtprojectionhistos.root","RECREATE");
	histos->cd();
	for (unsigned int i=1; i<=nqt; i++) {
		float binqtmin=(maxqt-minqt)/nqt*(i-1)+minqt, binqtmax=(maxqt-minqt)/nqt*i+minqt;
		Histo13->GetXaxis()->SetRange(i,i);
		Histo14->GetXaxis()->SetRange(i,i);
		Histo15->GetXaxis()->SetRange(i,i);
		Histo16->GetXaxis()->SetRange(i,i);
		std::string name("MCtruth");
		name+="_"+std::to_string(i);
		std::string title("MCtruth Z q_{T} projection wrt probe>");
		title+=std::to_string(binqtmin)+"&&Z q_{T} projection wrt probe<"+std::to_string(binqtmax);
		std::string name2("TNP");
		name2+="_"+std::to_string(i);
		std::string title2("TNP Z q_{T} projection wrt probe>");
		title2+=std::to_string(binqtmin)+"&&Z q_{T} projection wrt probe<"+std::to_string(binqtmax);
		std::string name3("MCvsTNP");
		name3+="_"+std::to_string(i);
		std::string title3("MCvsTNP Z q_{T} projection wrt probe>");
		title3+=std::to_string(binqtmin)+"&&Z q_{T} projection wrt probe<"+std::to_string(binqtmax);
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
	TFile* histos2=new TFile("zqtprojectionhistosfsr.root","RECREATE");
	histos2->cd();
	for (unsigned int i=1; i<=nqt2; i++) {
		float binqtmin=(maxqt-minqt)/nqt2*(i-1)+minqt, binqtmax=(maxqt-minqt)/nqt2*i+minqt;
		HistoA->GetXaxis()->SetRange(i,i);
		HistoB->GetXaxis()->SetRange(i,i);
		HistoC->GetXaxis()->SetRange(i,i);
		HistoD->GetXaxis()->SetRange(i,i);
		std::string name("lowpt1");
		name+="_"+std::to_string(i);
		std::string title("25 GeV<probe p_{T}<30 GeV&&");
		title+=std::to_string(binqtmin)+"<Z q_{T} projection wrt probe<"+std::to_string(binqtmax);
		std::string name2("highpt1");
		name2+="_"+std::to_string(i);
		std::string title2("probe p_{T}>40 GeV&&");
		title2+=std::to_string(binqtmin)+"<Z q_{T} projection wrt probe<"+std::to_string(binqtmax);
		std::string name3("lowpt2");
		name3+="_"+std::to_string(i);
		std::string title3("25 GeV<probe p_{T}<30 GeV&&");
		title3+=std::to_string(binqtmin)+"<Z q_{T} projection wrt probe<"+std::to_string(binqtmax);
		std::string name4("highpt2");
		name4+="_"+std::to_string(i);
		std::string title4("probe p_{T}>40 GeV&&");
		title4+=std::to_string(binqtmin)+"<Z q_{T} projection wrt probe<"+std::to_string(binqtmax);
		TH1D* histoA1=(TH1D*)HistoA->ProjectionY();
		TH1D* histoB2=(TH1D*)HistoB->ProjectionY();
		TH1D* histoC3=(TH1D*)HistoC->ProjectionY();
		TH1D* histoD4=(TH1D*)HistoD->ProjectionY();
		histoA1->SetName(name.c_str());
		histoA1->SetTitle(title.c_str());
		histoA1->SetLineColor(kRed);
		histoB2->SetName(name2.c_str());
		histoB2->SetTitle(title2.c_str());
		histoB2->SetLineColor(kRed);
		histoC3->SetName(name3.c_str());
		histoD4->SetName(name4.c_str());
		histoA1->Scale(1./histoA1->Integral(0,101));
		histoA1->SetMaximum(1.);
		histoB2->Scale(1./histoB2->Integral(0,101));
		histoB2->SetMaximum(1.);
		histoC3->Scale(1./histoC3->Integral(0,101));
		histoC3->SetMaximum(1.);
		histoD4->Scale(1./histoD4->Integral(0,101));
		histoD4->SetMaximum(1.);
		TCanvas *c1=new TCanvas();
		c1->cd();
		c1->Draw();
		histoA1->GetXaxis()->SetTitle("#Deltap_{T}(prefsr,postfsr)");
		histoA1->Draw("histsamee0");
		histoC3->Draw("histsamee0");
		histos2->cd();
		TCanvas *c2=new TCanvas();
		c2->cd();
		c2->Draw();
		histoB2->GetXaxis()->SetTitle("#Deltap_{T}(prefsr,postfsr)");
		histoB2->Draw("histsamee0");
		histoD4->Draw("histsamee0");
		histos2->cd();
		c1->Write();
		c2->Write();
	}
	histos2->Write();
	histos2->Close();
	TFile* histos3=new TFile("zqtprojectionhistosfsreta.root","RECREATE");
	histos3->cd();
	for (unsigned int i=1; i<=nqt2; i++) {
		float binqtmin=(maxqt-minqt)/nqt2*(i-1)+minqt, binqtmax=(maxqt-minqt)/nqt2*i+minqt;
		HistoE->GetXaxis()->SetRange(i,i);
		HistoF->GetXaxis()->SetRange(i,i);
		HistoG->GetXaxis()->SetRange(i,i);
		HistoH->GetXaxis()->SetRange(i,i);
		for (unsigned int j=1; j<=neta; j++) {
			float binetamin=(maxeta-mineta)/neta*(j-1)+mineta, binetamax=(maxeta-mineta)/neta*j+mineta;
			HistoE->GetYaxis()->SetRange(j,j);
			HistoF->GetYaxis()->SetRange(j,j);
			HistoG->GetYaxis()->SetRange(j,j);
			HistoH->GetYaxis()->SetRange(j,j);
			std::string name("lowpt1");
			name+="_"+std::to_string(i)+"_"+std::to_string(j);
			std::string title("25 GeV<probe p_{T}<30 GeV&&");
			title+=std::to_string(binetamin)+"<probe |#eta|<"+std::to_string(binetamax)+"&&"+std::to_string(binqtmin)+"<Z q_{T} projection wrt probe<"+std::to_string(binqtmax);
			std::string name2("highpt1");
			name2+="_"+std::to_string(i)+"_"+std::to_string(j);
			std::string title2("probe p_{T}>40 GeV&&");
			title2+=std::to_string(binetamin)+"<probe |#eta|<"+std::to_string(binetamax)+"&&"+std::to_string(binqtmin)+"<Z q_{T} projection wrt probe<"+std::to_string(binqtmax);
			std::string name3("lowpt2");
			name3+="_"+std::to_string(i)+"_"+std::to_string(j);
			std::string title3("25 GeV<probe p_{T}<30 GeV&&");
			title3+=std::to_string(binetamin)+"<probe |#eta|<"+std::to_string(binetamax)+"&&"+std::to_string(binqtmin)+"<Z q_{T} projection wrt probe<"+std::to_string(binqtmax);
			std::string name4("highpt2");
			name4+="_"+std::to_string(i)+"_"+std::to_string(j);
			std::string title4("probe p_{T}>40 GeV&&");
			title4+=std::to_string(binetamin)+"<probe |#eta|<"+std::to_string(binetamax)+"&&"+std::to_string(binqtmin)+"<Z q_{T} projection wrt probe<"+std::to_string(binqtmax);
			TH1D* histoE1=(TH1D*)HistoE->Project3D("z");
			TH1D* histoF2=(TH1D*)HistoF->Project3D("z");
			TH1D* histoG3=(TH1D*)HistoG->Project3D("z");
			TH1D* histoH4=(TH1D*)HistoH->Project3D("z");
			histoE1->SetName(name.c_str());
			histoE1->SetTitle(title.c_str());
			histoE1->SetLineColor(kRed);
			histoF2->SetName(name2.c_str());
			histoF2->SetTitle(title2.c_str());
			histoF2->SetLineColor(kRed);
			histoG3->SetName(name3.c_str());
			histoH4->SetName(name4.c_str());
			histoE1->Scale(1./histoE1->Integral(0,101));
			histoE1->SetMaximum(1.);
			histoF2->Scale(1./histoF2->Integral(0,101));
			histoF2->SetMaximum(1.);
			histoG3->Scale(1./histoG3->Integral(0,101));
			histoG3->SetMaximum(1.);
			histoH4->Scale(1./histoH4->Integral(0,101));
			histoH4->SetMaximum(1.);
			TCanvas *c1=new TCanvas();
			c1->cd();
			c1->Draw();
			histoE1->GetXaxis()->SetTitle("#Deltap_{T}(prefsr,postfsr)");
			histoE1->Draw("histsamee0");
			histoG3->Draw("histsamee0");
			histos3->cd();
			TCanvas *c2=new TCanvas();
			c2->cd();
			c2->Draw();
			histoF2->GetXaxis()->SetTitle("#Deltap_{T}(prefsr,postfsr)");
			histoF2->Draw("histsamee0");
			histoH4->Draw("histsamee0");
			histos3->cd();
			c1->Write();
			c2->Write();
		}
	}
	histos3->Write();
	histos3->Close();
	std::cout<<"FINISHED\n";
}

int main(int argc, char **argv) {
	std::cout<<"RUN ON CMSANALYSIS!\n";
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
