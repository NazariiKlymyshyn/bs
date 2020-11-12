#include "headers/server.h"

/**
 *primary creation and initialization, "linking" of the corresponding slots and signals.
 */
Server::Server(QObject * parent): QTcpServer(parent) {
  connect( & server_socket, SIGNAL(error(QAbstractSocket::SocketError)),
    this, SLOT(tcpError(QAbstractSocket::SocketError)));
  connect( & server_socket, SIGNAL(readyRead()),
    this, SLOT(tcpReady()));
  server_socket.setSocketOption(QAbstractSocket::KeepAliveOption, true);
}

void Server::tcpReady() {
  this -> array = server_socket.read(server_socket.bytesAvailable());
}

void Server::tcpError(QAbstractSocket::SocketError error) {
  QMessageBox::warning((QWidget * ) this -> parent(), tr("Error"), tr("TCP error: %1").arg(server_socket.errorString()));
}

bool Server::start_listen(int port_no) {
  if (!this -> listen(QHostAddress::Any, port_no)) {
    QMessageBox::warning((QWidget * ) this -> parent(), tr("Error!"), tr("Cannot listen to port %1").arg(port_no));
  } else
    return true;
}

Server::~Server() {
  server_socket.disconnectFromHost();
  server_socket.waitForDisconnected();
}

void Server::incomingConnection(int descriptor) {
  if (!server_socket.setSocketDescriptor(descriptor)) {
    QMessageBox::warning((QWidget * ) this -> parent(), tr("Error!"), tr("Socket error!"));
    return;
  }

}
