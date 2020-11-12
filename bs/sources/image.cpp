#include "headers/includes.h"

#include "headers/player.h"

/**
 *Retun as QImage object needed for displaying image
 */
const QImage & Player::Image::GetImage(
  const QString & PathToImage
) const {
  QMap < QString, QImage > ::const_iterator it = this -> image.find(PathToImage);
  return it.value();
}

/**
 *Primary initialization of any Image class' objects,. 
 */
Player::Image::Image() {
  /**
   *obtaining the absolute path to the executable file in the file explorer of the user who launched the application
   */
  QString qs = QDir::currentPath().toLocal8Bit().constData();

  /**
   *converting provided QString object in order to divide it to std::string tokens
   */
  std::string token, token2 = qs.toLocal8Bit().constData(), myText = qs.toLocal8Bit().constData();
  std::vector < std::string > strVec;
  std::istringstream iss(myText);

  /**
   *obtaining the absolute path to directory with all game needed  textures
   */
  while (std::getline(iss, token, '/')) strVec.push_back(token);
  strVec.pop_back(), strVec.pop_back(), strVec.erase(strVec.begin(), strVec.begin() + 1);
  token = "";
  for (const auto & entry: strVec) token += ("/" + entry);

  /**
   *directly  forming QMap with all needed objects
   */
  this -> image.insert("MatchWindowBackGround", QImage((token + "/bs/Images/MatchWindowBackGround.png").c_str())),
    image.insert("MainWindowBackGround", QImage((token + "/bs/Images/MainWindowBackGround.png").c_str())),
    image.insert("MissedShot", QImage((token + "/bs/Images/MissedShot.png").c_str())),
    image.insert("FilledUnit", QImage((token + "/bs/Images/FilledUnit.png").c_str())),
    image.insert("Damaged", QImage((token + "/bs/Images/Damaged.png").c_str())),
    image.insert("Destroyed", QImage((token + "/bs/Images/Destroyed.png").c_str()));
}

