#include <iostream>
#include <vector>
#include <conio.h>

using namespace std;

// PROJEKT NALEŻY WŁĄCZYĆ PRZY UŻYCIU PLIKU EXE KTÓRY STWORZY SIE PRZY PIERWSZYM ODPALENIU(VSC) LUB KTÓRY ODPALI SIĘ AUTOMATYCZNIE W VISUAL STUDIO(TEN FIOLETOWY)
// PROEJKT PRZEDSTAWIA PROSTĄ GRĘ FPS OPARTĄ NA KLASACH

class Postac;
class Gracz;

class Bron{
    private:
    int dmg;
    int amunicja;
    int maxAmmo;
    bool czyMaAmmo = true;

    public:
    Bron(int dmg, int maxAmmo){
        this->dmg=dmg;
        this->maxAmmo=maxAmmo;
        this->amunicja=maxAmmo;
    }

    void Przeladowanie(){
        amunicja = maxAmmo;
    }

    int getDMG() const{
        return dmg;
    }

    int getAMMO() const{
        return amunicja;
    }
    
    void setAMMO(int ammo){
        amunicja -= ammo;
    }

    bool getCzyMaAmmo() const{
        return czyMaAmmo;
    }

    void setCzyMaAmmo(bool czyMa){
        czyMaAmmo = czyMa;
    }
};

class Postac{
    private:
    int x; // pozycja pozioma na planszy
    int y; // pozycja pionowa na planszy
    int hbwidth; // szerokosc hitboxa
    int hbheight; // wysokosc hitboxa
    int odleglosc; // odleglosc od gracza
    int hp = 100; // hp postaci
    bool trafiony = false; // stan trafienia(dla nastepnego wyswietlenia)
    
    public:
    Postac(int x, int y, int odleglosc){
        this->x=x;
        this->y=y;
        this->odleglosc=odleglosc;
    }

    void hitbox(vector<vector<char>>& plansza){ // tworzy hitbox postaci na planszy
        hbwidth = x+5-odleglosc;
        hbheight = y+6-odleglosc;
        if(odleglosc<5 && odleglosc>0){
        for(int i=x;i<hbwidth;i++){
            for(int j=y;j<hbheight;j++){
                plansza[j][i] = 'X';
                }
            }
        }
        else{
            plansza[y][x] = 'X'; // jezeli przeciwnik jest daleko to robi hitbox jako 1x1
        }
    }

    void ruch(int rx, int ry, int odl, int szerokosc, int wysokosc){    
        // ruszanie na planszy
        odleglosc-=odl;
        x-=rx;
        y-=ry;
        
        //GRANICE MAPY
        if(odleglosc<1) { // odleglosc za mala
            odleglosc = 1;
        }
        if(odleglosc>5) { // odleglosc za duza
            odleglosc = 5;
        }

        hbwidth = x+4-odleglosc; // gdzie konczy sie hitbox x
        hbheight = y+5-odleglosc; // gdzie konczy sie hitbox y

        if(odleglosc == 5){ // prawa granica mapy gdy hitbox jest wielkosci 1x1
            hbwidth = x;
            if(hbwidth>=szerokosc){
            x=szerokosc - 6 + odleglosc;
            }
        }
        if(odleglosc<5){ // prawa granica mapy gdy hitbox jest wiekszy niz 1x1
                if(hbwidth>=szerokosc){
                x = szerokosc - 5 + odleglosc;
                }
        }
        
        if(x<0) { // lewa granica mapy
            x=0;
        }

        if(y<0) { // gorna granica mapy
            y=0;
        }
        
        if(hbheight>=wysokosc){ // dolna granica mapy
            y = wysokosc - 6 + odleglosc;
        }
    }

    void czyTrafiony(int celX, int celY){ // sprawdza czy strzal trafil przeciwnika
        if(celX==x && celY==y && odleglosc == 5 && hp>0){ // sprawdza dla hitboxa 1x1
            trafiony = true;
        }
        else if(celX>=x && celX < hbwidth && celY>=y && celY<hbheight && hp >0){ // sprawdza dla hitboxa wiekszego od 1x1
            trafiony = true;
        }
        else{
            trafiony = false;
        }
    }

    int getOdl() const{
        return odleglosc;
    }
    int getX() const{
        return x;
    }
    int getY() const{
        return y;
    }
    int getHP() const{
        return hp;
    }
    bool getTrafiony() const{
        return trafiony;
    }
    void setHP(int dmg){
        hp-=dmg;
    }
    void setTrafiony(bool trafiony){
        this->trafiony=trafiony;
    }
};

class Gracz{
    private:
    Bron *aktualna;
    Bron karabin;
    Bron pistolet;
    int hp = 100;
    bool trafiony = false;

    public:
    Gracz():karabin(15,30),pistolet(10,10){
        aktualna=&pistolet;
    }

    void Strzal(int szerokosc, int wysokosc, Postac &przeciwnik){ // odejmuje naboj z magazynka i sprawdza czy strzal trafil
        int celX = szerokosc/2; 
        int celY = wysokosc/2;
        
        if(getAktualnyAMMO()>0){
        przeciwnik.czyTrafiony(celX,celY);
        aktualna->setAMMO(1);
        aktualna->setCzyMaAmmo(true);
        }
        else{
            aktualna->setCzyMaAmmo(false);
        }
    }

    void ZmianaBroni(int numerBroni){
        if(numerBroni == 1){
            aktualna = &karabin;
        }
        if(numerBroni == 2){
            aktualna = &pistolet;
        }
    }

    void Przeladowanie(){
        aktualna->Przeladowanie();
    }

    int getHP() const{
        return hp;
    }
    bool getTrafiony() const{
        return trafiony;
    }
    int getAktualnyDMG(){
        return aktualna->getDMG();
    }
    int getAktualnyAMMO(){
        return aktualna->getAMMO();
    }
    bool getAktualnyCzyMaAMMO() const{
        return aktualna->getCzyMaAmmo();
    }

};

class Gra{
    private:
    Postac przeciwnik;
    Gracz gracz;
    int szerokosc;
    int wysokosc;
    bool dziala = true;

    public:
    Gra(int wysokosc, int szerokosc):przeciwnik(2,2,3),gracz(),szerokosc(szerokosc),wysokosc(wysokosc){}

    void WyswietlPlansze(int szerokosc, int wysokosc){ // wyswietla plansze
    
        vector<vector<char>> plansza(wysokosc,vector<char>(szerokosc, '.')); //plansza skladajaca sie z wektorow

        //celownik
        int celX = szerokosc/2; 
        int celY = wysokosc/2;

        przeciwnik.hitbox(plansza); // przeciwnik na planszy
    
        //jezeli przeciwnik jest na tyle daleko ze jego hitbox jest 1x1 to celownik zamienia sie w * gdy jest na nim
        if(przeciwnik.getOdl()>4 && przeciwnik.getX() == celX && przeciwnik.getY() == celY){
            plansza[celY][celX] = '*';
        }
        else{
            plansza[celY][celX] = '+';
        }
    
        //tworzy plansze
        for(const auto& wiersze : plansza) {
            for(const auto& komorki : wiersze) {
                if(komorki == 'X'){
                    cout << komorki << " ";
                }
                else{
                    cout << komorki << " ";
                }
            }
            cout << endl;
        }
    }

    void Przyciski(char input, int szerokosc, int wysokosc){ // wyswietla dostepne przyciski
        int x = 0;
        int y = 0;
        int odleglosc = 0;

        switch(input){
            case 'w':
            y=-1;
            przeciwnik.ruch(x,y,odleglosc,szerokosc,wysokosc);
            break;
            
            case 'a':
            x=-1;
            przeciwnik.ruch(x,y,odleglosc,szerokosc,wysokosc);
            break;
            
            case 'd':
            x=1;
            przeciwnik.ruch(x,y,odleglosc,szerokosc,wysokosc);
            break;
            
            case 's':
            y=1;
            przeciwnik.ruch(x,y,odleglosc,szerokosc,wysokosc);
            break;
            
            case 'u':
            odleglosc=1;
            przeciwnik.ruch(x,y,odleglosc,szerokosc,wysokosc);
            break;
            
            case 'i':
            odleglosc=-1;
            przeciwnik.ruch(x,y,odleglosc,szerokosc,wysokosc);
            break;

            case 'f':
            gracz.Strzal(szerokosc,wysokosc,przeciwnik);
            break;

            case 'r':
            gracz.Przeladowanie();
            break;

            case '1':
            gracz.ZmianaBroni(1);
            break;

            case '2':
            gracz.ZmianaBroni(2);
            break;

            default:
            cout << "podano zly przycisk";
        }
    }

    bool Input(int szerokosc, int wysokosc){ // pobiera przycisk od uzytkownika
        char input;
        cout << "\033[33m" <<"\n w - ruch w gore  a - ruch w lewo  d - ruch w prawo  s - ruch w dol" << "\033[0m";
        cout << "\033[37m" <<"\n u - przybliz  i - oddal \t 1 - karabin  2 - pistolet \t f - strzal " << "\033[0m";
        cout << "\033[35m" <<"\n q - wyjscie" << "\033[0m" << endl;
        input = _getch();

        if(input == 'q' || input == 'Q'){
            return false;
        }
        else{
            Przyciski(input, szerokosc, wysokosc);
            return true;
        }
    }

    void trafionyPrzeciwnik(){
        przeciwnik.setHP(gracz.getAktualnyDMG());
}
    
    void StanPrzeciwnika(){ // wyswietla hp oraz stan trafienia przeciwnika
        if(przeciwnik.getTrafiony() == true){
            trafionyPrzeciwnik();
        }
    
        if(przeciwnik.getHP()>0){
            cout << "\033[31m" << "HP PRZECIWNIKA = " << przeciwnik.getHP();
        }
        else{
            cout << "\033[31m" << "PRZECIWNIK NIE ZYJE ";
        }
        
        if(przeciwnik.getTrafiony() == true){
            cout << "\t przeciwnik zostal trafiony \n";
            przeciwnik.setTrafiony(false);
        }
        else{
            cout << "\n";
        }
    }

    void StanGracza(){
        if(gracz.getHP()>0){
            cout << "\033[32m" << "HP GRACZA = " << gracz.getHP() << "\t Amunicja = " << gracz.getAktualnyAMMO();
            if(gracz.getAktualnyCzyMaAMMO() == false){
                cout << "\t przeladuj lub zmien bron";
            }
        }
        else{
            cout << "\033[36m" << "NIE ZYJESZ ";
            dziala = false;
        }
        
        if(gracz.getTrafiony() == true){
            cout << "\t zostales trafiony \n";
        }
        else{
            cout << "\n";
        }
    }

    void Uruchom(){ // uruchamia cala gre zeby nie robic syfu w main
        while(dziala){
            system("cls");
            WyswietlPlansze(szerokosc,wysokosc);
            StanPrzeciwnika();
            StanGracza();
            dziala = Input(szerokosc, wysokosc);
        }
    }
};

int main(){
    Gra gra(21,21);
    gra.Uruchom();
}