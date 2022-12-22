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
#include "comb_filter.h"
#include "yaml-cpp/yaml.h"
#include <fstream>
#include <queue>
#include <string>

// Load or compute bias
void load_bias(std::string bias_filename, comb_filter::comb_filter &CF);
void compute_bias(std::string bias_filename, std::string input_file_name, comb_filter::comb_filter &CF,
                  int img_height, int img_width);

int main(int argc, char *argv[])
{
    // Load configs
    const YAML::Node config = YAML::LoadFile("../configs/EFR_config.yaml");
    std::string data_id = config["data_id"].as<std::string>();
    std::string input_file_name =
        "../data/" + data_id + "/" + config["input_event"].as<std::string>();
    int process_ts_start =
        config["process_ts_start"].as<double>() * 1e6; // unit microsecond
    int process_ts_end =
        config["process_ts_end"].as<double>() * 1e6; // unit microsecond
    int img_height = config["img_height"].as<int>();
    int img_width = config["img_width"].as<int>();
    int load_or_compute_bias = config["load_or_compute_bias"].as<int>();
    std::cout << "Filter events from "
              << config["process_ts_start"].as<double>() << " s to "
              << config["process_ts_end"].as<double>() << " s.\n"
              << "Input event data path: " << input_file_name << "\n";

    // Open input file
    std::ifstream input_file(input_file_name, std::ios::in | std::ios::binary);
    if (!input_file.is_open())
    {
        std::cerr << "Error : could not open file '" << input_file_name
                  << "' for reading" << std::endl;
        return 0;
    }

    comb_filter::comb_filter CF;

    std::string bias_filename = "../data/" + data_id + "/bias.txt";
    if (load_or_compute_bias)
    {
        // Load bias parameters
        load_bias(bias_filename, CF);
    }
    else
    {
        // Compute bias using the input event data
        compute_bias(bias_filename, input_file_name, CF, img_height, img_width);
    }

    // Load raw events
    std::ifstream myFileFilter(input_file_name);
    if (!myFileFilter.is_open())
        throw std::runtime_error("Could not open file");
    std::string line;
    int ts, c, r, p;
    int filtered_pos_e_count = 0;
    int filtered_neg_e_count = 0;
    int print_buffer = 0;
    int buffer_size = 200000;

    // Skip the header
    std::getline(myFileFilter, line);

    // Filter events
    while (std::getline(myFileFilter, line))
    {
        std::stringstream ss(line);
        ss >> ts;
        ss.get(); // skip space
        ss >> c;
        ss.get();
        ss >> r;
        ss.get();
        ss >> p;

        if (ts < process_ts_start)
        {
            continue;
        }
        else if (ts > process_ts_end)
        {
            break;
        }

        comb_filter::comb_filter::cell_comb_filter &ccf_xy =
            CF.filter_grid_.at(r).at(c);
        if (p == 1) // positive events
        {
            filtered_pos_e_count += 1;
            if ((c >= 0 && c < img_width && r >= 0 && r < img_height))
            {
                if (ccf_xy.x < 0)
                {
                    ccf_xy.x = c;
                    ccf_xy.y = r;
                }
                CF.update_q1(ts, 1, ccf_xy);
            }
        }
        else // negative events
        {
            filtered_neg_e_count += 1;
            if ((c >= 0 && c < img_width && r >= 0 && r < img_height))
            {
                if (ccf_xy.x < 0)
                {
                    ccf_xy.x = c;
                    ccf_xy.y = r;
                }
                CF.update_q1(ts, -1, ccf_xy);
            }
        }
        if (filtered_pos_e_count >= print_buffer)
        {
            std::cout << "Filtered positive event count: "
                      << filtered_pos_e_count << ", "
                      << "filtered negative event count: "
                      << filtered_neg_e_count << "\n";
            print_buffer += buffer_size;
        }
    }
    myFileFilter.close();
    return 0;
}

void load_bias(std::string bias_filename, comb_filter::comb_filter &CF)
{
    std::ifstream myFile(bias_filename);
    if (!myFile.is_open())
        throw std::runtime_error("Could not open the pre-calibrated bias file");
    std::string line, data_value;
    int r = 1, c = 1;
    while (std::getline(myFile, line))
    {
        std::stringstream ss(line);
        while (std::getline(ss, data_value, ' '))
        {
            comb_filter::comb_filter::cell_comb_filter &ccf_xy =
                CF.filter_grid_.at(r - 1).at(c - 1);
            ccf_xy.bias = std::stod(data_value);
            c++;
        }
        r++;
        c = 1;
    }
    myFile.close();
    std::cout << "Finish load event bias from: " << bias_filename << "\n";
}

void compute_bias(std::string bias_filename, std::string input_file_name, comb_filter::comb_filter &CF,
                  int img_height, int img_width)
{
    std::ifstream myFile(input_file_name);
    if (!myFile.is_open())
        throw std::runtime_error("Could not open the inpute event data file");
    std::ofstream myBiasFile(bias_filename);
    if (!myBiasFile.is_open())
        throw std::runtime_error("Could not open the bias file to write");
    std::string line;
    int ts, c, r, p;

    // Skip the header of the input file
    std::getline(myFile, line);
    std::cout << "Computing event bias...\n";
    while (std::getline(myFile, line))
    {
        std::stringstream ss(line);
        ss >> ts;
        ss.get(); // skip space
        ss >> c;
        ss.get();
        ss >> r;
        ss.get();
        ss >> p;
        comb_filter::comb_filter::cell_comb_filter &ccf_xy =
            CF.filter_grid_.at(r).at(c);
        if (p == 1) // positive events
        {
            ccf_xy.event_num += 1;
            ccf_xy.event_integ += 1;
        }
        else // negative events
        {
            ccf_xy.event_num += 1;
            ccf_xy.event_integ -= 1;
        }
    }
    myFile.close();
    for (int r = 0; r < img_height; r++)
    {
        for (int c = 0; c < img_width; c++)
        {
            comb_filter::comb_filter::cell_comb_filter &ccf_xy =
                CF.filter_grid_.at(r).at(c);
            // only compute bias when events number is higher than threshold
            if (ccf_xy.event_num < 20)
            {
                ccf_xy.bias = 0.0;
                myBiasFile << 0.0 << " ";
            }
            else
            {
                double bias_value = ccf_xy.event_integ / ccf_xy.event_num;
                ccf_xy.bias = bias_value;
                myBiasFile << bias_value << " ";
            }
        }
        myBiasFile << "\n";
    }
    myBiasFile.close();
    std::cout
        << "Finish computing event bias.\n";
}
