#ifndef WINDOW_H
#define WINDOW_H

#include <QMainWindow>

#include "server.h"

namespace Ui {
  class Window;
}

class Window: public QMainWindow {
  Q_OBJECT

  /**
   *means for storing and transmitting information about each
   *click of a player with a mouse in a program window for processing by software
   */
  QPoint COORD = {
    0,
    0
  };

  public:
    explicit Window(QWidget * parent = nullptr);
  ~Window();

  /**
   *function overloads inherent in each class inherited from QMainWindow   
   */
  void paintEvent(QPaintEvent * Event) override;
  void mousePressEvent(QMouseEvent * Event) override;

  /**
   *handed down code in a separate implementation 
   *of the means of changing the background image in the widget   
   */
  void ChangeBackground(QString PathToImage);

  private slots:
    void on_pushButton_3_clicked();

  void on_pushButton_2_clicked();

  void on_pushButton_5_clicked();

  void on_pushButton_clicked();

  void on_pushButton_4_clicked();

  void on_pushButton_6_clicked();

  private:
    Server server;
  Ui::Window * ui;
};

#endif /** WINDOW_H */
