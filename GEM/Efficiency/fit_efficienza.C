#include <TStyle.h>
#include "TFile.h"
#include "TFitResult.h"
#include <TGraph.h>
#include <TF1.h>
#include <TCanvas.h>

void fit_efficienza(){
   //Number of points in the data file
  Int_t n = 13;
  Float_t volt[n], efficienza[n], volt_err[n], efficienza_err[n];

  //Read input file
  ifstream in;                		
  in.open("efficienza.txt");
  for (Int_t i=0;i<n;i++){
    in >> volt[i] >> volt_err[i] >> efficienza[i] >> efficienza_err[i];
    std::cout << "Volt: " << volt[i] << ", efficienza: " << efficienza[i] << ", Volt_err: " << volt_err[i] << ", efficienza_err: " << efficienza_err[i] << std::endl; //stampo i dati
  }
  in.close();

  TGraphErrors *g = new TGraphErrors(n, volt, efficienza, volt_err, efficienza_err);
  		
  g->SetTitle("Efficiency vs Voltage");
  g->SetMarkerStyle(0);			//0 = no marker
  g->SetMarkerSize(0);   
  g->GetXaxis()->SetTitle("Voltage (V)");
  g->GetYaxis()->SetTitle("Efficiency");
  g->SetLineColor(kBlack);  
  g->SetLineWidth(2);     
  
  g->GetXaxis()->SetRangeUser(3250,3800);
  g->GetYaxis()->SetRangeUser(0,1.6);
    
    //Definition of the fit function
    TF1 *sigmoid = new TF1("sigmoid", "[0] / (1 + exp(-[1]*(x-[2]))) + [3]", 3250, 3800);

    //Initial guess
    sigmoid->SetParameters(1, 0.0035,3500 , 0); // Amplitude, Slope, Midpoint, traslazione
    TFitResultPtr fitResult1 = g->Fit(sigmoid, "S");  
    
    //Visualize the fit
    TCanvas *c1 = new TCanvas("c1", "Sigmoid Fit");
    g->Draw("AP");
    sigmoid->Draw("same");
    gStyle->SetOptFit(1111);  // Mostra dettagli sul fit nel box delle statistiche
    
    //Evaluate the efficiency on the plateau:
    float midpoint = 3650;
    float y_at_midpoint = sigmoid->Eval(midpoint); // Evaluate the function at x0
    printf("Value of the sigmoid at the midpoint (x = %f): Eff = %f \n", midpoint, y_at_midpoint);
    
    // Create a marker to highlight the evaluated point
TMarker *marker = new TMarker(midpoint, y_at_midpoint, 15); // Style 20 is a filled circle
marker->SetMarkerColor(kBlack); // Set marker color
marker->SetMarkerStyle(kDot); //set marker style
marker->SetMarkerSize(2);   // Set marker size
marker->Draw("same");         // Draw on the same canvas

// Optionally, add a text label to annotate the point
TLatex *label = new TLatex(midpoint, y_at_midpoint+0.2, Form("Eff = %.3f", y_at_midpoint));
label->SetTextSize(0.03);
label->SetTextColor(kBlack);
label->Draw("same");

}
    
