/*
	This file is part of CombFilter.
	CombFilter is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.
	CombFilter is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.
	You should have received a copy of the GNU General Public License
	along with CombFilter.  If not, see <http://www.gnu.org/licenses/>.
*/
#define _USE_MATH_DEFINES
#include "comb_filter.h"
#include "yaml-cpp/yaml.h"

namespace comb_filter
{
	comb_filter::comb_filter()
	{
		if (!initialised_)
		{
			initialise_image_states();
		}
	}

	// Initialization
	void comb_filter::initialise_image_states()
	{
		const YAML::Node config = YAML::LoadFile("../configs/EFR_config.yaml");
		int img_height = config["img_height"].as<int>();
		int img_width = config["img_width"].as<int>();
		std::string output_event = config["output_event"].as<std::string>();
		rho1_ = config["rho1"].as<double>();
		rho2_ = 1 - (1 - rho1_) / 10;
		delta_t_ = config["delta_t"].as<int>();
		std::string data_id = config["data_id"].as<std::string>();
		sampler_thresh_ = config["sampler_threshold"].as<double>();

		int base_frequency = config["base_frequency"].as<int>();
		int time_resolution = config["time_resolution"].as<int>();
		d1_ = 1 * time_resolution / base_frequency;
		d2_ = d1_ / 10;

		// output events
		std::string output_event_path;
		output_event_path += "../data/" + data_id + "/" + output_event;
		events_output_txt_.open(output_event_path);
		events_output_txt_ << img_width << " " << img_height << "\n";
		std::cout << "Save filtered event data to: " << output_event_path << "\n";

		filter_grid_.resize(img_height);
		for (std::vector<cell_comb_filter> &gridCol : filter_grid_)
		{
			gridCol.resize(img_width);
		}
		initialised_ = true;
	}

	void comb_filter::update_q1(const int t, const int polarity, cell_comb_filter &ccf_xy)
	{
		ccf_xy.q1.push({t, polarity - ccf_xy.bias});
		while (t - ccf_xy.q1.front().first >= d1_)
		{
			update_q2(ccf_xy.q1.front().first + d1_, -ccf_xy.q1.front().second, ccf_xy);
			ccf_xy.q1.pop();
		}
		// process current event
		update_q2(t, polarity - ccf_xy.bias, ccf_xy);
	}

	void comb_filter::update_q2(const int t, const double polarity, cell_comb_filter &ccf_xy)
	{
		while ((ccf_xy.q2.size() > 0) &&
			   ((t - ccf_xy.q2.front().first) >= d1_))
		{
			ccf_xy.q2.push({d1_ + ccf_xy.q2.front().first, rho1_ * ccf_xy.q2.front().second});
			update_q3(d1_ + ccf_xy.q2.front().first, rho1_ * ccf_xy.q2.front().second, ccf_xy);
			ccf_xy.q2.pop();
		}

		if ((ccf_xy.q2.size() > 0) &&
			(abs(ccf_xy.q2.back().first - t) < delta_t_))
		{
			ccf_xy.q2.back().second += polarity;
		}
		else
		{
			ccf_xy.q2.push({t, polarity});
		}
	}

	void comb_filter::update_q3(const int t, const double polarity, cell_comb_filter &ccf_xy)
	{
		ccf_xy.q3.push({t, polarity});
		while (t - ccf_xy.q3.front().first >= d2_)
		{
			update_q4(ccf_xy.q3.front().first + d2_, -rho2_ * ccf_xy.q3.front().second, ccf_xy);
			ccf_xy.q3.pop();
		}
		update_q4(t, polarity, ccf_xy);
	}

	void comb_filter::update_q4(const int t, const double polarity, cell_comb_filter &ccf_xy)
	{
		while ((ccf_xy.q4.size() > 0) &&
			   ((t - ccf_xy.q4.front().first) >= d2_))
		{
			ccf_xy.q4.push({d2_ + ccf_xy.q4.front().first, ccf_xy.q4.front().second});
			ccf_xy.sum_p += ccf_xy.q4.front().second;
			outputEventSampler(d2_ + ccf_xy.q4.front().first, ccf_xy);
			ccf_xy.q4.pop();
		}

		if ((ccf_xy.q4.size() > 0) &&
			(abs(ccf_xy.q4.back().first - t) < delta_t_))
		{
			ccf_xy.q4.back().second += polarity;
		}
		else
		{
			ccf_xy.q4.push({t, polarity});
		}
	}
	void comb_filter::outputEventSampler(const int t, cell_comb_filter &ccf_xy)
	{
		while ((ccf_xy.sum_p >= sampler_thresh_) ||
			   (ccf_xy.sum_p <= -sampler_thresh_))
		{
			if (ccf_xy.sum_p >= sampler_thresh_)
			{
				events_output_txt_ << t << " " << ccf_xy.x << " " << ccf_xy.y << " " << 1 << "\n";
				ccf_xy.sum_p -= sampler_thresh_;
			}
			else if (ccf_xy.sum_p <= -sampler_thresh_)
			{
				events_output_txt_ << t << " " << ccf_xy.x << " " << ccf_xy.y << " " << -1 << "\n";
				ccf_xy.sum_p += sampler_thresh_;
			}
		}
	}
} // namespace comb_filter