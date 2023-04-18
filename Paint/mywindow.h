#include <QApplication> //glowne okno naszej aplikacji
#include <QWidget> //klasa bazowa wszystkich elementow GUI
#include <QPushButton> //klasa do przyciskow
#include <QGroupBox> //grupowanie komponentow
#include <QPainter> //niskopoziomowe  rysowanie
#include <QImage> //reprezentacja obrazu
#include <QMouseEvent> //zdarzenia zwiazane z myszka
#include <cmath> //abs i inne podstawowe funkcje matematyczne
#include <iostream> //standard input output
#include <QSlider> //slidery
#include <QLabel> //czesc graficzna
#include <string> //string
#include <stack> //stos

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

    //deklaracje funkcji
    void czysc();
    void rysuj1();
    void rysuj2();

    //zmienna przechowuję informacje, czy klawisz myszy jest przytrzymany
    bool isPressed = false;

    //deklaracje slotow, czyli funkcji wywolywanych po wystapieniu zdarzen zwiazanych z GUI
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

//klasa point przechowuje wspolrzedne punktu
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

