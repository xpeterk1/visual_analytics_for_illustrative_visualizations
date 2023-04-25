#pragma once

#include <algorithm>
#include <cmath>
#include <cstddef>
#include <limits>
#include <numeric>
#include <vector>
#include <random>
#include <ranges>
#include <stdexcept>
#include <type_traits>

namespace subsetSelector {
	template <typename T>
	std::vector<T> normalized_histogram(const std::vector<T>& data, std::size_t histogram_bins) {
		std::vector<std::size_t> hist(histogram_bins);

		auto [min, max] = std::ranges::minmax(data);
		auto step = (max - min) / static_cast<decltype(min)>(histogram_bins);

		for (auto x : data) {
			std::size_t i = std::min(static_cast<std::size_t>(std::max((x - min) / step, static_cast<decltype(x)>(0))), histogram_bins - 1);
			hist[i]++;
		}

		std::vector<T> normalized_hist(histogram_bins);
		T data_size = static_cast<T>(data.size());
		for (std::size_t i = 0; i < histogram_bins; i++) {
			normalized_hist[i] = static_cast<T>(hist[i]) / data_size;
		}

		return normalized_hist;
	}

	// euclidean distance
	float histogram_similarity(const std::ranges::range auto& a, const std::ranges::range auto& b) {
		if (std::ranges::size(a) != std::ranges::size(b)) {
			throw std::invalid_argument("a and b do not have the same size");
		}

		float similarity = 0.0f;
		// we do not have std::views::zip yet
		for (int i = 0; i < std::ranges::size(a); i++) {
			float diff = static_cast<float>(a[i]) - static_cast<float>(b[i]);
			similarity += diff * diff;
		}
		return std::sqrt(similarity);
	}

	template <typename T>
	std::vector<std::size_t> select_subset(const std::vector<std::vector<T>>& dataset, std::size_t subset_size, unsigned long long int n_tries, std::size_t histogram_bins) {
		if (dataset.empty() || dataset[0].empty() || !subset_size) {
			return {};
		}

		std::size_t columns = dataset.size();
		std::size_t rows = dataset[0].size();

		std::vector<std::vector<T>> population_histograms(columns, std::vector<T>());
		for (std::size_t column = 0; column < columns; column++) {
			population_histograms[column] = normalized_histogram(dataset[column], histogram_bins);
		}

		std::random_device rd;
		std::mt19937 gen{ rd() };

		std::vector<size_t> best_subset(subset_size);
		float best_similarity_score = std::numeric_limits<float>::infinity();

		std::vector<size_t> indices(rows);
		std::iota(indices.begin(), indices.end(), 0);

		std::vector<std::vector<T>> datasubset(columns, std::vector<T>(subset_size, T()));
		std::vector<std::vector<std::size_t>> sample_histograms;
		sample_histograms.reserve(columns);

		for (unsigned long long int t = 0; t < n_tries; t++) {
			printf("\33[2K\r");
			std::cout << t << "/" << n_tries << std::endl;
			sample_histograms.clear();
			std::ranges::shuffle(indices, gen);

			float similarity_score = 0;

			for (std::size_t column = 0; column < columns; column++) {
				for (std::size_t row = 0; row < subset_size; row++) {
					datasubset[column][row] = static_cast<T>(dataset[column][indices[row]]);
				}
				similarity_score += histogram_similarity(population_histograms[column], normalized_histogram(datasubset[column], histogram_bins));
			}

			if (similarity_score < best_similarity_score) {
				best_similarity_score = similarity_score;
				for (std::size_t i = 0; i < subset_size; i++) {
					best_subset[i] = indices[i];
				}
			}
		}

		return best_subset;
	}

	template <typename T>
	std::vector<std::vector<T>> dataset_to_subset(const std::vector<std::vector<T>>& dataset, const std::vector<std::size_t>& indices) {
		if (dataset.empty() || dataset[0].empty() || indices.empty()) {
			return {};
		}

		std::size_t columns = dataset.size();
		std::size_t subset_rows = indices.size();

		std::vector<std::vector<T>> subset(columns, std::vector<T>(subset_rows, T()));

		for (std::size_t column = 0; column < columns; column++) {
			for (std::size_t subset_row = 0; subset_row < subset_rows; subset_row++) {
				subset[column][subset_row] = dataset[column][indices[subset_row]];
			}
		}

		return subset;
	}

	template <typename T>
	std::vector<std::vector<T>> compute(const std::vector<std::vector<T>>& dataset, std::size_t subset_size, unsigned long long int n_tries, std::size_t histogram_bins) {
		return dataset_to_subset(dataset, select_subset(dataset, subset_size, n_tries, histogram_bins));
	}
};