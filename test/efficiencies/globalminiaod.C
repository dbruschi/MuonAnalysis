#include "ROOT/RDataFrame.hxx"
#include "ROOT/RVec.hxx"
#include "TFile.h"
#include "TLorentzVector.h"
#include "TStyle.h"
#include "TCanvas.h"
#include "TH2D.h"
#include "TApplication.h"
#include "Functions.h"
#include <iostream>

using namespace ROOT;

RVec<bool> goodmuonboolglobal1(RVec<float> &goodgeneta, RVec<float> &goodgenphi, RVec<int> &goodmuonidx, RVec<int> &Muon_genPartIdx, RVec<bool> &Muon_isGlobal, RVec<float> &Muon_standeta, RVec<float> &Muon_standphi, RVec<float> &Muon_eta, RVec<float> &Muon_phi, RVec<bool> &Muon_isStandalone) {
	RVec<bool> v;
	for (auto i=0U;i < goodgeneta.size(); ++i) {
		bool condition=false;
		int idx=-1;
		for (auto j=0U; j < Muon_genPartIdx.size(); ++j) {
			if (Muon_genPartIdx[j]==goodmuonidx[i]) {condition=true;
				idx=j;
			}
		}
		if ((condition)&&(Muon_isGlobal[idx])) {
			TLorentzVector cand1, cand2;
			cand1.SetPtEtaPhiM(5.,Muon_eta[idx],Muon_phi[idx],0.);
			cand2.SetPtEtaPhiM(5.,Muon_standeta[idx],Muon_standphi[idx],0.);
			if (cand1.DeltaR(cand2)<0.3) {
				if (cleaner(idx,Muon_standeta,Muon_standphi,Muon_isStandalone)) v.emplace_back(true);
				else v.emplace_back(false);
			}
			else v.emplace_back(false);
		}
		else v.emplace_back(false);
	}
	return v;
}

RVec<int> count(RVec<float> &goodgeneta, RVec<float> &goodgenphi, RVec<int> &goodmuonidx, RVec<int> &Muon_genPartIdx, RVec<bool> &Muon_isGlobal, RVec<float> &Muon_standeta, RVec<float> &Muon_standphi, RVec<float> &Muon_eta, RVec<float> &Muon_phi, RVec<float> &Muon_pt, RVec<float> &Muon_standpt, RVec<float> &goodgenpt, RVec<int> &Muon_standValidDThits, RVec<int> &Muon_standValidRPChits) {
	RVec<bool> v;
	for (auto i=0U;i < goodgeneta.size(); ++i) {
		int counter=0;
		int idx=-1;
		bool condition=false;
		for (auto j=0U; j < Muon_genPartIdx.size(); ++j) {
			if (Muon_genPartIdx[j]==goodmuonidx[i]) {condition=true;
				idx=j;
			}
		}
		if ((condition)&&(Muon_isGlobal[idx])) {
			for (auto j=0U; j < Muon_genPartIdx.size(); ++j) {
				if (j==idx) continue;
				if (Muon_standeta[j]<-95) continue;
				TLorentzVector cand1, cand2;
				TLorentzVector Cand1, Cand2;
				cand1.SetPtEtaPhiM(5.,Muon_eta[idx],Muon_phi[idx],0.);
				cand2.SetPtEtaPhiM(5.,Muon_eta[j],Muon_phi[j],0.);
				Cand1.SetPtEtaPhiM(5.,Muon_standeta[idx],Muon_standphi[idx],0.);
				Cand2.SetPtEtaPhiM(5.,Muon_standeta[j],Muon_standphi[j],0.);
				if (cand1.DeltaR(Cand1)>0.3) continue;
				//if (cand2.DeltaR(Cand2)>0.3) continue;
				bool abcd=false;
				if (cand1.DeltaR(Cand2)<0.3) {counter++;
					if (abs(goodgeneta[i])>2.0) {
						if (goodgenpt[i]<25.) continue;
						TLorentzVector Cand3, Cand4;
						Cand3.SetPtEtaPhiM(goodgenpt[i],goodgeneta[i],goodgenphi[i],0.);
						//std::cout<<"Generator muon coordinates:\t\t\t\t\t\t\tpt "<<goodgenpt[i]<<"\teta "<<goodgeneta[i]<<"\tphi "<<goodgenphi[i]<<"\n";
						//std::cout<<"Reconstructed muon (matched to gen):\t\t\t\tpt "<<Muon_pt[idx]<<"\teta "<<Muon_eta[idx]<<"\tphi "<<Muon_phi[idx]<<"\n";
						//std::cout<<"Reconstructed standalone coordinates (same muon): \tpt "<<Muon_standpt[idx]<<"\teta "<<Muon_standeta[idx]<<"\tphi "<<Muon_standphi[idx]<<"\n";
						//std::cout<<"Valid CSC hits: "<<Muon_standValidDThits[idx]<<" Valid RPC hits: "<<Muon_standValidRPChits[idx]<<"\n";
						//std::cout<<"Clone muon coordinates:\t\t\t\t\t\t\t\tpt "<<Muon_pt[j]<<"\teta "<<Muon_eta[j]<<"\tphi "<<Muon_phi[j]<<"\tIs Global? "<<Muon_isGlobal[j]<<"\n";
						//std::cout<<"Clone muon stand coordinates:\t\t\t\t\t\tpt "<<Muon_standpt[j]<<"\teta "<<Muon_standeta[j]<<"\tphi "<<Muon_standphi[j]<<"\n";
						//std::cout<<"Valid CSC hits: "<<Muon_standValidDThits[j]<<" Valid RPC hits: "<<Muon_standValidRPChits[j]<<"\n";
						//if (Cand1.DeltaR(Cand3)>Cand2.DeltaR(Cand3)) std::cout<<"wrong matching\n";
						//abcd=true;
						/*for (auto I=0U;I < goodgeneta.size(); ++I) {
							std::cout<<goodgenpt[I]<<" "<<goodgeneta[I]<<" "<<goodgenphi[I]<<"\n";
							abcd=true;
						}*/
					}
				}
				//if (abcd) std::cout<<"\n";
			}
		}
		if (counter>0) v.emplace_back(1);
		else v.emplace_back(0);
	}
	return v;
}

RVec<bool> Goodstand(RVec<int> &goodmuonidx, RVec<bool> &Muon_isGlobal, RVec<float> &Muon_eta, RVec<float> &Muon_phi, RVec<float> &Muon_standeta, RVec<float> &Muon_standphi, RVec<bool> &Muon_isStandalone) {
	RVec<bool> v;
	for (auto i=0U;i < goodmuonidx.size(); ++i) {
		TLorentzVector cand1, cand2;
		cand1.SetPtEtaPhiM(3.,Muon_eta[goodmuonidx[i]],Muon_phi[goodmuonidx[i]],0.);
		cand2.SetPtEtaPhiM(3.,Muon_standeta[goodmuonidx[i]],Muon_standphi[goodmuonidx[i]],0.);
		if ((Muon_isGlobal[goodmuonidx[i]])&&(cand1.DeltaR(cand2)<0.3)) {
			if (cleaner(i,Muon_standeta,Muon_standphi,Muon_isStandalone)) v.emplace_back(true);
			else v.emplace_back(false);
		}
		else v.emplace_back(false);
	}
	return v;
}

RVec<float> deltarcritical(RVec<float> &goodgeneta, RVec<float> &goodgenphi, RVec<float> &Muon_standeta, RVec<float> &Muon_standphi, RVec<bool> &Muon_isStandalone){
	RVec<float> v;
	for (auto j=0U; j < goodgeneta.size(); ++j) {
		if (abs(goodgeneta[j])>0.1) continue;
		std::map<float,int> Map;
		for (auto i=0U;i < Muon_standeta.size(); ++i) {
			if (!Muon_isStandalone[i]) continue;
			TLorentzVector cand, cand2;
			cand.SetPtEtaPhiM(5.,goodgeneta[j],goodgenphi[j],0.);
			cand2.SetPtEtaPhiM(5.,Muon_standeta[i],Muon_standphi[i],0.);
			Map.insert({cand.DeltaR(cand2),i});
		}
		if (Map.size()==0) continue;
		if (Map.begin()->first<0.3) {
			for (auto z=0U; z < Muon_standeta.size(); ++z) {
				if (z==Map.begin()->second) continue;
				if (!Muon_isStandalone[z]) continue;
				TLorentzVector cand, cand2;
				cand.SetPtEtaPhiM(5.,Muon_standeta[Map.begin()->second],Muon_standphi[Map.begin()->second],0.);
				cand2.SetPtEtaPhiM(5.,Muon_standeta[z],Muon_standphi[z],0.);
				if (cand.DeltaR(cand2)<0.3) v.emplace_back(cand.DeltaR(cand2));
			}
		}
	}
	return v;
}

RVec<float> deltargood(RVec<float> &goodgeneta, RVec<float> &goodgenphi, RVec<float> &Muon_standeta, RVec<float> &Muon_standphi, RVec<bool> &Muon_isStandalone){
	RVec<float> v;
	for (auto j=0U; j < goodgeneta.size(); ++j) {
		if (!((goodgeneta[j]>0.5)&&(goodgeneta[j]<0.7))) continue;
		std::map<float,int> Map;
		for (auto i=0U;i < Muon_standeta.size(); ++i) {
			if (!Muon_isStandalone[i]) continue;
			TLorentzVector cand, cand2;
			cand.SetPtEtaPhiM(5.,goodgeneta[j],goodgenphi[j],0.);
			cand2.SetPtEtaPhiM(5.,Muon_standeta[i],Muon_standphi[i],0.);
			Map.insert({cand.DeltaR(cand2),i});
		}
		if (Map.size()==0) continue;
		if (Map.begin()->first<0.3) {
			for (auto z=0U; z < Muon_standeta.size(); ++z) {
				if (z==Map.begin()->second) continue;
				if (!Muon_isStandalone[z]) continue;
				TLorentzVector cand, cand2;
				cand.SetPtEtaPhiM(5.,Muon_standeta[Map.begin()->second],Muon_standphi[Map.begin()->second],0.);
				cand2.SetPtEtaPhiM(5.,Muon_standeta[z],Muon_standphi[z],0.);
				if (cand.DeltaR(cand2)<0.3) v.emplace_back(cand.DeltaR(cand2));
			}
		}
	}
	return v;
}

void globalefficiency(const char* name) {
	gStyle->SetOptStat(0);
	RDataFrame df("demo/Events",name);
	auto d1=df.Define("goodgenpt",goodgenvalue,{"GenPart_pt","GenPart_postFSRLepIdx1","GenPart_postFSRLepIdx2","GenPart_eta","GenPart_phi","GenPart_status"}).Define("goodgeneta",goodgenvalue,{"GenPart_eta","GenPart_postFSRLepIdx1","GenPart_postFSRLepIdx2","GenPart_eta","GenPart_phi","GenPart_status"}).Define("goodgenphi",goodgenvalue,{"GenPart_phi","GenPart_postFSRLepIdx1","GenPart_postFSRLepIdx2","GenPart_eta","GenPart_phi","GenPart_status"}).Define("goodgenidx",goodgenidx,{"GenPart_pt","GenPart_postFSRLepIdx1","GenPart_postFSRLepIdx2","GenPart_eta","GenPart_phi","GenPart_status"}).Define("goodmuonbool",goodmuonboolglobal1,{"goodgeneta", "goodgenphi", "goodgenidx", "Muon_genPartIdx", "Muon_isGlobal","Muon_standeta","Muon_standphi","Muon_eta","Muon_phi","Muon_isStandalone"}).Define("nmuon",count,{"goodgeneta", "goodgenphi", "goodgenidx", "Muon_genPartIdx", "Muon_isGlobal","Muon_standeta","Muon_standphi","Muon_eta","Muon_phi","Muon_pt","Muon_standpt","goodgenpt","Muon_standValidCSChits","Muon_standValidRPChits"});
	d1=d1.Define("goodtrackpt",goodtrack,{"goodgenpt","Track_eta","Track_phi","goodgeneta","goodgenphi","Track_chi2","Track_originalAlgo","Track_quality"}).Define("goodtracketa",goodtrack,{"goodgeneta","Track_eta","Track_phi","goodgeneta","goodgenphi","Track_chi2","Track_originalAlgo","Track_quality"}).Define("goodtrackphi",goodtrack,{"goodgenphi","Track_eta","Track_phi","goodgeneta","goodgenphi","Track_chi2","Track_originalAlgo","Track_quality"}).Define("goodtrackrealeta",goodtrackreal,{"Track_eta","Track_eta","Track_phi","goodgeneta","goodgenphi","Track_chi2","Track_originalAlgo","Track_quality"}).Define("goodtrackrealphi",goodtrackreal,{"Track_phi","Track_eta","Track_phi","goodgeneta","goodgenphi","Track_chi2","Track_originalAlgo","Track_quality"});
	d1=d1.Define("goodtrack",isgoodtrack,{"goodtrackrealeta","goodtrackrealphi","Muon_standeta","Muon_standphi","Muon_isStandalone"});
	d1=d1.Define("goodstandpt",goodmuon,{"goodgenpt","goodgeneta","goodgenphi","Muon_standeta","Muon_standphi","Muon_isStandalone"}).Define("goodstandeta",goodmuon,{"goodgeneta","goodgeneta","goodgenphi","Muon_standeta","Muon_standphi","Muon_isStandalone"}).Define("goodstandphi",goodmuon,{"goodgenphi","goodgeneta","goodgenphi","Muon_standeta","Muon_standphi","Muon_isStandalone"}).Define("goodmuonidx",goodmuonidx,{"goodgeneta","goodgenphi","Muon_standeta","Muon_standphi","Muon_isStandalone"}).Define("deltargood",deltargood,{"goodgeneta","goodgenphi","Muon_standeta","Muon_standphi","Muon_isStandalone"}).Define("deltarcritical",deltarcritical,{"goodgeneta","goodgenphi","Muon_standeta","Muon_standphi","Muon_isStandalone"});
	d1=d1.Define("goodstand",Goodstand,{"goodmuonidx","Muon_isGlobal","Muon_eta","Muon_phi","Muon_standeta","Muon_standphi","Muon_isStandalone"});
	auto histo1 = d1.Histo2D({"histo1", "", 48, -2.4, 2.4, 9, 25., 60.}, "goodgeneta", "goodgenpt", "goodmuonbool");
	auto counter = d1.Histo2D({"counter", "", 48, -2.4, 2.4, 9, 25., 60.}, "goodgeneta", "goodgenpt","nmuon");
	auto histo2 = d1.Histo2D({"histo2", "", 48, -2.4, 2.4, 9, 25., 60.}, "goodgeneta", "goodgenpt");
	auto histo3 = d1.Histo2D({"histo3", "", 48, -2.4, 2.4, 9, 25., 60.}, "goodtracketa", "goodtrackpt", "goodtrack");
	auto histo4 = d1.Histo2D({"histo4", "", 48, -2.4, 2.4, 9, 25., 60.}, "goodtracketa", "goodtrackpt");
	auto histo5 = d1.Histo2D({"histo5", "", 48, -2.4, 2.4, 9, 25., 60.}, "goodstandeta", "goodstandpt", "goodstand");
	auto histo6 = d1.Histo2D({"histo6", "", 48, -2.4, 2.4, 9, 25., 60.}, "goodstandeta", "goodstandpt");
	auto histoA = d1.Histo1D({"HistoA", "", 100, 0., 0.3}, "deltarcritical");
	auto histoB = d1.Histo1D({"HistoB", "", 100, 0., 0.3}, "deltargood");
//	TFile* output=new TFile("output.root","RECREATE");
//	output->cd();
	TH2D* Histo1=(TH2D*)histo1.GetPtr()->Clone(), *Histo2=(TH2D*)histo2.GetPtr()->Clone(), *Histo3=(TH2D*)histo3.GetPtr()->Clone(), *Histo4=(TH2D*)histo4.GetPtr()->Clone(), *Histo5=(TH2D*)histo5.GetPtr()->Clone(), *Histo6=(TH2D*)histo6.GetPtr()->Clone(), *Counter=(TH2D*)counter.GetPtr()->Clone();
	TH1D *HistoA=(TH1D*)histoA->Clone(), *HistoB=(TH1D*)histoB->Clone();
	double cont1=Histo1->GetBinContent(20,4), cont2=Histo2->GetBinContent(20,4), cont3=Histo3->GetBinContent(20,4), cont4=Histo4->GetBinContent(20,4), cont5=Histo5->GetBinContent(20,4), cont6=Histo6->GetBinContent(20,4);
	double cont1_2=Histo1->GetBinContent(13,4), cont2_2=Histo2->GetBinContent(13,4), cont3_2=Histo3->GetBinContent(13,4), cont4_2=Histo4->GetBinContent(13,4), cont5_2=Histo5->GetBinContent(13,4), cont6_2=Histo6->GetBinContent(13,4);
	Counter->Divide(Histo2);
	Histo1->Divide(Histo2);
	Histo3->Divide(Histo4);
	Histo5->Divide(Histo6);
	TH2D* product=(TH2D*)Histo3->Clone();
	product->Multiply(Histo5);
	TCanvas *c1=new TCanvas();
	c1->cd();
	c1->Draw();
	Histo1->SetTitle("P(global&&#DeltaR_{variables}<0.3|gen)");
//	Histo1->SetTitle("P(global&&gen)/P(standalone&&gen)");
	Histo1->GetXaxis()->SetTitle("GenMuon #eta");
	Histo1->GetYaxis()->SetTitle("GenMuon p_{T} [GeV]");
	Histo1->Draw("colz");
	TCanvas *c3=new TCanvas();
	c3->cd();
	c3->Draw();
	product->SetTitle("P(global&&#DeltaR_{variables}<0.3|standalone)*P(standalone|tracker seeded)");
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
	Histo5->SetTitle("P(global&&#DeltaR_{variables}<0.3|standalone)");
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
	Histo7->GetXaxis()->SetTitle("GenMuon #eta");
	Histo7->GetYaxis()->SetTitle("GenMuon p_{T} [GeV]");
	Histo7->Draw("colz");
	TCanvas *c8=new TCanvas();
	c8->cd();
	c8->Draw();
	Counter->SetTitle("Number of staMuon within 0.3 wrt global muon>1");
//	Histo1->SetTitle("P(global&&gen)/P(standalone&&gen)");
	Counter->GetXaxis()->SetTitle("GenMuon #eta");
	Counter->GetYaxis()->SetTitle("GenMuon p_{T} [GeV]");
	Counter->Draw("colz");
	std::cout<<cont1<<" "<<cont2<<" "<<cont3<<" "<<cont4<<" "<<cont5<<" "<<cont6<<"\n";
	std::cout<<cont1_2<<" "<<cont2_2<<" "<<cont3_2<<" "<<cont4_2<<" "<<cont5_2<<" "<<cont6_2<<"\n";
//	output->Write();
//	output->Close();
	TCanvas *c9=new TCanvas();
	c9->cd();
	c9->Draw();
	HistoA->SetTitle("abs(goodgeneta)<0.1");
	HistoA->GetXaxis()->SetTitle("#DeltaR");
	HistoA->Draw();
	TCanvas *c10=new TCanvas();
	c10->cd();
	c10->Draw();
	HistoB->SetTitle("(goodgeneta[j]>0.5)&&(goodgeneta[j]<0.7)");
	HistoB->GetXaxis()->SetTitle("#DeltaR");
	HistoB->Draw();
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
