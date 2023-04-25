#include "Exporter.h"

void Exporter::clear_all()
{
	VisibleTrianglesFeatures.clear();
	ProjectedAreaFeatures.clear();
	VisibleAreaRatioFeatures.clear();
	VisibilityRatioFeatures.clear();
	ViewpointEntropyFeatures.clear();
	DepthMaximizingFeatures.clear();
	SilhouetteCurvatureFeatures.clear();
	SilhouetteLengthFeatures.clear();

	ContourFeatures.clear();
	DiscontinuityFeatures.clear();
	ContourDistanceFeatures.clear();

	PixelPositionFeatures.clear();
	ViewDependentCurvatureFeatures.clear();
	NormalFeatures.clear();
	WorldPositionFeatures.clear();
}

void Exporter::add_features(FeatureSet* featureSet)
{
	for (int i = 0; i < 800*600; i++)
	{
		// is pixel background?
		if (featureSet->maskFeature->buffer[i] == 0)
			continue;

		// not background

		// float features
		VisibleTrianglesFeatures.push_back(featureSet->viewDependentFeatures[0]->buffer);
		ProjectedAreaFeatures.push_back(featureSet->viewDependentFeatures[1]->buffer);
		VisibleAreaRatioFeatures.push_back(featureSet->viewDependentFeatures[2]->buffer);
		VisibilityRatioFeatures.push_back(featureSet->viewDependentFeatures[3]->buffer);
		ViewpointEntropyFeatures.push_back(featureSet->viewDependentFeatures[4]->buffer);
		DepthMaximizingFeatures.push_back(featureSet->viewDependentFeatures[5]->buffer);
		SilhouetteCurvatureFeatures.push_back(featureSet->viewDependentFeatures[6]->buffer);
		SilhouetteLengthFeatures.push_back(featureSet->viewDependentFeatures[7]->buffer);

		//per pixel float features
		ContourFeatures.push_back(featureSet->pixelDependentFeatures[0]->buffer[i]);
		DiscontinuityFeatures.push_back(featureSet->pixelDependentFeatures[1]->buffer[i]);
		ContourDistanceFeatures.push_back(featureSet->pixelDependentFeatures[2]->buffer[i]);

		//per pixel vec2 features
		PixelPositionFeatures.push_back(featureSet->pixelDependentVec2Features[0]->buffer[i]);

		//per pixel vec3 features
		ViewDependentCurvatureFeatures.push_back(featureSet->pixelDependentVec3Features[0]->buffer[i]);
		NormalFeatures.push_back(featureSet->pixelDependentVec3Features[0]->buffer[i]);
		WorldPositionFeatures.push_back(featureSet->pixelDependentVec3Features[0]->buffer[i]);
	}
}

void Exporter::export_to_csv(std::string path, FeatureSet* set)
{
	std::cout << "Exporting of "<< VisibleTrianglesFeatures.size() << " rows started..." << std::endl;
	std::ofstream csv("C:\\Users\\Ondrej\\Desktop\\" + path);

	//write header
	std::string header = "";
	for (auto& feature : set->viewDependentFeatures)
	{
		header += feature->name + ", ";
	}

	for (auto& feature : set->pixelDependentFeatures)
	{
		header += feature->name + ", ";
	}

	for (auto& feature : set->pixelDependentVec2Features)
	{
		header += feature->name + ", ";
	}

	for (auto& feature : set->pixelDependentVec3Features)
	{
		header += feature->name + ", ";
	}

	header += "\n";
	csv << header;

	//write features
	for (int i = 0; i < VisibleTrianglesFeatures.size(); i++)
	{
		if (i % 100000 == 0)
			std::cout << i << std::endl;

		std::string line = "";

		//float features
		line += std::to_string(VisibleTrianglesFeatures[i]) + ", ";
		line += std::to_string(ProjectedAreaFeatures[i]) + ", ";
		line += std::to_string(VisibleAreaRatioFeatures[i]) + ", ";
		line += std::to_string(VisibilityRatioFeatures[i]) + ", ";
		line += std::to_string(ViewpointEntropyFeatures[i]) + ", ";
		line += std::to_string(DepthMaximizingFeatures[i]) + ", ";
		line += std::to_string(SilhouetteCurvatureFeatures[i]) + ", ";
		line += std::to_string(SilhouetteLengthFeatures[i]) + ", ";

		//per pixel float
		line += std::to_string(ContourFeatures[i]) + ", ";
		line += std::to_string(DiscontinuityFeatures[i]) + ", ";
		line += std::to_string(ContourDistanceFeatures[i]) + ", ";

		//per pixel vec2
		glm::vec2 v = PixelPositionFeatures[i];
		line += std::to_string(v.x) + ", " + std::to_string(v.y) + ", ";

		//per pixel vec3
		glm::vec3 vec = ViewDependentCurvatureFeatures[i];
		line += std::to_string(v.x) + ", " + std::to_string(v.y) + ", " + std::to_string(v.y) + ", ";

		vec = NormalFeatures[i];
		line += std::to_string(v.x) + ", " + std::to_string(v.y) + ", " + std::to_string(v.y) + ", ";

		vec = WorldPositionFeatures[i];
		line += std::to_string(v.x) + ", " + std::to_string(v.y) + ", " + std::to_string(v.y);

		line.pop_back();
		line.pop_back();
		line += "\n";
		csv << line;
	}

	csv.close();
	std::cout << "Exporting finished" << std::endl;
}

void Exporter::export_to_csv(std::string path, FeatureSet* set, std::vector<std::vector<float>>& data)
{
	std::cout << "Exporting of " << VisibleTrianglesFeatures.size() << " rows started..." << std::endl;
	std::ofstream csv("C:\\Users\\Ondrej\\Desktop\\" + path);

	//write header
	std::string header = "";
	for (auto& feature : set->viewDependentFeatures)
	{
		header += feature->name + ", ";
	}

	for (auto& feature : set->pixelDependentFeatures)
	{
		header += feature->name + ", ";
	}

	for (auto& feature : set->pixelDependentVec2Features)
	{
		header += feature->name + ", ";
	}

	for (auto& feature : set->pixelDependentVec3Features)
	{
		header += feature->name + ", ";
	}

	header += "\n";
	csv << header;

	//write features
	for (int i = 0; i < data[0].size(); i++)
	{
		if (i % 100000 == 0)
			std::cout << i << std::endl;

		std::string line = "";

		// iterate columns
		for (int j = 0; j < data.size(); j++)
		{
			line += std::to_string(data[j][i]) + ", ";
		}

		line.pop_back();
		line.pop_back();
		line += "\n";
		csv << line;
	}

	csv.close();
	std::cout << "Exporting finished" << std::endl;
}

void Exporter::standardize()
{
	std::cout << "Data standardization started..." << std::endl;
	perform_standardization(VisibleTrianglesFeatures);
	perform_standardization(ProjectedAreaFeatures);
	perform_standardization(VisibleAreaRatioFeatures);
	perform_standardization(VisibilityRatioFeatures);
	perform_standardization(ViewpointEntropyFeatures);
	perform_standardization(DepthMaximizingFeatures);
	perform_standardization(SilhouetteCurvatureFeatures);
	perform_standardization(SilhouetteLengthFeatures);
	perform_standardization(DiscontinuityFeatures);
	perform_standardization(ContourDistanceFeatures);
	perform_standardization(PixelPositionFeatures);
	perform_standardization(ViewDependentCurvatureFeatures);
	perform_standardization(NormalFeatures);
	perform_standardization(WorldPositionFeatures);
	std::cout << "Data standardization completed." << std::endl;
}

void Exporter::normalize()
{
	std::cout << "Data normalization started..." << std::endl;
	perform_normalization(VisibleTrianglesFeatures);
	perform_normalization(ProjectedAreaFeatures);
	perform_normalization(VisibleAreaRatioFeatures);
	perform_normalization(VisibilityRatioFeatures);
	perform_normalization(ViewpointEntropyFeatures);
	perform_normalization(DepthMaximizingFeatures);
	perform_normalization(SilhouetteCurvatureFeatures);
	perform_normalization(SilhouetteLengthFeatures);
	perform_normalization(DiscontinuityFeatures);
	perform_normalization(ContourDistanceFeatures);
	perform_normalization(PixelPositionFeatures);
	perform_normalization(ViewDependentCurvatureFeatures);
	perform_normalization(NormalFeatures);
	perform_normalization(WorldPositionFeatures);
	std::cout << "Data normalization completed." << std::endl;
}

std::vector<std::vector<float>> Exporter::reduce_rows()
{
	std::cout << "Row reduction started..." << std::endl;

	// divide non-float features to multiple float vectors
	std::vector<float> pix_pos_x;
	std::vector<float> pix_pos_y;
	std::vector<float> curv_x;
	std::vector<float> curv_y;
	std::vector<float> curv_z;
	std::vector<float> norm_x;
	std::vector<float> norm_y;
	std::vector<float> norm_z;
	std::vector<float> world_pos_x;
	std::vector<float> world_pos_y;
	std::vector<float> world_pos_z;

	for (int i = 0; i < VisibleTrianglesFeatures.size(); i++)
	{
		if (i % 100000 == 0)
			std::cout << i << std::endl;

		pix_pos_x.push_back(PixelPositionFeatures[i].x);
		pix_pos_y.push_back(PixelPositionFeatures[i].y);

		curv_x.push_back(ViewDependentCurvatureFeatures[i].x);
		curv_y.push_back(ViewDependentCurvatureFeatures[i].y);
		curv_z.push_back(ViewDependentCurvatureFeatures[i].z);

		norm_x.push_back(NormalFeatures[i].x);
		norm_y.push_back(NormalFeatures[i].y);
		norm_z.push_back(NormalFeatures[i].z);

		world_pos_x.push_back(WorldPositionFeatures[i].x);
		world_pos_y.push_back(WorldPositionFeatures[i].y);
		world_pos_z.push_back(WorldPositionFeatures[i].z);
	}

	std::vector<std::vector<float>> data {
		VisibleTrianglesFeatures,
		ProjectedAreaFeatures,
		VisibleAreaRatioFeatures,
		VisibilityRatioFeatures,
		ViewpointEntropyFeatures,
		DepthMaximizingFeatures,
		SilhouetteCurvatureFeatures,
		SilhouetteLengthFeatures,
		ContourFeatures,
		DiscontinuityFeatures,
		ContourDistanceFeatures,
		pix_pos_x,
		pix_pos_y,
		curv_x,
		curv_y,
		curv_z,
		norm_x,
		norm_y,
		norm_z,
		world_pos_x,
		world_pos_y,
		world_pos_z
	};

	auto data_out = subsetSelector::compute<float>(data, 5000, 10000, 1000);

	std::cout << "Row reduction completed." << std::endl;

	return data_out;
}

/// <summary>
/// Standardize components of float array
/// </summary>
/// <param name="values"></param>
void Exporter::perform_standardization(std::vector<float> &values)
{
	// compute mean
	float sum = 0.0;
	int n = 0;

	for (int i = 0; i < values.size(); i++)
	{
		if (values[i] != 0.0f)
		{
			sum += values[i];
			n++;
		}
	}

	float mean = sum / n;

	// compute std
	float tmp = 0.0;
	for (int i = 0; i < values.size(); i++) {
		tmp += pow(values[i] - mean, 2);
	}

	float std = sqrt(tmp / n);

	// transform data
	for (int i = 0; i < values.size(); i++) {
		if (values[i] != 0.0)
		{
			values[i] = (values[i] - mean) / std;
		}
	}
}

/// <summary>
/// Standardize components of vector array - channel-wise
/// </summary>
/// <param name="values"></param>
void Exporter::perform_standardization(std::vector<glm::vec2>& values)
{
	// compute means
	glm::vec2 sum(0.0);
	int n = 0;

	for (int i = 0; i < values.size(); i++)
	{
		if (values[i].x != 0.0f)
			sum.x += values[i].x;

		if (values[i].y != 0.0f)
			sum.y += values[i].y;

		if (values[i].x != 0.0f || values[i].y != 0.0f)
			n++;
	}

	glm::vec2 mean(sum.x / n, sum.y / n);

	// compute std
	glm::vec2 tmp(0.0);
	for (int i = 0; i < values.size(); i++) {

		tmp.x += pow(values[i].x - mean.x, 2);
		tmp.y += pow(values[i].y - mean.y, 2);
	}

	glm::vec2 std(tmp.x / n, tmp.y / n);

	// transform data
	for (int i = 0; i < values.size(); i++) {
		values[i].x = (values[i].x - mean.x) / std.x;
		values[i].y = (values[i].y - mean.y) / std.y;
	}
}

/// <summary>
/// Standardize components of vector array - channel-wise
/// </summary>
/// <param name="values"></param>
void Exporter::perform_standardization(std::vector<glm::vec3>& values)
{
	// compute means
	glm::vec3 sum(0.0);
	int n = 0;

	for (int i = 0; i < values.size(); i++)
	{
		if (values[i].x != 0.0f)
			sum.x += values[i].x;

		if (values[i].y != 0.0f)
			sum.y += values[i].y;

		if (values[i].z != 0.0f)
			sum.z += values[i].z;

		if (values[i].x != 0.0f || values[i].y != 0.0f || values[i].z != 0.0f)
			n++;
	}

	glm::vec3 mean(sum.x / n, sum.y / n, sum.z / n);

	// compute std
	glm::vec3 tmp(0.0);
	for (int i = 0; i < values.size(); i++) {

		tmp.x += pow(values[i].x - mean.x, 2);
		tmp.y += pow(values[i].y - mean.y, 2);
		tmp.z += pow(values[i].z - mean.z, 2);
	}

	glm::vec3 std(tmp.x / n, tmp.y / n, tmp.z / n);

	// transform data
	for (int i = 0; i < values.size(); i++) {
		values[i].x = (values[i].x - mean.x) / std.x;
		values[i].y = (values[i].y - mean.y) / std.y;
		values[i].z = (values[i].z - mean.z) / std.z;
	}
}

void Exporter::perform_normalization(std::vector<float>& values)
{
	float _min = FLT_MAX;
	float _max = FLT_MIN;
	for (const auto& val : values)
	{
		_min = std::min(_min, val);
		_max = std::max(_max, val);
	}

	for (int i = 0; i < values.size(); i++) {
		values[i] = (values[i] - _min) / (_max - _min);
	}
}

void Exporter::perform_normalization(std::vector<glm::vec2>& values)
{
	glm::vec2 _min(FLT_MAX);
	glm::vec2 _max(FLT_MIN);
	
	for (const auto& val : values)
	{
		_min.x = std::min(_min.x, val.x);
		_min.y = std::min(_min.y, val.y);
		_max.x = std::max(_max.x, val.x);
		_max.y = std::max(_max.y, val.y);
	}

	for (int i = 0; i < values.size(); i++) {
		values[i] = (values[i] - _min) / (_max - _min);
	}
}

void Exporter::perform_normalization(std::vector<glm::vec3>& values)
{
	glm::vec3 _min(FLT_MAX);
	glm::vec3 _max(FLT_MIN);

	for (const auto& val : values)
	{
		_min.x = std::min(_min.x, val.x);
		_min.y = std::min(_min.y, val.y);
		_min.z = std::min(_min.z, val.z);
		
		_max.x = std::max(_max.x, val.x);
		_max.y = std::max(_max.y, val.y);
		_max.z = std::max(_max.z, val.z);
	}

	for (int i = 0; i < values.size(); i++) {
		values[i] = (values[i] - _min) / (_max - _min);
	}
}