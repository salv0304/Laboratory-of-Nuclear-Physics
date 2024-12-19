#include <iostream>
#include <cmath>
#include <iomanip>
#include <vector>
#include "TRandom3.h"
#include "TH1F.h"
#include "TCanvas.h"

// Funzione per generare theta secondo la distribuzione specificata
double gen_theta(TRandom3 &randGen) {
    while (true) {
        double y = randGen.Uniform(0, 1);       // y uniformemente distribuito in [0, 1]
        double x = randGen.Uniform(0, M_PI_2); // x uniformemente distribuito in [0, pi/2]
        if (y < std::pow(std::cos(x), 2)) {    // Criterio di accettazione
            return x;
        }
    }
}

void monteCarlo_simulation(int n = 1000, int m = 1000) {
    long double pi = 3.141592653589793;
    long double h = 7.7;
    TRandom3 randGen(0);
    
    std::vector<long double> results;
    results.reserve(m);

    for (int j = 0; j < m; j++) {
        long double contatore_doppia = 0;
        long double contatore_tripla = 0;

        for (int i = 0; i < n; i++) {
            // Generazione uniforme sul piano dello scintillatore
            long double x = randGen.Uniform(-3.7, 3.7);
            long double y = randGen.Uniform(-3.7, 3.7);

            // Generazione isotropa su una sfera della direzione di volo
            long double phi = 2 * pi * randGen.Uniform(0., 1.);
            long double theta = gen_theta(randGen); // Usare la nuova funzione

            // Calcolo delle coordinate sul piano dell'ultimo scintillatore
            long double x1 = x + h * tan(theta) * cos(phi);
            long double y1 = y + h * tan(theta) * sin(phi);
            
            // Calcolo delle coordinate sul piano della GEM
            long double xGEM = x + h * tan(theta) * cos(phi)/2; //La GEM è posta a metà altezza
            long double yGEM = y + h * tan(theta) * sin(phi)/2;

            // Condizione di accettazione (coincidenza doppia)
            if (x1 < 3.7 && x1 > -3.7 && y1 < 3.7 && y1 > -3.7) {
                contatore_doppia += 1;
		        // Condizione di accettazione (coincidenza tripla)
		        if (xGEM < 3.7 && xGEM > -3.7 && yGEM < 3.7 && yGEM > -3.7) {
		            contatore_tripla += 1;
            	}
            }
        }

        // Calcolo dell'angolo solido
        long double frac = contatore_doppia / n;
        long double angolo_solido = frac * 4 * pi;
        results.push_back(angolo_solido);
        //confronto doppia/tripla coincidenza
        if( j %100==0){
        std::cout << "Conteggi doppia =" << contatore_doppia<< " Conteggi tripla =" << contatore_tripla << std::endl;}
    }

    // Plot della distribuzione usando ROOT
    TCanvas *c1 = new TCanvas("c1", "Distribuzione Monte Carlo", 800, 600);
    TH1F *h1 = new TH1F("h1", "Distribuzione angolo solido", 100, 0, 4 * pi);

    for (const auto &val : results) {
        h1->Fill(val);
    }

    h1->GetXaxis()->SetTitle("Angolo Solido (sr)");
    h1->GetYaxis()->SetTitle("Conteggi");
    h1->Draw();
    
    // Fit della distribuzione con una funzione gaussiana
    TFitResultPtr fitResult = h1->Fit("gaus", "S"); // "S" per ottenere il risultato del fit 
    gStyle->SetOptFit(1111);  // Mostra dettagli sul fit nel box delle statistiche
    //TF1 *gaussFit = new TF1("gaussFit", "gaus", 0, 4 * pi);
    //h1->Fit(gaussFit, "R");

    // Mostra i risultati del fit
    //h1->Draw();
    c1->SaveAs("fit_gaussiano.png");
    
    
    //Simulazione con GEM
}

