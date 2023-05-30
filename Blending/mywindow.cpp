#include "mywindow.h"
#include <QVBoxLayout>

MyWindow::MyWindow(QWidget *parent) : QWidget(parent)
{
    setWindowTitle("Blending");

    resize(800,700);

    szer = 600;
    wys = 600;
    poczX = 25;
    poczY = 25;

    img1 = new QImage("Graphics/butterfly.jpg");
    img2 = new QImage("Graphics/geese.jpg");
    img3 = new QImage("Graphics/widoczek.jpg");
    resultImage = new QImage(szer,wys,QImage::Format_RGB32);
    img = new QImage(szer,wys,QImage::Format_RGB32);

    grupa1 = new QGroupBox("wybierz tryb",this);

    // wymiary tej grupy i jej polozenie wzgledem glownego okna
    grupa1->setGeometry(QRect(poczX+szer+poczX,poczY,800-szer-2*poczX,poczY+100));

    // nowy layout pionowy
    QVBoxLayout *boxLayout1 = new QVBoxLayout;

    // Tworze slidery
    trybSlider = new QSlider(Qt::Horizontal, this);
    trybSlider->setRange(1, 5); // zakres slidera
    trybSlider->setTickPosition(QSlider::TicksBothSides);
    trybSlider->setTickInterval(1);
    trybSlider->setStyleSheet("QSlider::handle:horizontal { background-color: blue; }");

    photoSlider = new QSlider(Qt::Horizontal, this);
    photoSlider->setRange(0, 2); // zakres slidera
    photoSlider->setTickPosition(QSlider::TicksBothSides);
    photoSlider->setTickInterval(1);
    photoSlider->setStyleSheet("QSlider::handle:horizontal { background-color: pink; }");

    label2 = new QLabel(this);
    label2->setText("photo: " + QString::fromStdString("choose photo"));

    boxLayout1->addWidget(photoSlider);
    boxLayout1->addWidget(label2);

    // dodaje slidery do layoutu
    boxLayout1->addWidget(trybSlider);
    label = new QLabel(this);
    label->setText("tryb: " + QString::fromStdString("wybierz tryb"));

    // ustawim wartosci poczatkowe dla warstwy
    warstwy[0].img = img1;
    warstwy[1].img = img2;
    warstwy[2].img = img3;

    for(int i = 0; i < 3; i++)
    {
        warstwy[i].t = 1;
        warstwy[i].g = 1;
    }

    // Dodaje layout do grupy
    grupa1->setLayout(boxLayout1);

    //tworze slider glowny
    grupa2 = new QGroupBox("przenikanie",this);

    //troche na sztywno ale trudno
    grupa2->setGeometry(QRect(poczX+szer+poczX,poczY+170,800-szer-2*poczX,poczY+100));

    Slider = new QSlider(Qt::Horizontal, this);
    Slider->setRange(0, 100); // zakres slidera w stopniach
    Slider->setTickPosition(QSlider::TicksBothSides);
    Slider->setTickInterval(1);
    Slider->setStyleSheet("QSlider::handle:horizontal { background-color: yellow; }");

    // Dodajemy slidery do layoutu
    QVBoxLayout *boxLayout2 = new QVBoxLayout;

    boxLayout2->addWidget(Slider);

    // Dodajemy layout do grupy
    grupa2->setLayout(boxLayout2);

    boxLayout1->addWidget(label);

    //lacze slidery
    connect(Slider, &QSlider::valueChanged, this, &MyWindow::updateBlend);
    connect(trybSlider, &QSlider::valueChanged, this, &MyWindow::updateT);
    connect(photoSlider, &QSlider::valueChanged, this, &MyWindow::updateP);
    update();
}

// Definicja destruktora
MyWindow::~MyWindow()
{
    delete img;
    delete img2;
    delete img3;
    delete resultImage;
}

void MyWindow::paintEvent(QPaintEvent*)
{
    QPainter p(this);
    p.drawImage(poczX,poczY,*resultImage);
    update();
}

void MyWindow::updateP(int value)
{
    if(value == 0)
    {
        photo = "butterfly";
        p = 0;
    }
    else if(value == 1)
    {
        photo = "geese";
        p = 1;
    }
    else if(value == 2)
    {
        photo = "view";
        p = 2;
    }

    blend();
    Slider->setValue((int)(warstwy[p].g * 100));
    trybSlider->setValue(warstwy[p].t);
    label2->setText("photo: " + QString::fromStdString(photo));
    update();
}

void MyWindow::updateT(int value)
{
    if(value == 1)
    {
        tryb = "normal mode";
        warstwy[p].t = 1;
    }
    else if(value == 2)
    {
        tryb = "multiply mode";
        warstwy[p].t = 2;
    }
    else if(value == 3)
    {
        tryb = "score mode";
        warstwy[p].t = 3;
    }
    else if(value == 4)
    {
        tryb = "difference mode";
        warstwy[p].t = 4;
    }
    else if(value == 5)
    {
        tryb = "overlay mode";
        warstwy[p].t = 5;
    }
    label->setText("tryb: " + QString::fromStdString(tryb));
    Slider->setValue((int)(warstwy[p].g * 100));
    blend();
    update();
}

void MyWindow::updateBlend(int value)
{
    trybSlider->setValue(warstwy[p].t);
    warstwy[p].g = 0.01 * value;

    blend();
}

int MyWindow::getPixel(int x, int y, QImage *img, char kolor)
{
    unsigned char *ptr;
    ptr = img->bits();

    if(kolor == 'r')
    {
        return ptr[600*4*y + 4*x + 2];
    }
    else if(kolor == 'g')
    {
        return ptr[600*4*y + 4*x + 1];
    }
    else if(kolor == 'b')
    {
        return ptr[600*4*y + 4*x];
    }
}

void MyWindow::blend()
{
for(int i = 2; i >= 0; i--){
double g = warstwy[i].g;
img = warstwy[i].img;
    if(warstwy[i].t == 1) // normal mode
    {
        for (int i = 0; i < wys; i++)
        {
            for (int j = 0; j < szer; j++)
            {
                int R = g * getPixel(i, j, img, 'r') + (1 - g) * getPixel(i, j, resultImage, 'r');
                int G = g * getPixel(i, j, img, 'g') + (1 - g) * getPixel(i, j, resultImage, 'g');
                int B = g * getPixel(i, j, img, 'b') + (1 - g) * getPixel(i, j, resultImage, 'b');
                colorPixel(i, j, R, G, B);
            }
        }
    }

    else if(warstwy[i].t == 2) // multiply mode
    {
        for (int i = 0; i < wys; i++)
        {
            for (int j = 0; j < szer; j++) //jak mam tego g uzaleznić?
                {
                    int R = g * ((getPixel(i, j, img, 'r')*getPixel(i, j, resultImage, 'r'))>>8) + (1 - g) * getPixel(i, j, resultImage, 'r');
                    int G = g * ((getPixel(i, j, img, 'g')*getPixel(i, j, resultImage, 'g'))>>8) + (1 - g) * getPixel(i, j, resultImage, 'g');
                    int B = g * ((getPixel(i, j, img, 'b')*getPixel(i, j, resultImage, 'b'))>>8) + (1 - g) * getPixel(i, j, resultImage, 'b');
                    colorPixel(i, j, R, G, B);
                }
        }
    }
    else if(warstwy[i].t == 3) // screen mode f (a, b) = 255 − ((255 − a) ∗ (255 − b) >> 8)
    {
        for (int i = 0; i < wys; i++)
                {
                    for (int j = 0; j < szer; j++)
                    {
                        int R = g * (255 - (((255 - getPixel(i, j, img, 'r')) * (255 - getPixel(i, j, resultImage, 'r'))) >> 8) + (1 - g) * getPixel(i, j, resultImage, 'r'));
                        int G = g * (255 - (((255 - getPixel(i, j, img, 'g')) * (255 - getPixel(i, j, resultImage, 'g'))) >> 8) + (1 - g) * getPixel(i, j, resultImage, 'g'));
                        int B = g * (255 - (((255 - getPixel(i, j, img, 'b')) * (255 - getPixel(i, j, resultImage, 'b'))) >> 8) + (1 - g) * getPixel(i, j, resultImage, 'b'));

                        colorPixel(i, j, R, G, B);
                    }
                }
    }
    else if(warstwy[i].t == 4) //Difference mode f (a, b) = |a − b|
    {
        for (int i = 0; i < wys; i++)
        {
            for (int j = 0; j < szer; j++)
            {
                int R = g * abs(getPixel(i, j, img, 'r')-getPixel(i, j, resultImage, 'r')) + (1 - g) * getPixel(i, j, resultImage, 'r');
                int G = g * abs(getPixel(i, j, img, 'g')-getPixel(i, j, resultImage, 'g')) + (1 - g) * getPixel(i, j, resultImage, 'g');
                int B = g * abs(getPixel(i, j, img, 'b')-getPixel(i, j, resultImage, 'b')) + (1 - g) * getPixel(i, j, resultImage, 'b');
                colorPixel(i, j, R, G, B);
            }
        }
    }


    //    Overlay mode
    //    (a ∗ b) >> 7 dla a < 128
    //    255 − ((255 − a) ∗ (255 − b) >> 7) dla a > 128
    else if(warstwy[i].t == 5)
    {
        int R = 0, G = 0, B = 0;
        for (int i = 0; i < wys; i++)
        {
            for (int j = 0; j < szer; j++)
            {
                if(getPixel(i, j, img, 'r') < 128)
                    R = g * ((getPixel(i, j, img, 'r')*getPixel(i, j, resultImage, 'r'))>>7) + (1 - g) * getPixel(i, j, resultImage, 'r');
                else
                    R = g * (255 - (((255 - getPixel(i, j, img, 'r')) * (255 - getPixel(i, j, resultImage, 'r'))) >> 8) + (1 - g) * getPixel(i, j, resultImage, 'r'));
                if(getPixel(i, j, img, 'g') < 128)
                    G = g * ((getPixel(i, j, img, 'g')*getPixel(i, j, resultImage, 'g'))>>7) + (1 - g) * getPixel(i, j, resultImage, 'g');
                else
                    G = g * (255 - (((255 - getPixel(i, j, img, 'g')) * (255 - getPixel(i, j, resultImage, 'g'))) >> 8) + (1 - g) * getPixel(i, j, resultImage, 'g'));
                if(getPixel(i, j, img, 'b') < 128)
                    B = g * ((getPixel(i, j, img, 'b')*getPixel(i, j, img2, 'b'))>>7) + (1 - g) * getPixel(i, j, resultImage, 'b');
                else
                    B = g * (255 - (((255 - getPixel(i, j, img, 'b')) * (255 - getPixel(i, j, resultImage, 'b'))) >> 8) + (1 - g) * getPixel(i, j, resultImage, 'b'));

                colorPixel(i, j, R, G, B);
            }
        }
    }
    update();
}
}

void MyWindow::colorPixel(int x, int y, int R, int G, int B)
{
    unsigned char *ptr;
    ptr = resultImage -> bits();
    if ((y >= 0) && (y < wys) && (x >= 0) && (x < szer))
    {
        ptr[szer*4*y + 4*x] = B;
        ptr[szer*4*y + 4*x + 1] = G;
        ptr[szer*4*y + 4*x + 2] = R;
    }
    update();
}
