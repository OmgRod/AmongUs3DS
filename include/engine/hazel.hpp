#pragma once

#include <includes.hpp>

namespace engine {
namespace hazel {

    enum class PacketType : uint8_t {
        Unreliable = 0x00,
        Reliable = 0x01,
        Hello = 0x08,
        Disconnect = 0x09,
        Acknowledge = 0x0A,
        Ping = 0x0C
    };

    class MessageWriter {
    private:
        std::vector<uint8_t> m_buffer;

    public:
        MessageWriter() = default;
        
        void writeByte(uint8_t value);
        void writeBytes(const uint8_t* data, size_t length);
        void writeUInt16(uint16_t value);
        void writeUInt32(uint32_t value);
        void writeInt32(int32_t value);
        void writeFloat(float value);
        void writeString(const std::string& value);
        
        void writePackedInt32(int32_t value);
        void writePackedUInt32(uint32_t value);

        void startMessage(uint8_t tag);
        void endMessage();

        const std::vector<uint8_t>& getBuffer() const { return m_buffer; }
        void clear() { m_buffer.clear(); }

    private:
        std::vector<size_t> m_messageStack;
    };

    class MessageReader {
    private:
        const std::vector<uint8_t>& m_buffer;
        size_t m_cursor = 0;
        uint8_t m_tag = 0;

    public:
        MessageReader(const std::vector<uint8_t>& buffer);
        
        uint8_t readByte();
        std::vector<uint8_t> readBytes(size_t length);
        uint16_t readUInt16();
        uint32_t readUInt32();
        int32_t readInt32();
        float readFloat();
        std::string readString();
        
        int32_t readPackedInt32();
        uint32_t readPackedUInt32();

        uint8_t getTag() const { return m_tag; }
        MessageReader readMessage(); 

        bool hasMore() const { return m_cursor < m_buffer.size(); }
        size_t getPosition() const { return m_cursor; }
    };

    class HazelClient {
    private:
        int m_socket = -1;
        sockaddr_in m_serverAddr;
        uint16_t m_nonce = 0;
        bool m_connected = false;

    public:
        HazelClient() = default;
        ~HazelClient();

        bool connect(const std::string& ip, uint16_t port);
        void disconnect();
        void update();

        void sendUnreliable(const MessageWriter& msg);
        void sendReliable(const MessageWriter& msg);
        void sendHello(const std::string& username, uint32_t clientVersion);
        
        bool isConnected() const { return m_connected; }

    private:
        void sendAcknowledge(uint16_t nonce);
        void handleIncomingPacket(const std::vector<uint8_t>& data);
    };

}
}