#include <TStyle.h>
#include "TFile.h"
#include "TFitResult.h"
#include <TGraph.h>
#include <TF1.h>
#include <TCanvas.h>

void fit_rate(){

  //Points of the data file
  Int_t n = 11;
  Float_t volt[n], rate[n], volt_err[n], rate_err[n];

  // Read input file
  ifstream in;                			
  in.open("rate.txt");
  for (Int_t i=0;i<n;i++){
    in >> volt[i] >> volt_err[i] >> rate[i] >> rate_err[i];
    std::cout << "Volt: " << volt[i] << ", Rate: " << rate[i] << ", Volt_err: " << volt_err[i] << ", Rate_err: " << rate_err[i] << std::endl; //stampo i dati
  }
  in.close();

  TGraphErrors *g = new TGraphErrors(n, volt, rate, volt_err, rate_err);
  		
  g->SetTitle("Rate vs Voltage");
  g->SetMarkerStyle(0);			//0 = no marker
  g->SetMarkerSize(0);    
  g->GetXaxis()->SetTitle("Voltage (V)");
  g->GetYaxis()->SetTitle("Rate (Hz)");
  g->SetLineColor(kBlack);  
  g->SetLineWidth(2);     
  
  //Set axis limits
  g->GetXaxis()->SetRangeUser(3100,3700);
  g->GetYaxis()->SetRangeUser(-5,140);
    
    // Fit function definition
    TF1 *sigmoid = new TF1("sigmoid", "[0] / (1 + exp(-[1]*(x-[2]))) + [3]", 3100, 3650);

    //initial guess
    sigmoid->SetParameters(100, 0.1, 3200, 47); 
    // Fit
    TFitResultPtr fitResult1 = g->Fit(sigmoid, "S"); //TFitResultPtr is a useful root item for fitting; "S" prints fit results 
    
    // Visualize the fit
    TCanvas *c1 = new TCanvas("c1", "Sigmoid Fit");
    g->Draw("AP");
    sigmoid->Draw("same");
    gStyle->SetOptFit(0);  //Fit box in the canvas
    
    //Evaluate the rate for the plateau:
    float midpoint = 3500;
    float y_at_midpoint = sigmoid->Eval(midpoint); // Evaluate the function at x0
    printf("Value of the sigmoid at the midpoint (x = %f): Rate = %f Hz\n", midpoint, y_at_midpoint);
    
    // Create a marker to highlight the evaluated point
TMarker *marker = new TMarker(midpoint, y_at_midpoint, 15); // Style 20 is a filled circle
marker->SetMarkerColor(kBlack); // Set marker color
marker->SetMarkerSize(1.5);   // Set marker size
marker->Draw("same");         // Draw on the same canvas

// Add a text label to annotate the point
TLatex *label = new TLatex(midpoint, y_at_midpoint + 10, Form("Rate = %.2f Hz", y_at_midpoint));
label->SetTextSize(0.03);
label->SetTextColor(kBlack);
label->Draw("same");
    
}
    
