#ifndef SERVER_H
#define SERVER_H


#include <QObject>
#include "includes.h"

class Server : public QTcpServer
{
    Q_OBJECT
public:
    explicit Server(QObject *parent = 0);
        ~Server();
        QTcpSocket server_socket; //an object that displays the connection status of two players
         QByteArray array; //means for storing all information received and transmitted over the network
public slots:
         ///////////////////////////////////////////////////////////////
        // function overloads inherent in each class inherited from QTcpServer //
        ///////////////////////////////////////////////////////////////
    void tcpReady();
    void tcpError( QAbstractSocket::SocketError error );
    bool start_listen(int port_no);
protected:

    void incomingConnection( int descriptor );
};

#endif // SERVER_H
