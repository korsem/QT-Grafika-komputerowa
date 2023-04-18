// Plik naglowkowy klasy MyWindow
// Obiekt tej klasy to glowne okno naszej aplikacji

#include <QApplication>

// Dolaczamy plik naglowkowy klasy QWidget,
// Jest to klasa bazowa wszystkich elementow GUI
#include <QWidget>

// Dolaczamy plik naglowkowy klasy QPushButton
// (standardowy przycisk)
#include <QPushButton>

// Dolaczamy plik naglowkowy klasy QGroupBox
// (do grupowania komponentow GUI)
#include <QGroupBox>

// QPainter to klasa umozliwiajaca niskopoziomowe rysowanie
// na elementach GUI
#include <QPainter>

// QImage to klasa pozwalajaca na niezalezna od sprzetu reprezentacje obrazu.
// Pozwala na bezposredni dostep do poszczegolnych pikseli,
// Bedziemy jej uzywali do tworzenia i przechowywania
// naszych rysunkow
#include <QImage>

// QMouseEvent to klasa obslugujaca zdarzenia zwiazane z myszka
// klikniecia, ruch myszka itp.
#include <QMouseEvent>

#include <cmath> //abs()

#include <iostream>

#include <QSlider> //do slidera
#include <QLabel> //czesc graficzna do slidera

#include <string>

#include <stack>
#include <bits/stdc++.h> //do par

// MyWindow jest podklasa klasy QWidget
class MyWindow : public QWidget
{
    // Makro ktore musimy dodac jezeli definiujemy wlasne sygnaly lub sloty
    Q_OBJECT
public:
    // Typowa deklaracja konstruktora w Qt.
    // Parametr "parent" okresla rodzica komponenetu.
    // W przypadku naszej klasy parametr ten wskazuje na null
    // co oznacza, ze komponenet nie ma rodzica, jest to
    // komponenet najwyzszego poziomu
    MyWindow(QWidget *parent = 0);

    // Deklaracja destruktora
    ~MyWindow();

private:

    QLabel *label2;

    //ustala ktora figure ma rysowac
    int fig;

    //nazwa figury
    std::string figura;

    // Pole przechowujace obrazek
    QImage *img;
    //Pole przechowująe kopie obrazka
    QImage *img2;

    // Przyciski
    QPushButton *cleanButton;
    QPushButton *exitButton;
    QPushButton *circleButton;
    QPushButton *lineButton;
    QPushButton *fillButton;
    
    // Grupa przyciskow
    QGroupBox *grupa;
    
    // Pola przechowujace szerokosc i wysokosc rysunku
    // oraz wspolrzedne jego lewego gornego naroznika
    int szer;
    int wys;
    int poczX;
    int poczY;

    int startX; //pole przechowuje punkt kliknięcia
    int startY;

    // Deklaracje funkcji
    void czysc();
    void rysuj1();
    void rysuj2();

    //Zmienna przechowuję informacje, czy klawisz myzy jest przytrzymany
    bool isPressed = false;

    // Deklaracje slotow, czyli funkcji wywolywanych
    // po wystapieniu zdarzen zwiazanych z GUI
    // np. klikniecie na przycisk, ruch myszka
private slots:
    void toCirc();
    void toLine();
    void toFill();
    void slot_czysc();
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void colorPixel(int x, int y);
    void copy();
    void paste();
    void drawLine(int x1, int y1, int x2, int y2);
    void drawCircle(int x0, int y0, int x1, int y1);
    void paintEvent(QPaintEvent*);
    void floodFill(int x, int y);
};

typedef class point
{
public:
    int x, y;
    point()
    {
        this->x = x;
        this->y = y;
    }
    point(int x, int y)
    {
        this->x = x;
        this->y = y;
    }
}Point;

