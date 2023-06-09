#include "mywindow.h"
#include <QVBoxLayout>

//konstruktor
MyWindow::MyWindow(QWidget *parent) : QWidget(parent)
{
    // Ustawiamy tytul okna
    setWindowTitle("basic Paint");

    //rozmiar okna
    resize(800,700);

    //wymiary obrazka i wspolrzedne obrazka
    szer = 600;
    wys = 600;
    poczX = 25;
    poczY = 25;

    fig = 1; //domyslnie niech rysuje okregi
    figura = "okrag";

    img = new QImage(szer,wys,QImage::Format_RGB32); //aktualny obraz
    img2 = new QImage(szer,wys,QImage::Format_RGB32); //potrzebne do przechowywania kopii obrazu

    //pokazuje aktualna figure
    label2 = new QLabel(this);
    label2->setText("figura: " + QString::fromStdString(figura));

    //slider
    slider = new QSlider(Qt::Horizontal, this);
    slider->setRange(3, 30); // zakres slidera
    slider->setTickPosition(QSlider::TicksBothSides);
    slider->setTickInterval(1);
    int n = 3;

    //pokazuje aktualna wartosc n
    label = new QLabel(this);
    label->setText("n: " + QString::number(n));

    //grupa elementow
    grupa = new QGroupBox("Menu",this);

    //wymiary grupy i jej polozenie wzgledem glownego okna
    grupa->setGeometry(QRect(poczX+szer+poczX,poczY,800-szer-2*poczX,poczY+300));

    //layout
    QVBoxLayout *boxLayout = new QVBoxLayout;

    //tworze przyciski
    cleanButton = new QPushButton("Czysc");
    exitButton = new QPushButton("Wyjscie");
    drawButton = new QPushButton("Rysuj");
    circleButton = new QPushButton("Okrag");
    lineButton = new QPushButton("Linia");
    elipseButton = new QPushButton("Elipsa");

    fillButton = new QPushButton("wypelnij kolorem");

    //dodaje przyciski do layutu
    boxLayout->addWidget(cleanButton);
    boxLayout->addWidget(exitButton);
    boxLayout->addWidget(lineButton);
    boxLayout->addWidget(circleButton);
    boxLayout->addWidget(drawButton);
    boxLayout->addWidget(elipseButton);
    boxLayout->addWidget(fillButton);

    boxLayout->addWidget(label2);
    boxLayout->addWidget(slider);
    boxLayout->addWidget(label);


    // Dodajemy layout do grupy
    grupa->setLayout(boxLayout);

    //lacze buttony
    connect(exitButton,SIGNAL(clicked()),qApp,SLOT(quit()));
    connect(cleanButton,SIGNAL(clicked()),this,SLOT(slot_czysc()));
    connect(circleButton, SIGNAL(clicked()), this,SLOT(toCirc()));
    connect(lineButton, SIGNAL(clicked()), this,SLOT(toLine()));
    connect(fillButton, SIGNAL(clicked()), this, SLOT(toFill()));
    connect(elipseButton, SIGNAL(clicked()), this, SLOT(toElipse()));
    connect(drawButton, SIGNAL(clicked()), this, SLOT(toDraw()));

    //i slidera
    connect(slider, &QSlider::valueChanged, this, &MyWindow::updateN);
}

void MyWindow::toCirc() {
    fig = 1; //1 - oznacza okrag
    figura = "okrag";
    label2->setText("figura: " + QString::fromStdString(figura));
    update();
}

void MyWindow::toLine() {
    fig = 2; //2 - oznacza linie
    figura = "linia";
    label2->setText("figura: " + QString::fromStdString(figura));
    update();
}

void MyWindow::toElipse() {
    fig = 3; //3 - oznacza elipse
    figura = "elipsa";
    label2->setText("figura: " + QString::fromStdString(figura));
    update();
}

void MyWindow::toDraw() {
    fig = 5; //5 - oznacza rysowanie
    figura = "rysowanie";
    label2->setText("figura: " + QString::fromStdString(figura));
    update();
}

void MyWindow::toFill() {
    fig = 4; //4 - oznacza wypelnienie
    figura = "wypelnienie";
    label2->setText("figura: " + QString::fromStdString(figura));
    update();
}

void MyWindow::updateN(int value) {
    n = value;
    label->setText("n: " + QString::number(n));
    update();
}

//destruktor
MyWindow::~MyWindow()
{
    delete img;
    delete img2;
}

// Funkcja "odmalowujaca" komponent idk czy przydatna
void MyWindow::paintEvent(QPaintEvent*)
{
    //obiekt klasy QPainter pozwala nam rysowac na komponentach
    QPainter p(this);

    //rysuje obrazek "img" w punkcie (poczX,poczY)
    p.drawImage(poczX,poczY,*img);
}


//funkcja wywolana po nacisnieciu przycisku "Czysc"
void MyWindow::slot_czysc()
{
    czysc();
    update();
}

//funkcja powoduje wyczyszczenie ekranu
void MyWindow::czysc()
{
    // Wskaznik za pomoca, ktorego bedziemy modyfikowac obraz
    unsigned char *ptr;

    //funkcja "bits()" zwraca wskaznik do pierwszego piksela danych
    ptr = img->bits();

    int i,j;

    // Przechodzimy po wszystkich wierszach obrazu
    for(i=0; i<wys; i++)
    {
        // Przechodzimy po pikselach danego wiersza
        // W kazdym wierszu jest "szer" pikseli (tzn. 4 * "szer" bajtow)
        for(j=0; j<szer; j++)
        {
            ptr[szer*4*i + 4*j]=0;
            ptr[szer*4*i + 4*j + 1] = 0;
            ptr[szer*4*i + 4*j + 2] = 0;
        }
    }

    copy();
    update();
}

//funjcha wywolana po nacisnieciu przycisku myszy
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

    unsigned char *ptr;
    ptr = img->bits();

    if(fig == 4)
        floodFill(startX, startY);

    // Sprawdzamy czy klikniecie nastapilo w granicach rysunku
    if((x>=0)&&(y>=0)&&(x<szer)&&(y<wys))
    {
        colorPixel(x, y); //koloruje pixel klikniety

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

    // Sprawdzamy czy klikniecie nastapilo w granicach rysunku
    //700 i 800 wielkosc okna
    if ((x>=0)&&(y>=0)&&(x<800)&&(y<700) && startX != event->x() && startY != event->y())
    {
            if(fig == 5)
                colorPixel(x, y);
            else
                paste();
            if(fig == 2)
                drawLine(startX, startY, x, y);
            else if(fig == 1)
                drawCircle(startX, startY, x, y);
            else if(fig == 3)
                drawElipse(startX, startY, x, y);
            else if(fig == 4)
                    floodFill(startX, startY);

    }
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

           if(fig == 2)
               drawLine(startX, startY, x, y);
           else if(fig == 1)
               drawCircle(startX, startY, x, y);
           else if(fig == 3)
               drawElipse(startX, startY, x, y);
           else if(fig == 5)
               colorPixel(x, y);

           copy();
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

void MyWindow::colorPixel(int x, int y)
{
    unsigned char *ptr;
    ptr = img -> bits();
    if ((y >= 0) && (y < wys) && (x >= 0) && (x < szer))
    {
        ptr[szer*4*y + 4*x] = 255;
        ptr[szer*4*y + 4*x + 1] = 255;
        ptr[szer*4*y + 4*x + 2] = 255;
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
                colorPixel((int)floor(x+0.5), y);
            }
        }
        else
        {
            for(int y = y1; y <= y0; y++)
            {
                float x = x1;
                colorPixel((int)floor(x+0.5), y);
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
                    colorPixel(x, (int)floor(y+0.5));
               }
           }
           else
           {
               if(y0 <= y1)
               {
                   for(int y = y0; y <= y1; y++)
                    {
                        float x = (y - b)/a;
                        colorPixel((int)floor(x+0.5), y);
                    }
                }
               else
               {
                   for(int y = y1; y <= y0; y++)
                    {
                        float x = (y - b)/a;
                        colorPixel((int)floor(x+0.5), y);
                    }
               }
           }
       }
    // Odswiezamy komponent
    update();
}

//funkca rysuje okrag
void MyWindow::drawCircle(int x0, int y0, int x1, int y1)
{
    float r = sqrt((x1 - x0) * (x1 - x0) + (y1 - y0) * (y1 - y0));
    float a = r / sqrt(2) + 1;
    float y = 0;

    if(isPressed)
    {
        for(int x = 0; x <= a; x++)
        {
            y = sqrt(r*r - x*x);
            colorPixel(x + x0, y + y0);
            colorPixel(y + x0, x + y0);
            colorPixel(x + x0, -y + y0);
            colorPixel(y + x0, -x + y0);
            colorPixel(-x + x0, y + y0);
            colorPixel(-y + x0, x + y0);
            colorPixel(-x + x0, -y + y0);
            colorPixel(-y + x0, -x + y0);
        }
    }

    // Odswiezamy komponent
    update();
}

//funkcja rysuje elipse
void MyWindow::drawElipse(int x0, int y0, int x1, int y1)
{
    float a, b;
    a = abs(x1 - x0);
    b = abs(y1 - y0);
    float an = 2 * M_PI / n;

    float x , y = y0;
    for(int i = 0; i < n; i++)
    {
        x = x0 + a * cos(i * an);
        y = y0 + b * sin(i * an);

        drawLine(x, y, x0 + a * cos((i+1) * an), y0 + b * sin((i+1) * an));
    }
}

//funkcja wypelnia figure kolorem
void MyWindow::floodFill(int x, int y)
{
    unsigned char *ptr;
    ptr = img->bits();

    std::stack<Point> S;

    Point punkt(x, y);
    S.push(punkt); //dodaje poczatkoy element na stos

    Point p;
    while(!S.empty())
    {
        p = S.top();
        S.pop();

        if(ptr[szer*4*p.y + 4*p.x] == 0 && ptr[szer*4*p.y + 4*p.x + 1] == 0 && ptr[szer*4*p.y + 4*p.x + 2] == 0 && (p.y > 0) && (p.y < wys-1) && (p.x > 0) && (p.x < szer-1))
        {
            ptr[szer*4*p.y + 4*p.x] = 250;
            ptr[szer*4*p.y + 4*p.x + 1] = 12;
            ptr[szer*4*p.y + 4*p.x + 2] = 34;

            S.push(Point(p.x, p.y + 1));
            S.push(Point(p.x, p.y - 1));
            S.push(Point(p.x + 1, p.y));
            S.push(Point(p.x - 1, p.y));
        }
    }
    update();
}
