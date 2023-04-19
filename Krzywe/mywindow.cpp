#include "mywindow.h"
#include <QVBoxLayout>

MyWindow::MyWindow(QWidget *parent) : QWidget(parent)
{
    // Ustawiamy tytul okna
    setWindowTitle("Rysowanie pikseli w Qt");

    //Zmieniamy rozmiar okna
    resize(800,700);

    szer = 600;
    wys = 600;
    poczX = 25;
    poczY = 25;

    tryb = 1; //domyslnie niech dodaje punkty
    figura = "dodawanie";

    krzywa = 1; //domyslne niech rysuje krzywa beziera

    img = new QImage(szer,wys,QImage::Format_RGB32);
    img2 = new QImage(szer,wys,QImage::Format_RGB32);

    //pokazuje aktualna wartosc n
    label = new QLabel(this);
    label->setText("n: " + QString::number(punkty.size()));

    //pokazuje aktualna figure
    label2 = new QLabel(this);
    label2->setText("tryb: " + QString::fromStdString(figura));

    // Tworzymy grupe elementow
    // Drugi parametr konstruktora to rodzic elementu
    grupa = new QGroupBox("Sterowanie",this);

    // Ustawiamy wymiary tej grupy i jej polozenie wzgledem glownego okna
    grupa->setGeometry(QRect(poczX+szer+poczX,poczY,800-szer-2*poczX,poczY+200));

    // Tworzymy nowy layout pionowy
    QVBoxLayout *boxLayout = new QVBoxLayout;
    QVBoxLayout *boxLayout2 = new QVBoxLayout;

    // Tworzymy 4 przyciski
    cleanButton = new QPushButton("Czysc");
    exitButton = new QPushButton("Wyjscie");
    addButton = new QPushButton("add / delete punkt");
    moveButton = new QPushButton("move punkt");

    grupa2 = new QGroupBox("wybor krzywej",this);
    grupa2->setGeometry(QRect(poczX+szer+poczX,2*poczY + 300,800-szer-2*poczX,poczY+100));

    bezierButton = new QPushButton("bezier");
    bsklejaneButton = new QPushButton("b-sklejane");


    // Dodajemy przyciski do layoutu
    boxLayout->addWidget(cleanButton);
    boxLayout->addWidget(exitButton);
    boxLayout->addWidget(addButton);
    boxLayout->addWidget(moveButton);

    boxLayout2->addWidget(bezierButton);
    boxLayout2->addWidget(bsklejaneButton);

    //dodajemy opisik
    boxLayout->addWidget(label2);

    // Dodajemy layout do grupy
    grupa->setLayout(boxLayout);
    grupa2->setLayout(boxLayout2);

    connect(exitButton,SIGNAL(clicked()),qApp,SLOT(quit()));
    connect(cleanButton,SIGNAL(clicked()),this,SLOT(slot_czysc()));
    connect(addButton, SIGNAL(clicked()), this,SLOT(toAdd()));
    connect(moveButton, SIGNAL(clicked()), this,SLOT(toMove()));

    connect(bezierButton, SIGNAL(clicked()), this,SLOT(toBezier()));
    connect(bsklejaneButton, SIGNAL(clicked()), this,SLOT(toBsklejane()));

}

void MyWindow::toAdd() {
    tryb = 1; //1 - oznacza okragtryb dodawania / odejmowania
    label2->setText("lewym przyciskim dodajesz \n prawym odejmujesz");
    update();
}

void MyWindow::toMove() {
    tryb = 2; //2 - oznacza tryb przesuwania punktow
    label2->setText("mozesz przesunac punkt");
    update();
}

void MyWindow::toBezier() {
    krzywa = 1; //1 - oznacza krzywa beziera
    label2->setText("Krzywa beziera");
    update();
}

void MyWindow::toBsklejane() {
    krzywa = 2; //2 - oznacza rysowanie grzywj b-sklejane
    label2->setText("krzywa B-sklejana");
    update();
}

// Definicja destruktora
MyWindow::~MyWindow()
{
    delete img;
    delete img2;
}


// Funkcja "odmalowujaca" komponent
void MyWindow::paintEvent(QPaintEvent*)
{
    QPainter p(this);
    p.drawImage(poczX,poczY,*img);
}

// Funkcja (slot) wywolywana po nacisnieciu przycisku "Czysc" (cleanButton)
void MyWindow::slot_czysc()
{
    czysc();
    update();
}

// Funkcja powoduje wyczyszczenie zamalowanie na czarno
void MyWindow::czysc()
{
    unsigned char *ptr;
    ptr = img->bits();

    for(int i=0; i<wys; i++)
    {
        for(int j=0; j<szer; j++)
        {
            colorPixel(i,j,0,0,0);
        }
    }
    //czyszcze punkty
    n = 0;
    punkty.clear();
}

// Funkcja (slot) wywolywana po nacisnieciu przycisku myszy (w glownym oknie)
void MyWindow::mousePressEvent(QMouseEvent *event)
{
    // Pobieramy wspolrzedne punktu klikniecia
    // Sa to wspolrzedne wzgledem glownego okna
    int x = event->x();
    int y = event->y();

    isPressed = true;

    // Przesuwamy je zeby nie rysowac od brzegu
    x = x - poczX;
    y = y - poczY;

    startX = x; //Przechowuje pozycje klikniecia
    startY = y;


    // Sprawdzamy ktory przycisk myszy zostal klkniety
    if(tryb == 1 && ((x>=0)&&(y>=0)&&(x<szer)&&(y<wys)))
    {
        if(event->button() == Qt::LeftButton)
        {
            //dodaje punkt
            addPoint(x, y);
            if(krzywa == 1)
                drawAllBezier();
            else
                drawAllB();
        }
        else if(event->button() == Qt::RightButton)
        {
            //usuwa punkt
            removePoint(x, y);
            if(krzywa == 1)
                drawAllBezier();
            else
                drawAllB();
        }
    }

    // Odswiezamy komponent
    update();

}

void MyWindow::mouseMoveEvent(QMouseEvent *event)
{
    int x = event->x();
    int y = event->y();

    x = x - poczX;
    y = y - poczY;

    isPressed = true;

    //moge tu dac zeby bylo dynamiczne poruszane sie i rysowanie krzywych
   /* if ((x>=0)&&(y>=0)&&(x<800)&&(y<700) && startX != event->x() && startY != event->y())
    {

    }*/
    update();
}

void MyWindow::mouseReleaseEvent(QMouseEvent *event)
{
    if (isPressed)
    {
           int x = event->x();
           int y = event->y();
           x = x - poczX;
           y = y - poczY;

           if(tryb == 2)
           {
               Point klik(startX, startY);
               if(punkty.empty())
               {
                   std::cout << "jest pusto wiec nie zmodyfikuje";
               }
               else
               {
                   float min = distancePoints(klik, punkty[0]);
                   int minI = 0;
                   // szukam ktory indeks ma najmniejsza odleglosc od klikniecia
                   for(int i = 1; i < punkty.size(); i++)
                   {
                       if(distancePoints(punkty[i], klik) < min)
                       {
                           minI = i;
                           min = distancePoints(punkty[i], klik);
                       }
                   }
                   colorPixel(punkty[minI].x, punkty[minI].y, 0, 0, 0);

                   colorPixel(x, y, 255, 255, 0);

                   //podmieniam w vectorze stary punt na nowa
                   punkty[minI].x = x;
                   punkty[minI].y = y;

                   if(krzywa == 1)
                       drawAllBezier();
                   else
                       drawAllB();

                   update();
               }

           }

           isPressed = false;
    }
}

void MyWindow::paste() //funkcja wklejająca skopiowany obraz
{
    unsigned char *ptr;
    unsigned char *cp;
    ptr = img->bits();
    cp = img2->bits();
    int szer = img->width();
    int wys = img->height();

    int i,j;
    if(isPressed)
    for(i=0;i<wys;i++)
    {
            for(j=0;j<szer ;j++)
            {
                    ptr[szer*4*i + 4*j] = cp[szer*4*i + 4*j];
                    ptr[szer*4*i + 4*j + 1] = cp[szer*4*i + 4*j + 1];
                    ptr[szer*4*i + 4*j + 2] = cp[szer*4*i + 4*j + 2];
            }
    }
}

void MyWindow::copy() //funkcja kopiująca obecny obraz
{
    unsigned char *ptr;
    unsigned char *cp;
    ptr = img->bits();
    cp = img2->bits();
    int szer = img->width();
    int wys = img->height();

    int i,j;
    if(isPressed)
    for(i=0;i<wys;i++)
    {
            for(j=0;j<szer ;j++)
            {
                    cp[szer*4*i + 4*j] = ptr[szer*4*i + 4*j];
                    cp[szer*4*i + 4*j + 1] = ptr[szer*4*i + 4*j + 1];
                    cp[szer*4*i + 4*j + 2] = ptr[szer*4*i + 4*j + 2];
            }
    }
}

void MyWindow::colorPixel(int x, int y, int r, int g, int b)
{
    unsigned char *ptr;
    ptr = img -> bits();
    if ((y >= 0) && (y < wys) && (x >= 0) && (x < szer))
    {
        ptr[szer*4*y + 4*x] = r;
        ptr[szer*4*y + 4*x + 1] = b;
        ptr[szer*4*y + 4*x + 2] = g;
    }
    update();
}

void MyWindow::drawLine(int x0, int y0, int x1, int y1)
{
    float a = 1, b = 0;

    isPressed = true;
    if (x1 < x0)
    {
        int tempX = x0;
        int tempY = y0;
        x0 = x1;
        y0 = y1;
        x1 = tempX;
        y1 = tempY;
    }
    else if(x1 == x0)
    {
        if(y0 <= y1)
        {
            for(int y = y0; y <= y1; y++)
            {
                float x = x1;
                colorPixel((int)floor(x+0.5), y, 255, 255, 255);
            }
        }
        else
        {
            for(int y = y1; y <= y0; y++)
            {
                float x = x1;
                colorPixel((int)floor(x+0.5), y, 255, 255, 255);
            }
        }
    }

    a = ((y1 - y0) / (float)(x1 - x0));
    b = y0 - a * x0;

    // Sprawdzamy czy klikniecie nastapilo w granicach rysunku
    if (isPressed)
       {
           if(abs(a) <= 1)
           {
               for(int x = x0; x <= x1; x++)
               {
                    float y = a*x + b;
                    colorPixel(x, (int)floor(y+0.5), 255, 255, 255);
               }
           }
           else
           {
               if(y0 <= y1)
               {
                   for(int y = y0; y <= y1; y++)
                    {
                        float x = (y - b)/a;
                        colorPixel((int)floor(x+0.5), y, 255, 255, 255);
                    }
                }
               else
               {
                   for(int y = y1; y <= y0; y++)
                    {
                        float x = (y - b)/a;
                        colorPixel((int)floor(x+0.5), y, 255, 255, 255);
                    }
               }
           }
       }
    // Odswiezamy komponent
    update();
}
float MyWindow::Bezier(float P1, float P2, float P3, float P4, float t) //wzor
{
    return pow(1 - t, 3) * P1 + 3 * pow(1 - t, 2) * t * P2 + 3 * (1-t) * pow(t, 2) * P3 + pow(t, 3) * P4;
}

void MyWindow::drawBezier(int x1, int y1, int x2, int y2, int x3, int y3, int x4, int y4, int N) //N - liczba odcinkow przyblizenia
{
    float t;
    float Px, Py, Qx, Qy;

    for(int i = 0; i < N; i++)
    {
        t = (float)i / N;
        Px = Bezier(x1, x2, x3, x4, t); //rysuje po x
        Py = Bezier(y1, y2, y3, y4, t); //rysuje po y
        Qx = Bezier(x1, x2, x3, x4, t + 1.0/N);
        Qy = Bezier(y1, y2, y3, y4, t + 1.0/N);
        drawLine(Px, Py, Qx, Qy);
    }
}
void MyWindow::drawAllBezier()
{
    //przemalowuje na czarno, zeby narysowac swieza krzywa
    unsigned char *ptr;
    ptr = img->bits();

    int i,j;

    for(i=0; i<wys; i++)
    {
        for(j=0; j<szer; j++)
        {
            colorPixel(i, j, 0, 0, 0);
        }
    }

    for(int i = 0; i < punkty.size(); i+=3)
    {
        if(i+3 < punkty.size())
        {
            drawBezier(punkty[i].x, punkty[i].y, punkty[i+1].x, punkty[i+1].y, punkty[i+2].x, punkty[i+2].y, punkty[i+3].x, punkty[i+3].y, 10);
        }
    }
}


float MyWindow::Bsklejane(float P1, float P2, float P3, float P4, float t) //wzor od Pi-4 do Pi
{
    return (pow(-t, 3) + 3 * pow(t, 2) - 3 * t + 1) / 6 * P1 + (3 * pow(t, 3) - 6 * pow(t, 2) + 4) / 6 * P2 + (-3 * pow(t, 3) + 3 * pow(t, 2) + 3 * t + 1) / 6 * P3 + pow(t, 3) / 6 * P4;
}

void MyWindow::drawB(int x1, int y1, int x2, int y2, int x3, int y3, int x4, int y4, int N) //N - liczba odcinkow przyblizenia
{
    float t;
    float Px, Py, Qx, Qy;

    for(int i = 0; i < N; i++)
    {
        t = (float)i / N;
        Px = Bsklejane(x1, x2, x3, x4, t); //rysuje po x
        Py = Bsklejane(y1, y2, y3, y4, t); //rysuje po y
        Qx = Bsklejane(x1, x2, x3, x4, t + 1.0/N);
        Qy = Bsklejane(y1, y2, y3, y4, t + 1.0/N);
        drawLine(Px, Py, Qx, Qy);
    }
}

void MyWindow::drawAllB()
{
    //przemalowuje na czarno, zeby narysowac swieza krzywa
    unsigned char *ptr;
    ptr = img->bits();

    int i,j;

    for(i=0; i<wys; i++)
    {
        for(j=0; j<szer; j++)
        {
            colorPixel(i, j, 0, 0, 0);
        }
    }

    for(int i = 3; i < punkty.size(); i++)
    {
        drawB(punkty[i-3].x, punkty[i-3].y, punkty[i-2].x, punkty[i-2].y, punkty[i-1].x, punkty[i-1].y, punkty[i].x, punkty[i].y, 15);
    }
}

void MyWindow::addPoint(int x, int y)
{
    colorPixel(x, y, 255, 255, 0);
    punkty.push_back(Point(x, y));
    std::cout << punkty.size() << " ";
    label->setText("n: " + QString::number(punkty.size()));
        update();
}

void MyWindow::removePoint(int x, int y)
{
    Point klik(x, y);
    if(punkty.empty())
    {
        std::cout << "jest pusto wiec nie usune";
    }
    else
    {
        float min = distancePoints(klik, punkty[0]);
        int minI = 0;
        // szukam ktory indeks ma najmniejsza odleglosc od klikniecia
        for(int i = 1; i < punkty.size(); i++)
        {
            if(distancePoints(punkty[i], klik) < min)
            {
                minI = i;
                min = distancePoints(punkty[i], klik);
            }
        }

        // usuwam punkt z vectora punkty
        colorPixel(punkty[minI].x, punkty[minI].y, 0, 0, 0);
        punkty.erase(punkty.begin() + minI);

        label->setText("n: " + QString::number(punkty.size()));
        update();
    }
}

float MyWindow::distancePoints(Point p1, Point p2) //zwraca odleglosc medzy dwoma punktami
{
            return sqrt(pow(p1.x - p2.x, 2) + pow(p1.y - p2.y, 2));
}

//przeciazony operator ==
bool operator==(const Point &p1, const Point &p2)
{
    return (p1.x == p2.x && p1.y == p2.y);
}
