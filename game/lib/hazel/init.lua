local socket = require("socket")
local Writer = require("lib.hazel.writer")
local Reader = require("lib.hazel.reader")

local Hazel = {
    OPTION_UNRELIABLE = 0x00,
    OPTION_RELIABLE   = 0x01,
    OPTION_HELLO      = 0x08,
    OPTION_DISCONNECT = 0x09,
    OPTION_ACK        = 0x0A,
    OPTION_PING       = 0x0C,

    udp = nil,
    connected = false,
    sequenceNumber = 1,
    pingTimer = 0,
    pingInterval = 3,
    
    onConnect = function() end,
    onData = function(reader, option) end,
    onDisconnect = function(reason) end
}

function Hazel.connect(ip, port, clientName)
    Hazel.udp = socket.udp()
    Hazel.udp:settimeout(0)
    Hazel.udp:setpeername(ip, port)

    local writer = Writer.new()
    writer:writeByte(Hazel.OPTION_HELLO)
    writer:writeUInt16(0x0001)
    writer:writeString(clientName or "LÖVE Potion 3DS")

    Hazel.udp:send(writer:getBytes())
    Hazel.connected = false
    Hazel.sequenceNumber = 1
    Hazel.pingTimer = 0
end

function Hazel.sendUnreliable(payloadBytes)
    if not Hazel.udp then return end
    local writer = Writer.new()
    writer:writeByte(Hazel.OPTION_UNRELIABLE)
    writer:writeBytes(payloadBytes)
    Hazel.udp:send(writer:getBytes())
end

function Hazel.sendReliable(payloadBytes)
    if not Hazel.udp then return end
    local writer = Writer.new()
    writer:writeByte(Hazel.OPTION_RELIABLE)
    writer:writeUInt16(Hazel.sequenceNumber)
    writer:writeBytes(payloadBytes)

    Hazel.udp:send(writer:getBytes())

    Hazel.sequenceNumber = (Hazel.sequenceNumber + 1) % 65536
end

function Hazel.sendAck(seqHi, seqLo)
    if not Hazel.udp then return end
    local ackPacket = string.char(Hazel.OPTION_ACK, seqHi, seqLo, 0xFF)
    Hazel.udp:send(ackPacket)
end

function Hazel.ping()
    if not Hazel.udp then return end
    local writer = Writer.new()
    writer:writeByte(Hazel.OPTION_PING)
    writer:writeUInt16(Hazel.sequenceNumber)
    Hazel.udp:send(writer:getBytes())
    Hazel.sequenceNumber = (Hazel.sequenceNumber + 1) % 65536
end

function Hazel.disconnect()
    if Hazel.udp then
        local writer = Writer.new()
        writer:writeByte(Hazel.OPTION_DISCONNECT)
        Hazel.udp:send(writer:getBytes())
        Hazel.udp:close()
        Hazel.udp = nil
    end
    Hazel.connected = false
end

function Hazel.update(dt)
    if not Hazel.udp then return end

    if Hazel.connected then
        Hazel.pingTimer = Hazel.pingTimer + dt
        if Hazel.pingTimer >= Hazel.pingInterval then
            Hazel.pingTimer = 0
            Hazel.ping()
        end
    end

    while true do
        local data, err = Hazel.udp:receive()
        if not data then break end

        local reader = Reader.new(data)
        local option = reader:readByte()

        if option == Hazel.OPTION_HELLO then
            Hazel.connected = true
            Hazel.onConnect()

        elseif option == Hazel.OPTION_RELIABLE then
            local seqLo = reader:readByte()
            local seqHi = reader:readByte()
            Hazel.sendAck(seqHi, seqLo)

            Hazel.onData(reader, option)

        elseif option == Hazel.OPTION_UNRELIABLE then
            Hazel.onData(reader, option)

        elseif option == Hazel.OPTION_DISCONNECT then
            Hazel.connected = false
            local reason = reader:readRemaining()
            Hazel.onDisconnect(reason)
        end
    end
end

Hazel.Writer = Writer
Hazel.Reader = Reader

return Hazel