#include <QApplication>
#include <QWidget>
#include <QPushButton>
#include <QGroupBox>
#include <QPainter>
#include <QImage>
#include <QMouseEvent>
#include <cmath>
#include <iostream>
#include <QLabel>
#include <string>
#include <vector>
#include <algorithm>

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
class MyWindow : public QWidget
{
    Q_OBJECT
public:

    MyWindow(QWidget *parent = 0);
    ~MyWindow();

private:

    QLabel *label2;
    QLabel *label;
    int n;

    //ustala w ktorym trybie jest
    int tryb;

    //nazwa trybu
    std::string figura;

    int krzywa;

    // Pole przechowujace obrazek
    QImage *img;
    //Pole przechowująe kopie obrazka
    QImage *img2;

    // Przyciski
    QPushButton *cleanButton;
    QPushButton *exitButton;
    QPushButton *addButton;
    QPushButton *moveButton;

    QPushButton *bezierButton;
    QPushButton *bsklejaneButton;

    // Grupa przyciskow
    QGroupBox *grupa;
    QGroupBox *grupa2;

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

    //Vektor przechowuje moje dodane punkty
    std::vector<Point> punkty;

    //Zmienna przechowuję informacje, czy klawisz myzy jest przytrzymany
    bool isPressed = false;

    // Deklaracje slotow, czyli funkcji wywolywanych
    // po wystapieniu zdarzen zwiazanych z GUI
    // np. klikniecie na przycisk, ruch myszka
private slots:
    void toAdd();
    void toMove();
    void toBezier();
    void toBsklejane();
    void slot_czysc();
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void colorPixel(int x, int y, int r, int g, int b);
    void copy();
    void paste();
    void drawLine(int x1, int y1, int x2, int y2);
    void drawBezier(int x1, int y1, int x2, int y2, int x3, int y3, int x4, int y4, int N);
    void drawAllBezier();
    float Bezier(float P1, float P2, float P3, float P4, float t);
    void drawB(int x1, int y1, int x2, int y2, int x3, int y3, int x4, int y4, int N);
    void drawAllB();
    float Bsklejane(float P1, float P2, float P3, float P4, float t);
    void addPoint(int x, int y);
    void removePoint(int x, int y);
    void paintEvent(QPaintEvent*);
    float distancePoints(Point p1, Point p2);
};


