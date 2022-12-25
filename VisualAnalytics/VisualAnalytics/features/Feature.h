#pragma once
#include "../Model.h"
#include "../framebuffers/FramebufferData.h"
#include <string.h>

template<typename T>
class Feature
{

public:
	std::string name;
	T buffer;

protected:

	Model* model;
	FramebufferData* framebufferData;


public:
	Feature(Model* m, FramebufferData* framebufferData)
	{
		this->model = m;
		this->framebufferData = framebufferData;
	};

	/// <summary>
	/// Recompute feature
	/// </summary>
	virtual void compute(glm::vec3 cameraPosition) = 0;

	/// <summary>
	/// Return string representation of ith element
	/// </summary>
	/// <param name="idx"></param>
	virtual std::string ToString(int idx) = 0;

	/// <summary>
	/// Set feature value manually
	/// </summary>
	/// <typeparam name="T"></typeparam>
	void setFeature(T newValue) 
	{
		this->buffer = newValue;
	}
};

