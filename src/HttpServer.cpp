#include "Defines.hpp"
#include "HttpServer.hpp"

std::map<SOCKET, AbnfAdapter*> responderList;

socklen_t addrlen;
sockaddr localaddr;
struct sockaddr_in *sadd;
char localipaddress[32];

bool is_new_socket(int sock);
void add_new_socket(int sock);
void remove_new_socket(int sock);

HttpServer::HttpServer()
{
	responderList.clear();
    callback_ptr = NULL;
}

HttpServer::~HttpServer()
{
	stop();	
}

void HttpServer::cleanup()
{
    std::map<SOCKET, AbnfAdapter*>::iterator itr = responderList.begin();

    while(itr != responderList.end())
    {
        SOCKET sk = itr->first;
        AbnfAdapter *ptr = itr->second;

        if(ptr)
        {
            if(!ptr->getResponder()->isConnected())
            {
                delete ptr;
                ptr = NULL;
                responderList[sk]=NULL;
            }
        }

        itr++;
    }
}


void HttpServer::stop()
{
    ::shutdown(listener_socket,2);
    closesocket(listener_socket);
}

void HttpServer::suspend()
{
    writeLog("Callback -> suspend", LOG_INFO);
}

void HttpServer::resume()
{
    writeLog("Callback -> resume", LOG_INFO);
}

void HttpServer::shutdown()
{
    writeLog("Callback -> shutdown", LOG_INFO);
}

void HttpServer::alarm()
{
    writeLog("Callback -> alarm", LOG_INFO);
}

void HttpServer::reset()
{
    writeLog("Callback -> reset", LOG_INFO);
}

void HttpServer::childExit()
{
    writeLog("Callback -> childExit", LOG_INFO);
}

void HttpServer::userdefined1()
{
    writeLog("Callback -> userdefined1", LOG_INFO);
}

void HttpServer::userdefined2()
{
    writeLog("Callback -> userdefined2", LOG_INFO);
}

// RunState HttpServer::run(int port)
// {
//     _listenerSocket = socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);

//     sockaddr_in bindAddr;

//     memset((void*)&bindAddr, 0, sizeof(sockaddr_in));

//     bindAddr.sin_family = AF_INET;
//     bindAddr.sin_port = htons(port);
//     bindAddr.sin_addr.s_addr = htonl(INADDR_ANY);

//     if(bind(_listenerSocket,(sockaddr*)&bindAddr,sizeof(bindAddr)) == SOCKET_ERROR)
//     {
//         return BindFailed;
//     }

//     if(listen(_listenerSocket,5)==SOCKET_ERROR)
//     {
//         return ListenFailed;
//     }

//     while(true)
//     {
//         sockaddr remotehostaddr;
//         memset((void*)&remotehostaddr, 0, sizeof(remotehostaddr));
//         addrlen = sizeof(remotehostaddr);

//         cleanup();

//         SOCKET sock = accept(_listenerSocket,&remotehostaddr,&addrlen);
//         if(sock != INVALID_SOCKET)
//         {
//             ABNFAdapter* abnfadpater = new ABNFAdapter(sock);
//             responderList[sock] = abnfadpater;
//             abnfadpater->startResponder();

//         }
//         else
//         {
//             if ((errno != ECHILD) && (errno != ERESTART) && (errno != EINTR))
//             {
//                break;
//             }
//         }
//     }

//     return NormalExit;
// }

RunState HttpServer::run(int port)
{
    int    socket_index = 1;
    int    len = 1;
    int    rc = 1;
    int    on = 1;

    int    listenerSocket = -1;
    int    responderSocket = -1;
    int    max_sd = 0;

    bool    end_server = false;
    bool    desc_ready = false;
    bool    close_conn = false;

    struct sockaddr_in   addr;
    struct timeval       timeout;
    //struct fd_set        master_set;
    //struct fd_set        working_set;

    fd_set        master_set;
    fd_set        working_set;


    char   buffer[81] = {0};

    /*************************************************************/
    /* Create an AF_INET stream socket to receive incoming       */
    /* connections on                                            */
    /*************************************************************/
    listenerSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (listenerSocket < 0)
    {
        writeLog("socket() failed", LOG_ERROR);
        return StackFailure;
    }

    /*************************************************************/
    /* Allow socket descriptor to be reuseable                   */
    /*************************************************************/
    rc = setsockopt(listenerSocket, SOL_SOCKET,  SO_REUSEADDR, (char *)&on, sizeof(on));
    if (rc < 0)
    {
        writeLog("setsockopt() failed", LOG_ERROR);
        closesocket(listenerSocket);
        return StackFailure;
    }

    /*************************************************************/
    /* Set socket to be non-blocking.  All of the sockets for    */
    /* the incoming connections will also be non-blocking since  */
    /* they will inherit that state from the listening socket.   */
    /*************************************************************/
    rc = ioctl(listenerSocket, FIONBIO, (char *)&on);
    if (rc < 0)
    {
        writeLog("ioctl() failed", LOG_ERROR);
        closesocket(listenerSocket);
        return StackFailure;
    }

    /*************************************************************/
    /* Bind the socket                                           */
    /*************************************************************/
    memset(&addr, 0, sizeof(addr));
    addr.sin_family      = AF_INET;
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    addr.sin_port        = htons(port);
    rc = bind(listenerSocket, (struct sockaddr *)&addr, sizeof(addr));
    if (rc < 0)
    {
        writeLog("bind() failed", LOG_ERROR);
        closesocket(listenerSocket);
        return BindFailed;
    }

    /*************************************************************/
    /* Set the listen back log                                   */
    /*************************************************************/
    rc = listen(listenerSocket, 32);
    if (rc < 0)
    {
        writeLog("listen() failed", LOG_ERROR);
        closesocket(listenerSocket);
        return ListenFailed;
    }

    /*************************************************************/
    /* Initialize the master fd_set                              */
    /*************************************************************/
    FD_ZERO(&master_set);
    max_sd = listenerSocket;
    FD_SET(listenerSocket, &master_set);

    /*************************************************************/
    /* Initialize the timeval struct to 3 minutes.  If no        */
    /* activity after 3 minutes this program will end.           */
    /*************************************************************/
    timeout.tv_sec  = 3 * 60;
    timeout.tv_usec = 0;

    /*************************************************************/
    /* Loop waiting for incoming connects or for incoming data   */
    /* on any of the connected sockets.                          */
    /*************************************************************/
    do
    {
        /**********************************************************/
        /* Copy the master fd_set over to the working fd_set.     */
        /**********************************************************/
        memcpy(&working_set, &master_set, sizeof(master_set));

        /**********************************************************/
        /* Call select() and wait 5 minutes for it to complete.   */
        /**********************************************************/
        writeLog("Waiting on select()", LOG_INFO);
        //rc = select(max_sd + 1, &working_set, NULL, NULL, &timeout);
        rc = select(max_sd + 1, &working_set, NULL, NULL, NULL);

        /**********************************************************/
        /* Check to see if the select call failed.                */
        /**********************************************************/
        if (rc < 0)
        {
            writeLog("select() failed", LOG_ERROR);
            break;
        }

        /**********************************************************/
        /* Check to see if the 5 minute time out expired.         */
        /**********************************************************/
        if (rc == 0)
        {
            writeLog("select() timed out", LOG_ERROR);
            break;
        }

        /**********************************************************/
        /* One or more descriptors are readable.  Need to         */
        /* determine which ones they are.                         */
        /**********************************************************/
        desc_ready = rc;
        for (socket_index=0; socket_index <= max_sd  &&  desc_ready > 0; ++socket_index)
        {
            /*******************************************************/
            /* Check to see if this descriptor is ready            */
            /*******************************************************/
            if (FD_ISSET(socket_index, &working_set))
            {
                /****************************************************/
                /* A descriptor was found that was readable - one   */
                /* less has to be looked for.  This is being done   */
                /* so that we can stop looking at the working set   */
                /* once we have found all of the descriptors that   */
                /* were ready.                                      */
                /****************************************************/
                desc_ready -= 1;

                /****************************************************/
                /* Check to see if this is the listening socket     */
                /****************************************************/
                if (socket_index == listenerSocket)
                {
                    writeLog("Listening socket is readable", LOG_INFO);
                    /*************************************************/
                    /* Accept all incoming connections that are      */
                    /* queued up on the listening socket before we   */
                    /* loop back and call select again.              */
                    /*************************************************/
                    do
                    {
                        /**********************************************/
                        /* Accept each incoming connection.  If       */
                        /* accept fails with EWOULDBLOCK, then we     */
                        /* have accepted all of them.  Any other      */
                        /* failure on accept will cause us to end the */
                        /* server.                                    */
                        /**********************************************/
                        responderSocket = accept(listenerSocket, NULL, NULL);
                        if (responderSocket < 0)
                        {
                            if (errno != EWOULDBLOCK)
                            {
                                writeLog("accept() failed", LOG_ERROR);
                                end_server = true;
                            }
                            break;
                        }

                        /**********************************************/
                        /* Add the new incoming connection to the     */
                        /* master read set                            */
                        /**********************************************/
                        writeLog("New incoming connection", LOG_INFO);
                        FD_SET(responderSocket, &master_set);
                        if (responderSocket > max_sd)
                        {
                            max_sd = responderSocket;
                        }

                        if(is_new_socket(responderSocket))
                        {
                            add_new_socket(responderSocket);
                        }

                        /**********************************************/
                        /* Loop back up and accept another incoming   */
                        /* connection                                 */
                        /**********************************************/
                    } while (responderSocket != -1);
                }

                /****************************************************/
                /* This is not the listening socket, therefore an   */
                /* existing connection must be readable             */
                /****************************************************/
                else
                {
                    writeLog("Descriptor is readable", LOG_INFO);
                    close_conn = false;
                    /*************************************************/
                    /* Receive all incoming data on this socket      */
                    /* before we loop back and call select again.    */
                    /*************************************************/
                    do
                    {
                        /**********************************************/
                        /* Receive data on this connection until the  */
                        /* recv fails with EWOULDBLOCK.  If any other */
                        /* failure occurs, we will close the          */
                        /* connection.                                */
                        /**********************************************/
                        rc = recv(socket_index, buffer, sizeof(buffer), 0);
                        if (rc < 0)
                        {
                            if (errno != EWOULDBLOCK)
                            {
                                writeLog("recv() failed", LOG_ERROR);
                                close_conn = true;
                            }
                            break;
                        }

                        /**********************************************/
                        /* Check to see if the connection has been    */
                        /* closed by the client                       */
                        /**********************************************/
                        if (rc == 0)
                        {
                            writeLog("Connection closed", LOG_WARNING);
                            close_conn = true;
                            break;
                        }

                        /**********************************************/
                        /* Data was recevied                          */
                        /**********************************************/
                        len = rc;
                        //printf("  %d bytes received\n", len);
                        printf("%s", buffer);

                        /**********************************************/
                        /* Echo the data back to the client           */
                        /**********************************************/
                        //rc = send(i, buffer, len, 0);
                        //if (rc < 0)
                        //{
                        //   perror("  send() failed");
                        //   close_conn = TRUE;
                        //   break;
                        //}

                    } while (true);

                    /*************************************************/
                    /* If the close_conn flag was turned on, we need */
                    /* to clean up this active connection.  This     */
                    /* clean up process includes removing the        */
                    /* descriptor from the master set and            */
                    /* determining the new maximum descriptor value  */
                    /* based on the bits that are still turned on in */
                    /* the master set.                               */
                    /*************************************************/
                    if (close_conn)
                    {
                        closesocket(socket_index);
                        FD_CLR(socket_index, &master_set);
                        if (socket_index == max_sd)
                        {
                            while (FD_ISSET(max_sd, &master_set) == false)
                                max_sd -= 1;
                        }
                    }
                } /* End of existing connection is readable */
            } /* End of if (FD_ISSET(i, &working_set)) */
        } /* End of loop through selectable descriptors */

    } while (end_server == false);

    /*************************************************************/
    /* Cleanup all of the sockets that are open                  */
    /*************************************************************/
    for (socket_index=0; socket_index <= max_sd; ++socket_index)
    {
        if (FD_ISSET(socket_index, &master_set))
            closesocket(socket_index);
    }

    return NormalExit;
}

bool is_new_socket(int sock)
{
    std::map<SOCKET, AbnfAdapter*>::iterator it;
    it = responderList.find(sock);

    if (it != responderList.end())
    {
        return false;
    }

    return true;
}

void add_new_socket(int sock)
{
    AbnfAdapter *adapter = new AbnfAdapter(sock);
    responderList[sock] = adapter;
}

void remove_new_socket(int sock)
{
    AbnfAdapter *adapter = responderList[sock];
    delete adapter;
}

