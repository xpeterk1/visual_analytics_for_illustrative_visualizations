#pragma once
#include <vector>
#include <iostream>
#include <fstream>

#include "features/Feature.h"
#include "Model.h"

#include "features/VisibleTrianglesFeature.h"
#include "features/ProjectedAreaFeature.h"
#include "features/VisibleAreaRatioFeature.h"
#include "features/VisibilityRatioFeature.h"
#include "features/ViewpointEntropyFeature.h"
#include "features/SilhouetteLengthFeature.h"
#include "features/DepthMaximizingFeature.h"
#include "features/SilhouetteCurvatureFeature.h"
#include "features/PixelPositionFeature.h"
#include "features/ContourFeature.h"
#include "features/DiscontinuityFeature.h"
#include "features/NormalFeature.h"
#include "features/WorldPositionFeature.h"
#include "features/ViewDependentCurvatureFeature.h"
#include "features/MaskFeature.h"
#include "features/ContourDistanceFeature.h"

#include "framebuffers/FramebufferData.h"
#include "framebuffers/IdDepthFramebufferData.h"
#include "framebuffers/SilhouetteFramebufferData.h"
#include "framebuffers/NormalFramebufferData.h"
#include "framebuffers/WorldPositionFramebufferData.h"
#include "framebuffers/ViewDependentCurvatureFramebufferData.h"

class FeatureSet
{

public:
	std::vector<std::unique_ptr<Feature<float>>> viewDependentFeatures;
	std::vector<std::unique_ptr<Feature<std::vector<float>>>> pixelDependentFeatures;
	std::vector<std::unique_ptr<Feature<std::vector<glm::vec2>>>> pixelDependentVec2Features;
	std::vector<std::unique_ptr<Feature<std::vector<glm::vec3>>>> pixelDependentVec3Features;

	std::unique_ptr<Feature<std::vector<float>>> maskFeature;

	Model* model;
	IdDepthFramebufferData IdDepthDataBuffer;
	SilhouetteFramebufferData SilhouetteDataBuffer;
	NormalFramebufferData NormalDataBuffer;
	WorldPositionFramebufferData WorldPositionBuffer;
	ViewDependentCurvatureFramebufferData ViewCurvatureBuffer;
	glm::vec3 viewpoint;

public:
	FeatureSet(Model* m, glm::vec3 initial_camera_pos) :
		IdDepthDataBuffer(m), SilhouetteDataBuffer(m), NormalDataBuffer(m), WorldPositionBuffer(m), ViewCurvatureBuffer(m)
	{
		this->model = m;
		this->viewpoint = initial_camera_pos;

		maskFeature = std::make_unique<MaskFeature>(m, &IdDepthDataBuffer);

		viewDependentFeatures.push_back(std::make_unique<VisibleTrianglesFeature>(m, &IdDepthDataBuffer));
		viewDependentFeatures.push_back(std::make_unique<ProjectedAreaFeature>(m, &IdDepthDataBuffer));
		viewDependentFeatures.push_back(std::make_unique<VisibleAreaRatioFeature>(m, &IdDepthDataBuffer));
		viewDependentFeatures.push_back(std::make_unique<VisibilityRatioFeature>(m, &IdDepthDataBuffer));
		viewDependentFeatures.push_back(std::make_unique<ViewpointEntropyFeature>(m, &IdDepthDataBuffer));
		viewDependentFeatures.push_back(std::make_unique<DepthMaximizingFeature>(m, &IdDepthDataBuffer));

		viewDependentFeatures.push_back(std::make_unique<SilhouetteCurvatureFeature>(m, &SilhouetteDataBuffer));
		viewDependentFeatures.push_back(std::make_unique<SilhouetteLengthFeature>(m, &SilhouetteDataBuffer));

		pixelDependentFeatures.push_back(std::make_unique<ContourFeature>(m, &SilhouetteDataBuffer));
		pixelDependentFeatures.push_back(std::make_unique<DiscontinuityFeature>(m, &SilhouetteDataBuffer));
		pixelDependentFeatures.push_back(std::make_unique<ContourDistanceFeature>(m, &SilhouetteDataBuffer));

		pixelDependentVec2Features.push_back(std::make_unique<PixelPositionFeature>(m, nullptr));
		pixelDependentVec3Features.push_back(std::make_unique<ViewDependentCurvatureFeature>(m, &ViewCurvatureBuffer));
		pixelDependentVec3Features.push_back(std::make_unique<NormalFeature>(m, &NormalDataBuffer));
		pixelDependentVec3Features.push_back(std::make_unique<WorldPositionFeature>(m, &WorldPositionBuffer));
	}

	void recompute_features(glm::vec3 cameraPosition)
	{
		viewpoint = cameraPosition;
		IdDepthDataBuffer.refresh(viewpoint);
		SilhouetteDataBuffer.refresh(viewpoint);
		NormalDataBuffer.refresh(viewpoint);
		WorldPositionBuffer.refresh(viewpoint);
		ViewCurvatureBuffer.refresh(viewpoint);

		maskFeature->compute(viewpoint);

		for (auto& feature : viewDependentFeatures)
			feature->compute(viewpoint);

		for (auto& feature : pixelDependentFeatures)
			feature->compute(viewpoint);

		for (auto& feature : pixelDependentVec2Features)
			feature->compute(viewpoint);

		for (auto& feature : pixelDependentVec3Features)
			feature->compute(viewpoint);
	}

	void export_features_to_csv(std::string name)
	{
		std::cout << "Exporting started..." << std::endl;
		std::ofstream csv("C:\\Users\\Ondrej\\Desktop\\exports\\" + name);

		//write header
		std::string header = "";
		for (auto& feature : viewDependentFeatures)
		{
			header += feature->name + ", ";
		}

		for (auto& feature : pixelDependentFeatures)
		{
			header += feature->name + ", ";
		}

		for (auto& feature : pixelDependentVec2Features)
		{
			header += feature->name + ", ";
		}

		for (auto& feature : pixelDependentVec3Features)
		{
			header += feature->name + ", ";
		}

		header += "\n";
		csv << header;

		for (int i = 0; i < 800 * 600; i++)
		{

		}

		//write features
		for (int i = 0; i < 800 * 600; i++)
		{
			if (i % 100000 == 0)
				std::cout << i << std::endl;

			// only insert lines which represent points not in the background
			if (atof(maskFeature->ToString(i).c_str()) == 1.0f)
			{
				std::string line = "";
				for (auto& feature : viewDependentFeatures)
				{
					line += feature->ToString(i) + ", ";
				}

				for (auto& feature : pixelDependentFeatures)
				{
					line += feature->ToString(i) + ", ";
				}

				for (auto& feature : pixelDependentVec2Features)
				{
					line += feature->ToString(i) + ", ";
				}

				for (auto& feature : pixelDependentVec3Features)
				{
					line += feature->ToString(i) + ", ";
				}

				line.pop_back();
				line.pop_back();
				line += "\n";
				csv << line;
			}
		}

		csv.close();
		std::cout << "Exporting finished" << std::endl;
	}

};

