#include <TStyle.h>
#include <TCanvas.h>
#include "TFile.h"
#include "TFitResult.h"

void gain_all(){

  //Numero di punti del file da aprire.
  Int_t n = 3000;                    // guadagno 40 (40) [ 55V n=3045 ;56V n=3011; 58V n=3045; 60V n=3000; 62V n=3000]
  				    // guadagno 40 (46) [54V n=3045; 58 V n= 3040 (scartato); 56V n=3000, 55V n=3045;
  //Vettori ampiezza e conteggio.
  Float_t adc[n], count[n];              //le due variabili

  ifstream in;
  in.open("picchi_luce46_55V_40dB_histo.txt");
  for (Int_t i=0;i<n;i++){
    in >> adc[i] >> count [i];
  }
  in.close();

  TGraph *g = new TGraph(n,adc,count);
  new TCanvas();
  g->Draw("apl");                       //Mette i picchi in un TGraph
  g->SetTitle("Fit Picchi");
  g->GetXaxis()->SetTitle("Channels (ADC)");
  g->GetYaxis()->SetTitle("Counts");

  
  TSpectrum *s = new TSpectrum();                     //classe molto potente che permette ad esempio di cercare i picchi (search)

  int nPoints = g->GetN();
  TH1F *h = new TH1F("h","Selezione Picchi",500,-500,8000);

  for (int i=0;i<nPoints;i++){
    double x,y;
    g->GetPoint(i,x,y);
    h->Fill(x,y);
  }

  new TCanvas();
  h->Draw("hist");
  h->GetXaxis()->SetTitle("Channels (ADC)");
  h->GetYaxis()->SetTitle("Counts");

  float rmin, rmax;
  int irange = 200;
  
  //Cerca i picchi che hanno ampiezza > soglia*massima_ampiezza_nell'istogramma);
  //modificare l'ultimo parametro per togliere i picchi indesiderati! (soglia)
  
  Int_t nPeaks = s->Search(h,1,"",0.01);        		//(46) [54V = 1, 0.01; 55V = 1, 0.1, 56V = 1, 0.1]
  //cout << nPeaks << endl;            			//(40) [58V = 1, 0.1; 60V = 1, 0.4, 62V = 1, 0.1
  Double_t *xpeaks;
  Double_t mean[nPeaks], sigma[nPeaks];
  xpeaks = s->GetPositionX();
  
  gStyle->SetOptFit(1111);  // Mostra i parametri del fit (media, sigma, chi2, ecc.)
  TF1 *gp[nPeaks];
  for (int p=0;p<nPeaks;p++){
    //cout << p << " " << xpeaks[p] << endl;
    rmin = xpeaks[p] - irange;
    rmax = xpeaks[p] + irange;
    gp[p] = new TF1(TString::Format("m%d",p), "gaus", rmin, rmax);
    g->Fit(gp[p],"QR+");
    mean[p] = gp[p]->GetParameter(1);
    sigma[p] = gp[p]->GetParameter(2);
    //cout << p << " " << mean[p] << " " << sigma[p] << endl;
  }
    
 int i, j;
 float temp;
 for(i=0;i<nPeaks;i++) {
  for(j=0;j<nPeaks-i;j++){
   if(mean[j]>mean[j+1]){
     temp=mean[j];
     mean[j]=mean[j+1];
     mean[j+1]=temp;
   }
  }
 }

 int nph = nPeaks - 1;
 double Gdb, Gpsau, cal, avg_diff, avg_gain, diff[nph], gain[nph];
 double tot_gain = 0.;
 double tot_diff = 0.;
 double gain_star = 0.;
 
 cout<<"Enter the Digitizer Gain (db) \n";
 cin >> Gdb;
 Gdb = Gdb/20;
 Gpsau = pow(10,Gdb);
 //cout << Gpsau << endl;
 cal = 39/Gpsau;
 
 for(j=1;j<=nPeaks-1;j++){
   diff[j] = mean[j+1] - mean[j];
   gain[j] = (diff[j]*cal)/(1.6e-4);
   tot_gain = tot_gain + gain[j];
   tot_diff= tot_diff + diff[j];
   cout << j << " " << mean[j] << " " << diff[j] << " "<< "SiPM gain = " << gain[j] << endl;
   
  // cout << "SiPM gain = " << gain << endl;
 }
 avg_diff = tot_diff/nph;
 avg_gain = tot_gain/nph;
 //gain_star = (avg_diff*cal)/(1.6e-4); //guadagno calcolato dalla media delle differenze
 //cout << "Average difference = " << avg_diff << endl;
 cout << "Average SiPM Gain = " << avg_gain << endl;
 //cout << "SiPM Gain (From average differenzìce) = " << gain_star << endl;
 
 
 
 //Calcolo dell'errore sulla media metodo 1
 
double variance = 0.0, std_dev = 0.0, sem = 0.0;
// Calcola la varianza
for(int j = 1; j <= nph; j++) {
    variance += pow(gain[j] - avg_gain, 2);
}
variance /= nph; // Varianza
std_dev = sqrt(variance); // Deviazione standard
// Calcola l'errore standard della media (SEM)
sem = std_dev / sqrt(nph);
//cout << "Standard Deviation = " << std_dev << endl;
cout << "Error on the Average Gain = " << sem << endl;
 
 
//Calcolo dell'errore sulla media metodo 2
//double variance_2 = 0.0, std_dev_2 = 0.0, sem_2 = 0.0, std_dev_3;
// Calcola la varianza
//for(int j = 1; j <= nph; j++) {
//    variance_2 += pow(diff[j] - avg_diff, 2);
//}
//variance_2 /= nph; // Varianza
//std_dev_2 = sqrt(variance_2); // Deviazione standard
//sem_2 = std_dev_2 / sqrt(nph);
 //cout << "Standard Deviation = " << std_dev << endl;
//cout << "Error on the Average Difference = " << sem_2 << endl;
//std_dev_3 = (sem_2*cal)/(1.6e-4);
//cout << "Error on the Average Gain (second method) = " << std_dev_3 << endl;



 //cout << "Number of photons = " << nph << " Average ADC diff = " << avg << endl;

 //cout<<"Enter the Digitizer Gain (db) \n";
 //cin >> Gdb;
 //Gdb = Gdb/20;
 //Gpsau = pow(10,Gdb);
 //cout << Gpsau << endl;
 //cal = 39/Gpsau;
 //gain = (avg*cal)/(1.6e-4);

 //cout << "SiPM gain = " << gain << endl;

}

