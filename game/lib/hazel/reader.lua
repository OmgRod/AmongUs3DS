local Reader = {}
Reader.__index = Reader

function Reader.new(data)
    return setmetatable({ buffer = data, pos = 1 }, Reader)
end

function Reader:readByte()
    if self.pos > #self.buffer then return 0 end
    local val = string.byte(self.buffer, self.pos)
    self.pos = self.pos + 1
    return val
end

function Reader:readBoolean()
    return self:readByte() ~= 0
end

function Reader:readUInt16()
    local lo = self:readByte()
    local hi = self:readByte()
    return lo + (hi * 256)
end

function Reader:readInt32()
    local b1 = self:readByte()
    local b2 = self:readByte()
    local b3 = self:readByte()
    local b4 = self:readByte()
    return b1 + (b2 * 256) + (b3 * 65536) + (b4 * 16777216)
end

function Reader:readPackedInt()
    local result = 0
    local shift = 0
    while true do
        local b = self:readByte()
        result = result + ((b % 128) * (2 ^ shift))
        shift = shift + 7
        if b < 128 then break end
    end
    return math.floor(result)
end

function Reader:readString()
    local len = self:readPackedInt()
    if self.pos + len - 1 > #self.buffer then return "" end
    local str = string.sub(self.buffer, self.pos, self.pos + len - 1)
    self.pos = self.pos + len
    return str
end

function Reader:readRemaining()
    local remaining = string.sub(self.buffer, self.pos)
    self.pos = #self.buffer + 1
    return remaining
end

function Reader:isEOF()
    return self.pos > #self.buffer
end

return Reader