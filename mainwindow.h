#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <dlib/image_processing/frontal_face_detector.h>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void init();
    void DetectFace();

private:
    Ui::MainWindow *ui;
    bool initialized;
    dlib::frontal_face_detector m_detector;
};

#endif // MAINWINDOW_H
