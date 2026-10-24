local Writer = {}
Writer.__index = Writer

function Writer.new()
    return setmetatable({ buffer = "" }, Writer)
end

function Writer:writeByte(val)
    self.buffer = self.buffer .. string.char(math.floor(val) % 256)
end

function Writer:writeBoolean(val)
    self:writeByte(val and 1 or 0)
end

function Writer:writeUInt16(val)
    val = math.floor(val)
    local lo = val % 256
    local hi = math.floor(val / 256) % 256
    self.buffer = self.buffer .. string.char(lo, hi)
end

function Writer:writeInt32(val)
    val = math.floor(val)
    local b1 = val % 256
    local b2 = math.floor(val / 256) % 256
    local b3 = math.floor(val / 65536) % 256
    local b4 = math.floor(val / 16777216) % 256
    self.buffer = self.buffer .. string.char(b1, b2, b3, b4)
end

function Writer:writePackedInt(val)
    val = math.floor(val)
    repeat
        local byte = val % 128
        val = math.floor(val / 128)
        if val > 0 then
            byte = byte + 128
        end
        self:writeByte(byte)
    until val == 0
end

function Writer:writeString(str)
    self:writePackedInt(#str)
    self.buffer = self.buffer .. str
end

function Writer:writeBytes(rawBytes)
    self.buffer = self.buffer .. rawBytes
end

function Writer:getBytes()
    return self.buffer
end

function Writer:clear()
    self.buffer = ""
end

return Writer