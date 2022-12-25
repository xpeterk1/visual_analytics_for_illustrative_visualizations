#pragma once
#include "FeatureSet.h"
#include <string>

class Exporter
{
private:
	// one viewpoint <=> only one float
	std::vector<float> VisibleTrianglesFeatures;
	std::vector<float> ProjectedAreaFeatures;
	std::vector<float> VisibleAreaRatioFeatures;
	std::vector<float> VisibilityRatioFeatures;
	std::vector<float> ViewpointEntropyFeatures;
	std::vector<float> DepthMaximizingFeatures;
	std::vector<float> SilhouetteCurvatureFeatures;
	std::vector<float> SilhouetteLengthFeatures;

	// one viewpoint <=> float for each pixel
	std::vector<float> ContourFeatures;
	std::vector<float> DiscontinuityFeatures;
	std::vector<float> ContourDistanceFeatures;

	// one viewpoint <=> 2/3 float for each pixel
	std::vector<glm::vec2> PixelPositionFeatures;
	std::vector<glm::vec3> ViewDependentCurvatureFeatures;
	std::vector<glm::vec3> NormalFeatures;
	std::vector<glm::vec3> WorldPositionFeatures;


public:
	void clear_all();
	void add_features(FeatureSet* features);
	void export_to_csv(std::string path, FeatureSet* fs);
	void standardize();
	void normalize();

private:
	void perform_standardization(std::vector<float>& data);
	void perform_standardization(std::vector<glm::vec2>& data);
	void perform_standardization(std::vector<glm::vec3>& data);

	void perform_normalization(std::vector<float>& data);
	void perform_normalization(std::vector<glm::vec2>& data);
	void perform_normalization(std::vector<glm::vec3>& data);
};

