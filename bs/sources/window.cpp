#include "headers/window.h"

#include "ui_window.h"

#include "headers/includes.h"

#include "headers/player.h"

/**
 *primary creation and initialization
 */
Window::Window(QWidget * parent): QMainWindow(parent), ui(new Ui::Window) {
  setMouseTracking(true);
  ui -> setupUi(this);
  this -> ChangeBackground("MainWindowBackGround"), setWindowTitle("Battle Ships"), setFixedSize(892, 419), statusBar() -> setSizeGripEnabled(false);
}

Window::~Window() {
  delete ui;
}

void Window::paintEvent(QPaintEvent * Event = nullptr) {
  Q_UNUSED(Event);
  if (0 == Player::GetPlayerInstance().GetCurrentProgramState(Player::GetPlayerInstance()).main_menu) {
    QPainter painter(this);
    std::array < Player::Unit, 100 > bf = Player::GetPlayerInstance().GetWholeFieldState(Player::GetPlayerInstance().GetFirstPlayerField(Player::GetPlayerInstance()));
    for (uint_fast16_t i = 0, t = 0, r = 0; i < 100; ++i) {
      if (bf[i].ship) painter.drawImage(38 * (++r), 37 + (38 * t), Player::GetPlayerInstance().GetImage("FilledUnit"));
      else if (bf[i].miss) painter.drawImage(38 * (++r), 37 + (38 * t), Player::GetPlayerInstance().GetImage("MissedShot"));
      else if (bf[i].destroyed) painter.drawImage(38 * (++r), 37 + (38 * t), Player::GetPlayerInstance().GetImage("Destroyed"));
      else if (bf[i].hit) painter.drawImage(38 * (++r), 37 + (38 * t), Player::GetPlayerInstance().GetImage("Damaged"));
      else ++r;
      if (0 == (r % 10)) {
        r = 0;
        t++;
      }
    }

    if (Player::GetPlayerInstance().GetCurrentProgramState(Player::GetPlayerInstance()).opponent_field_set) {
      bf = Player::GetPlayerInstance().GetWholeFieldState(Player::GetPlayerInstance().GetSecondPlayerField(Player::GetPlayerInstance()));
      for (uint_fast16_t i = 0, t = 0, r = 0; i < 100; ++i) {
        if (bf[i].miss) painter.drawImage(466 + 38 * (++r), 37 + (38 * t), Player::GetPlayerInstance().GetImage("MissedShot"));
        else if (bf[i].hit) painter.drawImage(460 + 38 * (++r), 37 + (38 * t), Player::GetPlayerInstance().GetImage("Damaged"));
        else if (bf[i].destroyed) painter.drawImage(466 + 38 * (++r), 37 + (38 * t), Player::GetPlayerInstance().GetImage("Destroyed"));
        else ++r;
        if (0 == (r % 10)) {
          r = 0;
          t++;
        }
      }
    }
  }

  /**
   *the part of the program responsible for the transition of the application                      
   *to its initial state due to the receipt of a signal about the victory of one of the players 
   */
  Player::CurrentProgramState st {
    1,
    0,
    0,
    0,
    0,
    0,
    0
  };
  switch (Player::GetPlayerInstance().IsWin()) {
  case 1:
    QMessageBox::about(this, "", "Enemy won");
    this -> ui -> stackedWidget -> setCurrentIndex(0);
    Player::GetPlayerInstance().SetPlayerState(Player::GetPlayerInstance(), st);
    this -> ChangeBackground("MainWindowBackGround");
    break;
  case -1:
    QMessageBox::about(this, "", "You won");
    this -> ui -> stackedWidget -> setCurrentIndex(0);
    Player::GetPlayerInstance().SetPlayerState(Player::GetPlayerInstance(), st);
    this -> ChangeBackground("MainWindowBackGround");
    break;
  }
}

void Window::mousePressEvent(QMouseEvent * Event) {
  this -> COORD = Event -> pos(), Player::GetPlayerInstance().ProcessClick(this -> COORD.x(), this -> COORD.y()), update();
}

void Window::on_pushButton_3_clicked() {
  Player::CurrentProgramState st {
    0,
    1,
    0,
    0,
    0,
    0,
    0
  };
  Player::GetPlayerInstance().SetPlayerState(Player::GetPlayerInstance(), st);
  this -> ui -> stackedWidget -> setCurrentIndex(1), ChangeBackground("MatchWindowBackGround");
}

void Window::on_pushButton_2_clicked() {
  this -> close();
}

void Window::on_pushButton_5_clicked() {
  const Player::CurrentProgramState st = {
    0,
    0,
    0,
    0,
    1,
    0,
    0
  };
  if (Player::GetPlayerInstance().checkMyField()) Player::GetPlayerInstance().SetPlayerState(Player::GetPlayerInstance(), st), Player::GetPlayerInstance().ProcessClick(0, 0), update();
  else /*to do message box*/;
}

/**
 *handed down code in a separate implementation of the means 
 *of changing the background image in the widget 
 */
void Window::ChangeBackground(QString PathToImage) {
  QPixmap bkgnd = QPixmap::fromImage(Player::GetPlayerInstance().GetImage(PathToImage));
  QPalette palette;
  palette.setBrush(QPalette::Background, bkgnd);
  bkgnd = bkgnd.scaled(this -> size(), Qt::IgnoreAspectRatio);
  this -> setPalette(palette);
}

void Window::on_pushButton_clicked() {
  this -> server.start_listen(5000);
}

void Window::on_pushButton_4_clicked() {

}

void Window::on_pushButton_6_clicked() {
  if (Player::GetPlayerInstance().Player::GetCurrentProgramState(Player::GetPlayerInstance()).ships_placing) {
    Player::GetPlayerInstance().AutoShipsPlace();
    this -> update();
  }
}
