
#include "SetNiceStyle.C"
#include "LoadOscProb.C"

#ifndef __CINT__
#include "PremModel.h"
#include "PMNS_Fast.h"
bool isCINT = false;
#else
bool isCINT = true;
#endif

void DrawFixedCosT_zoa(double zoamod = 0.4991, double zoaref = 0.4661, double cosT = -1, bool isNuBar = false){

  // Load some nice styles
  SetNiceStyle();

  // Load OscProb library
  if(isCINT) LoadOscProb();

  // Probability Calculator
  OscProb::PMNS_Fast p;
  OscProb::PMNS_Fast p_mod;
  // Set neutrino or antineutrino
  p.SetIsNuBar(isNuBar);
  p_mod.SetIsNuBar(isNuBar);
  
  // PREM Model
  OscProb::PremModel prem;
  prem.SetLayerZoA(1, zoaref);

  OscProb::PremModel prem_mod;
  prem_mod.SetLayerZoA(1, zoamod);

  // set proper names
  string nprem = "FeNi";
  string nmod = "Pyrolite";
  
  // Fill path for cosT
  prem.FillPath(cosT);
  prem_mod.FillPath(cosT);
  
  // Give path to calculator
  p.SetPath(prem.GetNuPath());
  p_mod.SetPath(prem_mod.GetNuPath());

  cout << zoamod << endl;
  if (zoamod < 0){
    p_mod.SetPath(OscProb::NuPath(2*6368));
    nmod = "Vacuum";
  }

  // cout << &p << '\t' << &p_mod << endl;

  // cout << prem.GetNuPath()[30].zoa << '\t' << prem_mod.GetNuPath()[30].zoa << endl;

  // cout << p.Prob(1,1, 1) << '\t' << p_mod.Prob(1,1, 1) << endl;
  
  // Make some histograms
  int nbins = 1000;
  double xmin = 1;
  double xmax = 20;
  
  TH1D* hMuMu_NH = new TH1D("","",nbins,xmin,xmax);
  TH1D* hEMu_NH = new TH1D("","",nbins,xmin,xmax);
  TH1D* hMuMu_IH = new TH1D("","",nbins,xmin,xmax);
  TH1D* hEMu_IH = new TH1D("","",nbins,xmin,xmax);

  TH1D* hMuMu_NH_mod = new TH1D("","",nbins,xmin,xmax);
  TH1D* hEMu_NH_mod = new TH1D("","",nbins,xmin,xmax);
  TH1D* hMuMu_IH_mod = new TH1D("","",nbins,xmin,xmax);
  TH1D* hEMu_IH_mod = new TH1D("","",nbins,xmin,xmax);

  double dm31 = 2.5e-3;

  // Fill histograms
  for(int i=1; i<=nbins; i++){
  
    double energy = hMuMu_NH->GetBinCenter(i);
    
    // Set NH
    p.SetDm(3, dm31);
    p_mod.SetDm(3, dm31);
    
    // Fill NH 
    hMuMu_NH->SetBinContent(i, p.Prob(1,1, energy));
    hEMu_NH->SetBinContent(i, p.Prob(1,0, energy));

    hMuMu_NH_mod->SetBinContent(i, p_mod.Prob(1,1, energy));
    hEMu_NH_mod->SetBinContent(i, p_mod.Prob(1,0, energy));
    
    // cout << p.Prob(1,1, energy) << '\t' << p_mod.Prob(1,1, energy) << endl;

    // Set IH 
    p.SetDm(3, -dm31 + 7.52e-5);
    p_mod.SetDm(3, -dm31 + 7.52e-5);
    
    // Fill IH
    hMuMu_IH->SetBinContent(i, p.Prob(1,1, energy));
    hEMu_IH->SetBinContent(i, p.Prob(1,0, energy));

    hMuMu_IH_mod->SetBinContent(i, p_mod.Prob(1,1, energy));
    hEMu_IH_mod->SetBinContent(i, p_mod.Prob(1,0, energy));
  
  }
  
  // Set some nice styles
  SetHist(hMuMu_NH, kBlue);
  SetHist(hEMu_NH, kRed);
  SetHist(hMuMu_NH_mod, kBlue);
  SetHist(hEMu_NH_mod, kRed);

  SetHist(hMuMu_IH, kBlue);
  SetHist(hEMu_IH, kRed);
  SetHist(hMuMu_IH_mod, kBlue);
  SetHist(hEMu_IH_mod, kRed);
  
  // Make modified dashed
  hMuMu_NH_mod->SetLineStyle(7);
  hEMu_NH_mod->SetLineStyle(7);
  hMuMu_IH_mod->SetLineStyle(7);
  hEMu_IH_mod->SetLineStyle(7);
  
  // Set axis titles
  if(isNuBar) hMuMu_NH->SetTitle(";Neutrino Energy (GeV);P(#bar{#nu}_{x}#rightarrow#bar{#nu}_{#mu})");
  else        hMuMu_NH->SetTitle(";Neutrino Energy (GeV);P(#nu_{#mu}#rightarrow#nu_{x})");
  
  // Set y range
  hMuMu_NH->GetYaxis()->SetRangeUser(0,1);

  // Make a long canvas
  TCanvas* c1 = MakeLongCanvas();
  // Top margin for legend
  c1->SetTopMargin(0.2);
  
  // Draw everything
  if (!isNuBar){
    hMuMu_NH->Draw("C");
    hEMu_NH->Draw("curv same");
    hMuMu_NH_mod->Draw("curv same");
    hEMu_NH_mod->Draw("curv same");
  }
  else{
    hMuMu_IH->Draw("curv");
    hEMu_IH->Draw("curv same");
    hMuMu_IH_mod->Draw("curv same");
    hEMu_IH_mod->Draw("curv same");
  }

  // Print cosT in canvas
  // MiscText(0.8, 0.88, 0.04, TString::Format("cos#theta_{z} = %0.1f", cosT) );
  
  TLegend* leg = new TLegend(0.3,0.7,0.7,.97);
  string title_NMH = isNuBar ? "IH" : "NH";
  leg->SetHeader(TString::Format("%s, cos#theta_{z} = -1", title_NMH.c_str()), "C");
  leg->SetNColumns(2);
  
  if(!isNuBar){

    leg->AddEntry(hMuMu_NH, TString::Format("P(#bar{#nu}_{#mu}#rightarrow#bar{#nu}_{#mu}) %s" , nprem.c_str()), "l");
    leg->AddEntry(hMuMu_NH_mod, TString::Format("P(#bar{#nu}_{#mu}#rightarrow#bar{#nu}_{#mu}) %s" , nmod.c_str()), "l");
    leg->AddEntry(hEMu_NH, TString::Format("P(#bar{#nu}_{#mu}#rightarrow#bar{#nu}_{e}) %s" , nprem.c_str()), "l");
    leg->AddEntry(hEMu_NH_mod, TString::Format("P(#bar{#nu}_{#mu}#rightarrow#bar{#nu}_{e}) %s" , nmod.c_str()), "l");

  }
  else{
  
    leg->AddEntry(hMuMu_IH, TString::Format("P(#nu_{#mu}#rightarrow#nu_{#mu}) %s" , nprem.c_str()), "l");
    leg->AddEntry(hMuMu_IH_mod, TString::Format("P(#nu_{#mu}#rightarrow#nu_{#mu}) %s" , nmod.c_str()), "l");
    leg->AddEntry(hEMu_IH, TString::Format("P(#nu_{#mu}#rightarrow#nu_{e}) %s" , nprem.c_str()), "l");
    leg->AddEntry(hEMu_IH_mod, TString::Format("P(#nu_{#mu}#rightarrow#nu_{e}) %s" , nmod.c_str()), "l");
  
  }
  
  SetLeg(leg);
  gPad->SetGrid();
  gPad->SetLogx();
  hMuMu_NH->GetXaxis()->SetMoreLogLabels();
  
  leg->Draw();
  c1->Print(TString::Format("Oscillation_cosT%0.0f_%s_%s.png", cosT, title_NMH.c_str(), nmod.c_str() ) );
  
}
