#pragma once

class JsonNode;


class Module
{
private :
	bool enabled;

public:
	Module(bool start_enabled = true) : enabled(start_enabled)
	{}

	virtual ~Module()
	{}

	virtual bool Init() 
	{
		return true; 
	}

	virtual bool Start()
	{
		return true;
	}

	virtual updateStatus PreUpdate(float dt)
	{
		return UPDATE_CONTINUE;
	}

	virtual updateStatus Update(float dt)
	{
		return UPDATE_CONTINUE;
	}

	virtual updateStatus PostUpdate(float dt)
	{
		return UPDATE_CONTINUE;
	}

	virtual bool CleanUp() 
	{ 
		return true; 
	}

	virtual bool Save(JsonNode* config)
	{
		return true;
	}
};