void angolo_solido(){
  
long double pi = 3.1415926;
// Contatore numero di successi Ns
long double contatore = 0;
// Distanza scintillatori esterni
long double h = 6.5;
// Numero punti totali generati
long double n = 1000;

for ( int i = 0; i<n ; i++ )
	{
	// Generazione uniforme sul piano dello scintillatore
	TRandom3 * r1 = new TRandom3(0);
	long double x = r1->Uniform (0.,1.);
	x = x *7.4 - 3.7; // Coordinata x
	TRandom3 * r2 = new TRandom3(0);
	long double y = r2->Uniform (0.,1.);
	y = y *7.4 - 3.7; // Coordinata y
	// Generazione isotropa su una sfera della direzione di volo
	TRandom3 * r3 = new TRandom3(0);
	long double rand1 = r3->Uniform (0.,1.);
	long double phi = 2* pi * rand1 ; // Angolo phi
	TRandom3 * r4 = new TRandom3(0);
	long double rand2 = r4->Uniform (0.,1);
	long double theta = acos (1 - rand2 ); // Angolo theta
	// Calcolo dell ’ intersezione tra direzione  di volo e
	// piano dell ’ ultimo scintillatore ( il suo centro é
	// assunto come riferimento del sistema )
	long double x1 = x + h * tan ( theta ) * cos ( phi ); // Coordinata x
	long double y1 = y + h * tan ( theta ) * sin ( phi ); // Coordinata y
	double theta_deg = theta*180/pi;
	
	// Condizione di accettazione di un evento
	if ( x1 <3.7 && x1 > -3.7 && y1 < 3.7 && y1 > -3.7)
	  {
		contatore = contatore + 1;
	  }
	// cout << " theta = " << theta_deg << " degrees " << endl;  
	}
 
// Calcolo dell ’ angolo solido come prodotto tra accettanza
// e 4 pi e relativo errore
long double frac = contatore/n;
long double angolo_solido = frac*4*pi;
long double errore = 4* pi * sqrt ( contatore * (1 - contatore/n ))/n ;
// Stampa del risultato
cout << frac << endl ;
cout << " Monte Carlo " << angolo_solido << setw(15) << setprecision(15) << " +- " << errore << setw(15) << setprecision(15) << endl;
 
}
