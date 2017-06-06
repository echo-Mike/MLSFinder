--[[ 
*   MLSGen.lua
*   This script can create sequences of bits, represented as string of "1" and "0",
*   thst will contain "insCount" of MLS generated with LFSR with customizable feedback
*   by "coefs" variable.
]]
--Coefficients of LFSR feedback
local coefs = {0,0,0,0,1,0,0,0,1}
--Insert m-sequence in output sequence "insCount" times
local insCount = 5
--Length of output sequence in bits
local seqLen = 10000

--Power of creational polynom
local power = #coefs

--Check that MLS can be merged into output sequence "insCount" times
if seqLen + (1 - (2^power-1)) * insCount < 0 then
    print("ERROR::MAIN::Can't merge MLS into output sequence.")
    os.exit()
end

local inTable = function(_table, _value)
    for _,v in pairs(_table) do
        if v == _value then
            return true
        end    
    end
    return false
end

function main()
    --Create initial state of LFSR : {1, 0, 0,...}
    local reg = {1}
    for i = 1,power-1 do
        table.insert(reg, 0)
    end
    
    --Forming MLS as string of "1" and "0"
    local buff = 0
    local mls = ""
    local mlsLength = 2^power-1
    for _index = 1,mlsLength do
        buff = 0
        for _index1 = 1,power do
            buff = buff + reg[_index1] * coefs[_index1]
        end
        buff = math.fmod(buff,2)
        table.insert(reg, 1, buff)
        mls = mls..tostring(table.remove(reg))
    end
    
    --Initiate random number generator
    math.randomseed(os.time())
    
    --(1 - mlsLength) - mls take place of 1 bit in output sequence
    local outputSeqLength = seqLen + (1 - mlsLength) * insCount
    --Obtaint "insCount" distinct locations to put MLS
    local location = {}
    for _index = 1,insCount do
        local _buff = math.random(1, outputSeqLength)
        while inTable(location, _buff) do
            _buff = math.random(1, outputSeqLength)
        end
        table.insert(location, _buff)
    end
    table.sort(location)
    
    --Write output sequence to file
    local h = io.open("MLSResult.txt","w+")
    for _index = 1,outputSeqLength do
        if inTable(location, _index) then
            h:write(mls)
        else
            h:write(math.random(0,1))
        end
    end
    h:close()

    --Write support information (locations and generated mls) to file
    local h = io.open("MLSSequence.txt", "w+")
    h:write(mls.."\n")
    local _counter = 0
    for _,v in pairs(location) do
        -- v - in strings that starts with 1 (symbol number)
        -- v-1 - in strings that starts with 0
        h:write(v + _counter * (mlsLength-1), "\t", v-1 + _counter * (mlsLength-1), "\n")
        _counter = _counter + 1
    end
    h:close()
end

--call main function
main()