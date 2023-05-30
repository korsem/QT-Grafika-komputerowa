// Plik naglowkowy klasy MyWindow
// Obiekt tej klasy to glowne okno naszej aplikacji

#include <QApplication>
#include <QWidget>
#include <QPushButton>
#include <QGroupBox>
#include <QPainter>
#include <QImage>
#include <QMouseEvent>
#include <cmath>
#include <iostream>
#include <QSlider>
#include <QLabel>
#include <string>
#include <stack>
#include <bits/stdc++.h>

struct warstwa{
    QImage *img = nullptr;
    double g;
    int t;
};


// MyWindow jest podklasa klasy QWidget
class MyWindow : public QWidget
{
    // Makro ktore musimy dodac jezeli definiujemy wlasne sygnaly lub sloty
    Q_OBJECT
public:
    MyWindow(QWidget *parent = 0);

    // Deklaracja destruktora
    ~MyWindow();

private:

    QLabel *label;
    QLabel *label2;

    //slider tryb
    QSlider *trybSlider;
    int t;
    std::string tryb;

    //slidery slider
    QSlider *Slider;
    double g, g2, g3;

    //slider photo
    QSlider *photoSlider;
    std::string photo;
    int p;

    // Pole przechowujace obrazek
    QImage *img;
    QImage *img1;
    QImage *img2;
    QImage *img3;
    QImage *resultImage;
    
    // Grupa przyciskow
    QGroupBox *grupa1, *grupa2;
    
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

    bool isPressed = false;

    struct warstwa warstwy[3];

private slots:
    void colorPixel(int x, int y, int R, int G, int B);
    int getPixel(int x, int y, QImage *img, char kolor);
    void updateP(int value);
    void updateT(int value);
    void updateBlend(int value);
    void blend();
//    void copy();
//    void paste();
    void paintEvent(QPaintEvent*);
};


