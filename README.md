# A Linear Comb Filter for Event Flicker Removal

<p align="center">
  <a href="https://youtu.be/1XgOknDIV00">
    <img src="figures/video_thumbnail.png" alt="A Linear Comb Filter for Event Flicker Removal" width="500"/>
  </a>
</p>

## For academic use only
Event cameras are bio-inspired sensors that capture per-pixel asynchronous intensity change rather than the synchronous absolute intensity frames captured by a classical camera sensor.
Such cameras are ideal for robotics applications since they have high temporal resolution, high dynamic range and low latency.
However, due to their high temporal resolution, event cameras are particularly sensitive to flicker such as from fluorescent or LED lights.
During every cycle from bright to dark, pixels that image a flickering light source generate many events that provide little or no useful information for a robot, swamping the useful data in the scene.
In this paper, we propose a novel linear filter to preprocess event data to remove unwanted flicker events from an event stream.
The proposed algorithm achieves over 4.6 times relative improvement in the signal-to-noise ratio when compared to the raw event stream due to the effective removal of flicker from fluorescent lighting.
Thus, it is ideally suited to robotics applications that operate in indoor settings or scenes illuminated by flickering light sources.


Ziwei Wang, Dingran Yuan, Yonhon Ng, and Robert Mahony

The paper was accepted by the International Conf. Robotics and Automation (ICRA 2022) in Philadelphia, USA.

[[Paper]](https://ieeexplore.ieee.org/document/9812003) [[arXiv preprint]](https://arxiv.org/abs/2205.08090) [[Datasets]](https://anu365-my.sharepoint.com/:f:/g/personal/u6456661_anu_edu_au/EtnKCU4J78hAhO_3uewHefkBObwmxZu9u3iq_eN4_eHz6w?e=hlj9rJ)

## Citation
If you use or discuss our algorithm or datasets, please cite our paper as follows:
<pre>
@InProceedings{wang22icra,
  author        = {Ziwei Wang and Dingran Yuan and Yonhon Ng and Robert Mahony},
  title         = {A Linear Comb Filter for Event Flicker Removal},
  booktitle     = {"International Conference on Robotics and Automation (ICRA)" },
  year          = {2022}
}
</pre>

## Prerequisites
### Download Data
Download the dataset, save to folder `./data`
- [Example Event Flicker Datasets](https://anu365-my.sharepoint.com/:f:/g/personal/u6456661_anu_edu_au/EtnKCU4J78hAhO_3uewHefkBObwmxZu9u3iq_eN4_eHz6w?e=hlj9rJ).

Dataset overview: pure-event video reconstruction from the debiased data:



https://user-images.githubusercontent.com/56065023/209065202-5ecbbba4-90ca-4982-8199-deb326d64e78.mp4




### Installation
Dependencies:
- [OpenCV](https://opencv.org/)
- [Pandas](https://pandas.pydata.org/)

### Build
Our asynchronous comb filter is designed to be built as a cmake project. Assuming all prerequisites are installed and you are in the root folder of the repository, then you can follow these steps to build:
```
mkdir build
cd build
cmake -DCMAKE_BUILD_TYPE=Release ..
cmake --build . -j8
```
### Run 
```
./event_camera_comb_filter
```
The filtered event data will be saved in `data/{data_id}/events_filter.txt`.
Choose event data to filter by changing `{data_id}` in the configure file `EFR_config.ymal`.


### Sort and Zip Events by Timestamps
Our asynchronous comb filter process events occur at different pixels independently and asynchronously. For each pixel, event timestamps are in ascending order, but the timestamp is non-causal among pixels.
Because most of the event camera reconstruction algorithms require event data saved in ascending time order among all pixels, you can sort event data using: 
```
python ../sort_zip_event.py
```
The sorted and event data will be saved under `data/{data_id}/`, named `events_filter_sort.cvs` and `events_filter_sort.zip`.
The zipped data is used for E2VID evaluation. 


### Evaluate with E2VID
E2VID is a pure event reconstruction algorithm from paper High Speed and High Dynamic Range Video with an Event Camera [[Paper]](https://rpg.ifi.uzh.ch/docs/TPAMI19_Rebecq.pdf) by Henri Rebecq, Rene Ranftl, Vladlen Koltun and Davide Scaramuzza, CVPR 2019.
You can compare the event reconstruction performance using unfiltered and filtered event data.

Install E2VID following the instructions from its project page: [https://github.com/uzh-rpg/rpg_e2vid](https://github.com/uzh-rpg/rpg_e2vid). E2VID requires input event data in zip format. 
Replace {data_name} to `events_raw`, `events_raw_no_bias`, `events_filter_sort` for raw data reconstruction, de-biased raw data reconstruction and comb filtered data reconstruction, respectively:

```
python run_reconstruction.py  \
-c pretrained/E2VID_lightweight.pth.tar \
-i /{YourFolderPath}/data/{data_id}/{data_name}.zip  \
--auto_hdr \
--output_folder  /{your_folder_path}/CombFilter/data/{data_id}/ \
-T 7000000 \
--fixed_duration \
--dataset_name {your_data_save_name}
```


## Code Implementation Visualization
TODO: add video

## Notes
Should you have any questions or suggestions, please don't hesitate to get in touch with ziwei.wang1@anu.edu.au

