local ffi = require 'ffi'

local cdef = [[

    HANDLE CreateSequence();
	HANDLE CopySequence(HANDLE seqhandle);
	void CloseSequence(HANDLE seqhandle, bool del);
	HANDLE GetSequenceByModel(HANDLE modelhandle, int index);
	int GetModelSequenceSize(HANDLE modelhandle);
	bool AddModelSequence(HANDLE modelhandle, HANDLE seqhandle);
    bool RemoveModelSequence(HANDLE modelhandle, HANDLE  seqhandle);
    
    const char* GetSequenceName(HANDLE  seqhandle);
	void SetSequenceName(HANDLE  seqhandle, const char* name);
	float GetSequenceRarity(HANDLE  seqhandle);
	void SetSequenceRarity(HANDLE  seqhandle, float value);
	float GetSequenceMoveSpeed(HANDLE  seqhandle);
	void SetSequenceMoveSpeed(HANDLE  seqhandle, float value);
	bool GetSequenceNonLooping(HANDLE  seqhandle);
	void SetSequenceNonLooping(HANDLE  seqhandle, bool value);
	VECTOR2* GetSequenceInterval(HANDLE  seqhandle);
	void SetSequenceInterval(HANDLE  seqhandle, VECTOR2* value);
	EXTENT* GetSequenceExtent(HANDLE  seqhandle);
	void SetSequenceExtent(HANDLE  seqhandle, EXTENT* value);

]]

ffi.cdef(cdef)

local lib = ffi.load("modellib")


local modellib = require 'modellib.modellib'

local sequence = {}

setmetatable(sequence, sequence)

sequence.type = 'sequence'

sequence.all_sequence_map = {}

function sequence.new()
    local handle = lib.CreateSequence()

    local object = setmetatable({handle = handle}, sequence)

    sequence.all_sequence_map[tonumber(handle)] = object

    return object
end 


function sequence.open(model, index)
    local handle = lib.GetSequenceByModel(model.handle, index)
    if handle == nil then 
        return 
    end 
    local object = sequence.all_sequence_map[tonumber(handle)]
    if object then 
        return object 
    end 

    object = setmetatable({handle = handle, owner = model}, sequence)

    sequence.all_sequence_map[tonumber(handle)] = object

    return object
end 


function sequence:close()
    if self.handle == nil then 
        return 
    end 

  
    lib.CloseSequence(self.handle, self.owner == nil)
    
    sequence.all_sequence_map[tonumber(self.handle)] = nil
end 


function sequence:copy()
    if self.handle == nil then 
        return 
    end 

    local handle = lib.CopySequence(self.handle)

    local object = setmetatable({handle = handle}, sequence)

    sequence.all_sequence_map[tonumber(handle)] = object

    return object
end 




modellib.auto_method(sequence, cdef)


return sequence