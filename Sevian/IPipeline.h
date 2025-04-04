#pragma once
class IPipeline
{
public:
	virtual void bindPipeline () = 0;

	virtual void addResource () = 0;
};

