#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <dlib/image_io.h>
#include <sys/time.h>
#include <QFileInfo>
#include <Qurl>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    init();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::init()
{
    if (initialized)
        return;

    m_detector = dlib::get_frontal_face_detector();
    initialized = true;
}

void MainWindow::DetectFace()
{
    QString filename("test.bmp");
    struct timeval begin, end;
    double elapse;

#ifdef ANDROID
    QFile file(":/" + filename);
    file.copy(filename);
#else
    const QString appAssetsUrlPrefix = QString::fromLatin1("file://%1").arg(QCoreApplication::applicationDirPath());
    filename = appAssetsUrlPrefix + "/" + filename;
    QUrl url(filename);
    filename = url.toLocalFile();
#endif

    dlib::array2d<unsigned char> img;
    dlib::load_image(img, filename.toStdString());

    gettimeofday(&begin, NULL);
    std::vector<dlib::rectangle> dets = m_detector(img);
    gettimeofday(&end, NULL);
    elapse = 1000.0 * (end.tv_sec - begin.tv_sec) + (end.tv_usec - begin.tv_usec)/1000.0;
    ui->label->setText(QString::number(elapse) + "ms");
}
