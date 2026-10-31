#include <ftp_server.hpp>
#include <fcntl.h>
#include <cstdlib>

namespace engine {
    FtpServer::FtpServer(int port, const std::string &targetFilePath)
        : m_port(port), m_targetFilePath(targetFilePath) {
    }

    FtpServer::~FtpServer() {
        stop();
    }

    bool FtpServer::start() {
        if (m_running) return true;

        m_serverSocket = socket(AF_INET, SOCK_STREAM, 0);
        if (m_serverSocket < 0) return false;

        int opt = 1;
        setsockopt(m_serverSocket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

        sockaddr_in addr{};
        addr.sin_family = AF_INET;
        addr.sin_addr.s_addr = INADDR_ANY;
        addr.sin_port = htons(m_port);

        if (bind(m_serverSocket, (struct sockaddr*)&addr, sizeof(addr)) < 0) {
            close(m_serverSocket);
            return false;
        }

        if (listen(m_serverSocket, 1) < 0) {
            close(m_serverSocket);
            return false;
        }

        m_running = true;
        m_transferComplete = false;
        m_bytesReceived = 0;

        s32 prio = 0;
        svcGetThreadPriority(&prio, CUR_THREAD_HANDLE);
        m_thread = threadCreate(threadEntry, this, 32 * 1024, prio - 1, -2, false);

        return m_thread != nullptr;
    }

    void FtpServer::stop() {
        if (!m_running) return;
        m_running = false;
        if (m_serverSocket >= 0) {
            close(m_serverSocket);
            m_serverSocket = -1;
        }
        if (m_thread) {
            threadJoin(m_thread, U64_MAX);
            threadFree(m_thread);
            m_thread = nullptr;
        }
    }

    void FtpServer::threadEntry(void *arg) {
        FtpServer *server = static_cast<FtpServer*>(arg);
        server->run();
    }

    void FtpServer::run() {
        while (m_running) {
            sockaddr_in clientAddr{};
            socklen_t clientLen = sizeof(clientAddr);
            int clientSocket = accept(m_serverSocket, (struct sockaddr*)&clientAddr, &clientLen);
            if (clientSocket < 0) {
                if (!m_running) break;
                svcSleepThread(100000000ULL);
                continue;
            }

            const char *welcome = "220 AmongUs3DS Assets Helper FTP Server Ready\r\n";
            send(clientSocket, welcome, strlen(welcome), 0);

            char buf[512];
            int dataSocket = -1;
            int passiveSocket = -1;

            while (m_running) {
                memset(buf, 0, sizeof(buf));
                int received = recv(clientSocket, buf, sizeof(buf) - 1, 0);
                if (received <= 0) break;

                for (int i = 0; i < received; i++) {
                    if (buf[i] == '\r' || buf[i] == '\n') buf[i] = '\0';
                }

                if (strncmp(buf, "USER", 4) == 0) {
                    const char *resp = "331 Guest login ok, send password.\r\n";
                    send(clientSocket, resp, strlen(resp), 0);
                } else if (strncmp(buf, "PASS", 4) == 0) {
                    const char *resp = "230 User logged in.\r\n";
                    send(clientSocket, resp, strlen(resp), 0);
                } else if (strncmp(buf, "SYST", 4) == 0) {
                    const char *resp = "215 UNIX Type: L8\r\n";
                    send(clientSocket, resp, strlen(resp), 0);
                } else if (strncmp(buf, "FEAT", 4) == 0) {
                    const char *resp = "211-Features:\n End\r\n211 End\r\n";
                    send(clientSocket, resp, strlen(resp), 0);
                } else if (strncmp(buf, "PWD", 3) == 0) {
                    const char *resp = "257 \"/\" is current directory.\r\n";
                    send(clientSocket, resp, strlen(resp), 0);
                } else if (strncmp(buf, "TYPE", 4) == 0) {
                    const char *resp = "200 Type set.\r\n";
                    send(clientSocket, resp, strlen(resp), 0);
                } else if (strncmp(buf, "SIZE", 4) == 0) {
                    // Client asking for file size - reply ok with estimated/default size
                    const char *resp = "213 52428800\r\n"; // 50MB default estimate
                    send(clientSocket, resp, strlen(resp), 0);
                } else if (strncmp(buf, "PASV", 4) == 0) {
                    if (passiveSocket >= 0) close(passiveSocket);
                    passiveSocket = socket(AF_INET, SOCK_STREAM, 0);
                    
                    sockaddr_in pasvAddr{};
                    pasvAddr.sin_family = AF_INET;
                    pasvAddr.sin_addr.s_addr = INADDR_ANY;
                    pasvAddr.sin_port = htons(5002);

                    int opt = 1;
                    setsockopt(passiveSocket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
                    bind(passiveSocket, (struct sockaddr*)&pasvAddr, sizeof(pasvAddr));
                    listen(passiveSocket, 1);

                    u32 ip = gethostid();
                    unsigned char *p = (unsigned char*)&ip;
                    
                    char resp[128];
                    snprintf(resp, sizeof(resp), "227 Entering Passive Mode (%d,%d,%d,%d,%d,%d).\r\n",
                             p[0], p[1], p[2], p[3], 5002 / 256, 5002 % 256);
                    send(clientSocket, resp, strlen(resp), 0);
                } else if (strncmp(buf, "STOR", 4) == 0) {
                    const char *resp = "125 Data connection already open; transfer starting.\r\n";
                    send(clientSocket, resp, strlen(resp), 0);

                    sockaddr_in dataClientAddr{};
                    socklen_t dataClientLen = sizeof(dataClientAddr);
                    dataSocket = accept(passiveSocket, (struct sockaddr*)&dataClientAddr, &dataClientLen);
                    if (passiveSocket >= 0) {
                        close(passiveSocket);
                        passiveSocket = -1;
                    }

                    if (dataSocket >= 0) {
                        FILE *outFile = fopen(m_targetFilePath.c_str(), "wb");
                        if (outFile) {
                            char transferBuf[4096];
                            int bytesRead = 0;
                            m_bytesReceived = 0;
                            while ((bytesRead = recv(dataSocket, transferBuf, sizeof(transferBuf), 0)) > 0) {
                                fwrite(transferBuf, 1, bytesRead, outFile);
                                m_bytesReceived += bytesRead;
                                if (m_bytesReceived > m_totalBytes) {
                                    m_totalBytes = m_bytesReceived + 1024; // dynamically adjust total if larger
                                }
                            }
                            fclose(outFile);
                        }
                        close(dataSocket);
                        dataSocket = -1;

                        const char *completeResp = "226 Transfer complete.\r\n";
                        send(clientSocket, completeResp, strlen(completeResp), 0);
                        m_transferComplete = true;
                    } else {
                        const char *errResp = "425 Can't open data connection.\r\n";
                        send(clientSocket, errResp, strlen(errResp), 0);
                    }
                } else if (strncmp(buf, "QUIT", 4) == 0) {
                    const char *resp = "221 Goodbye.\r\n";
                    send(clientSocket, resp, strlen(resp), 0);
                    break;
                } else {
                    const char *res = "200 OK\r\n";
                    send(clientSocket, res, strlen(res), 0);
                }
            }

            if (passiveSocket >= 0) close(passiveSocket);
            if (dataSocket >= 0) close(dataSocket);
            close(clientSocket);
        }
    }
}
