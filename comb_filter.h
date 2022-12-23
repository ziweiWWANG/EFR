/*
	This file is part of comb_filter.
	comb_filter is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.
	comb_filter is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.
	You should have received a copy of the GNU General Public License
	along with comb_filter.  If not, see <http://www.gnu.org/licenses/>.
*/
#pragma once
#include <string>
#include <opencv2/opencv.hpp>

namespace comb_filter
{
	class comb_filter
	{
	public:
		comb_filter();
		struct cell_comb_filter
		{
			std::queue<std::pair<int, double>> q1;
			std::queue<std::pair<int, double>> q2;
			std::queue<std::pair<int, double>> q3;
			std::queue<std::pair<int, double>> q4;
			double bias = 0.0;
			double sum_p = 0;
			int x = -1;
			int y = -1;
			double event_num;
			double event_integ;
		};
		std::vector<std::vector<cell_comb_filter>> filter_grid_;
		void update_q1(const int t, const int polarity, cell_comb_filter &ccf_xy);

	private:
		void initialise_image_states();
		void update_q2(const int t, const double polarity, cell_comb_filter &ccf_xy);
		void update_q3(const int t, const double polarity, cell_comb_filter &ccf_xy);
		void update_q4(const int t, const double polarity, cell_comb_filter &ccf_xy);
		void outputEventSampler(const int t, cell_comb_filter &ccf_xy);

		bool initialised_ = false;
		std::ofstream events_output_txt_;
		int delta_t_;
		double sampler_thresh_;
		// time delay
		double d1_ = 0;
		double d2_ = 0;
		// delay gain
		double rho1_ = 0;
		double rho2_ = 0;
	};
} // namespace comb_filter
