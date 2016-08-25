#include <iostream>
#include <cmath>
using namespace std;

enum Pavillon { JollyRogers, CompagnieDuSenegal, CompagnieDOstende };

enum Etat { Intact, Endommage, Coule };

int sq(int x)
{
  return x*x;
}

class Coordonnees
{
public:
  Coordonnees(int un_x, int un_y) : x_(un_x), y_(un_y) {}
  int x() const {
    return x_;
  }
  int y() const {
    return y_;
  }
  void operator+=(Coordonnees const& autre); // à définir plus bas
private:
  int x_;
  int y_;
};

class Navire
{
  /*****************************************************
   * Compléter le code à partir d'ici
   *****************************************************/

protected:
  Coordonnees position_;
  Pavillon pavillon_;
  Etat etat_;
  const int rayon_rencontre = 10;

public:
  Navire(Coordonnees &position, Pavillon pavillon) : position_(position), pavillon_(pavillon), etat_(Etat::Intact) {  }
  Navire(int x, int y, Pavillon pavillon) : position_(Coordonnees(x, y)), pavillon_(pavillon), etat_(Etat::Intact) {  }

  Coordonnees position() const { return position_; }

  void avancer(int de_x, int de_y) {
    if (etat_ != Etat::Coule) {
      position_ += Coordonnees(de_x, de_y);
    }
  }

  void renflouer() {
    etat_ = Etat::Intact;
  }

  virtual string nom() const = 0;

  virtual void attaque(Navire &autre) = 0;

  virtual void replique(Navire &autre) = 0;

  virtual void est_touche() = 0;

  virtual void rencontrer(Navire &autre) {
    if (this->etat_ == Etat::Coule || autre.etat_ == Etat::Coule) { return; }
    if (this->pavillon_ == autre.pavillon_) { return; }

    double d = distance(autre);

    if (d >= rayon_rencontre) { return; }

    attaque(autre);
    autre.replique(*this);

  }

  double distance(Navire const &autre) const;

  void afficher(ostream& out) const;


};

#include <string>

void Coordonnees::operator+=(Coordonnees const& autre)
{
  // à définir ici
  this->x_ += autre.x_;
  this->y_ += autre.y_;
}

double distance(Coordonnees const &a, Coordonnees const &b) {
  return sqrt(sq(a.x() - b.x()) + sq(a.y() - b.y()));
}

double distance(Navire const &a, Navire const &b) {
  return distance(a.position(), b.position());
}

ostream& operator<<(ostream &out, Coordonnees const &a) {
  out << "(" << a.x() << ", " << a.y() << ")";
  return out;
}

ostream& operator<<(ostream &out, Pavillon p) {
  switch (p) {
  case Pavillon::JollyRogers:
    out << "pirate";
    break;
  case Pavillon::CompagnieDuSenegal:
    out << "français";
    break;
  case Pavillon::CompagnieDOstende:
    out << "autrichien";
    break;
  default:
    out << "pavillon inconnu";
  };
  return out;
}

ostream& operator<<(ostream &out, Etat e) {
  switch (e) {
  case Etat::Intact:
    out << "intact";
    break;
  case Etat::Endommage:
    out << "ayant subi des dommages";
    break;
  case Etat::Coule:
    out << "coulé";
    break;
  default: out << "etat inconnu";
  };
  return out;
}

ostream& operator<<(ostream &out, Navire const &n) {
  n.afficher(out);
  return out;
}


double Navire::distance(Navire const &autre) const {
  return ::distance(*this, autre);
}

void Navire::afficher(ostream& out) const {
  out << nom() << " en " << position_ << " battant pavillon " << pavillon_ << ", " << etat_;
}



class Pirate : public virtual Navire {
public:
  Pirate(int x, int y, Pavillon pavillon) : Navire(x, y, pavillon) {  }

  string nom() const override { return "bateau pirate"; }

  virtual void attaque(Navire &autre) override {
    cout << "A l'abordage !" << endl;
    autre.est_touche();
  }
  virtual void replique(Navire &autre) override {
    cout << "Non mais, ils nous attaquent ! On riposte !!" << endl;
    attaque(autre);
  }
  virtual void est_touche() override {
    if (etat_ == Etat::Coule) { return; }
    etat_ = static_cast<Etat>(etat_ + 1);
  }
};

class Marchand : public virtual Navire {
public:
  Marchand(int x, int y, Pavillon pavillon) : Navire(x, y, pavillon) {  }

  string nom() const override { return "navire marchand"; }

  virtual void attaque(Navire &autre) override {
    cout << " On vous aura ! (insultes) " << endl;
  }
  virtual void replique(Navire &autre) override {
    if (etat_ == Etat::Coule) {
      cout << "SOS je coule !" << endl;
    }
    else {
      cout << "Même pas peur !" << endl;
    }
  }
  virtual void est_touche() override {
    etat_ = Etat::Coule;
  }


};

class Felon : public Marchand, public Pirate {
public:
  Felon(int x, int y, Pavillon pavillon) : Navire(x, y, pavillon), Marchand(x, y, pavillon), Pirate(x, y, pavillon) {  }

  string nom() const override { return "navire félon"; }

  virtual void attaque(Navire &autre) override {
    Pirate::attaque(autre);
  }
  virtual void replique(Navire &autre) override {
    Marchand::replique(autre);
  }
  virtual void est_touche() override {
    Pirate::est_touche();
  }

};



/*******************************************
 * Ne rien modifier après cette ligne.
 *******************************************/

void rencontre(Navire& ship1, Navire& ship2)
{
  cout << "Avant la rencontre :" << endl;
  cout << ship1 << endl;
  cout << ship2 << endl;
  cout << "Distance : " << distance(ship1, ship2) << endl;
  ship1.rencontrer(ship2);
  cout << "Apres la rencontre :" << endl;
  cout << ship1 << endl;
  cout << ship2 << endl;
}

int main()
{
  // Test de la partie 1
  cout << "===== Test de la partie 1 =====" << endl << endl;

  // Un bateau pirate 0,0
  Pirate ship1(0, 0, JollyRogers);
  cout << ship1 << endl;

  // Un bateau marchand en 25,0
  Marchand ship2(25, 0, CompagnieDuSenegal);
  cout << ship2 << endl;

  cout << "Distance : " << distance(ship1, ship2) << endl;

  cout << "Quelques déplacements..." << endl;
  cout << "  en haut à droite :" << endl;
  // Se déplace de 75 unités à droite et 10 en haut
  ship1.avancer(75, 10);
  cout << ship1 << endl;

  cout << "  vers le bas :" << endl;
  ship1.avancer(0, -5);
  cout << ship1 << endl;

  cout << endl << "===== Test de la partie 2 =====" << endl << endl;

  cout << "Bateau pirate et marchand ennemis (trop loins) :" << endl;
  rencontre(ship1, ship2);

  cout << endl << "Bateau pirate et marchand ennemis (proches) :" << endl;
  ship1.avancer(-40, -2);
  ship2.avancer(10, 2);
  rencontre(ship1, ship2);

  cout << endl << "Deux bateaux pirates ennemis intacts (proches) :" << endl;
  Pirate ship3(33, 8, CompagnieDOstende);
  rencontre(ship1, ship3);

  cout << endl << "Bateaux pirates avec dommages, ennemis :" << endl;
  rencontre(ship1, ship3);

  cout << endl << "Bateaux marchands ennemis :" << endl;
  Marchand ship4(21, 7, CompagnieDuSenegal);
  Marchand ship5(27, 2, CompagnieDOstende);
  rencontre(ship4, ship5);

  cout << endl << "Pirate vs Felon :" << endl;
  ship3.renflouer();
  Felon ship6(32, 10, CompagnieDuSenegal);
  rencontre(ship3, ship6);

  cout << endl << "Felon vs Pirate :" << endl;
  rencontre(ship6, ship3);

  return 0;
}
